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

#ifndef LCDMenu_h
#   define LCDMenu_h

#   include "Arduino.h"

    typedef void(* LCDML_Menu_FuncPtr)();
 
    
    
 
    class lcdmlMenuElement
    {
        private:
            lcdmlMenuElement * parent;                    // Parent menu, NULL if this is the top
            lcdmlMenuElement * child;                        // First child menu, NULL if no children
            lcdmlMenuElement * sibling;                    // Next sibling menu, NULL if this is the last sibling

            void setParent(lcdmlMenuElement &p);                    // Sets the menu's parent to p
            void addSibling(lcdmlMenuElement &s,lcdmlMenuElement &p);        // Adds a sibling s with parent p.  If the menu already has a sibling, ask that sibling to add it
            
            uint8_t id;                                 // Name of this menu
            uint8_t group;                              // element group (0-7)
            uint8_t param;                              // element config (0-3 : parameter, 4 = callback function, 5 = switch function, 6-7 free)
            
            LCDML_Menu_FuncPtr callback_function;
            
        public:  
            lcdmlMenuElement(void);
            lcdmlMenuElement(uint8_t p_id, uint8_t p_group, uint8_t p_param, LCDML_Menu_FuncPtr p_callback_function);            // Constructs the menu with a name and a NULL use function (be careful calling it)
            
                        
            
            void addChild(lcdmlMenuElement &c);                    // Adds the child c to the menu.  
                                                        // If the menu already has a child, ask the child to add it as a sibling
            lcdmlMenuElement * getChild(uint8_t which);            // Returns a pointer to the which'th child of this menu
            lcdmlMenuElement * getSibling(uint8_t howfar);        // Returns a pointer to the sibling howfar siblings away from this menu
            lcdmlMenuElement * getParent();                        // Returns this menu's parent menu.  If no parent, returns itself
            
            uint8_t getID();
            uint8_t getGroup();
            uint8_t getParam();
            
            void callback();
            
    };
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
#endif