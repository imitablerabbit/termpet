#include "termpet_ui.h"

#include <ncurses.h>
#include <menu.h>

#include <stdlib.h>
#include <string.h>

int init_ui() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

int main_menu() {
    WINDOW *menu_window;
    MENU *menu;
    ITEM *menu_items[MAIN_MENU_ITEM_COUNT+1];
    ITEM *cur_item;

    int i;
    int c;
    int width;

    int name_width;
    char format[10];
    char *item_names[MAIN_MENU_ITEM_COUNT];

    int should_quit;
    int return_val;

    width = COLS-20;
    name_width = width-4;
    sprintf(format, "%%-%ds", name_width);
    for (i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        item_names[i] = (char*)calloc(name_width, sizeof(char));
        sprintf(item_names[i], format, choices[i]);
        menu_items[i] = new_item(item_names[i], descriptions[i]);
        set_item_userptr(menu_items[i], (void*)&item_data[i]);
    }
    menu_items[MAIN_MENU_ITEM_COUNT] = NULL;
    menu = new_menu(menu_items);
    menu_window = newwin(5, width, (LINES/2)-MAIN_MENU_ITEM_COUNT, 10);

    keypad(menu_window, TRUE);
    wtimeout(menu_window, 10);

    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 0, 0, 1, 1));

    menu_opts_off(menu, O_SHOWDESC);
    set_menu_mark(menu, "* ");

    box(menu_window, 0, 0);

    refresh();

    post_menu(menu);
    wrefresh(menu_window);

    should_quit = 0;
    return_val = -1;
    while ((c = wgetch(menu_window)) != 'q' && should_quit != 1 ) {
        switch(c) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10:
                cur_item = current_item(menu);
                return_val = *(int*)item_userptr(cur_item);
                goto EXIT;
        }
        wrefresh(menu_window);
    }

EXIT:
    unpost_menu(menu);
    free_menu(menu);
    for (i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        free_item(menu_items[i]);
        free(item_names[i]);
    }
    endwin();
    return return_val;
}
