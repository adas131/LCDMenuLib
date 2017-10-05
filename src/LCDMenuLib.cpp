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

#    include <LCDMenuLib.h>

// ********************************************************************
// constructor
// @param
//         menu instance
//        LCD instance
//        flash table for menu elements
//        lcd rows
//        lcd colls
// @return
//        ---
// ********************************************************************
LCDMenuLib::LCDMenuLib(LCDMenu &p_r, const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr triggerBackend)
// ********************************************************************
{
    // initialisation   
    rootMenu        = &p_r;
    curMenu         = rootMenu;
    control         = 0; 
    cursor_pos      = 0;
    layer           = 0;
    layer_save[0]   = 0;
    child_cnt       = 0;
    rows            = p_rows;
    cols            = (p_cols-1); 
    activMenu       = NULL;
    button          = 0;

    callback_contentUpdate          = contentUpdate;     // Update Content            
    callback_contentClear           = contentClear;      // Clear Content
    callback_functionTriggerBackend = triggerBackend;    // Trigger Backend
}







/* ******************************************************************** *
 * private
 * @param
 *        menu instance (pointer)
 *        menu element (pointer)
 * @return
 *        search status (uint8)
 * ******************************************************************** */
uint8_t        LCDMenuLib::selectElementDirect(LCDMenu &p_m, uint8_t p_search)
/* ******************************************************************** */
{
    //deklaration
    LCDMenu * search = &p_m;   
    uint8_t found    = 0;

    bitSet(control, _LCDML_control_search_display);
            
    do 
    {
        tmp = search->getChild(0);
            
        if (tmp) {//check elements for childs                    
            goEnter();                    
            
            if (tmp->getID() == p_search) { //search elements in this layer            
                found = 1;                
                break;
            }
            else {
                activMenu = NULL;
                //bitClear(reg1, _LCDML_reg1_func_active);            
            }

            found = selectElementDirect(*tmp, p_search); //recursive search until found is true or last item reached
                        
            if (found == 1) {  //check result
                //something found
                break; 
            } 
            else {
                activMenu = NULL;
                //bitClear(reg1, _LCDML_reg1_func_active);
                //nothing found
                //goto next root element
                goBack();
                Button_udlr(_LCDML_button_down);                
            }                
        } 
        else {            
            //no childs found                
            if (search->getID() == p_search) {  //search
            
                //found something
                found = 1;                
                break;
            } 
            else {
                activMenu = NULL;
                //bitClear(reg1, _LCDML_reg1_func_active);
                //select next element            
                Button_udlr(_LCDML_button_down);                
            }
        }        
    } while ((search=search->getSibling(1)) != NULL && found == 0);

    if (found == 1) {
        bitClear(control, _LCDML_control_search_display);
    }
    //return result
    return found;    
}


/* ******************************************************************** *
 * public: go to the first element in this menu with id = 0
 * @param
 * @return
 * ******************************************************************** */
void        LCDMenuLib::goRoot()
/* ******************************************************************** */
{
    if(activMenu != NULL) 
    {
        bitSet(control, _LCDML_control_go_root);
        Button_quit();
    } else {
        bitClear(control, _LCDML_control_go_root);
        //go back to root layer
        while(layer > 0) 
        { 
            goBack(); 
        }            
        cursor_pos = 0;
        curloc = 0;
        scroll = 0;
            
        //reset buttons and cursor position
        button = 0;
    }        
    //display menu
    display();
    bitSet(control, _LCDML_control_disp_update);
    
}

/* ******************************************************************** *
 * public: jump to an item of this menu if exists
 * @param
 *        item name (uint8)
 * @return
 * ******************************************************************** */
void        LCDMenuLib::jumpToElement(uint8_t p_element)
/* ******************************************************************** */
{    
    // check if function is active
    if(activMenu != NULL) 
    {
        if(activMenu->getID() == p_element) {
            return;
        }
        Button_quit();        
    }   
    
    goRoot();
    bitSet(control, _LCDML_control_disable_hidden);
    if(selectElementDirect(*rootMenu, p_element)) { //search element
        //open this element
        goEnter();
        //Button_enter();
    }
    bitClear(control, _LCDML_control_disable_hidden);
    bitClear(control, _LCDML_control_search_display);
   
}


/* ******************************************************************** *
 * public: go to the first element in this menu with id = 0
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::goBack()
/* ******************************************************************** */
{    
    if (layer > 0) {     //check layer
        //go back
        bitWrite(control, _LCDML_control_menu_back, 1);
        goMenu(*curMenu->getParent());
    }        
}


/* ******************************************************************** *
 * private: curloc correction
 * @param
 * @return
 *    correction (uint8)
 * ******************************************************************** */
