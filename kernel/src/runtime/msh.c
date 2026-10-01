#include "minemu/uart.h"

#define MAX_LINE 20
#define MAX_TOKENS 10

static char line_buf[MAX_LINE + 1];

static char strcmp(const char* str1, const char* str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2)
            return 0;
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

static void msh_read() {
    char c;
    uint32_t length = 0;

    while(1) {
        c = uart_buf_pop();

        if (c == '\0')
            continue;

        if (c == '\n')
            break;

        if (c == 0x08 || c == 0x7f) {
            if (length > 0)
                length--;
            continue;
        }

        if (length < MAX_LINE)
            line_buf[length++] = c;
    }

    line_buf[length] = '\0';
}

static void msh_process() {
    //minemu_printf(line_buf);

    uint32_t i = 0;
    char* tokens[MAX_TOKENS];
    char* token = line_buf;
    while (*token != '\0' && i < MAX_TOKENS) {
        while (*token == ' ')
            token++;
        if (*token == '\0')
            break;

        tokens[i++] = token;
        
        while (*token != ' ' && *token != '\0')
            token++;
        if (*token == ' ') {
            *token = '\0';
            token++;
        }
    }

    if (i == 0)
        return;

    if (strcmp(tokens[0], "echo")) {
        for (uint32_t j = 1; j < i; j++) {
            if (j != 1)
                minemu_printf(" ");
            minemu_printf(tokens[j]);
        }
        minemu_printf("\n");
    } else {
        minemu_printf("command not found: ");
        minemu_printf(tokens[0]);
        minemu_printf("\n");
    }
}

void run_msh() {
    while (1) {
        minemu_printf("msh> ");
        msh_read();
        msh_process();
    }
}