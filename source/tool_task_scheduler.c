//*****************************************************************************
//*			File Name		:TaskScheduler.c
//*			Version			:V1.1
//*			Create Date	:2018-09-09
//*			Modufy Date	:2019-06-10
//*			Information :
//*****************************************************************************
#include "tool_task_scheduler.h"
#include "tool_fifo.h"

//*****************************************************************************
//*	Typedef Struct
//*****************************************************************************

//-------------------------------------
//	Struct TS_Handler_Struct_T
//------------------------------------- 
typedef struct{
	struct{
		tool_fifo_t high;
		tool_fifo_t normal;
		tool_fifo_t low;
	}Fifo;
	struct{
		TS_Callback Idle;
		TS_Callback Reserved;
	}Callback;
	struct{
		uint16_t reserved2 	:16;
		uint8_t	 start 			:8;
		uint8_t	 reserved 	:8;
	}info;
}TS_Handler_Entity_T;

#define TS_tool_fifo_t tool_fifo_t

//-------------------------------------
//	TS_Idle_Callback
//------------------------------------- 
static void TS_Idle_Callback(void) {
	
}

//*****************************************************************************
//*	Public Function 
//*****************************************************************************
static bool TS_Initialze(ts_handle_t *pHandler, const TS_Config_T *config) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;
	tool_fifo_config_t fifo_cfg;
		
	if (config->idleCallback == 0x00000000)
		entity->Callback.Idle = TS_Idle_Callback;
	else
		entity->Callback.Idle = config->idleCallback;
	
  fifo_cfg.buffer = &config->PrtorityHigh.pEventBuffer[0];
	fifo_cfg.count = config->PrtorityHigh.bufferQuantity;
	fifo_cfg.itemSize = sizeof(TS_Event_T);
	tool_fifo_api.init(&entity->Fifo.high, &fifo_cfg);
	
  fifo_cfg.buffer = &config->PrtorityNormal.pEventBuffer[0];
	fifo_cfg.count = config->PrtorityNormal.bufferQuantity;
	fifo_cfg.itemSize = sizeof(TS_Event_T);
	tool_fifo_api.init(&entity->Fifo.normal, &fifo_cfg);
	
  fifo_cfg.buffer = &config->PrtorityLow.pEventBuffer[0];
	fifo_cfg.count = config->PrtorityLow.bufferQuantity;
	fifo_cfg.itemSize = sizeof(TS_Event_T);
	tool_fifo_api.init(&entity->Fifo.low, &fifo_cfg);

	entity->info.start = 0;
	return true;
}

static bool TS_Start(ts_handle_t *pHandler) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;
	TS_Event_T task = {0x00000000, 0x00000000};

	if (entity->info.start != 0)
		return false;

	entity->info.start = 1;

	while (entity->info.start != 0) {
		if (tool_fifo_api.pop(&entity->Fifo.high, &task)) 
			task.task(task.args);
		else if (tool_fifo_api.pop(&entity->Fifo.normal, &task))
			task.task(task.args);
		else if (tool_fifo_api.pop(&entity->Fifo.low, &task))
			task.task(task.args);
		else
			entity->Callback.Idle();
	}
	return true;
}

static bool TS_Stop(ts_handle_t *pHandler) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;
	if (entity->info.start == 0)
		return false;

	entity->info.start = 0;
	return true;
}


static bool TS_AddTask(ts_handle_t *pHandler, TS_Event_T task, TS_Task_Prtority prtority) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;

	switch (prtority) {
		case TS_PrtorityLow:
			return tool_fifo_api.insert(&entity->Fifo.low, &task);
		case TS_PrtorityNormal:
			return tool_fifo_api.insert(&entity->Fifo.normal, &task);
		case TS_PrtorityHigh:
			return tool_fifo_api.insert(&entity->Fifo.high, &task);
		default:
			return false;
	}
}

static bool TS_AddTaskSuper(ts_handle_t *pHandler, TS_Event_T task, TS_Task_Prtority prtority) {
	TS_Handler_Entity_T *entity = (TS_Handler_Entity_T*)pHandler;

	switch (prtority) {
		case TS_PrtorityLow:
			return tool_fifo_api.insertTail(&entity->Fifo.low, &task);
		case TS_PrtorityNormal:
			return tool_fifo_api.insertTail(&entity->Fifo.normal, &task);
		case TS_PrtorityHigh:
			return tool_fifo_api.insertTail(&entity->Fifo.high, &task);
		default:
			return false;
	}
}



const TS_API_T TS_API = {
	.initialze = TS_Initialze,
	.start = TS_Start,
	.stop = TS_Stop,
	.addTask = TS_AddTask,
	.addTaskSuper = TS_AddTaskSuper
};

//*****************************************************************************
//*	End of file
//*****************************************************************************

