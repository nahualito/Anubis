/*
 * Anubis.c
 *
 * Kernel module for Anubis, this will hook into execve() and send 
 * the file to be validated to the userland daemon
 *
 * By default it will let everything execute unless the device has been
 * opened in wich for us means you can activate it by loading the userland 
 * process and deactivate it by killing the userland daemon (some failure but
 * lets us be clean on this stage)
 *
 * Enrique A. Sanchez Montellano
 * nahual@g-con.org
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/socket.h>
#include <linux/smp_lock.h>
#include <linux/stat.h>
#include <linux/dirent.h>
#include <linux/fs.h>
#include <linux/if.h>
#include <linux/modversions.h>
#include <linux/slab.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/skbuff.h>
#include <sys/syscall.h>
#include <linux/dirent.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/errno.h>
#include <asm/unistd.h>
#include <linux/signal.h>

int Major = 0;
int Minor = 0;

struct file_operations filer;
static char the_filename[1024] = "/usr/local/bin/anubis";
static char result[12] = "DENIED";

extern void *sys_call_table[];

int _we_are_enabled = 0; //Not enabled by default
int _anubis_pid = -1;
int _we_got_answer = 0; //Ugly hack

//wait_queue_head_t _anubisq;
DECLARE_WAIT_QUEUE_HEAD(_anubisq);

int (*orig_execve)(struct pt_regs regs);

int anubis_open(struct inode *inode, struct file *filp) {
  printk("Process (%d) opened the Anubis device ... Securing\n", current->pid);
  _anubis_pid = current->pid;
  _we_are_enabled = 1;
  return 0;
}

int anubis_close(struct inode *inode, struct file *filp) {
  printk("The device has been closed disabling security\n");
  _we_are_enabled = 0;
  return 0;
}

ssize_t anubis_read(struct file *flip, char *buff, size_t count, loff_t *offp) {
  int len;

  if(!strlen(the_filename))
    return 0;

  len = sprintf(buff, "%s", the_filename);

  return len;
}

ssize_t anubis_write(struct file *flip, const char *buff, size_t count, loff_t *offp) {
  int len;
  static char the_filename_tmp[1024];
  
  if((len = copy_from_user(the_filename_tmp, buff, sizeof(the_filename_tmp)))) {
    printk("Copy from user failed\n");
    return -EFAULT;
  }
  
  len = snprintf(result, sizeof(result), "%s", the_filename_tmp);
  _we_got_answer = len;
  wake_up(&_anubisq);
  return len;
}

asmlinkage int hacked_execve(struct pt_regs regs) {
  int error, i;
  char *filename;

  lock_kernel();
  filename = getname((char *)regs.ebx);
  error = PTR_ERR(filename);
  if(IS_ERR(filename))
    goto out;

  for(i = 0; i < sizeof(the_filename); i++) 
    the_filename[i] = '\0';

  for(i = 0; i < sizeof(result); i++)
    result[i] = '\0';

  snprintf(the_filename, sizeof(the_filename), "%s", filename);

#ifdef DEBUG
  printk("The binary which was tried to execute is %s\n", the_filename);
#endif

  if(_we_are_enabled == 1) {
    kill_proc(_anubis_pid, SIGUSR1, 1);

    wait_event(_anubisq, (_we_got_answer > 0));

    if(result[0] == '\0') {
      printk("whooops .. Something terribly wrong happened to Anubis! DENYing evverything by default untill restarted\n");
      error = -EPERM;
      goto out;
    }

    if(!strncmp(result, "DENIED", 6)) {
      error = -EPERM;
      _we_got_answer = 0;
      goto out;
    }
    _we_got_answer = 0;
  }

  error = do_execve(filename, (char **)regs.ecx,(char **)regs.edx, &regs);

  if(error == 0)
    current->flags &= ~PT_DTRACE;

  putname(filename);

 out:
  unlock_kernel();
  return error;
}


int init_module(void) {
  int err;
  
  printk("Registering he Anubis Device\n");
  err = register_chrdev(Major, "anubis", &filer);
  
  filer.owner = THIS_MODULE;
  filer.open = anubis_open;
  filer.release = anubis_close;
  filer.read = anubis_read;
  filer.write = anubis_write;
  
  if(err < 0) {
    printk("OOoOoOPpPpspSsS Something wen terribly wrong, recompile or send dump and bugs\n");
    return err;
  }
  
  if(Major == 0) Major = err;
  
  printk("Anubis device has major %d\n", Major);
  orig_execve = sys_call_table[SYS_execve];
  sys_call_table[SYS_execve] = hacked_execve;
  
  return 0;
}

void cleanup_module(void) {
  printk("Unregistering Anubis device with major %d\n", Major);
  unregister_chrdev(Major, "anubis");
  sys_call_table[SYS_execve] = orig_execve;
}
