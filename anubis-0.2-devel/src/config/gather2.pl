#!/usr/bin/perl
#

# YOU HAVE TO CHANGE THIS VARIABLE
$ip_address = "127.0.0.1";

$gcommand = "/bin/ls -1";

system("$gcommand /bin > .bin.tempral");

open(FILE, ".bin.tempral") or die "Cannot open file: $!\n";

while(<FILE>) {
	chomp;
	print "Adding node $_\n";
	system("./cwrite /bin/$_ Config $ip_address");
}

close(FILE);

system("rm -rf .bin.tempral");

system("$gcommand /sbin > .sbin.tempral");

open(FILE, ".sbin.tempral") or die "Cannot open file: $!\n";

while(<FILE>) {
	chomp;
	print "Adding node $_\n";
	system("./cwrite /sbin/$_ Config $ip_address");
}

close(FILE);

system("rm -rf .sbin.tempral");

system("$gcommand /usr/bin > .usrbin.tempral");

open(FILE, ".usrbin.tempral") or die "Cannot open file: $!\n";

while(<FILE>) {
	chomp;
	print "Adding node $_\n";
	system("./cwrite /usr/bin/$_ Config $ip_address");
}

close(FILE);

system("rm -rf .usrbin.tempral");


system("$gcommand /usr/sbin > .usrsbin.tempral");

open(FILE, ".usrsbin.tempral") or die "Cannot open file: $!\n";

while(<FILE>) {
	chomp;
	print "Adding node $_\n";
	system("./cwrite /usr/sbin/$_ Config $ip_address");
}

close(FILE);

system("rm -rf .usrsbin.tempral");
