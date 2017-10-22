${DEVKITARM}/bin/arm-none-eabi-gcc -O3 -o hockey2.elf -T core/gba_cart.ld core/*.s core/*.c game/*.c media/map/*.c media/gfx/*.c
${DEVKITARM}/bin/arm-none-eabi-objcopy -O binary hockey2.elf hockey2.gba

cp ./hockey2.gba ../PopUnityBoy/Assets/hockey2.gba.bytes

