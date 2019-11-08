sdcc --model-small -c main.c
sdcc --model-small --code-loc 0x0000 --data-loc 0x08 --xram-loc 0x0000 main.rel
s51 -V -t 8052 -s/dev/pts/1 main.ihx
