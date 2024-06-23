#include "pch.h"
#include "list.hpp"
#include <sdk/internal/register/envregister.hpp>

List::List(JNIEnv* Env)
{
	this->ListMapper = this->ListMapper ? this->ListMapper : std::make_shared<GameClass>(Env, "java/util/List");
}

List::List(JNIEnv* Env, jobject InstanceObject)
{
	this->ListMapper = this->ListMapper ? this->ListMapper : std::make_shared<GameClass>(Env, "java/util/List");
	this->InstanceObject = InstanceObject;
}

int List::Size(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return ListMapper->CallIntMethod(Env, InstanceObject, "size");
}

List::~List()
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
