#include "ansi.h"
#include "boxen.h"
#include <stdio.h>
#include "strutil.h"
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

void drawBox(char *content, Box *attr){
    int realwidth = attr->width * 3;
    
    char *header = malloc( ( realwidth+1 ) * sizeof(char));
    
    for(int i = 0; i < realwidth; i+= 3){
        memcpy(header+i, attr->corners->line_up, 3);
    }
    memcpy(header, attr->corners->left_uc, 3);
    memcpy(&header[realwidth-3] , attr->corners->right_uc, 3);
    header[realwidth] = '\0';
    int title_position;
    
    switch (attr->content_position & 0x0F) {
        case TITLE_LEFT:
            title_position = attr->position_x + attr->padding->left +1;
            break;
        case TITLE_CENTER:
            title_position = attr->position_x + (attr->width/2 - strlen(attr->title)/2);
            break;
        case TITLE_RIGHT:
            title_position = (attr->position_x + attr->width) - strlen(attr->title) - attr->padding->right -1;
            break;
    }
    
    /*print header */
    printf(cursormto "%s" cursorto_c "%s" CURSOR_DO_LINE cursorto_c,
        attr->position_y,
        attr->position_x,
        header, title_position,
        attr->title,
        attr->position_x
    );
    
    Wrap *wrapContent = strwrap(content, attr->width - attr->padding->left - attr->padding->right);
    
    int height = attr->height;
    
    if(wrapContent != NULL &&  wrapContent->length > height)
        height = wrapContent->length;
    
    for(int i = 0, line = 0; i < height + (attr->padding->top +  attr->padding->bottom ); i++){

        printf("║" cursorto_c "║\n" cursorto_c, attr->position_x+attr->width-1, attr->position_x );

        if(wrapContent == NULL || i < attr->padding->top || i > (height + attr->padding->top -1)){
            continue;
        }

        int contentPosition;
        switch (attr->content_position & 0xF0) {
            case CONTENT_LEFT:
                contentPosition = attr->position_x + attr->padding->left +1;
                break;
            case CONTENT_CENTER:
                contentPosition = attr->position_x+ (attr->width/2 - strlen(wrapContent->line[line])/2);
                break;
            case CONTENT_RIGHT:
                contentPosition = (attr->position_x + attr->width -1) -  strlen(wrapContent->line[line]) - attr->padding->right;
                break;
        }
        printf(CURSOR_SAV_PD CURSOR_UP_LINE cursorto_c "%s" CURSOR_RES_PD, contentPosition, wrapContent->line[line]);
        line++;
    }
    for(int i = 0; i < realwidth; i+= 3){
        memcpy(header+i, attr->corners->line_do, 3);
    }
    memcpy(header, attr->corners->left_dc, 3);
    memcpy(header + realwidth -3, attr->corners->right_dc, 3);
    printf(cursorto_c "%s",attr->position_x, header);
};
