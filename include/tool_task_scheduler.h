/* *****************************************************************************************
 *    File Name   :tool_task_scheduler.h
 *    Create Date :2018-09-09
 *    Modufy Date :2021-03-28
 *    Information :
 */
#ifndef tool_task_scheduler_H
#define tool_task_scheduler_H
#include "stdint.h"
#include "stdbool.h" 
#include "tool_fifo.h"

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------
//  Typedef 
//------------------------------------- 
typedef void (*tool_task_scheduler_execute_t)(void* args);

/* *****************************************************************************************
 *  Struct tool_task_scheduler_event_t
 */
typedef struct _tool_task_scheduler_event_t{
  tool_task_scheduler_execute_t execute;
  void*  attachment;
}tool_task_scheduler_event_t;

/* *****************************************************************************************
 *  Struct tool_task_scheduler_handle_t
 */
typedef struct _tool_task_scheduler_handle_t{
  struct{
    tool_fifo_t high;
    tool_fifo_t normal;
    tool_fifo_t low;
  }fifo;
  uint32_t flag;
}tool_task_scheduler_handle_t;

/* *****************************************************************************************
 *  Enum tool_task_scheduler_prtority
 */
typedef enum{
  tool_task_scheduler_prtority_low,
  tool_task_scheduler_prtority_normal,
  tool_task_scheduler_prtority_high
}tool_task_scheduler_prtority;

/* *****************************************************************************************
 *  Struct tool_task_scheduler_config_buffer_t
 */
typedef struct _tool_task_scheduler_config_buffer_t{
  tool_task_scheduler_event_t *eventBuffer;
  uint16_t bufferQuantity;
}tool_task_scheduler_config_buffer_t;

/* *****************************************************************************************
 *  Struct tool_task_scheduler_config_t
 */
typedef struct{
  tool_task_scheduler_config_buffer_t prtorityLow;
  tool_task_scheduler_config_buffer_t prtorityNormal;
  tool_task_scheduler_config_buffer_t prtorityHigh;
}tool_task_scheduler_config_t;

/* *****************************************************************************************
 *  Struct tool_task_scheduler_handle_t
 */
typedef struct _tool_task_scheduler_api_t{
  bool(*initialze)    (tool_task_scheduler_handle_t *handle, const tool_task_scheduler_config_t *config);
  bool(*execute)      (tool_task_scheduler_handle_t *handle);
  bool(*breakExecute) (tool_task_scheduler_handle_t *handle);
  bool(*addTask)      (tool_task_scheduler_handle_t *handle, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority);
  bool(*addTaskSuper) (tool_task_scheduler_handle_t *handle, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority);  
}tool_task_scheduler_api_t;

/* *****************************************************************************************
 *  Extern API
 */
extern const tool_task_scheduler_api_t tool_task_scheduler_api;

bool tool_task_scheduler_initialze(tool_task_scheduler_handle_t* _this, const tool_task_scheduler_config_t *config);
bool tool_task_scheduler_execute(tool_task_scheduler_handle_t* _this);
bool tool_task_scheduler_breakExecute(tool_task_scheduler_handle_t* _this);
bool tool_task_scheduler_addTask(tool_task_scheduler_handle_t* _this, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority);
bool tool_task_scheduler_addTaskSuper(tool_task_scheduler_handle_t* _this, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority);

#ifdef __cplusplus
}
#endif
#endif
/* *****************************************************************************************
 *  End of file
 */
