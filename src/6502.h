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
  uint8_t unused; //
  int interrupt; // +28
  int nmi;	  // +32
  int takeint;  // +36
  int cycles;   // +40
  uint16_t pc_trigger_hit; //+44
  uint16_t pc_triggers[4]; //+46
  uint32_t cyclesTotal;	// +56
} M6502;

extern M6502* the_cpu;

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
