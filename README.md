# Orion_STM32_FW
To use the firmware with the external flash memory, you will have to copy the external loader file .stldr into 
```
\tools\bin\ExternalLoader
```
Then, run the Orion_Boot project. 
After this, you can run the Orion_STM32_FW or the Orion_Testbench for a small demo.
The code will start at address 0x080000000, so you will need to reset the chip (press the reset button) every time you start a debug session to jump at address 0x90000000.
