#include "pch.h"
#include "scaledresolution.hpp"

ScaledResolution::ScaledResolution(JNIEnv* Env)
{
	this->ScaledResolutionMapper = this->ScaledResolutionMapper ? this->ScaledResolutionMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/ScaledResolution");
}

ScaledResolution::ScaledResolution(JNIEnv* Env, jobject InstanceObject)
{
	this->ScaledResolutionMapper = this->ScaledResolutionMapper ? this->ScaledResolutionMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/ScaledResolution");
	this->InstanceObject = InstanceObject;
}

ScaledResolution::ScaledResolution(JNIEnv* Env, jobject InstanceObject, bool Minecraft)
{
	this->ScaledResolutionMapper = this->ScaledResolutionMapper ? this->ScaledResolutionMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/gui/ScaledResolution");
	const auto Initializer = Env->GetMethodID(ScaledResolutionMapper->GetClass(), "<init>", "(Lnet/minecraft/client/Minecraft;)V");
	const auto Instance = Env->NewObject(ScaledResolutionMapper->GetClass(), Initializer, InstanceObject);

	std::cout << "Scaled Resolution: " << Instance << std::endl;

	this->InstanceObject = Instance;

}

int ScaledResolution::GetScaledHeight(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return ScaledResolutionMapper->CallIntMethod(Env, InstanceObject, "getScaledHeight");
}

int ScaledResolution::GetScaledWidth(JNIEnv* Env)
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return ScaledResolutionMapper->CallIntMethod(Env, InstanceObject, "getScaledWidth");
}

ScaledResolution::~ScaledResolution()
{
}
