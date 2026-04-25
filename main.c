#include <asm-generic/ioctls.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <signal.h>
#include "boxen.h"
#include "ansi.h"
#include "input.h"
#include "message.h"


static struct termios origin_term;
static struct winsize screenSize; 
static Input *text_input;
static Box box_container, box_input;
static MsgInstance_t *message_buffer;
static struct Padding padding = {0,0,0,0};
static struct Corners
box_container_corners = {"╔", "╗", "╝", "╚", "═", "═"},
box_input_corners = {"╟", "╢", "╝", "╚", "─", "═"};

void closeRawMode(void) {
    tcsetattr(STDERR_FILENO, TCSAFLUSH, &origin_term);
}
void openRawMode(int isread ){
    tcgetattr(STDIN_FILENO, &origin_term);
    atexit(closeRawMode);
    struct termios raw = origin_term;
    raw.c_iflag &= ~( INPCK | ISTRIP | BRKINT);
    raw.c_oflag &= OPOST;
    raw.c_lflag &= ~(ISIG | ICANON | IEXTEN | ECHO) | ECHOE;
    if(isread == 1){
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] =1;
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
void printUi() {
	printf(CURSOR_MT_ORI CLEAR_FROM_CURSOR_ES);
	drawBox("", &box_container);
	drawBox("", &box_input);
	printf(cursormto, 2, 2);
    printall_msg(message_buffer, screenSize.ws_row -4, 2);
	input_print(text_input , 2, box_input.position_y +1, box_input.width);
    //printf(cursormto "%d", screenSize.ws_row/2, screenSize.ws_col/2,message_buffer->dynamic_index);
}
void screeChHandle(int n) {
	ioctl(STDOUT_FILENO,TIOCGWINSZ, &screenSize);
	box_container.width = screenSize.ws_col;
	box_container.height = screenSize.ws_row -2;
	box_input.width = screenSize.ws_col;
	box_input.position_y = screenSize.ws_row -2;
	printUi();
}

int main() {
	int rowcount = 0;
    /* get initial window size*/
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &screenSize);
    box_container.position_x = 1;
    box_container.position_y = 1;
    box_container.width = screenSize.ws_col;
    box_container.height = screenSize.ws_row -2;
    box_container.title = " Open Chat CLI V 1.0 ";
    box_container.content_position = TITLE_CENTER | CONTENT_LEFT;
    box_container.corners = &box_container_corners;
    box_container.padding = &padding;
    
    box_input = box_container;
    box_input.height = 1;
    box_input.content_position = TITLE_LEFT;
    box_input.position_y =  screenSize.ws_row - 2;
    box_input.corners = &box_input_corners;
    box_input.title = "";//"┤@Alexdev011├";
    
    setvbuf(stdout, NULL, _IONBF, 0);
    openRawMode(0);
    signal(SIGWINCH, screeChHandle);
    text_input = new_input();
    message_buffer = newMessageBuff();
    if(message_buffer == NULL) {
   		printf("Errorr");
     	exit(EXIT_FAILURE);
    }
    
    char c;
    //message_buffer = newMessageBuff();
    printUi();
    while(read(STDIN_FILENO, &c, 1)){
       if(c == *ESC){
           char buff[2];
           read(STDIN_FILENO, &buff, 2);
           switch (buff[1]) {
               case 'C':
                    if(input_cursormto_right(text_input))
                    	printf(CURSOR_RI_LINE);
                    break;
               case 'D':
                    if(input_cursormto_left(text_input))
                    	printf(CURSOR_LE_LINE);
                    break;
                case 'A':
              		if(scrollup_buffer(1, message_buffer)){
              			printUi();
                	}
                    break;
                case 'B':
               		if(scrolldown_buffer(1, message_buffer)){
                		printUi();
                 		};
                	break;
           }
       }
       else if(c == DEL){
           input_del(text_input);
           input_print(text_input , 2, box_input.position_y +1,box_input.width );
       }
       else if(c == 0x3){ //CTRL + C
           break;
       }
       else if(c == '\n'){
           if(text_input->buff_length == 0) continue;
           if(message_buffer->buffer_index - message_buffer->dynamic_index >= screenSize.ws_row-4 ){
         		resetscroll_buffer(message_buffer, screenSize.ws_row-4);
           } 
           if(message_buffer->row_count >= screenSize.ws_row-4 ){
         		scrolldown_buffer(1, message_buffer);
           }
           Msg_t *msg = newMessage("Alexdev", text_input->buff, RED);
           clearInput(text_input);
           storeMessage(msg, message_buffer);
           printUi();
       }
       else {
           input_write(text_input, c);
           input_print(text_input , 2, box_input.position_y +1, box_input.width );
       }
       /*Temporal */
    }
    input_free(text_input);
    return 0;
}
