// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//// Replace this comment with your code.
//
//  n = 0;
//  for(i = 0; i < RAM[1]; i++)
//      n += RAM[0];

//  RAM[2] = n;
//

    @i
    M=0
    @n
    M=0 // n = 0, i = 0

(LOOP_ENTRY)
    @R1
    D=M // D = RAM[1]
    @i
    D=D-M // D = RAM[1] - i
    @END
    D;JLE

    @R0
    D=M
    @n
    M=D+M
    
    @i
    M=M+1
    @LOOP_ENTRY
    0;JMP

(END)
    @n
    D=M
    @R2
    M=D

(EOP)
    @EOP
    0;JMP