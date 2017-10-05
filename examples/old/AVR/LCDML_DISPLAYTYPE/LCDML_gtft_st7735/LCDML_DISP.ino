// =====================================================================
//
// Output function
//
// =====================================================================

/* ******************************************************************** */
void LCDML_lcd_menu_display()
/* ******************************************************************** */
{
    // save the content text of every menu element
    // _LCDML_DISP_cfg_max_string_length = 20 (default), can be changed in LCDMenuLib.h
    char content_text[_LCDML_DISP_cfg_max_string_length];

    // clear lcd
    gfx.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    // set text color / Textfarbe setzen
    gfx.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
    // set text size / Textgroesse setzen
    gfx.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
  
  
    if (LCDML_DISP_update()) {

        // init vars
        uint8_t n_max = (LCDML.getChilds() >= _LCDML_ADAFRUIT_rows) ? ((_LCDML_ADAFRUIT_rows > _LCDML_ADAFRUIT_rows_max) ? _LCDML_ADAFRUIT_rows : _LCDML_ADAFRUIT_rows_max) : (LCDML.getChilds());
    
        // display rows and cursor
        for (uint8_t n = 0; n < n_max; n++)
        {
        // set cursor
        if (n == LCDML.getCursorPos()) {
            gfx.setCursor(0, _LCDML_ADAFRUIT_FONT_H * (n));
            gfx.println(">");        
        }
      
      

        // set content
        // with content id you can add special content to your static menu or replace the content
        // the content_id contains the id wich is set on main tab for a menuitem
        switch(LCDML.content_id[n])
        {         
            //case 0: // dynamic content            
            //    gfx.setCursor(0, _LCDML_ADAFRUIT_FONT_H * (n));
            //    gfx.println("var_datetime");
            //    break;
        
            default: // static content
                gfx.setCursor(0, _LCDML_ADAFRUIT_FONT_H * (n));
                gfx.println(LCDML.content[n]);
                
                #if (defined ( ESP8266 ) || defined (_LCDML_DISP_cfg_enable_use_ram_mode )) 
                    gfx.println(LCDML_DISP_getRamContent(n));              
                #else
                    LCDML_DISP_copyFlashContent(content_text, n);              
                    gfx.println(content_text);
                #endif
                
                
                break;                
        }
              
    }

    // display scrollbar when more content as rows available 
    if (LCDML.getChilds() > n_max) {

      // set frame for scrollbar
      gfx.drawRect(_LCDML_ADAFRUIT_lcd_w - _LCDML_ADAFRUIT_scrollbar_w, 0,_LCDML_ADAFRUIT_scrollbar_w,_LCDML_ADAFRUIT_lcd_h, _LCDML_ADAFRUIT_TEXT_COLOR); 
      
      // calculate scrollbar length
      uint8_t scrollbar_block_length = LCDML.getChilds() - n_max;
      scrollbar_block_length = _LCDML_ADAFRUIT_lcd_h / (scrollbar_block_length + _LCDML_DISP_rows);

      //set scrollbar
      // hier muss du mal schauen wie du blöcke einfügen kannst, bei der u8glib geht das wie unten angezeigt, bei deiner lib weis ich das nicht
      if (LCDML.getCursorPosAbs() == 0) {                                   // top position     (min)
        //u8g.drawBox(_LCDML_ADAFRUIT_lcd_w - (_LCDML_ADAFRUIT_scrollbar_w-1), 1                                                    , (_LCDML_ADAFRUIT_scrollbar_w-2)  , scrollbar_block_length);
      } 
      else if (LCDML.getCursorPosAbs() == (LCDML.getChilds())) {            // bottom position  (max)         
        //u8g.drawBox(_LCDML_ADAFRUIT_lcd_w - (_LCDML_ADAFRUIT_scrollbar_w-1), _LCDML_ADAFRUIT_lcd_h - scrollbar_block_length            , (_LCDML_ADAFRUIT_scrollbar_w-2)  , scrollbar_block_length);
      } 
      else {                                                                // between top and bottom
        //u8g.drawBox(_LCDML_ADAFRUIT_lcd_w - (_LCDML_ADAFRUIT_scrollbar_w-1), (scrollbar_block_length * LCDML.getCursorPosAbs() + 1),(_LCDML_ADAFRUIT_scrollbar_w-2)  , scrollbar_block_length);
      }
    }
    
  }
  
  

  // reinit some vars
  LCDML_DISP_update_end();
}

// lcd clear
void LCDML_lcd_menu_clear()
{
}
