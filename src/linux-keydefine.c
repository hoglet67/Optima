/*Atomulator v1.0 by Tom Walker
   Linux keyboard redefinition GUI*/

#ifndef WIN32
#include <stdio.h>
#include <allegro.h>
#include <string.h>
#include "atom.h"

int keytemp[128];

char *key_names[] =
{
	"",	      "A",		  "B",		     "C",
	"D",	      "E",		  "F",		     "G",
	"H",	      "I",		  "J",		     "K",
	"L",	      "M",		  "N",		     "O",
	"P",	      "Q",		  "R",		     "S",
	"T",	      "U",		  "V",		     "W",
	"X",	      "Y",		  "Z",		     "0",
	"1",	      "2",		  "3",		     "4",
	"5",	      "6",		  "7",		     "8",
	"9",	      "0_PAD",		  "1_PAD",	     "2_PAD",
	"3_PAD",      "4_PAD",		  "5_PAD",	     "6_PAD",
	"7_PAD",      "8_PAD",		  "9_PAD",	     "F1",
	"F2",	      "F3",		  "F4",		     "F5",
	"F6",	      "F7",		  "F8",		     "F9",
	"F10",	      "F11",		  "F12",	     "ESC",
	"TILDE",      "MINUS",		  "EQUALS",	     "BACKSPACE",
	"TAB",	      "OPENBRACE",	  "CLOSEBRACE",	     "ENTER",
	"COLON",      "QUOTE",		  "BACKSLASH",	     "BACKSLASH2",
	"COMMA",      "STOP",		  "SLASH",	     "SPACE",
	"INSERT",     "DEL",		  "HOME",	     "END",
	"PGUP",	      "PGDN",		  "LEFT",	     "RIGHT",
	"UP",	      "DOWN",		  "SLASH_PAD",	     "ASTERISK",
	"MINUS_PAD",  "PLUS_PAD",	  "DEL_PAD",	     "ENTER_PAD",
	"PRTSCR",     "PAUSE",		  "ABNT_C1",	     "YEN",
	"KANA",	      "CONVERT",	  "NOCONVERT",	     "AT",
	"CIRCUMFLEX", "COLON2",		  "KANJI",	     "EQUALS_PAD",
	"BACKQUOTE",  "SEMICOLON",	  "COMMAND",	     "UNKNOWN1",
	"UNKNOWN2",   "UNKNOWN3",	  "UNKNOWN4",	     "UNKNOWN5",
	"UNKNOWN6",   "UNKNOWN7",	  "UNKNOWN8",	     "LSHIFT",
	"RSHIFT",     "LCONTROL",	  "RCONTROL",	     "ALT",
	"ALTGR",      "LWIN",		  "RWIN",	     "MENU",
	"SCRLOCK",    "NUMLOCK",	  "CAPSLOCK",	     "MAX"
};

int d_getkey(int msg, DIALOG *d, int cd)
{
	BITMAP *b;
	int x, y;
	int ret = d_button_proc(msg, d, cd);
	int k, k2;
	int c;
	char s[1024], s2[1024], s3[64];

	if (ret == D_EXIT)
	{
		k = (int)d->dp2;
		x = (SCREEN_W / 2) - 100;
		y = (SCREEN_H / 2) - 36;
		b = create_bitmap(200, 72);
		blit(screen, b, x, y, 0, 0, 200, 72);
		rectfill(screen, x, y, x + 199, y + 71, makecol(0, 0, 0));
		rect(screen, x, y, x + 199, y + 71, makecol(255, 255, 255));
		if (d->dp3)
			textprintf_ex(screen, font, x + 8, y + 8, makecol(255, 255, 255), 0, "Redefining %s", (char*)d->dp3);
		else
			textprintf_ex(screen, font, x + 8, y + 8, makecol(255, 255, 255), 0, "Redefining %s", (char*)d->dp);
		textprintf_ex(screen, font, x + 8, y + 24, makecol(255, 255, 255), 0, "Assigned to PC key(s) :");

		s[0] = 0;
		for (c = 0; c < 128; c++)
		{
			if (keylookup[c] == k)
			{
				if (s[0])
					sprintf(s3, ", %s", key_names[c]);
				else
					sprintf(s3, "%s", key_names[c]);
				sprintf(s2, "%s%s", s, s3);
				strcpy(s, s2);
			}
		}

		textprintf_ex(screen, font, x + 8, y + 40, makecol(255, 255, 255), 0, s);

		textprintf_ex(screen, font, x + 8, y + 56, makecol(255, 255, 255), 0, "Please press new key...");
 getnewkey:
		while (!keypressed())
			;
		k2 = readkey() >> 8;
		if (k2 == KEY_F11 || k2 == KEY_F12)
			goto getnewkey;
		keylookup[k2] = k;

		blit(b, screen, 0, 0, x, y, 200, 72);
		destroy_bitmap(b);
		while (key[KEY_SPACE])
			;
		while (keypressed())
			readkey();
		return 0;
	}
	return ret;
}

