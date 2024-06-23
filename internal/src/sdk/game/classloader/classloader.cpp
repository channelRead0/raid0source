#include "pch.h"
#include "classloader.hpp"
#include <sstream>

#include "cheat/raid0.hpp"

bool ClassLoader::SetupClassLoader(JNIEnv* Env)
{
	if (ClassLoaderFetched) {
		return true;
	}

	jclass* LoadedClasses = nullptr;
	jint LoadedClassCount = 0;

	JvmtiPtrM->GetLoadedClasses(&LoadedClassCount, &LoadedClasses);

	jclass MinecraftClass = nullptr;

	for (jint ClassIndex = 0; ClassIndex < LoadedClassCount; ++ClassIndex)
	{
		const auto ClassAtIndex = LoadedClasses[ClassIndex];

		const auto NameString = GetJavaClassName(Env, ClassAtIndex);

		std::string ClassName = NameString.substr(NameString.find_last_of('.') + 1);

		if (ClassName != "Minecraft")
		{
			Env->DeleteLocalRef(ClassAtIndex);
			continue;
		}

		MinecraftClass = ClassAtIndex;
		break;
	}

	if (MinecraftClass == nullptr) {
		return false;
	}

	JvmtiPtrM->GetClassLoader(MinecraftClass, &ClassLoaderObject);
	ClassLoaderObject = Env->NewGlobalRef(ClassLoaderObject);

	if (!ClassLoaderObject) {
		return false;
	}

	const auto ClassLoaderClass = Env->GetObjectClass(ClassLoaderObject);

	LoadClassMethodID = Env->GetMethodID(ClassLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	FindClassMethodID = Env->GetMethodID(ClassLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");

	if (!LoadClassMethodID || !FindClassMethodID) {
		return false;
	}

	//Env->DeleteLocalRef(ClassLoaderClass);

	JvmtiPtrM->Deallocate(reinterpret_cast<unsigned char*>(LoadedClasses));

	ClassLoaderFetched = true;
	return ClassLoaderFetched;
}

bool ClassLoader::DestroyClassLoader(JNIEnv* Env) const
{
	if (!ClassLoaderFetched) {
		return false;
	}

	if (Env->IsSameObject(ClassLoaderObject, NULL)) {
		return false;
	}

	for (const auto& Class : ClassCache)
	{
#ifdef SHOW_CONSOLE
		std::cout << "Freed: " << Class.first << std::endl;
#endif
		Env->DeleteGlobalRef(Class.second);
	}

	Env->DeleteGlobalRef(ClassLoaderObject);

	return true;
}

jclass ClassLoader::GetClass(JNIEnv* Env, const char* ClassName) 
{
	if (!ClassLoaderFetched) {
		return nullptr;
	}

	if (const auto FoundClassCache = ClassCache.find(std::string(ClassName)); FoundClassCache != ClassCache.end()) {
		return FoundClassCache->second;
	}

	const jstring ClassNameObject = Env->NewStringUTF(ClassName);
	const jobject LoadedClass = Env->CallObjectMethod(ClassLoaderObject, LoadClassMethodID, ClassNameObject);

	Env->DeleteLocalRef(ClassNameObject);

	if (LoadedClass) 
	{
		const auto ReturnClass = static_cast<jclass>(Env->NewGlobalRef(LoadedClass));
		ClassCache[std::string(ClassName)] = ReturnClass;

		return ReturnClass;
	}

	return nullptr;
}

jobject ClassLoader::GetClassLoaderObject() const
{
	return ClassLoaderObject;
}

bool ClassLoader::SetupClassLoader(JNIEnv* JniEnv, jvmtiEnv* JvmtiEnv)
{
	if (ClassLoaderFetched) {
		return true;
	}

	this->JniPtr = JniEnv;
	this->JvmtiPtr = JvmtiEnv;

	jclass* LoadedClasses = nullptr;
	jint LoadedClassCount = 0;

	JvmtiPtr->GetLoadedClasses(&LoadedClassCount, &LoadedClasses);

	for (jint ClassIndex = 0; ClassIndex < LoadedClassCount; ++ClassIndex)
	{
		const auto ClassAtIndex = LoadedClasses[ClassIndex];

		const auto NameString = GetJavaClassName(JniEnv, ClassAtIndex);

		std::string ClassName = NameString.substr(NameString.find_last_of('.') + 1);

		if (ClassName != "ave" && ClassName != "Minecraft") 
		{
			JniPtr->DeleteLocalRef(ClassAtIndex);
			continue;
		}

		JvmtiPtr->GetClassLoader(ClassAtIndex, &ClassLoaderObject);
		const auto ClassLoaderClass = JniPtr->FindClass("java/lang/ClassLoader");

		LoadClassMethodID = JniPtr->GetMethodID(ClassLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		FindClassMethodID = JniPtr->GetMethodID(ClassLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");

		JniPtr->DeleteLocalRef(ClassLoaderClass);

		JniPtr->DeleteLocalRef(ClassAtIndex);
		JvmtiPtr->Deallocate(reinterpret_cast<unsigned char*>(LoadedClasses));

		ClassLoaderFetched = true;
		return ClassLoaderFetched;
	}

	return false;
}

bool ClassLoader::AssignClass(const char* ClassName, jclass& OutClass)
{
	if (!ClassLoaderFetched) {
		return false;
	}

	auto ClassNameString = std::string(ClassName);
	std::ranges::replace(ClassNameString, '/', '.');

	const auto FoundClassCache = ClassCache.find(ClassNameString);

	if (FoundClassCache != ClassCache.end())
	{
		OutClass = FoundClassCache->second;
		return true;
	}

	const jstring ClassNameJString = JniPtr->NewStringUTF(ClassNameString.c_str());
	const jobject FoundClass = JniPtr->CallObjectMethod(ClassLoaderObject, LoadClassMethodID, ClassNameJString);

	JniPtr->DeleteLocalRef(ClassNameJString);

	if (FoundClass)
	{
		const auto ClassToRegister = static_cast<jclass>(JniPtr->NewGlobalRef(FoundClass));
		OutClass = ClassToRegister;

		ClassCache[ClassNameString] = ClassToRegister;

		return true;
	}

	return false;
}

ClassLoader::~ClassLoader()
{
	if (ClassLoaderFetched && ClassLoaderObject) 
	{
		INSTANCE_DESTRUCT = true;

#ifdef SHOW_CONSOLE
		std::cout << "Destructor Called!" << std::endl;
#endif

		for (const auto& CachedClass : ClassCache) 
		{
#ifdef SHOW_CONSOLE
			std::cout << "Freeing " << CachedClass.first << std::endl;
#endif
			//JniPtr->DeleteGlobalRef(CachedClass.second);
		}

		//JniPtr->DeleteLocalRef(ClassLoaderObject);
	}
}
