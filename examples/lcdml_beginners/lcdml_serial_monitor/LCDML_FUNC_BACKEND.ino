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
    void LCDML_BACK_setup(LCDML_BACKEND__your_name)
    {
      // setup
      // is called only if it is started or restartet (reset+start)
    }    
    void LCDML_BACK_loop(LCDML_BACKEND__your_name)
    {    
      // called in a interval time which is set by initialisation
    }    
    void LCDML_BACK_stable(LCDML_BACKEND__your_name)
    {
      // stable stop
      // is called when a backend function is stopped with stopStable  
    }
 * ===================================================================== *
 */

// *********************************************************************
// insert here your own backend functions
// *********************************************************************

/*
void LCDML_BACK_setup(LCDML_BACKEND_motor_control)
{
  // setup
  // is called only if it is started or restartet (reset+start)
}
void LCDML_BACK_loop(LCDML_BACKEND_motor_control)
{    
  // called in a interval time which is set by initialisation
}
void LCDML_BACK_stable(LCDML_BACKEND_motor_control)
{
  // stable stop
  // is called when a backend function is stopped with stopStable  
}



void LCDML_BACK_setup(LCDML_BACKEND_program_1_washing)
{
  // setup
  // is called only if it is started or restartet (reset+start)
}
void LCDML_BACK_loop(LCDML_BACKEND_program_1_washing)
{    
  // called in a interval time which is set by initialisation
}
void LCDML_BACK_stable(LCDML_BACKEND_program_1_washing)
{
  // stable stop
  // is called when a backend function is stopped with stopStable  
}


void LCDML_BACK_setup(LCDML_BACKEND_screensaver)
{
  // setup
  // is called only if it is started or restartet (reset+start)
}
void LCDML_BACK_loop(LCDML_BACKEND_screensaver)
{ 
  // called in a interval time which is set by initialisation  
  //LCDML_DISP_jumpToFunc(LCDML_FUNC_screensaver);
}
void LCDML_BACK_stable(LCDML_BACKEND_screensaver)
{
  // stable stop
  // is called when a backend function is stopped with stopStable  
}
*/


// *********************************************************************
// Menu Task, DO NOT CHANGE
// *********************************************************************
void LCDML_BACK_setup(LCDML_BACKEND_menu) 
// *********************************************************************                            
{   
   Serial.println("delete backend");                      
                                                              
}                                                                   
void LCDML_BACK_loop(LCDML_BACKEND_menu)                            
{   
  
}                                                                    
void LCDML_BACK_stable(LCDML_BACKEND_menu)                            
{                                                                                                                                  
}

 
