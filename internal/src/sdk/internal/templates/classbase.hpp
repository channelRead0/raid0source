#pragma once
#include "sdk/internal/gameclass/gameclass.hpp"

class ClassBase
{
public:
	bool IsGlobal = false;
	jobject InstanceObject = nullptr;

	void PromoteInstanceObject(JNIEnv* Env);
	void UpdateInstanceObject(jobject UpdatedObject);
	jobject GetInstanceObject() const;

	bool operator!() const {
		return InstanceObject == nullptr;
	}
};