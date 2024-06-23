#include "pch.h"
#include "mousehelper.hpp"

MouseHelper::MouseHelper(JNIEnv* Env)
{
	this->MouseHelperMapper = this->MouseHelperMapper ? this->MouseHelperMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/MouseHelper");
}

MouseHelper::MouseHelper(JNIEnv* Env, jobject InstanceObject)
{
	this->MouseHelperMapper = this->MouseHelperMapper ? this->MouseHelperMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/MouseHelper");
	this->InstanceObject = InstanceObject;
}

Vector2<double> MouseHelper::MouseDelta(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return Vector2<double>
	(
		MouseHelperMapper->GetIntField(Env, InstanceObject, "deltaX"),
		MouseHelperMapper->GetIntField(Env, InstanceObject, "deltaY")
	);
}

MouseHelper::~MouseHelper()
{
}
