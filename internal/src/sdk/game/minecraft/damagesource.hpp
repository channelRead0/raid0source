#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class DamageSource : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> DamageSourceMapper = nullptr;
public:
	DamageSource(JNIEnv* Env);
	DamageSource(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<DamageSource> OnFireDamageSource(JNIEnv* Env) const;

	~DamageSource();
};
