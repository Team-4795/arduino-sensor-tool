/*
 * class to display a menu of options on a 4x20 character LCD,
 * and 
 */

typedef void (*PFV)();

struct LcdMenu_entry {
  int code;
  const char *label;
  PFV init;
  PFV loop;
};


class LcdMenu {
 LcdMenu_entry *menu;
 uint8_t n_entries;
	int top;        // entry displayed at top of screen
	int cur_line;   // screen line that cursor is pointing to
	int cur_item;   // current menu item index

 private:
  void draw(LcdMenu_entry *menu, int n_entries, int top_entry);
   
 public:
   void display(LcdMenu_entry *menu, int n_entries);

   int keypress(uint8_t key);
// old interface
   int run(LcdMenu_entry *menu, int n_entries);
};

extern const char lcd_clear_str[];
extern const char lcd_blink_cursor_str[];
extern void lcd_gotoline(uint8_t ln);
extern void lcd_goto(uint8_t ln, uint8_t pos);

