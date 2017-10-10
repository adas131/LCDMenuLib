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

// *********************************************************************
// Prototypes
// *********************************************************************
  void menu_display(); 
  void menu_clear(); 
  void trigger_backend();

// *********************************************************************
// Condetions to show a menu element on the display
// *********************************************************************
  boolean COND_shown() { return true; }    // show 
  boolean COND_hidden() { return false; }  // hidden

// *********************************************************************
// Objects
// *********************************************************************
  lcdmlMenuElement LCDML_0 (255, 0, NULL, NULL); // root menu element (do not change)
  
  LCDMenuLib LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, menu_display, menu_clear, trigger_backend);

  
// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // create menu
  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    17
  
  // LCDML_0        => layer 0 
  // LCDML_0_X      => layer 1 
  // LCDML_0_X_X    => layer 2 
  // LCDML_0_X_X_X  => layer 3 
  // LCDML_0_...      => layer ... 

  // LCDMenuLib_add(id, group, prev_layer_element, new_element_num, lang_char_array, callback_function)
  LCDML_DISP_addFunction    (0  , LCDML_0         , 1  , COND_shown,  "Information"   , FUNC_information);  // a small function
  LCDML_DISP_addFunction    (1  , LCDML_0         , 2  , COND_shown,  "Time info"     , FUNC_timer_info);   // a function which display a secound counter
  LCDML_DISP_addMenu        (2  , LCDML_0         , 3  , COND_shown,  "Settings");
  LCDML_DISP_addPlaceholder (3  , LCDML_0_3       , 1  , COND_shown);                     // you can find the content in "LCDML_DISP" tab
  LCDML_DISP_addPlaceholder (4  , LCDML_0_3       , 2  , COND_shown);                     // you can find the content in "LCDML_DISP" tab
  LCDML_DISP_addFunction    (5  , LCDML_0_3       , 3  , COND_shown,  "Back"          , FUNC_back);
  LCDML_DISP_addMenu        (6  , LCDML_0         , 4  , COND_shown,  "Program");
  LCDML_DISP_addMenu        (7  , LCDML_0_4       , 1  , COND_shown,  "Program 1");
  LCDML_DISP_addFunction    (8  , LCDML_0_4_1     , 1  , COND_shown,  "P1 start"      , FUNC_back);//LCDML_FUNC_p1_start);
  LCDML_DISP_addMenu        (9  , LCDML_0_4_1     , 2  , COND_shown,  "Settings");
  LCDML_DISP_addMenu        (10 , LCDML_0_4_1_2   , 1  , COND_shown,  "Warm");
  LCDML_DISP_addMenu        (11 , LCDML_0_4_1_2   , 2  , COND_shown,  "Long");
  LCDML_DISP_addFunction    (12 , LCDML_0_4_1_2   , 3  , COND_shown,  "Back"          , FUNC_back);
  LCDML_DISP_addFunction    (13 , LCDML_0_4_1     , 3  , COND_shown,  "Back"          , FUNC_back);
  LCDML_DISP_addFunction    (14 , LCDML_0_4       , 2  , COND_shown,  "Program 2"     , FUNC_back);// LCDML_FUNC_p2);           // a small function to press
  LCDML_DISP_addFunction    (15 , LCDML_0_4       , 3  , COND_shown,  "Back"          , FUNC_back);
  LCDML_DISP_addFunction    (16 , LCDML_0         , 5  , COND_hidden, "a"             , FUNC_back);// LCDML_FUNC_screensaver);
  LCDML_DISP_addFunction    (17 , LCDML_0         , 6  , COND_hidden, "b"             , FUNC_back);//LCDML_FUNC_initscreen);         
  LCDML_DISP_createMenu(_LCDML_DISP_cnt);


// ********************************************************************* 
// LCDML BACKEND (core of the menu, do not change here anything yet)
// ********************************************************************* 
   
  #define _LCDML_BACK_cnt    1  // last backend function id
  
  LCDML_BACK_init();
  //LCDML_BACK_new_timebased_static  (0  , ( 1000UL )       , _LCDML_us, _LCDML_stop        , _LCDML_next, LCDML_BACKEND_motor_control);    // custom function
  LCDML_BACK_new_timebased_dynamic (0  , ( 20000UL )      , _LCDML_us, _LCDML_startDirect , _LCDML_root, LCDML_BACKEND_control);          // you need this function for the menu 
  LCDML_BACK_new_timebased_dynamic (1  , ( 10000000UL )   , _LCDML_ms, _LCDML_stop        , _LCDML_root, LCDML_BACKEND_menu);             // you need this function for the menu
  //LCDML_BACK_new_timebased_static  (3  , ( 1000UL )       , _LCDML_ms, _LCDML_stop        , _LCDML_root, LCDML_BACKEND_program_1_washing);// custom function
  //LCDML_BACK_new_timebased_static  (4  , ( 10000UL )      , _LCDML_ms, _LCDML_start       , _LCDML_root, LCDML_BACKEND_screensaver);      // custom function
   

// *********************************************************************
// SETUP
// *********************************************************************
  void setup()
  {  
    // serial init; only be needed if serial control is used     
    Serial.begin(9600);                // start serial    
    Serial.println(F(_LCDML_VERSION)); // only for examples
    
    // LCDMenuLib Setup
    LCDML_setup(_LCDML_BACK_cnt);
  }

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  {
    LCDML.loop();
                                                                                                       
    for(uint8_t l_LCDML_BACK_i = 0; l_LCDML_BACK_i<g_LCDML_BACK_cnt;l_LCDML_BACK_i++) {                 
      g_LCDML_BACK_priority[(l_LCDML_BACK_i)]();                                                      
      if (_LCDML_priority == true && g_LCDML_BACK_loop_status == false)                                          
      {                                                                                               
        g_LCDML_BACK_loop_status = true;                                                            
        break;                                                                                      
      }                                                                                               
    }
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
