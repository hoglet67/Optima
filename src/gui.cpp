#include "tgui/tgui2.hpp"
#include "tgui/tgui2_widgets.hpp"
#include <allegro.h>
#include <allegro_color.h>

#include "atom.h"
#include "roms.h"

extern "C" {
  void optima_gui_init(ALLEGRO_DISPLAY *display, int displayW, int displayH, ALLEGRO_FONT *font, int menuFontSize);
  void optima_gui_handleEvent(ALLEGRO_EVENT *event);
  void optima_gui_refresh();
  void optima_gui_update();
  void optima_gui_draw();
}

TGUI_TextMenuItem *file_reset;
TGUI_TextMenuItem *file_exit;

TGUI_TextMenuItem *tape_load;
TGUI_TextMenuItem *tape_eject;
TGUI_TextMenuItem *tape_rewind;
TGUI_TextMenuItem *tape_catalog;
TGUI_RadioMenuItem *tape_speed_normal;
TGUI_RadioMenuItem *tape_speed_fast;

TGUI_TextMenuItem *disc_load0;
TGUI_TextMenuItem *disc_load1;
TGUI_TextMenuItem *disc_eject0;
TGUI_TextMenuItem *disc_eject1;
TGUI_TextMenuItem *disc_new0;
TGUI_TextMenuItem *disc_new1;
TGUI_CheckMenuItem *disc_writeprot0;
TGUI_CheckMenuItem *disc_writeprot1;
TGUI_CheckMenuItem *disc_defaultwriteprot;

TGUI_CheckMenuItem *settings_video_snow;
TGUI_CheckMenuItem *settings_video_fullscreen;

TGUI_CheckMenuItem *settings_hardware_colourboard;
TGUI_CheckMenuItem *settings_hardware_bbcmode;

TGUI_CheckMenuItem *settings_ramrom_ramrom;
TGUI_CheckMenuItem *settings_ramrom_diskrom;

TGUI_CheckMenuItem *settings_sound_internalspeaker;
TGUI_CheckMenuItem *settings_sound_atomsid;
TGUI_CheckMenuItem *settings_sound_tapenoise;
TGUI_CheckMenuItem *settings_sound_discnoise;

TGUI_RadioMenuItem *settings_sound_ddtype_525;
TGUI_RadioMenuItem *settings_sound_ddtype_35;

TGUI_RadioMenuItem *settings_sound_ddvol_33;
TGUI_RadioMenuItem *settings_sound_ddvol_66;
TGUI_RadioMenuItem *settings_sound_ddvol_100;

TGUI_TextMenuItem *settings_keyboard_redefine;
TGUI_TextMenuItem *settings_keyboard_default;

TGUI_TextMenuItem *misc_screenshot;

TGUI_MenuBar *menuBar;

TGUI_RadioGroup tapeSpeedGroup;
TGUI_RadioGroup settingsSoundDDTypeGroup;
TGUI_RadioGroup settingsSoundDDVolGroup;

std::vector<tgui::TGUIWidget *> fileItems;
std::vector<tgui::TGUIWidget *> tapeSpeedItems;
std::vector<tgui::TGUIWidget *> tapeItems;
std::vector<tgui::TGUIWidget *> discItems;
std::vector<tgui::TGUIWidget *> settingsItems;
std::vector<tgui::TGUIWidget *> settingsVideoItems;
std::vector<tgui::TGUIWidget *> settingsHardwareItems;
std::vector<tgui::TGUIWidget *> settingsRamRomItems;
std::vector<tgui::TGUIWidget *> settingsSoundItems;
std::vector<tgui::TGUIWidget *> settingsKeyboardItems;
std::vector<tgui::TGUIWidget *> settingsSoundDDTypeItems;
std::vector<tgui::TGUIWidget *> settingsSoundDDVolItems;
std::vector<tgui::TGUIWidget *> miscItems;

std::vector<std::string> menu_names;
std::vector<TGUI_Splitter *> menu_splitters;

