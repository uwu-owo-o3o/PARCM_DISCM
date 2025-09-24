#pragma once
#include "IExecutionEvent.h"
#include "iostream"

class ThreadHandler : public IExecutionEvent
{
	public:
		bool threadDone = false;
		void OnFinishedExecution() override;
};

