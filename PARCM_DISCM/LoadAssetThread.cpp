#include "LoadAssetThread.h"

LoadAssetThread::LoadAssetThread()
{

}

LoadAssetThread::LoadAssetThread(int _id, IExecutionEvent* callback)
{
	this->id = _id;
	this->onFinished = callback;
}

LoadAssetThread::~LoadAssetThread()
{
}

void LoadAssetThread::run()
{
	TextureManager::getInstance()->loadSingleStreamAsset(this->id);
	this->onFinished->OnFinishedExecution();
	delete this;
}
