#! /bin/bash
gcc -c des.c run_des.c

ld -static -r /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/gcc/i686-pc-linux-gnu/4.9.3/crtbegin.o -L/usr/lib/gcc/i686-pc-linux-gnu/4.9.3 -L/usr/lib -L/lib run_des.o -lgcc -lgcc_eh -lc_nonshared /usr/lib/gcc/i686-pc-linux-gnu/4.9.3/crtend.o /usr/lib/crtn.o -o obj.o

cd ../scarab/ && make
cd ../testsuite/
cp ../scarab/main .

./main obj.o /lib/libc.so.6 /lib/ld-linux.so.2
