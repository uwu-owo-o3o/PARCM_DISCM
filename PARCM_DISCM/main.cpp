#include "iostream"
#include "SampleThread.h"
#include "ThreadHandler.h"
#include "BaseRunner.h"
#include <cstdlib>
#include <vector>
#include <mutex>

std::mutex printMutex;
int x = 99;

void PrintX() {
	std::cout << "X From PrintX: " << x << std::endl;
}
void TestThread(int runTime, int num) {
	auto duration = std::chrono::system_clock::now() + std::chrono::milliseconds(runTime);
	while (std::chrono::system_clock::now() < duration) {
		x = rand() % 11;

		printMutex.lock();
		std::cout << "X From TestThread " << num << ": " << x << std::endl;
		printMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	printMutex.lock();
	std::cout << "Thread " << num << " finished." << std::endl;
	printMutex.unlock();
}	

void oldMain() {
	srand(time(0));
	const int maxThreadCount = 10;
	const int runTime = 10000;

	std::vector <std::thread> threads;

	for (int i = 0; i < maxThreadCount; i++) {
		threads.push_back(std::thread(TestThread, runTime, i));
	}

	for (int i = 0; i < maxThreadCount; i++) {
		threads[i].join();
	}

	std::cout << "End Main." << std::endl;

	ThreadHandler* handler = new ThreadHandler();
	SampleThread* sample = new SampleThread();
	sample->SetData("Sample1");
	sample->SetCallback(handler);
	sample->start();
	do {
		std::cout << "Am waiting..." << std::endl;
	} while (!handler->threadDone);

	delete handler;
}

int main() {
	BaseRunner runner;
	runner.run();
	return 0;
}