DIALOG bemdefinegui[] =
{
	{ d_box_proc,	 0,	   0,	538, 256,  15, 15, 0, 0,       0,     0, 0,	   NULL,		  NULL	       },

	{ d_button_proc, 205,	   218, 60,  28,   15, 15, 0, D_CLOSE, 0,     0, "OK",	   NULL,		  NULL	       },
	{ d_button_proc, 271,	   218, 60,  28,   15, 15, 0, D_CLOSE, 0,     0, "Cancel", NULL,		  NULL	       },

	{ d_getkey,	 10 + 24,  42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "ESC",	   (void*)ALLEGRO_KEY_ESCAPE,	  NULL	       },
	{ d_getkey,	 42 + 24,  42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "1",	   (void*)ALLEGRO_KEY_1,	  NULL	       },
	{ d_getkey,	 74 + 24,  42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "2",	   (void*)ALLEGRO_KEY_2,	  NULL	       },
	{ d_getkey,	 106 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "3",	   (void*)ALLEGRO_KEY_3,	  NULL	       },
	{ d_getkey,	 138 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "4",	   (void*)ALLEGRO_KEY_4,	  NULL	       },
	{ d_getkey,	 170 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "5",	   (void*)ALLEGRO_KEY_5,	  NULL	       },
	{ d_getkey,	 202 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "6",	   (void*)ALLEGRO_KEY_6,	  NULL	       },
	{ d_getkey,	 234 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "7",	   (void*)ALLEGRO_KEY_7,	  NULL	       },
	{ d_getkey,	 266 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "8",	   (void*)ALLEGRO_KEY_8,	  NULL	       },
	{ d_getkey,	 298 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "9",	   (void*)ALLEGRO_KEY_9,	  NULL	       },
	{ d_getkey,	 330 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "0",	   (void*)ALLEGRO_KEY_0,	  NULL	       },
	{ d_getkey,	 362 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "=",	   (void*)ALLEGRO_KEY_MINUS,	  NULL	       },
	{ d_getkey,	 394 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, ":",	   (void*)ALLEGRO_KEY_QUOTE,	  NULL	       },
	{ d_getkey,	 426 + 24, 42,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "UP",	   (void*)ALLEGRO_KEY_EQUALS,	  NULL	       },
	{ d_getkey,	 -6 + 24,  74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "<->",	   (void*)ALLEGRO_KEY_RIGHT,	  NULL	       },
	{ d_getkey,	 26 + 24,  74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "CPY",	   (void*)ALLEGRO_KEY_TAB,	  "COPY"       },
	{ d_getkey,	 58 + 24,  74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "Q",	   (void*)ALLEGRO_KEY_Q,	  NULL	       },
	{ d_getkey,	 90 + 24,  74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "W",	   (void*)ALLEGRO_KEY_W,	  NULL	       },
	{ d_getkey,	 122 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "E",	   (void*)ALLEGRO_KEY_E,	  NULL	       },
	{ d_getkey,	 154 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "R",	   (void*)ALLEGRO_KEY_R,	  NULL	       },
	{ d_getkey,	 186 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "T",	   (void*)ALLEGRO_KEY_T,	  NULL	       },
	{ d_getkey,	 218 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "Y",	   (void*)ALLEGRO_KEY_Y,	  NULL	       },
	{ d_getkey,	 250 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "U",	   (void*)ALLEGRO_KEY_U,	  NULL	       },
	{ d_getkey,	 282 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "I",	   (void*)ALLEGRO_KEY_I,	  NULL	       },
	{ d_getkey,	 314 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "O",	   (void*)ALLEGRO_KEY_O,	  NULL	       },
	{ d_getkey,	 346 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "P",	   (void*)ALLEGRO_KEY_P,	  NULL	       },
	{ d_getkey,	 378 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "@",	   (void*)ALLEGRO_KEY_BACKSLASH2, NULL	       },
	{ d_getkey,	 410 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "\\",	   (void*)ALLEGRO_KEY_BACKSLASH,  NULL	       },
	{ d_getkey,	 442 + 24, 74,	28,  28,   15, 15, 0, D_EXIT,  0,     0, "DEL",	   (void*)ALLEGRO_KEY_DEL,	  NULL	       },
	{ d_getkey,	 10 + 24,  106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "ADJ",	   (void*)ALLEGRO_KEY_CAPSLOCK,	  "ADJUST"     },
	{ d_getkey,	 42 + 24,  106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "CTL",	   (void*)ALLEGRO_KEY_LCTRL,	  "CTRL"       },
	{ d_getkey,	 74 + 24,  106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "A",	   (void*)ALLEGRO_KEY_A,	  NULL	       },
	{ d_getkey,	 106 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "S",	   (void*)ALLEGRO_KEY_S,	  NULL	       },
	{ d_getkey,	 138 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "D",	   (void*)ALLEGRO_KEY_D,	  NULL	       },
	{ d_getkey,	 170 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "F",	   (void*)ALLEGRO_KEY_F,	  NULL	       },
	{ d_getkey,	 202 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "G",	   (void*)ALLEGRO_KEY_G,	  NULL	       },
	{ d_getkey,	 234 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "H",	   (void*)ALLEGRO_KEY_H,	  NULL	       },
	{ d_getkey,	 266 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "J",	   (void*)ALLEGRO_KEY_J,	  NULL	       },
	{ d_getkey,	 298 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "K",	   (void*)ALLEGRO_KEY_K,	  NULL	       },
	{ d_getkey,	 330 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "L",	   (void*)ALLEGRO_KEY_L,	  NULL	       },
	{ d_getkey,	 362 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, ";",	   (void*)ALLEGRO_KEY_SEMICOLON,  NULL	       },
	{ d_getkey,	 394 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "[",	   (void*)ALLEGRO_KEY_OPENBRACE,  NULL	       },
	{ d_getkey,	 426 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "]",	   (void*)ALLEGRO_KEY_CLOSEBRACE, NULL	       },
	{ d_getkey,	 458 + 24, 106, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "RET",	   (void*)ALLEGRO_KEY_ENTER,	  "RETURN"     },
	{ d_getkey,	 26 + 24,  138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "SLK",	   (void*)ALLEGRO_KEY_ALT,	  "SHIFT LOCK" },
	{ d_getkey,	 58 + 24,  138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "SHF",	   (void*)ALLEGRO_KEY_LSHIFT,	  NULL	       },
	{ d_getkey,	 90 + 24,  138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "Z",	   (void*)ALLEGRO_KEY_Z,	  NULL	       },
	{ d_getkey,	 122 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "X",	   (void*)ALLEGRO_KEY_X,	  NULL	       },
	{ d_getkey,	 154 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "C",	   (void*)ALLEGRO_KEY_C,	  NULL	       },
	{ d_getkey,	 186 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "V",	   (void*)ALLEGRO_KEY_V,	  NULL	       },
	{ d_getkey,	 218 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "B",	   (void*)ALLEGRO_KEY_B,	  NULL	       },
	{ d_getkey,	 250 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "N",	   (void*)ALLEGRO_KEY_N,	  NULL	       },
	{ d_getkey,	 282 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "M",	   (void*)ALLEGRO_KEY_M,	  NULL	       },
	{ d_getkey,	 314 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, ",",	   (void*)ALLEGRO_KEY_COMMA,	  NULL	       },
	{ d_getkey,	 346 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, ".",	   (void*)ALLEGRO_KEY_FULLSTOP,	  NULL	       },
	{ d_getkey,	 378 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "/",	   (void*)ALLEGRO_KEY_SLASH,	  NULL	       },
	{ d_getkey,	 410 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "SHF",	   (void*)ALLEGRO_KEY_RSHIFT,	  NULL	       },
	{ d_getkey,	 442 + 24, 138, 28,  28,   15, 15, 0, D_EXIT,  0,     0, "RPT",	   (void*)ALLEGRO_KEY_DEL,	  "REPEAT"     },
	{ d_getkey,	 122 + 24, 170, 256, 28,   15, 15, 0, D_EXIT,  0,     0, "SPACE",  (void*)ALLEGRO_KEY_SPACE,	  NULL	       },

	{ d_yield_proc },
	{ 0,		 0,	   0,	0,   0,	   0,  0,  0, 0,       0,     0, NULL,	   NULL,		  NULL	       }
};


