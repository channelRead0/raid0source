#include "pch.h"
#include "queue.hpp"
#include <sdk/internal/register/envregister.hpp>

Queue::Queue(JNIEnv* Env)
{
	this->QueueMapper = this->QueueMapper ? this->QueueMapper : std::make_shared<GameClass>(Env, "java/util/Queue");
}

Queue::Queue(JNIEnv* Env, jobject InstanceObject)
{
	this->QueueMapper = this->QueueMapper ? this->QueueMapper : std::make_shared<GameClass>(Env, "java/util/Queue");
	this->InstanceObject = InstanceObject;
}

bool Queue::Add(JNIEnv* Env, jobject Object)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return QueueMapper->CallBooleanMethod(Env, InstanceObject, "add", Object);
}

Queue::~Queue()
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
