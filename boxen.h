#include <stdint.h>
#include <wchar.h>
#ifndef _STRING_H
    #include <string.h>
#endif

struct Padding {
  unsigned int top;
  unsigned int right;
  unsigned int bottom;
  unsigned int left;
};
/*
union Padding {
    int padding;
    struct padding_struct padding_struct;
};*/

#define TITLE_LEFT 0x00
#define TITLE_CENTER 0x01
#define TITLE_RIGHT 0x02

#define CONTENT_LEFT 0x00
#define CONTENT_CENTER 0x10
#define CONTENT_RIGHT 0x30

typedef  struct {
    int width;
    int height;
    int position_x;
    int position_y;
    uint8_t content_position;
    struct Padding *padding;
    struct Corners *corners;
    const char *title;
    
} Box;
struct Corners {
  const char *left_uc;
  const char *right_uc;
  const char *right_dc;
  const char *left_dc;
  const char *line_up;
  const char *line_do;
};

void drawBox(char *content, Box *box_attr);

struct text_wrap *strbox(const char *s, int s_len, int w_len);