uint8_t        LCDMenuLib::curloc_correction()
{
    uint8_t    curloc_cor = 0;
    uint8_t j = 0;
    //correct function / menu element to open for hidden menu elements
    if ((tmp = curMenu->getChild(0)) != NULL) {
        do
        {
            if (bitRead(group_en, tmp->getGroup()) || bitRead(control, _LCDML_control_disable_hidden)) {
                j++;
            }
            else {
                if (j <= curloc) {
                    curloc_cor++;
                }
            }
        } while ((tmp = tmp->getSibling(1)) != NULL);
    }
    return curloc_cor;
}


/* ******************************************************************** *
 * private: go in a menu item and start function
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::goEnter()
/* ******************************************************************** */
{
    LCDMenu *activMenu_tmp;
    uint8_t j            = 0;
       
    if (activMenu == NULL) 
    {
        //check if element has childs

        if ((tmp = curMenu->getChild(curloc + curloc_correction())) != NULL) { // check child 
            activMenu_tmp = tmp;
            
            goMenu(*tmp);
            
            


            //check if element has childs            
            if ((tmp = tmp->getChild(0)) != NULL) { 
                /*
                while ((tmp = tmp->getSibling(1)) != NULL)
                {
                    Serial.println("a7");
                    Serial.println(tmp->getID());
                    if (bitRead(group_en, tmp->getGroup()) || bitRead(control, _LCDML_control_disable_hidden)) {
                        j++;
                    }                    
                }
                */
            }
      

            if (j == 0) { 
                activMenu = activMenu_tmp;
                

                countChilds();
            }          
        }        
    }
}


/* ******************************************************************** *
 * private: go in a menu item and display next elements
 * @param
 *        pointer to next menu item
 * @return
 * ******************************************************************** */
void    LCDMenuLib::goMenu(LCDMenu &m)
/* ******************************************************************** */
{
    //declare variables
    int diff;
    scroll = 0;    

    //set current menu object    
    curMenu=&m;    
        
    if(layer < _LCDML_DISP_cfg_cursor_deep) { //check layer deep        
        if(bitRead(control, _LCDML_control_menu_back) == 0) { //check back button        
            //go into the next layer
            layer_save[layer] = curloc;            
            layer++;            
            // save parent id
            if(layer < _LCDML_DISP_cfg_cursor_deep) {
                parents[layer] = curMenu->getID();
            }            
            curloc = 0;
        } 
        else {
            //button reset
            bitWrite(control, _LCDML_control_menu_back, 0);
            
            if(layer > 0) {                
                layer--;                
                curloc = layer_save[layer];

                if(curloc >= rows) {
                    diff = curloc-(rows-1);
                    for(int i=0; i<diff; i++) 
                    {                        
                        doScroll();
                    }
                }
            }
        }
    }
    display();    
}


/* ******************************************************************** *
 * private: count sibling for an menu item if exists
 * @param
 * @return
 *    sibling count 0 = one 
 * ******************************************************************** */
uint8_t    LCDMenuLib::countChilds()
/* ******************************************************************** */
{
    //declaration
    uint8_t j = 0;

    //check if element has childs
    if ((tmp = curMenu->getChild(0)) != NULL) {    
        do
        {                
            if (bitRead(group_en, tmp->getGroup()) || bitRead(control, _LCDML_control_disable_hidden)) {                
                j++;
            } 
        } while ((tmp = tmp->getSibling(1)) != NULL);        
    }    

    if(j == 0) {
        return 0;
    } else {    
        return --j;
    }
}

/* ******************************************************************** *
 * private: clear content
 * @param
 * @return
 * ******************************************************************** */
void LCDMenuLib::display_clear()
{        
    for(uint8_t n=0; n<_LCDML_DISP_cfg_max_rows;n++) {
        content_id[n] = _LCDML_NO_FUNC;
    }    
}


/* ******************************************************************** *
 * public: display the current menu
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::display(uint8_t update)
/* ******************************************************************** */
{
    //declaration
    uint8_t i = scroll;
    uint8_t maxi = (rows + scroll);
    char buffer[_LCDML_DISP_cfg_max_string_length];

    child_cnt = countChilds();    
    //check children
    if ((tmp = curMenu->getChild(i))) {
        if (!bitRead(control, _LCDML_control_search_display)) {
            //clear lcd
            display_clear();            
            //show menu structure
            do
            {                
                if (bitRead(group_en, tmp->getGroup())) {                     
                    content_id[i-scroll] = tmp->getID();
                    i++;                    
                }

            } while ((tmp = tmp->getSibling(1)) != NULL && i<maxi);
            bitSet(control, _LCDML_control_disp_update);            
        }
    }
    else { // no children
        goBack();
        // function can run ...        
        
    }
    setCursor(); 

    if(update == 1) {
        MENU_contentUpdate();
    }

   
}


