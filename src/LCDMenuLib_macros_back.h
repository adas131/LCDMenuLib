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
 
#ifndef _LCDML_macros_back_h
    #define _LCDML_macros_back_h
    
    /* ------------------ 
     * Include Arduino                                                     
     * ------------------
     */
    #include <Arduino.h>

// ----------------------------------------
// define loop modes
// ----------------------------------------
    #define     _LCDML_no_priority          0
    #define     _LCDML_priority             1
    
    #define     _LCDML_stop                 0
    #define     _LCDML_start                1
    #define     _LCDML_startDirect          2
    #define     _LCDML_stable               3
    
    #define     _LCDML_root                 0
    #define     _LCDML_next                 1
    
    #define     _LCDML_ms                   0
    #define     _LCDML_us                   1

    #define     _LCDML_BACK_default_id      255        

// ----------------------------------------
// Generation of function names
// ----------------------------------------
    // macro: setup function
    #define LCDML_BACK_setup(name)\
        LCDML_BACK_setup_##name(void)
    // macro: loop function
    #define LCDML_BACK_loop(name)\
        LCDML_BACK_loop_##name(void)
    // macro: stableState function
    #define LCDML_BACK_stable(name)\
        LCDML_BACK_stable_##name(void)

// init thread system
    // macro: creates the LCDML_BACK name pointer on a function
    #define LCDML_BACK(name)\
        LCDML_BACK_function_##name
    // macro: creates the LCDML_BACK managemand variables
    

        
// ----------------------------------------
// Initialisation
// ----------------------------------------        
        
    #ifndef _LCDML_ESP
        // stored in flash (arduino)
        #define LCDML_BACK_H_cnt() \
            const PROGMEM uint8_t g_LCDML_BACK_cnt = _LCDML_BACK_cnt+1
            
        #define LCDML_BACK_H_id(id, name) \
            const PROGMEM uint8_t g_LCDML_BACK_id__##name  = id
            
        #define LCDML_BACK_H_time(name, init_time) \
            const PROGMEM uint32_t   _LCDML_BACK_time_default__##name  = (uint32_t)(init_time)            
    #else
        // stored in ram (esp)
        #define LCDML_BACK_H_cnt() \
            const uint8_t g_LCDML_BACK_cnt = _LCDML_BACK_cnt+1
            
        #define LCDML_BACK_H_id(id, name) \
            const uint8_t g_LCDML_BACK_id__##name  = id
            
        #define LCDML_BACK_H_time(name, init_time) \
            const uint32_t   _LCDML_BACK_time_default__##name  = (uint32_t)(init_time)
    
    #endif        
    

    #define LCDML_BACK_init()\
        LCDML_FuncPtr g_LCDML_BACK_priority[_LCDML_BACK_cnt+1];\
        LCDML_BACK_H_cnt(); \
        LCDML_bitCreateVar(g_LCDML_BACK_start_stop, _LCDML_BACK_cnt);\
        LCDML_bitCreateVar(g_LCDML_BACK_reset, _LCDML_BACK_cnt);\
        LCDML_bitCreateVar(g_LCDML_BACK_mil_mic, _LCDML_BACK_cnt);\
        LCDML_bitCreateVar(g_LCDML_BACK_ret, _LCDML_BACK_cnt);\
        uint8_t g_LCDML_BACK_loop_status = true;\
        uint8_t g_LCDML_BACK_lastFunc = _LCDML_NO_FUNC;\
        unsigned long g_LCDML_BACK_timer[(_LCDML_BACK_cnt+1)];\
        void LCDML_CONTROL_setup();\
        void LCDML_CONTROL_loop()

