/*Atomulator v1.0 by Tom Walker
   Disc drive noise*/

#include <allegro.h>
#include <allegro_audio.h>
#include <stdio.h>
#include <string.h>
#include "atom.h"

int ddvol = 3;

float ddnoise_mpos = 0;
int ddnoise_mstat = -1;
int oldmotoron = 0;

float ddnoise_spos = 0;
int ddnoise_sstat = -1;
int ddnoise_sdir = 0;
int ddtype = 0;

ALLEGRO_SAMPLE *seeksmp[4][2], *seek2smp, *seek3smp;
ALLEGRO_SAMPLE *stepsmp;
ALLEGRO_SAMPLE *motorsmp[3];

void loaddiscsamps()
{
	char path[512], p2[512];

	getcwd(p2, 511);
	if (ddtype)
		sprintf(path, "%sddnoise/35", exedir);
	else
		sprintf(path, "%sddnoise/525", exedir);
//        printf("path now %s\n",path);
	chdir(path);

	seeksmp[0][0] = al_load_sample("stepo.wav");
	if (seeksmp[0][0])
	{
		seeksmp[0][1] = al_load_sample("stepi.wav");
		seeksmp[1][0] = al_load_sample("seek1o.wav");
		seeksmp[1][1] = al_load_sample("seek1i.wav");
		seeksmp[2][0] = al_load_sample("seek2o.wav");
		seeksmp[2][1] = al_load_sample("seek2i.wav");
		seeksmp[3][0] = al_load_sample("seek3o.wav");
		seeksmp[3][1] = al_load_sample("seek3i.wav");
	}
	else
	{
		seeksmp[0][0] = al_load_sample("step.wav");
		seeksmp[0][1] = al_load_sample("step.wav");
		seeksmp[1][0] = al_load_sample("seek.wav");
		seeksmp[1][1] = al_load_sample("seek.wav");
		seeksmp[2][0] = al_load_sample("seek3.wav");
		seeksmp[2][1] = al_load_sample("seek3.wav");
		seeksmp[3][0] = al_load_sample("seek2.wav");
		seeksmp[3][1] = al_load_sample("seek2.wav");
	}
	motorsmp[0] = al_load_sample("motoron.wav");
	motorsmp[1] = al_load_sample("motor.wav");
	motorsmp[2] = al_load_sample("motoroff.wav");
	chdir(p2);
//        printf("done!\n");
}

void closeddnoise()
{
	int c;

	for (c = 0; c < 4; c++)
	{
		if (seeksmp[c][0])
			al_destroy_sample(seeksmp[c][0]);
		if (seeksmp[c][1])
			al_destroy_sample(seeksmp[c][1]);
		seeksmp[c][0] = seeksmp[c][1] = NULL;
	}
	for (c = 0; c < 3; c++)
	{
		if (motorsmp[c])
			al_destroy_sample(motorsmp[c]);
		motorsmp[c] = NULL;
	}
}

int16_t ddbuffer[4410];

void ddnoise_seek(int len)
{
//        printf("Seek %i tracks\n",len);
	ddnoise_sdir = (len < 0) ? 1 : 0;

	if (len < 0)
		len = -len;

	ddnoise_spos = 0;

	if (len == 0)
	{
		ddnoise_sstat = -1; fdctime = 200;
	}
	else if (len == 1)
	{
		ddnoise_sstat = 0; fdctime = 140000;
	}
	else if (len < 7)
		ddnoise_sstat = 1;
	else if (len < 30)
		ddnoise_sstat = 2;
	else
		ddnoise_sstat = 3;

	if (!sndddnoise)
		fdctime = 200;
//        rpclog("Start seek!\n");
}

FILE *f1, *f2;
void mixddnoise()
{
	int c;

//        if (!f1) f1=fopen("f1.pcm","wb");
//        if (!f2) f2=fopen("f2.pcm","wb");

	memset(ddbuffer, 0, 4410 * 2);
//        fwrite(ddbuffer,4410*2,1,f1);
	if (motoron && !oldmotoron)
	{
		ddnoise_mstat = 0;
		ddnoise_mpos = 0;
	}
	if (!motoron && oldmotoron)
	{
		ddnoise_mstat = 2;
		ddnoise_mpos = 0;
	}

	if (sndddnoise)
	{

		for (c = 0; c < 4410; c++)
		{
			ddbuffer[c] = 0;
			if (ddnoise_mstat >= 0)
			{
			  if (ddnoise_mpos >= al_get_sample_length(motorsmp[ddnoise_mstat]))
				{
					ddnoise_mpos = 0;
					if (ddnoise_mstat != 1)
						ddnoise_mstat++;
					if (ddnoise_mstat == 3)
						ddnoise_mstat = -1;
				}
				if (ddnoise_mstat != -1)
				{
//                                if (!c) rpclog("MixM!\n");

				  ddbuffer[c] += ((int16_t)((((int16_t*)al_get_sample_data(motorsmp[ddnoise_mstat]))[(int)ddnoise_mpos])));
				  ddnoise_mpos += ((float)al_get_sample_frequency(motorsmp[ddnoise_mstat]) / 44100.0);
				}
			}
		}
		for (c = 0; c < 4410; c++)
		{
			if (ddnoise_sstat >= 0)
			{
//                        rpclog("SSTAT %i %i\n",ddnoise_sstat,c);
			  if (ddnoise_spos >= al_get_sample_length(seeksmp[ddnoise_sstat][ddnoise_sdir]))
				{
					if (ddnoise_sstat > 0)
					{
						fdctime = 100;
//                                        printf("FDC time\n");
					}
					ddnoise_spos = 0;
					ddnoise_sstat = -1;
//                                printf("Stat off\n");
//                                rpclog("Stat off!\n");
				}
				else
				{
//                                if (!c) rpclog("MixS!\n");
				  ddbuffer[c] += ((int16_t)((((int16_t*)al_get_sample_data(seeksmp[ddnoise_sstat][ddnoise_sdir]))[(int)ddnoise_spos])));
				  ddnoise_spos += ((float)al_get_sample_frequency(seeksmp[ddnoise_sstat][ddnoise_sdir]) / 44100.0);
				}
			}
			ddbuffer[c] = (ddbuffer[c] / 3) * ddvol;
		}
	}

//        mixtapenoise(ddbuffer);
//        fwrite(ddbuffer,4410*2,1,f2);
//rpclog("Give buffer... %i %i\n",ddnoise_mstat,ddnoise_sstat);
	givealbufferdd(ddbuffer);

	oldmotoron = motoron;
}