/* ******************************************************************** *
 * private: set cursor position
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::setCursor()
/* ******************************************************************** */
{
    child_cnt = countChilds();    

    //save current cursor position
    cursor_pos = curloc - scroll;

    
    
    if (cursor_pos > child_cnt) {
        cursor_pos = child_cnt;
    }    
    
    bitSet(control, _LCDML_control_cursor_update);
}


/* ******************************************************************** *
 * private: do scroll if lines in display to small to show all items
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::doScroll()
/* ******************************************************************** */
{    
    //only allow it to go up to menu element   
    while (curloc>0 && !curMenu->getChild(curloc))
    {
        curloc--;
    }
    //scroll down
    if (curloc >= (rows + scroll))
    {
        scroll++;
        display();
    }
    //scroll up
    else if (curloc < scroll)
    {
        scroll--;
        display();
    }
    //do not scroll
    else
    {
        setCursor();
    }
}
  

/* ******************************************************************** *
 * public: button enter was pressed, save the state or go in an item
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::Button_enter()
/* ******************************************************************** */
{ 
    if(activMenu != NULL) 
    {            
        //function is active        
        bitSet(button, _LCDML_button_enter);
    }
    else {
        //menu is active
        goEnter();
    }            
}

/* ******************************************************************** *
 * public: button quit was pressed
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::Button_quit()
/* ******************************************************************** */
{
    if(activMenu != NULL) 
    {
        bitSet(control, _LCDML_control_funcend);
        bitSet(button,  _LCDML_button_quit);
        activMenu = NULL;
       
    } else {
        goBack();
    }        
}


/* ******************************************************************** *
 * public: button up down left right were pressed 
 * @param
 *        (u)p (d)own (l)eft (r)ight status (uint8)
 * @return
 * ******************************************************************** */
void    LCDMenuLib::Button_udlr(uint8_t but)
/* ******************************************************************** */
{          
    if(activMenu == NULL) 
    {
        //enable up and down button for menu mode and scroll        
        switch(but)
        {
            case _LCDML_button_up:   if (curloc > 0)         { curloc--; doScroll();} break;
            case _LCDML_button_down: if (curloc < child_cnt) { curloc++; doScroll();} break;
        }        
    }
    else {
        bitSet(button, but);
    }            
}




void    LCDMenuLib::resetFunction()
{
    activMenu = NULL;
}

 

/* ******************************************************************** *
 * public: get the name of the active function
 * @param
 * @return
 *    active function (uint8)
 * ******************************************************************** */
uint8_t    LCDMenuLib::getFunction()
/* ******************************************************************** */
{ 
    if(activMenu != NULL) {
        return activMenu->getID();
    } else {
        return _LCDML_NO_FUNC;
    }        
}


/* ******************************************************************** *
 * public: get the name of the active function
 * @param
 * @return
 *    active function (uint8)
 * ******************************************************************** */
uint8_t    LCDMenuLib::getFunctionParam()
/* ******************************************************************** */
{    
    if(activMenu != NULL) {
        return activMenu->getParam();
    } else {
        return 0;
    }     
}




/* ******************************************************************** *
 * public: return the position of the current layer
 * @param
 * @return
 *    layer (uint8)
 * ******************************************************************** */
uint8_t    LCDMenuLib::getLayer()
/* ******************************************************************** */
{
    return layer;
}


/* ******************************************************************** *
 * public: the current cursor position with correcture for hidden elements
 * @param
 * @return
 *    current corsor position
 * ******************************************************************** */
uint8_t    LCDMenuLib::getCursorPos()
/* ******************************************************************** */
{    
    return (cursor_pos); //return the current cursor position
}

/* ******************************************************************** *
 * public: the current cursor position with correcture for hidden elements
 * @param
 * @return
 *    current corsor position
 * ******************************************************************** */
uint8_t    LCDMenuLib::getCursorPosAbs()
/* ******************************************************************** */
{    
    return (curloc + curloc_correction()); //return the current cursor position
}


/* ******************************************************************** *
 * public: return the cnt of childs on next layer
 * @param
 * @return
 *    child cnt
 * ******************************************************************** */
uint8_t LCDMenuLib::getChilds()
/* ******************************************************************** */
{
    return child_cnt+1;
}


/* ******************************************************************** *
 * public: get parent id 
 * @param
 * @return
 *    return the current parent id when the layer is > 0
 * ******************************************************************** */
