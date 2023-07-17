//=====[Libraries]=============================================================

#include <stdint.h>
#include "display.h"
#include "Arduino.h"

//=====[Declaration of private defines]========================================

#define DISPLAY_IR_CLEAR_DISPLAY B00000001
#define DISPLAY_IR_ENTRY_MODE_SET B00000100
#define DISPLAY_IR_DISPLAY_CONTROL B00001000
#define DISPLAY_IR_FUNCTION_SET B00100000
#define DISPLAY_IR_SET_DDRAM_ADDR B10000000

#define DISPLAY_IR_ENTRY_MODE_SET_INCREMENT B00000010
#define DISPLAY_IR_ENTRY_MODE_SET_DECREMENT B00000000
#define DISPLAY_IR_ENTRY_MODE_SET_SHIFT B00000001
#define DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT B00000000

#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON B00000100
#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF B00000000
#define DISPLAY_IR_DISPLAY_CONTROL_CURSOR_ON B00000010
#define DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF B00000000
#define DISPLAY_IR_DISPLAY_CONTROL_BLINK_ON B00000001
#define DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF B00000000

#define DISPLAY_IR_FUNCTION_SET_8BITS B00010000
#define DISPLAY_IR_FUNCTION_SET_4BITS B00000000
#define DISPLAY_IR_FUNCTION_SET_2LINES B00001000
#define DISPLAY_IR_FUNCTION_SET_1LINE B00000000
#define DISPLAY_IR_FUNCTION_SET_5x10DOTS B00000100
#define DISPLAY_IR_FUNCTION_SET_5x8DOTS B00000000

#define DISPLAY_20x4_LINE1_FIRST_CHARACTER_ADDRESS 0
#define DISPLAY_20x4_LINE2_FIRST_CHARACTER_ADDRESS 64
#define DISPLAY_20x4_LINE3_FIRST_CHARACTER_ADDRESS 20
#define DISPLAY_20x4_LINE4_FIRST_CHARACTER_ADDRESS 84

#define DISPLAY_RS_INSTRUCTION 0
#define DISPLAY_RS_DATA 1

#define DISPLAY_RW_WRITE 0
#define DISPLAY_RW_READ 1

#define DISPLAY_PIN_RS 4
#define DISPLAY_PIN_RW 5
#define DISPLAY_PIN_EN 6
#define DISPLAY_PIN_D0 7
#define DISPLAY_PIN_D1 8
#define DISPLAY_PIN_D2 9
#define DISPLAY_PIN_D3 10
#define DISPLAY_PIN_D4 11
#define DISPLAY_PIN_D5 12
#define DISPLAY_PIN_D6 13
#define DISPLAY_PIN_D7 14

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

#define displayD4 5
#define displayD5 4
#define displayD6 3
#define displayD7 10
#define displayRs 7
#define displayEn 6


//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static display_t display;
static bool initial8BitCommunicationIsCompleted;

//=====[Declarations (prototypes) of private functions]========================

static void displayPinWrite(uint8_t pinName, int value);
static void displayDataBusWrite(uint8_t dataByte);
static void displayCodeWrite(bool type, uint8_t dataBus);



//=====[Implementations of public functions]===================================

void displayInit(displayConnection_t connection) {

  pinMode(10, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  display.connection = connection;

  initial8BitCommunicationIsCompleted = false;

  delay(50);

  displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                   DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_8BITS);
  delay(5);

  displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                   DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_8BITS);
  delay(1);

  displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                   DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_8BITS);
  delay(1);

  switch (display.connection) {
    case DISPLAY_CONNECTION_GPIO_8BITS:
      displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                       DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_8BITS | DISPLAY_IR_FUNCTION_SET_2LINES | DISPLAY_IR_FUNCTION_SET_5x8DOTS);
      delay(1);
      break;

    case DISPLAY_CONNECTION_GPIO_4BITS:
      displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                       DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_4BITS);
      delay(1);

      initial8BitCommunicationIsCompleted = true;

      displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                       DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_4BITS | DISPLAY_IR_FUNCTION_SET_2LINES | DISPLAY_IR_FUNCTION_SET_5x8DOTS);
      delay(1);
      break;
  }

  displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                   DISPLAY_IR_DISPLAY_CONTROL | DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF | DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF | DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF);
  delay(1);

  displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                   DISPLAY_IR_CLEAR_DISPLAY);
  delay(1);

  displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                   DISPLAY_IR_ENTRY_MODE_SET | DISPLAY_IR_ENTRY_MODE_SET_INCREMENT | DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT);
  delay(1);

  displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                   DISPLAY_IR_DISPLAY_CONTROL | DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON | DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF | DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF);
  delay(1);
}

