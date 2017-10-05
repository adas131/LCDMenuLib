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
  void LCDML_lcd_menu_display(); 
  void LCDML_lcd_menu_clear(); 
  void LCDML_lcd_trigger_backend();

// *********************************************************************
// Objects
// *********************************************************************
  LCDMenu LCDML_0 (0, true, 0, NULL); // root menu element (do not change)
  LCDMenuLib LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, LCDML_lcd_menu_display, LCDML_lcd_menu_clear, LCDML_lcd_trigger_backend);

  
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
  LCDML_DISP_addFunction    (0  , _LCDML_G1   , LCDML_0         , 1  , "Information"   , LCDML_FUNC_information);  // a small function
  LCDML_DISP_addFunction    (1  , _LCDML_G1   , LCDML_0         , 2  , "Time info"     , LCDML_FUNC_timer_info);   // a function which display a secound counter
  LCDML_DISP_addMenu        (2  , _LCDML_G1   , LCDML_0         , 3  , "Settings");
  LCDML_DISP_addPlaceholder (3  , _LCDML_G1   , LCDML_0_3       , 1);                     // you can find the content in "LCDML_DISP" tab
  LCDML_DISP_addPlaceholder (4  , _LCDML_G1   , LCDML_0_3       , 2);                     // you can find the content in "LCDML_DISP" tab
  LCDML_DISP_addFunction    (5  , _LCDML_G1   , LCDML_0_3       , 3  , "Back"          , LCDML_FUNC_back);
  LCDML_DISP_addMenu        (6  , _LCDML_G1   , LCDML_0         , 4  , "Program");
  LCDML_DISP_addMenu        (7  , _LCDML_G1   , LCDML_0_4       , 1  , "Program 1");
  LCDML_DISP_addFunction    (8  , _LCDML_G1   , LCDML_0_4_1     , 1  , "P1 start"      , LCDML_FUNC_back);//LCDML_FUNC_p1_start);
  LCDML_DISP_addMenu        (9  , _LCDML_G1   , LCDML_0_4_1     , 2  , "Settings");
  LCDML_DISP_addMenu        (10 , _LCDML_G1   , LCDML_0_4_1_2   , 1  , "Warm");
  LCDML_DISP_addMenu        (11 , _LCDML_G1   , LCDML_0_4_1_2   , 2  , "Long");
  LCDML_DISP_addFunction    (12 , _LCDML_G1   , LCDML_0_4_1_2   , 3  , "Back"          , LCDML_FUNC_back);
  LCDML_DISP_addFunction    (13 , _LCDML_G1   , LCDML_0_4_1     , 3  , "Back"          , LCDML_FUNC_back);
  LCDML_DISP_addFunction    (14 , _LCDML_G1   , LCDML_0_4       , 2  , "Program 2"     , LCDML_FUNC_back);// LCDML_FUNC_p2);           // a small function to press
  LCDML_DISP_addFunction    (15 , _LCDML_G1   , LCDML_0_4       , 3  , "Back"          , LCDML_FUNC_back);
  LCDML_DISP_addFunction    (16 , _LCDML_HIDE , LCDML_0         , 5  , "a"              , LCDML_FUNC_back);// LCDML_FUNC_screensaver);
  LCDML_DISP_addFunction    (17 , _LCDML_HIDE , LCDML_0         , 6  , "b"              , LCDML_FUNC_back);//LCDML_FUNC_initscreen);         
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
          
    // Enable all items with _LCDML_G1
    LCDML.GP_display(_LCDML_G1);      

    // LCDMenuLib Setup
    LCDML_setup(_LCDML_BACK_cnt);

  
    

    // Display at first this function:
    // When you want to see the menu at first, delete this line 
    // not working at the moment 
    //LCDML_DISP_jumpToFunc(LCDML_FUNC_initscreen);



    
  }

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  { 
    // lcdml loop control
    // try to change here nothing
    if (bitRead(LCDML.control, _LCDML_control_funcend)) {                                               
      LCDML_BACK_reset(LCDML_BACKEND_menu);                                                           
      LCDML_BACK_dynamic_setDefaultTime(LCDML_BACKEND_menu);                                          
      LCDML_BACK_stopStable(LCDML_BACKEND_menu);                                                      
    }                                                                                                   
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
