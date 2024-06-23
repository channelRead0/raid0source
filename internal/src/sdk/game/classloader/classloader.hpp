#pragma once

class ClassLoader
{
private:
    jvmtiEnv* JvmtiPtr;
    JNIEnv* JniPtr;

    jobject ClassLoaderObject;

    jmethodID LoadClassMethodID;
    jmethodID FindClassMethodID;

    bool ClassLoaderFetched = false;
    std::unordered_map<std::string, jclass> ClassCache;
    std::mutex Mutex;
public:
    ClassLoader() = default;

    bool SetupClassLoader(JNIEnv* Env);
    bool DestroyClassLoader(JNIEnv* Env) const;
    jclass GetClass(JNIEnv* Env, const char* ClassName);

    jobject GetClassLoaderObject() const;

    bool SetupClassLoader(JNIEnv* JniEnv, jvmtiEnv* JvmtiEnv);
    bool AssignClass(const char* ClassName, jclass& OutClass);

    ~ClassLoader();
};