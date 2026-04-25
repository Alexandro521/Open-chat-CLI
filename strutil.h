struct wrap {
    char **line;
    unsigned int length;
};
typedef struct wrap  Wrap;

Wrap *strwrap(char *s, int maxwidth);
void free_wrap(Wrap *wrap);