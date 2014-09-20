#include "tgui/tgui2.hpp"
#include "tgui/tgui2_widgets.hpp"
#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include <allegro_color.h>

#include "atom.h"
#include "roms.h"

#define ID_ENTER_DISK0  4
#define ID_ENTER_DISK1  5

int id;

extern "C" {
  void optima_gui_init(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, int menuFontSize);
  void optima_gui_handleEvent(ALLEGRO_EVENT *event);
  void optima_gui_refresh();
  void optima_gui_update();
  void optima_gui_draw();
  void setejecttext(int drive, const char *fn);
  void setquit();

  char tapefn[260] = "";
  int fskipmax = 0;
}

void gui_file_browser(char *title, char *dirpath, int mode);

void gui_file_enterer(char *title, char *dirpath, int mode);

char ejecttext[3][260] = { "Eject disc :0/2", "Eject disc :1/3", "Eject tape" };

void setejecttext(int id, const char *fn)
{
  ALLEGRO_PATH *path = al_create_path(fn);

  if (id == ID_TAPE) {
    if (fn[0])
      sprintf(ejecttext[id], "Eject tape - %s", al_get_path_filename(path));
    else
      sprintf(ejecttext[id], "Eject tape");
  } else {
    if (fn[0])
      sprintf(ejecttext[id], "Eject drive :%i/%i - %s", id, id + 2, al_get_path_filename(path));
    else
      sprintf(ejecttext[id], "Eject drive :%i/%i", id, id + 2);
  }
}

void setquit()
{
}

TGUI_Frame *fileEnterer;
TGUI_Frame *fileBrowser;
TGUI_Button *buttonOK;
TGUI_Button *buttonCancel;
TGUI_List *fileList;
TGUI_TextField *textField;



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


std::vector<std::string> files;

int fontSize;

