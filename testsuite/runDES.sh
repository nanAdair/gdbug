#! /bin/bash
gcc -c des.c run_des.c

ld -static -r /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/gcc/i686-pc-linux-gnu/5.2.0/crtbegin.o -L/usr/lib/gcc/i686-pc-linux-gnu/5.2.0 -L/usr/lib -L/lib des.o run_des.o -lgcc -lgcc_eh -lc_nonshared /usr/lib/gcc/i686-pc-linux-gnu/5.2.0/crtend.o /usr/lib/crtn.o -o obj.o   

cd ../scarab/ && make
cd ../testsuite/
cp ../scarab/main .

./main obj.o /usr/lib/libc.so.6 /usr/lib/ld-linux.so.2
