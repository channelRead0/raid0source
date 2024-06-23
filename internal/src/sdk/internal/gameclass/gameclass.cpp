#include "pch.h"

#include "gameclass.hpp"

#include <shared_mutex>

#include "src/cheat/raid0.hpp"

using namespace Cache;

jclass GameClass::GetClass() const
{
	return JavaClass;
}

std::string GameClass::GetName() const
{
	return ClassName;
}

bool GameClass::GetInitializationState() const
{
	return Initialized;
}

GameClass::GameClass(JNIEnv* Env, const char* ClassNameMCP)
{
	this->ClassName = ClassNameMCP;
	this->JvmtiPtr = JvmtiPtrM;

	/*auto ClassNameString = std::string(ClassNameMCP);
	std::ranges::replace(ClassNameString, '/', '.');*/

	const auto RawClass = RawClassCache.find(ClassNameMCP);
	if (RawClass != RawClassCache.end()) {
		this->JavaClass = RawClass->second;
	}
	else
	{
		this->JavaClass = ClassLoaderPtr->GetClass(Env, ClassNameMCP);

		RawClassCache[ClassNameMCP] = this->JavaClass;
	}

	/*try
	{
		jclass Class = RawClassCache.at(ClassNameMCP);

		this->JavaClass = Class;
	}
	catch (const std::out_of_range&)
	{
		const jclass Class = ClassLoaderPtr->GetClass(Env, ClassNameMCP);

		this->JavaClass = Class;

		RawClassCache[ClassNameMCP] = Class;
	}*/

	if (JavaClass == nullptr)
	{
		std::cerr << "Failed to find " << ClassNameMCP << " !" << std::endl;
		return;
	}

	const auto WeakClassCache = ClassCache.find(ClassNameMCP);
	if (WeakClassCache != ClassCache.end())
	{
		this->ClassData = WeakClassCache->second;
		Initialized = true;

		return;
	}

	if (WeakClassCache == ClassCache.end())
	{
		JvmtiPtr->GetClassMethods(JavaClass, &this->ClassData.ClassMethodCount, &this->ClassData.ClassMethodsPointer);
		JvmtiPtr->GetClassFields(JavaClass, &this->ClassData.ClassFieldCount, &this->ClassData.ClassFieldsPointer);

		ClassCache[ClassNameMCP] = this->ClassData;

		Initialized = true;
	}
	//this->ClassName = ClassNameMCP;
	//this->JvmtiPtr = JvmtiPtrM;

	///*auto ClassNameString = std::string(ClassNameMCP);
	//std::ranges::replace(ClassNameString, '/', '.');*/

	//this->JavaClass = ClassLoaderPtr->GetClass(Env, ClassNameMCP);

	//if (JavaClass == nullptr)
	//{
	//	std::cerr << "Failed to find " << ClassNameMCP << " !" << std::endl;
	//	return;
	//}

	//const auto FoundClassCache = ClassCache.find(ClassNameMCP);

	//if (FoundClassCache != ClassCache.end())
	//{
	//	this->ClassData = FoundClassCache->second;
	//	Initialized = true;

	//	return;
	//}

	//std::lock_guard<std::mutex> Lock(CacheMutex);

	//JvmtiPtr->GetClassMethods(JavaClass, &this->ClassData.ClassMethodCount, &this->ClassData.ClassMethodsPointer);
	//JvmtiPtr->GetClassFields(JavaClass, &this->ClassData.ClassFieldCount, &this->ClassData.ClassFieldsPointer);

	//ClassCache[ClassNameMCP] = this->ClassData;

	//Initialized = true;
}

void GameClass::PrintClassNames(JNIEnv* Env)
{
	for (int MethodIndex = 0; MethodIndex < ClassData.ClassMethodCount; MethodIndex++)
	{
		char* MethodNameAtIndex;
		char* MethodSignatureAtIndex;

		const auto MethodAtIndex = ClassData.ClassMethodsPointer[MethodIndex];

		JvmtiPtr->GetMethodName(MethodAtIndex, &MethodNameAtIndex, &MethodSignatureAtIndex, nullptr);

		const auto MethodNameString = std::string(MethodNameAtIndex);

		std::cout << MethodNameString << std::endl;
	}
}

