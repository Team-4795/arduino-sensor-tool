/*
 * lcdvar - display and change config variables, using an HD44780 LCD display 
 * and simple keypad.
 */

#include <Arduino.h>
#include "gndkeys.h"
#include "lcdvar.h"

extern SoftwareSerial lcdSerial;

//#define LCDV_DEFPOS 0xd4+8

const char *pwmmodes[] = {
	"OFF",
	"center",
	"var",
};

lcdvar lcdvar_table[] = {
	LCDVAR_INT,	1000,  500,  1500, 50, "1min", NULL, 0xc0+8,
	LCDVAR_INT,	2000,  1500, 2500, 50, "1max", NULL, 0xc0+8,
	LCDVAR_INDEX,	2,     1,       2,  1, "1mode", pwmmodes, 0xc0+8,

	LCDVAR_INT,	1000,  500,  1500, 50, "2min", NULL, 0x94+8,
	LCDVAR_INT,	2000,  1500, 2500, 50, "2max", NULL, 0x94+8,
	LCDVAR_INDEX,	2,     1,       2,  1, "2mode", pwmmodes, 0x94+8,
};
int lcdvar_current = 0;
const int n_lcdvars = sizeof(lcdvar_table)/sizeof(lcdvar);

/*
lcdVarEdit::lcdVarEdit(lcdvar *tab, NewSoftSerial ser)
{
	int i;
	c_table = tab;
	c_ser = ser;
	for(i = 0; c_table[i].name; i++)
		;
	c_nvars = i;
}
*/

void
lcdvar_draw()
{
	lcdvar *lv = &lcdvar_table[lcdvar_current];
	char buf[8];
	
	lcdSerial.write(0xfe); 	lcdSerial.write(lv->disploc);
	lcdSerial.print(lv->name);
	lcdSerial.write('=');
	switch(lv->type) {
	case LCDVAR_INT:
		snprintf(buf, 8, "%6d", lv->curval);
		break;
	case LCDVAR_INDEX:
		snprintf(buf, 8, "%-6s", lv->valnames[lv->curval]);
		break;
	}
	lcdSerial.print(buf);
}

void
lcdvar_clear()
{
	int i;
	lcdvar *lv = &lcdvar_table[lcdvar_current];
	
	lcdSerial.write(0xfe);
	lcdSerial.write(lv->disploc);
	for(i = 0; i < 12; i++)
		lcdSerial.write(' ');
}

void
lcdvar_key(unsigned char key)
{
	lcdvar *lv = &lcdvar_table[lcdvar_current];
	int newvar = lcdvar_current;
	int newvalue = lv->curval;

	switch(key) {
	case K_RIGHT:
		if(lcdvar_current == n_lcdvars-1)
			newvar = 0;
		else
			newvar = lcdvar_current+1;
		break;

	case K_LEFT:
		if(lcdvar_current == 0)
			newvar = n_lcdvars-1;
		else
			newvar = lcdvar_current-1;
		break;

	case K_UP:
		if(lv->curval + lv->step > lv->maxval) {
			if(lv->type == LCDVAR_INDEX)
				newvalue = lv->minval;
			else
				newvalue = lv->maxval;
		} else
			newvalue = lv->curval + lv->step;
		break;

	case K_DOWN:
		if(lv->curval - lv->step < lv->minval) {
			if(lv->type == LCDVAR_INDEX)
				newvalue = lv->maxval;
			else
			newvalue = lv->minval;
		} else 
			newvalue = lv->curval - lv->step;
		break;

	}

	
	if(newvar != lcdvar_current) { // change item
		if(lv->disploc != lcdvar_table[newvar].disploc) {
			lcdvar_clear();
		}
		lcdvar_current = newvar;

	} 
	lv->curval = newvalue;
	lcdvar_draw();
}


