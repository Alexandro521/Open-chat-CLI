//[ANSI ESCAPES]
#define CSI "\x9B"
#define DCS "\x90"
#define OSC "\x9D"
#define ESC "\x1B"
#define BS '\b'
#define CR '\r'
#define HT '\t'
#define VT '\v'
#define FF '\f'
#define LF '\n'
#define DEL 0x7F
#define ANSI_OPEN ESC "["
#define ANSI_CLOSE ANSI_OPEN

//[ANSI MODES]
#define ANSI_STYLE_RESET ANSI_OPEN "0m"

//[ANSI COLOR]
#define BLACK  "30"
#define RED    "31"
#define GREEN  "32"
#define YELLOW "33"
#define BLUE   "34"
#define MAGENT "35"
#define CYAN   "36"
#define WHITE  "37"
#define DEFAULT_COLOR "39"

#define BG_BLACK  "40"
#define BG_RED    "41"
#define BG_GREEN  "42"
#define BG_YELLOW "43"
#define BG_BLUE   "44"
#define BG_MAGENT "45"
#define BG_CYAN   "46"
#define BG_WHITE  "47"
#define BG_DEFAULT_COLOR "49"

#define BRIGHT_BLACK  "90"
#define BRIGHT_RED    "91"
#define BRIGHT_GREEN  "92"
#define BRIGHT_YELLOW "93"
#define BRIGHT_BLUE   "94"
#define BRIGHT_MAGENT "95"
#define BRIGHT_CYAN   "96"
#define BRIGHT_WHITE  "97"

#define BG_BRIGHT_BLACK  "100"
#define BG_BRIGHT_RED    "101"
#define BG_BRIGHT_GREEN  "102"
#define BG_BRIGHT_YELLOW "103"
#define BG_BRIGHT_BLUE   "104"
#define BG_BRIGHT_MAGENT "105"
#define BG_BRIGHT_CYAN   "106"
#define BG_BRIGHT_WHITE  "107"
#define UNDERLINE ";4"
#define strcolor(color_code,str, mode) \
ANSI_OPEN  color_code  mode  "m" \
 str "" ANSI_STYLE_RESET

//[CURSOR]

#define cursorup        ANSI_OPEN "%dA"
#define cursordown      ANSI_OPEN "%dB"
#define cursoright      ANSI_OPEN "%dC"
#define cursorleft      ANSI_OPEN "%dD"
#define cursornext_n    ANSI_OPEN "%dE"
#define cursorprev_n    ANSI_OPEN "%dF"
#define cursorto_c      ANSI_OPEN "%dG"
#define cursormto       ANSI_OPEN "%d;%dH"
#define CURSOR_MT_ORI   ANSI_OPEN "H"
#define SCROLL_UP  ESC "M"
#define CURSOR_UP_LINE  ANSI_OPEN "1A"
#define CURSOR_DO_LINE  ANSI_OPEN "1B"
#define CURSOR_RI_LINE  ANSI_OPEN "1C"
#define CURSOR_LE_LINE  ANSI_OPEN "1D"
#define CURSOR_SAV_PD   ESC "7"  //DEC
#define CURSOR_RES_PD   ESC "8" //DEC
#define CURSOR_SAV_PS   ANSI_OPEN "s" //SCO
#define CURSOR_RES_PS   ANSI_OPEN "U" //SCO

//[ERASE FUNCTIONS]

#define CLEAR_VIEW_SCREEN     ANSI_OPEN "J"
#define CLEAR_FROM_CURSOR_ES  ANSI_OPEN "0J" //from cursor to end screen
#define CLEAR_FROM_CURSOR_SS  ANSI_OPEN "1J" //from cursor to start screen
#define CLEAR_ALL_SCREEN      ANSI_OPEN "2J"
#define CLEAR_SAVE_LINES      ANSI_OPEN "3J"
#define CLEAR_LINE            ANSI_OPEN "K"
#define CLEAR_FROM_CURSOR_EL  ANSI_OPEN "0K" //from cursor to end line
#define CLEAR_FROM_CURSOR_SL  ANSI_OPEN "1K" //from start line to cursor
#define CLEAR_ALL_LINE        ANSI_OPEN "2K"

#define TERM_SIZE ANSI_OPEN "18t"
