// =====================================================================
//
// Output function
//
// =====================================================================




/* ******************************************************************** */
void menu_display()
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


  
  // init vars
  uint8_t n_max = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());
  
  // update content
  if (LCDML.DISP_chkMenuUpdate() || LCDML.DISP_chkMenuCursorUpdate() ) {
    // clear menu
    LCDML.DISP_menuClear();

    Serial.println(F("==========================================="));
    Serial.println(F("================  Menu ===================="));
    Serial.println(F("==========================================="));
    // display rows
    for (uint8_t n = 0; n < n_max; n++)
    {      
      //set cursor char
      if (n == LCDML.MENU_getCursorPos()) {
        Serial.print(F("(x) "));          
      } else {
        Serial.print(F("( ) "));
      }                
      // print content
      // with content id you can add special content to your static menu or replace the content
      // the content_id contains the id wich is set on main tab for a menuitem
      switch(LCDML.DISP_getMenuContentId(n))
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
void menu_clear()
{
  for(uint8_t i=0;i<15;i++) {
    Serial.println();
  }
}


//Hier weitermachen 
//Irgenwie einen Callback für das Backend einbinden um diesen Check zu vereinfachen und direkt aus der klasse zu beeinflussen. 

void trigger_backend()
{
  LCDML_BACK_startDirect(LCDML_BACKEND_menu);  
}



