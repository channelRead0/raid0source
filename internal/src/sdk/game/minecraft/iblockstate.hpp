#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class IBlockState : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> IBlockStateMapper = nullptr;
	//net/minecraft/client/gui/inventory/GuiInventory
public:
	IBlockState(JNIEnv* Env);
	IBlockState(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<Block> GetBlockPtr(JNIEnv* Env) const;

	~IBlockState();
};