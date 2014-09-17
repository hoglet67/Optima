/*Atomulator v1.0 by Tom Walker
   6847 video emulation*/

#include <stdio.h>
#include <allegro.h>
#include <allegro_font.h>
#include <allegro_ttf.h>
#include "atom.h"
#include "6502.h"

// The native pi cursor seems to be broken
// As a workaround, we'll plot this ourselves
#include "picursor.h"
#define DEFAULT_CURSOR_WIDTH 17
#define DEFAULT_CURSOR_HEIGHT 28

extern M6502* the_cpu;

int show_menu = 0;

int fullscreen = 1;

uint8_t fontdata[] =
{
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x02, 0x1a, 0x2a, 0x2a, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x22, 0x3e, 0x22, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3c, 0x12, 0x12, 0x1c, 0x12, 0x12, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x20, 0x20, 0x20, 0x22, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3c, 0x12, 0x12, 0x12, 0x12, 0x12, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3e, 0x20, 0x20, 0x3c, 0x20, 0x20, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3e, 0x20, 0x20, 0x3c, 0x20, 0x20, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1e, 0x20, 0x20, 0x26, 0x22, 0x22, 0x1e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x3e, 0x22, 0x22, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x22, 0x22, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x24, 0x28, 0x30, 0x28, 0x24, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x36, 0x2a, 0x2a, 0x22, 0x22, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x32, 0x2a, 0x26, 0x22, 0x22, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3e, 0x22, 0x22, 0x22, 0x22, 0x22, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3c, 0x22, 0x22, 0x3c, 0x20, 0x20, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x22, 0x22, 0x2a, 0x24, 0x1a, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3c, 0x22, 0x22, 0x3c, 0x28, 0x24, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x10, 0x08, 0x04, 0x22, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x2a, 0x2a, 0x36, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x22, 0x14, 0x08, 0x14, 0x22, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3e, 0x02, 0x04, 0x08, 0x10, 0x20, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x38, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x20, 0x20, 0x10, 0x08, 0x04, 0x02, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x1c, 0x2a, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x3e, 0x10, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x14, 0x14, 0x36, 0x00, 0x36, 0x14, 0x14, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x1e, 0x20, 0x1c, 0x02, 0x3c, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x32, 0x32, 0x04, 0x08, 0x10, 0x26, 0x26, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x10, 0x28, 0x28, 0x10, 0x2a, 0x24, 0x1a, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x10, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x3e, 0x1c, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x10, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x02, 0x1c, 0x20, 0x20, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x02, 0x0c, 0x02, 0x22, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x04, 0x0c, 0x14, 0x3e, 0x04, 0x04, 0x04, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3e, 0x20, 0x3c, 0x02, 0x02, 0x22, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x20, 0x20, 0x3c, 0x22, 0x22, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3e, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x22, 0x1c, 0x22, 0x22, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1c, 0x22, 0x22, 0x1e, 0x02, 0x02, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x24, 0x04, 0x08, 0x08, 0x00, 0x08, 0x00, 0x00,
};

ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *b;
ALLEGRO_BITMAP *cursor;
ALLEGRO_LOCKED_REGION *lr;

ALLEGRO_FONT *font;
ALLEGRO_FONT *menufont;

int screenW = 256;
int screenH = 192;
int scaleW;
int scaleH;
int scaleX;
int scaleY;
int displayW;
int displayH;

char popup_message[255];
int popup_time = 0;


void popup(char *message, int time) {
  strcpy(popup_message, message);
  popup_time = time;
}

