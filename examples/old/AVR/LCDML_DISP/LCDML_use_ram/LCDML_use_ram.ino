// ============================================================                                                            
// Example:     LCDML: use ram                         
// ============================================================
// Autor:       Nils Feldkämper
// Last update: 08.01.2017
// License:     MIT                                     
// ============================================================ 
// Descripton: 
// This example shows how the menu works with a menu wich only 
// use ram for saving the menu items. This example is for controller
// with enogh ram. All menu item texts can be changed in the program 
// at runtime.
// ============================================================ 

  // include libs
  #include <LiquidCrystal.h>
  #include <LCDMenuLib.h>
  
  // lib config
  #define _LCDML_DISP_cfg_button_press_time          200    // button press time in ms
  #define _LCDML_DISP_cfg_enable_use_ram_mode        1      // this mode save the menu item strings in ram 
                                                            // normaly the data is saved in flash 

// ********************************************************************* 
// LCDML TYPE SELECT
// *********************************************************************
  // settings for lcd 
  #define _LCDML_DISP_cols             20
  #define _LCDML_DISP_rows             4  

  // lcd object
  // liquid crystal needs (rs, e, dat4, dat5, dat6, dat7)
  LiquidCrystal lcd(4,5,6,7,8,9);
  

// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // create menu
  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    11
  
  // LCDML_root        => layer 0 
  // LCDML_root_X      => layer 1 
  // LCDML_root_X_X    => layer 2 
  // LCDML_root_X_X_X  => layer 3 
  // LCDML_root_...      => layer ... 
  
  // LCDMenuLib_add(id, group, prev_layer_element, new_element_num, lang_char_array, callback_function)
  LCDML_DISP_init(_LCDML_DISP_cnt);
  LCDML_DISP_add_use_ram      (0  , _LCDML_G1  , LCDML_root        , 1  , "Information"        , LCDML_FUNC_information);
  LCDML_DISP_add_use_ram      (1  , _LCDML_G1  , LCDML_root        , 2  , "Time info"          , LCDML_FUNC_timer_info);
  LCDML_DISP_add_use_ram      (2  , _LCDML_G1  , LCDML_root        , 3  , "Settings"           , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (3  , _LCDML_G1  , LCDML_root_3      , 1  , "Change value"       , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (4  , _LCDML_G1  , LCDML_root_3      , 2  , "Something"          , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (5  , _LCDML_G1  , LCDML_root        , 4  , "Program"            , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (6  , _LCDML_G1  , LCDML_root_4      , 1  , "Program 1"          , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (7  , _LCDML_G1  , LCDML_root_4_1    , 1  , "P1 start"           , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (8  , _LCDML_G1  , LCDML_root_4_1    , 2  , "Settings"           , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (9  , _LCDML_G1  , LCDML_root_4_1_2  , 1  , "Warm"               , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (10 , _LCDML_G1  , LCDML_root_4_1_2  , 2  , "Long"               , LCDML_FUNC);
  LCDML_DISP_add_use_ram      (11 , _LCDML_G1  , LCDML_root_4      , 2  , "Program 2"          , LCDML_FUNC_p2);
  LCDML_DISP_createMenu_use_ram(_LCDML_DISP_cnt);
  //LCDML_DISP_createMenu(_LCDML_DISP_cnt);


  



// ********************************************************************* 
// LCDML BACKEND (core of the menu, do not change here anything yet)
// ********************************************************************* 
  // define backend function  
  #define _LCDML_BACK_cnt    1  // last backend function id
  
  LCDML_BACK_init(_LCDML_BACK_cnt);
  LCDML_BACK_new_timebased_dynamic (0  , ( 20UL )         , _LCDML_start  , LCDML_BACKEND_control);
  LCDML_BACK_new_timebased_dynamic (1  , ( 10000000UL )   , _LCDML_stop   , LCDML_BACKEND_menu);
  LCDML_BACK_create();


// *********************************************************************
// SETUP
// *********************************************************************
  void setup()
  {  
    // serial init; only be needed if serial control is used     
    Serial.begin(9600);                // start serial    
    Serial.println(F(_LCDML_VERSION)); // only for examples
      
    // Enable all items with _LCDML_G1
    LCDML_DISP_groupEnable(_LCDML_G1); // enable group 1
  
    // LCDMenu Setup
    LCDML_setup(_LCDML_BACK_cnt);  


    // change content dynamic after initialisation
    // can only used when "_LCDML_DISP_cfg_enable_use_ram_mode" is defined and "use_ram" menu items are used       
    LCDML_DISP_dynText(1, "Test 12345678");   

    
  }

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  { 
    // this function must called here, do not delete it
    LCDML_run(_LCDML_priority); 
  }




// *********************************************************************
// check some errors - do not change here anything
// *********************************************************************
# if(_LCDML_DISP_rows > _LCDML_DISP_cfg_max_rows)
# error change value of _LCDML_DISP_cfg_max_rows in LCDMenuLib.h
# endif
# if(_LCDML_DISP_cols > _LCDML_DISP_cfg_max_string_length)
# error change value of _LCDML_DISP_cfg_max_string_length in LCDMenuLib.h
# endif
