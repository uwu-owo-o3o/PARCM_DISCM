#include "SampleThread.h"

SampleThread::SampleThread() : IETThread()
{
}

SampleThread::~SampleThread()
{
}

void SampleThread::SetData(std::string _data)
{
	data = _data;
}

void SampleThread::SetCallback(IExecutionEvent* event)
{
	this->onFinished = event;
}

void SampleThread::run()
{
	std::cout << "Printing Data: " << data << std::endl;
	this->isRunning = false;
	this->onFinished->OnFinishedExecution();
	delete this;
}