void initvideo()
{
  int x, y;
  ALLEGRO_MONITOR_INFO info;

  al_get_monitor_info(0, &info);
  displayW = info.x2 - info.x1;
  displayH = info.y2 - info.y1;
  rpclog("screen is %d x %d\n", displayW, displayH);

  al_set_new_display_flags(ALLEGRO_FULLSCREEN);

  display = al_create_display(displayW, displayH);

  int sx = displayW / screenW;
  int sy = displayH / screenH;
  int scale = sx < sy ? sx : sy;

  rpclog("scale is %d\n", scale);

  scaleW = screenW * scale;
  scaleH = screenH * scale;
  scaleX = (displayW - scaleW) / 2;
  scaleY = (displayH - scaleH) / 2;

  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();

  // Create the mouse cursor
  cursor = al_create_bitmap(DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT);
  al_set_target_bitmap(cursor);
  uint32_t *cursor = default_cursor;
  for (y = 0; y < DEFAULT_CURSOR_HEIGHT; y++) {
    for (x = 0; x < DEFAULT_CURSOR_WIDTH; x++) {
      uint32_t col = *cursor++;
      al_put_pixel(x, y, al_map_rgba(col & 255, (col >> 8) & 255, (col >> 16) & 255,  (col >> 24) & 255));
    }
  }

  b = al_create_bitmap(screenW, screenH);
  al_set_target_bitmap(b);
  lr = al_lock_bitmap(b, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);

  updatepal();

  font = al_load_font("fonts/DejaVuSans.ttf", POPUP_FONT_SIZE, 0);

  if (!font) {
    rpclog("Failed to load font\n");
  }

  menufont = al_load_font("fonts/DejaVuSans.ttf", MENU_FONT_SIZE, 0);

  if (!menufont) {
    rpclog("Failed to load menufont\n");
  }

  optima_gui_init(display, menufont);

  //  al_grab_mouse(display);
  //if (!al_show_mouse_cursor(display)) {
  //  rpclog("Failed to show mouse cursor\n");
  //}

}



int cy = 0, sy = 0;

int *textcol;
int *semigrcol;
int *grcol;
int black;

int textcols[2][4] = {
  {
    0xff202020,
    0xffe0e0e0,
    0xff202020,
    0xffe0e0e0,
  },
  {
    0xff000000,
    0xff00ff00,
    0xff000000,
    0xff007fff
  }
};


int semigrcols[2][8] = {
  {
    0xffe0e0e0,
    0xffffffff,
    0xff808080,
    0xff808080,
    0xffffffff,
    0xffe0e0e0,
    0xffe0e0e0,
    0xffe0e0e0,
  },
  {
    0xff00ff00,
    0xff00ffff,
    0xffff0000,
    0xff0000ff,
    0xffffffff,
    0xffffff00,
    0xffff00ff,
    0xff007fff
  }
};

int grcols[2][4] = {
  {
    0xff202020,
    0xffe0e0e0,
    0xff202020,
    0xffffffff
  },
  {
    0xff000000,
    0xff00ff00,
    0xff000000,
    0xffffffff
  }
};



int blacks[2] = {
  0xff202020,
  0xff000000
};


void updatepal()
{
  textcol = textcols[colourboard];
  semigrcol = semigrcols[colourboard];
  grcol = grcols[colourboard];
  black = blacks[colourboard];
}


void togglepal() {
  colourboard = 1 - colourboard;
  updatepal();
  if (colourboard) {
      popup("Colour Palette", 120);
  } else {
      popup("Monochrome Palette", 120);
  }
}

int tapeon;
int frmcount;
int fskipcount = 0;

char scrshotname[260];
int savescrshot = 0;

uint8_t fetcheddat[32];

