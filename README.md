# 🎯 Project Objective
To design and implement a microprocessor-based system using the Intel 8086 and 8255 PPI to interface input (push buttons) and output (7-segment display) devices. The dual mode system dynamically switches between real-time data mapping and a "freeze" state using software logic based on a selection switch.

## 🛠️ Hardware Interfacing & Components
The circuit uses specific addressing logic to manage I/O ports:

* Microprocessor: Intel 8086.
* PPI (Programmable Peripheral Interface): Two 8255A chips.

  * PPI #1: Port A handles the 7-segment output; Port B handles the 8 active-low push buttons.
  * PPI #2: Port A monitors the SPDT mode-selection switch.
* Support ICs: 74HC373 Address Latch and 74LS138 Decoder for precise chip selection.

## 🧠 Operational Logic (Assembly)
The system operates in a continuous loop, checking the mode selection switch at the start of every cycle :

1. Mode 0 (Real-Time Mapping): The system polls Port B for button presses (S0-S7).

    * If a button is detected (Active-Low), the corresponding hex code for digits 0-7 is loaded and sent to the display.

2. Mode 1 (Display Freeze): The microprocessor stops reading the push buttons.

    * It refreshes the display using the last known value stored in the BH register, keeping the display static even if buttons are pressed.

## 💻 Source Code Snippet
The project utilizes EQU directives for port mapping to ensure clean, readable code:

```
; Port Mapping & EQU Directives
PORTA1   EQU 00H     ; 8255#1 Port A (7-segment output)
PORTB1   EQU 02H     ; 8255#1 Port B (button input)
CTRL1    EQU 06H     ; 8255#1 Control Port

PORTA2   EQU 08H     ; 8255#2 Port A (mode input (PA0))
PORTB2   EQU 0AH     ; 8255#2 Port B (unused)
CTRL2    EQU 0EH     ; 8255#2 Control Port

; 8255 Configuration
MODE0_CFG EQU 10000010B  ; 8255#1: PA=Out, PB=In, PC=Out (Mode 0)
MODE2_CFG EQU 10010000B  ; 8255#2: PA=In, PB=Out, PC=Out (Mode 0)
```

## 🖼️ Circuit & Simulation
Below is the hardware interfacing schematic designed in Proteus. It highlights the address decoding logic using the 74LS138 decoder and the data latching via 74HC373.

<img width="645" height="499" alt="image" src="https://github.com/user-attachments/assets/e5e1355e-5f64-4762-aa21-8b5820a76780" />

<img width="644" height="499" alt="image" src="https://github.com/user-attachments/assets/029c8bc5-b4e7-43eb-bfc8-53fa46f0303e" />

## ✅ Conclusion
This implementation demonstrates how external hardware behavior can be managed entirely through software logic without physical rewiring. It highlights proficiency in Assembly language, I/O port addressing, and digital logic design .
