/*
 * 6502.h
 *
 *
 * Written by Reuben Scratton, based on original code by Tom Walker
 *
 */
#ifndef M6502_H_INCLUDED
#define M6502_H_INCLUDED

#include <stdint.h>
#include <string.h>

#define FLAG_C 1
#define FLAG_Z 2
#define FLAG_I 4
#define FLAG_D 8
#define FLAG_V 0x40
#define FLAG_N 0x80

typedef struct VIA
{
  int t1c;	// accessed by 6502asm +0
  int t2c;	// accessed by 6502asm +4
  uint8_t acr;	// accessed by 6502asm +8
  uint8_t pcr;
  uint8_t ifr;
  uint8_t ier;
  uint8_t ora;
  uint8_t orb;
  uint8_t ira;
  uint8_t irb;
  uint8_t ddra;
  uint8_t ddrb;
  uint8_t porta;
  uint8_t portb;
  uint32_t t1l;
  uint32_t t2l;
  int t1hit;
  int t2hit;
} VIA;

typedef struct {
  int writeMask0;
  int writeMask1;
  int writeMask2;
  int writeMask3;
  uint8_t* mem; // +16
  uint16_t pc;  // +20
  uint8_t a;    // +22
  uint8_t x;    // +23
  uint8_t y;    // +24
  uint8_t s;    // +25
  uint8_t p;    // +26
  uint8_t unused; // +27
  int interrupt; // +28
  int nmi;	  // +32
  int takeint;  // +36
  int cycles;   // +40
  uint16_t pc_trigger_hit; //+44
  uint16_t pc_triggers[2]; //+46
  uint16_t pc_tapeon; //+50
  uint16_t pc_tapeoff; //+52  
  uint16_t tapeon; //+54
  uint32_t tapecyc;	// +56
  uint32_t cyclesTotal;	// +60
  VIA via; // +64
} M6502;

extern M6502* the_cpu;
extern VIA *via;

extern uint8_t *roms;
extern int swram[16];
extern int motorspin;
extern int fdctime;
extern int motoron;
extern int disctime;
extern int frames;


void reset6502();
void exec6502();

#define readmem(x)  ((x<0xb000 || x>=0xc000) ? the_cpu->mem[x] : readmem_ex(x))
#define readword(x) ((x<0xb000 || x>=0xc000) ? (*((uint16_t*)&(the_cpu->mem[x]))) : (readmem_ex(x) | (readmem_ex(x+1)<<8)))
#define readwordpc() readword(the_cpu->pc); the_cpu->pc+=2
#define writemem(x, val) if (((uint16_t)x)<0xa000u) the_cpu->mem[x]=val; else writemem_ex(x, val)
///#define setzn(v) the_cpu->p_z=!(v); the_cpu->p_n=(v)&0x80
#define SET_FLAG(flag,val) if (val) the_cpu->p |= flag; else the_cpu->p &= ~flag
#define setzn(v) if(v) the_cpu->p &=~FLAG_Z; else the_cpu->p|=FLAG_Z;  if ((v)&0x80) the_cpu->p|=FLAG_N; else the_cpu->p&=~FLAG_N;
#define push(v) the_cpu->mem[0x100+(the_cpu->s--)]=v
#define pull()  the_cpu->mem[0x100+(++the_cpu->s)]


#endif
