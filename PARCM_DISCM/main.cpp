#include "iostream"
#include "SampleThread.h"
#include "ThreadHandler.h"
#include "BaseRunner.h"
#include <cstdlib>
#include <vector>
#include <mutex>

#include <semaphore> 

// Max number of permits
std::counting_semaphore<5> my_semaphore(1); // initialized with the starting number of permits/keys

std::counting_semaphore<1> mtxA(1); // for semaphorethread
std::counting_semaphore<1> mtxB(0);

class ThreadA : public IETThread {
	public:
		float* data;
		bool* flag;
		int* turn;
	private:
		void run() override {
			while (true) {
				mtxA.acquire();
				IETThread::sleep(500);
				*data = 50;
				std::cout << *data << std::endl;
				mtxB.release();
			}
		}
};

class ThreadB : public IETThread {
	public:
		float* data;
		bool* flag;
		int* turn;
	private:
		void run() override {
			while (true) {
				mtxB.acquire();

				IETThread::sleep(500);
				*data = 100;
				std::cout << *data << std::endl;
				mtxA.release();
			}
		}
};

class ThreadC : public IETThread {
public:
	float* data;
	bool* flag;
	int* turn;
private:
	void run() override {
		while (true) {
			flag[2] = true;
			*turn = 0;
			while (flag[0] && *turn == 0) {
				//std::cout << "Thread C is waiting." << std::endl;
			}

			IETThread::sleep(500);
			*data = 150;
			std::cout << *data << std::endl;
			flag[2] = false;
		}
	}
};

class SemaphoreThread1 : public IETThread {
	private:
		void run() override {
			std::cout << "1: Trying to acquire semaphore." << std::endl;
			my_semaphore.acquire(); //c++ retrieves 1 by 1 with acquire
			std::cout << "1: A Crit Section\n";
			IETThread::sleep(1000);
			my_semaphore.release();
			std::cout << "1: Released Semaphore \n";
		}
};

class SemaphoreThread2 : public IETThread {
private:
	void run() override {
		IETThread::sleep(100);
		std::cout << "2: Trying to acquire semaphore." << std::endl;
		my_semaphore.acquire(); //c++ retrieves 1 by 1 with acquire
		std::cout << "2: A Crit Section\n";
		my_semaphore.release();
		std::cout << "2: Released Semaphore \n";
	}
};

void oldMain() {
	float data = 0.0f;
	int turn = 0;
	bool flag[3] = { false, false, false};

	ThreadA a;
	ThreadB b;
	ThreadC c;

	a.data = &data;
	b.data = &data;
	//c.data = &data;

	a.turn = &turn;
	b.turn = &turn;
	//c.turn = &turn;

	a.flag = flag;
	b.flag = flag;
	//c.flag = flag;

	a.start();
	b.start();
	//c.start();

	//SemaphoreThread1 thread1;
	//SemaphoreThread2 thread2;

	//thread1.start();
	//thread2.start();

	IETThread::sleep(10000);
}

int main() {
	/*BaseRunner runner;
	runner.run();*/
	oldMain();
	return 0;
}