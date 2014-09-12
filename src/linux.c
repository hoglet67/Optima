/*Atomulator v1.0 by Tom Walker
  Linux main*/

#ifndef WIN32

#include <stdio.h>
#include <allegro.h>
#include "atom.h"

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
    fprintf(stderr, "failed to initialize allegro audio!\n");
    return -1;
  }

  if (!al_init_acodec_addon()) {
    fprintf(stderr, "failed to initialize allegro audio codecs!\n");
    return -1;
  }

  if (!al_reserve_samples(16)) {
    fprintf(stderr, "failed to reserve samples!\n");
    return -1;
  }


  // al_install_system(ALLEGRO_VERSION_INT, NULL);

  //get_executable_name(exedir, 511);
  //p = get_filename(exedir);
  //p[0] = 0;
  ALLEGRO_PATH *path = al_create_path(argv[0]);
  al_set_path_filename(path, NULL);
  strcpy(exedir, al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));


  loadconfig();

  if (defaultwriteprot)
    writeprot[0] = writeprot[1] = 1;

  atom_init(argc, argv);
  al_install_keyboard();

  int F1_pressed = 0;
  int F2_pressed = 0;

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
	    if (ramrom_enable) {
	      // Disable RamRom
	      ramrom_enable = 0;
	      sndddnoise = 1;
	    } else {
	      // Enable RamRom
	      ramrom_enable = 1;
	      sndddnoise = 0;
	    }
	    atom_reset(0);
	    F2_pressed = 1;
	  }
      } else {
	F2_pressed = 0;
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
