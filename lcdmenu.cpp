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

// todo export these
char lcd_clear_str[] = "\xfe\x01";
char lcd_blink_cursor_str[] = "\xfe\x0d";

void
lcd_gotoline(uint8_t ln)
{
	static char line_positions[] = { '\x80', '\xc0', '\x94', '\xd4' };
	if(ln > 3)
		ln = 3;
 	lcdSerial.print("\xfe");
	lcdSerial.print(line_positions[ln]);
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
	int cur_line;   // screen line that we're pointing to

	cur_line = 0;
	top = 0;
	draw(menu, n_entries, top);
	lcdSerial.print(lcd_blink_cursor_str);
	pdkeys.addhook(NULL);	 // todo use callback
 	int k;

	while(1) {
		k = pdkeys.poll();
		switch(k) {
 		case K_RIGHT:
 			return menu[top + cur_line].code;
 			break;
		case K_UP:
			if(cur_line > 0) {
 				cur_line--;
				lcd_gotoline(cur_line);
			} else {
			}
			break;
		case K_DOWN:
			if(cur_line < 3) {
 				cur_line++;
				lcd_gotoline(cur_line);
			} else {
			}
			break;
		}
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
	if(top+4 >= n_entries)
 		top = n_entries-4-1;

	Serial.print("menu_n ");
	Serial.print(n_entries);
	Serial.print(" top ");
	Serial.print(top);
	Serial.print("\r\n");

	lcdSerial.print(lcd_clear_str);
	for(i = 0; i < 4; i++) {
		// move to line i
		lcd_gotoline(i);
		lcdSerial.print(menu[top+i].label);
	}
}
