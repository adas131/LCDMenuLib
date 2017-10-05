
boolean LCDML_DISP_update_content()  { if(   bitRead(LCDML.control, _LCDML_control_disp_update) || bitRead(LCDML.control, _LCDML_control_update_direct)) { return true; } else { return false; } }
boolean LCDML_DISP_update_cursor()   { if(   bitRead(LCDML.control, _LCDML_control_cursor_update)) { return true; } else { return false; } }










