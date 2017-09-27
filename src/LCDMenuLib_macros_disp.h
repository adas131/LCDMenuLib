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
 
#ifndef _LCDML_macros_disp_h
    #define _LCDML_macros_disp_h
    
    /* ------------------ 
     * Include Arduino IOS                                                     
     * ------------------
     */
    #include <Arduino.h>

    
    
    /* ------------------ 
     * DISP / MENU                                                      
     * ------------------
     */
    #define LCDML_0                         LCDML_Item
    // compatible to 2.3.x 
    #define LCDML_root                      LCDML_Item
    
    // function declaration
    #define LCDML_DISP_setup(name)          name ##_loop_setup(void)
    #define LCDML_DISP_loop(name)           name ##_loop(void)
    #define LCDML_DISP_loop_end(name)       name ##_loop_end(void)
    
    // function end check
    #define LCDML_DISP_funcend()            bitSet(LCDML.control, _LCDML_control_funcend)
    
    // get flobal language variable for name xyz 
    #define LCDML_DISP_lang(name)           g_LCDML_DISP_lang_ ## name ## _var
    
    // call repeat of function 
    #define LCDML_DISP_initFunction(N)      LCDML_DISP_func_repeat(N);
    
    #define LCDML_DISP_groupEnable(g)       bitSet(LCDML.group_en, g)
    #define LCDML_DISP_groupDisable(g)      bitClear(LCDML.group_en, g)
           
    
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
    
    
    #define LCDML_DISP_update_menu()\
        if (LCDML.getFunction() != _LCDML_NO_FUNC){\
            LCDML_BACK_startDirect(LCDML_BACKEND_menu);\
        } \
        if (!bitRead(LCDML.button,  _LCDML_button_quit)) {\
            bitClear(LCDML.button,  _LCDML_button_quit);\
            LCDML_lcd_menu_display(); \
        }
    
    
    
    
    #define LCDML_DISP_triggerMenu(time)\
        LCDML_BACK_dynamic_setLoopTime(LCDML_BACKEND_menu, time);\
        LCDML_BACK_startDirect(LCDML_BACKEND_menu)
    
    #define LCDML_DISP_triggerControl(time)\
        LCDML_BACK_dynamic_setLoopTime(LCDML_BACKEND_control, time);\
        LCDML_BACK_startDirect(LCDML_BACKEND_control)            
    
        
        
        
    #define LCDML_DISP_init()\
        typedef void(* LCDML_FuncPtr)();\
        LCDML_FuncPtr g_LCDML_DISP_functions_loop_setup[(_LCDML_DISP_cnt+1)];\
        LCDML_FuncPtr g_LCDML_DISP_functions_loop[(_LCDML_DISP_cnt+1)];\
        LCDML_FuncPtr g_LCDML_DISP_functions_loop_end[(_LCDML_DISP_cnt+1)];\
        void LCDML_FUNC_loop_setup(){}\
        void LCDML_FUNC_loop(){}\
        void LCDML_FUNC_loop_end(){}\
        unsigned long g_LCDML_DISP_press_time = 0;\
        LCDMenu LCDML_Item (0, true, 0);\
        void LCDML_lcd_menu_display(); \
        void LCDML_lcd_menu_clear(); \
        uint8_t g_lcdml_jump_func = _LCDML_NO_FUNC;\
      
 
    
    #ifndef _LCDML_ESP
        // stored in flash (arduino) 
        #define LCDML_LANG_DEF(name, content) \
            const char g_LCDML_DISP_lang_ ## name ##_var[] PROGMEM = {content}
            
        #define LCDML_DISP_getContent(var, id) \
            strcpy_P(var, (char*)pgm_read_word(&(g_LCDML_DISP_lang_table[LCDML.content_id[id]])));
                    
        #define LCDML_DISP_createMenu(N)\
            const char * const g_LCDML_DISP_lang_table[] PROGMEM = { LCDML_DISP_lang_repeat(N) }; LCDMenuLib LCDML(LCDML_Item, _LCDML_DISP_rows, _LCDML_DISP_cols);
          
        #define LCDML_DISP_getElementName(var, element_id) \
            if(element_id != _LCDML_NO_FUNC && (sizeof(g_LCDML_DISP_lang_table)-1) >= element_id) {\
                strcpy_P(var, (char*)pgm_read_word(&(g_LCDML_DISP_lang_table[element_id])));\
            }\
    
        #define LCDML_DISP_getElementNameCheck(var, element_id, check) \
            if(element_id != _LCDML_NO_FUNC && (sizeof(g_LCDML_DISP_lang_table)-1) >= element_id) {\
                strcpy_P(var, (char*)pgm_read_word(&(g_LCDML_DISP_lang_table[element_id])));\
                check = true;\
            } else {\
                check = false;\
            }  
          
    #else
        // stored in ram (esp)
        #define LCDML_LANG_DEF(name, content) \
            char g_LCDML_DISP_lang_ ## name ##_var[_LCDML_DISP_cfg_max_string_length] = {content}
                    
        #define LCDML_DISP_getContent(var, id) \
            strcpy(var, g_LCDML_DISP_lang_table[LCDML.content_id[id]])
                    
        #define LCDML_DISP_createMenu(N)\
            char * g_LCDML_DISP_lang_table[] = { LCDML_DISP_lang_repeat(N) }; LCDMenuLib LCDML(LCDML_Item, _LCDML_DISP_rows, _LCDML_DISP_cols);

        #define LCDML_DISP_getElementName(var, element_id) \
            if(element_id != _LCDML_NO_FUNC && (sizeof(g_LCDML_DISP_lang_table)-1) >= element_id) {\
                strcpy_P(var, (char*)(g_LCDML_DISP_lang_table[element_id]));\
            }\
    
        #define LCDML_DISP_getElementNameCheck(var, element_id, check) \
            if(element_id != _LCDML_NO_FUNC && (sizeof(g_LCDML_DISP_lang_table)-1) >= element_id) {\
                strcpy_P(var, (char*)(g_LCDML_DISP_lang_table[element_id]));\
                check = true;\
            } else {\
                check = false;\
            }                
            
    #endif
    
    
         
    //Menu Item Types
    #define LCDML_DISP_addMenu(id, group, parent, child, content) \
        LCDML_LANG_DEF(id, content); \
        LCDMenu parent ## _ ## child(id, group, 0); \
        void LCDML_DISP_ ## id ## _function() { \
            g_LCDML_DISP_functions_loop_setup[id]   = LCDML_FUNC_loop_setup; \
            g_LCDML_DISP_functions_loop[id]         = LCDML_FUNC_loop; \
            g_LCDML_DISP_functions_loop_end[id]     = LCDML_FUNC_loop_end; \
            parent.addChild(parent ## _ ## child); \
        }
    
    #define LCDML_DISP_addFunction(id, group, parent, child, content, callback) \
        LCDML_LANG_DEF(id, content); \
        LCDMenu parent ## _ ## child(id, group, 0); \
        void callback ## _loop_setup(); \
        void callback ## _loop(); \
        void callback ## _loop_end(); \
        void LCDML_DISP_ ## id ## _function() { \
            g_LCDML_DISP_functions_loop_setup[id]   = callback ## _loop_setup;\
            g_LCDML_DISP_functions_loop[id]         = callback ## _loop; \
            g_LCDML_DISP_functions_loop_end[id]     = callback ## _loop_end; \
            parent.addChild(parent ## _ ## child); \
        }
        
    #define LCDML_DISP_addFunctionParam(id, group, parent, child, content, callback, param) \
        LCDML_LANG_DEF(id, content); \
        LCDMenu parent ## _ ## child(id, group, (param & 0x0F)); \
        void callback ## _loop_setup(); \
        void callback ## _loop(); \
        void callback ## _loop_end(); \
        void LCDML_DISP_ ## id ## _function() { \
            g_LCDML_DISP_functions_loop_setup[id]   = callback ## _loop_setup;\
            g_LCDML_DISP_functions_loop[id]         = callback ## _loop; \
            g_LCDML_DISP_functions_loop_end[id]     = callback ## _loop_end; \
            parent.addChild(parent ## _ ## child); \
        }
        
    #define LCDML_DISP_addPlaceholder(id, group, parent, child) \
        LCDML_DISP_addMenu(id, group, parent, child, "")   
  
    
    #define LCDML_DISP_initSetup(N)\
        LCDML_DISP_initFunction(N); \
        LCDML.display();\
        LCDML_lcd_menu_display()
        
    
    #define LCDML_DISP_jumpToFunc(name)\
        for(uint8_t i=0; i<_LCDML_NO_FUNC;i++) {\
            if ( name##_loop_setup == g_LCDML_DISP_functions_loop_setup[i]) { \
                if(LCDML.getFunction() != _LCDML_NO_FUNC) {\
                    g_lcdml_jump_func = i;\
                } else {\
                    LCDML.jumpToElement(i);\
                    LCDML_DISP_update_menu(); \
                }\
                break;\
            }\
        }\
        LCDML_BUTTON_resetAll()
 
 
#endif