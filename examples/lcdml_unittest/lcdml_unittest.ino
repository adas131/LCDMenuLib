// ============================================================                                                            
// Example:     LCDML_serialmonitor
// ============================================================ 
// Descripton: 
// This example shows how the menu works without any LCD.
// The output is print in serial console. This example can be 
// extended with network functions to print this menu with telnet 
// or other protocols.
// ============================================================ 

  #include <LCDMenuLib.h>
  
// ********************************************************************* 
// LCDML TYPE SELECT
// *********************************************************************
  // settings for lcd 
  #define _LCDML_DISP_cols  20
  #define _LCDML_DISP_rows  4 

  #define _LCDML_DISP_cfg_max_string_length    20  // max string length witch can be display

// *********************************************************************
// Prototypes
// *********************************************************************
  void lcdml_menu_display(); 
  void lcdml_menu_clear(); 
  void lcdml_menu_control();

// *********************************************************************
// Objects
// *********************************************************************
  LCDMenuLib_menu LCDML_0 (255, 0, NULL, NULL); // root menu element (do not change)
  LCDMenuLib LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);
  
// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // LCDML_0        => layer 0 
  // LCDML_0_X      => layer 1 
  // LCDML_0_X_X    => layer 2 
  // LCDML_0_X_X_X  => layer 3 
  // LCDML_0_...      => layer ... 

  // LCDMenuLib_add(id, prev_layer, new_num, condetion, lang_char_array, dyncontent_callback, callback_function, parameter (0-255))
  LCDML_add         (0  , LCDML_0         , 1  , "Information"   , mFunc_information);  // a small function
  LCDML_addAdvanced (1  , LCDML_0         , 2  , NULL     ,  "Time info"         , NULL    , mFunc_timer_info    , 0   );  // a function which display a secound counter
  LCDML_addAdvanced (2  , LCDML_0         , 3  , NULL     ,  "Settings"          , NULL    , NULL                , 0   );
  LCDML_addAdvanced (3  , LCDML_0_3       , 1  , NULL     ,  ""                  , cFunc_s1, NULL                , 0   );       
  LCDML_addAdvanced (4  , LCDML_0_3       , 2  , NULL     ,  ""                  , cFunc_s2, NULL                , 0   );       
  LCDML_addAdvanced (5  , LCDML_0_3       , 3  , NULL     ,  "Back"              , NULL    , mFunc_back          , 0   );
  LCDML_addAdvanced (6  , LCDML_0         , 4  , NULL     ,  "Program"           , NULL    , NULL                , 0   );
  LCDML_addAdvanced (7  , LCDML_0_4       , 1  , NULL     ,  "Program 1"         , NULL    , NULL                , 0   );
  LCDML_addAdvanced (8  , LCDML_0_4_1     , 1  , NULL     ,  "P1 start"          , NULL    , NULL                , 0   );//LCDML_FUNC_p1_start);
  LCDML_addAdvanced (9  , LCDML_0_4_1     , 2  , NULL     ,  "Settings"          , NULL    , NULL                , 0   );
  LCDML_addAdvanced (10 , LCDML_0_4_1_2   , 1  , NULL     ,  "Warm"              , NULL    , NULL                , 0   );
  LCDML_addAdvanced (11 , LCDML_0_4_1_2   , 2  , NULL     ,  "Long"              , NULL    , NULL                , 0   );
  LCDML_addAdvanced (12 , LCDML_0_4_1_2   , 3  , NULL     ,  "Back"              , NULL    , mFunc_back          , 0   );
  LCDML_addAdvanced (13 , LCDML_0_4_1     , 3  , NULL     ,  "Back"              , NULL    , mFunc_back          , 0   );
  LCDML_addAdvanced (14 , LCDML_0_4       , 2  , NULL     ,  "Program 2"         , NULL    , mFunc_p2            , 0   );           // a small function to press
  LCDML_addAdvanced (15 , LCDML_0_4       , 3  , NULL     ,  "Back"              , NULL    , mFunc_back          , 0   );
  
  // test: hidden menu elements
  LCDML_addAdvanced (16 , LCDML_0         , 5  , COND_hide,  ""                  , NULL    , mFunc_screensaver   , 0   ); 
  LCDML_addAdvanced (17 , LCDML_0         , 6  , COND_hide,  ""                  , NULL    , mFunc_initscreen    , 0   ); 
  
  // test: normal buttons after hidden menus + jump functions
  LCDML_addAdvanced (18 , LCDML_0         , 7  , NULL     ,  "goRoot"            , NULL    , mFunc_goRoot        , 0   ); 
  LCDML_addAdvanced (19 , LCDML_0         , 8  , NULL     ,  "special"           , NULL    , NULL                , 0   ); 
  LCDML_addAdvanced (20 , LCDML_0_8       , 1  , NULL     ,  "jump to func p2"   , NULL    , mFunc_jumpToFunc    , 0   ); 
  LCDML_addAdvanced (21 , LCDML_0_8       , 2  , NULL     ,  "something else"    , NULL    , NULL                , 0   ); 
  
  // special test: a hidden menu with visible sub menus (you can only enter this menu with LCDML.OTHER_setCursorToMenu(id) or with LCDML.Other_jumpToFunc(id) 
  LCDML_addAdvanced (22 , LCDML_0         , 9  , COND_hide,  "hidden menu"       , NULL    , NULL                , 0   ); 
  LCDML_addAdvanced (23 , LCDML_0_9       , 1  , NULL     ,  "visible submenu 1" , NULL    , mFunc_test_hidden   , 0   ); 
  LCDML_addAdvanced (24 , LCDML_0_9       , 2  , NULL     ,  "visible submenu 2" , NULL    , NULL                , 0   );
  LCDML_addAdvanced (25 , LCDML_0_9       , 3  , NULL     ,  "Back"              , NULL    , NULL                , 0   );  
  
  // normal function
  LCDML_add         (26 , LCDML_0         ,10  , "B1"  , NULL );          
  
  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    26
  
  LCDML_createMenu(_LCDML_DISP_cnt);

// *********************************************************************
// SETUP
// *********************************************************************
  void setup()
  {  
    // serial init; only be needed if serial control is used     
    Serial.begin(9600);                // start serial    
    Serial.println(F(_LCDML_VERSION)); // only for examples
    
    // LCDMenuLib Setup
    LCDML_setup(_LCDML_DISP_cnt);

    // Enable Menü Rollover 
    LCDML.MENU_enRollover();    

    // Enable Screensaver (screensaver menu function, time to activate in ms)
    LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 secounds  
    
  }

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  {
    LCDML.loop();
  }

// *********************************************************************
// check some errors - do not change here anything
// *********************************************************************
# if(_LCDML_DISP_rows > _LCDML_DISP_cfg_max_rows)
# error change value of _LCDML_DISP_cfg_max_rows in LCDMenuLib.h
# endif
# if(_LCDML_DISP_cols > _LCDML_DISP_cfg_max_string_length)
# error change value of _LCDML_DISP_cfg_max_string_length on the top of this file
# endif
