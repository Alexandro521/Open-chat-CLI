#include "message.h"
#include <stddef.h>
#include <stdio.h>
#include "ansi.h"
#include <stdlib.h>
#include <string.h>

MsgInstance_t *newMessageBuff(void) {
	MsgInstance_t *temp = (MsgInstance_t *) malloc(sizeof(MsgInstance_t));
	if(temp == NULL) 
		return NULL;
	temp->buffer = (Msg_t **) malloc(1024 * sizeof(Msg_t *));
	if(temp->buffer == NULL) {
		free(temp);
		return  NULL;
	}
	temp->buffer_size = 1024;
	temp->buffer_index = 0;
	temp->dynamic_index = 0;
	return temp;
};

int reallocMessageBuff(MsgInstance_t *buffer);
int freeMessageBuff(MsgInstance_t *buffer);
int freeMessage(Msg_t *msg);
Msg_t *newMessage(const char *username, char *body, char *color) {
	Msg_t *temp = (Msg_t *) malloc(sizeof(Msg_t));
	if(temp ==  NULL) return  NULL;
	unsigned short int char_s = sizeof(char);
	if(( temp->username = (char *) malloc((strlen(username) + 1) * char_s)) == NULL){
		free(temp);
		return NULL;
	}
	if((temp->text = (char *) malloc((strlen(body) + 1 ) * char_s)) == NULL) {
		free(temp->username);
		free(temp);
		return NULL;
	};
	if((temp->color = (char *) malloc((strlen(color) + 1) * char_s)) ==  NULL) {
		free(temp->username);
		free(temp->text);
		free(temp);
		return NULL;
	};
	
	if(
	strcpy(temp->username, username) == NULL ||
	strcpy(temp->text, body) == NULL ||
	strcpy(temp->color, color) == NULL
	) {
	//	freeMessage(temp);
		return NULL;
	};
	return temp;
};
int storeMessage(Msg_t *message, MsgInstance_t *buff) {
	if(buff->buffer_index < buff->buffer_size) {
		buff->buffer[buff->buffer_index++] = message;
	}else {
		return  0;
	}
	return  1;
};

int printall_msg(MsgInstance_t *buffer, int screenHeight, int start_c) {
	buffer->row_count = 0;
	for(int i = buffer->dynamic_index;
	 i < buffer->dynamic_index + screenHeight &&
	 i < buffer->buffer_index;
	 i++)
	{	
		buffer->row_count++;
		printf(
			cursorto_c "" strcolor("%s" ,"%s ", UNDERLINE) "%s" CURSOR_DO_LINE,
		 	start_c,
			buffer->buffer[i]->color,
			buffer->buffer[i]->username,
			buffer->buffer[i]->text
		);
	}
	return 1;
};

int print_msg(MsgInstance_t *buffer, int index, int win_x, int win_y){
	buffer->row_count = 0;
	if(index < buffer->buffer_index){		
		printf(
			cursormto "" strcolor("%s" ,"%s ", UNDERLINE) "%s" CURSOR_DO_LINE,
		 	win_y,
			win_x,
			buffer->buffer[index]->color,
			buffer->buffer[index]->username,
			buffer->buffer[index]->text
		);
		buffer->row_count++;
	}
	return 1;
}

int scrollup_buffer(int n, MsgInstance_t *buffer){
	if(buffer->dynamic_index - n >= 0 && buffer->dynamic_index > 0){
		buffer->dynamic_index -= n;
		return 1;
	}
	return 0;
}

int scrolldown_buffer(int n, MsgInstance_t *buffer){
	if(buffer->dynamic_index + n < buffer->buffer_index) {
		buffer->dynamic_index += n;
		return 1;
	}
	return  0;
}

int resetscroll_buffer(MsgInstance_t *buffer, int h) {
	int diff = buffer->buffer_index < h ? 0 : buffer->buffer_index - h;
	buffer->dynamic_index = diff;
	return 1;
};
int clear_buffer(MsgInstance_t *buffer);