int gui_keydefine()
{
	DIALOG_PLAYER *dp;
	BITMAP *b;
	DIALOG *d = bemdefinegui;
	int x = 0, y;

	while (d[x].proc)
	{
		d[x].x += (SCREEN_W / 2) - (d[0].w / 2);
		d[x].y += (SCREEN_H / 2) - (d[0].h / 2);
		d[x].fg = 0xFFFFFF;
		x++;
	}
	for (x = 0; x < 128; x++)
		keytemp[x] = keylookup[x];
	x = (SCREEN_W / 2) - (d[0].w / 2);
	y = (SCREEN_H / 2) - (d[0].h / 2);
	b = create_bitmap(d[0].w, d[0].h);
	blit(screen, b, x, y, 0, 0, d[0].w, d[0].h);
	dp = init_dialog(d, 0);
	while (x && !key[ALLEGRO_KEY_F11] && !(mouse_b & 2) && !key[ALLEGRO_KEY_ESCAPE])
	{
		x = update_dialog(dp);
	}
	shutdown_dialog(dp);
	if (x == 1)
	{
		for (x = 0; x < 128; x++)
			keylookup[x] = keytemp[x];
	}
	x = (SCREEN_W / 2) - (d[0].w / 2);
	y = (SCREEN_H / 2) - (d[0].h / 2);
	blit(b, screen, 0, 0, x, y, d[0].w, d[0].h);
	x = 0;
	while (d[x].proc)
	{
		d[x].x -= (d[0].w / 2) - (538 / 2);
		d[x].y -= (d[0].h / 2) - (256 / 2);
		x++;
	}
	return D_O_K;
}
#endif
