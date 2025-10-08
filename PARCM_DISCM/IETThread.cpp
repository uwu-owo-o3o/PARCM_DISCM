#include "IETThread.h"

IETThread::IETThread()
{
}

IETThread::~IETThread()
{
}

void IETThread::start()
{
	if (this == nullptr) {
		std::cout << " this is null!" << std::endl;
		return;
	}
	else {
		std::cout << "this is NOT null!" << std::endl;
	}
	std::thread(&IETThread::run, this).detach();
}

void IETThread::sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
