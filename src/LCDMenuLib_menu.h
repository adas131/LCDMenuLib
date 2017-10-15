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

#ifndef LCDMenuLib_menu_h
#   define LCDMenuLib_menu_h

#   include "Arduino.h"
#   include "LCDMenuLib_typedef.h"

    
 
    class LCDMenuLib_menu
    {
        private:
            LCDMenuLib_menu * parent;                               // Parent menu, NULL if this is the top
            LCDMenuLib_menu * child;                                // First child menu, NULL if no children
            LCDMenuLib_menu * sibling;                              // Next sibling menu, NULL if this is the last sibling

            void setParent(LCDMenuLib_menu &p);                     // Sets the menu's parent to p
            void addSibling(LCDMenuLib_menu &s,LCDMenuLib_menu &p); // Adds a sibling s with parent p.  If the menu already has a sibling, ask that sibling to add it
            
            uint8_t id;                                             // Name of this menu           
            uint8_t param;
            
        public:  
             // Constructs the menu with a name and a NULL use function (be careful calling it)
            LCDMenuLib_menu(uint8_t p_id, uint8_t p_param, LCDML_Menu_FuncPtr p_callback_function, LCDML_Con_FuncPtr p_condetion_function);           
            
            LCDML_Menu_FuncPtr callback_function;                   // Menu Funktion callback            
            LCDML_Con_FuncPtr condetion_function;                   // Menu Condetion callback
            
            
            
            void addChild(LCDMenuLib_menu &c);                      // Adds the child c to the menu.  
                                                                    // If the menu already has a child, ask the child to add it as a sibling
            LCDMenuLib_menu * getChild(uint8_t which);              // Returns a pointer to the which'th child of this menu
            LCDMenuLib_menu * getSibling(uint8_t howfar);           // Returns a pointer to the sibling howfar siblings away from this menu
            LCDMenuLib_menu * getParent();                          // Returns this menu's parent menu.  If no parent, returns itself
            
            uint8_t getID();                                        // return the id of a menu element
            boolean checkCondetion();                                 // check menu display condetion
            uint8_t getParam();                                     // return a parameter which is set on the initialisation
            
            void callback(uint8_t p);                                        // call menu callback
            boolean checkCallback();                                  // check if callback can called
            
           
           
            
    };
#endif