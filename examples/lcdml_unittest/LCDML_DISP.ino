// =====================================================================
//
// Output function
//
// =====================================================================

// this function is for debugging and only called if it is enabled
// enable it with LCDML.MENU_enableStatusCode();
//void lcdml_menu_statusCodes()
//{  
//  Serial.println(LCDML.getStatusCode()); // get status code and reset it
//}

/* ******************************************************************** */
void lcdml_menu_clear()
/* ******************************************************************** */
{
  for(uint8_t i=0;i<15;i++) {
    Serial.println();
  }
}

/* ******************************************************************** */
void lcdml_menu_display()
/* ******************************************************************** */
{
  // save the content text of every menu element
  char content_text[_LCDML_DISP_cfg_max_string_length];  // can be changed in main tab
  
  // init vars
  //uint8_t n_max = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());
  
  // update content
  if (LCDML.DISP_checkMenuUpdate() || LCDML.DISP_checkMenuCursorUpdate() ) {
    // clear menu
    LCDML.DISP_clear();

    Serial.println(F("==========================================="));
    Serial.println(F("================  Menu ===================="));
    Serial.println(F("==========================================="));
   
    LCDMenuLib_menu *tmp;    

    uint8_t i = LCDML.MENU_getScroll();
    uint8_t maxi = _LCDML_DISP_rows + i;
    uint8_t n = 0;
    
    //check children
    if (tmp = LCDML.MENU_getObj()->getChild(LCDML.MENU_getScroll())) 
    {        
      do
      {            
        // check if a menu element has a condetion and if the condetion be true               
        if (tmp->checkCondetion()) 
        {
          if(tmp->getParam() != _LCDML_TYPE_dynParam || tmp->checkCallback() == false)
          {
            if (n == LCDML.MENU_getCursorPos()) 
            {
              Serial.print(F("(x) "));          
            } 
            else 
            {
              Serial.print(F("( ) "));
            } 

            // display normal content
            LCDML_getContent(content_text, tmp->getID()); 
            Serial.print(content_text);
            Serial.println();
          }            
          else 
          {            
            tmp->callback(n);              
          }                  
           
          i++; 
          n++;             
        }            
      } while (((tmp = tmp->getSibling(1)) != NULL) && (i < maxi));
    }
  }
}



/* ===================================================================== *
 *                                                                       *
 * Dynamic content                                                       *
 *                                                                       *
 * ===================================================================== *
 */


// *********************************************************************
uint8_t g_dynParam = 100; // when this value comes from an eeprom, load it in setup 
                          // at the moment here is no setup function (todo) 
void cFunc_s1(uint8_t n)
// *********************************************************************
{ 
  if (n == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));

    // make only an action when the cursor stands on this menuitem
    //check Button
    if(LCDML.BT_checkAny()) {
      if(LCDML.BT_checkEnter()) {
        // dosomething for example save the data or something else
        Serial.println("Save data");     
        LCDML.BT_resetEnter();
      }
      if(LCDML.BT_checkLeft()) {
        g_dynParam++;
        LCDML.BT_resetLeft();
      }
      if(LCDML.BT_checkRight()) {
        g_dynParam--;
        LCDML.BT_resetRight();
      }
    }

              
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

 

  // display normal content    
  Serial.print(F("change 'a' or 'd': "));
  Serial.println(g_dynParam);
  
}

// *********************************************************************
void cFunc_s2(uint8_t n)
// *********************************************************************
{  
  if (n == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("dummy 2"));  
}

// *********************************************************************
void cFunc_s3(uint8_t n)
// *********************************************************************
{  
  if (n == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("dummy 3"));    
}

// *********************************************************************
void cFunc_s4(uint8_t n)
// *********************************************************************
{
 
  if (n == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("dummy 4"));   
}

// *********************************************************************
void cFunc_s5(uint8_t n)
// *********************************************************************
{  
  if (n == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("hidden dummy 5"));    
}


// *********************************************************************
void cFunc_s6(uint8_t n)
// *********************************************************************
{  
  if (n == LCDML.MENU_getCursorPos()) 
  {
    Serial.print(F("(x) "));          
  } 
  else 
  {
    Serial.print(F("( ) "));
  } 

  // display normal content    
  Serial.println(F("dummy 6"));    
}









