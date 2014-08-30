/*Atomulator v1.0 by Tom Walker
   Main loop*/

#include <string.h>
#include <stdio.h>
#include <allegro.h>
#include "atom.h"
#include "atommc.h"
#include "6502.h"


char exedir[MAXPATH+1];

extern int quited;

int colourboard = 0;
int sndddnoise = 1;

// SID
int sndatomsid=1;
int cursid=0;
int sidmethod=0;

// Joystick support
unsigned char joyst = 0;

int ramrom_enable = 1;

int bbcmode = 0;
int fasttape = 0;
char discfn[260];

extern M6502* the_cpu;

FILE *rlog;
void rpclog(char *format, ...)
{
	char buf[256];

//   return;
	
	sprintf(buf,"%s/rlog.txt",exedir);
	
	if (!rlog)
		rlog = fopen(buf, "wt");
//turn;
	va_list ap;
	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);
	fputs(buf, rlog);
	fflush(rlog);
}

int tapeon;
//uint8_t *ram;
char filelist[256][17];
int filelistsize;
int filepos[256];

int drawscr = 0;
int ddframes = 0;


void atom_reset(int power_on)
{
	debuglog("atom_reset(%d)\n",power_on);
//	memset(ram, 0, 0x10000);
	if(power_on)
	{
		the_cpu->mem[8] = rand();
		the_cpu->mem[9] = rand();
		the_cpu->mem[10] = rand();
		the_cpu->mem[11] = rand();
		
		// Clear BBC basic workspace.
		if(bbcmode)
			memset(&the_cpu->mem[0], 0, 0x10000);
	}
	resetvia();
	sid_reset();
	debuglog("exedir=%s\n",exedir);
	InitMMC();
	
	reset8271();
	gfxmode=0;
	reset6502();
	debuglog("atom_reset():done\n");

      // Init joystick
      // install_joystick(JOY_TYPE_AUTODETECT);

}

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* eventQueue;

void atom_init(int argc, char **argv)
{
	int c;
	int tapenext = 0, discnext = 0;

	debuglog("atom_init() done\n");


	for (c = 1; c < argc; c++)
	{
		if (!strcasecmp(argv[c], "--help"))
		{
			printf("Atomulator v1.10 command line options :\n\n");
			printf("-disc disc.ssd  - load disc.ssd into drives :0/:2\n");
			printf("-disc1 disc.ssd - load disc.ssd into drives :1/:3\n");
			printf("-tape tape.uef  - load tape.uef\n");
			printf("-fasttape       - set tape speed to fast\n");
			printf("-debug          - start debugger\n");
			exit(-1);
		}
		else
		if (!strcasecmp(argv[c], "-tape"))
		{
			tapenext = 2;
		}
		else if (!strcasecmp(argv[c], "-disc") || !strcasecmp(argv[c], "-disk"))
		{
			discnext = 1;
		}
		else if (!strcasecmp(argv[c], "-disc1"))
		{
			discnext = 2;
		}
		else if (!strcasecmp(argv[c], "-fasttape"))
		{
			fasttape = 1;
		}
		else if (!strcasecmp(argv[c], "-debug"))
		{
			debug = debugon = 1;
		}
		else if (tapenext)
			strcpy(tapefn, argv[c]);
		else if (discnext)
		{
			strcpy(discfns[discnext - 1], argv[c]);
			discnext = 0;
		}
		else
		{
			strcpy(discfns[0], argv[c]);
			discnext = 0;
		}
		if (tapenext)
			tapenext--;
	}
	initalmain(0, NULL);
	inituef();
	initmem();
	loadroms();
	reset6502();
	initvideo();
	init8255();
	disc_reset();
	reset8271();
	resetvia();
	#ifndef WIN32
	al_install_keyboard();
	#endif
	//install_mouse();

	
	// install_timer();
	// install_int_ex(scrupdate, BPS_TO_TIMER(300));



	inital();
	sid_init();
	sid_settype(sidmethod, cursid);
	// loaddiscsamps();
	loaddisc(0, discfns[0]);
	loaddisc(1, discfns[1]);
	atom_reset(1);


	timer = al_create_timer(ALLEGRO_BPS_TO_SECS(colourboard ? 50 : 60));	
	eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_start_timer(timer);


	debuglog("atom_init() done\n");
}

void changetimerspeed(int i)
{
  // remove_int(scrupdate);
  //	install_int_ex(scrupdate, BPS_TO_TIMER(i * 6));
  al_set_timer_speed(timer, ALLEGRO_BPS_TO_SECS(i));
}

int oldf12 = 0;
int count = 0;
int skipped = 0;

void atom_run()
{

  bool need_redraw = false;

  ALLEGRO_EVENT e;
  al_wait_for_event(eventQueue, &e);
  //debuglog("atom_run() event type = %d\n", e.type;)
  count++;

  // if (count == 60 * 30) {
  //quited = true;
  //}

  if (e.type == ALLEGRO_EVENT_TIMER) {

    need_redraw = true;
    ddframes++;
    drawscr++;
  } else {
    debuglog("atom_run() unexpected event type = %d\n", e.type);
  }


  if (need_redraw) {

    if (al_event_queue_is_empty(eventQueue)) {

      need_redraw=false;

      ALLEGRO_KEYBOARD_STATE key_state;
      int newf12;
    
      //debuglog("before exec6502");

      double t1 = al_get_time();

      // Alse executes a frame's worth of 6502 code...
      update_atom_display(colourboard ? 312 : 262);
      // debuglog("pc=%04x a=%02x x=%02x y=%02x s=%04x p=%02x\n", the_cpu->pc, the_cpu->a, the_cpu->x, the_cpu->y, the_cpu->s, the_cpu->p);

      //      if (count % 25 == 0) {
      //double t2 = al_get_time();
      //	debuglog("drawing frame %d took %d us\n", count, (int) (1000000.0 * (t2 - t1)));
      // }

      // debuglog("after exec6502");
	
      if (tapeon && fasttape)
	drawscr = 0;
      else
	drawscr -= 1; //(colourboard) ? 6 : 5;
		
      if (drawscr > 25)
	drawscr = 0;
		
      if (ddframes >= 25) {
	ddframes -= 25;
	// mixddnoise();
      }

      al_get_keyboard_state(&key_state);
      newf12 = al_key_down(&key_state, ALLEGRO_KEY_F12);
		
      if (newf12 && !oldf12)
	atom_reset(0);
		
      oldf12 = newf12;

    } else {
      skipped++;
    }

    if (count == 300) {
      debuglog("skipped %d out of %d frames (%.2f)%\n", skipped, count, 100.0 * skipped / count);
      count = 0;
      skipped = 0;
    }


      
  } 

  //debuglog("atom_run() done\n");
}

void atom_exit()
{
  saveconfig();
  //	closeddnoise();
  FinalizeMMC();
  //        dumpregs();
  //        dumpram();
}

