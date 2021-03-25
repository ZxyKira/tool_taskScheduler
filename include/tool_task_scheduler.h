//*****************************************************************************
//*			File Name		:tool_task_scheduler.h
//*			Version			:V1.1
//*			Create Date	:2018-09-09
//*			Modufy Date	:2019-06-10
//*			Information :
//*****************************************************************************
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
typedef void (*tool_ts_Callback)(void);
typedef void (*tool_ts_Task)(void* args);

//-------------------------------------
//	Struct tool_ts_event_t 
//------------------------------------- 
typedef struct{
	tool_ts_Task task;
	void*	args;
}tool_ts_event_t;

//-------------------------------------
//	Struct tool_ts_handle_t
//------------------------------------- 
typedef struct{
	uint32_t mem[0x10];
}tool_ts_handle_t;

//-------------------------------------
//	Struct tool_ts_handle_t
//------------------------------------- 
typedef enum{
	tool_ts_PrtorityLow,
	tool_ts_PrtorityNormal,
	tool_ts_PrtorityHigh
}tool_ts_Task_Prtority;

//-------------------------------------
//	Struct tool_ts_Config_T
//------------------------------------- 
typedef struct{
	struct{
		tool_ts_event_t *pEventBuffer;
		uint16_t bufferQuantity;
	}PrtorityLow;
	struct{
		tool_ts_event_t *pEventBuffer;
		uint16_t bufferQuantity;
	}PrtorityNormal;
	struct{
		tool_ts_event_t *pEventBuffer;
		uint16_t bufferQuantity;
	}PrtorityHigh;
	tool_ts_Callback idleCallback;

}tool_ts_Config_T;

//-------------------------------------
//	Struct tool_ts_API_T
//------------------------------------- 
typedef struct _tool_ts_api_t{
	bool(*initialze)(tool_ts_handle_t *handle, const tool_ts_Config_T *config);
	bool(*start)(tool_ts_handle_t *handle);
	bool(*stop)(tool_ts_handle_t *handle);
	bool(*addTask)(tool_ts_handle_t *handle, tool_ts_event_t task, tool_ts_Task_Prtority prtority);
	bool(*addTaskSuper)(tool_ts_handle_t *handle, tool_ts_event_t task, tool_ts_Task_Prtority prtority);	
}tool_ts_api_t;

extern const tool_ts_api_t tool_ts_api;

#ifdef __cplusplus
}
#endif
#endif
//*****************************************************************************
//*	End of file
//*****************************************************************************

