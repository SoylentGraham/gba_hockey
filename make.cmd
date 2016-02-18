echo off
d:
cd \
cd "code hockey"
del hockey.gba
del hockey.elf
echo compiling...
PATH=D:\gbadev\devkitadv\devkitadv\bin;%PATH%;
gcc -O3 -o hockey.elf core/*.c game/*.c media/map/*.c media/gfx/*.c
objcopy -O binary hockey.elf hockey.gba
