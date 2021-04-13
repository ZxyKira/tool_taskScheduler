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
 *  tool_task_scheduler_initialze 
 */ 
bool tool_task_scheduler_initialze(tool_task_scheduler_handle_t* _this, const tool_task_scheduler_config_t *config) {
  tool_fifo_config_t fifo_cfg;
  
  fifo_cfg.buffer = &config->prtorityHigh.eventBuffer[0];
  fifo_cfg.count = config->prtorityHigh.bufferQuantity;
  fifo_cfg.itemSize = sizeof(tool_task_scheduler_event_t);
  tool_fifo_init(&_this->fifo.high, &fifo_cfg);
  
  fifo_cfg.buffer = &config->prtorityNormal.eventBuffer[0];
  fifo_cfg.count = config->prtorityNormal.bufferQuantity;
  fifo_cfg.itemSize = sizeof(tool_task_scheduler_event_t);
  tool_fifo_init(&_this->fifo.normal, &fifo_cfg);
  
  fifo_cfg.buffer = &config->prtorityLow.eventBuffer[0];
  fifo_cfg.count = config->prtorityLow.bufferQuantity;
  fifo_cfg.itemSize = sizeof(tool_task_scheduler_event_t);
  tool_fifo_init(&_this->fifo.low, &fifo_cfg);
  
  _this->flag = 0;
  return true;
}

/*-------------------------------------
 *  tool_task_scheduler_execute 
 */ 
bool tool_task_scheduler_execute(tool_task_scheduler_handle_t* _this) {
  tool_task_scheduler_event_t task = {0x00000000, 0x00000000};

  if(_this->flag)
    return false;
  
  _this->flag = 1;
  
  while(_this->flag) {
    if (tool_fifo_pop(&_this->fifo.high, &task)) 
      task.execute(task.attachment);
    else if (tool_fifo_pop(&_this->fifo.normal, &task))
      task.execute(task.attachment);
    else if (tool_fifo_pop(&_this->fifo.low, &task))
      task.execute(task.attachment);
    else
      break;
  }
  
  _this->flag = 0;
  return true;
}

/*-------------------------------------
 *  tool_task_scheduler_breakExecute 
 */ 
bool tool_task_scheduler_breakExecute(tool_task_scheduler_handle_t* _this) {
  if (!_this->flag)
    return false;

  _this->flag = 0;
  return true;
}

/*-------------------------------------
 *  tool_task_scheduler_addTask 
 */ 
bool tool_task_scheduler_addTask(tool_task_scheduler_handle_t* _this, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority) {
  if(!execute)
    return false;
  
  tool_task_scheduler_event_t task = {
    .execute = execute,
    .attachment = attachment
  };
  
  switch (prtority) {
    case tool_task_scheduler_prtority_low:
      return tool_fifo_insert(&_this->fifo.low, &task);
    case tool_task_scheduler_prtority_normal:
      return tool_fifo_insert(&_this->fifo.normal, &task);
    case tool_task_scheduler_prtority_high:
      return tool_fifo_insert(&_this->fifo.high, &task);
    default:
      return false;
  }
}

/*-------------------------------------
 *  tool_task_scheduler_addTaskSuper 
 */ 
bool tool_task_scheduler_addTaskSuper(tool_task_scheduler_handle_t* _this, tool_task_scheduler_execute_t execute, void* attachment, tool_task_scheduler_prtority prtority) {
  if(!execute)
    return false;
  
  tool_task_scheduler_event_t task = {
    .execute = execute,
    .attachment = attachment
  };
    
  switch (prtority) {
    case tool_task_scheduler_prtority_low:
      return tool_fifo_insertTail(&_this->fifo.low, &task);
    case tool_task_scheduler_prtority_normal:
      return tool_fifo_insertTail(&_this->fifo.normal, &task);
    case tool_task_scheduler_prtority_high:
      return tool_fifo_insertTail(&_this->fifo.high, &task);
    default:
      return false;
  }
}

/* *****************************************************************************************
 *  Link API 
 */
const tool_task_scheduler_api_t tool_task_scheduler_api = {
  .initialze    = tool_task_scheduler_initialze,
  .execute      = tool_task_scheduler_execute,
  .breakExecute = tool_task_scheduler_breakExecute,
  .addTask      = tool_task_scheduler_addTask,
  .addTaskSuper = tool_task_scheduler_addTaskSuper
};

/* *****************************************************************************************
 *  End of file
 */
