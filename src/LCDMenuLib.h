/* ******************************************************************************                                                                        
 *                        LCDMenuLib (LCDML)                                                                                                       
 * ****************************************************************************** 
 *
 * MIT License
 * 
 * Copyright (c) [2017] [Nils Feldkämper]
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *                                      
 * ******************************************************************************
 *   
 * BUG / ISSUES REPORTING                                      
 *    https://github.com/Jomelo/LCDMenuLib/issues 
 * 
 * ARDUIONO FORUM                                                    
 *     http://forum.arduino.cc/index.php?topic=73816.0  
 *   
 * ****************************************************************************** 
 */

// File header
#ifndef LCDMenuLib_h
    #define LCDMenuLib_h

    // Version
    #define _LCDML_VERSION                       "LCDML v3.0.0 beta for ESP32"

    // Configuration 
    #define _LCDML_DISP_cfg_cursor_deep          6   // save the last position of the cursor until layer xx
    #define _LCDML_DISP_cfg_max_string_length    20  // max string length witch can be display
    #define _LCDML_DISP_cfg_max_rows             10  // max rows which are supported
    
   
    
    #define lcdml_dbg_println(str)\
        Serial.println(str)
        
    #define lcdml_dbg_print(str)\
        Serial.println(str)  
    
    
    

    // Include arduino ios 
    #include "Arduino.h"

    // Arduino specific settings
    #if ARDUINO >= 160
        #define _LCDMenuLib_arduino_version         1    // for new arduino version like 1.6.0 or higher
    #else
        #define _LCDMenuLib_arduino_version         0    // for old arduino version like 1.0.6 or 1.0.5
    #endif

    // ESP specific settings
    #if ( ESP8266 ) || defined ( ESP32 )
        #define _LCDML_ESP
    #endif
    
    // No function constante 
    #define _LCDML_NO_FUNC                      255

    // Bit pos control
    #define _LCDML_control_menu_back            7
    #define _LCDML_control_disp_update          6
    #define _LCDML_control_cursor_update        5
    //#define _LCDML_control_go_root              4
    #define _LCDML_control_update_direct        3
    #define _LCDML_control_search_display       2
    //#define _LCDML_control_funcend              1
    #define _LCDML_control_disable_hidden       0
    
    #define _LCDML_funcReg_f7                   7
    #define _LCDML_funcReg_f6                   6
    #define _LCDML_funcReg_f5                   5
    #define _LCDML_funcReg_f4                   4
    #define _LCDML_funcReg_setup                3
    //#define _LCDML_funcReg_loop                 2
    #define _LCDML_funcReg_end                  1
    #define _LCDML_funcReg_return               0
    
    // Bit pos funcmode
    #define _LCDML_func_setup                   7
    #define _LCDML_func_stable_end              6

    // Bit position groups
    #define _LCDML_HIDE                         7
    #define _LCDML_G7                           6
    #define _LCDML_G6                           5
    #define _LCDML_G5                           4
    #define _LCDML_G4                           3
    #define _LCDML_G3                           2
    #define _LCDML_G2                           1
    #define _LCDML_G1                           0

    
    typedef void(* LCDML_FuncPtr)();
    

    // Configure arduino flash lib and load it*/
    #ifndef __PROG_TYPES_COMPAT__
        #define __PROG_TYPES_COMPAT__
    #endif

    // Include PGMSPACE
    #ifndef _LCDML_ESP
        #include <avr/pgmspace.h>
    #endif
    
    
    #define LCDML_bitCreateVar(var, size) \
        uint8_t var[((size+1)/7)+1]
        
    #define LCDML_bitReadValue(var, bit) \
        bitRead(var[bit / 7], bit % 7)
        
    #define LCDML_bitWriteValue(var, bit, val) \
        bitWrite(var[bit / 7], bit % 7, val) 
    
    

    // Include menu class
    #include "lcdml_c_menuElement.h"
    
    #define _LCDML_button_free                  7
    #define _LCDML_button                       6
    #define _LCDML_button_quit                  5
    #define _LCDML_button_enter                 4
    #define _LCDML_button_up                    3
    #define _LCDML_button_down                  2
    #define _LCDML_button_left                  1
    #define _LCDML_button_right                 0
    
    
    #define _LCDML_reg1_f7               7
    #define _LCDML_reg1_f6                6
    #define _LCDML_reg1_f5              5
    #define _LCDML_reg1_f4                4
    #define _LCDML_reg1_f3               3
    #define _LCDML_reg1_f2               2
    #define _LCDML_reg1_f1               1
    #define _LCDML_reg1_func_active              0

    // Include macros for this lib 
    //#include "LCDMenuLib_macros_control.h"
    #include "LCDMenuLib_macros_disp.h"    
    #include "LCDMenuLib_macros_back.h"
    #include "LCDMenuLib_macros_messages.h"
    #include "LCDMenuLib_macros_recursive.h"
  