void displayCharPositionWrite(uint8_t charPositionX, uint8_t charPositionY) {
  switch (charPositionY) {
    case 0:
      displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                       DISPLAY_IR_SET_DDRAM_ADDR | (DISPLAY_20x4_LINE1_FIRST_CHARACTER_ADDRESS + charPositionX));
      delay(1);
      break;

    case 1:
      displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                       DISPLAY_IR_SET_DDRAM_ADDR | (DISPLAY_20x4_LINE2_FIRST_CHARACTER_ADDRESS + charPositionX));
      delay(1);
      break;

    case 2:
      displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                       DISPLAY_IR_SET_DDRAM_ADDR | (DISPLAY_20x4_LINE3_FIRST_CHARACTER_ADDRESS + charPositionX));
      delay(1);
      break;

    case 3:
      displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                       DISPLAY_IR_SET_DDRAM_ADDR | (DISPLAY_20x4_LINE4_FIRST_CHARACTER_ADDRESS + charPositionX));
      delay(1);
      break;
  }
}


void displayStringWrite(const char* str) {
  while (*str) {
    displayCodeWrite(DISPLAY_RS_DATA, *str++);
  }
}

//=====[Implementations of private functions]==================================

static void displayCodeWrite(bool type, uint8_t dataBus) {
  if (type == DISPLAY_RS_INSTRUCTION)
    displayPinWrite(DISPLAY_PIN_RS, DISPLAY_RS_INSTRUCTION);
  else
    displayPinWrite(DISPLAY_PIN_RS, DISPLAY_RS_DATA);
  displayPinWrite(DISPLAY_PIN_RW, DISPLAY_RW_WRITE);
  displayDataBusWrite(dataBus);
}

static void displayPinWrite(uint8_t pinName, int value) {
  switch (display.connection) {
    case DISPLAY_CONNECTION_GPIO_4BITS:
      switch (pinName) {
        case DISPLAY_PIN_D4: digitalWrite(displayD4, value); break;
        case DISPLAY_PIN_D5: digitalWrite(displayD5, value); break;
        case DISPLAY_PIN_D6: digitalWrite(displayD6, value); break;
        case DISPLAY_PIN_D7: digitalWrite(displayD7, value); break;
        case DISPLAY_PIN_RS: digitalWrite(displayRs, value); break;
        case DISPLAY_PIN_EN: digitalWrite(displayEn, value); break;
        case DISPLAY_PIN_RW: break;
        default: break;
      }
      break;
  }
}

static void displayDataBusWrite(uint8_t dataBus) {
  displayPinWrite(DISPLAY_PIN_EN, LOW);
  displayPinWrite(DISPLAY_PIN_D7, dataBus & B10000000);
  displayPinWrite(DISPLAY_PIN_D6, dataBus & B01000000);
  displayPinWrite(DISPLAY_PIN_D5, dataBus & B00100000);
  displayPinWrite(DISPLAY_PIN_D4, dataBus & B00010000);
  switch (display.connection) {
    case DISPLAY_CONNECTION_GPIO_4BITS:
      if (initial8BitCommunicationIsCompleted == true) {
        displayPinWrite(DISPLAY_PIN_EN, HIGH);
        delay(1);
        displayPinWrite(DISPLAY_PIN_EN, LOW);
        delay(1);
        displayPinWrite(DISPLAY_PIN_D7, dataBus & B00001000);
        displayPinWrite(DISPLAY_PIN_D6, dataBus & B00000100);
        displayPinWrite(DISPLAY_PIN_D5, dataBus & B00000010);
        displayPinWrite(DISPLAY_PIN_D4, dataBus & B00000001);
      }
      break;
  }
  displayPinWrite(DISPLAY_PIN_EN, HIGH);
  delay(1);
  displayPinWrite(DISPLAY_PIN_EN, LOW);
  delay(1);
}