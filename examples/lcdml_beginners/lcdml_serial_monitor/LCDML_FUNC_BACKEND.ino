/* ===================================================================== *
 *                                                                       *
 * BACKEND SYSTEM                                                        *
 *                                                                       *
 * ===================================================================== *
 * every "backend function" needs three functions 
 * - void LCDML_BACK_setup(..func_name..)    
 * - void LCDML_BACK_loop(..func_name..)     
 * - void LCDML_BACK_stable(..func_name..)
 *
 * - every BACKEND function can be stopped and started
 * EXAMPLE CODE:
    void LCDML_BACK_setup(LCDML_BACKEND_control)
    {
      // setup
      // is called only if it is started or restartet (reset+start)
    }
    
    void LCDML_BACK_loop(LCDML_BACKEND_control)
    {    
      // runs in loop
      
     
      return false;  
    }
    
    void LCDML_BACK_stable(LCDML_BACKEND_control)
    {
      // stable stop
      // is called when a backend function is stopped with stopStable  
    }
 * ===================================================================== *
 */

// *********************************************************************
// insert here your own backend functions
// *********************************************************************


















// *********************************************************************
// Control Task, DO NOT CHANGE
// *********************************************************************
void LCDML_BACK_setup(LCDML_BACKEND_control)      
// *********************************************************************
{
  // call setup   
  LCDML_CONTROL_setup();      
}
// backend loop
void LCDML_BACK_loop(LCDML_BACKEND_control)
{    
  // call loop
  LCDML_CONTROL_loop();
}
// backend stop stable
void LCDML_BACK_stable(LCDML_BACKEND_control)
{
}



// *********************************************************************
// Menu Task, DO NOT CHANGE
// *********************************************************************
void LCDML_BACK_setup(LCDML_BACKEND_menu) 
// *********************************************************************                            
{    
  // get current function id                                                                 
  g_LCDML_BACK_lastFunc = LCDML.getFunction();                     
  
  // check current running function 
  if (g_LCDML_DISP_functions_loop_setup[g_LCDML_BACK_lastFunc] == LCDML_FUNC_loop_setup) 
  {
    // stop old function if running
    bitSet(LCDML.control, _LCDML_control_funcend);              
  }                                                                
  else if (g_LCDML_BACK_lastFunc != _LCDML_NO_FUNC) 
  {
    // display new function content              
    LCDML_lcd_menu_clear();                                        
    LCDML_BUTTON_resetAll();                                    
    g_LCDML_DISP_functions_loop_setup[g_LCDML_BACK_lastFunc](); 
  }                                                                
}                                                                   
void LCDML_BACK_loop(LCDML_BACKEND_menu)                            
{                                                                    
  if (LCDML.getFunction() != _LCDML_NO_FUNC) 
  {
    g_LCDML_DISP_functions_loop[LCDML.getFunction()]();
  }
}                                                                    
void LCDML_BACK_stable(LCDML_BACKEND_menu)                            
{                                                                    
  if (g_LCDML_BACK_lastFunc != _LCDML_NO_FUNC) 
  {
    // call loop end function                   
    g_LCDML_DISP_functions_loop_end[g_LCDML_BACK_lastFunc]();   
    // reset last function
    g_LCDML_BACK_lastFunc = _LCDML_NO_FUNC;                     
    // clear display/console
    LCDML_lcd_menu_clear();                                        
    // display menu
    LCDML.display();                                               
    LCDML_lcd_menu_display();  
    // reset all button values                                 
    LCDML_BUTTON_resetAll(); 
    // reset current function                                   
    LCDML.function = _LCDML_NO_FUNC;                            
    bitClear(LCDML.control, _LCDML_control_funcend);            
    // jump to a function if enabled
    if(g_lcdml_jump_func != _LCDML_NO_FUNC) {                    
      LCDML.jumpToElement(g_lcdml_jump_func);                    
      LCDML_DISP_update_menu();                                 
      g_lcdml_jump_func = _LCDML_NO_FUNC;                        
    }
    // go root if enabled                                                            
    if(bitRead(LCDML.control, _LCDML_control_go_root)) {        
      LCDML.goRoot();                                            
      LCDML.display();                                        
      LCDML_lcd_menu_display();                               
    }                                                            
  }                                                                
}

 