uint8_t LCDMenuLib::getParentId()
/* ******************************************************************** */
{
    if(layer > 0 && layer < _LCDML_DISP_cfg_cursor_deep) {
        return parents[layer];
    } else {
        return _LCDML_NO_FUNC;
    }    
}

/* ******************************************************************** *
 * public: get parent id from a special layer
 * @param
 * @return
 *    return the current parent id when the layer is > 0
 * ******************************************************************** */
uint8_t LCDMenuLib::getParentId(uint8_t p_layer)
/* ******************************************************************** */
{
    if(p_layer > 0 && p_layer < _LCDML_DISP_cfg_cursor_deep) {
        return parents[p_layer];
    } else {
        return _LCDML_NO_FUNC;
    }    
}


uint8_t LCDMenuLib::getContentId(uint8_t n)
{
    if (n < _LCDML_DISP_cfg_max_rows) {
        return content_id[n];
    } else {
        return 0;
    }
    
}

void LCDMenuLib::MENU_contentUpdate()
{    
    if(activMenu == NULL || bitRead(control, _LCDML_control_funcend) || bitRead(control, _LCDML_control_update_direct)) 
    { 
        callback_contentUpdate();
    }
    bitClear(control, _LCDML_control_disp_update);
    bitClear(control, _LCDML_control_cursor_update);
    bitClear(control, _LCDML_control_update_direct);
}

void LCDMenuLib::MENU_contentClear()
{
    callback_contentClear();    
}

void LCDMenuLib::FUNC_triggerBackend()
{
    if(activMenu != NULL)
    {
        callback_functionTriggerBackend();
    }
}




void LCDMenuLib::BT_enter()             { Button_enter();                   MENU_contentUpdate(); FUNC_triggerBackend(); } 
void LCDMenuLib::BT_up()                { Button_udlr(_LCDML_button_up);    MENU_contentUpdate();  }
void LCDMenuLib::BT_down()              { Button_udlr(_LCDML_button_down);  MENU_contentUpdate();  }
void LCDMenuLib::BT_left()              { Button_udlr(_LCDML_button_left);  MENU_contentUpdate();  }
void LCDMenuLib::BT_right()             { Button_udlr(_LCDML_button_right); MENU_contentUpdate();  }
void LCDMenuLib::BT_quit()              { Button_quit();                    MENU_contentUpdate();  }
    
uint8_t LCDMenuLib::BT_checkAny()       { if((button > 0)) { return true; } else { return false; }                              }     
uint8_t LCDMenuLib::BT_checkEnter()     { if(bitRead(button, _LCDML_button_enter)) { return true; } else { return false; }      }
uint8_t LCDMenuLib::BT_checkUp()        { if(bitRead(button, _LCDML_button_up)) { return true; } else { return false; }         }     
uint8_t LCDMenuLib::BT_checkDown()      { if(bitRead(button, _LCDML_button_down)) { return true; } else { return false; }       }  
uint8_t LCDMenuLib::BT_checkLeft()      { if(bitRead(button, _LCDML_button_left)) { return true; } else { return false; }       }  
uint8_t LCDMenuLib::BT_checkRight()     { if(bitRead(button, _LCDML_button_right)) { return true; } else { return false; }      }  
    
void LCDMenuLib::BT_resetAll()          { button = 0;}
void LCDMenuLib::BT_resetEnter()        { bitClear(button, _LCDML_button_enter);    }
void LCDMenuLib::BT_resetUp()           { bitClear(button, _LCDML_button_up);       }
void LCDMenuLib::BT_resetDown()         { bitClear(button, _LCDML_button_down);     }
void LCDMenuLib::BT_resetLeft()         { bitClear(button, _LCDML_button_left);     }
void LCDMenuLib::BT_resetRight()        { bitClear(button, _LCDML_button_right);    } 
// Group
void LCDMenuLib::GP_display(uint8_t g)  { bitSet(group_en, g);                      }
void LCDMenuLib::GP_hide(uint8_t g)     { bitClear(group_en, g);                    }


void LCDMenuLib::FUNC_call()
{
    if(activMenu != NULL) {
        activMenu->callback();
    }
}


uint8_t LCDMenuLib::FUNC_setup()
{
    if(activMenu != NULL) 
    {
        // if setup == 1 then 
    }
    return 1;
      
}
uint8_t LCDMenuLib::FUNC_loop()
{
    // 
    return 1;
    
}
uint8_t LCDMenuLib::FUNC_close()
{
    // if function end
    return 1;
}


void LCDMenuLib::FUNC_goBackToMenu()
{
    if(activMenu != NULL) {
        bitSet(control, _LCDML_control_funcend);
    }
}

void LCDMenuLib::goToFunc(uint8_t id)
{
    
}

void LCDMenuLib::goToMenu(uint8_t id)
{
    
}









