#ifndef MENUORGANIZER_H
#define MENUORGANIZER_H

#include <Arduino.h>

#define MAX_MENU_TEXT 40

#define INVALID 255
#define CANCELLED 254

#include "global_includes.h"

#include "LinkedList.h"
#include "Buttons.h"
extern Buttons buttons;

#include "NamePicker.h"
extern NamePicker namePicker;

#include "Notification.h"
extern Notification notification;

#include "ProgressBar.h"
extern ProgressBar progressBar;

#include "Settings.h"
extern Settings settings;

#include "RSSI_gatherer.h"
extern RSSI_gatherer rssi_gatherer;


struct MenuOption{
  String text; //text[MAX_MENU_TEXT];
  bool active;
  std::function<void()> function;
};


struct Menu{
  Menu *parent;
  int len = 0;
  int active_len = 0;
  XY current_option = {0,0};
  int selected = INVALID;
  int max_options_displayed_default = 5;
  int max_options_displayed = max_options_displayed_default;
  int starting_option = 0;
  int current_option_relative = 0;
  LinkedList<MenuOption> *options = new LinkedList<MenuOption>;
  LinkedList<MenuOption> *horizontal_options = new LinkedList<MenuOption>;
};



class MenuOrganizer
{
public:
  void Init();

  void Update();
  void ResetMenuState(Menu *_menu);

  LinkedList<MenuOption> GetActiveOptions();
  LinkedList<String> GetOnScreenOptionNames();
  int GetCurrentOnScreenOption();
  
  LinkedList<MenuOption> GetOnScreenOptions();
  void MenuDataDump(String src);

  bool HasHorizontalOptions(){return HasHorizontalOptions(menu);}
  bool HasHorizontalOptions(Menu *_menu){return (bool)menu->horizontal_options->size();}
  
  String GetHorizontalOptionName(){return menu->horizontal_options->get(menu->current_option.x).text;}

  int GetMaxOptionsDisplayed(){return menu->max_options_displayed;}
  int GetStartingOptionIndex(){return menu->starting_option;}
  int GetActiveOptionsCount(){return GetActiveOptions().size();}

  void Notify(String msg, int timeout, int gui_mode_to_revert);
  void Notify(String msg, int timeout);
  
  void Notify(String msg, std::function<void()> onAccept, std::function<void()> onDecline);
  void Notify(int level, String msg, int timeout);
  void Notify(int level, String msg, std::function<void()> onAccept, std::function<void()> onDecline);
  void Notify(bool res);
private:
  Menu *menu;
  
  Menu *mainMenu = new Menu; 
    Menu *settingsMenu = new Menu;
        Menu *debugMenu = new Menu;
            Menu *infoMenu = new Menu;
        

   

  
  void SetMenu(Menu *menu);
  void AddOption(Menu *_menu, MenuOption option);
  void RemoveOption(Menu *_menu, int option_num);
  void SetOptionText(Menu *_menu, int option_num, String new_name);
  void AddHorizontalOption(Menu *_menu, MenuOption option);
  void ClearOptions(Menu *_menu);
  void SetOptionName(Menu * _menu, int opt_num, String opt_name){MenuOption mo = _menu->options->get(opt_num); mo.text = opt_name; _menu->options->set(opt_num, mo);}
  
  void HandleControls();
  bool Cancelled();
  bool WasSelected();
  int GetSelection();
  void ResetSelection();
  void ActivateOption(Menu *_menu, int num);
  void DeactivateOption(Menu *_menu, int num);
  void UpdateActiveMenu();
  void ShiftCurrentOption(int buttonID);

};




#endif
