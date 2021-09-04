/*
 * File:   _7Module.c
 * Author: hassan Elsaied
 *
 * Created on 03 9, 2021, 10:43 ?
 */


#include <xc.h>
#include "module.h"
#include "stdbool.h"
#define F_CPU 1000000  
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
/**
 * G    F   E   D   C    B    A      HEX       Digit
 * 0    1   1   1   1    1    1      0x3F       0
 * 0    0   0   0   1    1    0      0x03       1
 * 1    0   1   1   0    1    1      0x5B       2
 * 1    0   0   1   1    1    1      0x4F       3
 * 1    1   0   0   1    1    0      0x66       4
 * 1    1   0   1   1    0    1      0x6D       5
 * 1    1   1   1   1    0    1      0x7D       6
 * 0    0   0   0   1    1    1      0x07       7
 * 1    1   1   1   1    1    1      0x7F       8
 * 1    1   0   1   1    1    1      0x6F       9
 *  
 
 */
char DigitData [10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
/**
 * G    F   E   D   C    B    A      HEX       Digit
 * 0    0   0   0   0    0    1      0x01       LED A
 * 0    0   0   0   0    1    0      0x02       LED B
 * 0    0   0   0   1    0    0      0x04       LED C
 * 0    0   0   1   0    0    0      0x08       LED D
 * 0    0   1   0   0    0    0      0x10       LED E
 * 0    1   0   0   0    0    0      0x20       LED F
 * 1    0   0   0   0    0    0      0x40       LED G
 
 *  
 */
char LEDData [7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

void InitModule() {
    MODbits.MOD = 0x00;
}

_7MODULE_State IDELS() {
    PORTC = 0x3F;
    MODbits.MOD = 0x00;
    return IDLE;
}

bool StartCount() {
    char Input = MODbits.MOD & 0x01;
    if (Input == 0x01)
        return true;
    else
        return false;
}

bool StartSetData() {
    char Input = MODbits.MOD & 0x04;
    if (Input == 0x04)
        return true;
    else
        return false;
}

bool StartLEDTEST() {
    char Input = MODbits.MOD & 0x02;
    if (Input == 0x02)
        return true;
    else
        return false;
}

_7MODULE_State Counting(char Digit) {
    PORTD &= ~0x04; // clear bit 3
    PORTD |= 0x01; //set bit 1
    PORTD &= ~0x02; //clear bit 2
    Counter(Digit);
    OUTbits.OUT = 0x01; // displayed is Counted 
    return Count;
}

_7MODULE_State LEDTested(char Digit) {
    PORTD &= ~0x04;
    PORTD &= ~0x01;
    PORTD |= 0x02;
    counterLED(Digit);
    OUTbits.OUT = 0x02; //LED Tested
    return LEDTest;
}

_7MODULE_State DataSet() {
    PORTD |= 0x04;
    PORTD &= ~0x01;
    PORTD &= ~0x02;
    OUTbits.OUT = 0x04; //Data is displayed
    return SETData;

}

_7MODULE_Events ReadEvent() {
    _7MODULE_Events event = 0;
    if (StartCount() && !StartLEDTEST() && !StartSetData()) {
        event = count;
    } else if (StartSetData() && !StartLEDTEST() && !StartCount()) {
        event = set;
    } else if (StartLEDTEST() && !StartSetData() && !StartCount()) {
        event = test;
    } else {
        event = idel;
    }
    return event;
}

_7MODULE_State FSM(_7MODULE_State NextState, _7MODULE_Events event, char Digit) {
    switch (NextState) {
        case Count:
            NextState = EventAction(event, NextState, Digit);
            break;
        case LEDTest:
            NextState = EventAction(event, NextState, Digit);
            break;
        case SETData:
            NextState = EventAction(event, NextState, Digit);
            break;
        case IDLE:
            NextState = EventAction(event, NextState, Digit);
        default:
            break;
    }
    return NextState;

}

_7MODULE_State EventAction(_7MODULE_Events event, _7MODULE_State NextState, char Digit) {
    switch (event) {
        case count:
            NextState = Counting(Digit);
            break;
        case test:
            NextState = LEDTested(Digit);
            break;
        case set:
            NextState = DataSet();
            break;
        case idel:
            NextState = IDELS();
            break;
        default:
            break;

    }
    return NextState;
}

void Counter(char Digit) {
    for (int i = 0; i < 10; i++) {
        switch (Digit) {
            case 0:
                PORTC = DigitData[i];
                PORTD |= 0x78; //Disable  all Digit 
                _delay_ms(5);
                PORTD &= ~0x08; //Enable Digit 0
                break;
            case 1:
                PORTC = DigitData[i];
                PORTD |= 0x78; //Disable  all Digit  
                _delay_ms(5);
                PORTD &= ~0x10; //Enable Digit 1
                break;
            case 2:
                PORTC = DigitData[i];
                PORTD |= 0x78; //Disable  all Digit 
                _delay_ms(5);
                PORTD &= ~0x20; //Enable Digit 2
                break;
            case 3:
                PORTC = DigitData[i];
                PORTD |= 0x78; //Disable  all Digit 
                _delay_ms(5);
                PORTD &= ~0x40; //Enable Digit 3
                break;

        }
        _delay_ms(200);
    }

}
void InterFacingPORT();

void counterLED(char Digit) {
    for (int i = 0; i < 7; i++) {
        switch (Digit) {
            case 0:
                PORTC = LEDData[i];
                PORTD |= 0x78; //Disable  all Digit 
                _delay_ms(5);
                PORTD &= ~0x08; //Enable Digit 0
                break;
            case 1:
                PORTC = LEDData[i];
                PORTD |= 0x78; //Disable  all Digit  
                _delay_ms(5);
                PORTD &= ~0x10; //Enable Digit 1
                break;
            case 2:
                PORTC = LEDData[i];
                PORTD |= 0x78; //Disable  all Digit 
                _delay_ms(5);
                PORTD &= ~0x20; //Enable Digit 2
                break;
            case 3:
                PORTC = LEDData[i];
                PORTD |= 0x78; //Disable  all Digit 
                _delay_ms(5);
                PORTD &= ~0x40; //Enable Digit 3
                break;
        }
        _delay_ms(200);
    }
}

void Tigger_Function(int Data) {
    int a = Data / 1000; // Digit 1000's 
    int b = (Data / 100) % 10; // Digit 100's
    int c = (Data / 10) % 10; // Digit 10's 
    int d = Data % 10; //Digit 1's

}

void InterfacingModule(_7MODULE_State NextState, _7MODULE_Events event, char NDigit) {
    for (int i = 0; i < NDigit; i++) {
        NextState = FSM(NextState, event, i);
    }
}




