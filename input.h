#include <stddef.h>

typedef struct {
    char *buff;
    size_t buff_length;
    size_t buff_length_max;
    unsigned int cursor_position_x;
    unsigned int cursor_position_y;
} Input;

int input_cursormto_left(Input *input);
int input_cursormto_right(Input *input);
void input_del(Input *input);
void input_write(Input *input, char c);
void input_print(Input *input, int start_c, int start_r, int lineWidht);
Input *new_input(void);
void clearInput(Input *input);
void input_free(Input *input);