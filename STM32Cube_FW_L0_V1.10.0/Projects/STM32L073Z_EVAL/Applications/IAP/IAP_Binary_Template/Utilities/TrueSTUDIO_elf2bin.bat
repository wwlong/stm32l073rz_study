@echo off
echo ELF to BIN file generation ...

"C:\Program Files (x86)\Atollic\TrueSTUDIO for ARM Pro 5.1.1\ARMTools\bin\arm-atollic-eabi-objcopy.exe" -O binary ..\TrueStudio\STM32L073Z_EVAL\Debug\STM32L073Z_EVAL.elf ..\TrueStudio\STM32L073Z_EVAL\STM32L073Z_EVAL_SysTick.bin

echo ELF to BIN file generation completed.

pause