void optima_gui_init(ALLEGRO_DISPLAY *display, int displayW, int displayH, ALLEGRO_FONT *font, int menuFontSize) {
  tgui::init(display);
  tgui::setFont(font);

  tguiWidgetsSetColors(al_color_name("cyan"), al_color_name("darkgray"));

  int menuWidth = displayW / 6;
  int menuHeight = menuFontSize * 4 / 3;

  // Create the file menu
  fileItems.push_back(file_reset = new TGUI_TextMenuItem("Reset", 0)); 
  fileItems.push_back(file_exit = new TGUI_TextMenuItem("Exit", 0)); 
  TGUI_Splitter *fileMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * fileItems.size(), TGUI_VERTICAL, true, fileItems);

  // Create the tape speed menu
  tapeSpeedGroup.selected = 0;
  tapeSpeedItems.push_back(tape_speed_normal = new TGUI_RadioMenuItem("Normal", 0, &tapeSpeedGroup, 0));
  tapeSpeedItems.push_back(tape_speed_fast = new TGUI_RadioMenuItem("Fast", 0, &tapeSpeedGroup, 1));
  TGUI_Splitter *tapeSpeedMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * tapeSpeedItems.size(), TGUI_VERTICAL, false, tapeSpeedItems);

  // Create the tape menu
  tapeItems.push_back(tape_load = new TGUI_TextMenuItem("Load tape...", 0));
  tapeItems.push_back(tape_eject = new TGUI_TextMenuItem("Eject tape", 0));
  tapeItems.push_back(tape_rewind = new TGUI_TextMenuItem("Rewind tape", 0));
  tapeItems.push_back(tape_catalog = new TGUI_TextMenuItem("Show tape catalogue", 0));
  tapeItems.push_back(new TGUI_SubMenuItem("TapeSpeed", tapeSpeedMenu));
  TGUI_Splitter *tapeMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * tapeItems.size(), TGUI_VERTICAL, false, tapeItems);

  // Create the disc menu
  discItems.push_back(disc_load0 = new TGUI_TextMenuItem("Load disc: 0/2...", 0));
  discItems.push_back(disc_load1 = new TGUI_TextMenuItem("Load disc: 1/3...", 0));
  discItems.push_back(disc_eject0 = new TGUI_TextMenuItem("Eject disc: 0/2", 0));
  discItems.push_back(disc_eject1 = new TGUI_TextMenuItem("Eject disc: 1/3", 0));
  discItems.push_back(disc_new0 = new TGUI_TextMenuItem("New disc: 0/2", 0));
  discItems.push_back(disc_new1 = new TGUI_TextMenuItem("New disc: 1/3", 0));
  discItems.push_back(disc_writeprot0 = new TGUI_CheckMenuItem("Write protect disc: 0/2", 0, writeprot[0]));
  discItems.push_back(disc_writeprot1 = new TGUI_CheckMenuItem("Write protect disc: 1/3", 0, writeprot[1]));
  discItems.push_back(disc_defaultwriteprot = new TGUI_CheckMenuItem("Default write protect", 0, defaultwriteprot));
  TGUI_Splitter *discMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * discItems.size(), TGUI_VERTICAL, false, discItems);

  // Create the settings/video menu
  settingsVideoItems.push_back(settings_video_snow = new TGUI_CheckMenuItem("Snow", 0, snow));
  settingsVideoItems.push_back(settings_video_fullscreen = new TGUI_CheckMenuItem("Fullscreen", 0, fullscreen));
  TGUI_Splitter *settingsVideoMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsVideoItems.size(), TGUI_VERTICAL, false, settingsVideoItems);

  // Create the settings/hardware menu
  settingsHardwareItems.push_back(settings_hardware_colourboard = new TGUI_CheckMenuItem("Colour Board", 0, colourboard));
  settingsHardwareItems.push_back(settings_hardware_bbcmode = new TGUI_CheckMenuItem("BBC BASIC", 0, bbcmode));
  TGUI_Splitter *settingsHardwareMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsHardwareItems.size(), TGUI_VERTICAL, false, settingsHardwareItems);
  
  // Create the settings/ramrom menu
  settingsRamRomItems.push_back(settings_ramrom_ramrom = new TGUI_CheckMenuItem("RAM/ROM Enabled", 0, ramrom_enable));
  settingsRamRomItems.push_back(settings_ramrom_diskrom = new TGUI_CheckMenuItem("RAM/ROM Diskrom Enabled", 0, (RR_jumpers & RAMROM_FLAG_DISKROM)));
  TGUI_Splitter *settingsRamRomMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsRamRomItems.size(), TGUI_VERTICAL, false, settingsRamRomItems);

  // Create the settings/sound/ddtype menu
  settingsSoundDDTypeGroup.selected = ddtype;
  settingsSoundDDTypeItems.push_back(settings_sound_ddtype_525 = new TGUI_RadioMenuItem("5.25", 0, &settingsSoundDDTypeGroup, 0));
  settingsSoundDDTypeItems.push_back(settings_sound_ddtype_35 = new TGUI_RadioMenuItem("3.5", 0, &settingsSoundDDTypeGroup, 1));
  TGUI_Splitter *settingsSoundDDTypeMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsSoundDDTypeItems.size(), TGUI_VERTICAL, false, settingsSoundDDTypeItems);

  // Create the settings/sound/ddvol menu
  settingsSoundDDVolGroup.selected = ddvol - 1;
  settingsSoundDDVolItems.push_back(settings_sound_ddvol_33 = new TGUI_RadioMenuItem("33%", 0, &settingsSoundDDVolGroup, 0));
  settingsSoundDDVolItems.push_back(settings_sound_ddvol_66 = new TGUI_RadioMenuItem("66%", 0,  &settingsSoundDDVolGroup, 1));
  settingsSoundDDVolItems.push_back(settings_sound_ddvol_100 = new TGUI_RadioMenuItem("100%", 0,  &settingsSoundDDVolGroup, 2));
  TGUI_Splitter *settingsSoundDDVolMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsSoundDDVolItems.size(), TGUI_VERTICAL, false, settingsSoundDDVolItems);

  // Create the settings/sound menu
  settingsSoundItems.push_back(settings_sound_internalspeaker = new TGUI_CheckMenuItem("Internal speaker", 0, spon));
  settingsSoundItems.push_back(settings_sound_atomsid = new TGUI_CheckMenuItem("AtomSID", 0, sndatomsid));
  settingsSoundItems.push_back(settings_sound_tapenoise = new TGUI_CheckMenuItem("Tape noise", 0, tpon));
  settingsSoundItems.push_back(settings_sound_discnoise = new TGUI_CheckMenuItem("Disc drive noise", 0, sndddnoise));
  settingsSoundItems.push_back(new TGUI_SubMenuItem("Disc drive type", settingsSoundDDTypeMenu));
  settingsSoundItems.push_back(new TGUI_SubMenuItem("Disc drive vol", settingsSoundDDVolMenu));
  TGUI_Splitter *settingsSoundMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsSoundItems.size(), TGUI_VERTICAL, false, settingsSoundItems);

  // Create the settings/keyboard menu
  settingsRamRomItems.push_back(settings_keyboard_redefine = new TGUI_TextMenuItem("Redefine keys...", 0));
  settingsRamRomItems.push_back(settings_keyboard_default = new TGUI_TextMenuItem("Default mapping", 0));
  TGUI_Splitter *settingsKeyboardMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsKeyboardItems.size(), TGUI_VERTICAL, false, settingsKeyboardItems);

  // Create the settings menu
  settingsItems.push_back(new TGUI_SubMenuItem("Video", settingsVideoMenu));
  settingsItems.push_back(new TGUI_SubMenuItem("Hardware", settingsHardwareMenu));
  settingsItems.push_back(new TGUI_SubMenuItem("RamRom", settingsRamRomMenu));
  settingsItems.push_back(new TGUI_SubMenuItem("Sound", settingsSoundMenu));
  settingsItems.push_back(new TGUI_SubMenuItem("Keyboard", settingsKeyboardMenu));
  TGUI_Splitter *settingsMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsItems.size(), TGUI_VERTICAL, false, settingsItems);

  // Create the misc menu
  miscItems.push_back(misc_screenshot = new TGUI_TextMenuItem("Save screenshot", 0));
  TGUI_Splitter *miscMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * miscItems.size(), TGUI_VERTICAL, false, miscItems);

  // Create the menu bar
  menu_names.push_back("File        ");
  menu_splitters.push_back(fileMenu);
  menu_names.push_back("Tape        ");
  menu_splitters.push_back(tapeMenu);
  menu_names.push_back("Disc        ");
  menu_splitters.push_back(discMenu);
  menu_names.push_back("Settings    ");
  menu_splitters.push_back(settingsMenu);
  menu_names.push_back("Misc        ");
  menu_splitters.push_back(miscMenu);
  menuBar = new TGUI_MenuBar(0, 0, displayW, menuHeight, menu_names, menu_splitters);

  // Add the menu bar
  tgui::setNewWidgetParent(0);
  tgui::addWidget(menuBar);

}

