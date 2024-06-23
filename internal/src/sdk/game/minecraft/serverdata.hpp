#pragma once

#include "player.hpp"
#include "src/sdk/internal/templates/classbase.hpp"

class ServerData : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> ServerDataMapper = nullptr;
public:
	ServerData(JNIEnv* Env);
	ServerData(JNIEnv* Env, jobject InstanceObject);

	std::string ServerIP(JNIEnv* Env) const;
	std::string ServerName(JNIEnv* Env) const;

	~ServerData();
};

//TODO: check differences and define them in World1_7 derivation, 1.8 should be fully working
//use respective class version derivation on declaration, methods with differences will be added here
class ServerData1_7 : public ServerData
{
public:
};

class ServerData1_8 : public ServerData
{
public:
};