#pragma once

class IWorkerAction {
	public:
		//To replace threads run function 
		virtual void OnStartTask() = 0;
};