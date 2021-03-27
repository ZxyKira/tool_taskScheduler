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
//	Typedef 
//------------------------------------- 
typedef void (*tool_ts_execute)(void* args);

//-------------------------------------
//	Struct tool_ts_execute_t 
//------------------------------------- 
typedef struct{
	tool_ts_execute execute;
	void*	attachment;
}tool_ts_execute_t;

//-------------------------------------
//	Struct tool_ts_handle_t
//------------------------------------- 
typedef struct _tool_ts_handle_t{
	struct{
		tool_fifo_t high;
		tool_fifo_t normal;
		tool_fifo_t low;
	}fifo;
	struct{
		tool_ts_execute_t idle;
		tool_ts_execute_t reserved;
	}execute;
  uint32_t flag;
	tool_ts_execute_t idleCallback;
}tool_ts_handle_t;

//-------------------------------------
//	Struct tool_ts_handle_t
//------------------------------------- 
typedef enum{
	tool_ts_prtority_low,
	tool_ts_prtority_normal,
	tool_ts_prtority_high
}tool_ts_prtority;

//-------------------------------------
//	Struct tool_ts_config_buffer_t
//------------------------------------- 
typedef struct _tool_ts_config_buffer_t{
	tool_ts_execute_t *eventBuffer;
	uint16_t bufferQuantity;
}tool_ts_config_buffer_t;

//-------------------------------------
//	Struct tool_ts_config_t
//------------------------------------- 
typedef struct{
	tool_ts_config_buffer_t prtorityLow;
	tool_ts_config_buffer_t prtorityNormal;
	tool_ts_config_buffer_t prtorityHigh;
	tool_ts_execute_t idleCallback;
}tool_ts_config_t;

//-------------------------------------
//	Struct tool_ts_API_T
//------------------------------------- 
typedef struct _tool_ts_api_t{
	bool(*initialze)    (tool_ts_handle_t *handle, const tool_ts_config_t *config);
	bool(*start)        (tool_ts_handle_t *handle);
	bool(*stop)         (tool_ts_handle_t *handle);
	bool(*addTask)      (tool_ts_handle_t *handle, tool_ts_execute_t task, tool_ts_prtority prtority);
	bool(*addTaskSuper) (tool_ts_handle_t *handle, tool_ts_execute_t task, tool_ts_prtority prtority);	
}tool_ts_api_t;

extern const tool_ts_api_t tool_ts_api;

#ifdef __cplusplus
}
#endif
#endif
/* *****************************************************************************************
 *	End of file
 */