//# Lcd Menu Lib
//# =======================
    class LCDMenuLib
    {
        private:
            /* Menu Objects */
            lcdmlMenuElement *rootMenu;
            lcdmlMenuElement *curMenu;
            lcdmlMenuElement *activMenu;
            lcdmlMenuElement *tmp;
            
            LCDML_FuncPtr   callback_contentUpdate;     // Update Content            
            LCDML_FuncPtr   callback_contentClear;      // Clear Content            
            LCDML_FuncPtr   callback_functionTriggerBackend;    // Trigger Backend            
            
            
            unsigned long timer_control;
            
            /* */
            uint8_t reg1;
            /* display cols */
            uint8_t cols;
            /* display rows */
            uint8_t rows;
            /* save the last layer */
            uint8_t layer_save[_LCDML_DISP_cfg_cursor_deep];      // save cursor position for every layer        
            /* current corsor position */
            uint8_t curloc;            
            /* current scroll position */
            uint8_t scroll;
            /* save content ids that are displayed currently */
            uint8_t content_id[_LCDML_DISP_cfg_max_rows+1];   
            /* save the last cursor position when a menue element is called */
            uint8_t cursor_pos;
            /* how many childs exists on next layer */
            uint8_t child_cnt;
            /* containes the current layer */
            uint8_t layer;
            /* save last parent */
            uint8_t parents[_LCDML_DISP_cfg_cursor_deep];           
            
            void    MENU_goEnter();                                                         /* activate the menu under the cursor */            
            void    MENU_setCursor();                                                       /* set the cursor to the current position in the menu */            
            void    MENU_doScroll();                                                        /* scroll the menu */            
            void    MENU_goMenu();                                       /* go to a menu element */               
            uint8_t MENU_selectElementDirect(lcdmlMenuElement &p_m, uint8_t p_search);      /* works with jump to element on globale function */            
            uint8_t MENU_countChilds();                                                     /* how many childs exists on next layer */            
            uint8_t MENU_curlocCorrection();                                                /* correction of the cursor position with hidden button */
          

            unsigned long menu_timer;
            unsigned long menu_default_time;
          
          
        public:  
               
            /* Constructor */
            LCDMenuLib(lcdmlMenuElement &p_r, const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr triggerBackend);
              

            void loop();

              
            // diese variablen privat machen 
            //todo
            uint8_t func;
            /* button variable */
            uint8_t button;
            /* control bits */
            uint8_t control;
            /* menumode */
            uint8_t menumode; 
            
            uint8_t funcReg;            
            
            
           
            void    MENU_display(uint8_t update=0);     /* Display the current menu on the lcd */            
            void    MENU_goRoot();                      /* jump to root menu */            
            void    MENU_goBack();                      /* move to the parent menu */ 
            void    MENU_jumpToElement(uint8_t element);/* jump to menu element */ 
            void    MENU_groupShow(uint8_t g);
            void    MENU_groupHide(uint8_t g); 
            uint8_t MENU_getLayer();                    /* get the layer where the cursor stands in the menu */
            uint8_t MENU_getCursorPos();                /* get the corrent cursor position */
            uint8_t MENU_getCursorPosAbs();             /* get the corrent cursor position */             
            uint8_t MENU_getChilds();                   /* get the number of childs from a parent element */           
            uint8_t MENU_getParentId();                 /* get parent */            
            uint8_t MENU_getParentId(uint8_t p_layer);  /* get parent id in a specific layer */

            
            void    BT_enter();
            void    BT_up();
            void    BT_down();
            void    BT_left();
            void    BT_right();
            void    BT_quit();
            
            void    BT_resetAll();
            void    BT_resetEnter();
            void    BT_resetUp();
            void    BT_resetDown();
            void    BT_resetLeft();
            void    BT_resetRight();                
            
            uint8_t BT_chkAny();
            uint8_t BT_chkEnter();
            uint8_t BT_chkUp();
            uint8_t BT_chkDown();
            uint8_t BT_chkLeft();
            uint8_t BT_chkRight();
            
  
            

            void    DISP_menuUpdate();  // rename to DISP_update();
            void    DISP_menuClear();   // rename to DISP_clear();
            uint8_t DISP_chkMenuUpdate();
            uint8_t DISP_chkMenuCursorUpdate();            
            uint8_t DISP_getMenuContentId(uint8_t n);
            
            void    FUNC_call();            
            uint8_t FUNC_setup();
            uint8_t FUNC_loop();
            uint8_t FUNC_close();            
            void    FUNC_goBackToMenu();                       
            uint8_t FUNC_getFunction();         /* get active function id */             
            uint8_t FUNC_getFunctionParam();    /* get parameter */
            void    FUNC_setTrigger(unsigned long t);
            
            
            
            void    FUNC_triggerBackend();
            
            boolean OTHER_timer_ms(unsigned long &var, unsigned long t);
            boolean OTHER_timer_us(unsigned long &var, unsigned long t);
            void    OTHER_goToFunc(uint8_t id);
            void    OTHER_goToMenu(uint8_t id);
            
    };
#endif


