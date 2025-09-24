#pragma once
#include "IETThread.h"
#include "IExecutionEvent.h"

class SampleThread : public IETThread
{
	public:
		SampleThread();
		~SampleThread();
		void SetData(std::string _data);
		void SetCallback(IExecutionEvent* event);
		bool isRunning = true;

	private: 
		void run() override;
		std::string data = "Default";
		IExecutionEvent* onFinished;
};

