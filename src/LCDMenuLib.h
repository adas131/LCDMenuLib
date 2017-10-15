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
    #define _LCDML_VERSION                       "LCDML v3.0.0 beta 1"

    // Configuration 
    #define _LCDML_DISP_cfg_cursor_deep          6   // save the last position of the cursor until layer xx
    #define _LCDML_DISP_cfg_max_rows             10  // max rows which are supported
    #define _LCDML_cfg_use_ram                   0   // set this to 1 when you will use the ram mode
    
    // Include arduino ios 
    #include "Arduino.h"

    // Arduino specific settings
    #if ARDUINO >= 160
        #define _LCDMenuLib_arduino_version         1    // for new arduino version like 1.6.0 or higher
    #else
        #define _LCDMenuLib_arduino_version         0    // for old arduino version like 1.0.6 or 1.0.5
    #endif

    // ESP specific settings
    #if ( ESP8266 ) || defined ( ESP32 ) || (_LCDML_cfg_use_ram == 1)
        #define _LCDML_ESP
    #endif
    
    // No function constante 
    #define _LCDML_NO_FUNC                      255
    
    //0-200 = parameter
    #define _LCDML_TYPE_dynParam                201
    
    
    // Bit pos control
    #define _LCDML_control_menu_back            7
    #define _LCDML_control_disp_update          6
    #define _LCDML_control_cursor_update        5
    #define _LCDML_control_setup                4
    #define _LCDML_control_update_direct        3
    #define _LCDML_control_search_display       2
    #define _LCDML_control_rollover             1
    #define _LCDML_control_disable_hidden       0
    
    #define _LCDML_funcReg_disable_screensaver  7
    #define _LCDML_funcReg_f6                   6
    #define _LCDML_funcReg_f5                   5
    #define _LCDML_funcReg_f4                   4
    #define _LCDML_funcReg_setup                3
    //#define _LCDML_funcReg_loop                 2
    #define _LCDML_funcReg_end                  1
    #define _LCDML_funcReg_return               0
    
    

    

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
    #include "LCDMenuLib_typedef.h"
    #include "LCDMenuLib_menu.h"
    
    #define _LCDML_button_free                  7
    #define _LCDML_button                       6
    #define _LCDML_button_quit                  5
    #define _LCDML_button_enter                 4
    #define _LCDML_button_up                    3
    #define _LCDML_button_down                  2
    #define _LCDML_button_left                  1
    #define _LCDML_button_right                 0
    
    
    #define _LCDML_reg1_f7                      7
    #define _LCDML_reg1_f6                      6
    #define _LCDML_reg1_f5                      5
    #define _LCDML_reg1_f4                      4
    #define _LCDML_reg1_f3                      3
    #define _LCDML_reg1_f2                      2
    #define _LCDML_reg1_f1                      1
    #define _LCDML_reg1_func_active             0

    // Include macros for this lib
    #include "LCDMenuLib_macros.h"  
    
  

