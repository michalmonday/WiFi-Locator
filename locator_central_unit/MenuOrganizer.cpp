#include "MenuOrganizer.h"

#include "GUI.h"
extern GUI gui;

void MenuOrganizer::Update(){
  HandleControls();
  if(menu->selected != INVALID){   
    if(menu->selected != CANCELLED){
      MenuOption mo = GetActiveOptions().get(menu->selected);
      Serial.print("Picked menu option - "); Serial.println(mo.text);
      mo.function();
    }else{
      // switch to some parent menu or something
      ResetMenuState(menu);
      SetMenu(menu->parent);
      //clean up any possible runtime actions regardless of the option cancelled
      //rfid.ResetRequest(); // idk
    }
    menu->selected = INVALID;
  }
}

void MenuOrganizer::ResetMenuState(Menu *_menu){
  _menu->current_option = {0,0};
  _menu->selected = INVALID;
  _menu->starting_option = 0;
  _menu->current_option_relative = 0;
}

LinkedList<String> MenuOrganizer::GetOnScreenOptionNames(){
  LinkedList<MenuOption> screen_options = GetOnScreenOptions(); 
  LinkedList<String> names;
  for(int i=0; i < screen_options.size(); i++){ 
    names.add(screen_options.get(i).text);
  }
  return names;
}

LinkedList<MenuOption> MenuOrganizer::GetOnScreenOptions(){
  LinkedList<MenuOption> screen_options;
  LinkedList<MenuOption> active_options = GetActiveOptions();
  for(int i = menu->starting_option; i < menu->starting_option + menu->max_options_displayed && i < menu->starting_option + active_options.size(); i++){
    screen_options.add(active_options.get(i));
  }
  return screen_options;
}

LinkedList<MenuOption> MenuOrganizer::GetActiveOptions(){
  LinkedList<MenuOption> opts;
  for(int i=0; i < menu->len; i++){
    if(menu->options->get(i).active){  
      opts.add(menu->options->get(i));
    }
  }
  return opts;
}


/*
LinkedList<std::function<void()>> MenuOrganizer::GetActiveOptionFunctions(){
  LinkedList<std::function<void()>> funcs;
  for(int i=0; i < menu->len; i++){
    if(menu->options->get(i).active){
      funcs.add(menu->options->get(i).function);
    }
  }
  return funcs;
}
*/

int MenuOrganizer::GetCurrentOnScreenOption(){
  return menu->current_option_relative;
}



void MenuOrganizer::Notify(String msg, int timeout){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(0, msg, {64, 32-5}, timeout);   
  notification.OnAccept(0, [msg](){Serial.println("MenuOrganizer::Notify - onAccept - " + msg); gui.SetMode(MODE_MENU); });
  notification.OnDecline(0, [msg](){ Serial.println("MenuOrganizer::Notify - onDecline - " + msg); gui.SetMode(MODE_MENU); });  
}

void MenuOrganizer::Notify(String msg, int timeout, int gui_mode_to_revert){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(0, msg, {64, 32-5}, timeout);   
  notification.OnAccept(0, [msg, gui_mode_to_revert](){Serial.println("MenuOrganizer::Notify - onAccept - " + msg); gui.SetMode(gui_mode_to_revert); });
  notification.OnDecline(0, [msg, gui_mode_to_revert](){ Serial.println("MenuOrganizer::Notify - onDecline - " + msg); gui.SetMode(gui_mode_to_revert); });  
}

void MenuOrganizer::Notify(String msg, std::function<void()> onAccept, std::function<void()> onDecline){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(0, msg, {64, 32-5}, 0);   
  notification.OnAccept(0, onAccept);
  notification.OnDecline(0, onDecline); 
}


void MenuOrganizer::Notify(int level, String msg, int timeout){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(level, msg, {64, 32-5}, timeout);   
  notification.OnAccept(level, [msg](){Serial.println("MenuOrganizer::Notify2 - onAccept - " + msg); gui.SetMode(MODE_MENU); });
  notification.OnDecline(level, [msg](){Serial.println("MenuOrganizer::Notify2 - onDecline - " + msg); gui.SetMode(MODE_MENU); });  
}

