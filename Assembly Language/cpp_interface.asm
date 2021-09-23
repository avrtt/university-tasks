.386
.MODEL FLAT
.STACK 4096
 
.DATA
 
.CODE
 
PUBLIC _FormatString
 
_FormatString PROC
 
    PUSH    EBP
    MOV     EBP, ESP
 
    MOV     EAX, [EBP] + 8
    MOV     ECX, [EBP] + 12
    MOV     LEN, ECX
 
MAINLOOP:
    DEC     ECX
    MOV     DL, BYTE PTR [EAX + ECX]
    CMP     DL, 2Eh; 2Eh - '.'
    JE      REPLACE
 
    CMP     ECX, 0
    JE      EX
JMP MAINLOOP
 
REPLACE:
    PUSH    ECX
 
SHIFT:
    INC     ECX
    MOV     DL, BYTE PTR [EAX + ECX]
    MOV     BYTE PTR [EAX + ECX - 1], DL
    CMP     DL, 0h
    JNE     SHIFT
    
    POP     ECX
    CMP     ECX, 0
    JE      EX
    JMP     MAINLOOP
 
EX:
 
    POP     EBP 
RET
_FormatString ENDP
 
END
