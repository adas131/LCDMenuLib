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

#include "LCDMenuLib_menu.h"




/* ******************************************************************** *
 * constructor 
 *    @param
 *        name (uint8)
 *        group (uint8)
 *    @return
 *
 * ******************************************************************** */
LCDMenuLib_menu::LCDMenuLib_menu(uint8_t p_id, uint8_t p_param, LCDML_Menu_FuncPtr p_callback_function, LCDML_Con_FuncPtr p_condetion_function)
/* ******************************************************************** */
{    
    parent  = NULL;
    sibling = NULL;
    child   = NULL;
    id      = p_id;        // element name 
    param   = p_param;     // element configuration
    callback_function = p_callback_function;
    condetion_function = p_condetion_function;
}



/* ******************************************************************** *
 * private: set parent to current menu eitem
 *    @param
 *        LCDMenuLib_menu (pointer)
 *    @return
 *
 * ******************************************************************** */
void LCDMenuLib_menu::setParent(LCDMenuLib_menu &p)
/* ******************************************************************** */
{
    parent = &p;
}


/* ******************************************************************** *
 * private: add a sibling to the current menu  
 *    @param
 *        sibling before this (pointer) 
 *        parent (pointer)
 *    @return
 *
 * ******************************************************************** */
void LCDMenuLib_menu::addSibling(LCDMenuLib_menu &s,LCDMenuLib_menu &p)
/* ******************************************************************** */
{
    if (sibling) { // add sibling if sibing exists 
        sibling->addSibling(s,p);
    } else
    { // add sibling and parent 
        sibling =& s;
        sibling->setParent(p);
    }
}


/* ******************************************************************** *
 * public: add a child to current menu
 *    @param
 *        LCDMenuLib_menu (pointer)
 *    @return
 *
 * ******************************************************************** */
void LCDMenuLib_menu::addChild(LCDMenuLib_menu &c)
/* ******************************************************************** */
{
    if (child) { // add sibling if child exists
        child->addSibling(c,*this);
    } else
    { // add child  and parent
        child=&c;
        child->setParent(*this);
    }
}


/* ******************************************************************** *
 * public: returns the childs pointer if it exists
 *    @param
        which ...
 *    @return
 *
 * ******************************************************************** */
LCDMenuLib_menu * LCDMenuLib_menu::getChild(uint8_t which)
/* ******************************************************************** */
{
    if (child) { // return child if exists        
        return child->getSibling(which);        
    }
    else { // this menu item has no children
        return NULL;
    }
}


/* ******************************************************************** *
 * public: returns a sibling pointer if exists
 *    @param
 *
 *    @return
 *
 * ******************************************************************** */
LCDMenuLib_menu * LCDMenuLib_menu::getSibling(uint8_t howfar)
/* ******************************************************************** */
{
    if (howfar == 0) { // this sibling                    
        return this;                
    }
    else if (sibling) {    // get next sibling    
        return sibling->getSibling(howfar - 1);
    }
    else { // asking for a nonexistent sibling    
        return NULL;
    }
}


/* ******************************************************************** *
 * public: return the parent pointer if exists
 *    @param
 *
 *    @return
 *
 * ******************************************************************** */
LCDMenuLib_menu * LCDMenuLib_menu::getParent()
/* ******************************************************************** */
{
    if (parent) { // get parent if exists        
        return parent;        
    }
    else { // root menu    
        return this;
    }
}



uint8_t LCDMenuLib_menu::getID()
{
    return id;
}

boolean LCDMenuLib_menu::chkCondetion()
{
    if(condetion_function != NULL)
    {
        return condetion_function();
    }
    else
    {
        return true;
    }
}


uint8_t LCDMenuLib_menu::getParam() {
    return param;
}

void LCDMenuLib_menu::callback()
{
    if(callback_function != NULL) {
        callback_function();
    }
}

boolean LCDMenuLib_menu::chkCallback()
{
    if(callback_function == NULL) {
        return false;
    } else {
        return true;
    }
}
