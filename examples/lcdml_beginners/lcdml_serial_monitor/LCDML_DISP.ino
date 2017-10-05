// =====================================================================
//
// Output function
//
// =====================================================================

//Hier weitermachen 
//Irgenwie einen Callback für das Backend einbinden um diesen Check zu vereinfachen und direkt aus der klasse zu beeinflussen. 

void LCDML_lcd_trigger_backend()
{
  LCDML_BACK_startDirect(LCDML_BACKEND_menu);  
}


/* ******************************************************************** */
void LCDML_lcd_menu_display()
/* ******************************************************************** */
{

  
  /*
  if (bitRead(LCDML.button,  _LCDML_button_quit)) {
    return;   
  } 
  
  bitClear(LCDML.button,  _LCDML_button_quit);
  */ 

  
  
  // save the content text of every menu element
  // _LCDML_DISP_cfg_max_string_length = 20 (default), can be changed in LCDMenuLib.h
  char content_text[_LCDML_DISP_cfg_max_string_length]; 


  // if you are using a screensaver, you will reset the waittime here
  //LCDML_BACK_restart(LCDML_BACKEND_screensaver);  
  // init vars
  uint8_t n_max = (LCDML.getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.getChilds());
  
  // update content
  if (LCDML_DISP_update_content() || LCDML_DISP_update_cursor()) {
    // clear menu
    LCDML_lcd_menu_clear();    

    Serial.println(F("==========================================="));
    Serial.println(F("================  Menu ===================="));
    Serial.println(F("==========================================="));
    // display rows
    for (uint8_t n = 0; n < n_max; n++)
    {
      
      //set cursor char
      if (n == LCDML.getCursorPos()) {
        Serial.print(F("(x) "));          
      } else {
        Serial.print(F("( ) "));
      }                
      // print content
      // with content id you can add special content to your static menu or replace the content
      // the content_id contains the id wich is set on main tab for a menuitem
      switch(LCDML.getContentId(n))
      {         
          //case 0:
          //    Serial.print("special"); // or datetime or other things
          //    break;         

          case 3: 
            Serial.print(F("Placeholder 1"));
            break;

          case 4:
            Serial.print(F("Placeholder 2"));
            break;
      
          default: // static content
            //Serial.println(n);
            LCDML_DISP_getContent(content_text, n); 
            Serial.print(content_text);
            break;                
      }
      Serial.println();
    } 
  }
}


// lcd clear
void LCDML_lcd_menu_clear()
{
  for(uint8_t i=0;i<15;i++) {
    Serial.println();
  }
}



