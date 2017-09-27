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
    #define _LCDML_VERSION                       "LCDML v2.4.0 beta for ESP32"

    // Debug Settings
    #define _LCDML_DBG                           0
    #define _LCDML_DBG_DISP                      0
    #define _LCDML_DBG_BACK                      0

    // Configuration 
    #define _LCDML_DISP_cfg_cursor_deep          6   // save the last position of the cursor until layer xx
    #define _LCDML_DISP_cfg_max_string_length    20  // max string length witch can be display
    #define _LCDML_DISP_cfg_max_rows             10  // max rows which are supported

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

    // Bit pos buttons
    #define _LCDML_button_free                  7
    #define _LCDML_button                       6
    #define _LCDML_button_quit                  5
    #define _LCDML_button_enter                 4
    #define _LCDML_button_up                    3
    #define _LCDML_button_down                  2
    #define _LCDML_button_left                  1
    #define _LCDML_button_right                 0
                                                
    // Bit pos control
    #define _LCDML_control_menu_back            7
    #define _LCDML_control_disp_update          6
    #define _LCDML_control_cursor_update        5
    #define _LCDML_control_go_root              4
    #define _LCDML_control_update_direct        3
    #define _LCDML_control_search_display       2
    #define _LCDML_control_funcend              1
    #define _LCDML_control_disable_hidden       0
    
    // Bit pos funcmode
    #define _LCDML_menumode_rollover            7   

    // Bit position groups
    #define _LCDML_G8                           7
    #define _LCDML_G7                           6
    #define _LCDML_G6                           5
    #define _LCDML_G5                           4
    #define _LCDML_G4                           3
    #define _LCDML_G3                           2
    #define _LCDML_G2                           1
    #define _LCDML_G1                           0

    

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
    #include "LCDMenuLib_menu.h"
    
    
    #define _LCDML_itemcfg_free2               7
    #define _LCDML_itemcfg_free                6
    #define _LCDML_itemcfg_switch              5
    #define _LCDML_itemcfg_func                4
    #define _LCDML_itemcfg_para3               3
    #define _LCDML_itemcfg_para2               2
    #define _LCDML_itemcfg_para1               1
    #define _LCDML_itemcfg_para0               0

    // Include macros for this lib 
    #include "LCDMenuLib_macros_control.h"
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
            LCDMenu *rootMenu;
            LCDMenu *curMenu;    
            /* Saves the string position from menu elments in flash memory */
            const char * const *flash_table;
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
            /* save the last cursor position when a menue element is called */
            uint8_t cursor_pos;
            /* how many childs exists on next layer */
            uint8_t child_cnt;
            /* containes the current layer */
            uint8_t layer;
            /* save last parent */
            uint8_t parents[_LCDML_DISP_cfg_cursor_deep];
            /* correction of the cursor position with hidden button */
            uint8_t curloc_correction();
            /* activate the menu under the cursor */
            void    goEnter();
            /* set the cursor to the current position in the menu */
            void    setCursor();
            /* scroll the menu */
            void    doScroll();
            /* go to a menu element */
            void    goMenu(LCDMenu &m);    
            /* works with jump to element on globale function */
            uint8_t selectElementDirect(LCDMenu &p_m, uint8_t p_search);
            /* how many childs exists on next layer */
            uint8_t countChilds();
        

        public:
            /* button variable */
            uint8_t button;
            /* control bits */
            uint8_t control;
            /* menumode */
            uint8_t menumode; 
            /* save group_hidden_status */
            uint8_t group_en;
            /* save the last id from a menu element, when a menu elmend is called */
            uint8_t function;                
            /* save content ids that are displayed currently */
            uint8_t content_id[_LCDML_DISP_cfg_max_rows];        
            /* Constructor */
            LCDMenuLib(LCDMenu &p_r, const uint8_t p_rows, const uint8_t p_cols);
            /* Display the current menu on the lcd */
            void    display();
            /* jump to root menu */
            void    goRoot();
            /* move to the parent menu */
            void    goBack();
            /* update display content */
            void    display_update();
            /* clear display content */
            void    display_clear();
            /* jump to menu element */
            void    jumpToElement(uint8_t element);        
            /* go in a menu element */
            void    Button_enter();
            /* go out of a menu element */
            void    Button_quit();
            /* navigate through the menu */
            void    Button_udlr(uint8_t but);
            /* get active function id */
            uint8_t getFunction();
            /* get the layer where the cursor stands in the menu */
            uint8_t getLayer();
            /* get the corrent cursor position */
            uint8_t getCursorPos();    
            /* get the corrent cursor position */
            uint8_t getCursorPosAbs();    
            /* get the number of childs from a parent element */
            uint8_t getChilds();
            /* get parent */
            uint8_t getParentId();
            /* get parent id in a specific layer */
            uint8_t getParentId(uint8_t p_layer);
    };
#endif


