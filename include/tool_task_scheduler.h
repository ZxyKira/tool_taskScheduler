/* *****************************************************************************************
 *    File Name   :tool_task_scheduler.h
 *    Create Date :2018-09-09
 *    Modufy Date :2021-04-25
 *    Information :
 */

#ifndef TOOL_TASK_SCHEDULER_VERSION
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* *****************************************************************************************
 *    Include
 */ 
#include <stdint.h>
#include <stdbool.h>

#include "version.h"
#include "tool_fifo.h"



/* *****************************************************************************************
 *    Macro
 */ 
/*----------------------------------------
 *  TOOL_TASK_SCHEDULER_REQ_FW_TOOL_FIFO_VERSION
 *----------------------------------------*/
#define TOOL_TASK_SCHEDULER_REQ_FW_TOOL_FIFO_VERSION VERSION_DEFINEE(1,0,0)
#if VERSION_CHECK_COMPATIBLE(TOOL_FIFO_VERSION, TOOL_TASK_SCHEDULER_REQ_FW_TOOL_FIFO_VERSION)
  #if VERSION_CHECK_COMPATIBLE(TOOL_FIFO_VERSION, TOOL_TASK_SCHEDULER_REQ_FW_TOOL_FIFO_VERSION) == 2
    #error "TOOL_FIFO_VERSION under 1.0.x"
  #else
    #warning "TOOL_FIFO_VERSION revision under 1.0.0"
  #endif
#endif



/*----------------------------------------
 *  TOOL_TASK_SCHEDULER_VERSION
 *----------------------------------------*/
#define TOOL_TASK_SCHEDULER_VERSION VERSION_DEFINEE(1, 0, 0)



/* *****************************************************************************************
 *    Typedef List
 */ 
typedef struct _tool_task_scheduler_event_t tool_task_scheduler_event_t;
typedef struct _tool_task_scheduler_handle_t tool_task_scheduler_handle_t;
typedef enum _tool_task_scheduler_prtority tool_task_scheduler_prtority;
typedef struct _tool_task_scheduler_config_t tool_task_scheduler_config_t;



/* *****************************************************************************************
 *    Typedef Function
 */ 

/*----------------------------------------
 *  tool_task_scheduler_execute_t
 *----------------------------------------*/
typedef void (*tool_task_scheduler_execute_t)(void* args);



/* *****************************************************************************************
 *    Struct/Union/Enum
 */ 

/*----------------------------------------
 *  tool_task_scheduler_api_t
 *----------------------------------------*/
struct tool_task_scheduler_api_t{
  bool(*initialze)    (tool_task_scheduler_handle_t *handle, const tool_task_scheduler_config_t *config);
  bool(*execute)      (tool_task_scheduler_handle_t *handle);
  bool(*breakExecute) (tool_task_scheduler_handle_t *handle);
  bool(*addTask)      (tool_task_scheduler_handle_t *handle, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority);
  bool(*addTaskSuper) (tool_task_scheduler_handle_t *handle, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority);  
};



/* *****************************************************************************************
 *    Typedef Struct/Union/Enum
 */ 

/*----------------------------------------
 *  tool_task_scheduler_event_t
 *----------------------------------------*/
typedef struct _tool_task_scheduler_event_t{
  tool_task_scheduler_execute_t execute;
  void*  attachment;
}tool_task_scheduler_event_t;



/*----------------------------------------
 *  tool_task_scheduler_handle_t
 *----------------------------------------*/
typedef struct _tool_task_scheduler_handle_t{
  struct{
    tool_fifo_t high;
    tool_fifo_t normal;
    tool_fifo_t low;
  }fifo;
  uint32_t flag;
}tool_task_scheduler_handle_t;



/*----------------------------------------
 *  tool_task_scheduler_prtority
 *----------------------------------------*/
typedef enum _tool_task_scheduler_prtority{
  tool_task_scheduler_prtority_low,
  tool_task_scheduler_prtority_normal,
  tool_task_scheduler_prtority_high
}tool_task_scheduler_prtority;



/*----------------------------------------
 *  tool_task_scheduler_config_buffer_t
 *----------------------------------------*/
typedef struct _tool_task_scheduler_config_buffer_t{
  tool_task_scheduler_event_t *eventBuffer;
  uint16_t bufferQuantity;
}tool_task_scheduler_config_buffer_t;



/*----------------------------------------
 *  tool_task_scheduler_config_t
 *----------------------------------------*/
typedef struct _tool_task_scheduler_config_t{
  tool_task_scheduler_config_buffer_t prtorityLow;
  tool_task_scheduler_config_buffer_t prtorityNormal;
  tool_task_scheduler_config_buffer_t prtorityHigh;
}tool_task_scheduler_config_t;



/* *****************************************************************************************
 *    Inline Function
 */ 



/* *****************************************************************************************
 *    Extern
 */ 
extern const struct tool_task_scheduler_api_t tool_task_scheduler_api;
extern bool tool_task_scheduler_initialze(tool_task_scheduler_handle_t* _this, const tool_task_scheduler_config_t *config);
extern bool tool_task_scheduler_execute(tool_task_scheduler_handle_t* _this);
extern bool tool_task_scheduler_breakExecute(tool_task_scheduler_handle_t* _this);
extern bool tool_task_scheduler_addTask(tool_task_scheduler_handle_t* _this, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority);
extern bool tool_task_scheduler_addTaskSuper(tool_task_scheduler_handle_t* _this, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority);



#ifdef __cplusplus
}
#endif //__cplusplus
#endif //TOOL_TASK_SCHEDULER_VERSION
/* *****************************************************************************************
 *  End of file
 */
