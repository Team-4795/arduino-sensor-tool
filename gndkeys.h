/*
 * single-purpose keypad class, with a somewhat general API
 */

#define K_UP	5	
#define K_DOWN	4	
#define K_ENTER	3	
#define K_RIGHT	2	
#define K_LEFT	1	

#define K_NONE 0

extern int checkkbd(void);

#define ST_KBD_BUFF_SIZE 8

class Keyboard {
private:
	unsigned char k_pressed;
	unsigned char k_new;

	unsigned char key_buffer[ST_KBD_BUFF_SIZE];
	uint8_t key_buffer_head;
	uint8_t key_buffer_tail;

	void enqueue(unsigned char k);
	
public:
	Keyboard(void);
	
	void poll(void);

	void flush();
	unsigned char getkey(void);
	uint8_t available(void);

#ifdef LINUX_TEST
	int test_harness(void);
#endif
	
};
