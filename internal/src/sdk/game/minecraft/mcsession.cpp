#include "pch.h"
#include "mcsession.hpp"
#include <sdk/internal/register/envregister.hpp>

McSession::McSession(JNIEnv* Env)
{
	this->SessionMapper = this->SessionMapper ? this->SessionMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/Session");
}

McSession::McSession(JNIEnv* Env, jobject InstanceObject)
{
	this->SessionMapper = this->SessionMapper ? this->SessionMapper : std::make_shared<GameClass>(Env, "net/minecraft/util/Session");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<McSession> McSession::NewOffline(JNIEnv* Env, const char* Username)
{
	jmethodID Constructor = Env->GetMethodID(SessionMapper->GetClass(), "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

	jstring UsernameObject = Env->NewStringUTF(Username);
	jstring SessionTypeObject = Env->NewStringUTF("mojang");
	jobject SessionObject = Env->NewObject(SessionMapper->GetClass(), Constructor, UsernameObject, jobject(), jobject(), SessionTypeObject);
	std::shared_ptr<McSession> SessionWrappedPtr = std::make_shared<McSession>(Env, SessionObject);

	Env->DeleteLocalRef(UsernameObject);
	Env->DeleteLocalRef(SessionTypeObject);

	return SessionWrappedPtr;
}

jobject McSession::NewOfflineSessionObject(JNIEnv* Env, const char* Username)
{
	jmethodID Constructor = Env->GetMethodID(SessionMapper->GetClass(), "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

	jstring UsernameObject = Env->NewStringUTF(Username);
	jstring SessionTypeObject = Env->NewStringUTF("mojang");
	jobject SessionObject = Env->NewObject(SessionMapper->GetClass(), Constructor, UsernameObject, jobject(), jobject(), SessionTypeObject);
	
	Env->DeleteLocalRef(UsernameObject);
	Env->DeleteLocalRef(SessionTypeObject);

	return SessionObject;
}

jobject McSession::GetProfile(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return {};
	}

	return SessionMapper->CallObjectMethod(Env, InstanceObject, "getProfile");
}


McSession::~McSession()
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
	}

	InstanceObject = nullptr;
}
