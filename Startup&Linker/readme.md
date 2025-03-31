## What is MicroController?
Microcontroller (MCU, uC) is a small computer system on a single chip. But its resource and capabilities such as memory, speed, external interfaces are very much limited than of a deskstop computer because MCH target embbeded application

**Code memory**:
* non-valatile, and it is used to store program instructions
* The purpose of the code (Program) memory is to store instructions and constant data of your program
* There are differrent types of code memory
  * ROM (read only memory)
  * OTP (On time programmable)
  * FLASH
* System memory: This is ROM, and here ST factory built bootloader is stored, which never executes unless we invoke it by something.

**Data memory**
* Volatile and it is used to store data of your program and also used as a sratch pad
* Usually, the data memory of the MCU is consumed during the run-time of the program.




## Analyzing ELF files

```
arm-none-eabi-objdump.exe -h
```
```arduino
firmware.elf:     file format elf32-littlearm

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .isr_vector   00000100  08000000  08000000  00010000  2**0
  1 .text         00005678  08000100  08000100  00010100  2**2
  2 .rodata       00001234  08005778  08005778  00015778  2**2
  3 .data         00000456  20000000  080069ac  000169ac  2**2
  4 .bss          00000800  20000456  20000456  00000000  2**2
  5 .heap         00001000  20000c56  20000c56  00000000  2**2
  6 .stack        00001000  20001c56  20001c56  00000000  2**2
```

ELF file has many section:

`.text` section of the elf file contains 'code' or 'instruction' of your program

`.rodata` section contains all the constant (read only) data 

`.data` section contains all the initialized data 

`.bss` section contains all uninitialized data

`LMA` is Load Memory Address (source in Flash)

`VMA` is Virtual Memory Address (destination in SRAM)

So when your IDE loads the .elf file into Flash memory apart from .text, .data, .rodata, .bss section are also stored in FLASH

Basically, LMA says "*Where the section is currently loaded*" abd VMA says "*Where the section should finally be copied to*"


## Create Startup File
- Creating a vector table
  - Create an array to hold MSP and handlers address
```c
unint32_t vectors[] = {/*store MSP and address of array handler8*/}
```

- Instruct the compile not to include the above array in `.data` section but in a different user defined section

**Function attribute: WEAK and ALIAS**

`WEAK` let programmer to override already defined weak function (dummy) with the same function name

`ALIAS` let programmer to give naem for a function

```c
__attribute__(weak, (alias("Default_Handler")))
```

Now we are working with `WEAK` this is because, the programmer can implement real implementation of handling that exception


**Importance of startup file**
- The start-up file is responsible for setting up the right environment for the main user code to run
- Code written in startup file runs before main(). So you can say startup file call main()
- some part of the startup code file is the target (processor) dependent
- Startup code take care of vector table placement in code memory as required by the ARM Cortex M
- Startup code may also take care of stack re-initialization
- Startup code is responsible of `.data, .bss,` section initialization in main memory


## Linker and Locator
- Use the linker to merge similar sections of different object files and use linker scripts to assign suitable code memory address and data memory address to different section

- Locator (part of linker) takes the help of a linker scripts to understand how you wish to merge different section and assigns mentioned addresses to different sections

## Linker Scripts
- Linker script i a text file which explains how different sections of the object files should be merged to create an output file
- Linker and locator combination assigns unique absolute address to different sections of the output file by referring to address information mentioned in the linker script
- Linker script also includes the code and data memory address and size information
- Linker scripts are written using GNU linker command language.
- GNU linker scripts has the file extension of .ld
- You must supply linker script at the linking phase to linker using -T option

### Linker script command

#### 1. Entry Command
- This command is used to set the "Entry point address" information in the header or final .elf file generated
- In our case, `Reset_Handler` is the entry point into the application. The first piece of code that executes right after the processor reset
- The debugger used this information to locate the first function to execute
- Not a mandatory command to use, but required when you debug the .elf file using debugger (GDB)

#### 2. Memory Command
- This command allow you to describe the different memories present in the target and their start address and size information
- The linker use information mentioned in this command to assign address to merge sections
- The information is given under this command also helps the linker to calculate total code and data memory consumed so far and throw an error message if data, code, heap, or stack areas cannot fit into variable size.
- By using memory command, you can fine-tune various memories available in your target and allow different section to occupy different memory areas
- Typically one linker script has one memory command

EXAMPLE:
```C
MEMORY
{
    name(['ALIRWX']): ORIGIN = origin, LENGTH = len
}
```

#### 3. Section Command
- Section command is used to create different output sections in the final .elf file executable generated
- Important command by which you can instruct the linker how to merge the input sections to yield an output section
- This command also controls the order in which different output sections appear in the .elf file generated
- By using this command, you also mention the placement of a section in a memory region. For example, your instruct the linker to place the .text section in the flash memory region, which is describe by the memory command

```c
SECTIONS
{
    .text:
    {
        /*merge all .isr_vector section of all input file*/
        /*merge all .text section*/
        /*merge all .rodata section*/
    }>(VMA) AT>(LMA)

    .data:
    {

    }>(VMA) AT>(LMA)
}
```

#### 4. Location counter (.)
- This is a special linker denoted by a dot '.'
- This sumbol is called `Location counter` ssince linker auto update this symbol with location (address) information
- You can use this symbol inside the linker script to track and define boundaries of various sections
- You can also set location counter to any specific value while writing linker script
- Location counter should appear only inside the sections commands
- Thee location counter is incremented by the side of the output section

#### 5. Linker Script symbol
- A symbol is the name of an address
- A symbol declaration is not equivalent to a variable declaration what you do in your C 
 











