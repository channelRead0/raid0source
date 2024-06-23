#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class ChannelHandlerContext : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ChannelHandlerContextMapper = nullptr;
public:
	ChannelHandlerContext(JNIEnv* Env);
	ChannelHandlerContext(JNIEnv* Env, jobject InstanceObject);

	~ChannelHandlerContext();
};
