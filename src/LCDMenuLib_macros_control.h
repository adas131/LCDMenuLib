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

#ifndef _LCDML_macros_control_h
    #define _LCDML_macros_control_h

    /* ------------------ 
     * Include Arduino IOS                                                     
     * ------------------
     */
    #include <Arduino.h>

    /* ------------------ 
     * UPDATE DISPLAY                                                     
     * ------------------
     */
    #define LCDML_DISP_update()             (LCDML.getFunction() == _LCDML_NO_FUNC || bitRead(LCDML.control, _LCDML_control_funcend) || bitRead(LCDML.control, _LCDML_control_update_direct))
    #define LCDML_DISP_update_content()     (bitRead(LCDML.control, _LCDML_control_disp_update) || bitRead(LCDML.control, _LCDML_control_update_direct))
    #define LCDML_DISP_update_cursor()      (bitRead(LCDML.control, _LCDML_control_cursor_update))
    
    #define LCDML_DISP_update_end() \
        bitClear(LCDML.control, _LCDML_control_disp_update); \
        bitClear(LCDML.control, _LCDML_control_cursor_update); \
        bitClear(LCDML.control, _LCDML_control_update_direct)
    
    #define LCDML_DISP_update_menu_direct(id) \
        if(LCDML.getFunction() == _LCDML_NO_FUNC) { \
            for(int i=0; i < ((LCDML.getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.getChilds())); i++) { \
                if(LCDML.content_id[i] == id) { \
                    bitSet(LCDML.control, _LCDML_control_update_direct); \
                    LCDML_lcd_menu_display(); \
                    break; \
                } \
            } \
        } 

    /* ------------------ 
     * CONTROL                                                      
     * ------------------
     */
    #define LCDML_BUTTON_enter()        LCDML.Button_enter();                   LCDML_DISP_update_menu()
    #define LCDML_BUTTON_up()           LCDML.Button_udlr(_LCDML_button_up);    LCDML_DISP_update_menu()
    #define LCDML_BUTTON_down()         LCDML.Button_udlr(_LCDML_button_down);  LCDML_DISP_update_menu()
    #define LCDML_BUTTON_left()         LCDML.Button_udlr(_LCDML_button_left);  LCDML_DISP_update_menu()
    #define LCDML_BUTTON_right()        LCDML.Button_udlr(_LCDML_button_right); LCDML_DISP_update_menu()
    #define LCDML_BUTTON_quit()         LCDML.Button_quit();                    LCDML_DISP_update_menu()

    #define LCDML_BUTTON_checkAny()     ((LCDML.button > 0) ? true : false)        
    #define LCDML_BUTTON_checkEnter()   bitRead(LCDML.button, _LCDML_button_enter)        
    #define LCDML_BUTTON_checkUp()      bitRead(LCDML.button, _LCDML_button_up)        
    #define LCDML_BUTTON_checkDown()    bitRead(LCDML.button, _LCDML_button_down)
    #define LCDML_BUTTON_checkLeft()    bitRead(LCDML.button, _LCDML_button_left)
    #define LCDML_BUTTON_checkRight()   bitRead(LCDML.button, _LCDML_button_right)

    #define LCDML_BUTTON_resetAll()     LCDML.button = 0
    #define LCDML_BUTTON_resetEnter()   bitClear(LCDML.button, _LCDML_button_enter)
    #define LCDML_BUTTON_resetUp()      bitClear(LCDML.button, _LCDML_button_up)
    #define LCDML_BUTTON_resetDown()    bitClear(LCDML.button, _LCDML_button_down)
    #define LCDML_BUTTON_resetLeft()    bitClear(LCDML.button, _LCDML_button_left)
    #define LCDML_BUTTON_resetRight()   bitClear(LCDML.button, _LCDML_button_right) 
        

#endif



