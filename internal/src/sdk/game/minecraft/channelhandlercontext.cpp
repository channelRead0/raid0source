#include "pch.h"
#include "channelhandlercontext.hpp"

#include "sdk/internal/register/envregister.hpp"

ChannelHandlerContext::ChannelHandlerContext(JNIEnv* Env)
{
	this->ChannelHandlerContextMapper = this->ChannelHandlerContextMapper ? this->ChannelHandlerContextMapper : std::make_shared<GameClass>(Env, "io/netty/channel/ChannelHandlerContext");
}

ChannelHandlerContext::ChannelHandlerContext(JNIEnv* Env, jobject InstanceObject)
{
	this->ChannelHandlerContextMapper = this->ChannelHandlerContextMapper ? this->ChannelHandlerContextMapper : std::make_shared<GameClass>(Env, "io/netty/channel/ChannelHandlerContext");
	this->InstanceObject = InstanceObject;
}

ChannelHandlerContext::~ChannelHandlerContext()
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
		//Env->DeleteGlobalRef(InstanceObject);
	}

	InstanceObject = nullptr;
}