// ----------------------------------------
// Create threads
// ----------------------------------------        
        
    // thread create
    // macro: help to create a new thread: generate weak functions
    #define LCDML_BACK_help_new_thread(id, name, status, time_type, ret)\
        LCDML_BACK_H_id(id, name); \
        void LCDML_BACK_setup_##name(void);\
        void LCDML_BACK_loop_##name(void);\
        void LCDML_BACK_stable_##name(void);\
        void LCDML_BACK_function_##name(void);\
        void LCDML_BACK_setupInit_##id(void){ \
            g_LCDML_BACK_priority[id] = LCDML_BACK_function_##name;\
            LCDML_bitWriteValue(g_LCDML_BACK_mil_mic, id, time_type); \
            LCDML_bitWriteValue(g_LCDML_BACK_ret, id, ret); \
            if(status == _LCDML_start) {\
                LCDML_BACK_start(name);\
            } else if(status == _LCDML_startDirect) { \
                LCDML_BACK_startDirect(name); \
            } else if(status == _LCDML_stop) { \
                LCDML_BACK_stopStable(name); \
            }\
        }
        
    // macro: create a new thread with dynamc times
    #define LCDML_BACK_new_timebased_dynamic(id, init_time, time_type, status, ret, name)\
        LCDML_BACK_help_new_thread(id, name, status, time_type, ret);\
        LCDML_BACK_H_time(name, init_time); \
        unsigned long g_LCDML_BACK_dynTime_##name = (uint32_t)(init_time); \
        LCDML_BACK_THREAD_FUNCTION_TIME_BASED(name, g_LCDML_BACK_dynTime_##name);          
        
    // macro: create a new thread with static times
    #define LCDML_BACK_new_timebased_static(id, init_time, time_type, status, ret, name)\
        LCDML_BACK_help_new_thread(id, name, status, time_type, ret);\
        LCDML_BACK_THREAD_FUNCTION_TIME_BASED(name, init_time);
        

    // macro: create a event based thread
    #define LCDML_BACK_new_eventbased(id, name)\
        LCDML_BACK_help_new_thread(id, name, false, false, false);\
        LCDML_BACK_THREAD_FUNCTION_EVENT_BASED(name);        


// ----------------------------------------
// Timebased control
// ----------------------------------------
    // macro: thread start single
    #define LCDML_BACK_start(name)\
        LCDML_bitWriteValue(g_LCDML_BACK_start_stop, g_LCDML_BACK_id__##name, true)
        
    #define LCDML_BACK_startDirect(name)\
        LCDML_BACK_dynamic_timeToZero(name);\
        LCDML_bitWriteValue(g_LCDML_BACK_start_stop, g_LCDML_BACK_id__##name, true)

    // macro: thread stop
    #define LCDML_BACK_stop(name)\
        LCDML_bitWriteValue(g_LCDML_BACK_start_stop, g_LCDML_BACK_id__##name, false) 
        
    // macro: thread stop stable => calls a function at the end
    #define LCDML_BACK_stopStable(name)\
        LCDML_BACK_stop(name);\
        LCDML_BACK_stable_##name()
        
    // macro: thread reset
    #define LCDML_BACK_reset(name)\
        LCDML_bitWriteValue(g_LCDML_BACK_reset, g_LCDML_BACK_id__##name, false); \
        LCDML_BACK_dynamic_restartTimer(name)
            
    //macro: thread reStart 
    #define LCDML_BACK_restart(name)\
        LCDML_BACK_reset(name);\
        LCDML_BACK_start(name)
        
    #define LCDML_BACK_restartDirect(name)\
        LCDML_BACK_reset(name);\
        LCDML_BACK_startDirect(name)

// ----------------------------------------
// Event control
// ----------------------------------------
    #define LCDML_BACK_event_start(name)\
        LCDML_BACK_start(name)
    #define LCDML_BACK_event_reset(name)\
        LCDML_BACK_reset(name)
    #define LCDML_BACK_event_restart(name)\
        LCDML_BACK_restart(name)

// ----------------------------------------
// All Taks
// ----------------------------------------
    // macro: thread start all
    #define LCDML_BACK_all_start()\
        for(uint8_t l_LCDML_BACK_i = 0; l_LCDML_BACK_i<g_LCDML_BACK_cnt;l_LCDML_BACK_i++) { \
            LCDML_bitWriteValue(g_LCDML_BACK_start_stop, l_LCDML_BACK_i, true); \
        }
    // macro: thread stop all
    #define LCDML_BACK_all_stop()\
        for(uint8_t l_LCDML_BACK_i = 0; l_LCDML_BACK_i<g_LCDML_BACK_cnt;l_LCDML_BACK_i++) { \
            LCDML_bitWriteValue(g_LCDML_BACK_start_stop, l_LCDML_BACK_i, false); \
        }
    // macro: thread reset all
    #define LCDML_BACK_all_reset()\
        for(uint8_t l_LCDML_BACK_i = 0; l_LCDML_BACK_i<g_LCDML_BACK_cnt;l_LCDML_BACK_i++) { \
            LCDML_bitWriteValue(g_LCDML_BACK_reset, l_LCDML_BACK_i, false); \
        }
    //macro: thread reStart all 
    #define LCDML_BACK_all_restart()\
        LCDML_BACK_all_reset();\
        LCDML_BACK_all_start()

// ----------------------------------------
// Group Markos
// ----------------------------------------
    // macro: add group elements
    #define LCDML_BACK_group(name)\
        {g_LCDML_BACK_id__##name/7, g_LCDML_BACK_id__##name%7} 
    // macro: create a new group
    #define LCDML_BACK_group_init(name, thread_cnt)\
        uint8_t g_LCDML_BACK_group__##name##_cnt = thread_cnt;\
        uint8_t g_LCDML_BACK_group__##name[thread_cnt][2] = 
    // macro: thread start group
    #define LCDML_BACK_group_start(group_name)\
        for(uint8_t l_LCDML_BACK_i = 0; l_LCDML_BACK_i<(g_LCDML_BACK_group__##group_name##_cnt);l_LCDML_BACK_i++) {\
            bitWrite(g_LCDML_BACK_start_stop[g_LCDML_BACK_group__##group_name[l_LCDML_BACK_i][0]], g_LCDML_BACK_group__##group_name[l_LCDML_BACK_i][1], true);\
        }
    // macro: thread stop group
    #define LCDML_BACK_group_stop(group_name)\
        for(uint8_t l_LCDML_BACK_i = 0; l_LCDML_BACK_i<(g_LCDML_BACK_group__##group_name##_cnt);l_LCDML_BACK_i++) {\
            bitWrite(g_LCDML_BACK_start_stop[g_LCDML_BACK_group__##group_name[l_LCDML_BACK_i][0]], g_LCDML_BACK_group__##group_name[l_LCDML_BACK_i][1], false);\
        }
    // macro: thread reset group
    #define LCDML_BACK_group_reset(group_name)\
        for(uint8_t l_LCDML_BACK_i = 0; l_LCDML_BACK_i<(g_LCDML_BACK_group__##group_name##_cnt);l_LCDML_BACK_i++) {\
            bitWrite(g_LCDML_BACK_reset[g_LCDML_BACK_group__##group_name[l_LCDML_BACK_i][0]], g_LCDML_BACK_group__##group_name[l_LCDML_BACK_i][1], false);\
        }
    // macro: thread start group
    #define LCDML_BACK_group_restart(group_name)\
        LCDML_BACK_group_reset(group_name);\
        LCDML_BACK_group_start(group_name)
    




// ----------------------------------------
// Check thread state
// ----------------------------------------
    //macro: thread is running ? 
    #define LCDML_BACK_isRun(name)\
        LCDML_bitReadValue(g_LCDML_BACK_start_stop, g_LCDML_BACK_id__##name)
    
// ----------------------------------------    
// Dynamic times    
// ----------------------------------------
    //macro: edit thread loop time
    #define LCDML_BACK_dynamic_setLoopTime(name, time)\
        g_LCDML_BACK_dynTime_##name = time
        
    //macro: get thread loop time
    #define LCDML_BACK_dynamic_getLoopTime(name)\
        g_LCDML_BACK_dynTime_##name  
        
    // macro: reset a dynamic thread to default settings
    #define LCDML_BACK_dynamic_setDefaultTime(name)\
        g_LCDML_BACK_dynTime_##name =_LCDML_BACK_time_default__##name
        
    // macro: restart dynamic thread time
    #define LCDML_BACK_dynamic_restartTimer(name)\
        g_LCDML_BACK_timer[g_LCDML_BACK_id__##name] = (!LCDML_bitReadValue(g_LCDML_BACK_mil_mic, g_LCDML_BACK_id__##name)) ? millis() : micros()        
        
    // macro: setTime 0
    #define LCDML_BACK_dynamic_timeToZero(name)\
        g_LCDML_BACK_timer[g_LCDML_BACK_id__##name] = (!LCDML_bitReadValue(g_LCDML_BACK_mil_mic, g_LCDML_BACK_id__##name)) ? (millis() +1) : (micros() + 1)


// ---------------------------------------- 
// Direct call
// ---------------------------------------- 
    // macro: call a thread
    #define LCDML_BACK_call(name)\
        LCDML_BACK_function_##name()
    // macro: call a thread loop function
    #define LCDML_BACK_call_loop(name)\
        LCDML_BACK_loop_##name()
    // macro: call a thread setup function
    #define LCDML_BACK_call_setup(name)\
        LCDML_BACK_setup_##name()
    // macro: call a thread stable function
    #define LCDML_BACK_call_stable(name)\
        LCDML_BACK_stable_##name()    
    


//help macros
    // macro: thread timer with return
    // attention: when the wait_time is bigger then millis on startup the result ist true 
    // to fix this, the timer_var have to be initialise with the waittime 
    #define LCDML_BACK_TIMER(timer_var, wait_time, time_type)\
		if(!((time_type() - timer_var) >= wait_time)) {  return; }\
		timer_var = time_type();
        
    //macro: thread is running with return !
    #define LCDML_BACK_THREAD_isRun(name)\
        if(!LCDML_bitReadValue(g_LCDML_BACK_start_stop, g_LCDML_BACK_id__##name)) { return; } 

    //macro: reset the reset bin from a thread
    #define LCDML_BACK_UNSET_reset(name)\
        LCDML_bitWriteValue(g_LCDML_BACK_reset, g_LCDML_BACK_id__##name, true)        

    //macro: get the reset bit from a thread
    #define LCDML_BACK_GET_reset(name)\
        LCDML_bitReadValue(g_LCDML_BACK_reset, g_LCDML_BACK_id__##name)        

    //macro: create the thread setup function 
    #define LCDML_BACK_THREAD_FUNCTION_TIME_BASED(name, time)                                               \
        void LCDML_BACK_function_##name(void)                                                               \
        {                                                                                                   \
            LCDML_BACK_THREAD_isRun(name);                                                                  \
            if(LCDML_BACK_GET_reset(name) == false) {                                                       \
                LCDML_BACK_UNSET_reset(name);                                                               \
                LCDML_BACK_setup_##name();                                                                  \
                g_LCDML_BACK_timer[g_LCDML_BACK_id__##name] = time;                                         \
            }                                                                                               \
                                                                                                            \
            if(!LCDML_bitReadValue(g_LCDML_BACK_mil_mic, g_LCDML_BACK_id__##name)) {                        \
                LCDML_BACK_TIMER(g_LCDML_BACK_timer[g_LCDML_BACK_id__##name], time, millis);                \
            } else {                                                                                        \
                LCDML_BACK_TIMER(g_LCDML_BACK_timer[g_LCDML_BACK_id__##name], time, micros);                \
            }                                                                                               \
                                                                                                            \
            LCDML_BACK_loop_##name();                                                                       \
            g_LCDML_BACK_loop_status = LCDML_bitReadValue(g_LCDML_BACK_ret, g_LCDML_BACK_id__##name);       \
        }

    //macro: create the thread setup function 
    #define LCDML_BACK_THREAD_FUNCTION_EVENT_BASED(name)                                                    \
        void LCDML_BACK_function_##name(void)                                                               \
        {                                                                                                   \
            LCDML_BACK_THREAD_isRun(name);                                                                  \
            if(LCDML_BACK_GET_reset(name) == true) {                                                        \
                LCDML_BACK_UNSET_reset(name);                                                               \
                LCDML_BACK_setup_##name();                                                                  \
            }                                                                                               \
                                                                                                            \
            LCDML_BACK_loop_##name();                                                                       \
            g_LCDML_BACK_loop_status = LCDML_bitReadValue(g_LCDML_BACK_ret, g_LCDML_BACK_id__##name);       \
            LCDML_BACK_stop(name);                                                                          \
        }

    #define LCDML_BACK_help_setup(id) \
        LCDML_BACK_setupInit_##id();

    #define LCDML_setup(N)\
        LCDML_DISP_initSetup(_LCDML_DISP_cnt);\
        LCDML_BACK_help_setupInit(N)

    

        
    
    
#endif