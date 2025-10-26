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
std::counting_semaphore<1> mtxC(0);

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
				mtxC.release();
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
			mtxC.acquire();

			IETThread::sleep(500);
			
			std::cout << "C!" << std::endl;
			mtxA.release();
		}
	}
};

std::binary_semaphore output(1);
std::binary_semaphore a2keys(0);

class SemaphoreThread1 : public IETThread {
	private:
		void A1() {
			output.acquire();
			std::cout << "A1\n";
			output.release();
		}

		void A2() {
			output.acquire();
			std::cout << "A2\n";
			output.release();
		}

		void run() override {
			A1();
			a2keys.acquire(); 
			A2();
		}
};

std::binary_semaphore goldkey(1);
int gold = 0;
int user_gold = 0;

class Mine : public IETThread {
	private:
		void Print() {
			std::cout << "Mine add 1 gold\n";
			std::cout << "Total gold: " << gold << "\n";


		}

		void run() {
			while (true) {
				IETThread::sleep(500);
				goldkey.acquire();
				gold++;
				Print();
				goldkey.release();
			}
		}
};

class Miner : public IETThread {
	private:
		void Print() {
			std::cout << "\n";
			std::cout << "Miner mined 1 gold\n";
			std::cout << "Total gold: " << gold << "\n";
			std::cout << "Total user gold: " << user_gold << "\n";

		}

		void run() {
			while (true) {
				if (gold > 0) {
					IETThread::sleep(1000);
					goldkey.acquire();
					gold--;
					user_gold++;
					Print();
					goldkey.release();
				}
			}
		}
};

class SemaphoreThread2 : public IETThread {
private:
	void B1() {
		output.acquire();
		std::cout << "B1\n";
		output.release();
	}

	void B2() {
		output.acquire();
		std::cout << "B2\n";
		output.release();
	}

	void run() override {
		B1();
		a2keys.release();
		B2();
	}
};

void oldMain() {
	float data = 0.0f;
	int turn = 0;
	bool flag[3] = { false, false, false};

	//ThreadA a;
	//ThreadB b;
	//ThreadC c;

	//a.data = &data;
	//b.data = &data;
	////c.data = &data;

	//a.turn = &turn;
	//b.turn = &turn;
	////c.turn = &turn;

	//a.flag = flag;
	//b.flag = flag;
	////c.flag = flag;

	//a.start();
	//b.start();
	//c.start();

	//SemaphoreThread1 thread1;
	//SemaphoreThread2 thread2;

	//thread1.start();
	//thread2.start();

	Mine mine;
	Miner miner;

	mine.start();
	miner.start();

	IETThread::sleep(10000);
}

int main() {
	/*BaseRunner runner;
	runner.run();*/
	oldMain();
	return 0;
}