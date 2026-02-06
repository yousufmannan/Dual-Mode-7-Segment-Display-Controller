; 8255A, 74LS138 and 74HC373 Configuration:

; - 74HC373 Q1 -> A0 of both 8255s, Q2 -> A1 of both 8255s
; - 74LS138 Y -> Chip Select (Y0=8255#1, Y1=8255#2)
; - 8255#1: PA=7-segment display, PB=buttons, PC=unused
; - 8255#2: PA=mode input (PA0), PB=unused, PC=unused

PORTA1   EQU 00H     ; 8255#1 Port A (7-segment output)
PORTB1   EQU 02H     ; 8255#1 Port B (button input)
CTRL1    EQU 06H     ; 8255#1 Control Port

PORTA2   EQU 08H     ; 8255#2 Port A (mode input (PA0))
PORTB2   EQU 0AH     ; 8255#2 Port B (unused)
CTRL2    EQU 0EH     ; 8255#2 Control Port

MODE0_CFG EQU 10000010B  ; 8255#1: PA=Out, PB=In, PC=Out (Mode 0)
MODE2_CFG EQU 10010000B  ; 8255#2: PA=In, PB=Out, PC=Out (Mode 0)

CODE SEGMENT
    ASSUME CS:CODE
    ORG 100H

START:
    ; Initialize 8255#1 (Mode 0)
    MOV DX, CTRL1
    MOV AL, MODE0_CFG
    OUT DX, AL

    ; Initialize 8255#2 (PA=In)
    MOV DX, CTRL2
    MOV AL, MODE2_CFG
    OUT DX, AL

    ; Initialize display to "0"
    MOV AL, 11000000B   ; 7-segment code for "0"
    MOV DX, PORTA1
    OUT DX, AL
    MOV BH, AL           ; BH stores current display value

    MOV BL, 0            ; BL=0 (Mode 0), BL=1 (Mode 1)

MAIN_LOOP:
    ; Read mode switch (PA0 of 8255#2)
    MOV DX, PORTA2
    IN AL, DX
    AND AL, 01H          ; Check PA0 (Mode 0: PA0=0, Mode 1: PA0=1)
    CMP AL, 0
    JZ SET_MODE0         ; Jump to Mode 0 if PA0=0

    ; --- Mode 1: Freeze Display ---
    CMP BL, 1
    JZ SKIP_MODE1        ; Already in Mode 1

    ; Enter Mode 1
    MOV BL, 1
    ; Refresh display with last value
    MOV DX, PORTA1
    MOV AL, BH
    OUT DX, AL

SKIP_MODE1:
    JMP MAIN_LOOP        ; Ignore buttons in Mode 1

SET_MODE0:
    ; --- Mode 0: Active Input ---
    CMP BL, 0
    JZ CHECK_BUTTONS     ; Already in Mode 0

    ; Enter Mode 0
    MOV BL, 0
    ; Refresh display with last value
    MOV DX, PORTA1
    MOV AL, BH
    OUT DX, AL

CHECK_BUTTONS:
    ; Read buttons (8255#1 Port B)
    MOV DX, PORTB1
    IN AL, DX

    ; Check button presses (active-low)
    CMP AL, 11111110B   ; S0 pressed?
    JZ S0
    
    CMP AL, 11111101B   ; S1 pressed?
    JZ S1
    
    CMP AL, 11111011B   ; S2 pressed?
    JZ S2
    
    CMP AL, 11110111B   ; S3 pressed?
    JZ S3
    
    CMP AL, 11101111B   ; S4 pressed?
    JZ S4
    
    CMP AL, 11011111B   ; S5 pressed?
    JZ S5
    
    CMP AL, 10111111B   ; S6 pressed?
    JZ S6
    
    CMP AL, 01111111B   ; S7 pressed?
    JZ S7
    JMP MAIN_LOOP        ; No valid button press

UPDATE_DISPLAY:
    MOV BH, AL           ; Store new value
    MOV DX, PORTA1
    OUT DX, AL           ; Update 7-segment display
    JMP MAIN_LOOP

; 7-segment codes (Common anode: 0=segment on)
S0: MOV AL, 11000000B    ; 0
    JMP UPDATE_DISPLAY
    
S1: MOV AL, 11111001B    ; 1
    JMP UPDATE_DISPLAY
    
S2: MOV AL, 10100100B    ; 2
    JMP UPDATE_DISPLAY
    
S3: MOV AL, 10110000B    ; 3
    JMP UPDATE_DISPLAY
    
S4: MOV AL, 10011001B    ; 4
    JMP UPDATE_DISPLAY
    
S5: MOV AL, 10010010B    ; 5
    JMP UPDATE_DISPLAY
    
S6: MOV AL, 10000010B    ; 6
    JMP UPDATE_DISPLAY
    
S7: MOV AL, 11111000B    ; 7
    JMP UPDATE_DISPLAY

CODE ENDS
END START