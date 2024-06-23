//java.util.concurrent.locks.ReentrantReadWriteLock
#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class Queue : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> QueueMapper = nullptr;
public:
	Queue(JNIEnv* Env);
	Queue(JNIEnv* Env, jobject InstanceObject);

	bool Add(JNIEnv* Env, jobject Object);

	~Queue();
};