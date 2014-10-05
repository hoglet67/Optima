#include <stdint.h>
#include <allegro.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXPATH	512

#define SNDBUFLEN 1024

#define POPUP_TIME 60
#define POPUP_DECAY 32

void rpclog(char *format, ...);
extern char exedir[MAXPATH+1];

void startblit();
void endblit();

int vbl;
int gfxmode;
int css;
int speaker;
uint8_t lastdat;
extern int cswena;
int cswpoint;

//#define printf rpclog

extern int colourboard;
extern int bbcmode;
extern int fasttape;
extern int ramrom_enable;
extern int RR_jumpers;
extern int RR_enables;
extern int quited;
extern int displayW;
extern int displayH;

extern ALLEGRO_BITMAP *atomscreen;

void lockAtomScreen();
void unlockAtomScreen();

#define ID_DISK0 0
#define ID_DISK1 1
#define ID_TAPE  2

#define NUM_IDS 3

extern char ejecttext[NUM_IDS][260];
void setejecttext(int id, const char *fn);

extern int tapecyc;

extern int sndtape;

int interrupt;


int fetchc[65536], readc[65536], writec[65536];
uint16_t pc;
uint8_t a, x, y, s;
struct
{
	int c, z, i, d, v, n;
} p;
//extern int nmi;
int debug, debugon;

//extern uint8_t opcode;

int spon, tpon;


void (*fdccallback)();
void (*fdcdata)(uint8_t dat);
void (*fdcspindown)();
void (*fdcfinishread)();
void (*fdcnotfound)();
void (*fdcdatacrcerror)();
void (*fdcheadercrcerror)();
void (*fdcwriteprotect)();
int (*fdcgetdata)(int last);

extern int writeprot[2], fwriteprot[2];

void ssd_reset();
void ssd_load(int drive, char *fn);
void ssd_close(int drive);
void dsd_load(int drive, char *fn);
void ssd_seek(int drive, int track);
void ssd_readsector(int drive, int sector, int track, int side, int density);
void ssd_writesector(int drive, int sector, int track, int side, int density);
void ssd_readaddress(int drive, int sector, int side, int density);
void ssd_format(int drive, int sector, int side, int density);
void ssd_poll();

void fdi_reset();
void fdi_load(int drive, char *fn);
void fdi_close(int drive);
void fdi_seek(int drive, int track);
void fdi_readsector(int drive, int sector, int track, int side, int density);
void fdi_writesector(int drive, int sector, int track, int side, int density);
void fdi_readaddress(int drive, int sector, int side, int density);
void fdi_format(int drive, int sector, int side, int density);
void fdi_poll();

void loaddisc(int drive, char *fn);
void newdisc(int drive, char *fn);
void closedisc(int drive);
void disc_reset();
void disc_poll();
void disc_seek(int drive, int track);
void disc_readsector(int drive, int sector, int track, int side, int density);
void disc_writesector(int drive, int sector, int track, int side, int density);
void disc_readaddress(int drive, int track, int side, int density);
void disc_format(int drive, int track, int side, int density);
extern int defaultwriteprot;
extern char discfns[2][260];


void loaddiscsamps();
void mixddnoise();
extern int ddvol, ddtype;

extern int motorspin;
extern int fdctime;
extern int motoron;
extern int disctime;

struct
{
	void (*seek)(int drive, int track);
	void (*readsector)(int drive, int sector, int track, int side, int density);
	void (*writesector)(int drive, int sector, int track, int side, int density);
	void (*readaddress)(int drive, int track, int side, int density);
	void (*format)(int drive, int track, int side, int density);
	void (*poll)();
} drives[2];

extern int curdrive;

extern int sndddnoise;
extern int sndatomsid;
extern int sndsidfilter;
extern int cursid;
extern int sidmethod;
extern unsigned char joyst;

extern int show_menu;

void opencsw(char *fn);
void closecsw();
int getcsw();
void findfilenamescsw();

void inituef();
void polluef();
void openuef(char *fn);
void closeuef();
void rewindit();
int getftell();
void findfilenamesuef();

void initalmain(int argc, char *argv[]);
void inital();
void givealbuffer(int16_t *buf);
void givealbufferdd(int16_t *buf);

void reset6502();
void exec6502();
void dumpregs();

void initmem();
void loadroms();
void dumpram();

void initvideo();
void popup(int time, char *message, ...);
void drawline(int l);
void updatepal();

void reset8271();
uint8_t read8271(uint16_t addr);
void write8271(uint16_t addr, uint8_t val);

void loaddiscsamps();
void closeddnoise();
void ddnoise_seek(int len);

void pollsound();
void polltape();

void init8255();
void write8255(uint16_t addr, uint8_t val);
uint8_t read8255(uint16_t addr);
void receive(uint8_t dat);
void dcd();
void dcdlow();

void writevia(uint16_t addr, uint8_t val);
uint8_t readvia(uint16_t addr);
void resetvia();
void updatetimers();

void startdebug();
void enddebug();
void killdebug();
void debugread(uint16_t addr);
void debugwrite(uint16_t addr, uint8_t val);
void dodebugger();
void debuglog(char *format, ...);

void cataddname(char *s);

void atom_init(int argc, char **argv);
void atom_run();
void atom_exit();
void atom_reset(int power_on);

void setquit();

uint8_t readmeml(uint16_t addr);
void writememl(uint16_t addr, uint8_t val);

extern int winsizex, winsizey;


void redefinekeys();
extern int keylookup[128];

void loadconfig();
void saveconfig();
void set_rr_ptrs();

extern int snow;
extern int fullscreen;
void enterfullscreen();
void leavefullscreen();

void updatewindowsize(int x, int y);
void loadtape(char *fn);

void clearscreen();

#ifndef WIN32
void entergui();
#endif

extern char tapefn[260];
extern int emuspeed, fskipmax;
extern char scrshotname[260];
extern int savescrshot;

void changetimerspeed(int i);

#ifdef __cplusplus
}
#endif
