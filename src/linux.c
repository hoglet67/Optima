/*Atomulator v1.0 by Tom Walker
  Linux main*/

#ifndef WIN32

#include <stdio.h>
#include <allegro.h>
#include "atom.h"
#include "roms.h"

int quited = 0;

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


  int F1_pressed = 0;
  int F2_pressed = 0;
  int F3_pressed = 0;
  int F11_pressed = 0;
  int oldsndddnoise;

  while (!quited) {
      atom_run();
      // Poll the keyboard
      al_get_keyboard_state(&key_state);


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
	      popup("RamRom Enabled", 120);
	    } else {
	      sndddnoise = oldsndddnoise;
	      popup("RamRom Disabled", 120);
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
	      popup("Disc Noise Enabled", 120);
	    } else {
	      popup("Disc Noise Disabled", 120);
	    }
	    optima_gui_refresh();
	    F3_pressed = 1;
	  }
      } else {
	F3_pressed = 0;
      }

      // Toggle the Disc Noise if F3 is pressed
      if (al_key_down(&key_state, ALLEGRO_KEY_F11)) {
	  if (!F11_pressed) {
	   show_menu =  !show_menu;
	    if (show_menu) {
	      popup("Menu Enabled", 120);
	    } else {
	      popup("Menu Disabled", 120);
	    }
	    F11_pressed = 1;
	  }
      } else {
	F11_pressed = 0;
      }

      // Exit if F4 is pressed
      if (al_key_down(&key_state, ALLEGRO_KEY_F4)) {
	quited = 1;
      }
    }
  atom_exit();
  return 0;
}

END_OF_MAIN();

#endif
