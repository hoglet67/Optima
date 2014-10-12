#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "fastsid.h"

int sndsidfilter=1;

int sidrunning=0;

int sample_in_ptr;

uint8_t sample_buf[SAMPLE_BUF_SIZE];

sound_t _fastSID;

extern void rpclog();

void sid_reset() {
  rpclog("sid_reset() called\n");
  fastsid_reset(&_fastSID);
  sample_in_ptr = 0;
  memset(sample_buf, 0, SAMPLE_BUF_SIZE);
  sidrunning=0;
}

void sid_init() {
  rpclog("sid_init() called\n");
  bzero(&_fastSID, sizeof(sound_t));
  _fastSID.sample_buf = sample_buf;
  fastsid_init(&_fastSID, SAMPLE_FREQ, SID_FREQ, sndsidfilter);
  sid_reset();
}

void sid_settype(int resamp, int model) {
  rpclog("sid_settype is not implemented\n");
}

uint8_t sid_read(uint16_t addr) {
  rpclog("sid_read is not implemented\n");
}

void sid_write(uint16_t addr, uint8_t val) {
  sidrunning=1;
  fastsid_store(&_fastSID, addr, val);
}

void sid_fillbuf(int16_t *buf, int len) {
  if (sidrunning) {
    fastsid_calculate_samples(&_fastSID, buf, len, sample_in_ptr);
  } else {
    memset(buf,0,len*2);
  }
}


