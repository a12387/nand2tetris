// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.
(INFLOOP)
    @KBD
    D=M
    @SETWHITE
    D;JEQ
    @SETBLACK
    0;JMP

(SETWHITE)
    @i
    M=0
(LOOPWHITE)    
    @8192
    D=A
    @i
    D=D-M // 8192 - i
    @ENDWHITE
    D;JLE

    @SCREEN
    D=A
    @i
    A=D+M // A = SCREEN + i
    M=0

    @i
    M=M+1
    @LOOPWHITE
    0;JMP
(ENDWHITE)
    @INFLOOP
    0;JMP

(SETBLACK)
    @i
    M=0
(LOOPBLACK)
    @8192
    D=A
    @i
    D=D-M // 8192 - i
    @ENDBLACK
    D;JLE

    @SCREEN
    D=A
    @i
    A=D+M // A = SCREEN + i
    M=-1

    @i
    M=M+1
    @LOOPBLACK
    0;JMP
(ENDBLACK)
    @INFLOOP
    0;JMP