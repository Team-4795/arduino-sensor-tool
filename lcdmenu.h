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
  PFV event;
};


class LcdMenu {

 private:
  void draw(LcdMenu_entry *menu, int n_entries, int top_entry);
   
 public:
   int run(LcdMenu_entry *menu, int n_entries);
};

extern const char lcd_clear_str[];
extern const char lcd_blink_cursor_str[];
extern void lcd_gotoline(uint8_t ln);
extern void lcd_goto(uint8_t ln, uint8_t pos);