void MenuOrganizer::Notify(int level, String msg, std::function<void()> onAccept, std::function<void()> onDecline){
  gui.SetMode(MODE_NOTIFICATION);
  notification.Set(level, msg, {64, 32-5}, 0);   
  notification.OnAccept(level, onAccept);
  notification.OnDecline(level, onDecline); 
}

void MenuOrganizer::Notify(bool res){
  gui.SetMode(MODE_NOTIFICATION);
  if(res == true){
    notification.Set(0, "Success\n:)", {64, 32-5}, NOTIFICATION_TIME);
  }else{
    notification.Set(0, "Failed...\n:(", {64, 32-5}, 0);
  }
  notification.OnAccept(0, [](){gui.SetMode(MODE_MENU);});
  notification.OnDecline(0, [](){gui.SetMode(MODE_MENU);});  
}


void MenuOrganizer::Init(){  
  mainMenu->parent = mainMenu;
  settingsMenu->parent = mainMenu;
  debugMenu->parent = settingsMenu;
  infoMenu->parent = debugMenu;
  

  
  Serial.println("MenuOrganizer::Init - 1");



  /*
  AddOption(mainMenu, MenuOption{"PC interface\0", true, [this](){
        ClearOptions(pcMenu);
        AddOption(pcMenu, MenuOption{"Send card\0", true, [this](){
          pcInterface.SendCard(String("college"));
          }});        
        SetMenu(pcMenu);      
  }});
  */
  
  /*
  AddOption(mainMenu, MenuOption{"example\0", true, [this](){
    
    }
  });
  */

  AddOption(mainMenu, MenuOption{"Real time RSSI", true, [this](){
      gui.SetMode(MODE_RSSI);
      rssi_gatherer.Reset();
      rssi_gatherer.SetMode(RSSI_MODE_REAL_TIME);
    }
  });

  AddOption(mainMenu, MenuOption{"Mean RSSI", true, [this](){
      Notify("The measurement will\ntake " + String(rssi_gatherer.GetMeanGatheringTime())  +" seconds\nPress 'Yes' to begin..." , 
              /*onAccept*/ [this](){
                  gui.SetMode(MODE_RSSI);
                  rssi_gatherer.Reset();
                  rssi_gatherer.BeginGathering();
                  rssi_gatherer.SetMode(RSSI_MODE_MEAN);
                },
              /*onDeclined*/ [this](){
                  gui.SetMode(MODE_MENU);
                }
      ); 
    }
  });

  AddOption(mainMenu, MenuOption{"GPS + RSSI", true, [this](){
      gui.SetMode(MODE_RSSI);
      rssi_gatherer.Reset();
      rssi_gatherer.SetMode(RSSI_MODE_GPS);
    }
  });
  
  AddOption(mainMenu, MenuOption{"Settings", true, [this](){
      ClearOptions(settingsMenu);

      AddOption(settingsMenu, MenuOption{"Mean RSSI gathering time", true,[this](){
          progressBar.SetLabel("Time in seconds");
          progressBar.SetBorders(3, 120);
          progressBar.SetValue(settings.Get("RSSI_gathering_time").toInt());
          progressBar.OnSuccess([this](){
            int val = progressBar.GetValue();
            rssi_gatherer.SetMeanGatheringTime(val);
            settings.Set("RSSI_gathering_time", val);
            Notify("Gathering time has been\nset to " + String(val) + " seconds", 1000);
            });
            
          progressBar.OnFail([this](){
            gui.SetMode(MODE_MENU);
            });
            
          progressBar.SetValGetter([this](int key){
            int to_add = 0;
            switch(key){
              case BUTTON_RIGHT:
                to_add = 1;
                break;
              case BUTTON_LEFT:
                to_add = -1; 
                break;  
              case BUTTON_UP:
                to_add = 10;
                break;
              case BUTTON_DOWN:
                to_add = -10;
                break;
              default:
                return progressBar.GetValue();
                break;
            }
            return progressBar.GetValue() + to_add;
            });
  
          gui.SetMode(MODE_PROGRESS_BAR);
        }
      });

      AddOption(settingsMenu, MenuOption{"Brightness", true,[this](){
          progressBar.SetLabel("Brightness");
          progressBar.SetBorders(0, 255);
          progressBar.SetValue(settings.Get("Brightness").toInt());
          progressBar.OnSuccess([this](){
            // save it (eeprom or spiffs file, create settings.cpp / settings.h)
            int val = progressBar.GetValue();
            gui.SetBrightness(val);
            settings.Set("Brightness", val);
            Notify("Brightness has been\nset to " + String(val), 1000);
            });
            
          progressBar.OnFail([this](){
            gui.SetMode(MODE_MENU);
            });
            
          progressBar.SetValGetter([this](int key){
            int to_add = 0;
            switch(key){
              case BUTTON_RIGHT:
                to_add = 1;
                break;
              case BUTTON_LEFT:
                to_add = -1; 
                break;  
              case BUTTON_UP:
                to_add = 10;
                break;
              case BUTTON_DOWN:
                to_add = -10;
                break;
              default:
                return progressBar.GetValue();
                break;
            }
            int brightness = progressBar.GetValue() + to_add;
            gui.SetBrightness(brightness > 255 ? 255 : brightness); // in progressbar.Update the border check is done after getValFunc so additional check is added here
            return brightness;
            });
  
          gui.SetMode(MODE_PROGRESS_BAR);
        }
      });

      AddOption(settingsMenu, MenuOption{"Debugging", true, [this](){
            ClearOptions(debugMenu);

            AddOption(debugMenu, MenuOption{"Info", true, [this](){
              ClearOptions(infoMenu);
              
              AddOption(infoMenu, MenuOption{"Free heap: " + FormatBytes(ESP.getFreeHeap()), true, [this](){Notify("Free heap memory size.\n(Free dynamic memory)",0);}});
              AddOption(infoMenu, MenuOption{"Sketch size: " + FormatBytes(ESP.getSketchSize()), true, [this](){Notify("The size of the\ncurrent sketch.",0);}});
              AddOption(infoMenu, MenuOption{"Unused sketch: " + FormatBytes(ESP.getFreeSketchSpace()), true, [this](){Notify("The free sketch space.",0);}});
              AddOption(infoMenu, MenuOption{"Chip ID: " + String(ESP.getChipId()), true, [this](){Notify("Chip ID.",0);}});     
              AddOption(infoMenu, MenuOption{"Sdk version: " + String(ESP.getSdkVersion()), true, [this](){Notify("Version of software\ndevelopment kit.",0);}});
              AddOption(infoMenu, MenuOption{"CPU frequency: " + String(ESP.getCpuFreqMHz()) + " MHz", true, [this](){Notify("The CPU frequency in MHz.\nMega Hertz - millions of\noccurences within a second.",0);}});
              AddOption(infoMenu, MenuOption{"Fl chip ID: " + String(ESP.getFlashChipId()), true, [this](){Notify("The flash chip ID.",0);}});
              AddOption(infoMenu, MenuOption{"Fl chip sz: " + FormatBytes(ESP.getFlashChipSize()), true, [this](){Notify("The flash chip size,\nin bytes, as seen by\nthe SDK (may be less\nthan actual size).",0);}});
              AddOption(infoMenu, MenuOption{"Fl chip real sz: " + FormatBytes(ESP.getFlashChipRealSize()), true, [this](){Notify("The real chip size,\nin bytes, based on\nthe flash chip ID.",0);}});
                  
              //ESP.getCoreVersion(); 
              //ESP.getFlashChipSpeed(void);

              SetMenu(infoMenu);
            }});
        
            SetMenu(debugMenu);
          }});   
          
      SetMenu(settingsMenu);
    }
  });
  
  SetMenu(mainMenu);
}



