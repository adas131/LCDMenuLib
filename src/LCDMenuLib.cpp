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

#   include "LCDMenuLib.h"

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
LCDMenuLib::LCDMenuLib(lcdmlMenuElement &p_r, const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr triggerBackend)
// ********************************************************************
{
    // initialisation   
    rootMenu        = &p_r;
    curMenu         = rootMenu;
    control         = 0;
    funcReg         = 0;
    cursor_pos      = 0;
    layer           = 0;
    layer_save[0]   = 0;
    child_cnt       = 0;
    rows            = p_rows;
    cols            = (p_cols-1); 
    activMenu       = NULL;
    button          = 0;
    

    BT_resetAll();

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
uint8_t        LCDMenuLib::MENU_selectElementDirect(lcdmlMenuElement &p_m, uint8_t p_search)
/* ******************************************************************** */
{
    //deklaration
    lcdmlMenuElement * search = &p_m;   
    uint8_t found    = 0;

    bitSet(control, _LCDML_control_search_display);
            
    do 
    {
        tmp = search->getChild(0);
            
        if (tmp) {//check elements for childs                    
            MENU_goEnter();                    
            
            if (tmp->getID() == p_search) { //search elements in this layer            
                found = 1;                
                break;
            }
            else {
                activMenu = NULL;
                //bitClear(reg1, _LCDML_reg1_func_active);            
            }

            found = MENU_selectElementDirect(*tmp, p_search); //recursive search until found is true or last item reached
                        
            if (found == 1) {  //check result
                //something found
                break; 
            } 
            else {
                activMenu = NULL;
                //bitClear(reg1, _LCDML_reg1_func_active);
                //nothing found
                //goto next root element
                MENU_goBack();
                if (curloc < child_cnt) { 
                    curloc++; 
                    MENU_doScroll();
                }               
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
                if (curloc < child_cnt) { 
                    curloc++; 
                    MENU_doScroll();
                }                
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
void        LCDMenuLib::MENU_goRoot()
/* ******************************************************************** */
{
    if(activMenu != NULL) 
    {               
        bitSet(funcReg, _LCDML_funcReg_end);
        FUNC_call();  // call active function for save close; 
        activMenu = NULL;
        
    } 
        
    //go back to root layer
    while(layer > 0) 
    { 
        MENU_goBack(); 
    }            
    cursor_pos = 0;
    curloc = 0;
    scroll = 0;
        
    //reset buttons and cursor position
    button = 0;
            
    //display menu
    MENU_display();
    bitSet(control, _LCDML_control_disp_update);
    
}

/* ******************************************************************** *
 * public: jump to an item of this menu if exists
 * @param
 *        item name (uint8)
 * @return
 * ******************************************************************** */
void        LCDMenuLib::MENU_jumpToElement(uint8_t p_element)
/* ******************************************************************** */
{    
    // check if function is active
    if(activMenu != NULL) 
    {
        if(activMenu->getID() == p_element) {
            return;
        }
        
        bitSet(funcReg, _LCDML_funcReg_end);       
        
        FUNC_call();    // call active function for save close;
        activMenu = NULL;           
    }   
    
    MENU_goRoot();
    bitSet(control, _LCDML_control_disable_hidden);
    if(MENU_selectElementDirect(*rootMenu, p_element)) { //search element
        //open this element
        MENU_goEnter();
    }
    bitClear(control, _LCDML_control_disable_hidden);
    bitClear(control, _LCDML_control_search_display);
   
}


/* ******************************************************************** *
 * public: go to the first element in this menu with id = 0
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::MENU_goBack()
/* ******************************************************************** */
{    
    if (layer > 0) {     //check layer
        //go back
        bitWrite(control, _LCDML_control_menu_back, 1);
        MENU_goMenu(*curMenu->getParent());
    }        
}


/* ******************************************************************** *
 * private: curloc correction
 * @param
 * @return
 *    correction (uint8)
 * ******************************************************************** */
uint8_t        LCDMenuLib::MENU_curlocCorrection()
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
void    LCDMenuLib::MENU_goEnter()
/* ******************************************************************** */
{
    lcdmlMenuElement *activMenu_tmp;
    uint8_t j            = 0;
       
    if (activMenu == NULL) 
    {
        //check if element has childs

        if ((tmp = curMenu->getChild(curloc + MENU_curlocCorrection())) != NULL) { // check child 
            activMenu_tmp = tmp;
            
            MENU_goMenu(*tmp);
            
            


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
                

                MENU_countChilds();
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
void    LCDMenuLib::MENU_goMenu(lcdmlMenuElement &m)
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
                        MENU_doScroll();
                    }
                }
            }
        }
    }
    MENU_display();    
}


/* ******************************************************************** *
 * private: count sibling for an menu item if exists
 * @param
 * @return
 *    sibling count 0 = one 
 * ******************************************************************** */
uint8_t    LCDMenuLib::MENU_countChilds()
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
 * public: display the current menu
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::MENU_display(uint8_t update)
/* ******************************************************************** */
{
    //declaration
    uint8_t i = scroll;
    uint8_t maxi = (rows + scroll);
    char buffer[_LCDML_DISP_cfg_max_string_length];

    child_cnt = MENU_countChilds();    
    //check children
    if ((tmp = curMenu->getChild(i))) {
        if (!bitRead(control, _LCDML_control_search_display)) {
            // clear content list
            for(uint8_t n=0; n<_LCDML_DISP_cfg_max_rows;n++) {
                content_id[n] = _LCDML_NO_FUNC;
            }              
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
        MENU_goBack();
        // function can run ...        
        
    }
    MENU_setCursor(); 

    if(update == 1) {
        DISP_menuUpdate();
    }

   
}


/* ******************************************************************** *
 * private: set cursor position
 * @param
 * @return
 * ******************************************************************** */
void    LCDMenuLib::MENU_setCursor()
/* ******************************************************************** */
{
    child_cnt = MENU_countChilds();    

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
void    LCDMenuLib::MENU_doScroll()
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
        MENU_display();
    }
    //scroll up
    else if (curloc < scroll)
    {
        scroll--;
        MENU_display();
    }
    //do not scroll
    else
    {
        MENU_setCursor();
    }
} 

/* ******************************************************************** *
 * public: get the name of the active function
 * @param
 * @return
 *    active function (uint8)
 * ******************************************************************** */
uint8_t    LCDMenuLib::FUNC_getFunction()
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
uint8_t    LCDMenuLib::FUNC_getFunctionParam()
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
uint8_t    LCDMenuLib::MENU_getLayer()
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
uint8_t    LCDMenuLib::MENU_getCursorPos()
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
uint8_t    LCDMenuLib::MENU_getCursorPosAbs()
/* ******************************************************************** */
{    
    return (curloc + MENU_curlocCorrection()); //return the current cursor position
}


/* ******************************************************************** *
 * public: return the cnt of childs on next layer
 * @param
 * @return
 *    child cnt
 * ******************************************************************** */
uint8_t LCDMenuLib::MENU_getChilds()
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
uint8_t LCDMenuLib::MENU_getParentId()
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
uint8_t LCDMenuLib::MENU_getParentId(uint8_t p_layer)
/* ******************************************************************** */
{
    if(p_layer > 0 && p_layer < _LCDML_DISP_cfg_cursor_deep) {
        return parents[p_layer];
    } else {
        return _LCDML_NO_FUNC;
    }    
}

/* ******************************* 
   DISP_
 * ******************************* */

uint8_t LCDMenuLib::DISP_getMenuContentId(uint8_t n)
{
    if (n < _LCDML_DISP_cfg_max_rows) {
        return content_id[n];
    } else {
        return 0;
    }
    
}

void LCDMenuLib::DISP_menuUpdate()
{    
    if(activMenu == NULL || bitRead(funcReg, _LCDML_funcReg_end) || bitRead(control, _LCDML_control_update_direct)) 
    { 
        callback_contentUpdate();
    }
    bitClear(control, _LCDML_control_disp_update);
    bitClear(control, _LCDML_control_cursor_update);
    bitClear(control, _LCDML_control_update_direct);
}

void LCDMenuLib::DISP_menuClear()
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

uint8_t LCDMenuLib::DISP_chkMenuUpdate()
{
    if (bitRead(control, _LCDML_control_disp_update) || bitRead(control, _LCDML_control_update_direct)) { 
        return true; 
    } else { 
        return false; 
    }
}

uint8_t LCDMenuLib::DISP_chkMenuCursorUpdate()
{
    if(   bitRead(control, _LCDML_control_cursor_update)) { 
        return true; 
    } else { 
        return false;
    }
}






// Group
void LCDMenuLib::GP_display(uint8_t g)  { bitSet(group_en, g);                      }
void LCDMenuLib::GP_hide(uint8_t g)     { bitClear(group_en, g);                    }


void LCDMenuLib::FUNC_call()
{
    if(activMenu != NULL) 
    {
        activMenu->callback();
    }
}


uint8_t LCDMenuLib::FUNC_setup()
{
    if(activMenu != NULL) 
    {
        // function active 
        if(bitRead(funcReg, _LCDML_funcReg_setup) == 0) {
            BT_resetAll();
            bitSet(funcReg, _LCDML_funcReg_setup); // run setup
            return true;
        } 
        else
        {
            return false;
        }         
    } 
    else 
    {
        bitClear(funcReg, _LCDML_funcReg_setup);
        return false; 
    }      
}
uint8_t LCDMenuLib::FUNC_loop()
{
    if(activMenu != NULL) 
    {
        if(bitRead(funcReg, _LCDML_funcReg_end) == false) {
            return true;
        } else {
            return false;
        }  
    }
    else
    {
        return false;
    }    
}
uint8_t LCDMenuLib::FUNC_close()
{
    if(activMenu != NULL) 
    {
        if(bitRead(funcReg, _LCDML_funcReg_end) == true) 
        {
            bitClear(funcReg, _LCDML_funcReg_setup);
            activMenu = NULL;
            return true;
        } else {
            return false;
        }
    }
    else
    {
        return false;
    }
}


void LCDMenuLib::FUNC_goBackToMenu()
{
    if(activMenu != NULL) {
        bitSet(funcReg, _LCDML_funcReg_end);
        FUNC_call();  // call active function for save close; 
        activMenu = NULL;        
    }
    BT_resetAll();
    bitClear(funcReg, _LCDML_funcReg_end);
    bitClear(funcReg, _LCDML_funcReg_setup);
    DISP_menuClear();
    MENU_display(1);
}

void LCDMenuLib::MENU_goToFunc(uint8_t id)
{
    
}

void LCDMenuLib::MENU_goToMenu(uint8_t id)
{
    
}








/* ******************************* 
   BT_
 * ******************************* */


void LCDMenuLib::BT_enter()             
{ 
    if(activMenu == NULL) 
    {            
        //menu is active
        MENU_goEnter();
    }    

    bitSet(button, _LCDML_button_enter);
    DISP_menuUpdate();    
    FUNC_triggerBackend(); 
} 

void LCDMenuLib::BT_up()                
{ 
    if(activMenu == NULL) 
    {
        if (curloc > 0) { 
            curloc--; 
            MENU_doScroll();             
        }        
    }
   
    bitSet(button, _LCDML_button_up);
    DISP_menuUpdate();  
}

void LCDMenuLib::BT_down()              
{ 

    if(activMenu == NULL) 
    {
        if(curloc < child_cnt) { 
            curloc++; 
            MENU_doScroll();
        }                
    }
   
    bitSet(button, _LCDML_button_down);
    DISP_menuUpdate(); 
}

void LCDMenuLib::BT_left()              
{
    bitSet(button, _LCDML_button_left);
    DISP_menuUpdate(); 
}

void LCDMenuLib::BT_right()             
{ 
    bitSet(button, _LCDML_button_right);   
    DISP_menuUpdate(); 
}


void LCDMenuLib::BT_quit()              
{ 
    if(activMenu != NULL) 
    {
        FUNC_goBackToMenu();       
    } else {
        MENU_goBack();
    } 
    
    DISP_menuUpdate(); 
}
    
uint8_t LCDMenuLib::BT_chkAny()       { if((button > 0)) { return true; } else { return false; }                              }     

uint8_t LCDMenuLib::BT_chkEnter()     { if(bitRead(button, _LCDML_button_enter)) { return true; } else { return false; }      }
uint8_t LCDMenuLib::BT_chkUp()        { if(bitRead(button, _LCDML_button_up)) { return true; } else { return false; }         }     
uint8_t LCDMenuLib::BT_chkDown()      { if(bitRead(button, _LCDML_button_down)) { return true; } else { return false; }       }  
uint8_t LCDMenuLib::BT_chkLeft()      { if(bitRead(button, _LCDML_button_left)) { return true; } else { return false; }       }  
uint8_t LCDMenuLib::BT_chkRight()     { if(bitRead(button, _LCDML_button_right)) { return true; } else { return false; }      }  
//uint8_t LCDMenuLib::BT_chkQuit()      { if(bitRead(button, _LCDML_button_quit)) { return true; } else { return false; }      }  
    
void LCDMenuLib::BT_resetAll()          { button = 0;}

void LCDMenuLib::BT_resetEnter()        { bitClear(button, _LCDML_button_enter);    }
void LCDMenuLib::BT_resetUp()           { bitClear(button, _LCDML_button_up);       }
void LCDMenuLib::BT_resetDown()         { bitClear(button, _LCDML_button_down);     }
void LCDMenuLib::BT_resetLeft()         { bitClear(button, _LCDML_button_left);     }
void LCDMenuLib::BT_resetRight()        { bitClear(button, _LCDML_button_right);    } 
//void LCDMenuLib::BT_resetQuit()         { bitClear(button, _LCDML_button_quit);    } 













