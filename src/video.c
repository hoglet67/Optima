/*Atomulator v1.0 by Tom Walker
   6847 video emulation*/

#include <stdio.h>
#include <allegro.h>
#include "atom.h"

int fullscreen = 0;
int winsizex = 512, winsizey = 384;

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

int atompal[9][3] =
{
	{ 0,  0,  0  }, /*Black*/
	{ 0,  63, 0  }, /*Green*/
	{ 63, 63, 0  }, /*Yellow*/
	{ 0,  0,  63 }, /*Blue*/
	{ 63, 0,  0  }, /*Red*/
	{ 63, 63, 63 }, /*Buff*/
	{ 0,  63, 63 }, /*Cyan*/
	{ 63, 0,  63 }, /*Magenta*/
	{ 63, 0,  0  }, /*Orange - actually red on the Atom*/
};

int monopal[9][3] =
{
	{ 8,  8,  8  }, /*Black*/
	{ 55, 55, 55 }, /*Green*/
	{ 63, 63, 63 }, /*Yellow*/
	{ 32, 32, 32 }, /*Blue*/
	{ 32, 32, 32 }, /*Red*/
	{ 63, 63, 63 }, /*Buff*/
	{ 55, 55, 55 }, /*Cyan*/
	{ 55, 55, 55 }, /*Magenta*/
	{ 55, 55, 55 }, /*Orange - actually red on the Atom*/
};

ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *b;
ALLEGRO_LOCKED_REGION *lr;

int screenW;
int screenH;
int scaleW;
int scaleH;
int scaleX;
int scaleY;

//int depth;
void initvideo()
{

  screenW = 256;
  screenH = 192;


  // TODO: Determine these dynamically

  int displayW = 1920;
  int displayH = 1080;

  al_set_new_display_flags(ALLEGRO_FULLSCREEN);

  display = al_create_display(displayW, displayH);
  displayW = al_get_display_width(display);
  displayH = al_get_display_height(display);

  debuglog("screen is %d x %d\n", displayW, displayH);

  int sx = displayW / screenW;
  int sy = displayH / screenH;
  int scale = sx < sy ? sx : sy;

  debuglog("scale is %d\n", scale);

  scaleW = screenW * scale;
  scaleH = screenH * scale;
  scaleX = (displayW - scaleW) / 2;
  scaleY = (displayH - scaleH) / 2;

  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();

  b = al_create_bitmap(screenW, screenH);

  al_set_target_bitmap(b);
  lr = al_lock_bitmap(b, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);

  // if (colourboard)
  //    set_palette(atompal);
  // else
  //    set_palette(monopal);

  updatepal();
}


uint8_t *ram;
int cy = 0, sy = 0;


int textcol[4] = {
  0xff000000,
  0xff00ff00,
  0xff000000,
  0xff007fff
};


int semigrcol[8] = {
  0xff00ff00,
  0xff00ffff,
  0xffff0000,
  0xff0000ff,
  0xffffffff,
  0xffffff00,
  0xffff00ff,
  0xff007fff
};

int grcol[4] = {
  0xff000000,
  0xff00ff00,
  0xff000000,
  0xffffffff
};

int black = 0xff000000;

void updatepal()
{

  //	if (colourboard) 
  //		set_palette(atompal);
  //	else
  //		set_palette(monopal);
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

	// debuglog("drawing line %d gfxmode %d \n", line, gfxmode);
	// double t1 = al_get_time();



	if (!line)
		vbl = cy = sy = 0;
		
	if (line < 192)
	{
	  // al_set_target_bitmap(b);

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
				fetcheddat[x] = ram[addr++];
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
				fetcheddat[x] = ram[addr + (x >> 1)];
			
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
				fetcheddat[x] = ram[addr + (x >> 1)];
			
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
			fetcheddat[x] = ram[addr + x];
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
				fetcheddat[x] = ram[addr + (x >> 1)];

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
				fetcheddat[x] = ram[addr + x];

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
				fetcheddat[x] = ram[addr + (x >> 1)];

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
				fetcheddat[x] = ram[addr + x];

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
				fetcheddat[x] = ram[addr + x];

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
			/*
			if (colourboard)
				save_bmp(scrshotname, b, atompal);
			else
				save_bmp(scrshotname, b, monopal);
			*/
		}

		if ((!(tapeon && fasttape) && fskipcount >= fskipmax) || frmcount == 60)
		{
			fskipcount = 0;


			if (fullscreen)
			{
			  // stretch_blit(b2, screen, 0, 0, 256, 192, 0, 0, 1024, 768);
			  // if (tapeon)
			  //	rectfill(screen, 1000, 0, 1023, 8, makecol(255, 0, 0));
			}
			else
			{
			  // stretch_blit(b2, screen, 0, 0, 256, 192, 0, 0, winsizex, winsizey-1);
			  //	if (tapeon)
			  //		rectfill(screen, winsizex - 12, 0, winsizex, 4, makecol(255, 0, 0));
			}

			al_unlock_bitmap(b);
			al_set_target_bitmap(al_get_backbuffer(display));
			// al_draw_bitmap(b, 0, 0, 0);
			al_draw_scaled_bitmap(b, 0, 0, screenW, screenH, scaleX, scaleY, scaleW, scaleH, 0);
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

	if ((line == 261 && !colourboard) || line == 311)
	{
		switch (gfxmode)
		{

/* CHANGED FOR CORRECT CLEAR2a */

		case 0: case 2: case 4: case 6:         /*Text mode*/
		case 8: case 10: case 12: case 14:
		case 5: case 9: case 13: case 15:
			for (x = 0; x < 32; x++)
				fetcheddat[x] = ram[0x8000 + x];
			break;
/* END CHANGES */

		case 1: case 3: case 7: case 11:         /*16-byte per line*/
			for (x = 0; x < 32; x++)
				fetcheddat[x] = ram[0x8000 + (x >> 1)];
			break;

		}
	}

	//double t2 = al_get_time();
	//debuglog("drawing line took %d\n", (int) (1000000.0 * (t2 - t1)));


//        sndbuffer[line]=(speaker)?255:0;
}

/*void mixaudio(uint8_t *p)
   {
        memcpy(p,sndbuffer,262);
   }*/

void enterfullscreen()
{
/*	if (opengl)
        {
                rpclog("Enter fullscreen start\n");
                openglreinit();
                rpclog("Enter fullscreen end\n");
                return;
        }*/
	updatepal();
}

void leavefullscreen()
{
/*	if (opengl)
        {
                openglreinit();
                return;
        }*/

	//	set_color_depth(depth);
	//#ifdef WIN32
	//	set_gfx_mode(ALLEGRO_WINDOWED, 2048, 2048, 0, 0);
	//#else
	//	set_gfx_mode(ALLEGRO_WINDOWED, 512, 384, 0, 0);
	//#endif

	//set_color_depth(8);
	updatepal();

	updatewindowsize(512, 384);
}

void clearscreen()
{
  al_set_target_bitmap(al_get_backbuffer(display));
  al_clear_to_color(al_map_rgb(0, 0, 0));

  al_set_target_bitmap(b);
  al_clear_to_color(al_map_rgb(0, 0, 0));  
}
