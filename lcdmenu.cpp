/*
 * lcdmenu - choose an option using 4x20 serial lcd 
 * and simple keypad.
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "gndkeys.h"
#include "lcdmenu.h"

extern SoftwareSerial lcdSerial;
extern Keyboard pdkeys;

const char lcd_clear_str[] = "\xfe\x01";
const char lcd_blink_cursor_str[] = "\xfe\x0d";

static const char lcd_line_positions[] = { '\x80', '\xc0', '\x94', '\xd4' };

void
lcd_gotoline(uint8_t ln)
{
	if(ln > 3)
		ln = 3;
 	lcdSerial.print("\xfe");
	lcdSerial.print(lcd_line_positions[ln]);
}

void
lcd_goto(uint8_t ln, uint8_t pos)
{
	if(ln > 3)
		ln = 3;
	if(pos > 19)
		pos = 19; 
	char c = lcd_line_positions[ln] + pos;
 	lcdSerial.print("\xfe");
	lcdSerial.print(c);
}


/*
 * display menu and set up for choosing an entry
 */
void
LcdMenu::init_display(LcdMenu_entry *mp, int n)
{
	menu = mp;
	n_entries = n;

	Serial.print("menu_n ");
	Serial.print(n_entries);
	Serial.print("\r\n");
	
	cur_line = 0; 
	top = 0;
	cur_item = top + cur_line;

	draw();
	lcdSerial.print(lcd_blink_cursor_str);
}

/*
 * process a keystroke and update the display accordingly.
 * if an entry is selected, returns nonzero, else returns 0.
 */
int
LcdMenu::keypress(unsigned char key)
{
	int new_item = cur_item;
 	switch(key) {
	case K_RIGHT:
		return cur_item;
		break;
	case K_UP:
		if(cur_item > 0)
			new_item = cur_item - 1;
		break;
	case K_DOWN:
		if(cur_item <= n_entries)
			new_item = cur_item + 1;
		break;
	}

	if(new_item != cur_item) {
		Serial.print(" new_item ");
		Serial.print(new_item);
		// deal with change of cur-item
		if(new_item < top) {   // offscreen above
			top -= 4;
			if(top < 0)
				top = 0;
			cur_line = new_item - top;
			draw();//menu, n_entries, top);
		} else if(new_item > top + 3) { // offscreen below
			top += 4;
			if(new_item + top > n_entries) {
				top = n_entries - 4;
			}
			cur_line = new_item - top;
			draw();//draw(menu, n_entries, top);
		} else  {  // still onscreen but moved cursor
			cur_line = new_item - top;
			lcd_gotoline(cur_line);
		}
		cur_item = new_item;

		Serial.print(" cur_item ");
		Serial.print(cur_item);
		Serial.print(" top ");
		Serial.print(top);
		Serial.print(" cur_line ");
		Serial.print(cur_line);
		Serial.print("\r\n");
	}
	return 0;
}


/*
 * draw a screenful of menu options, starting with entry number 'top'.
 * entries start at 0 and run though n_entries-1.
 */
void
//LcdMenu::draw(LcdMenu_entry *menu, int n_entries, int top)
LcdMenu::draw()
{
 	uint8_t i;
	if(top+4 > n_entries)
 		top = n_entries-4;

	lcdSerial.print(lcd_clear_str);
	for(i = 0; i < 4; i++) {
		lcd_gotoline(i);
		lcdSerial.print(menu[top+i].label);
	}
	lcd_gotoline(cur_line);
}