void MenuOrganizer::AddOption(Menu *_menu, MenuOption option){
  _menu->len++;
  if(option.active == true){
    _menu->active_len++;
  }
  _menu->options->add(option);
}

void MenuOrganizer::RemoveOption(Menu *_menu, int option_num){
  if(option_num < _menu->options->size()){
    _menu->len--;
    _menu->active_len--;
    _menu->options->remove(option_num);
  }
}

void MenuOrganizer::SetOptionText(Menu *_menu, int option_num, String new_name){
  if(option_num < _menu->options->size()){
    MenuOption mo = _menu->options->get(option_num);
    mo.text = new_name;
    _menu->options->set(_menu->current_option.y, mo);
  }
}

void MenuOrganizer::AddHorizontalOption(Menu *_menu, MenuOption option){
  _menu->horizontal_options->add(option);
}

void MenuOrganizer::ClearOptions(Menu *_menu){
  _menu->len = 0;
  _menu->active_len = 0;
  _menu->options->clear();
  
  _menu->horizontal_options->clear();
  
  ResetMenuState(_menu);
}

void MenuOrganizer::SetMenu(Menu *new_menu){
  menu = new_menu;
}

void MenuOrganizer::ActivateOption(Menu *_menu, int num){
  if(_menu->options->get(num).active == false){
    _menu->active_len++;
    MenuOption mo = _menu->options->get(num);
    mo.active = true;
    _menu->options->set(num, mo);
  }   
}

