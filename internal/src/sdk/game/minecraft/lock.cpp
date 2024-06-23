#include "pch.h"
#include "lock.hpp"
#include <sdk/internal/register/envregister.hpp>

Lock::Lock(JNIEnv* Env)
{
	this->LockMapper = this->LockMapper ? this->LockMapper : std::make_shared<GameClass>(Env, "java.util.concurrent.locks.Lock");
}

Lock::Lock(JNIEnv* Env, jobject InstanceObject)
{
	this->LockMapper = this->LockMapper ? this->LockMapper : std::make_shared<GameClass>(Env, "java.util.concurrent.locks.Lock");
	this->InstanceObject = InstanceObject;
}

void Lock::LockT(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	LockMapper->CallVoidMethod(Env, InstanceObject, "unlock");
}

void Lock::UnlockT(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	LockMapper->CallVoidMethod(Env, InstanceObject, "lock");
}

Lock::~Lock()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;
}


ReentrantReadWriteLock::ReentrantReadWriteLock(JNIEnv* Env)
{
	this->ReentrantReadWriteLockMapper = this->ReentrantReadWriteLockMapper ? this->ReentrantReadWriteLockMapper : std::make_shared<GameClass>(Env, "java.util.concurrent.locks.ReentrantReadWriteLock");
}

ReentrantReadWriteLock::ReentrantReadWriteLock(JNIEnv* Env, jobject InstanceObject)
{
	this->ReentrantReadWriteLockMapper = this->ReentrantReadWriteLockMapper ? this->ReentrantReadWriteLockMapper : std::make_shared<GameClass>(Env, "java.util.concurrent.locks.ReentrantReadWriteLock");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<Lock> ReentrantReadWriteLock::WriteLock(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Lock>(Env, jobject());
	}

	if (const auto WriteLockObject = ReentrantReadWriteLockMapper->CallObjectMethod(Env, InstanceObject, "writeLock")) {
		return std::make_shared<Lock>(Env, WriteLockObject);
	}

	return std::make_shared<Lock>(Env, jobject());
}

std::shared_ptr<Lock> ReentrantReadWriteLock::ReadLock(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Lock>(Env, jobject());
	}

	if (const auto ReadLockObject = ReentrantReadWriteLockMapper->CallObjectMethod(Env, InstanceObject, "readLock")) {
		return std::make_shared<Lock>(Env, ReadLockObject);
	}

	return std::make_shared<Lock>(Env, jobject());
}

ReentrantReadWriteLock::~ReentrantReadWriteLock()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;
}
