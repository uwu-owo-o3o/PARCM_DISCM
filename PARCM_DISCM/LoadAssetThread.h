#pragma once
#include "IETThread.h"
#include "IExecutionEvent.h"
#include "TextureManager.h"
#include "IWorkerAction.h"

class LoadAssetThread : public IWorkerAction
{
	public:
		LoadAssetThread();
		LoadAssetThread(int _id, IExecutionEvent* callback);
		~LoadAssetThread();

	private:
		int id = 0;
		//void run() override;
		void OnStartTask() override;
		IExecutionEvent* onFinished;

};

