#include "pch.h"
#include "activerenderinfo.hpp"
#include <ppl.h>
#include <concurrent_vector.h>

ActiveRenderInfo::ActiveRenderInfo(JNIEnv* Env)
{
	this->ActiveRenderInfoMapper = this->ActiveRenderInfoMapper ? this->ActiveRenderInfoMapper : std::make_shared<GameClass>(Env, "net/minecraft/client/renderer/ActiveRenderInfo");
	this->FloatBufferMapper = this->FloatBufferMapper ? this->FloatBufferMapper : std::make_shared<GameClass>(Env, "java/nio/FloatBuffer");
}

std::vector<float> ActiveRenderInfo::GetModelView(JNIEnv* Env) const
{
	const auto ActiveRenderInfoClass = ActiveRenderInfoMapper;

	if (!ActiveRenderInfoClass) {
		return {};
	}

	const auto ModelView_FieldID = ActiveRenderInfoClass->GetFieldID("MODELVIEW");

	if (!ModelView_FieldID)
	{
		ActiveRenderInfoClass->RefreshClass(Env);
		return{};
	}
	const auto ModelViewObject = Env->GetStaticObjectField(ActiveRenderInfoClass->GetClass(), ModelView_FieldID);

	if (!ModelViewObject) {
		return {};
	}

	const auto FloatBufferClass = FloatBufferMapper;
	const auto Get_MethodID = Env->GetMethodID(FloatBufferClass->GetClass(), "get", "(I)F");

	std::vector<float> ModelViewVector;
	for (int Index = 0; Index < 16; Index++)
	{
		float ModelViewAtIndex = Env->CallFloatMethod(ModelViewObject, Get_MethodID, Index);
		ModelViewVector.push_back(ModelViewAtIndex);
	}

	if (ModelViewObject) {
		Env->DeleteLocalRef(ModelViewObject);
	}

	return ModelViewVector;
}

concurrency::concurrent_vector<float> ActiveRenderInfo::GetModelViewVectorSafe(JNIEnv* Env) const
{
	const auto ActiveRenderInfoClass = ActiveRenderInfoMapper;

	if (!ActiveRenderInfoClass) {
		return concurrency::concurrent_vector<float>();
	}

	const auto ModelView_FieldID = ActiveRenderInfoClass->GetFieldID("MODELVIEW");

	if (!ModelView_FieldID)
	{
		ActiveRenderInfoClass->RefreshClass(Env);
		return concurrency::concurrent_vector<float>();
	}
	const auto ModelViewObject = Env->GetStaticObjectField(ActiveRenderInfoClass->GetClass(), ModelView_FieldID);

	if (!ModelViewObject) {
		return concurrency::concurrent_vector<float>();
	}

	const auto FloatBufferClass = FloatBufferMapper;
	const auto Get_MethodID = Env->GetMethodID(FloatBufferClass->GetClass(), "get", "(I)F");

	concurrency::concurrent_vector<float> ModelViewVector;
	for (int Index = 0; Index < 16; Index++)
	{
		float ModelViewAtIndex = Env->CallFloatMethod(ModelViewObject, Get_MethodID, Index);
		ModelViewVector.push_back(ModelViewAtIndex);
	}

	if (ModelViewObject) {
		Env->DeleteLocalRef(ModelViewObject);
	}

	return ModelViewVector;
}

concurrency::concurrent_vector<float> ActiveRenderInfo::GetProjectionVectorSafe(JNIEnv* Env) const
{
	const auto ActiveRenderInfoClass = ActiveRenderInfoMapper;

	if (!ActiveRenderInfoClass) {
		return concurrency::concurrent_vector<float>();
	}

	const auto ModelView_FieldID = ActiveRenderInfoClass->GetFieldID("PROJECTION");

	if (!ModelView_FieldID)
	{
		ActiveRenderInfoClass->RefreshClass(Env);
		return concurrency::concurrent_vector<float>();
	}

	const auto ModelViewObject = Env->GetStaticObjectField(ActiveRenderInfoClass->GetClass(), ModelView_FieldID);

	if (!ModelViewObject) {
		return concurrency::concurrent_vector<float>();
	}

	const auto FloatBufferClass = FloatBufferMapper;
	const auto Get_MethodID = Env->GetMethodID(FloatBufferClass->GetClass(), "get", "(I)F");

	concurrency::concurrent_vector<float> ModelViewVector;
	for (int Index = 0; Index < 16; Index++)
	{
		float ModelViewAtIndex = Env->CallFloatMethod(ModelViewObject, Get_MethodID, Index);
		ModelViewVector.push_back(ModelViewAtIndex);
	}

	if (ModelViewObject) {
		Env->DeleteLocalRef(ModelViewObject);
	}

	return ModelViewVector;
}

std::vector<float> ActiveRenderInfo::GetProjection(JNIEnv* Env) const
{
	const auto ActiveRenderInfoClass = ActiveRenderInfoMapper;

	if (!ActiveRenderInfoClass) {
		return {};
	}

	const auto ModelView_FieldID = ActiveRenderInfoClass->GetFieldID("PROJECTION");

	if (!ModelView_FieldID)
	{
		ActiveRenderInfoClass->RefreshClass(Env);
		return{};
	}

	const auto ModelViewObject = Env->GetStaticObjectField(ActiveRenderInfoClass->GetClass(), ModelView_FieldID);

	if (!ModelViewObject) {
		return {};
	}

	const auto FloatBufferClass = FloatBufferMapper;
	const auto Get_MethodID = Env->GetMethodID(FloatBufferClass->GetClass(), "get", "(I)F");

	std::vector<float> ModelViewVector;
	for (int Index = 0; Index < 16; Index++)
	{
		float ModelViewAtIndex = Env->CallFloatMethod(ModelViewObject, Get_MethodID, Index);
		ModelViewVector.push_back(ModelViewAtIndex);
	}

	if (ModelViewObject) {
		Env->DeleteLocalRef(ModelViewObject);
	}

	return ModelViewVector;
}

ActiveRenderInfo::~ActiveRenderInfo()
{
}
