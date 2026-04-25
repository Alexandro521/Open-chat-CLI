#include "strutil.h"
#include "string.h"
#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

Wrap *strwrap(char *s, int width) {
	size_t slen =  strlen(s);
	uint tmplines = ceil((double) slen / width );
	if(slen <= 1  || tmplines <= 1 || slen < width) 
		return NULL;
	
	Wrap *wrap = (Wrap *) malloc(sizeof( Wrap ));
	wrap->length = 0;
	wrap->line = (char **) malloc(tmplines * sizeof(char *));
	char **linep = wrap->line;
	uint startIndex, endIndex;
	startIndex = 0;
	endIndex = width;
	
	while(startIndex < slen) {
		if(linep >= &wrap->line[tmplines]){
		//	printf("mem out \n");
			wrap->line =  realloc(wrap->line, (tmplines + 8) * sizeof(char *));
			if(wrap->line != NULL) {
				linep = &wrap->line[tmplines];
				tmplines+= 8;
			}else {
				printf("memory err: strwrso Line 25:43");
				exit(EXIT_FAILURE);
			}
		} 
		int index = endIndex;
		int start = startIndex;
		char c = s[index];
		while(isspace(s[start])) start ++;
		while(c != ' ' && index > start && endIndex < slen) {
			c = s[--index];
		}
		if(index <= start ) {
			index =  endIndex;
		}
		int sublen = index - start;
		*linep = (char * ) malloc((sublen +1) * sizeof(char));
		strncpy(*linep, s+start , sublen);
		(*linep)[sublen]  = '\0';
		wrap->length++;
		linep++;
		endIndex = index;
		startIndex = endIndex;
		endIndex+= (slen -  endIndex) < width ? (slen -  endIndex) : width;
	}
	return wrap;
}
void free_wrap(Wrap *wrap) {
	for(int i = 0; i < wrap->length; i++){
		free(wrap->line[i]);
	}
	free(wrap->line);
	free(wrap);
}
