/*
 * class to display a menu of options on a 4x20 character LCD,
 * and 
 */

typedef int PFV;

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