void GameClass::RefreshClass(JNIEnv* Env)
{
	std::unique_lock<std::mutex>Lock(ClassMutex);
	
	Initialized = false;

	this->JavaClass = ClassLoaderPtr->GetClass(Env, this->ClassName.data());

	if (JavaClass == nullptr)
	{
		std::cerr << "Failed to find " << this->ClassName << " !" << std::endl;
		return;
	}

	JvmtiPtr->GetClassMethods(JavaClass, &this->ClassData.ClassMethodCount, &this->ClassData.ClassMethodsPointer);
	JvmtiPtr->GetClassFields(JavaClass, &this->ClassData.ClassFieldCount, &this->ClassData.ClassFieldsPointer);

	ClassCache[this->ClassName] = this->ClassData;

	Initialized = true;
}

bool GameClass::RegisterMethod(const char* MethodNameMCP)
{
	//std::scoped_lock<std::recursive_mutex> LocalLock(Mutex);

	const auto MethodExists = RegisteredMethods.find(MethodNameMCP);

	if (MethodExists != RegisteredMethods.end())
	{
		std::cerr << "Method " << MethodNameMCP << " already registered!" << std::endl;
		return false;
	}

	const auto FoundMethodCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (FoundMethodCache != MethodCache.end())
	{
		const auto& MethodData = FoundMethodCache->second;

		const jmethodID MethodToRegister = MethodData.MethodPtr;

		if (MethodToRegister != nullptr)
		{
			#ifdef SHOW_CONSOLE
			//std::cout << "Registered method cache " << MethodData.MethodName << "[" << MethodToRegister << "]!" << std::endl;
			#endif

			RegisteredMethods[MethodNameMCP] = MethodToRegister;
			return true;
		}
	}

	for (int MethodIndex = 0; MethodIndex < ClassData.ClassMethodCount; MethodIndex++)
	{
		char* MethodNameAtIndex;
		char* MethodSignatureAtIndex;

		const auto MethodAtIndex = ClassData.ClassMethodsPointer[MethodIndex];

		JvmtiPtr->GetMethodName(MethodAtIndex, &MethodNameAtIndex, &MethodSignatureAtIndex, nullptr);

		const auto MethodNameString = std::string(MethodNameAtIndex);

		if (MethodNameString != std::string(MethodNameMCP)) {
			continue;
		}

		jint MethodModifiers;
		JvmtiPtr->GetMethodModifiers(MethodAtIndex, &MethodModifiers);

		const bool IsStatic = (MethodModifiers & 0x0008);

		const MethodData MethodData
		{
			MethodAtIndex,
			std::string(MethodNameAtIndex),
			std::string(MethodSignatureAtIndex),
			IsStatic
		};

		RegisteredMethods[MethodNameMCP] = MethodAtIndex;
		MethodCache[std::string(ClassName + "." + MethodNameMCP)] = MethodData;

		JvmtiPtr->Deallocate(reinterpret_cast<unsigned char*>(MethodNameAtIndex));
		JvmtiPtr->Deallocate(reinterpret_cast<unsigned char*>(MethodSignatureAtIndex));

		#ifdef SHOW_CONSOLE
		//std::cout << "Registered method " << MethodNameString << "[" << MethodAtIndex << "]!" << std::endl;
		#endif

		return true;
	}

	return false;
}

