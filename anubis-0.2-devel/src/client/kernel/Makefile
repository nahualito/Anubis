#!/bin/bash
#
#

if [ $1 == "" ]; then
exit 1
else
gcc -I/usr/src/linux/include -D__KERNEL__ -Wall -Werror -Wstrict-prototypes -Wno-trigraphs -O2 -fomit-frame-pointer -fno-strict-aliasing -fno-common -pipe -mpreferred-stack-boundary=2 -march=i686 -DMODULE -DDEBUG -c $1
fi
