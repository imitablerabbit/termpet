#include "termpet_ui.h"

#include <ncurses.h>
#include <menu.h>

#include <stdlib.h>
#include <string.h>

int init_ui() {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

int main_menu() {
    WINDOW *menu_window;
    MENU *menu;
    ITEM *menu_items[MAIN_MENU_ITEM_COUNT+1];
    ITEM *cur_item;

    int i;
    int c;
    int startx, starty;
    int width;

    int name_width;
    char format[10];
    char *item_names[MAIN_MENU_ITEM_COUNT];

    int should_quit;
    int return_val;

    startx = COLS/10;
    starty = (LINES-9)/2;

    width = COLS-(startx*2);
    name_width = width-4;
    sprintf(format, "%%-%ds", name_width);

    for (i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        item_names[i] = (char*)calloc(name_width, sizeof(char));
        sprintf(item_names[i], format, main_menu_choices[i]);
        menu_items[i] = new_item(item_names[i], main_menu_descriptions[i]);
        set_item_userptr(menu_items[i], (void*)&main_menu_item_data[i]);
    }
    menu_items[MAIN_MENU_ITEM_COUNT] = NULL;
    menu = new_menu(menu_items);
    menu_window = newwin(5, width, starty + 2, startx);

    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 0, 0, 1, 1));
    menu_opts_off(menu, O_SHOWDESC);
    set_menu_mark(menu, "* ");

    keypad(menu_window, TRUE);
    wtimeout(menu_window, 10);

    mvprintw((LINES/2)-MAIN_MENU_ITEM_COUNT-1, (COLS-strlen(TITLE_TEXT))/2,
            TITLE_TEXT);

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

int new_game_menu(char **name) {
    WINDOW *menu_window;
    MENU *menu;
    ITEM *menu_items[NEW_GAME_MENU_ITEM_COUNT+1];
    ITEM *cur_item;

    WINDOW *form_window;
    FORM *form;
    FIELD *fields[2];

    int i;
    int c;
    int startx, starty;
    int width;

    int name_width;
    char format[10];
    char *item_names[NEW_GAME_MENU_ITEM_COUNT];

    int should_quit;
    int return_val;

    startx = COLS/10;
    starty = (LINES-9)/2;

    wclear(stdscr);
    width = COLS-(startx*2);
    name_width = (width/2)-4;
    sprintf(format, "%%-%ds", name_width);

    for (i = 0; i < NEW_GAME_MENU_ITEM_COUNT; i++) {
        item_names[i] = (char*)calloc(name_width, sizeof(char));
        sprintf(item_names[i], format, new_game_menu_choices[i]);
        menu_items[i] = new_item(item_names[i], new_game_menu_descriptions[i]);
        set_item_userptr(menu_items[i], (void*)&new_game_menu_item_data[i]);
    }
    menu_items[NEW_GAME_MENU_ITEM_COUNT] = NULL;
    menu = new_menu(menu_items);
    menu_window = newwin(7, width, starty+2, startx);

    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 0, 0, 5, 1));
    menu_opts_off(menu, O_SHOWDESC);
    set_menu_mark(menu, "* ");
    set_menu_format(menu, 1, 2);

    keypad(menu_window, TRUE);
    wtimeout(menu_window, 10);

    fields[0] = new_field(1, width-6, 2, 0, 0, 0);
    fields[1] = NULL;

    set_field_back(fields[0], A_UNDERLINE);
    field_opts_off(fields[0], O_AUTOSKIP);
    set_field_buffer(fields[0], 0, "Matthew");

    form = new_form(fields);
    form_window = newwin(3, width-5, starty+3, startx+3);
    set_form_win(form, form_window);
    set_form_sub(form, derwin(form_window, 0, 0, 0, 0));

    mvprintw(starty, (COLS-strlen(NEW_GAME_MENU_TEXT))/2,
            NEW_GAME_MENU_TEXT);

    box(menu_window, 0, 0);

    refresh();

    post_menu(menu);
    wrefresh(menu_window);

    post_form(form);
    wrefresh(form_window);

    mvwprintw(form_window, 0, 0, "Pet Name:");

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
            case KEY_LEFT:
                menu_driver(menu, REQ_LEFT_ITEM);
                break;
            case KEY_RIGHT:
                menu_driver(menu, REQ_RIGHT_ITEM);
                break;
            case 10:
                cur_item = current_item(menu);
                return_val = *(int*)item_userptr(cur_item);
                *name = field_buffer(fields[0], 0);
                goto EXIT;
        }
        wrefresh(menu_window);
        wrefresh(form_window);
        mvwprintw(form_window, 0, 0, "Pet Name:");
    }

EXIT:
    unpost_form(form);
    unpost_menu(menu);
    free_form(form);
    free_menu(menu);
    for (i = 0; i < NEW_GAME_MENU_ITEM_COUNT; i++) {
        free_item(menu_items[i]);
        free(item_names[i]);
    }
    endwin();
    return return_val;
}
