#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLS 100

void init_screen();
void handle_input(char text[MAX_ROWS][MAX_COLS], int *row, int *col, char *filename);
void draw_text(char text[MAX_ROWS][MAX_COLS]);
void save_file(char text[MAX_ROWS][MAX_COLS], const char *filename);
void prompt_filename(char *filename);
void print_help();

int main(int argc, char *argv[]) {
    char text[MAX_ROWS][MAX_COLS] = {0};
    int row = 0, col = 0;
    char *filename = argv[1];

    init_screen();


    while (1) {
        draw_text(text);
        move(row, col);
        refresh();
        handle_input(text, &row, &col, filename);

    }

    endwin();
    return 0;
}

void init_screen() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
}

void handle_input(char text[MAX_ROWS][MAX_COLS], int *row, int *col, char *filename) {
    int ch = getch();

    switch (ch) {
        case KEY_BACKSPACE:
        case 127:  // Handle both backspace and delete keys
            if (*col > 0) {
                (*col)--;
                text[*row][*col] = ' ';
            } else if (*row > 0) {
                (*row)--;
                *col = strlen(text[*row]);
                text[*row][*col] = ' ';
            }
            break;

        case KEY_LEFT:
            if (*col > 0) (*col)--;
            break;

        case KEY_RIGHT:
            if (*col < MAX_COLS - 1) (*col)++;
            break;

        case KEY_UP:
            if (*row > 0) (*row)--;
            break;

        case KEY_DOWN:
            if (*row < MAX_ROWS - 1) (*row)++;
            break;

        case '\n':
            if (*row < MAX_ROWS - 1) {
                (*row)++;
                *col = 0;
            }
            break;

        // Magic keys
        case 17: // Ctrl+Q to exit
            endwin();
            exit(0);
            break;
        case 19: // Ctrl+S to save
            if (strlen(filename) == 0) {
                prompt_filename(filename);
            }
            save_file(text, filename);
            break;
        case 7: // Ctrl+Shift+? to display help
            print_help();
            break;

        default:
            if (*col < MAX_COLS - 1 && ch >= 32 && ch <= 126) {  // Printable characters
                text[*row][*col] = ch;
                (*col)++;
            }
            break;
    }
}

void save_file(char text[MAX_ROWS][MAX_COLS], const char *filename) {

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        mvprintw(MAX_ROWS, 0, "Error: Could not save file.");
        return;
    }

    for (int i = 0; i < MAX_ROWS; i++) {
        fprintf(file, "%s\n", text[i]);
    }

    fclose(file);
    mvprintw(MAX_ROWS, 0, "File saved as '%s'.", filename);
}

void prompt_filename(char *filename) {
    echo();
    curs_set(1);
    mvprintw(MAX_ROWS, 0, "Enter filename: ");
    getstr(filename);
    noecho();
    curs_set(1);
    clear();
}

void draw_text(char text[MAX_ROWS][MAX_COLS]) {
    clear();
    for (int i = 0; i < MAX_ROWS; i++) {
        mvprintw(i, 0, "%s", text[i]);
    }
}

void print_help() {
    clear();
    mvprintw(0, 0, "Special Key Combinations:");
    mvprintw(1, 0, "Ctrl+Q  : Exit the program");
    mvprintw(2, 0, "Ctrl+S  : Save the current text");
    mvprintw(3, 0, "Ctrl+Shift+? : Display this help message");
    mvprintw(4, 0, "Press any key to return to the editor...");
    refresh();
    getch();  // Wait for user input before returning
    clear();
}
