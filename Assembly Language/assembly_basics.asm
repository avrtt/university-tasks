.386
.MODEL FLAT, STDCALL
.STACK 4096
EXTERN ExitProcess@4: PROC
EXTERN GetStdHandle@4: PROC
EXTERN ReadConsoleA@20: PROC
EXTERN WriteConsoleA@20: PROC
 
.DATA
    BUFFER  DB      20 dup (?)
    LEN     DWORD   ?
    SUM     DD      0
    STRING  DB      20 dup (?)
 
.CODE
READC PROC
    PUSH    -10
    CALL    GetStdHandle@4
 
    PUSH    0
    PUSH    OFFSET LEN
    PUSH    20
    PUSH    OFFSET BUFFER
    PUSH    EAX
    CALL    ReadConsoleA@20
 
    SUB     LEN, 3
RET
READC ENDP
 
WRITEC PROC
    PUSH    -11
    CALL    GetStdHandle@4
    PUSH    0
    PUSH    OFFSET LEN
    PUSH    20
    PUSH    OFFSET STRING
    PUSH    EAX
    CALL    WriteConsoleA@20
RET
WRITEC ENDP
 
POW PROC
    XOR     EBX, EBX
    MOV     BL, 16
MAINLOOP:
    CMP     LEN, 1
    JLE     EX
    DEC     LEN
    MUL     EBX
    JMP     MAINLOOP
EX:
    ADD     SUM, EAX
    XOR     EBX, EBX
    XOR     EAX, EAX
RET
POW ENDP
 
CHARTONUM PROC
    CMP     LEN, 0
    JL      ERR
    MOV     ECX, LEN
 
MAINLOOP:
    CMP     ECX, 0
    JL      EX
    MOV     AL, BUFFER[ECX]
    DEC     ECX
    CMP     AX, 2Dh
    JE      MINUS
    CMP     AX, 39h
    JLE     NUMBER
    CMP     AX, 46h
    JLE     CHAR
    JMP     ERR
 
MINUS:
    CMP     LEN, 1
    JL      ERR
    CMP     ECX, 0
    JGE     ERR
    NEG     SUM
    JMP     MAINLOOP
 
NUMBER:
    CMP     AX, 2Fh
    JLE     ERR
    SUB     AX, 30h
    PUSH    LEN
    SUB     LEN, ECX
    CALL    POW
    POP     LEN
    JMP     MAINLOOP
 
CHAR:
    CMP     AX, 40h
    JLE     ERR
    SUB     AX, 37h
    PUSH    LEN
    SUB     LEN, ECX
    CALL    POW
    POP     LEN
    JMP     MAINLOOP
 
ERR:
    PUSH    -1
    CALL    ExitProcess@4
 
EX:
    XOR     EAX, EAX
    XOR     ECX, ECX
RET
CHARTONUM ENDP
 
NUMTOCHAR PROC
    MOV     EAX, SUM
    MOV     BX, 10
    TEST    EAX, EAX
    JNS     MAINLOOP
    NEG     EAX
    INC     ECX
 
MAINLOOP:
    XOR     EDX, EDX
    DIV     EBX
    ADD     DL, 30h
    MOV     STRING[ECX], DL
    INC     ECX
    CMP     EAX, 0
    JG      MAINLOOP
 
    DEC	  ECX
    MOV     EBX, SUM
    TEST    EBX, EBX
    JNS     REVERSELOOP
	    INC	  ECX
    MOV     STRING[ECX], 2Dh
 
REVERSELOOP:
    MOV BL, STRING[ECX]
    MOV BH, STRING[EAX]
    MOV STRING[ECX], BH
    MOV STRING[EAX], BL
    INC EAX
    DEC ECX
    CMP ECX, EAX
    JG REVERSELOOP
 
EX:
    XOR     EAX, EAX
    XOR     EBX, EBX
    XOR     ECX, ECX
 
RET
NUMTOCHAR ENDP
 
MAIN PROC
    CALL    READC
    CALL    CHARTONUM
    PUSH    SUM
    MOV     SUM, 0
    CALL    READC
    CALL    CHARTONUM
    POP     EAX
    ADD     SUM, EAX
    CALL    NUMTOCHAR
    CALL    WRITEC
 
    PUSH    0
    CALL    ExitProcess@4
 
MAIN ENDP
END MAIN

