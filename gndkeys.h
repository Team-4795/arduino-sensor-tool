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

typedef void (*keyhook)(unsigned char k);


class Keyboard {
private:
	keyhook k_hook;
	
	unsigned char k_pressed;
	unsigned char k_new;
	
public:
	Keyboard(void);
	void addhook(keyhook kh);
	
	int poll(void);

};
