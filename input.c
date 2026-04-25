#include <stdio.h>
#include <stdlib.h>
#include "ansi.h"
#include <string.h>
#include "input.h"


int input_cursormto_left(Input *input){
    if(input->cursor_position_x > 0){
        input->cursor_position_x--;
        return 1;
    }
    return 0;
}

int input_cursormto_right(Input *input){
    if(input->cursor_position_x < input->buff_length){
        input->cursor_position_x++;
        return 1;
    }
    return  0;
}

void input_del(Input *input){
    if(input->cursor_position_x == 0 || input->buff_length == 0)
        return;

   if(input->cursor_position_x >= input->buff_length){
       input->buff[--input->cursor_position_x] = 0;
       input->buff_length--;
   }else{
       char *p = (input->buff + input->cursor_position_x);
       memmove(p-1, p,input->buff_length - input->cursor_position_x +1);
       input->buff[input->buff_length] = 0;
       input->cursor_position_x--;
       input->buff_length--;
   }
}

void input_write(Input *input, char c){
    if(input->cursor_position_x >= input->buff_length){
            input->buff[input->cursor_position_x++] = c;
            input->buff_length++;
    }else{
        char *p = (input->buff + input->cursor_position_x);
        memmove( p+1, p, input->buff_length - input->cursor_position_x +1);
        input->buff[input->cursor_position_x++] = c;
        input->buff_length++;
    }
    /* buffer size out */
    if(input->buff_length >= input->buff_length_max-1){
        input->buff_length_max += 1024;
        input->buff = realloc(input->buff, input->buff_length_max * sizeof(char));
        if(input->buff == NULL){
            fwrite("error on input buffer memory reasignement", sizeof(char),11,stderr);
            exit(EXIT_FAILURE);
        }
    }
}

void input_print(Input *input, int start_c, int start_r, int lineWidht) {
    printf( cursormto CLEAR_ALL_LINE "⦿→ %s" cursorto_c, start_r ,start_c,input->buff, input->cursor_position_x + start_c + 3 );
    printf(CURSOR_SAV_PD cursorto_c "║" cursorto_c "║" CURSOR_RES_PD, start_c-1,start_c + lineWidht);
}

Input *new_input(){
    Input *input = (Input *) malloc(sizeof(Input));
    memset(input, 0, sizeof(Input));
    input->buff_length_max = 1024;
    input->buff = (char *) malloc(input->buff_length_max * sizeof(char *));
    return  input;
}

void input_free(Input *input){
    free(input->buff);
    free(input);
}

void clearInput(Input *input){
	memset(input->buff, 0, input->buff_length);
	input->buff_length = 0;
	input->cursor_position_x = 0;
}