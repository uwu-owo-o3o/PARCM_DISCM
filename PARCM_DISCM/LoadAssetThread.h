#pragma once
#include "IETThread.h"
#include "IExecutionEvent.h"
#include "TextureManager.h"

class LoadAssetThread : public IETThread
{
	public:
		LoadAssetThread();
		LoadAssetThread(int _id, IExecutionEvent* callback);
		~LoadAssetThread();

	private:
		int id = 0;
		void run() override;
		IExecutionEvent* onFinished;

};