void drawline(int line)
{
        int addr, chr;
        ALLEGRO_COLOR col;
	int x, xx;
	uint8_t temp;


	if (!line)
		vbl = cy = sy = 0;
		
	if (line < 192)
	{

	  unsigned int *ptr = (unsigned int *)(lr->data + lr->pitch * line);
	  int col;

		switch (gfxmode)
		{
		case 0: case 2: case 4: case 6:         /*Text mode*/
		case 8: case 10: case 12: case 14:
			addr = (cy << 5) + 0x8000;
			for (x = 0; x < 256; x += 8)
			{
				chr = fetcheddat[x >> 3];
				if (chr & 0x40)
				{
					temp = chr;
					chr <<= ((sy >> 2) << 1);
					chr = (chr >> 4) & 3;
					if (chr & 2)
						col = semigrcol[(temp >> 6) | (css << 1)];
					else
						col = black;

					// b->line[line][x] = b->line[line][x + 1] = b->line[line][x + 2] = b->line[line][x + 3] = col;
					*ptr++ = col;
					*ptr++ = col;
					*ptr++ = col;
					*ptr++ = col;
					if (chr & 1)
						col = semigrcol[(temp >> 6) | (css << 1)];
					else
						col = black;
					// b->line[line][x + 4] = b->line[line][x + 5] = b->line[line][x + 6] = b->line[line][x + 7] = col;
					*ptr++ = col;
					*ptr++ = col;
					*ptr++ = col;
					*ptr++ = col;
				}
				else
				{
					chr = ((chr & 0x3F) * 12) + sy;
					if (fetcheddat[x >> 3] & 0x80)
					{
						for (xx = 0; xx < 8; xx++)
						{
						  // b->line[line][x + xx] = textcol[(((fontdata[chr] >> (xx ^ 7)) & 1) ^ 1) | css];
						  *ptr++ = textcol[(((fontdata[chr] >> (xx ^ 7)) & 1) ^ 1) | css];
						}
					}
					else
					{
						for (xx = 0; xx < 8; xx++)
						{
						  //b->line[line][x + xx] = textcol[((fontdata[chr] >> (xx ^ 7)) & 1) | css];
						  *ptr++ = textcol[((fontdata[chr] >> (xx ^ 7)) & 1) | css]; 
						}
					}
				}
			}
			sy++;
			if (sy == 12)
			{
				sy = 0;
				cy++;
			}
			addr = (cy << 5) + 0x8000;
			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[addr++];
			break;
		
		/* Propper graphics modes */
		case 1:         /*64x64, 4 colours*/
			for (x = 0; x < 256; x += 16)
			{
				temp = fetcheddat[x >> 3];
				for (xx = 0; xx < 16; xx += 4)
				{
				  //b->line[line][x + xx] = b->line[line][x + xx + 1] = b->line[line][x + xx + 2] = b->line[line][x + xx + 3] = semigrcol[(temp >> 6) | (css << 1)];
				  col = semigrcol[(temp >> 6) | (css << 1)];
				  *ptr++ = col;
				  *ptr++ = col;
				  *ptr++ = col;
				  *ptr++ = col;
				  temp <<= 2;
				}
			}
			
			addr = (((line + 1) / 3) << 4) | 0x8000;
			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[addr + (x >> 1)];
			
			break;
			
		case 3:         /*128x64, 2 colours*/
			for (x = 0; x < 256; x += 16)
			{
				temp = fetcheddat[x >> 3];
				for (xx = 0; xx < 16; xx += 2)
				{
				  // b->line[line][x + xx] = b->line[line][x + xx + 1] = (temp & 0x80) ? grcol[css | 1] : grcol[css];
				  col = (temp & 0x80) ? grcol[css | 1] : grcol[css];
				  *ptr++ = col;
				  *ptr++ = col;
				  temp <<= 1;
				}
			}
			
			addr = (((line + 1) / 3) << 4) | 0x8000;
			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[addr + (x >> 1)];
			
			break;

/* CHANGED FOR CORRECT CLEAR2a */

		case 5: /*128x64, 4 colours*/
			for (x = 0; x < 256; x += 8)
			{
				temp = fetcheddat[x >> 3];
				for (xx = 0; xx < 8; xx += 2)
				{
				  // b->line[line][x + xx] = b->line[line][x + xx + 1] = semigrcol[(temp >> 6) |(css << 1)];
				  col = semigrcol[(temp >> 6) |(css << 1)];
				  *ptr++ = col;
				  *ptr++ = col;
				  temp <<= 2;
				}
			}

			addr = (((line + 1) / 3) << 5) | 0x8000;
			for (x = 0; x < 32; x++)
			fetcheddat[x] = the_cpu->mem[addr + x];
			break;

/* END CHANGES */

		case 7:         /*128x96, 2 colours*/
			for (x = 0; x < 256; x += 16)
			{
				temp = fetcheddat[x >> 3];
				for (xx = 0; xx < 16; xx += 2)
				{
				  // b->line[line][x + xx] = b->line[line][x + xx + 1] = (temp & 0x80) ? grcol[css | 1] : grcol[css];;
				  col = (temp & 0x80) ? grcol[css | 1] : grcol[css];;
				  *ptr++ = col;
				  *ptr++ = col;
				  temp <<= 1;
				}
			}

			addr = (((line + 1) >> 1) << 4) | 0x8000;
			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[addr + (x >> 1)];

			break;

		case 9:         /*128x96, 4 colours*/
			for (x = 0; x < 256; x += 8)
			{
				temp = fetcheddat[x >> 3];
				for (xx = 0; xx < 8; xx += 2)
				{
				  // b->line[line][x + xx] = b->line[line][x + xx + 1] = semigrcol[(temp >> 6) | (css << 1)];
				  col = semigrcol[(temp >> 6) | (css << 1)];
				  *ptr++ = col;
				  *ptr++ = col;
				  temp <<= 2;
				}
			}

			addr = (((line + 1) >> 1) << 5) | 0x8000;

			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[addr + x];

			break;

		case 11:         /*128x192, 2 colours*/
			for (x = 0; x < 256; x += 16)
			{
				temp = fetcheddat[x >> 3];
				for (xx = 0; xx < 16; xx += 2)
				{
				  // b->line[line][x + xx] = b->line[line][x + xx + 1] = (temp & 0x80) ? grcol[css | 1] : grcol[css];
				  col = (temp & 0x80) ? grcol[css | 1] : grcol[css];
				  *ptr++ = col;
				  *ptr++ = col;
				  temp <<= 1;
				}
			}

			addr = ((line + 1) << 4) | 0x8000;
			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[addr + (x >> 1)];

			break;

		case 13:         /*128x192, 4 colours*/
			for (x = 0; x < 256; x += 8)
			{
				temp = fetcheddat[x >> 3];
				for (xx = 0; xx < 8; xx += 2)
				{
				  // b->line[line][x + xx] = b->line[line][x + xx + 1] = semigrcol[(temp >> 6) | (css << 1)];
				  col = semigrcol[(temp >> 6) | (css << 1)];
				  *ptr++ = col;
				  *ptr++ = col;
				  temp <<= 2;
				}
			}

			addr = ((line + 1) << 5) | 0x8000;

			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[addr + x];

			break;

		case 15:         /*256x192, 2 colours*/
			for (x = 0; x < 256; x += 8)
			{
				temp = fetcheddat[x >> 3];
				for (xx = 0; xx < 8; xx++)
				{
				  // b->line[line][x + xx] = (temp & 0x80) ? grcol[css | 1] : grcol[css];
				  // al_put_pixel(x + xx, line, (temp & 0x80) ? grcol[css | 1] : grcol[css]);
				  *ptr++ = (temp & 0x80) ? grcol[css | 1] : grcol[css];
				  temp <<= 1;
				}
			}

			addr = ((line + 1) << 5) | 0x8000;
/*rpclog("addr=%04X\n",addr);*/
			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[addr + x];

			break;

//                        default:
//                        printf("Bad GFX mode %i\n",gfxmode);
//                        dumpregs();
//                        dumpram();
//                        exit(-1);
		}
	}

	if (line == 192)
	{
	  // startblit();
		frmcount++;
		fskipcount++;

		if (savescrshot)
		{
			savescrshot = 0;
		}

		if ((!(tapeon && fasttape) && fskipcount >= fskipmax) || frmcount == 60)
		{
			fskipcount = 0;

			unsigned int border;
			// Work out border colour
			if (gfxmode & 1) {
			  // Graphics
			  border = semigrcols[colourboard][css << 1];
			} else {
			  // Text, so use black
			  border = textcols[colourboard][0];
			}

			al_unlock_bitmap(b);
			al_set_target_bitmap(al_get_backbuffer(display));
			al_clear_to_color(al_map_rgb((border >> 16) & 255, (border >> 8) & 255, border & 255));
			al_draw_scaled_bitmap(b, 0, 0, screenW, screenH, scaleX, scaleY, scaleW, scaleH, 0);

			// If a popup is active, then render it
			if (popup_time) {
			  float a = popup_time >= 32 ? 1.0 : popup_time / 32.0;
			  al_draw_text(font, al_map_rgba_f(a, 0.0, 0.0, a), (displayW >> 1), (displayH >> 1), ALLEGRO_ALIGN_CENTRE, popup_message);
			  popup_time--;
			}

			// If the menu is active, then render it
			if (show_menu) {
			  optima_gui_draw();
			  ALLEGRO_MOUSE_STATE mouseState;
			  al_get_mouse_state(&mouseState);
			  al_draw_bitmap(cursor, mouseState.x, mouseState.y, 0);
			}

			al_flip_display();
			al_lock_bitmap(b, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
			al_set_target_bitmap(b);


			frmcount = 0;
		}
		endblit();
	}

	if (line == 200)
		vbl = 1;

	if (line == 261)
		vbl = 0;

	// if ((line == 261 && !colourboard) || line == 311)
	if (line == 261)
	{
		switch (gfxmode)
		{

/* CHANGED FOR CORRECT CLEAR2a */

		case 0: case 2: case 4: case 6:         /*Text mode*/
		case 8: case 10: case 12: case 14:
		case 5: case 9: case 13: case 15:
			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[0x8000 + x];
			break;
/* END CHANGES */

		case 1: case 3: case 7: case 11:         /*16-byte per line*/
			for (x = 0; x < 32; x++)
				fetcheddat[x] = the_cpu->mem[0x8000 + (x >> 1)];
			break;

		}
	}

}
