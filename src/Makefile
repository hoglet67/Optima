VPATH = . resid-fp atommc
CPP  = g++-4.7
CC   = gcc-4.7
DEBUG =
OPT   = -O3

#WINDRES = windres.exe
CFLAGS = $(DEBUG) $(OPT) -mfpu=vfp -mfloat-abi=hard -ffast-math -fomit-frame-pointer -falign-loops -falign-jumps -falign-functions -I/usr/local/include -I/usr/local/include/allegro5
CXXFLAGS = $(DEBUG) $(OPT)  -Wno-write-strings  -I/usr/local/include -I/usr/local/include/allegro5

# -march=i686 -ggdb
OBJ = 6502.o 6502asm.o 6522via.o 8255.o 8271.o atom.o config.o csw.o debugger.o disc.o fdi.o fdi2raw.o soundopenal.o ssd.o uef.o video.o linux.o resid.o ddnoise.o tgui/tgui2.o tgui/tgui2_widgets.o gui.o
SIDOBJ = convolve-sse.o convolve.o envelope.o extfilt.o filter.o pot.o sid.o voice.o wave6581__ST.o wave6581_P_T.o wave6581_PS_.o wave6581_PST.o wave8580__ST.o wave8580_P_T.o wave8580_PS_.o wave8580_PST.o wave.o
MMCOBJ = atommc.o atmmc2core.o atmmc2wfn.o ff_emu.o ff_emudir.o wildcard.o 

# linux-keydefine.o

LIBS =  -L/usr/local/lib -lallegro-static -lz -lopenal -lstdc++ -lm -L /opt/vc/lib -lGLESv2 -lX11  /usr/lib/libalut.a -lallegro_audio-static -lasound -lallegro_acodec-static  -lallegro_font-static  -lallegro_ttf-static -lfreetype -lallegro_color-static -l allegro_primitives-static -l allegro_image-static -ljpeg -lpng

Atomulator: $(OBJ) $(SIDOBJ) $(MMCOBJ)
	$(CC) $(DEBUG) $(OBJ) $(SIDOBJ) $(MMCOBJ) -o "Atomulator" $(LIBS) 

all : Atomulator

clean :
	rm *.o
	rm Atomulator
	rm *.res

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.o : %.cc
	$(CPP) $(CFLAGS) -c $<

atom.res: atom.rc
	$(WINDRES) -i atom.rc --input-format=rc -o atom.res -O coff 
