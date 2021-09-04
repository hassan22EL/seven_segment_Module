/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* Data  3/9/2021  10:28:30 AM
 * File:  MODULE 
 * Author: Hassan Elsayed  
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_7MODULE_H
#define	XC_7MODULE_H
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>

typedef union {

    struct {
        unsigned A : 1;
        unsigned B : 1;
        unsigned C : 1;
        unsigned D : 1;
        unsigned E : 1;
        unsigned F : 1;
        unsigned G : 1;

    } Databits_t;
    unsigned char Databits;
} MOUDLEData_t;

typedef union {

    struct {
        MOUDLEData_t Data;
        unsigned EN : 1;
        unsigned XP : 1;
    };
    unsigned char MOUDLE;
} MOUDLEbits_t;

MOUDLEbits_t MOUDLEbits;

typedef union {

    struct {
        unsigned C : 1;
        unsigned S : 1;
        unsigned T : 1;

    };
    unsigned char MOD;
} MODbits_t;
MODbits_t MODbits;

typedef union {

    struct {
        unsigned Count_done : 1;
        unsigned setData_done : 1;
        unsigned Test_done : 1;

    };
    unsigned char OUT;
} OUTbits_t;
OUTbits_t OUTbits;

typedef union {

    struct {
        unsigned EN0 : 1; //select Digit 0 
        unsigned EN1 : 1; //select Digit 1 
        unsigned EN2 : 1; //Select Digit 2
        unsigned EN3 : 1; //select Digit 3

    };
    unsigned char Enable;
} Enablebits_t;
Enablebits_t Enablebits;

typedef enum {
    IDLE,
    Count,
    LEDTest,
    SETData

} _7MODULE_State;

typedef enum {
    count,
    set,
    test,
    idel,
} _7MODULE_Events;
void InitModule();
bool StartCount();
bool StartSetData();
bool StartLEDTEST();
void InterFacingPORT();
void Counter(char Digit);
void counterLED(char Digit); 
void Tigger_Function(int Data);
void InterfacingModule(_7MODULE_State NextState, _7MODULE_Events event ,  char  NDigit);
void selectDigit(char Digit);
_7MODULE_State IDELS();
_7MODULE_State Counting(char Digit);
_7MODULE_State LEDTested(char Digit);
_7MODULE_State DataSet();
_7MODULE_Events ReadEvent();
_7MODULE_State EventAction(_7MODULE_Events event, _7MODULE_State NextState ,char Digit);
_7MODULE_State FSM(_7MODULE_State NextState, _7MODULE_Events event ,char Digit);


#endif	/* XC_7MODULE_H */