void optima_gui_refresh() {
  disc_writeprot0->setChecked(writeprot[0]);
  disc_writeprot1->setChecked(writeprot[1]);
  disc_defaultwriteprot->setChecked(defaultwriteprot);
  settings_video_snow->setChecked(snow);
  settings_video_fullscreen->setChecked(fullscreen);
  settings_hardware_colourboard->setChecked(colourboard);
  settings_hardware_bbcmode->setChecked(bbcmode);
  settings_ramrom_ramrom->setChecked(ramrom_enable);
  settings_ramrom_diskrom->setChecked((RR_jumpers & RAMROM_FLAG_DISKROM));
  settings_sound_internalspeaker->setChecked(spon);
  settings_sound_atomsid->setChecked(sndatomsid);
  settings_sound_tapenoise->setChecked(tpon);
  settings_sound_discnoise->setChecked(sndddnoise);
}

void optima_gui_handleEvent(ALLEGRO_EVENT *event) {
  tgui::handleEvent(event);
}

void optima_gui_update() {
  int resetit = 0;
  tgui::TGUIWidget *ret = tgui::update();
  if (!ret) {
    return;
  }
  if (ret == file_reset) {
    resetit = 1;
  }
  if (ret == file_exit) {
    quited = 1;
  }
  if (ret == tape_load) {
    popup("Not yet implemented!!!", 120);
  }
  if (ret == tape_eject) {
    closeuef();
    closecsw();
  }
  if (ret == tape_rewind) {
    closeuef();
    closecsw();
    loadtape(tapefn);
  }
  if (ret == tape_catalog) {
    popup("Not yet implemented!!!", 120);
  }
  if (ret == tape_speed_normal) {
    fasttape = 0;
  }
  if (ret == tape_speed_fast) {
    fasttape = 1;
  }
  if (ret == disc_load0) {
    popup("Not yet implemented!!!", 120);
  }
  if (ret == disc_load1) {
    popup("Not yet implemented!!!", 120);
  }
  if (ret == disc_eject0) {
    closedisc(0);
    discfns[0][0] = 0;
  }
  if (ret == disc_eject1) {
    closedisc(1);
    discfns[1][0] = 0;
  }
  if (ret == disc_new0) {
    popup("Not yet implemented!!!", 120);
  }
  if (ret == disc_new1) {
    popup("Not yet implemented!!!", 120);
  }
  if (ret == disc_writeprot0) {
    writeprot[0] = disc_writeprot0->isChecked();
    if (fwriteprot[0])
      fwriteprot[0] = 1;
  }
  if (ret == disc_writeprot1) {
    writeprot[1] = disc_writeprot1->isChecked();
    if (fwriteprot[1])
      fwriteprot[1] = 1;
  }
  if (ret == disc_defaultwriteprot) {
    defaultwriteprot = disc_defaultwriteprot->isChecked();
  }
  if (ret == settings_video_snow) {
    snow = settings_video_snow->isChecked();
  }
  if (ret == settings_video_fullscreen) {
    popup("Not yet implemented!!!", 120);
  }
  if (ret == settings_hardware_colourboard) {
    colourboard = settings_hardware_colourboard->isChecked();
    updatepal();
  }
  if (ret == settings_hardware_bbcmode) {
    bbcmode = settings_hardware_bbcmode->isChecked();
    if (bbcmode) {
      RR_enables |= RAMROM_FLAG_BBCMODE;
    } else {
      RR_enables &= ~RAMROM_FLAG_BBCMODE;
    }
    set_rr_ptrs();
    resetit = 1;
  }
  if (ret == settings_ramrom_ramrom) {
    ramrom_enable = settings_ramrom_ramrom->isChecked();;
    resetit = 1;
  }
  if (ret == settings_ramrom_diskrom) {
    if (settings_ramrom_diskrom->isChecked()) {
      RR_jumpers |= RAMROM_FLAG_DISKROM;
    } else {
      RR_jumpers &= ~RAMROM_FLAG_DISKROM;
    }
    resetit = 1;
  }
  if (ret == settings_sound_internalspeaker) {
    spon = settings_sound_internalspeaker->isChecked();
  }
  if (ret == settings_sound_atomsid) {
    sndatomsid = settings_sound_atomsid->isChecked();
  }
  if (ret == settings_sound_tapenoise) {
    tpon = settings_sound_tapenoise->isChecked();
  }
  if (ret == settings_sound_discnoise) {
    sndddnoise = settings_sound_discnoise->isChecked();
  }
  if (ret == settings_sound_ddtype_525) {
    ddtype = 0;
    closeddnoise();
    loaddiscsamps();
  }
  if (ret == settings_sound_ddtype_35) {
    ddtype = 1;
    closeddnoise();
    loaddiscsamps();
  }
  if (ret == settings_sound_ddvol_33) {
    ddvol = 1;
  }
  if (ret == settings_sound_ddvol_66) {
    ddvol = 2;
  }
  if (ret == settings_sound_ddvol_100) {
    ddvol = 3;
  }
  if (ret == settings_keyboard_redefine) {
    popup("Not yet implemented!!!", 120);
  }
  if (ret == settings_keyboard_default) {
    int c;
    for (c = 0; c < 128; c++)
      keylookup[c] = c;
  }
  if (ret == misc_screenshot) {
    popup("Not yet implemented!!!", 120);
  }


  if (resetit) {
    atom_reset(0);
  }
}






