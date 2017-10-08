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

#include "lcdml_c_menuElement.h"



lcdmlMenuElement::lcdmlMenuElement(void)
/* ******************************************************************** */
{    
    parent  = NULL;
    sibling = NULL;
    child   = NULL;
    id      = 0;        // element name 
    group   = 1;     // element group
    param   = 0;     // element configuration
    callback_function = NULL;
}

/* ******************************************************************** *
 * constructor 
 *    @param
 *        name (uint8)
 *        group (uint8)
 *    @return
 *
 * ******************************************************************** */
lcdmlMenuElement::lcdmlMenuElement(uint8_t p_id, uint8_t p_group, uint8_t p_param, LCDML_Menu_FuncPtr p_callback_function)
/* ******************************************************************** */
{    
    parent  = NULL;
    sibling = NULL;
    child   = NULL;
    id      = p_id;        // element name 
    group   = p_group;     // element group
    param   = p_param;     // element configuration
    callback_function = p_callback_function;
}



/* ******************************************************************** *
 * private: set parent to current menu eitem
 *    @param
 *        lcdmlMenuElement (pointer)
 *    @return
 *
 * ******************************************************************** */
void lcdmlMenuElement::setParent(lcdmlMenuElement &p)
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
void lcdmlMenuElement::addSibling(lcdmlMenuElement &s,lcdmlMenuElement &p)
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
 *        lcdmlMenuElement (pointer)
 *    @return
 *
 * ******************************************************************** */
void lcdmlMenuElement::addChild(lcdmlMenuElement &c)
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
lcdmlMenuElement * lcdmlMenuElement::getChild(uint8_t which)
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
lcdmlMenuElement * lcdmlMenuElement::getSibling(uint8_t howfar)
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
lcdmlMenuElement * lcdmlMenuElement::getParent()
/* ******************************************************************** */
{
    if (parent) { // get parent if exists        
        return parent;        
    }
    else { // root menu    
        return this;
    }
}



uint8_t lcdmlMenuElement::getID()
{
    return id;
}

uint8_t lcdmlMenuElement::getGroup() {
    return group;
}

uint8_t lcdmlMenuElement::getParam() {
    return param;
}

void lcdmlMenuElement::callback()
{
    if(callback_function != NULL) {
        callback_function();
    }
}
