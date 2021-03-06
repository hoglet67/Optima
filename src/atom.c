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

//uint8_t *ram;
char filelist[256][17];
int filelistsize;
int filepos[256];

int ddframes = 0;


void atom_reset(int power_on)
{
	rpclog("atom_reset(%d)\n",power_on);
//	memset(ram, 0, 0x10000);
	if(power_on)
	{
		the_cpu->mem[8] = rand();
		the_cpu->mem[9] = rand();
		the_cpu->mem[10] = rand();
		the_cpu->mem[11] = rand();
	}
	resetvia();
	sid_reset();
	rpclog("exedir=%s\n",exedir);
	InitMMC();
	
	reset8271();
	gfxmode=0;
	reset6502();

	if (!al_install_joystick()) {
	  rpclog("Failed to install joystick");
	}

	rpclog("atom_reset():done\n");

}

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* eventQueue;

void atom_init(int argc, char **argv)
{
	int c;
	int tapenext = 0, discnext = 0;

	rpclog("atom_init() done\n");


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

	inital();
	sid_init();
	sid_settype(sidmethod, cursid);
	loaddiscsamps();
	loaddisc(0, discfns[0]);
	loaddisc(1, discfns[1]);
	atom_reset(1);


	timer = al_create_timer(ALLEGRO_BPS_TO_SECS(refreshRate));	
	eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	if (mousePresent) {
	  al_register_event_source(eventQueue, al_get_mouse_event_source());
	}

	rpclog("atom_init() done\n");
}

void updateTimer()
{
  refreshRate = pal ? 50 : 60;
  al_set_timer_speed(timer, ALLEGRO_BPS_TO_SECS(refreshRate));
}

int oldf12 = 0;
int count = 0;
int skipped = 0;
int menu_timer = 0;

void atom_run()
{

  bool need_redraw = false;

  ALLEGRO_EVENT e;
  al_wait_for_event(eventQueue, &e);

  //rpclog("atom_run() event type = %d\n", e.type;)
  
  if (e.type == ALLEGRO_EVENT_TIMER) {

    need_redraw = true;

    count++;
    ddframes++;
    if (menu_timer > 0) {
      menu_timer--;
      if (menu_timer == 0) {
	show_menu = false;
      }
    }
    if (show_menu) {
      optima_gui_update();
    }

  } else {
    if (e.type == ALLEGRO_EVENT_MOUSE_AXES) {
      if (show_menu) {
	menu_timer = 120;
      } else {
	menu_timer++;
	if (menu_timer > 15) {
	  show_menu = true;
	}
      }
    }
    if (show_menu) {
      optima_gui_handleEvent(&e);
    }
  } 

  if (need_redraw) {
    
    // All bets are off when running in fast tape mode....
    int fasttapeactive = the_cpu->tapeon && fasttape && !show_menu;
    int skip = !al_event_queue_is_empty(eventQueue) && !fasttapeactive;

    if (skip) {
      skipped++;
    }
    need_redraw=false;

    ALLEGRO_KEYBOARD_STATE key_state;
    int newf12;
        
    // double t1 = al_get_time();
    // Alse executes a frame's worth of 6502 code...

    update_atom_display((pal ? 312 : 262) * (fasttapeactive ? 5 : 1), skip);
    // rpclog("pc=%04x a=%02x x=%02x y=%02x s=%04x p=%02x\n", the_cpu->pc, the_cpu->a, the_cpu->x, the_cpu->y, the_cpu->s, the_cpu->p);


    if (ddframes >= 5) {
      ddframes -= 5 ;
      mixddnoise();
    }

    al_get_keyboard_state(&key_state);
    newf12 = al_key_down(&key_state, ALLEGRO_KEY_F12);
		
    if (newf12 && !oldf12)
      atom_reset(0);
		
    oldf12 = newf12;


    if (count == 300) {
      rpclog("skipped %d out of %d frames (%.2f)%\n", skipped, count, 100.0 * skipped / count);
      count = 0;
      skipped = 0;
    }

  } 

  //rpclog("atom_run() done\n");
}

void atom_exit()
{
  saveconfig();
  closeddnoise();
  FinalizeMMC();
  //        dumpregs();
  //        dumpram();
}

