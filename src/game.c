#include "termpet.h"
#include "termpet_ui.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

int handle_pet_window(WINDOW *w, Pet *pet) {
    int x, y;
    getmaxyx(w, y, x);
    x = (x - 7) / 2;
    y = (y - 3) / 2;

    werase(w);
    if (!pet->is_dead) {
        mvwprintw(w, y++, x, "-o   o-");
        mvwprintw(w, y++, x, "   |   ");
        mvwprintw(w, y++, x, " -___- ");
    } else {
        mvwprintw(w, y++, x, "-o   o-");
        mvwprintw(w, y++, x, "   |   ");
        mvwprintw(w, y++, x, " _---_ ");
    }
    box(w, 0, 0);
    wrefresh(w);
}

void handle_stat_window(WINDOW *w, Pet *pet) {
    werase(w);
    mvwprintw(w, 1, 2, "Pet Stats:");
    mvwprintw(w, 3, 2, "Age: %d", pet->age);
    mvwprintw(w, 4, 2, "Hunger: %0.2f", pet->current_hunger);
    mvwprintw(w, 5, 2, "Happiness: %0.2f", pet->current_happiness);
    mvwprintw(w, 6, 2, "Health %d / %d", pet->health, pet->max_health);
    mvwprintw(w, 7, 2, "Is Sick: %d", pet->is_sick);
    mvwprintw(w, 8, 2, "Is Dead: %d", pet->is_dead);
    box(w, 0, 0);
    wrefresh(w);
}

int handle_action_window(int c, MENU* m, WINDOW *w, Pet *pet) {
    ITEM *current;
    if (menu_handler(m, c, &current) == MENU_DRIVER_SELECTED) {
        return *(int*)item_userptr(current);
    }
    box(w, 0, 0);
    wrefresh(w);
    return -1;
}

int game_loop(Config *config, Pet *pet) {
    int should_quit;
    int return_val;
    char title_text[255];

    int title_h = 3;
    int padding = 1;

    WINDOW *stat_window = NULL;
    int stat_x, stat_y;
    int stat_w, stat_h;

    WINDOW *action_window = NULL;
    int action_x, action_y;
    int action_w, action_h;

    WINDOW *pet_window = NULL;
    int pet_x, pet_y;
    int pet_w, pet_h;

    MENU *menu;
    ITEM *menu_items[GAME_LOOP_MENU_ITEM_COUNT+1];
    ITEM *cur_item;
    int name_width;
    char format[10];
    char *item_names[GAME_LOOP_MENU_ITEM_COUNT];

    int i, c;

    return_val = -1;

    erase();
    curs_set(0);

    sprintf(title_text, GAME_LOOP_TITLE_FORMAT, pet->name);
    mvprintw(2, (COLS-strlen(title_text))/2,
            title_text);

    // Create the side panel
    stat_x = (COLS * 0.6);
    stat_y = title_h + padding;
    stat_w = (COLS * 0.4) - (padding * 2);
    stat_h = (LINES - title_h - padding - 1) / 2;
    stat_window = newwin(stat_h, stat_w, stat_y, stat_x);
    if (stat_window == NULL) {
        goto EXIT;
    }
    box(stat_window, 0, 0);

    // Create the user action menu
    action_x = (COLS * 0.6);
    action_y = stat_y + stat_h + padding;
    action_w = (COLS * 0.4) - (padding * 2);
    action_h = (LINES - title_h - padding) / 2;
    action_window = newwin(action_h, action_w, action_y, action_x);
    if (action_window == NULL) {
        goto EXIT;
    }
    box(action_window, 0, 0);

    // Create the user action menu
    pet_x = padding;
    pet_y = title_h + padding;
    pet_w = (COLS * 0.6) - (padding * 2);
    pet_h = LINES - title_h - padding;
    pet_window = newwin(pet_h, pet_w, pet_y, pet_x);
    if (pet_window == NULL) {
        goto EXIT;
    }

    name_width = action_w - 4;
    sprintf(format, "%%-%ds", name_width);
    for (i = 0; i < GAME_LOOP_MENU_ITEM_COUNT; i++) {
        item_names[i] = (char*)calloc(name_width+1, sizeof(char));
        sprintf(item_names[i], format, game_loop_menu_choices[i]);
        menu_items[i] = new_item(item_names[i], game_loop_menu_descriptions[i]);
        set_item_userptr(menu_items[i], (void*)&game_loop_menu_item_data[i]);
    }
    menu_items[GAME_LOOP_MENU_ITEM_COUNT] = NULL;
    menu = new_menu(menu_items);
    set_menu_win(menu, action_window);
    set_menu_sub(menu, derwin(action_window, 0, 0, 1, 1));
    set_menu_format(menu, 0, 0);
    init_menu(menu, action_window);
    post_menu(menu);

    // User input loop
    should_quit = 0;
    while (should_quit != 1 ) {
        c = getch();
        if (c == 'q') {
            return_val = 0;
            goto EXIT;
        }
        update_pet(pet);
        handle_stat_window(stat_window, pet);
        handle_pet_window(pet_window, pet);
        switch (handle_action_window(c, menu, action_window, pet)) {
            case -1:
                break;
            case GAME_LOOP_MENU_OPTION_FEED:
                feed_pet(pet);
                break;
            case GAME_LOOP_MENU_OPTION_PLAY:
                play_with_pet(pet);
                break;
            case GAME_LOOP_MENU_OPTION_MEDICINE:
                give_medicine_to_pet(pet);
                break;
            case GAME_LOOP_MENU_OPTION_EXIT:
                should_quit = 1;
                return_val = 0;
                break;
        }
    }

EXIT:
    unpost_menu(menu);
    free_menu(menu);
    for (i = 0; i < GAME_LOOP_MENU_ITEM_COUNT; i++) {
        free_item(menu_items[i]);
        free(item_names[i]);
    }
    endwin();
    return return_val;
}