//# Lcd Menu Lib
//# =======================
    class LCDMenuLib
    {
        private:
            // object pointer
            LCDMenuLib_menu *rootMenu;
            LCDMenuLib_menu *curMenu;
            LCDMenuLib_menu *activMenu;            
            
            // callback functions
            LCDML_FuncPtr   callback_menuControl;
            LCDML_FuncPtr   callback_contentUpdate;     // Update Content            
            LCDML_FuncPtr   callback_contentClear;      // Clear Content 
            LCDML_FuncPtr2  cb_screensaver;
        
            // private variables            
            uint8_t reg1;                                    // */                                      
            uint8_t cols;                                    // display cols 
            uint8_t rows;                                    // display rows 
            uint8_t layer_save[_LCDML_DISP_cfg_cursor_deep]; // save cursor position for every layer        
            uint8_t curloc;                                  // current corsor position          
            uint8_t scroll;                                  // current scroll position 
            uint8_t scroll_save[_LCDML_DISP_cfg_cursor_deep];// save cursor position for every layer 
            uint8_t cursor_pos;                              // save the last cursor position when a menue element is called 
            uint8_t child_cnt;                               // how many childs exists on next layer 
            uint8_t layer;                                   // containes the current layer 
            uint8_t parents[_LCDML_DISP_cfg_cursor_deep];    // save last parent 
            uint8_t button;                                  // button variable 
            uint8_t control;                                 // control bits 
            uint8_t funcReg;                                 // control bits            
            uint8_t statusCode;                              // containts an error code which can help to find problems

            unsigned long menu_timer;
            unsigned long menu_default_time;
            
            unsigned long screensaver_timer;
            unsigned long screensaver_default_time;
            
            // private methods
                                                             
            void    MENU_goInto();                                                    // activate the menu under the cursor     
            void    MENU_goBack();                                                    // move to the parent menu
            void    MENU_setCursor();                                                 // set the cursor to the current position in the menu 
            void    MENU_doScroll();                                                  // scroll the menu             
            void    MENU_goMenu(LCDMenuLib_menu &m);                                  // go to a menu element               
            boolean MENU_selectElementDirect(LCDMenuLib_menu &p_m, uint8_t p_search); // works with jump to element on globale function            
            boolean MENU_selectElementDirect2(LCDMenuLib_menu &p_m, LCDML_FuncPtr2 p_search);
            uint8_t MENU_countChilds();                                               // how many childs exists on next layer             
            uint8_t MENU_curlocCorrection();                                          // correction of the cursor position with hidden button
            void    BT_control();
            void    FUNC_call(); 
                      
        public:  
               
            // constructor
            LCDMenuLib(LCDMenuLib_menu &p_r, const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr menuControl);
              
            // public methods
            void loop();
            
                        
            LCDMenuLib_menu * MENU_getObj();
            void    MENU_display(uint8_t update=0);     // Display the current menu on the lcd            
            void    MENU_goRoot();                      // jump to root menu            
            uint8_t MENU_getLayer();                    // get the layer where the cursor stands in the menu
            uint8_t MENU_getCursorPos();                // get the corrent cursor position
            uint8_t MENU_getCursorPosAbs();             // get the corrent cursor position            
            uint8_t MENU_getChilds();                   // get the number of childs from a parent element
            uint8_t MENU_getParentId();                 // get parent
            uint8_t MENU_getParentId(uint8_t p_layer);  // get parent id in a specific layer
            uint8_t MENU_getScroll(void);
            void    MENU_enRollover(void);
            void    MENU_disRollover(void);
            
            
            boolean BT_setup();                         // check if setup was run 
                                                        
            void    BT_enter();                         // press button enter
            void    BT_up();                            // press button up
            void    BT_down();                          // press button down 
            void    BT_left();                          // press button left
            void    BT_right();                         // press button right
            void    BT_quit();                          // press button quit
                                                   
            void    BT_resetAll();                      // reset all buttons
            void    BT_resetEnter();                    // reset enter button 
            void    BT_resetUp();                       // reset up button 
            void    BT_resetDown();                     // reset down button 
            void    BT_resetLeft();                     // reset left button 
            void    BT_resetRight();                    // reset right button 
                                                       
            boolean BT_checkAny();                       // get state of all buttons
            boolean BT_checkEnter();                     // get 
            boolean BT_checkUp();                        // 
            boolean BT_checkDown();                      //
            boolean BT_checkLeft();                      //
            boolean BT_checkRight();                     //
                                                   
            void    DISP_menuUpdate();                      //
            void    DISP_clear();                           //
            boolean DISP_checkMenuUpdate();                 //
            boolean DISP_checkMenuCursorUpdate();           //   
            uint8_t DISP_getMenuContentId(uint8_t n);       //         
                                                    
            boolean FUNC_setup();                           //
            boolean FUNC_loop();                            //
            boolean FUNC_stableEnd();                       //
            void    FUNC_goBackToMenu(uint8_t e=0);         //       
            uint8_t FUNC_getID();                           // get active id              
            void    FUNC_setLoopInterval(unsigned long t);
            void    FUNC_disableScreensaver();
                        
            boolean TIMER_ms(unsigned long &var, unsigned long t);  // 
            void    TIMER_msReset(unsigned long &var);              // 
            boolean TIMER_us(unsigned long &var, unsigned long t);  // 
            void    TIMER_usReset(unsigned long &var);              // 
            
            boolean OTHER_jumpToFunc(uint8_t id);                     //
            boolean OTHER_jumpToFunc(LCDML_FuncPtr2 p_search);        //

            void    SCREEN_enable(LCDML_FuncPtr2 function, unsigned long t);    //
            void    SCREEN_disable();                                           //
            void    SCREEN_resetTimer();                                        //
            
            boolean DP_check(uint8_t n);                                        //
    };
#endif