bool GameClass::RegisterField(const char* FieldNameMPC)
{
	//std::scoped_lock<std::recursive_mutex> LocalLock(Mutex);

	const auto FieldExists = RegisteredFields.find(FieldNameMPC);

	if (FieldExists != RegisteredFields.end())
	{
#ifdef SHOW_CONSOLE
		std::cerr << "Field " << FieldNameMPC << " already registered!" << std::endl;
#endif
		return false;
	}

	const auto FoundFieldCache = FieldCache.find(std::string(ClassName + "." + FieldNameMPC));

	if (FoundFieldCache != FieldCache.end())
	{
		const auto& FieldData = FoundFieldCache->second;

		const jfieldID FieldToRegister = FieldData.FieldPtr;

		if (FieldToRegister != nullptr)
		{
			#ifdef SHOW_CONSOLE
			//std::cout << "Registered field cache " << FieldData.FieldName << "[" << FieldToRegister << "]!" << std::endl;
			#endif

			RegisteredFields[FieldNameMPC] = FieldToRegister;
			return true;
		}
	}

	for (int FieldIndex = 0; FieldIndex < ClassData.ClassFieldCount; FieldIndex++)
	{
		char* FieldNameAtIndex;
		char* FieldSignatureAtIndex;

		const auto FieldAtIndex = ClassData.ClassFieldsPointer[FieldIndex];

		JvmtiPtr->GetFieldName(JavaClass, FieldAtIndex, &FieldNameAtIndex, &FieldSignatureAtIndex, nullptr);

		const auto FieldNameString = std::string(FieldNameAtIndex);

		if (FieldNameString != std::string(FieldNameMPC)) {
			continue;
		}

		jint FieldModifiers;
		JvmtiPtr->GetFieldModifiers(JavaClass, FieldAtIndex, &FieldModifiers);

		const bool IsStatic = (FieldModifiers & 0x0008);

		const FieldData FieldData
		{
			FieldAtIndex,
			std::string(FieldNameAtIndex),
			std::string(FieldSignatureAtIndex),
			IsStatic
		};

		RegisteredFields[FieldNameMPC] = FieldAtIndex;
		FieldCache[std::string(ClassName + "." + FieldNameMPC)] = FieldData;

		JvmtiPtr->Deallocate(reinterpret_cast<unsigned char*>(FieldNameAtIndex));
		JvmtiPtr->Deallocate(reinterpret_cast<unsigned char*>(FieldSignatureAtIndex));

		#ifdef SHOW_CONSOLE
		//std::cout << "Registered field " << FieldNameString << "[" << FieldAtIndex << "]!" << std::endl;
		#endif

		return true;
	}

	return false;
}

jmethodID GameClass::RegisterAndFetchMethodID(const char* MethodNameMCP)
{
	const auto MethodExists = RegisteredMethods.find(MethodNameMCP);

	if (MethodExists != RegisteredMethods.end()) {
		return MethodExists->second;
	}

	if (RegisterMethod(MethodNameMCP)) {
		return RegisteredMethods[MethodNameMCP];
	}

	return jmethodID();
}

jfieldID GameClass::RegisterAndFetchFieldID(const char* FieldNameMPC)
{
	const auto FieldExists = RegisteredFields.find(FieldNameMPC);

	if (FieldExists != RegisteredFields.end()) {
		return FieldExists->second;
	}

	if (RegisterField(FieldNameMPC)) {
		return RegisteredFields[FieldNameMPC];
	}

	return jfieldID();
}

jmethodID GameClass::GetMethodID(const char* MethodNameMCP)
{
	auto MethodData = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodData == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return jmethodID();
		}

		MethodData = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	return MethodData->second.MethodPtr;
}

jfieldID GameClass::GetFieldID(const char* FieldNameMPC)
{
	auto FieldData = FieldCache.find(std::string(ClassName + "." + FieldNameMPC));

	if (FieldData == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMPC);

		if (!FieldID)
		{
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMPC << " field in " << ClassName << " class!" << std::endl;
#endif
			return jfieldID();
		}

		FieldData = FieldCache.find(std::string(ClassName + "." + FieldNameMPC));
	}

	return FieldData->second.FieldPtr;
}