void MenuOrganizer::DeactivateOption(Menu *_menu, int num){
  if(_menu->options->get(num).active == true){
    _menu->active_len--;
    MenuOption mo = _menu->options->get(num);
    mo.active = false;
    _menu->options->set(num, mo);
  }  
}

void MenuOrganizer::HandleControls(){
  //Serial.println("KEY PRESSED");
  //int key = buttons.GetLast();
  //if(key && buttons.Limiter(250, 125, 600)){  
  if(int key = buttons.GetLast(/*use limiter?*/ true)){
    if(key == BUTTON_YES){
      //active_menu[current_option.y].function();
      menu->selected = menu->current_option.y;
    }
    else if(key == BUTTON_NO){
      menu->selected = CANCELLED;
      //DeclineCurrentOption();
    }
    else{
      ShiftCurrentOption(key); // replace with menu vars
      //MenuDataDump("MenuOrganizer::HandleControls");
    }
  }
}

void MenuOrganizer::MenuDataDump(String src){
  Serial.print("\n" + src + " MenuOrganizer::MenuDataDump");
  char buff[300];
  sprintf(buff, "\nGUI::HandleMenuControls - menu data dump\nlen=%d\nactive_len=%d\ncurrent_option.x=%d\ncurrent_option.y=%d\nselected=%d\nmax_options_displayed=%d\nstarting_option=%d\ncurrent_option_relative=%d\n\0",
    menu->len,
    menu->active_len,
    menu->current_option.x,
    menu->current_option.y,
    menu->selected,
    menu->max_options_displayed,
    menu->starting_option,
    menu->current_option_relative
  );
   Serial.println(buff);
   Serial.println();
}


