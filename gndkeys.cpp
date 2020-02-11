
#ifdef LINUX_TEST
#include <stdint.h>
#include <stdio.h>
uint8_t PORTD, DDRD, PIND;
#else
#include <Arduino.h>
#endif

#include "gndkeys.h"

Keyboard::Keyboard()
{
// possible future args: port addr, bitmask
	k_pressed = 0;
	k_new = 0;
	flush();
	
	// keypad: port D bits 7:3.  input with pullup
	DDRD &=  0x07;
	PORTD |= 0xf8;
}

void
Keyboard::poll()
{
	unsigned char rawkeys;
	unsigned char m;
	int i;
	int res = 0;
	rawkeys = 0x1f ^ (PIND >> 3);

	if(rawkeys == k_new) {  /* stable for at least one loop */
		if(k_pressed ^ rawkeys) { /* new stable keys pressed */
			for(i = 1, m = 1; i < 6; i++, m<<=1) {
				if( (m & rawkeys) && !(m & k_pressed)) {
					// key pressed
/*					Serial.write('\n');
					Serial.write('p'); 
					Serial.print(i, DEC);
					Serial.write('\n');
					Serial.write('\r');*/
					
					enqueue(i);
				}
				if( !(m & rawkeys) && (m & k_pressed)) {
					// key released
/*					Serial.write('\n');
					Serial.write('u');
					Serial.print(i, DEC);
					Serial.write('\n');
					Serial.write('\r');*/
					// todo optional enqueue key-UP for user
				}
			}
			k_pressed = rawkeys;
		}
	}
	k_new = rawkeys;
}

/* input queue of pending keycodes */
void
Keyboard::flush()
{
	key_buffer_head = 0;
	key_buffer_tail = 0;
}

void
Keyboard::enqueue(unsigned char k)
{
	uint8_t buf_next = (key_buffer_tail + 1) % ST_KBD_BUFF_SIZE;
	if(buf_next != key_buffer_head) {
		key_buffer[ key_buffer_tail ] = k;
		key_buffer_tail = buf_next;
	} // else flag overflow
}

unsigned char
Keyboard::getkey()
{
	if(key_buffer_head == key_buffer_tail)
		return 0;
	else {
		unsigned char k;
		k = key_buffer[ key_buffer_head ];
		key_buffer_head = (key_buffer_head + 1 ) % ST_KBD_BUFF_SIZE;
		return k;	
	}
}

uint8_t
Keyboard::available()
{
	if(key_buffer_head == key_buffer_tail)
		return 0;
	else
		return 1;
}



#ifdef LINUX_TEST

int
Keyboard::test_harness(void)
{
	int rc = 0;
	if(available())
 		rc = 1;
	enqueue(1);
	if(!available())
 		rc = 2;

	unsigned char k;
	k = getkey();
	if(k != 1)
 		rc = 3;

	int i;
	for(i = 2; i <= 8; i++) {
 		enqueue(i);
	}
	for(i = 2; i <= 8; i++) {
 		k = getkey();
		printf("  %d", k);
		if(k != i)
			rc = 4;
	}
	printf("    (rc=%d)\n", rc);
	
	for(i = 10; i <= 19; i++) {   // overfill
 		enqueue(i);
	}
	for(i = 10; i <= 16; i++) {
 		k = getkey();
		printf("  %d", k);
		if(k != i)
			rc = 5;
	}
	printf("    (rc=%d)\n", rc);

	return rc;
}

int
main(int argc, char **argv)
{
	printf("gndkeys test\n");

	Keyboard kbd;
	int rc = kbd.test_harness();
	printf("finshed rc=%d\n", rc);		
}


#endif