void optima_gui_init(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font, int menuFontSize) {
  tgui::init(display);
  tgui::setFont(font);

  tguiWidgetsSetColors(al_color_name("cyan"), al_color_name("darkgray"));

  fontSize = menuFontSize;

  int menuWidth = displayW / 5;
  int splitterPad = 0;
  int menuPad = menuFontSize / 3;

  int menuHeight = menuFontSize + menuPad * 2;


  // Create the file menu
  fileItems.push_back(file_reset = new TGUI_TextMenuItem("Reset", 0)); 
  fileItems.push_back(file_exit = new TGUI_TextMenuItem("Exit", 0)); 
  TGUI_Splitter *fileMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * fileItems.size(), TGUI_VERTICAL, true, fileItems);
  fileMenu->setPadding(splitterPad, splitterPad);

  // Create the tape speed menu
  tapeSpeedItems.push_back(tape_speed_normal = new TGUI_RadioMenuItem("Normal", 0, &tapeSpeedGroup, 0));
  tapeSpeedItems.push_back(tape_speed_fast = new TGUI_RadioMenuItem("Fast", 0, &tapeSpeedGroup, 1));
  TGUI_Splitter *tapeSpeedMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * tapeSpeedItems.size(), TGUI_VERTICAL, false, tapeSpeedItems);
  tapeSpeedMenu->setPadding(splitterPad, splitterPad);

  // Create the tape menu
  tapeItems.push_back(tape_load = new TGUI_TextMenuItem("Load tape...", 0));
  tapeItems.push_back(tape_eject = new TGUI_TextMenuItem(ejecttext[ID_TAPE], 0));
  tapeItems.push_back(tape_rewind = new TGUI_TextMenuItem("Rewind tape", 0));
  tapeItems.push_back(tape_catalog = new TGUI_TextMenuItem("Show tape catalogue", 0));
  tapeItems.push_back(new TGUI_SubMenuItem("TapeSpeed", tapeSpeedMenu));
  TGUI_Splitter *tapeMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * tapeItems.size(), TGUI_VERTICAL, false, tapeItems);
  tapeMenu->setPadding(splitterPad, splitterPad);

  // Create the disc menu
  discItems.push_back(disc_load0 = new TGUI_TextMenuItem("Load disc: 0/2...", 0));
  discItems.push_back(disc_load1 = new TGUI_TextMenuItem("Load disc: 1/3...", 0));
  discItems.push_back(disc_eject0 = new TGUI_TextMenuItem(ejecttext[ID_DISK0], 0));
  discItems.push_back(disc_eject1 = new TGUI_TextMenuItem(ejecttext[ID_DISK1], 0));
  discItems.push_back(disc_new0 = new TGUI_TextMenuItem("New disc: 0/2", 0));
  discItems.push_back(disc_new1 = new TGUI_TextMenuItem("New disc: 1/3", 0));
  discItems.push_back(disc_writeprot0 = new TGUI_CheckMenuItem("Write protect disc: 0/2", 0, 0));
  discItems.push_back(disc_writeprot1 = new TGUI_CheckMenuItem("Write protect disc: 1/3", 0, 0));
  discItems.push_back(disc_defaultwriteprot = new TGUI_CheckMenuItem("Default write protect", 0, 0));
  TGUI_Splitter *discMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * discItems.size(), TGUI_VERTICAL, false, discItems);
  discMenu->setPadding(splitterPad, splitterPad);

  // Create the settings/video menu
  settingsVideoItems.push_back(settings_video_snow = new TGUI_CheckMenuItem("Snow", 0, 0));
  settingsVideoItems.push_back(settings_video_fullscreen = new TGUI_CheckMenuItem("Fullscreen", 0, 0));
  TGUI_Splitter *settingsVideoMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsVideoItems.size(), TGUI_VERTICAL, false, settingsVideoItems);
  settingsVideoMenu->setPadding(splitterPad, splitterPad);

  // Create the settings/hardware menu
  settingsHardwareItems.push_back(settings_hardware_colourboard = new TGUI_CheckMenuItem("Colour Board", 0, 0));
  settingsHardwareItems.push_back(settings_hardware_bbcmode = new TGUI_CheckMenuItem("BBC BASIC", 0, 0));
  TGUI_Splitter *settingsHardwareMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsHardwareItems.size(), TGUI_VERTICAL, false, settingsHardwareItems);
  settingsHardwareMenu->setPadding(splitterPad, splitterPad);

  // Create the settings/ramrom menu
  settingsRamRomItems.push_back(settings_ramrom_ramrom = new TGUI_CheckMenuItem("RAM/ROM Enabled", 0, 0));
  settingsRamRomItems.push_back(settings_ramrom_diskrom = new TGUI_CheckMenuItem("RAM/ROM Diskrom Enabled", 0, 0));
  TGUI_Splitter *settingsRamRomMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsRamRomItems.size(), TGUI_VERTICAL, false, settingsRamRomItems);
  settingsRamRomMenu->setPadding(splitterPad, splitterPad);

  // Create the settings/sound/ddtype menu
  settingsSoundDDTypeItems.push_back(settings_sound_ddtype_525 = new TGUI_RadioMenuItem("5.25", 0, &settingsSoundDDTypeGroup, 0));
  settingsSoundDDTypeItems.push_back(settings_sound_ddtype_35 = new TGUI_RadioMenuItem("3.5", 0, &settingsSoundDDTypeGroup, 1));
  TGUI_Splitter *settingsSoundDDTypeMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsSoundDDTypeItems.size(), TGUI_VERTICAL, false, settingsSoundDDTypeItems);
  settingsSoundDDTypeMenu->setPadding(splitterPad, splitterPad);;

  // Create the settings/sound/ddvol menu
  settingsSoundDDVolItems.push_back(settings_sound_ddvol_33 = new TGUI_RadioMenuItem("33%", 0, &settingsSoundDDVolGroup, 0));
  settingsSoundDDVolItems.push_back(settings_sound_ddvol_66 = new TGUI_RadioMenuItem("66%", 0,  &settingsSoundDDVolGroup, 1));
  settingsSoundDDVolItems.push_back(settings_sound_ddvol_100 = new TGUI_RadioMenuItem("100%", 0,  &settingsSoundDDVolGroup, 2));
  TGUI_Splitter *settingsSoundDDVolMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsSoundDDVolItems.size(), TGUI_VERTICAL, false, settingsSoundDDVolItems);
  settingsSoundDDVolMenu->setPadding(splitterPad, splitterPad);

  // Create the settings/sound menu
  settingsSoundItems.push_back(settings_sound_internalspeaker = new TGUI_CheckMenuItem("Internal speaker", 0, 0));
  settingsSoundItems.push_back(settings_sound_atomsid = new TGUI_CheckMenuItem("AtomSID", 0, 0));
  settingsSoundItems.push_back(settings_sound_tapenoise = new TGUI_CheckMenuItem("Tape noise", 0, 0));
  settingsSoundItems.push_back(settings_sound_discnoise = new TGUI_CheckMenuItem("Disc drive noise", 0, 0));
  settingsSoundItems.push_back(new TGUI_SubMenuItem("Disc drive type", settingsSoundDDTypeMenu));
  settingsSoundItems.push_back(new TGUI_SubMenuItem("Disc drive vol", settingsSoundDDVolMenu));
  TGUI_Splitter *settingsSoundMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsSoundItems.size(), TGUI_VERTICAL, false, settingsSoundItems);
  settingsSoundMenu->setPadding(splitterPad, splitterPad);

  // Create the settings/keyboard menu
  settingsKeyboardItems.push_back(settings_keyboard_redefine = new TGUI_TextMenuItem("Redefine keys...", 0));
  settingsKeyboardItems.push_back(settings_keyboard_default = new TGUI_TextMenuItem("Default mapping", 0));
  TGUI_Splitter *settingsKeyboardMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsKeyboardItems.size(), TGUI_VERTICAL, false, settingsKeyboardItems);
  settingsKeyboardMenu->setPadding(splitterPad, splitterPad);

  // Create the settings menu
  settingsItems.push_back(new TGUI_SubMenuItem("Video", settingsVideoMenu));
  settingsItems.push_back(new TGUI_SubMenuItem("Hardware", settingsHardwareMenu));
  settingsItems.push_back(new TGUI_SubMenuItem("RamRom", settingsRamRomMenu));
  settingsItems.push_back(new TGUI_SubMenuItem("Sound", settingsSoundMenu));
  settingsItems.push_back(new TGUI_SubMenuItem("Keyboard", settingsKeyboardMenu));
  TGUI_Splitter *settingsMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * settingsItems.size(), TGUI_VERTICAL, false, settingsItems);
  settingsMenu->setPadding(splitterPad, splitterPad);

  // Create the misc menu
  miscItems.push_back(misc_screenshot = new TGUI_TextMenuItem("Save screenshot", 0));
  TGUI_Splitter *miscMenu = new TGUI_Splitter(0, 0, menuWidth, menuHeight * miscItems.size(), TGUI_VERTICAL, false, miscItems);
  miscMenu->setPadding(splitterPad, splitterPad);

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

  optima_gui_refresh();

  tgui::setMenuPad(menuPad);

  // Add the menu bar
  tgui::setNewWidgetParent(0);
  tgui::addWidget(menuBar);

  // Create the file navigator components

  int panelW = displayW / 2;
  int panelH = displayH / 2;
  int panelX = (displayW - panelW) / 2;
  int panelY = (displayH - panelH) / 2;
  int buttonOffset = panelW / 3;
  int buttonW = fontSize * 10;
  int buttonH = fontSize;

  // Create the List widget
  fileList = new TGUI_List(0, 0, 0);

  // Create the File Browser Franme
  fileBrowser = new TGUI_Frame("", panelX, panelY, panelW, panelH);

  // Create the scroll pane
  TGUI_ScrollPane *scrollPane = new TGUI_ScrollPane(fileList);
  scrollPane->setX(0);
  scrollPane->setY(fileBrowser->barHeight());
  scrollPane->setWidth(panelW);
  scrollPane->setHeight(panelH - fileBrowser->barHeight() - buttonH * 2);
  fileList->setWidth(panelW - scrollPane->SCROLLBAR_THICKNESS);

  fileBrowser->setChild(scrollPane);

  //Create the file Browser OK and Cancel Buttons
  buttonOK = new TGUI_Button("OK", displayW / 2 - buttonOffset - buttonW / 2, panelY + panelH - buttonH * 3 / 2, buttonW, buttonH);
  buttonCancel = new TGUI_Button("Cancel", displayW / 2 + buttonOffset - buttonW / 2, panelY + panelH - buttonH * 3 / 2, buttonW, buttonH);


  // Create the File Enterer Widges
  fileEnterer = new TGUI_Frame("", panelX, panelY, panelW, panelH);
  textField = new TGUI_TextField("", 0, fileEnterer->barHeight(), panelW);  
  fileEnterer->setChild(textField);

}

