//java.util.concurrent.locks.ReentrantReadWriteLock
#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class Lock : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> LockMapper = nullptr;
public:
	Lock(JNIEnv* Env);
	Lock(JNIEnv* Env, jobject InstanceObject);

	void LockT(JNIEnv* Env);
	void UnlockT(JNIEnv* Env);

	~Lock();
};

class ReentrantReadWriteLock : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ReentrantReadWriteLockMapper = nullptr;
public:
	ReentrantReadWriteLock(JNIEnv* Env);
	ReentrantReadWriteLock(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<Lock> WriteLock(JNIEnv* Env);
	std::shared_ptr<Lock> ReadLock(JNIEnv* Env);

	~ReentrantReadWriteLock();
};
