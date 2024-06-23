// ReSharper disable CppInconsistentNaming
#pragma once
#include <shared_mutex>
#include <unordered_map>
#include <ppl.h>
#include <concurrent_unordered_map.h>

#include "src/sdk/internal/classfactory.hpp"
#include "src/sdk/game/classloader/classloader.hpp"

struct ClassDataWeak
{
	jint ClassMethodCount = 0;
	jint ClassFieldCount = 0;

	jmethodID* ClassMethodsPointer = nullptr;
	jfieldID* ClassFieldsPointer = nullptr;
};

struct FieldData
{
	jfieldID FieldPtr;

	std::string FieldName;
	std::string FieldSignature;

	bool IsStatic = false;
};

struct FieldDataWeak
{
	std::string FieldName;
	std::string FieldSignature;

	bool IsStatic = false;
};

struct MethodData
{
	jmethodID MethodPtr;

	std::string MethodName;
	std::string MethodSignature;

	bool IsStatic = false;
};

struct MethodDataWeak
{
	std::string MethodName;
	std::string MethodSignature;

	bool IsStatic = false;
};

namespace Cache
{
	inline concurrency::concurrent_unordered_map<std::string, MethodData> MethodCache;
	inline concurrency::concurrent_unordered_map<std::string, FieldData> FieldCache;
	inline concurrency::concurrent_unordered_map<std::string, jclass> RawClassCache;
	inline concurrency::concurrent_unordered_map<std::string, ClassDataWeak> ClassCache;
	/*inline std::unordered_map<std::string, MethodData> MethodCache;
	inline std::unordered_map<std::string, FieldData> FieldCache;*/
}

class GameClass
{
private:
	std::mutex ClassMutex;

	bool Initialized = false;

	JNIEnv* JniPtr = nullptr;
	jvmtiEnv* JvmtiPtr = nullptr;
	ClassLoader* ClassInterface = nullptr;

	jclass JavaClass;
	ClassDataWeak ClassData;
	std::string ClassName;

	concurrency::concurrent_unordered_map<std::string, jmethodID> RegisteredMethods;
	concurrency::concurrent_unordered_map<std::string, jfieldID> RegisteredFields;
public:

	jclass GetClass() const;
	std::string GetName() const;
	bool GetInitializationState() const;

	//Definitions for just MCP mappings
	GameClass(JNIEnv* JniEnv, const char* ClassNameMCP);

	void PrintClassNames(JNIEnv* Env);
	void RefreshClass(JNIEnv* Env);
	bool RegisterMethod(const char* MethodNameMCP);
	bool RegisterField(const char* FieldNameMPC);

	jmethodID RegisterAndFetchMethodID(const char* MethodNameMCP);
	jfieldID RegisterAndFetchFieldID(const char* FieldNameMPC);
	jmethodID GetMethodID(const char* MethodNameMCP);
	jfieldID GetFieldID(const char* FieldNameMPC);

	std::string CallStringMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP ...);
	std::vector<jobject> CallArrayMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP ...);
	jobject CallObjectMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP ...);
	bool CallBooleanMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP ...);
	void CallVoidMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP ...);
	int CallIntMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP ...);
	float CallFloatMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP ...);
	double CallDoubleMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP ...);


	std::string GetStringField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP);
	jobject GetObjectField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP);
	std::vector<jobject> GetArrayField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP);
	bool GetBooleanField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP);
	int GetIntField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP);
	jbyte GetByteField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP);
	float GetFloatField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP);
	double GetDoubleField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP);

	void SetObjectField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, jobject Param);
	void SetBooleanField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, bool Param);
	void SetIntField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, int Param);
	void SetFloatField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, float Param);
	void SetDoubleField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, double Param);
};