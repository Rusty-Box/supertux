/*
  menu.h
  
  Super Tux - Menu
  
  by Tobias Glaesser
  tobi.web@gmx.de
  http://www.newbreedsoftware.com/supertux/
  
  December 20, 2003
*/

#ifndef SUPERTUX_MENU_H
#define SUPERTUX_MENU_H

#include <SDL.h>
#include "texture.h"
#include "timer.h"
#include "type.h"

/* Kinds of menu items */
enum MenuItemKind {
  MN_ACTION,
  MN_GOTO,
  MN_TOGGLE,
  MN_BACK,
  MN_DEACTIVE,
  MN_TEXTFIELD,
  MN_NUMFIELD,
  MN_STRINGSELECT,
  MN_LABEL,
  MN_HL /* horizontal line */
};

typedef struct menu_item_type
  {
    MenuItemKind kind;
    int toggled;
    char *text;
    char *input;
    string_list_type* list;
    void* target_menu;
  }
menu_item_type;

menu_item_type* menu_item_create(MenuItemKind kind, char *text, int init_toggle, void* target_menu);
void menu_item_change_text (menu_item_type* pmenu_item, const char *text);
void menu_item_change_input(menu_item_type* pmenu_item, const char *text);

typedef struct menu_type
{
  // center of the menu on the screen
  int x;
  int y;

  int num_items;
  int active_item;
  int arrange_left;
  menu_item_type *item;
  timer_type effect;
}
menu_type;

void menu_init   (menu_type* pmenu);
void menu_free   (menu_type* pmenu);
void menu_additem(menu_type* pmenu, menu_item_type* pmenu_item);
menu_item_type* menu_additem(menu_type* pmenu, MenuItemKind kind, char *text, int init_toggle, void* target_menu);
void menu_action (menu_type* pmenu);
int  menu_check  (menu_type* pmenu);
void menu_draw   (menu_type* pmenu);
void menu_set_current(menu_type* pmenu);

/* Action done on the menu */
enum MenuAction {
  MENU_ACTION_NONE = -1,
  MENU_ACTION_UP,
  MENU_ACTION_DOWN,
  MENU_ACTION_LEFT,
  MENU_ACTION_RIGHT,
  MENU_ACTION_HIT,
  MENU_ACTION_INPUT,
  MENU_ACTION_REMOVE
};

/* (global) menu variables */
extern MenuAction menuaction;
extern bool show_menu;
extern bool menu_change;
extern texture_type checkbox, checkbox_checked, back, arrow_left, arrow_right;

extern menu_type main_menu, game_menu, options_menu, highscore_menu, load_game_menu, save_game_menu;
extern menu_type* current_menu, * last_menu;

/* input implementation variables */
extern int delete_character;
extern char mn_input_char;

/* Reset the global menu variables */
void menu_reset(void);

/* "Calculate" and draw the menu */
void menu_process_current(void);

/* Check for a menu event */
void menu_event(SDL_keysym* keysym);

#endif /*SUPERTUX_MENU_H*/