std::string GameClass::CallStringMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP, ...)
{
	auto MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodDataCache == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	const auto MethodData = MethodDataCache->second;

	jstring StringObject;

	va_list Arguments;
	va_start(Arguments, MethodNameMCP);

	if (MethodData.IsStatic) {
		StringObject = static_cast<jstring>(Env->CallStaticObjectMethodV(static_cast<jclass>(InstanceObject), MethodData.MethodPtr, Arguments));
	}
	else {
		StringObject = static_cast<jstring>(Env->CallObjectMethodV(InstanceObject, MethodData.MethodPtr, Arguments));
	}

	va_end(Arguments);

	if (!StringObject) {
		return {};
	}

	const char* ObjectCharacters = Env->GetStringUTFChars(StringObject, 0);

	Env->ReleaseStringUTFChars(StringObject, ObjectCharacters);
	Env->DeleteLocalRef(StringObject);

	return std::string(ObjectCharacters);
}

std::vector<jobject> GameClass::CallArrayMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP, ...)
{
	auto MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodDataCache == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	const auto MethodData = MethodDataCache->second;

	jobject ArrayObject;

	va_list Arguments;
	va_start(Arguments, MethodNameMCP);

	if (MethodData.IsStatic) {
		ArrayObject = Env->CallStaticObjectMethodV(static_cast<jclass>(InstanceObject), MethodData.MethodPtr, Arguments);
	}
	else {
		ArrayObject = Env->CallObjectMethodV(InstanceObject, MethodData.MethodPtr, Arguments);
	}

	va_end(Arguments);

	if (!ArrayObject) {
		return {};
	}

	if (!IsArrayObject(Env, ArrayObject)) {
		return {};
	}

	const jsize ArrayLength = Env->GetArrayLength(static_cast<jobjectArray>(ArrayObject));

	if (ArrayLength == 0)
	{
		//std::cerr << "Failed to find array elements in " << MethodNameMCP << " method!" << std::endl;
		return {};
	}

	std::vector<jobject> ReturnObjects;
	ReturnObjects.reserve(ArrayLength);
	for (jsize ArrayIndex = 0; ArrayIndex < ArrayLength; ++ArrayIndex)
	{
		const jobject Element = Env->GetObjectArrayElement(static_cast<jobjectArray>(ArrayObject), ArrayIndex);
		ReturnObjects.push_back(Env->NewLocalRef(Element));
	}

	Env->DeleteLocalRef(ArrayObject);

	return ReturnObjects;
}

jobject GameClass::CallObjectMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP, ...)
{
	auto MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodDataCache == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	const auto MethodData = MethodDataCache->second;

	jobject ReturnObject;

	va_list Arguments;
	va_start(Arguments, MethodNameMCP);

	if (MethodData.IsStatic) {
		ReturnObject = Env->CallStaticObjectMethodV(static_cast<jclass>(InstanceObject), MethodData.MethodPtr, Arguments);
	}
	else {
		ReturnObject = Env->CallObjectMethodV(InstanceObject, MethodData.MethodPtr, Arguments);
	}

	va_end(Arguments);

	return Env->NewLocalRef(ReturnObject);
}

bool GameClass::CallBooleanMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP, ...)
{
	auto MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodDataCache == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	const auto MethodData = MethodDataCache->second;

	bool ReturnBool;

	va_list Arguments;
	va_start(Arguments, MethodNameMCP);

	if (MethodData.IsStatic) {
		ReturnBool = Env->CallStaticBooleanMethodV(static_cast<jclass>(InstanceObject), MethodData.MethodPtr, Arguments);
	}
	else {
		ReturnBool = Env->CallBooleanMethodV(InstanceObject, MethodData.MethodPtr, Arguments);
	}

	va_end(Arguments);

	return ReturnBool;
}

