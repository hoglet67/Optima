/*Atomulator v1.0 by Tom Walker
   8255 PIA + keyboard + sound emulation*/
#include <string.h>
#include <allegro.h>
#include "atom.h"
#include "6502.h"

extern M6502* the_cpu;

int16_t sndbuffer[SNDBUFLEN * 2];
int16_t sidbuffer[SNDBUFLEN];
int16_t last_val = 0;
int     last_cyclecount = 0;


/*SWARM - CTRL=LEFT, ADJ=RIGHT, REPT=FIRE*/
/*PINBALL - SHIFT,REPT*/
int output;
int tapecyc;
int inchunk;
int intone = 0, tapedat, hightone = 0;
int bytevalid = 0, bitvalid = 0;
int tapeon = 0;
uint16_t databyte;

uint16_t pc;

int keyl[128];
int keys[16][6] =
{
	{ 0,		  ALLEGRO_KEY_3,	 ALLEGRO_KEY_MINUS,	ALLEGRO_KEY_G,	    ALLEGRO_KEY_Q, ALLEGRO_KEY_ESCAPE },
	{ 0,		  ALLEGRO_KEY_2,	 ALLEGRO_KEY_COMMA,	ALLEGRO_KEY_F,	    ALLEGRO_KEY_P, ALLEGRO_KEY_Z   },
	{ ALLEGRO_KEY_UP,	  ALLEGRO_KEY_1,	 ALLEGRO_KEY_SEMICOLON, ALLEGRO_KEY_E,	    ALLEGRO_KEY_O, ALLEGRO_KEY_Y   },
	{ ALLEGRO_KEY_RIGHT,	  ALLEGRO_KEY_0,	 ALLEGRO_KEY_QUOTE,	ALLEGRO_KEY_D,	    ALLEGRO_KEY_N, ALLEGRO_KEY_X   },
	{ ALLEGRO_KEY_CAPSLOCK,	  ALLEGRO_KEY_BACKSPACE, ALLEGRO_KEY_9,		ALLEGRO_KEY_C,	    ALLEGRO_KEY_M, ALLEGRO_KEY_W   },
	{ ALLEGRO_KEY_TAB,	  ALLEGRO_KEY_END,	 ALLEGRO_KEY_8,		ALLEGRO_KEY_B,	    ALLEGRO_KEY_L, ALLEGRO_KEY_V   },
	{ ALLEGRO_KEY_CLOSEBRACE, ALLEGRO_KEY_ENTER,	 ALLEGRO_KEY_7,		ALLEGRO_KEY_A,	    ALLEGRO_KEY_K, ALLEGRO_KEY_U   },
	{ ALLEGRO_KEY_BACKSLASH,  0,		 ALLEGRO_KEY_6,		ALLEGRO_KEY_EQUALS, ALLEGRO_KEY_J, ALLEGRO_KEY_T   },
	{ ALLEGRO_KEY_OPENBRACE,  0,		 ALLEGRO_KEY_5,		ALLEGRO_KEY_SLASH,  ALLEGRO_KEY_I, ALLEGRO_KEY_S   },
	{ ALLEGRO_KEY_SPACE,	  0,		 ALLEGRO_KEY_4,		ALLEGRO_KEY_FULLSTOP,   ALLEGRO_KEY_H, ALLEGRO_KEY_R   }
};

void init8255()
{
	int c, d;

	memset(keyl, 0, sizeof(keyl));
	for (c = 0; c < 16; c++)
	{
		for (d = 0; d < 6; d++)
		{
			keyl[keys[c][d]] = c | (d << 4) | 0x80;
		}
	}
}