void MenuOrganizer::ShiftCurrentOption(int buttonID){
  switch(buttonID){
    case BUTTON_UP:
      menu->current_option.y--;      
      if(menu->current_option.y < 0){
        menu->current_option.y = menu->active_len - 1;
        menu->current_option_relative = menu->max_options_displayed - 1;
        if(menu->current_option_relative >= menu->active_len){menu->current_option_relative = menu->active_len -1;}

        menu->starting_option--;
        if (menu->starting_option < 0){
          menu->starting_option = menu->active_len - menu->max_options_displayed;
          if(menu->starting_option < 0){menu->starting_option = 0;}
        }
      }else{
        menu->current_option_relative--;
        if(menu->current_option_relative < 0){
          menu->current_option_relative = 0;
          menu->starting_option--;
          if (menu->starting_option < 0){
            menu->starting_option = menu->active_len - menu->max_options_displayed;
            if(menu->starting_option < 0){menu->starting_option = 0;}
            menu->current_option_relative = menu->max_options_displayed-1;
            if(menu->current_option_relative > menu->current_option.y){menu->current_option_relative = menu->current_option.y;}
          }
        }    
      }
      
      
      break;
    case BUTTON_DOWN:
      menu->current_option.y++;
      if(menu->current_option.y >= menu->active_len){
        menu->current_option.y = 0;  
        menu->starting_option = 0;
        menu->current_option_relative = 0;
      }
      else{
        menu->current_option_relative++;
        if(menu->current_option_relative >= menu->max_options_displayed){
          menu->current_option_relative = menu->max_options_displayed-1;
          menu->starting_option++;
          if (menu->starting_option > menu->active_len - menu->max_options_displayed){
            menu->starting_option = 0;
            menu->current_option_relative = 0;
          }
        }
      }
      //if(menu->current_option.y > 5){
        //starting_y_draw = font_size*-1 * (menu->current_option.y - 5);
      //}

      break;
    case BUTTON_LEFT:
      menu->current_option.x--;
      if(menu->current_option.x < 0){
        menu->current_option.x = menu->horizontal_options->size() - 1;
      }
      break;
    case BUTTON_RIGHT:
      menu->current_option.x++;
      if(menu->current_option.x >= menu->horizontal_options->size()){
        menu->current_option.x = 0;
      }
      break;
  }

  //char buff[100];
  //sprintf(buff, "menu->current_option.x = %d \nmenu->current_option.y = %d \nmenu_len = %d\nactive_menu_len = %d", menu->current_option.x, menu->current_option.y, menu_len, active_menu_len);
  //Serial.println(buff);
}




/*
void MenuOrganizer::InitMenu(Menu *_menu, int max_options_disp){
  // useless 

  _menu->len = 0;
  _menu->active_len = 0;
  _menu->current_option = {0,0};
  _menu->selected = INVALID;
  _menu->max_options_displayed_default = max_options_disp;
  _menu->max_options_displayed = max_options_disp;
  _menu->starting_option = 0;
  _menu->current_option_relative = 0;
}
*/



void MenuOrganizer::UpdateActiveMenu(){
  //memset(active_menu, 0, sizeof(Menu)); WRONG (LinkedList must be resetted explicitly)
  /*
  active_menu->len = 0;
  for(int i=0; i < menu->len; i++){
    if(menu->options->get(i).active == true){
      active_menu->len++;
      
      for(int c=0; c < MAX_MENU_TEXT-1; c++){
        active_menu->option[active_menu->len-1].text[c] = menu->options->get(i).text[c];
      }
      active_menu->option[active_menu->len-1].text[MAX_MENU_TEXT-1] = '\0';
      active_menu->option[active_menu->len-1].function = menu->options->get(i).function;
    }   
  }

  if(active_menu->current_option.y >= active_menu->len){
    active_menu->current_option.y = active_menu->len - 1;
  }

  selected = INVALID;
  menu->starting_option = 0;
  menu->current_option_relative = 0;
  if(active_menu_len < max_options_displayed_default){
    menu->max_options_displayed = active_menu->len;
  }
  else{
    menu->max_options_displayed = menu->max_options_displayed_default;
  }

  */
}





