void GameClass::CallVoidMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP, ...)
{
	auto MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodDataCache == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return;
		}

		MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	const auto MethodData = MethodDataCache->second;

	va_list Arguments;
	va_start(Arguments, MethodNameMCP);

	if (MethodData.IsStatic) {
		Env->CallStaticVoidMethodV(static_cast<jclass>(InstanceObject), MethodData.MethodPtr, Arguments);
	}
	else {
		Env->CallVoidMethodV(InstanceObject, MethodData.MethodPtr, Arguments);
	}

	va_end(Arguments);

	if (Env->ExceptionCheck())
	{
#ifdef SHOW_CONSOLE
		Env->ExceptionDescribe();
#endif
		Env->ExceptionClear();
	}
}

int GameClass::CallIntMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP, ...)
{
	auto MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodDataCache == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	const auto MethodData = MethodDataCache->second;

	int ReturnInt;

	va_list Arguments;
	va_start(Arguments, MethodNameMCP);

	if (MethodData.IsStatic) {
		ReturnInt = Env->CallStaticIntMethodV(static_cast<jclass>(InstanceObject), MethodData.MethodPtr, Arguments);
	}
	else {
		ReturnInt = Env->CallIntMethodV(InstanceObject, MethodData.MethodPtr, Arguments);
	}

	va_end(Arguments);

	return ReturnInt;
}

float GameClass::CallFloatMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP, ...)
{
	auto MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodDataCache == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	const auto MethodData = MethodDataCache->second;

	float ReturnFloat;

	va_list Arguments;
	va_start(Arguments, MethodNameMCP);

	if (MethodData.IsStatic) {
		ReturnFloat = Env->CallStaticFloatMethodV(static_cast<jclass>(InstanceObject), MethodData.MethodPtr, Arguments);
	}
	else {
		ReturnFloat = Env->CallFloatMethodV(InstanceObject, MethodData.MethodPtr, Arguments);
	}

	va_end(Arguments);

	return ReturnFloat;
}

double GameClass::CallDoubleMethod(JNIEnv* Env, jobject InstanceObject, const char* MethodNameMCP, ...)
{
	auto MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));

	if (MethodDataCache == MethodCache.end())
	{
		const auto MethodID = RegisterAndFetchMethodID(MethodNameMCP);

		if (!MethodID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << MethodNameMCP << " method in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		MethodDataCache = MethodCache.find(std::string(ClassName + "." + MethodNameMCP));
	}

	const auto MethodData = MethodDataCache->second;

	double ReturnDouble;

	va_list Arguments;
	va_start(Arguments, MethodNameMCP);

	if (MethodData.IsStatic) {
		ReturnDouble = Env->CallStaticDoubleMethodV(static_cast<jclass>(InstanceObject), MethodData.MethodPtr, Arguments);
	}
	else {
		ReturnDouble = Env->CallDoubleMethodV(InstanceObject, MethodData.MethodPtr, Arguments);
	}

	va_end(Arguments);

	return ReturnDouble;
}

std::string GameClass::GetStringField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	jstring StringObject;

	if (FieldData.IsStatic) {
		StringObject = static_cast<jstring>(Env->GetStaticObjectField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr));
	}
	else {
		StringObject = static_cast<jstring>(Env->GetObjectField(InstanceObject, FieldData.FieldPtr));
	}

	const char* ObjectCharacters = Env->GetStringUTFChars(StringObject, 0);

	Env->ReleaseStringUTFChars(StringObject, ObjectCharacters);
	Env->DeleteLocalRef(StringObject);

	return std::string(ObjectCharacters);
}

jobject GameClass::GetObjectField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;


	jobject ReturnObject;

	if (FieldData.IsStatic) {
		ReturnObject = Env->GetStaticObjectField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr);
	}
	else {
		ReturnObject = Env->GetObjectField(InstanceObject, FieldData.FieldPtr);
	}

	return Env->NewLocalRef(ReturnObject);
}

