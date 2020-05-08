#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_




typedef struct
{
    unsigned char Value;
    unsigned char State;
    unsigned char lastValue;
} KeyStruct;
typedef enum  {CheckKey,DelayKey,KeyFallEdge,KeyDown,KeyRisEdge} KeyState;

//°´¼ü¼ì²â
unsigned char key_scan(KeyStruct* KeyNum);
void InitKeyboard();
#endif