void optima_gui_draw() {
  tgui::draw();
}




#if 0
void optima_gui_loadt()
{
  char tempname[260];
  int ret;
  int xsize = windx - 32, ysize = windy - 16;

  memcpy(tempname, tapefn, 260);
  ret = file_select_ex("Please choose a tape image", tempname, "UEF;CSW", 260, xsize, ysize);
  if (ret)
    {
      closeuef();
      closecsw();
      memcpy(tapefn, tempname, 260);
      loadtape(tapefn);
    }
}
int gui_load0()
{
  char tempname[260];
  int ret;
  int xsize = windx - 32, ysize = windy - 16;

  memcpy(tempname, discfns[0], 260);
  ret = file_select_ex("Please choose a disc image", tempname, "DSK;SSD;DSD;IMG;FDI", 260, xsize, ysize);
  if (ret)
    {
      closedisc(0);
      memcpy(discfns[0], tempname, 260);
      loaddisc(0, discfns[0]);
      if (defaultwriteprot)
	writeprot[0] = 1;
    }
  updatelinuxgui();
  return D_O_K;
}

int gui_load1()
{
  char tempname[260];
  int ret;
  int xsize = windx - 32, ysize = windy - 16;

  memcpy(tempname, discfns[1], 260);
  ret = file_select_ex("Please choose a disc image", tempname, "DSK;SSD;DSD;IMG;FDI", 260, xsize, ysize);
  if (ret)
    {
      closedisc(1);
      memcpy(discfns[1], tempname, 260);
      loaddisc(1, discfns[1]);
      if (defaultwriteprot)
	writeprot[1] = 1;
    }
  updatelinuxgui();
  return D_O_K;
}
void optima_gui_sidtype()
{
  cursid=(int)active_menu->dp;
  sid_settype(sidmethod, cursid);
}

void optima_gui_method()
{
  sidmethod=(int)active_menu->dp;
  sid_settype(sidmethod, cursid);
}
void optima_gui_scrshot()
{
  char tempname[260];
  int ret, c;
  int xsize = windx - 32, ysize = windy - 16;

  tempname[0] = 0;
  ret = file_select_ex("Please enter filename", tempname, "BMP", 260, xsize, ysize);
  if (ret)
    {
      memcpy(scrshotname, tempname, 260);
      printf("Save scrshot\n");
      savescrshot = 1;
    }
}

void optima_gui_speed()
{
  emuspeed = (int)active_menu->dp;
  changetimerspeed(timerspeeds[emuspeed]);
  fskipmax = frameskips[emuspeed];
}

void optima_gui_joystk_en()
{
  joyst = !joyst;
}
#endif