void optima_gui_refresh() {
  tapeSpeedGroup.selected = fasttape;
  settingsSoundDDTypeGroup.selected = ddtype;
  settingsSoundDDVolGroup.selected = ddvol - 1;
  disc_eject0->setName(ejecttext[ID_DISK0]);
  disc_eject1->setName(ejecttext[ID_DISK1]);
  tape_eject->setName(ejecttext[ID_TAPE]);
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
    popup(POPUP_TIME, "Reset 6502");
  }
  if (ret == file_exit) {
    popup(POPUP_TIME, "Exiting...");
    quited = 1;
  }
  if (ret == tape_load) {
    gui_file_browser("Please choose a tape image", "tapes", ID_TAPE);
  }
  if (ret == tape_eject) {
    closeuef();
    closecsw();
    setejecttext(ID_TAPE, "");
    popup(POPUP_TIME, "Ejected tape");
  }
  if (ret == tape_rewind) {
    closeuef();
    closecsw();
    loadtape(tapefn);
    popup(POPUP_TIME, "Rewound tape");
  }
  if (ret == tape_catalog) {
    popup(POPUP_TIME, "Not yet implemented!!!");
  }
  if (ret == tape_speed_normal) {
    fasttape = 0;
    popup(POPUP_TIME, "Normal tape speed");
  }
  if (ret == tape_speed_fast) {
    popup(POPUP_TIME, "Not yet implemented!!!");
    // fasttape = 1;
  }
  if (ret == disc_load0) {
    gui_file_browser("Please choose a disc image", "disks", ID_DISK0);
  }
  if (ret == disc_load1) {
    gui_file_browser("Please choose a disc image", "disks", ID_DISK1);
  }
  if (ret == disc_eject0) {
    closedisc(0);
    discfns[0][0] = 0;
    setejecttext(ID_DISK0, "");
    popup(POPUP_TIME, "Ejected disk :0/2");
  }
  if (ret == disc_eject1) {
    closedisc(1);
    discfns[1][0] = 0;
    setejecttext(ID_DISK1, "");
    popup(POPUP_TIME, "Ejected disk :1/3");
  }
  if (ret == disc_new0) {
    gui_file_enterer("Please enter disk image name", "disks", ID_ENTER_DISK0);
  }
  if (ret == disc_new1) {
    gui_file_enterer("Please enter disk image name", "disks", ID_ENTER_DISK1);
  }
  if (ret == disc_writeprot0) {
    writeprot[0] = disc_writeprot0->isChecked();
    if (fwriteprot[0]) {
      writeprot[0] = 1;
    }
    if (writeprot[0]) {
      popup(POPUP_TIME, "Disk :0/2 write protected");
    } else {
      popup(POPUP_TIME, "Disk :0/2 write enabled");
    }
  }
  if (ret == disc_writeprot1) {
    writeprot[1] = disc_writeprot1->isChecked();
    if (fwriteprot[1]) {
      writeprot[1] = 1;
    }
    if (writeprot[1]) {
      popup(POPUP_TIME, "Disk :1/3 write protected");
    } else {
      popup(POPUP_TIME, "Disk :1/3 write enabled");
    }
  }
  if (ret == disc_defaultwriteprot) {
    defaultwriteprot = disc_defaultwriteprot->isChecked();
    if (defaultwriteprot) {
      popup(POPUP_TIME, "Default is write protected");
    } else {
      popup(POPUP_TIME, "Default is write enabled");
    }
  }
  if (ret == settings_video_snow) {
    snow = settings_video_snow->isChecked();
    if (snow) {
      popup(POPUP_TIME, "Snow enabled");
    } else {
      popup(POPUP_TIME, "Snow disabled");
    }
  }
  if (ret == settings_video_fullscreen) {
    popup(POPUP_TIME, "Not yet implemented!!!");
  }
  if (ret == settings_hardware_colourboard) {
    colourboard = settings_hardware_colourboard->isChecked();
    updatepal();
    if (colourboard) {
      popup(POPUP_TIME, "Colour palette");
    } else {
      popup(POPUP_TIME, "Monochrome palette");
    }
  }
  if (ret == settings_hardware_bbcmode) {
    bbcmode = settings_hardware_bbcmode->isChecked();
    if (bbcmode) {
      RR_enables |= RAMROM_FLAG_BBCMODE;
      popup(POPUP_TIME, "BBC mode enabled");
    } else {
      RR_enables &= ~RAMROM_FLAG_BBCMODE;
      popup(POPUP_TIME, "BBC mode disabled");
    }
    set_rr_ptrs();
    resetit = 1;
  }
  if (ret == settings_ramrom_ramrom) {
    ramrom_enable = settings_ramrom_ramrom->isChecked();;
    resetit = 1;
    if (ramrom_enable) {
      popup(POPUP_TIME, "RamRom board enabled");
    } else {
      popup(POPUP_TIME, "RamRom board disabled");
    }

  }
  if (ret == settings_ramrom_diskrom) {
    if (settings_ramrom_diskrom->isChecked()) {
      RR_jumpers |= RAMROM_FLAG_DISKROM;
    } else {
      RR_jumpers &= ~RAMROM_FLAG_DISKROM;
    }
    resetit = 1;
    if (RR_jumpers & RAMROM_FLAG_DISKROM) {
      popup(POPUP_TIME, "Disk ROM enabled");
    } else {
      popup(POPUP_TIME, "Disk ROM disabled");
    }
  }
  if (ret == settings_sound_internalspeaker) {
    spon = settings_sound_internalspeaker->isChecked();
    if (spon) {
      popup(POPUP_TIME, "Speaker enabled");
    } else {
      popup(POPUP_TIME, "Speaker disabled");
    }
  }
  if (ret == settings_sound_atomsid) {
    sndatomsid = settings_sound_atomsid->isChecked();
    if (sndatomsid) {
      popup(POPUP_TIME, "SID enabled");
    } else {
      popup(POPUP_TIME, "SID disabled");
    }
  }
  if (ret == settings_sound_tapenoise) {
    tpon = settings_sound_tapenoise->isChecked();
    if (tpon) {
      popup(POPUP_TIME, "Tape noise enabled");
    } else {
      popup(POPUP_TIME, "Tape noise disabled");
    }
  }
  if (ret == settings_sound_discnoise) {
    sndddnoise = settings_sound_discnoise->isChecked();
    if (sndddnoise) {
      popup(POPUP_TIME, "Disk noise enabled");
    } else {
      popup(POPUP_TIME, "Disk noise disabled");
    }
  }
  if (ret == settings_sound_ddtype_525) {
    ddtype = 0;
    closeddnoise();
    loaddiscsamps();
    popup(POPUP_TIME, "Disk noise type: 5.25\" drive");
  }
  if (ret == settings_sound_ddtype_35) {
    ddtype = 1;
    closeddnoise();
    loaddiscsamps();
    popup(POPUP_TIME, "Disk noise type: 3.5\" drive");
  }
  if (ret == settings_sound_ddvol_33) {
    ddvol = 1;
    popup(POPUP_TIME, "Disk noise volume: 33%%");
  }
  if (ret == settings_sound_ddvol_66) {
    ddvol = 2;
    popup(POPUP_TIME, "Disk noise volume: 66%%");
  }
  if (ret == settings_sound_ddvol_100) {
    ddvol = 3;
    popup(POPUP_TIME, "Disk noise volume: 100%%");
  }
  if (ret == settings_keyboard_redefine) {
    popup(POPUP_TIME, "Not yet implemented!!!");
  }
  if (ret == settings_keyboard_default) {
    int c;
    for (c = 0; c < 128; c++) {
      keylookup[c] = c;
    }
    popup(POPUP_TIME, "Reset key bindings to default");
  }
  if (ret == misc_screenshot) {
    popup(POPUP_TIME, "Not yet implemented!!!");
  }
  if (ret == buttonOK || ret == buttonCancel) {
    if (ret == buttonOK) {
      if (id >= ID_ENTER_DISK0) {
	const char *entered = textField->getText().c_str();
	id &= 3;
	closedisc(id);
	strcpy(discfns[id], entered);
	newdisc(id, discfns[id]);
	if (defaultwriteprot) {
	  writeprot[id] = 1;
	}
	popup(POPUP_TIME, "Created disk :%d/%d %s", id, id + 2, entered);
      } else {
	const char *selected = files[fileList->getSelected()].c_str();
	if (id == ID_TAPE) {
	  closeuef();
	  closecsw();
	  strcpy(tapefn, selected);
	  loadtape(tapefn);
	  setejecttext(ID_TAPE, selected);
	  popup(POPUP_TIME, "Loaded tape %s", selected);
	  
	} else {
	  closedisc(id);
	  strcpy(discfns[id], selected);
	  loaddisc(id, discfns[id]);
	  if (defaultwriteprot) {
	    writeprot[id] = 1;
	  }
	  popup(POPUP_TIME, "Loaded disk :%d/%d %s", id, id + 2, selected);
	}
      }
    }
    buttonOK->remove();
    buttonCancel->remove();
    fileEnterer->remove();
    fileBrowser->remove();
  }

  if (resetit) {
    atom_reset(0);
  }

  optima_gui_refresh();

}

