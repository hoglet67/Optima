/*
 * fastsid.h - MOS6581 (SID) emulation.
 *
 * Written by
 *  Teemu Rantanen <tvr@cs.hut.fi>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#ifndef _FASTSID_H
#define _FASTSID_H

#include "types.h"
#include "fixpoint.h"

/* use wavetables (sampled waveforms) */
// slower on embedded device, due to cache thrashing to load memory
// #define WAVETABLES

// Sample output frequency in Hz
#define SAMPLE_FREQ 31250

// SID frequency in Hz
#define SID_FREQ 985248

// Size of buffer for sampled voice (double buffered)
#define SAMPLE_BUF_SIZE (0x138*2)

// Enable emulation of the SID Filter (more CPU intensive)
#define EMULATE_FILTER 1

/* needed data for one voice */
typedef struct voice_s
{
    struct sound_s	*s;
    struct voice_s	*vprev;
    struct voice_s	*vnext;
    int			 nr;

    /* counter value */
    DWORD		 f;
    /* counter step / sample */
    DWORD		 fs;
#ifdef WAVETABLES
    /* do we have noise enabled? */
    BYTE		 noise;
#else
    /* waveform that we use */
    BYTE		 fm;
    /* pulse threshold compared to the 32-bit counter */
    DWORD		 pw;
#endif

    /* 31-bit adsr counter */
    DWORD		 adsr;
    /* adsr counter step / sample */
    SDWORD		 adsrs;
    /* adsr sustain level compared to the 31-bit counter */
    DWORD		 adsrz;

    /* does this voice use hard sync? */
    BYTE		 sync;
    /* does this voice use filter? */
    BYTE		 filter;
    /* does this structure need updating before next sample? */
    BYTE		 update;
    /* did we do multiple gate flips after last calculated sample? */
    BYTE		 gateflip;

    /* ADSR mode */
    BYTE		 adsrm;
    /* 4-bit attack value */
    BYTE		 attack;
    /* 4-bit decay value */
    BYTE		 decay;
    /* 4-bit sustain value */
    BYTE		 sustain;
    /* 4-bit release value */
    BYTE		 release;

    /* pointer to registers of this voice */
    BYTE		*d;

    /* noise shift register. Note! rv may be 0 to 15 shifts 'behind' the
       real noise shift register value. Remaining shifts are done when
       it is referenced */
    DWORD		 rv;
#ifdef WAVETABLES
    /* pointer to wavetable data */
    WORD		*wt;
    /* 32-bit offset to add to the counter before referencing the wavetable.
       This is used on combined waveforms, when other waveforms are combined
       with pulse */
    DWORD		 wtpf;
    /* length of wavetable (actually number of shifts needed for 32-bit
       counter) */
    DWORD		 wtl;
    /* kludge for ring modulation. Set wtr[1] = 0x7fff if ring modulation is
       used */
    WORD		 wtr[2];
#endif

    signed char		filtIO;
    vreal_t		filtLow, filtRef;
} voice_t;

/* needed data for SID */
struct sound_s
{
    /* number of voices */
    voice_t              v[3];
    /* SID registers */
    BYTE                 d[32];
    /* is voice 3 enabled? */
    BYTE                 has3;
    /* 4-bit volume value */
    BYTE                 vol;

    /* ADSR counter step values for each adsr values */
    SDWORD               adrs[16];
    /* sustain values compared to 31-bit ADSR counter */
    DWORD                sz[16];

    /* internal constant used for sample rate dependent calculations */
    DWORD                speed1;

    /* does this structure need updating before next sample? */
    BYTE                 update;
#ifdef WAVETABLES
    /* do we have a new sid or an old one? */
    BYTE                 newsid;
#endif
    /* constants needed to implement write-only register reads */
    BYTE                 laststore;
    BYTE                 laststorebit;
    //CLOCK                laststoreclk;
    /* do we want to use filters? */
    int                  emulatefilter;

    /* filter variables */
    vreal_t              filterDy;
    vreal_t              filterResDy;
    BYTE                 filterType;
    BYTE                 filterCurType;
    WORD                 filterValue;
	
	BYTE*				 sample_buf;
};

typedef struct sound_s sound_t;

#endif

