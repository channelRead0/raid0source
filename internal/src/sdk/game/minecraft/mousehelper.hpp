//java.util.concurrent.locks.ReentrantReadWriteLock
#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class MouseHelper : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> MouseHelperMapper = nullptr;
public:
	MouseHelper(JNIEnv* Env);
	MouseHelper(JNIEnv* Env, jobject InstanceObject);

	Vector2<double> MouseDelta(JNIEnv* Env) const;

	~MouseHelper();
};