/*
mainMenu = {
  5, //len
  2, //active_len
  0,0, // current option
  INVALID, //selection
  5, //max_options_displayed_default
  5, //max_options_displayed
  0, //starting_option
  0, //current_option_relative

  
    "Read card\0", true, [](){
        gui.DrawNotification("> Reading <\nWaiting for the card...\0");
        if(WaitForCard_AllowCancel()){
          bool res = rfid.ReadCard();
          DrawResult(res);
          if(res == true){
            gui.ActivateOption(2);
            gui.ActivateOption(3);
            gui.ActivateOption(4);
            
          }
        }
      },
      
    "Load from memory", true, [](){
        MenuOption dirSelection[30];
        Dir dir = SPIFFS.openDir("/rfid");
        int count = 0;
        while(dir.next()) {
          strcpy(dirSelection[count].text, dir.fileName().c_str()+6);
          dirSelection[count].text[dir.fileName().length()-10] = '\0';
          dirSelection[count].active = true;
          count++;
        }
        gui.SetMenu(dirSelection, count);
        while(true){
          gui.HandleMenuControls();
          if(gui.WasSelected()){
            int choice = gui.GetSelection();
            if(choice != CANCELLED){
              //dirSelection[choice].text is the chosen file name
              int res = rfid.ReadFromFile(dirSelection[choice].text);
              DrawResult(res);
              if(res == true){
                mainMenu[2].active = true; gui.ActivateOption(2);
                mainMenu[3].active = true; gui.ActivateOption(3);
                mainMenu[4].active = false; gui.DeactivateOption(4);
              }
              //Serial.println(dirSelection[choice].text);
              //rfid.DumpFromFile(dirSelection[choice].text);
              break;
            }
            else{
              break;
            }
            gui.ResetSelection();
          }
          gui.Draw();
        }
        //gui.Set(mainMenu, mainMenuLen);
      },
      
    "Write card\0", false, [](){
        gui.DrawNotification("< Writing >\nWaiting for the card...\0");
        if(WaitForCard_AllowCancel()){
          bool res = rfid.WriteCard();
          DrawResult(res);
        }
      },
      
    "Dump to serial\0", false, [](){
        rfid.DumpToSerial();
        DrawResult(true);
      },
      
    "Save to memory\0", false, [](){
        gui.GetNamePrompt();
      },
      
    "Manage files\0", true, [](){
      
      },
    
};
*/




/*
https://arduino-esp8266.readthedocs.io/en/2.4.1/libraries.html#esp-specific-apis
ESP-specific APIs
Some ESP-specific APIs related to deep sleep, RTC and flash memories are available in the ESP object.
ESP.deepSleep(microseconds, mode) will put the chip into deep sleep. mode is one of WAKE_RF_DEFAULT, WAKE_RFCAL, WAKE_NO_RFCAL, WAKE_RF_DISABLED. (GPIO16 needs to be tied to RST to wake from deepSleep.)
ESP.rtcUserMemoryWrite(offset, &data, sizeof(data)) and ESP.rtcUserMemoryRead(offset, &data, sizeof(data)) allow data to be stored in and retrieved from the RTC user memory of the chip respectively. Total size of RTC user memory is 512 bytes, so offset + sizeof(data) shouldn’t exceed 512. Data should be 4-byte aligned. The stored data can be retained between deep sleep cycles. However, the data might be lost after power cycling the chip.
ESP.restart() restarts the CPU.
ESP.getResetReason() returns a String containing the last reset reason in human readable format.
ESP.getFreeHeap() returns the free heap size.
ESP.getChipId() returns the ESP8266 chip ID as a 32-bit integer.
ESP.getCoreVersion() returns a String containing the core version.
ESP.getSdkVersion() returns the SDK version as a char.
ESP.getCpuFreqMHz() returns the CPU frequency in MHz as an unsigned 8-bit integer.
ESP.getSketchSize() returns the size of the current sketch as an unsigned 32-bit integer.
ESP.getFreeSketchSpace() returns the free sketch space as an unsigned 32-bit integer.
ESP.getSketchMD5() returns a lowercase String containing the MD5 of the current sketch.
ESP.getFlashChipId() returns the flash chip ID as a 32-bit integer.
ESP.getFlashChipSize() returns the flash chip size, in bytes, as seen by the SDK (may be less than actual size).
ESP.getFlashChipRealSize() returns the real chip size, in bytes, based on the flash chip ID.
ESP.getFlashChipSpeed(void) returns the flash chip frequency, in Hz.
ESP.getCycleCount() returns the cpu instruction cycle count since start as an unsigned 32-bit. This is useful for accurate timing of very short actions like bit banging.
ESP.getVcc() may be used to measure supply voltage. ESP needs to reconfigure the ADC at startup in order for this feature to be available. Add the following line to the top of your sketch to use getVcc:
ADC_MODE(ADC_VCC);
TOUT pin has to be disconnected in this mode.
Note that by default ADC is configured to read from TOUT pin using analogRead(A0), and ESP.getVCC() is not available.  
*/
