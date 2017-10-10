/* ===================================================================== *
 *                                                                       *
 * DISPLAY SYSTEM                                                        *
 *                                                                       *
 * ===================================================================== *
 *
 * EXAMPLE CODE:
    void your_function_name(void) 
    {
      if(LCDML.FUNC_setup()) 
      { // ****** SETUP *********
        // setup
        // is called only if it is started
        LCDML.DISP_menuClear();                                        
        LCDML.BT_resetAll();
        
  
        // starts a trigger event for the loop function every 100 millisecounds
        LCDML.FUNC_setTrigger(100); 
      }     
    
      if(LCDML.FUNC_loop()) 
      { // ****** LOOP *********   
        // loop
        // is called when it is triggert
        // - with LCDML_DISP_triggerMenu( millisecounds ) 
        // - with every button status change
  
        // check if any button is presed (enter, up, down, left, right)
        if(LCDML.BT_chkAny()) {         
          LCDML.FUNC_goBackToMenu();
        } 
      }

      if(LCDML.FUNC_close()) 
      { // ****** STABLE END *********
        // loop end
        // this functions is ever called when a DISP function is quit
        // you can here reset some global vars or do nothing 
      } 
    } 
 * ===================================================================== * 
 */









// *********************************************************************
void FUNC_information(void)
// *********************************************************************
{
  if(LCDML.FUNC_setup()) 
  {  /* **** SETUP **** */
    // setup function
    LCDML.DISP_menuClear();                                        
    LCDML.BT_resetAll();
        
    Serial.println(F("==========================================="));
    Serial.println(F("================  FUNC ===================="));
    Serial.println(F("==========================================="));
   
    Serial.println(F("To close this"));  
    Serial.println(F("function press"));
    Serial.println(F("any button or use")); 
    Serial.println(F("back button"));
  }


  if(LCDML.FUNC_loop()) 
  { /* **** LOOP **** */
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function   
    if(LCDML.BT_chkAny()) { // check if any button is presed (enter, up, down, left, right)
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }


  if(LCDML.FUNC_close()) 
  { /* **** LOOP END **** */
    // this functions is ever called when a DISP function is quit
    // you can here reset some global vars or do nothing 
  } 
}



uint8_t g_func_timer_info = 0;  // time counter (global variable)
unsigned long g_timer_1 = 0;    // timer variable (globale variable)
void FUNC_timer_info(void)
{
  if(LCDML.FUNC_setup()) 
  {  /* **** SETUP **** */
    LCDML.DISP_menuClear();                                        
    LCDML.BT_resetAll();
    Serial.println(F("==========================================="));
    Serial.println(F("================  FUNC ===================="));
    Serial.println(F("==========================================="));
    Serial.println(F("wait 10 secounds or press back button"));     
    g_func_timer_info = 10;       // reset and set timer    
    LCDML.FUNC_setTrigger(100);  // starts a trigger event for the loop function every 100 millisecounds
  }


  if(LCDML.FUNC_loop()) 
  { // ****** LOOP *********
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function 
    
    // this function is called every 100 millisecounds
    
    // this timer checks every 1000 millisecounds if it is called
    if(LCDML.OTHER_timer_ms(g_timer_1, 1000)) {       
      g_func_timer_info--;                // increment the value every secound
      Serial.println(g_func_timer_info);  // print the time counter value     
    }
    
    // if you are using a screensaver, you will reset the waittime here
    //LCDML_BACK_dynamic_restartTimer(LCDML_BACKEND_screensaver);
    
    // this function can only be ended when quit button is pressed or the time is over
    // check if the function ends normaly
    if (g_func_timer_info <= 0)
    {
      // end function for callback
      LCDML.FUNC_goBackToMenu();  
    } 
  } 

  if(LCDML.FUNC_close()) 
  { // ****** STABLE END *********
    // this functions is ever called when a DISP function is quit
    // you can here reset some global vars or do nothing
  }  
}


void FUNC_back(void)
{
  // is called when it is triggert
  LCDML.DISP_menuClear();                                        
  LCDML.BT_resetAll();
  
  LCDML.MENU_goBack();            // go back
  LCDML.FUNC_goBackToMenu();      // LCDML_DISP_funcend calls the loop_end function
}




/*


// *********************************************************************
uint8_t g_button_value = 0; // button value counter (global variable)
void LCDML_DISP_setup(LCDML_FUNC_p2)
// *********************************************************************
{ 
  // setup function
  // print lcd content
  Serial.println(F("==========================================="));
  Serial.println(F("================  FUNC ===================="));
  Serial.println(F("==========================================="));
  Serial.println(F("press a or w"));  
  Serial.println(F("count: 0 of 3"));
  // Reset Button Value
  g_button_value = 0; 
}

void LCDML_DISP_loop(LCDML_FUNC_p2)
{
  // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
  // the quit button works in every DISP function without any checks; it starts the loop_end function 


  // if you are using a screensaver, you will reset the waittime here
  //LCDML_BACK_dynamic_restartTimer(LCDML_BACKEND_screensaver);

  
  if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
  {
    if (LCDML.BT_checkLeft() || LCDML.BT_checkUp()) // check if button left is pressed
    {
      LCDML.BT_resetLeft(); // reset the left button
      LCDML.BT_resetUp(); // reset the left button
      g_button_value++;
      
      // update lcd content
      Serial.print(F("count: "));
      Serial.print(g_button_value); //print change content
      Serial.println(F(" of 3"));       
    }    
  }
  
  // check if button count is three
  if (g_button_value >= 3) {
    // end function for callback
    LCDML_DISP_funcend();   
  } 
}

void LCDML_DISP_loop_end(LCDML_FUNC_p2) 
{
  // this functions is ever called when a DISP function is quit
  // you can here reset some global vars or do nothing
}




void LCDML_DISP_setup(LCDML_FUNC_p1_start) 
{
  // setup
  // is called only if it is started

  // starts a trigger event for the loop function every 100 millisecounds
  LCDML_DISP_triggerMenu(100);  
}

void LCDML_DISP_loop(LCDML_FUNC_p1_start)
{ 
  // loop
  // is called when it is triggert
  // - with LCDML_DISP_triggerMenu( millisecounds ) 
  // - with every button status change

  // if you are using a screensaver, you will reset the waittime here
  //LCDML_BACK_dynamic_restartTimer(LCDML_BACKEND_screensaver);

  // check if any button is presed (enter, up, down, left, right)
  if(LCDML.BT_checkAny()) {         
    LCDML_DISP_funcend();
  } 
}

void LCDML_DISP_loop_end(LCDML_FUNC_p1_start)
{
  // loop end
  // this functions is ever called when a DISP function is quit
  // you can here reset some global vars or do nothing  
}







void LCDML_DISP_setup(LCDML_FUNC_screensaver) 
{
  // setup
  // is called only if it is started
  Serial.println("Screensaver"); 

  // starts a trigger event for the loop function every 100 millisecounds
  LCDML_DISP_triggerMenu(100);  
}

void LCDML_DISP_loop(LCDML_FUNC_screensaver)
{ 
  // loop
  // is called when it is triggert
  // - with LCDML_DISP_triggerMenu( millisecounds ) 
  // - with every button status change

  // if you are using a screensaver, you will reset the waittime here
  //LCDML_BACK_dynamic_restartTimer(LCDML_BACKEND_screensaver);

  // check if any button is presed (enter, up, down, left, right)
  if(LCDML.BT_checkAny()) {         
    LCDML_DISP_funcend();
  } 
}

void LCDML_DISP_loop_end(LCDML_FUNC_screensaver)
{
  // loop end
  // this functions is ever called when a DISP function is quit
  // you can here reset some global vars or do nothing  
}




void LCDML_DISP_setup(LCDML_FUNC_initscreen) 
{
  // setup
  // is called only if it is started
  Serial.println("Initscreen");
  Serial.println("press any Button to leave this screen");
  // starts a trigger event for the loop function every 100 millisecounds
  LCDML_DISP_triggerMenu(100);  
}

void LCDML_DISP_loop(LCDML_FUNC_initscreen)
{ 
  // loop
  // is called when it is triggert
  // - with LCDML_DISP_triggerMenu( millisecounds ) 
  // - with every button status change
  Serial.println("looop");
  // if you are using a screensaver, you will reset the waittime here
  //LCDML_BACK_dynamic_restartTimer(LCDML_BACKEND_screensaver);

  // check if any button is presed (enter, up, down, left, right)
  if(LCDML.BT_checkAny()) {         
    LCDML_DISP_funcend();
    LCDML.goRoot();
  } 
}

void LCDML_DISP_loop_end(LCDML_FUNC_initscreen)
{
  // loop end
  // this functions is ever called when a DISP function is quit
  // you can here reset some global vars or do nothing 
}



*/