void optima_gui_draw() {
  tgui::draw();
}


void gui_file_browser(char *title, char *dirpath, int mode)
{
  id = mode;

  ALLEGRO_FS_ENTRY *dir = al_create_fs_entry(dirpath);
  ALLEGRO_FS_ENTRY *file;
  al_open_directory(dir);
  files.clear();
  while ((file = al_read_directory(dir))) {
    files.push_back(al_get_fs_entry_name(file));
    al_destroy_fs_entry(file);
  }
  al_close_directory(dir);
  al_destroy_fs_entry(dir);

  std::sort(files.begin(), files.end());
  fileList->setLabels(files);
  fileList->setSelected(0);
  fileBrowser->setTitle(title);

  tgui::setNewWidgetParent(0);
  tgui::addWidget(fileBrowser);
  tgui::addWidget(buttonOK);
  tgui::addWidget(buttonCancel);
  return;
}

void gui_file_enterer(char *title, char *dirpath, int mode)
{
  id = mode;

  textField->setText("");
  fileEnterer->setTitle(title);

  tgui::setNewWidgetParent(0);
  tgui::addWidget(fileEnterer);
  tgui::addWidget(buttonOK);
  tgui::addWidget(buttonCancel);
  tgui::setFocus(textField);
  return;
}


#if 0
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

