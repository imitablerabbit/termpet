#include "termpet.h"

#include <string.h>

int trim_newlines(char *s) {
    int i;
    int len;

    len = strlen(s);
    for (i = len-1; i > 0; i--) {
        if (s[i] != '\n') {
            break;
        }
    }
    i++;
    s[i] = '\0';
    return len - i;
}

int trim_spaces(char *s) {
    int i;
    int len;

    len = strlen(s);
    for (i = len-1; i > 0; i--) {
        if (s[i] != ' ') {
            break;
        }
    }
    i++;
    s[i] = '\0';
    return len - i;
}
