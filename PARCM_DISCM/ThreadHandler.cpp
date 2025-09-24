#include "ThreadHandler.h"

void ThreadHandler::OnFinishedExecution()
{
	std::cout << "Thread is done." << std::endl;
	this->threadDone = true;
}
