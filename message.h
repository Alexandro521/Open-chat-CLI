#include <sys/types.h>
typedef struct {
     char *username;
     char *color;
    char *text;
} Msg_t;

typedef struct {
    Msg_t **buffer;
    uint buffer_index;
    uint dynamic_index;
    uint buffer_size;
    uint row_count;
} MsgInstance_t;

MsgInstance_t *newMessageBuff(void);
int reallocMessageBuff(MsgInstance_t *buffer);
int freeMessageBuff(MsgInstance_t *buffer);
Msg_t *newMessage(const char *username, char *body, char *color);
int storeMessage(Msg_t *message, MsgInstance_t *buff);
int scrollup_buffer(int n, MsgInstance_t *buffer);
int scrolldown_buffer(int n, MsgInstance_t *buffer);
int resetscroll_buffer(MsgInstance_t *buffer, int h);
int clear_buffer(MsgInstance_t *buffer);
int printall_msg(MsgInstance_t *buffer, int screenHeight, int start_c);
int print_msg(MsgInstance_t *buffer, int index, int win_x, int win_y);



