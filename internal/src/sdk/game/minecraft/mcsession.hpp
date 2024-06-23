#pragma once

#include "itemstack.hpp"
#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class McSession : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> SessionMapper = nullptr;
public:
	McSession(JNIEnv* Env);
	McSession(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<McSession> NewOffline(JNIEnv* Env, const char* Username);
	jobject NewOfflineSessionObject(JNIEnv* Env, const char* Username);

	jobject GetProfile(JNIEnv* Env) const;

	~McSession();
};
