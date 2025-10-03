#include "iostream"
#include "SampleThread.h"
#include "ThreadHandler.h"
#include "BaseRunner.h"
#include <cstdlib>
#include <vector>
#include <mutex>

class ThreadA : public IETThread {
	public:
		float* data;
		bool* flag;
		int* turn;
	private:
		void run() override {
			while (true) {
				flag[0] = true;
				*turn = 1;
				while (flag[1] && *turn == 1) {
					//std::cout << "Thread A is waiting." << std::endl;
				}

				IETThread::sleep(500);
				*data = 50;
				std::cout << *data << std::endl;
				flag[0] = false;
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
				flag[1] = true;
				*turn = 2;
				while (flag[2] && *turn == 2) {
					//std::cout << "Thread B is waiting." << std::endl;
				}

				IETThread::sleep(500);
				*data = 100;
				std::cout << *data << std::endl;
				flag[1] = false;
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


class Escoffier : public IETThread {
	public:
		bool* hasFood;
		bool* hasOrdered;
	private:
		void WaitForOrder() {
			while (!*hasOrdered) {
				std::cout << "Escoffier is waiting\n";
			}
		}
		void Cook() {
			std::cout << "Escoffier cooks\n" << std::endl;
			*hasFood = true;
		}
		void Enter() {
			std::cout << "Escoffier enters\n" << std::endl;
		}
		void run() override {
			Enter();
			WaitForOrder();
			Cook();
		}
};

class Furina : public IETThread {
	public:
		bool* hasFood;
		bool* hasOrdered;
	private:
		void WaitForFood() {
			while (!*hasFood) {
				std::cout << "Furina is waiting\n";
			}
		}
		void Eats() {
			*hasFood = false;
			std::cout << "Furina eats\n" << std::endl;
		}
		void Enter() {
			std::cout << "Furina enters\n" << std::endl;
		}
		void Order() {
			std::cout << "Furina orders\n" << std::endl;
			*hasOrdered = true;
		}

		void run() override {
			Enter();
			WaitForFood();
			Eats();
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
	c.data = &data;

	a.turn = &turn;
	b.turn = &turn;
	c.turn = &turn;

	a.flag = flag;
	b.flag = flag;
	c.flag = flag;

	a.start();
	b.start();
	c.start();
	//bool hasFood = false;
	//bool hasOrdered =  false;
	//Escoffier esc;
	//Furina furi;

	//esc.hasFood = &hasFood;
	//furi.hasFood = &hasFood;
	//esc.hasOrdered = &hasOrdered;
	//furi.hasOrdered = &hasOrdered;

	//furi.start();
	//esc.start();


	IETThread::sleep(10000);
}

int main() {
	/*BaseRunner runner;
	runner.run();*/
	oldMain();
	return 0;
}