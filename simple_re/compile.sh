#!/bin/sh

gcc -c simple_re.c -o simple_re.o -I./ -I./onig/include -L./onig/lib -lonigmo

gcc test.c simple_re.o -o test_run  -I./ -I./onig/include -L./onig/lib -lonigmo

