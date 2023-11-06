#ifndef _MY_UI_H
#define _MY_UI_H

#include "config.h"

typedef enum 
{
	UL = 0,
	UR,
	DR,
	DL,
	CENTER,
	PSUM,
}Point_e;

typedef struct 
{
	int x;
	int y;
}Point_t;

extern Point_t Point[PSUM];
void Point_Update(Point_t *Point);
void MoveAndZoom_Notch(Point_t *Point);

void UI_send_char_group1(void);
void UI_send_char_1(void);
void UI_send_char_2(void);
void UI_send_char_3(void);
void UI_send_char_4(void);
void UI_send_char_5(void);
void UI_send_char_6(void);
void UI_send_char_7(void);
void UI_send_char_8(void);
void UI_send_int_1(void);
void UI_send_group1(void);
void UI_send_group2(void);
void UI_send_group3(void);
void UI_send_group4(void);




#endif
