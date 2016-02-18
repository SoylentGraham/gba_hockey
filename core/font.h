#ifndef CORE_FONT_H
#define CORE_FONT_H




typedef struct {
	byte 
		char_id:6,
		extra_data: 2;
} a_fontchar;



#define CHAR_LOWEST 32
#define CHAR_HIGHEST 95

#define ALPHABET_LENGTH 48
#define BLANK_CHAR (unsigned char)' '

unsigned char alphabet[ALPHABET_LENGTH];


byte get_font_tile(char letter, byte extra_data);
a_fontchar get_fontchar(char the_char, byte extra_data);
void draw_text(u16 x, u16 y, u8 wrap, char *fmt, ...);
void draw_string(u16 x, u16 y, u8 wrap, char *string);








#endif

