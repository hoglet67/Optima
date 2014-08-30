/*Atomulator v1.0 by Tom Walker
   Configuration handling*/

#include <string.h>
#include <stdio.h>
#include <allegro.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "atom.h"
#include "atommc.h"
#include "debugger.h"

#define LABEL_DISC0		"disc0"
#define LABEL_DISC1		"disc1"

#define LABEL_MMC_PATH	"mmc_path"

#define LABEL_COLOUR	"colourboard"
#define LABEL_BBCBASIC	"bbcbasic"
#define LABEL_SNOW		"snow"
#define LABEL_RAMROM	"ramrom_enable"
#define LABEL_RAMROMJMP	"ramrom_jumpers"

#define LABEL_FASTTAPE	"fasttape"

#define LABEL_DEF_WP 	"defaultwriteprot"
#define LABEL_DDVOL 	"ddvol"
#define LABEL_DDTYPE 	"ddtype"

#define LABEL_SND_INT 	"snd_internal"
#define LABEL_SND_TAPE 	"snd_tape"
#define LABEL_SND_DD 	"snd_ddnoise"

// Joystick support
#define LABEL_JOYSTICK  "joystick"

#define LABEL_KEY_DEF	"key_define_"
#define LABEL_USER_KBD	"user_keyboard"

#define LABEL_DEBUG_BRK	"debug_on_brk"

int snow;
int defaultwriteprot;
char discfns[2][260];

struct stat	statbuf;

int dir_exists(char *path)
{
	int			result = 0;
	
	if (0==stat(path,&statbuf))
		if(S_ISDIR(statbuf.st_mode))
			result=1;
		
	return result;
}

void load_config_string(ALLEGRO_CONFIG *cfg, char *section, char *label, char *dest)
{
	char *strtmp;	
	strtmp = (char*)al_get_config_value(cfg, section, label);
	if (strtmp)
		strcpy(dest, strtmp);
	else
		dest[0] = 0;
}

void save_config_string(ALLEGRO_CONFIG *cfg, char *section, char *label, char *value)
{
  al_set_config_value(cfg, section, label, value);
}

int load_config_int(ALLEGRO_CONFIG *cfg, char *section, char *label, int deflt) {
  char *val = al_get_config_value(cfg, section, label);
  if (val) {
    return atoi(val);
  } else {
    return deflt;
  }
}

void save_config_int(ALLEGRO_CONFIG *cfg, char *section, char *label, int value) {
  char sValue[30];
  sprintf(sValue, "%d", value);
  al_set_config_value(cfg, section, label, sValue);
}

void loadconfig()
{
	int c;
	char s[256];
	char *p;
	ALLEGRO_CONFIG *cfg;

	sprintf(s, "%satom.cfg", exedir);
	cfg = al_load_config_file(s);	
	
	load_config_string(cfg, NULL, LABEL_DISC0,discfns[0]);
	load_config_string(cfg, NULL, LABEL_DISC1,discfns[1]);
	load_config_string(cfg, NULL, LABEL_MMC_PATH,BaseMMCPath);
	//	strcpy(&discfns[0], al_get_config_value(cfg, NULL, LABEL_DISC0));
	//strcpy(&discfns[1], al_get_config_value(cfg, NULL, LABEL_DISC1));
	//strcpy(BaseMMCPath, al_get_config_value(cfg, NULL, LABEL_MMC_PATH));

	// check to see if the mmc path is valid and exists, else set to
	// the default.
	if((0==strlen(BaseMMCPath)) || (!dir_exists(BaseMMCPath)))
		sprintf(BaseMMCPath,"%s%s",exedir,DEF_MMC_DIR);
	
	colourboard 	= load_config_int(cfg, NULL, LABEL_COLOUR, 1);
	bbcmode 		= load_config_int(cfg, NULL, LABEL_BBCBASIC, 0);
	snow 			= load_config_int(cfg, NULL, LABEL_SNOW, 0);
	ramrom_enable 	= load_config_int(cfg, NULL, LABEL_RAMROM, 0);
	RR_jumpers 		= load_config_int(cfg, NULL, LABEL_RAMROMJMP, 0);

	fasttape 		= load_config_int(cfg, NULL, LABEL_FASTTAPE, 0);

	defaultwriteprot = load_config_int(cfg, NULL, LABEL_DEF_WP, 1);
	ddvol 			= load_config_int(cfg, NULL, LABEL_DDVOL, 2);
	ddtype 			= load_config_int(cfg, NULL, LABEL_DDTYPE, 0);

	spon 			= load_config_int(cfg, NULL, LABEL_SND_INT, 1);
	tpon 			= load_config_int(cfg, NULL, LABEL_SND_TAPE, 0);
	sndddnoise 		= load_config_int(cfg, NULL, LABEL_SND_DD, 1);

	// Joystick support
	joyst			= load_config_int(cfg, NULL, LABEL_JOYSTICK, 0);

	debug_on_brk	= load_config_int(cfg, NULL, LABEL_DEBUG_BRK, 0);

	for (c = 0; c < 128; c++)
	{
		sprintf(s, "%s%03i", LABEL_KEY_DEF, c);
		keylookup[c] = load_config_int(cfg, LABEL_USER_KBD, s, c);
	}
}

void saveconfig()
{
	int c;
	char s[256];

	ALLEGRO_CONFIG *cfg = al_create_config();
 
	save_config_string(cfg, NULL, LABEL_DISC0, discfns[0]);
	save_config_string(cfg, NULL, LABEL_DISC1, discfns[1]);
	save_config_string(cfg, NULL, LABEL_MMC_PATH,BaseMMCPath);

	save_config_int(cfg, NULL, LABEL_COLOUR, colourboard);
	save_config_int(cfg, NULL, LABEL_BBCBASIC, bbcmode);
	save_config_int(cfg, NULL, LABEL_SNOW, snow);
	save_config_int(cfg, NULL, LABEL_RAMROM,ramrom_enable);
	save_config_int(cfg, NULL, LABEL_RAMROMJMP,RR_jumpers);
	
	// Joystick support
	save_config_int(cfg, NULL, LABEL_JOYSTICK, joyst);
	
	save_config_int(cfg, NULL, LABEL_FASTTAPE, fasttape);

	save_config_int(cfg, NULL, LABEL_DEF_WP, defaultwriteprot);

	save_config_int(cfg, NULL, LABEL_DDVOL, ddvol);
	save_config_int(cfg, NULL, LABEL_DDTYPE, ddtype);

	save_config_int(cfg, NULL, LABEL_SND_INT, spon);
	save_config_int(cfg, NULL, LABEL_SND_TAPE, tpon);
	save_config_int(cfg, NULL, LABEL_SND_DD, sndddnoise);

	save_config_int(cfg, NULL, LABEL_DEBUG_BRK, debug_on_brk);

	for (c = 0; c < 128; c++)
	{
		sprintf(s, "%s%03i", LABEL_KEY_DEF, c);
		save_config_int(cfg, LABEL_USER_KBD, s, keylookup[c]);
	}


	sprintf(s, "%satom.cfg", exedir);
	al_save_config_file(s, cfg);

}
