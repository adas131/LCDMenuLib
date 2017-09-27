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

#ifndef _LCDML_macros_messages_h
    #define _LCDML_macros_messages_h
    
    /* ------------------ 
     * Include Arduino                                                    
     * ------------------
     */
    #include <Arduino.h>
    
    /* ------------------ 
     * small message system                                                     
     * ------------------
     */   
    // 
    #define LCDML_MSG(id, name)    uint8_t  g_lcdml_msg_id__##name  = id    
    // creates the 
    #define LCDML_MSG_init(cnt)    uint8_t g_lcdml_msg_status[(cnt/7)+1]
    // 
    #define LCDML_MSG_set(name)    bitSet(g_lcdml_msg_status[g_lcdml_msg_id__##name/7], g_lcdml_msg_id__##name%7)
    //    
    #define LCDML_MSG_get(name)    bitRead(g_lcdml_msg_status[g_lcdml_msg_id__##name/7], g_lcdml_msg_id__##name%7)
    // 
    #define LCDML_MSG_clear(name)  bitClear(g_lcdml_msg_status[g_lcdml_msg_id__##name/7], g_lcdml_msg_id__##name%7)
    
#endif