std::vector<jobject> GameClass::GetArrayField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	jobject ArrayObject;

	if (FieldData.IsStatic) {
		ArrayObject = Env->GetStaticObjectField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr);
	}
	else {
		ArrayObject = Env->GetObjectField(InstanceObject, FieldData.FieldPtr);
	}

	if (!ArrayObject) {
		return {};
	}

	if (!IsArrayObject(Env, ArrayObject)) {
		return {};
	}

	const jsize ArrayLength = Env->GetArrayLength(static_cast<jobjectArray>(ArrayObject));

	if (ArrayLength == 0)
	{
		//std::cerr << "Failed to find array elements in " << FieldNameMCP << " field!" << std::endl;
		return {};
	}

	std::vector<jobject> ReturnObjects;
	ReturnObjects.reserve(ArrayLength);
	for (jsize ArrayIndex = 0; ArrayIndex < ArrayLength; ++ArrayIndex)
	{
		const jobject Element = Env->GetObjectArrayElement(static_cast<jobjectArray>(ArrayObject), ArrayIndex);
		ReturnObjects.push_back(Env->NewLocalRef(Element));
	}

	Env->DeleteLocalRef(ArrayObject);

	return ReturnObjects;
}

bool GameClass::GetBooleanField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		return Env->GetStaticBooleanField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr);
	}

	return Env->GetBooleanField(InstanceObject, FieldData.FieldPtr);
}

int GameClass::GetIntField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		return Env->GetStaticIntField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr);
	}

	return Env->GetIntField(InstanceObject, FieldData.FieldPtr);
}

jbyte GameClass::GetByteField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		return Env->GetStaticByteField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr);
	}

	return Env->GetByteField(InstanceObject, FieldData.FieldPtr);
}

float GameClass::GetFloatField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		return Env->GetStaticFloatField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr);
	}

	return Env->GetFloatField(InstanceObject, FieldData.FieldPtr);
}

double GameClass::GetDoubleField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return {};
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		return Env->GetStaticDoubleField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr);
	}

	return Env->GetDoubleField(InstanceObject, FieldData.FieldPtr);
}

void GameClass::SetObjectField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, jobject Param)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return;
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		Env->SetStaticObjectField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr, Param);
	}
	else {
		Env->SetObjectField(InstanceObject, FieldData.FieldPtr, Param);
	}
}

void GameClass::SetBooleanField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, bool Param)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return;
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		Env->SetStaticBooleanField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr, Param);
	}
	else {
		Env->SetBooleanField(InstanceObject, FieldData.FieldPtr, Param);
	}
}

void GameClass::SetIntField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, int Param)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return;
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		Env->SetStaticIntField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr, Param);
	}
	else {
		Env->SetIntField(InstanceObject, FieldData.FieldPtr, Param);
	}
}

void GameClass::SetFloatField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, float Param)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return;
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		Env->SetStaticFloatField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr, Param);
	}
	else {
		Env->SetFloatField(InstanceObject, FieldData.FieldPtr, Param);
	}
}

void GameClass::SetDoubleField(JNIEnv* Env, jobject InstanceObject, const char* FieldNameMCP, double Param)
{
	auto FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));

	if (FieldDataCache == FieldCache.end())
	{
		const auto FieldID = RegisterAndFetchFieldID(FieldNameMCP);

		if (!FieldID)
		{
			this->RefreshClass(Env);
#ifdef SHOW_CONSOLE
			std::cerr << "Failed to find " << FieldNameMCP << " field in " << ClassName << " class!" << std::endl;
#endif
			return;
		}

		FieldDataCache = FieldCache.find(std::string(ClassName + "." + FieldNameMCP));
	}

	const auto FieldData = FieldDataCache->second;

	if (FieldData.IsStatic) {
		Env->SetStaticDoubleField(static_cast<jclass>(InstanceObject), FieldData.FieldPtr, Param);
	}
	else {
		Env->SetDoubleField(InstanceObject, FieldData.FieldPtr, Param);
	}
}
