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
 * display menu, use keypad UP/DOWN to select an entry.
 *    LEFT key cancels, returning -1
 *	RIGHT key selects, returns entry index.
*/
int
LcdMenu::run(LcdMenu_entry *menu, int n_entries)
{
	int top;        // entry displayed at top of screen
	int cur_line;   // screen line that cursor is pointing to
	int cur_item;   // current menu item index
 	int new_item;

	Serial.print("menu_n ");
	Serial.print(n_entries);
	Serial.print("\r\n");
	
	cur_line = 0; 
	top = 0;
	cur_item = top + cur_line;
	draw(menu, n_entries, top);
	lcdSerial.print(lcd_blink_cursor_str);
	lcd_gotoline(cur_line);
	pdkeys.addhook(NULL);	 // reorg to use callback
 	int k;

	// invariant at the top of the while loop:
	//   cur_item = top + cur_line
	while(1) {
		new_item = cur_item;
		k = pdkeys.poll();
		switch(k) {
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
				draw(menu, n_entries, top);
				cur_line = new_item - top;
			} else if(new_item > top + 3) { // offscreen below
				top += 4;
				if(new_item + top > n_entries) {
					top = n_entries - 4;
				}
				draw(menu, n_entries, top);
				cur_line = new_item - top;
			} else  {  // still onscreen but moved cursor
				cur_line = new_item - top;
			}
 			lcd_gotoline(cur_line);
			cur_item = new_item;

			Serial.print(" cur_item ");
			Serial.print(cur_item);
			Serial.print(" top ");
			Serial.print(top);
			Serial.print(" cur_line ");
			Serial.print(cur_line);
			Serial.print("\r\n");
		}
		delay(30);
	}
}

/*
 * draw a screenful of menu options, starting with entry number 'top'.
 * entries start at 0 and run though n_entries-1.
 */
void
LcdMenu::draw(LcdMenu_entry *menu, int n_entries, int top)
{
 	uint8_t i;
	if(top+4 > n_entries)
 		top = n_entries-4;

	lcdSerial.print(lcd_clear_str);
	for(i = 0; i < 4; i++) {
		// move to line i
		lcd_gotoline(i);
		lcdSerial.print(menu[top+i].label);
	}
}
