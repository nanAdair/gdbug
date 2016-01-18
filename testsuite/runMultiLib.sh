#! /bin/bash
gcc -c testMultiLib.c

ld -static -r /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/gcc/i686-pc-linux-gnu/5.2.0/crtbegin.o -L/usr/lib/gcc/i686-pc-linux-gnu/5.2.0 -L/usr/lib -L/lib testMultiLib.o -lgcc -lgcc_eh -lc_nonshared /usr/lib/gcc/i686-pc-linux-gnu/5.2.0/crtend.o /usr/lib/crtn.o -o obj.o   

cd ../scarab/ && touch a.o && make
cd ../testsuite
cp ../scarab/main .

./main obj.o /usr/lib/libc.so.6 /usr/lib/libm.so.6 /usr/lib/ld-linux.so.2
