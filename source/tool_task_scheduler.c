/* *****************************************************************************************
 *    File Name   :tool_task_scheduler.c
 *    Create Date :2018-09-09
 *    Modufy Date :2021-03-28
 *    Information :
 */

#include "tool_task_scheduler.h"
#include "tool_fifo.h"

/* *****************************************************************************************
 *  Typedef Struct
 */

/* *****************************************************************************************
 *  Public Function 
 */
 
/*-------------------------------------
 *  tool_ts_initialze 
 */ 
static bool tool_ts_initialze(tool_ts_handle_t* handle, const tool_ts_config_t *config) {
  tool_fifo_config_t fifo_cfg;
  
  fifo_cfg.buffer = &config->prtorityHigh.eventBuffer[0];
  fifo_cfg.count = config->prtorityHigh.bufferQuantity;
  fifo_cfg.itemSize = sizeof(tool_ts_event_t);
  tool_fifo_init(&handle->fifo.high, &fifo_cfg);
  
  fifo_cfg.buffer = &config->prtorityNormal.eventBuffer[0];
  fifo_cfg.count = config->prtorityNormal.bufferQuantity;
  fifo_cfg.itemSize = sizeof(tool_ts_event_t);
  tool_fifo_init(&handle->fifo.normal, &fifo_cfg);
  
  fifo_cfg.buffer = &config->prtorityLow.eventBuffer[0];
  fifo_cfg.count = config->prtorityLow.bufferQuantity;
  fifo_cfg.itemSize = sizeof(tool_ts_event_t);
  tool_fifo_init(&handle->fifo.low, &fifo_cfg);
  
  handle->flag = 0;
  return true;
}

/*-------------------------------------
 *  tool_ts_execute 
 */ 
static bool tool_ts_execute(tool_ts_handle_t* handle) {
  tool_ts_event_t task = {0x00000000, 0x00000000};

  if(handle->flag)
    return false;
  
  handle->flag = 1;
  
  while(handle->flag) {
    if (tool_fifo_pop(&handle->fifo.high, &task)) 
      task.execute(task.attachment);
    else if (tool_fifo_pop(&handle->fifo.normal, &task))
      task.execute(task.attachment);
    else if (tool_fifo_pop(&handle->fifo.low, &task))
      task.execute(task.attachment);
    else
      break;
  }
  
  handle->flag = 0;
  return true;
}

/*-------------------------------------
 *  tool_ts_breakExecute 
 */ 
static bool tool_ts_breakExecute(tool_ts_handle_t* handle) {
  if (!handle->flag)
    return false;

  handle->flag = 0;
  return true;
}

/*-------------------------------------
 *  tool_ts_addTask 
 */ 
static bool tool_ts_addTask(tool_ts_handle_t* handle, tool_ts_execute_t execute, void* attachment, tool_ts_prtority prtority) {
  if(!execute)
    return false;
  
  tool_ts_event_t task = {
    .execute = execute,
    .attachment = attachment
  };
  
  switch (prtority) {
    case tool_ts_prtority_low:
      return tool_fifo_insert(&handle->fifo.low, &task);
    case tool_ts_prtority_normal:
      return tool_fifo_insert(&handle->fifo.normal, &task);
    case tool_ts_prtority_high:
      return tool_fifo_insert(&handle->fifo.high, &task);
    default:
      return false;
  }
}

/*-------------------------------------
 *  tool_ts_addTaskSuper 
 */ 
static bool tool_ts_addTaskSuper(tool_ts_handle_t* handle, tool_ts_execute_t execute, void* attachment, tool_ts_prtority prtority) {
  if(!execute)
    return false;
  
  tool_ts_event_t task = {
    .execute = execute,
    .attachment = attachment
  };
    
  switch (prtority) {
    case tool_ts_prtority_low:
      return tool_fifo_insertTail(&handle->fifo.low, &task);
    case tool_ts_prtority_normal:
      return tool_fifo_insertTail(&handle->fifo.normal, &task);
    case tool_ts_prtority_high:
      return tool_fifo_insertTail(&handle->fifo.high, &task);
    default:
      return false;
  }
}

/* *****************************************************************************************
 *  Link API 
 */
const tool_ts_api_t tool_ts_api = {
  .initialze    = tool_ts_initialze,
  .execute      = tool_ts_execute,
  .breakExecute = tool_ts_breakExecute,
  .addTask      = tool_ts_addTask,
  .addTaskSuper = tool_ts_addTaskSuper
};

/* *****************************************************************************************
 *  End of file
 */
