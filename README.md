# CHIP-8 Interpreter

I used Tvil's guide [1] to get familiar with its specifications and instruction descriptions. I also liked LINGIB's architecture diagram [2] and opcode instructions [3].

<img src="assets/chip-8 computer.png" alt="CHIP-8 Computer Architecture Diagram" width="500">

To run:

```
sudo apt install g++ libsdl2-dev
make
./chip8 <ROM_FILE>
```

Download IBM Logo program [here](https://github.com/loktar00/chip8/blob/master/roms/IBM%20Logo.ch8) to quickly test it. 

I made a mistake when designating each opcode with a separate function (tolerable for 35 opcode though), but it's not a scalable approach. Don't be me.

## References
* [1] Tobias V. Langhoff, ["How to write a CHIP-8 emulator"](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
* [2] LINGIB, ["CHIP-8 Computer" Architecture Diagram](https://www.instructables.com/CHIP-8-Computer/)
* [3] LINGIB [CHIP-8 Instruction Set](https://content.instructables.com/FWH/119Y/KFQTWS0I/FWH119YKFQTWS0I.pdf)