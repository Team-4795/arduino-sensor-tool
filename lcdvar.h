/*
 * lcdvar - display and change config variables, using an HD44780 LCD display 
 * and simple keypad.
 */
#include <SoftwareSerial.h>


#define LCDVAR_INT	0
#define LCDVAR_INDEX	1


typedef struct {
	int type;
	int curval;
	int minval;
	int maxval;
	int step;
	const char *name;
	const char **valnames; // if lcdvar_index
	unsigned char disploc;  /* need name+6 chars available */
} lcdvar;


void lcdvar_key(unsigned char key);
void lcdvar_draw(void);
void lcdvar_clear(void);
/*
class lcdVarEdit {
private:
	NewSoftSerial c_ser;
	lcdvar *c_table;
	int c_current;
	int c_nvars;
	NewSoftSerial c_disp;

protected:
	void draw(void);
	void clear(void);

public:
	lcdVarEdit(lcdvar *tab, NewSoftSerial ser);

	void handle_key(unsigned char key);

};
*/