/*int keys2[16][6]=
   {
        {0,ALLEGRO_KEY_3,ALLEGRO_KEY_MINUS,ALLEGRO_KEY_G,ALLEGRO_KEY_Q,ALLEGRO_KEY_ESC},
        {0,ALLEGRO_KEY_2,ALLEGRO_KEY_COMMA,ALLEGRO_KEY_F,ALLEGRO_KEY_P,ALLEGRO_KEY_Z},
        {ALLEGRO_KEY_DEL,ALLEGRO_KEY_1,ALLEGRO_KEY_COLON,ALLEGRO_KEY_E,ALLEGRO_KEY_O,ALLEGRO_KEY_Y},
        {ALLEGRO_KEY_RIGHT,ALLEGRO_KEY_0,ALLEGRO_KEY_QUOTE,ALLEGRO_KEY_D,ALLEGRO_KEY_N,ALLEGRO_KEY_X},
        {ALLEGRO_KEY_CAPSLOCK,ALLEGRO_KEY_BACKSPACE,ALLEGRO_KEY_9,ALLEGRO_KEY_C,ALLEGRO_KEY_M,ALLEGRO_KEY_W},
        {ALLEGRO_KEY_TAB,ALLEGRO_KEY_END,ALLEGRO_KEY_8,ALLEGRO_KEY_B,ALLEGRO_KEY_L,ALLEGRO_KEY_V},
        {ALLEGRO_KEY_CLOSEBRACE,ALLEGRO_KEY_ENTER,ALLEGRO_KEY_7,ALLEGRO_KEY_A,ALLEGRO_KEY_K,ALLEGRO_KEY_U},
        {ALLEGRO_KEY_BACKSLASH,0,ALLEGRO_KEY_6,ALLEGRO_KEY_EQUALS,ALLEGRO_KEY_J,ALLEGRO_KEY_T},
        {ALLEGRO_KEY_OPENBRACE,0,ALLEGRO_KEY_5,ALLEGRO_KEY_SLASH,ALLEGRO_KEY_I,ALLEGRO_KEY_S},
        {ALLEGRO_KEY_SPACE,0,ALLEGRO_KEY_4,ALLEGRO_KEY_STOP,ALLEGRO_KEY_H,ALLEGRO_KEY_R}
   };*/


int keyrow;
int last_speaker;

void write8255(uint16_t addr, uint8_t val)
{
	int oldgfx = gfxmode;

	
	switch (addr & 0xF)
	{
	case 0:
		keyrow = val & 0xF;
//                if (gfxmode!=(val>>4)) printf("GFX mode now %02X %04X\n",val,pc);
		gfxmode = (val >> 4) & 0x0F;
//		if(gfxmode!=oldgfx)
//			rpclog("gfxmode changed at PC=%04X from %02X to %02X\n",pc,oldgfx,gfxmode);
//                printf("GFX mode now %02X %04X\n",val,pc);
//                printf("Keyrow now %i %04X\n",keyrow,pc);
		break;
	case 2:
		css = (val & 8) >> 2;
		speaker = (val & 4) >> 2;
		if (speaker != last_speaker) {
		  sndbuffer[(the_cpu->cyclesTotal >> 5) % (SNDBUFLEN * 2)] = speaker ? 4095 : -4095;
		  last_speaker = speaker;
		}
		//rpclog("B002 = %02x\n", val);
		break;
	case 3:
		switch (val & 0x8E)
		{
		case 0x04: 
		  speaker = (val & 1);
		  if (speaker != last_speaker) {
		    sndbuffer[(the_cpu->cyclesTotal >> 5) % (SNDBUFLEN * 2)] = speaker ? 4095 : -4095;
		    last_speaker = speaker;
		  }
		  //rpclog("B003 = %02x\n", val);
			break;
			
		case 0x06: 
			css = (val & 1) ? 2 : 0; 
			break;
		}
		break;
//                        rpclog("8255 port 3 %02X\n",val);
	}
        //rpclog("Write 8255 %04X %02X\n",addr,val);
}

uint8_t read8255(uint16_t addr)
{
	uint8_t temp = 0xFF;
	int c;
	int i;
	int *kptr;

	ALLEGRO_KEYBOARD_STATE key_state;
	al_get_keyboard_state(&key_state);


        //rpclog("Read 8255 %04X %04X\n",addr,pc);
	switch (addr & 3)
	{
	case 0:
		return (keyrow & 0x0F) | ((gfxmode << 4) & 0xF0);
	case 1:
	  /*
		for (c = 0; c < 128; c++)
		{
		  if (al_key_down(&key_state, c) && keyl[keylookup[c]] & 0x80 && keyrow == (keyl[keylookup[c]] & 0xF))
				temp &= ~(1 << ((keyl[keylookup[c]] & 0x70) >> 4));
		}
	  */
 	        kptr = keys[keyrow & 0xF];
		for (i = 0; i < 6; i++) {
		  c = *kptr++;
		  if (al_key_down(&key_state, c))
		    temp &= ~(1 << ((keyl[keylookup[c]] & 0x70) >> 4));
		}

		if (al_key_down(&key_state, ALLEGRO_KEY_LCTRL))
		  temp &= ~0x40;
		if (al_key_down(&key_state, ALLEGRO_KEY_LSHIFT) || al_key_down(&key_state, ALLEGRO_KEY_RSHIFT))
		  temp &= ~0x80;

/*                if (pressed_keys[keys[keyrow][0]]) temp&=~1;
                if (pressed_keys[keys[keyrow][1]]) temp&=~2;
                if (pressed_keys[keys[keyrow][2]]) temp&=~4;
                if (pressed_keys[keys[keyrow][3]]) temp&=~8;
                if (pressed_keys[keys[keyrow][4]]) temp&=~0x10;
                if (pressed_keys[keys[keyrow][5]]) temp&=~0x20;
                if (pressed_keys[keys2[keyrow][0]]) temp&=~1;
                if (pressed_keys[keys2[keyrow][1]]) temp&=~2;
                if (pressed_keys[keys2[keyrow][2]]) temp&=~4;
                if (pressed_keys[keys2[keyrow][3]]) temp&=~8;
                if (pressed_keys[keys2[keyrow][4]]) temp&=~0x10;
                if (pressed_keys[keys2[keyrow][5]]) temp&=~0x20;
   //                rpclog("temp=%02X\n",temp);*/
//                if (pressed_keys[keylookup[ALLEGRO_KEY_LCONTROL]] || pressed_keys[keylookup[ALLEGRO_KEY_RCONTROL]]) temp&=~0x40;
//                if (pressed_keys[keylookup[ALLEGRO_KEY_LSHIFT]] || pressed_keys[keylookup[ALLEGRO_KEY_RSHIFT]]) temp&=~0x80;
		return temp;
	case 2:
		if (vbl)
			temp &= ~0x80;
		if (al_key_down(&key_state, ALLEGRO_KEY_ALT) || al_key_down(&key_state, ALLEGRO_KEY_ALTGR))
			temp &= ~0x40;
		if (!css)
			temp &= ~8;
		if (!speaker)
			temp &= ~4;
		if (!intone)
			temp &= ~0x10;
		if (!tapedat)
			temp &= ~0x20;
//                printf("VBL %i %04X\n",vbl,pc);
		return temp;
//                default:
//                printf("Read 8255 %04X\n",addr);
	}
	return 0;
}

