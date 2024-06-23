#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class Potion : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> PotionMapper = nullptr;
public:
	Potion(JNIEnv* Env);
	Potion(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<Potion> FireResistance(JNIEnv* Env) const;

	~Potion();
};
