#pragma once

#include "player.hpp"
#include "slot.hpp"
#include "globals/structures/axisalignedbb_t.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class AxisAlignedBoundingBox : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> AxisAlignedBoundingBoxMapper = nullptr;
public:
	AxisAlignedBoundingBox(JNIEnv* Env);
	AxisAlignedBoundingBox(JNIEnv* Env, jobject InstanceObject);

	jobject NewObject(JNIEnv* Env, const AxisAlignedBB_T<double>& Bounds);
	AxisAlignedBB_T<double> GetAxisAlignedBoundingBoxNative(JNIEnv* Env) const;


	~AxisAlignedBoundingBox();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class AxisAlignedBoundingBox1_7 : public AxisAlignedBoundingBox
{
public:
};

class AxisAlignedBoundingBox1_8 : public AxisAlignedBoundingBox
{
public:
};