#include "pch.h"
#include "framebuffer.hpp"

Framebuffer::Framebuffer(JNIEnv* Env)
{
	this->FramebufferMapper = this->FramebufferMapper ? this->FramebufferMapper : std::make_shared<GameClass>(Env, "net.minecraft.client.shader.Framebuffer");
}

Framebuffer::Framebuffer(JNIEnv* Env, int Width, int Height, bool UseDepth)
{
	this->FramebufferMapper = this->FramebufferMapper ? this->FramebufferMapper : std::make_shared<GameClass>(Env, "net.minecraft.client.shader.Framebuffer");

	const auto Initializer = Env->GetMethodID(FramebufferMapper->GetClass(), "<init>", "(IIZ)V");
	const auto Instance = Env->NewObject(FramebufferMapper->GetClass(), Initializer, Width, Height, UseDepth);

	std::cout << "Framebuffer: " << Instance << std::endl;

	this->InstanceObject = Instance;
}

Framebuffer::Framebuffer(JNIEnv* Env, jobject InstanceObject)
{
	this->FramebufferMapper = this->FramebufferMapper ? this->FramebufferMapper : std::make_shared<GameClass>(Env, "net.minecraft.client.shader.Framebuffer");
	this->InstanceObject = InstanceObject;
}

bool Framebuffer::IsValid(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return false;
	}

	return true;
}

void Framebuffer::DeleteFrameBuffer(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	FramebufferMapper->CallVoidMethod(Env, InstanceObject, "deleteFramebuffer");
}

int Framebuffer::FrameBufferTexture(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return 0;
	}

	return FramebufferMapper->GetIntField(Env, InstanceObject, "framebufferTexture");
}

void Framebuffer::BindFrameBuffer(JNIEnv* Env, bool Bind) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	FramebufferMapper->CallVoidMethod(Env, InstanceObject, "bindFramebuffer", Bind);
}

Framebuffer::~Framebuffer()
{

}
