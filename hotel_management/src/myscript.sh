#!/bin/bash 
# Author : all tem members 
# pls run this shell script so you don't have to open and write to much things in terminal, run, seat and enjoy the code :)
gcc  alarm.c -o alarm.out 
make -f makefile clean 
rm log.txt
make -f makefile all 
echo "     "
echo "     "
echo "Displaying log file... "
echo "     "
echo "     "
cat log.txt 



