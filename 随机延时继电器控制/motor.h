#ifndef __MOTOR_H_
#define __MOTOR_H_


#ifndef OSC
#define OSC 12000000
#endif
#ifndef freqT0
#define freqT0 1000    //�����жϵ�Ƶ��
#endif
#ifndef CountT0
#define CountT0 65536-OSC/12/freqT0
#endif

extern char  SW_MOTO;//ֱ������Ͳ��������ѡ�񿪹�

//��ʼ����� ʹ�ö�ʱ��0
void motorInit();
void run(char enble);
void SelectMotorDC(char enble);
#endif