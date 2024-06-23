//java.util.concurrent.locks.ReentrantReadWriteLock
#pragma once

#include "src/sdk/internal/templates/classbase.hpp"

class List : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ListMapper = nullptr;
public:
	List(JNIEnv* Env);
	List(JNIEnv* Env, jobject InstanceObject);

	int Size(JNIEnv* Env) const;

	~List();
};
