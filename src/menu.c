#include "termpet_ui.h"
#include "termpet.h"

#include <ncurses.h>
#include <menu.h>

#include <stdlib.h>
#include <string.h>

#include <assert.h>

int init_ui() {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

int init_menu(MENU *m, WINDOW *menu_window) {
    menu_opts_off(m, O_SHOWDESC);
    set_menu_mark(m, "* ");
    keypad(menu_window, TRUE);
    wtimeout(menu_window, 10);
    return 0;
}

int init_form(FORM *f, WINDOW *form_window) {
    keypad(form_window, TRUE);
    wtimeout(form_window, 10);
    return 0;
}

int activate_menu(MENU *m) {
    set_menu_fore(m, A_REVERSE);
    set_menu_mark(m, "* ");
    return 0;
}

int deactivate_menu(MENU *m) {
    set_menu_fore(m, A_NORMAL);
    set_menu_mark(m, "  ");
    return 0;
}

int activate_form(FORM *f) {
    FIELD *field;

    curs_set(1);
    field = current_field(f);
    return 0;
}

int deactivate_form(FORM *f) {
    FIELD *field;

    curs_set(0);
    field = current_field(f);
    return 0;
}

int menu_handler(MENU *m, int ch, ITEM** current) {
    ITEM* item;

    switch(ch) {
        case KEY_DOWN:
            // TODO: these need to account for the number of columns in the
            // menu.
            item = current_item(m);
            if (item_index(item) >= item_count(m)-1) {
                return MENU_DRIVER_EXIT_DOWN;
            }
            menu_driver(m, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            // TODO: these need to account for the number of columns in the
            // menu.
            item = current_item(m);
            if (item_index(item) <= 0) {
                return MENU_DRIVER_EXIT_UP;
            }
            menu_driver(m, REQ_UP_ITEM);
            break;
        case KEY_LEFT:
            item = current_item(m);
            if (item_index(item) <= 0) {
                return MENU_DRIVER_EXIT_UP;
            }
            menu_driver(m, REQ_LEFT_ITEM);
            break;
        case KEY_RIGHT:
            item = current_item(m);
            if (item_index(item) >= item_count(m)-1) {
                return MENU_DRIVER_EXIT_DOWN;
            }
            menu_driver(m, REQ_RIGHT_ITEM);
            break;
        case ENTER_CHAR:
        case KEY_ENTER:
            *current = current_item(m);
            return MENU_DRIVER_SELECTED;
        default:
            // Nothing to actually do if the user presses something
            // unrecognised.
            break;
    }
    return 0;
}

int form_handler(FORM *f, int ch) {
    FIELD *field;

    switch(ch) {
        case KEY_ENTER:
        case KEY_DOWN:
            field = current_field(f);
            if (field_index(field) == field_count(f)-1) {
                return FORM_DRIVER_EXIT_DOWN;
            }
            form_driver(f, REQ_NEXT_FIELD);
            break;
        case KEY_UP:
            field = current_field(f);
            if (field_index(field) == 0) {
                return FORM_DRIVER_EXIT_UP;
            }
            form_driver(f, REQ_PREV_FIELD);
            break;
        case KEY_LEFT:
            form_driver(f, REQ_LEFT_CHAR);
            break;
        case KEY_RIGHT:
            form_driver(f, REQ_RIGHT_CHAR);
            break;
        case BACKSPACE_CHAR:
        case KEY_BACKSPACE:
            form_driver(f, REQ_DEL_PREV);
            break;
        case KEY_DC:
            form_driver(f, REQ_DEL_CHAR);
            break;
        default:
            // Assume it is an alphanumeric character and just pass it to the
            // form to handle directly.
            form_driver(f, ch);
            break;
    }
    return 0;
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

    // Create main menu objects
    for (i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
        item_names[i] = (char*)calloc(name_width+1, sizeof(char));
        sprintf(item_names[i], format, main_menu_choices[i]);
        menu_items[i] = new_item(item_names[i], main_menu_descriptions[i]);
        set_item_userptr(menu_items[i], (void*)&main_menu_item_data[i]);
    }
    menu_items[MAIN_MENU_ITEM_COUNT] = NULL;
    menu = new_menu(menu_items);
    menu_window = newwin(5, width, starty + 2, startx);
    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 0, 0, 1, 1));
    init_menu(menu, menu_window);
    box(menu_window, 0, 0);
    activate_menu(menu);
    post_menu(menu);

    mvprintw(starty, (COLS-strlen(TITLE_TEXT))/2,
            TITLE_TEXT);

    refresh();
    wrefresh(menu_window);

    // User input loop
    should_quit = 0;
    return_val = -1;
    while (should_quit != 1 ) {
        c = wgetch(menu_window);
        switch (menu_handler(menu, c, &cur_item)) {
            case -1:
                should_quit = 1;
                break;
            case MENU_DRIVER_SELECTED:
                if (cur_item == NULL) {
                    break;
                }
                return_val = *(int*)item_userptr(cur_item);
                should_quit = 1;
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

int new_game_menu(Pet **pet) {
    WINDOW *menu_window;
    MENU *menu;
    ITEM *menu_items[NEW_GAME_MENU_ITEM_COUNT+1];
    ITEM *cur_item;

    WINDOW *form_window;
    FORM *form;
    FIELD *fields[2] = {NULL, NULL};

    int i;
    int c;
    int startx, starty;
    int width;

    int name_width;
    char format[10];
    char *item_names[NEW_GAME_MENU_ITEM_COUNT];

    int should_quit;
    int return_val;
    int on_menu = 0;

    char* temp_pet_name;

    wclear(stdscr);

    startx = COLS/10;
    starty = (LINES-9)/2;
    width = COLS-(startx*2);
    name_width = (width/2)-4;
    sprintf(format, "%%-%ds", name_width);

    // User action menu
    for (i = 0; i < NEW_GAME_MENU_ITEM_COUNT; i++) {
        item_names[i] = (char*)calloc(name_width+1, sizeof(char));
        sprintf(item_names[i], format, new_game_menu_choices[i]);
        menu_items[i] = new_item(item_names[i], new_game_menu_descriptions[i]);
        set_item_userptr(menu_items[i], (void*)&new_game_menu_item_data[i]);
    }
    menu_items[NEW_GAME_MENU_ITEM_COUNT] = NULL;
    menu = new_menu(menu_items);
    menu_window = newwin(7, width, starty+2, startx);
    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 0, 0, 5, 1));
    set_menu_format(menu, 1, 2);
    init_menu(menu, menu_window);
    box(menu_window, 0, 0);
    deactivate_menu(menu);
    post_menu(menu);

    // Pet atttributes input form
    fields[0] = new_field(1, width-6, 2, 0, 0, 0);
    fields[1] = NULL;
    set_field_back(fields[0], A_UNDERLINE);
    field_opts_off(fields[0], O_AUTOSKIP);
    set_field_buffer(fields[0], 0, "Matthew");
    form = new_form(fields);
    form_window = newwin(3, width-5, starty+3, startx+3);
    set_form_win(form, form_window);
    set_form_sub(form, derwin(form_window, 0, 0, 0, 0));
    activate_form(form);
    post_form(form);

    mvprintw(starty, (COLS-strlen(NEW_GAME_MENU_TEXT))/2,
            NEW_GAME_MENU_TEXT);
    mvwprintw(form_window, 0, 0, "Pet Name:");

    refresh();
    wrefresh(menu_window);
    wrefresh(form_window);

    should_quit = 0;
    return_val = -1;
    on_menu = 0;
    while (should_quit != 1 ) {
        wrefresh(menu_window);
        wrefresh(form_window);
        mvwprintw(form_window, 0, 0, "Pet Name:");

        c = wgetch(menu_window);
        if (on_menu) {
            switch (menu_handler(menu, c, &cur_item)) {
                case -1:
                    should_quit = 1;
                    break;
                case MENU_DRIVER_SELECTED:
                    if (cur_item == NULL) {
                        break;
                    }
                    activate_form(form);
                    form_driver(form, REQ_NEXT_FIELD);
                    form_driver(form, REQ_PREV_FIELD);

                    // Note it would be easier to directly construct a pet
                    // object here.
                    return_val = *(int*)item_userptr(cur_item);
                    temp_pet_name = field_buffer(fields[0], 0);
                    trim_spaces(temp_pet_name);
                    *pet = create_pet(temp_pet_name);
                    should_quit = 1;
                    break;
                case MENU_DRIVER_EXIT_UP:
                    deactivate_menu(menu);
                    activate_form(form);
                    on_menu = 0;
                    break;
            }
            pos_menu_cursor(menu);
        } else {
            switch (form_handler(form, c)) {
                case -1:
                    should_quit = 1;
                    break;
                case FORM_DRIVER_EXIT_DOWN:
                    deactivate_form(form);
                    activate_menu(menu);
                    on_menu = 1;
                    break;
            }
            pos_form_cursor(form);
        }
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
    assert(free_field(fields[0]) == E_OK);
    endwin();
    return return_val;
}

int load_game_menu(Config *config, Save **save) {
    WINDOW *menu_window;
    MENU *menu;
    ITEM **menu_items;
    ITEM *cur_item;

    Save **saves, **save_iter;
    int save_count;

    int i;
    int c;
    int startx, starty;
    int width;

    int name_width;
    char format[10];

    int should_quit;
    int return_val;

    saves = save_files(config->save_dir);

    wclear(stdscr);
    startx = COLS/10;
    starty = (LINES-9)/2;
    width = COLS-(startx*2);
    name_width = (width/2)-4;
    sprintf(format, "%%-%ds", name_width);

    // Count the number of save files so we can allocate enough space in the
    // menu item list.
    for (save_iter = saves; *save_iter; save_iter++);
    save_count = save_iter - saves;
    menu_items = (ITEM**)calloc(save_count+2, sizeof(ITEM*));
    save_iter = saves;
    for (i = 0; i < save_count; i++, save_iter++) {
        menu_items[i] = new_item((*save_iter)->name,
                new_game_menu_descriptions[i]);
    }
    menu_items[i] = new_item(LOAD_GAME_MENU_OPTION_EXIT_TEXT,
            LOAD_GAME_MENU_OPTION_EXIT_DESCRIPTION);
    i++;
    menu_items[i] = NULL;

    menu = new_menu(menu_items);
    menu_window = newwin(7, width, starty+2, startx);
    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, 0, 0, 1, 1));
    set_menu_format(menu, 5, 1);
    init_menu(menu, menu_window);
    box(menu_window, 0, 0);
    deactivate_menu(menu);
    post_menu(menu);

    mvprintw(starty, (COLS-strlen(LOAD_GAME_MENU_TEXT))/2,
            LOAD_GAME_MENU_TEXT);

    refresh();
    wrefresh(menu_window);

    should_quit = 0;
    return_val = -1;
    while (should_quit != 1 ) {
        wrefresh(menu_window);
        c = wgetch(menu_window);
        switch (menu_handler(menu, c, &cur_item)) {
            case -1:
                should_quit = 1;
                break;
            case MENU_DRIVER_SELECTED:
                if (cur_item == NULL) {
                    break;
                }

                cur_item = current_item(menu);
                i = item_index(cur_item);
                should_quit = 1;
                if (i < save_count) {
                    return_val = LOAD_GAME_MENU_OPTION_START;
                    copy_save(save, saves[i]);
                    load_pet(*save);
                } else if (i == save_count) {
                    return_val = LOAD_GAME_MENU_OPTION_EXIT;
                }
                break;
        }
        pos_menu_cursor(menu);
    }

EXIT:
    free_save_files(saves);
    unpost_menu(menu);
    free_menu(menu);
    for (i = 0; i < save_count+1; i++) {
        free_item(menu_items[i]);
    }
    free(menu_items);
    endwin();
    return return_val;
}
