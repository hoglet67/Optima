/*Atomulator v1.0 by Tom Walker
  Linux main*/

#ifndef WIN32

#include <stdio.h>
#include <allegro.h>
#include "atom.h"
#include "roms.h"

int quited = 0;

char catnames[256][260];
int catnum = 0;

void startblit()
{
}
void endblit()
{
}
void updatewindowsize(int x, int y)
{
}

int keylookup[128];

void cataddname(char *s)
{
  if (catnum < 256) {
      strcpy(catnames[catnum], s);
      rpclog("%s\n", s); 
      catnum++;
    }
}

int main(int argc, char **argv)
{
  char *p;
  int newf11;
  ALLEGRO_KEYBOARD_STATE key_state;

  if (!al_init()) {
    fprintf(stderr, "failed to initialize allegro!\n");
    return -1;
  }

  if (!al_install_audio()) {
    fprintf(stderr, "failed to initialize allegro audio addon!\n");
    return -1;
  }

  if (!al_init_acodec_addon()) {
    fprintf(stderr, "failed to initialize allegro audio codec addon!\n");
    return -1;
  }

  if (!al_reserve_samples(16)) {
    fprintf(stderr, "failed to reserve samples!\n");
    return -1;
  }

  if (!al_init_font_addon()) {
    fprintf(stderr, "failed to initialize allegro font addon!\n");
    return -1;
  }

  if (!al_init_ttf_addon()) {
    fprintf(stderr, "failed to initialize allegro ttf addon!\n");
    return -1;
  }

  if (!al_init_primitives_addon()) {
    fprintf(stderr, "failed to initialize allegro primitives addon!\n");
    return -1;
  }

  if (!al_init_image_addon()) {
    fprintf(stderr, "failed to initialize allegro image addon!\n");
    return -1;
  }

  ALLEGRO_PATH *path = al_create_path(argv[0]);
  al_set_path_filename(path, NULL);
  strcpy(exedir, al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));


  loadconfig();

  if (bbcmode) {
    RR_enables |= RAMROM_FLAG_BBCMODE;
  }

  if (defaultwriteprot) {
    writeprot[0] = writeprot[1] = 1;
  }

  al_install_keyboard();

  if (!al_install_mouse()) {
    fprintf(stderr, "failed to install mouse driver!\n");
    return -1;
  }

  atom_init(argc, argv);



#ifdef FUNCTION_KEYS
  int F1_pressed = 0;
  int F2_pressed = 0;
  int F3_pressed = 0;
#endif

  int F11_pressed = 0;
  int oldsndddnoise;

  while (!quited) {
      atom_run();
      // Poll the keyboard
      al_get_keyboard_state(&key_state);


#ifdef FUNCTION_KEYS

      // Toggle the pallette if F1 is pressed
      if (al_key_down(&key_state, ALLEGRO_KEY_F1)) {
	  if (!F1_pressed) {
	    togglepal();
	    F1_pressed = 1;
	  }
      } else {
	F1_pressed = 0;
      }

      // Toggle the ramrom support if F2 is pressed
      if (al_key_down(&key_state, ALLEGRO_KEY_F2)) {
	  if (!F2_pressed) {
	    ramrom_enable = !ramrom_enable;
	    if (ramrom_enable) {
	      oldsndddnoise = sndddnoise;
	      sndddnoise = 0;
	      popup(POPUP_TIME, "RamRom Enabled");
	    } else {
	      sndddnoise = oldsndddnoise;
	      popup(POPUP_TIME, "RamRom Disabled");
	    }
	    optima_gui_refresh();
	    atom_reset(0);
	    F2_pressed = 1;
	  }
      } else {
	F2_pressed = 0;
      }

      // Toggle the Disc Noise if F3 is pressed
      if (al_key_down(&key_state, ALLEGRO_KEY_F3)) {
	  if (!F3_pressed) {
	    sndddnoise =  !sndddnoise;
	    if (sndddnoise) {
	      popup(POPUP_TIME, "Disc Noise Enabled");
	    } else {
	      popup(POPUP_TIME, "Disc Noise Disabled");
	    }
	    optima_gui_refresh();
	    F3_pressed = 1;
	  }
      } else {
	F3_pressed = 0;
      }
#endif

      // Exit if F4 is pressed
      if (al_key_down(&key_state, ALLEGRO_KEY_F4)) {
	quited = 1;
      }

      // Toggle the Disc Noise if F3 is pressed
      if (al_key_down(&key_state, ALLEGRO_KEY_F11)) {
	  if (!F11_pressed) {
	   show_menu =  !show_menu;
	    if (show_menu) {
	      popup(POPUP_TIME, "Menu enabled");
	    } else {
	      popup(POPUP_TIME, "Menu disabled");
	    }
	    F11_pressed = 1;
	  }
      } else {
	F11_pressed = 0;
      }
  }
  atom_exit();
  return 0;
}

END_OF_MAIN();

#endif
