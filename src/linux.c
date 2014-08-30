/*Atomulator v1.0 by Tom Walker
   Linux main*/

#ifndef WIN32

#include <stdio.h>
#include <allegro.h>
#include "atom.h"

int quited = 0;
extern int oldf11;

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

	al_init();

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
	while (!quited)
	{
		atom_run();

		al_get_keyboard_state(&key_state);
		newf11 = al_key_down(&key_state, ALLEGRO_KEY_F11);
		if (newf11 && !oldf11)
		{
			oldf11 = 1;
			// entergui();
		}
		oldf11 = newf11;
	}
	atom_exit();
	return 0;
}

END_OF_MAIN();

#endif
