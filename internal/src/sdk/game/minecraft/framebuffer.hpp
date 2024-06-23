#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class Framebuffer : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> FramebufferMapper = nullptr;
public:
	Framebuffer(JNIEnv* Env);
	Framebuffer(JNIEnv* Env, int Width, int Height, bool UseDepth);
	Framebuffer(JNIEnv* Env, jobject InstanceObject);

	bool IsValid(JNIEnv* Env) const;
	void DeleteFrameBuffer(JNIEnv* Env) const;
	int FrameBufferTexture(JNIEnv* Env) const;
	void BindFrameBuffer(JNIEnv* Env, bool Bind) const;

	~Framebuffer();
};