void polltape()
{
	if (cswena)
	{
		if (tapeon)
		{
			tapecyc += getcsw();
			tapedat = !tapedat;
		}
	}
	else
	{
		tapecyc += 794;
		intone ^= 0x10;
		if (tapeon)
		{
			if (hightone)
			{
				hightone--;
				tapedat = hightone & 1;
				inchunk = 0;
				if (!hightone)
					polluef();
			}
			else if (bytevalid)
			{
				if (databyte & 1)
					tapedat = bitvalid & 1;
				else
					tapedat = bitvalid & 2;
				bitvalid--;
				if (!bitvalid)
				{
					bytevalid--;
					databyte >>= 1;
					if (!bytevalid)
						polluef();
					else
					{
						bitvalid = 16;
					}
				}
			}
			else
				polluef();
		}
	}
}

void dcdlow()
{
	hightone = 0;
//        printf("High tone off\n");
}

void dcd()
{
	hightone = 15000;
//        printf("High tone on\n");
}

void receive(uint8_t dat)
{
	bytevalid = 10;
	bitvalid = 16;
	databyte = (dat << 1) | 0x200;
//        rpclog("Recieved byte %02X - first bit %i\n",dat,databyte&1);
	lastdat = dat;
}

// Currently pollsound is called every two lines (128us) emulated time
void pollsound()
{

  int16_t temp;  
  int i;
  int cycle_count = (the_cpu->cyclesTotal >> 5) % SNDBUFLEN;

  // Detect when we have moved from one sound buffer to the other (i.e. cycle_count has wrapped)
  if (cycle_count < last_cyclecount) {

    // Offset is implementing double buffering
    int offset = (((the_cpu->cyclesTotal >> 5) / SNDBUFLEN) & 1) ? 0 : SNDBUFLEN;

    // Iterate through the buffer overwriting 0's with the previous active value
    int16_t *sndptr = sndbuffer + offset;
    for (i = 0; i < SNDBUFLEN; i++) {
      if (*sndptr == 0) {
	*sndptr++ = last_val;
      } else {
	last_val = *sndptr++;
      }
    }
    
    // Add in the SID data 
    if (sndatomsid) {
      sndptr = sndbuffer + offset;
      int16_t *sidptr = sidbuffer;
      for (i = 0; i < SNDBUFLEN; i++) {
	*sndptr++ += *sidptr++;
      }
    }

    // Send to the OpenAL sound API
    givealbuffer(sndbuffer + offset);

    // Reset the buffer back to 0's again
    memset(sndbuffer + offset, 0, SNDBUFLEN * 2);
  }

  // The SID code was setup to generate one sample per line
  // Generate twp SID samples for the two lines
  // Then double them to upsample to 31250Hz
  if (sndatomsid) {
    sid_fillbuf(&temp,2);
    sidbuffer[cycle_count] = temp;
    sidbuffer[cycle_count + 1] = temp;
    sid_fillbuf(&temp,2);
    sidbuffer[cycle_count + 2] = temp;
    sidbuffer[cycle_count + 3] = temp;
  }

  // Remember for next time
  last_cyclecount = cycle_count;
}
