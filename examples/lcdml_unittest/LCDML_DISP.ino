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
  uint8_t n_max = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());
  
  // update content
  if (LCDML.DISP_chkMenuUpdate() || LCDML.DISP_chkMenuCursorUpdate() ) {
    // clear menu
    LCDML.DISP_clear();

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
            LCDML_getContent(content_text, n); 
            Serial.print(content_text);
            break;                
      }
      Serial.println();
    } 
  }
}







