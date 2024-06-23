#pragma once

namespace JNIUtils
{
    inline bool IsArrayObject(JNIEnv* Env, jobject Object)
    {
	    const jclass ClassClass = Env->FindClass("java/lang/Class");
        if (ClassClass == nullptr) 
        {
            std::cerr << "Failed to find java.lang.Class class!" << std::endl;
            return {};
        }

        jmethodID Class_isArray_m = (Env)->GetMethodID(ClassClass, "isArray", "()Z");
        jclass obj_class = (Env)->GetObjectClass(Object);
        jboolean is_array = (Env)->CallBooleanMethod(obj_class, Class_isArray_m);

        return is_array;
    }

    inline void PrintAllMethods(JNIEnv* env, jclass clazz) {
        jclass classClass = env->FindClass("java/lang/Class");
        if (classClass == nullptr) {
            std::cerr << "Failed to find java.lang.Class class!" << std::endl;
            return;
        }

        jmethodID getMethodsMethod = env->GetMethodID(classClass, "getMethods", "()[Ljava/lang/reflect/Method;");
        if (getMethodsMethod == nullptr) {
            std::cerr << "Failed to find getMethods method in java.lang.Class!" << std::endl;
            return;
        }

        jobjectArray methodsArray = (jobjectArray)env->CallObjectMethod(clazz, getMethodsMethod);
        if (methodsArray == nullptr) {
            std::cerr << "Failed to get methods from class!" << std::endl;
            return;
        }

        std::cout << "Methods of class:" << std::endl;
        for (jsize i = 0; i < env->GetArrayLength(methodsArray); ++i) {
            jobject methodObj = env->GetObjectArrayElement(methodsArray, i);
            jmethodID getNameMethod = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");
            if (getNameMethod == nullptr) {
                std::cerr << "Failed to find getName method in java.lang.Class!" << std::endl;
                return;
            }
            jstring methodName = (jstring)env->CallObjectMethod(methodObj, getNameMethod);
            const char* methodNameStr = env->GetStringUTFChars(methodName, nullptr);
            std::cout << "  Method Name: " << methodNameStr << std::endl;
            env->ReleaseStringUTFChars(methodName, methodNameStr);
        }
    }

    inline void PrintClassName(JNIEnv* env, jclass clazz) {
        jclass classClass = env->FindClass("java/lang/Class");
        if (classClass == nullptr) {
            std::cerr << "Failed to find java.lang.Class class!" << std::endl;
            return;
        }

        jmethodID getNameMethod = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");
        if (getNameMethod == nullptr) {
            std::cerr << "Failed to find getName method in java.lang.Class!" << std::endl;
            return;
        }

        jstring className = (jstring)env->CallObjectMethod(clazz, getNameMethod);
        const char* classNameStr = env->GetStringUTFChars(className, nullptr);
        std::cout << "Class Name: " << classNameStr << std::endl;
        env->ReleaseStringUTFChars(className, classNameStr);
    }

    inline std::string GetJavaClassName(JNIEnv* env, jclass clazz) {
        jclass classClass = env->FindClass("java/lang/Class");
        if (classClass == nullptr) {
            std::cerr << "Failed to find java.lang.Class class!" << std::endl;
            return {};
        }

        jmethodID getNameMethod = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");
        if (getNameMethod == nullptr) {
            std::cerr << "Failed to find getName method in java.lang.Class!" << std::endl;
            return {};
        }

        jstring className = (jstring)env->CallObjectMethod(clazz, getNameMethod);
        const char* classNameStr = env->GetStringUTFChars(className, nullptr);
        std::string returnname = std::string(classNameStr);
        env->ReleaseStringUTFChars(className, classNameStr);

        return returnname;
    }

    inline void PrintAllFields(JNIEnv* env, jclass clazz) {
        jclass classClass = env->FindClass("java/lang/Class");
        if (classClass == nullptr) {
            std::cerr << "Failed to find java.lang.Class class!" << std::endl;
            return;
        }

        jmethodID getFieldsMethod = env->GetMethodID(classClass, "getFields", "()[Ljava/lang/reflect/Field;");
        if (getFieldsMethod == nullptr) {
            std::cerr << "Failed to find getFields method in java.lang.Class!" << std::endl;
            return;
        }

        jobjectArray fieldsArray = (jobjectArray)env->CallObjectMethod(clazz, getFieldsMethod);
        if (fieldsArray == nullptr) {
            std::cerr << "Failed to get fields from class!" << std::endl;
            return;
        }

        std::cout << "Fields of class:" << std::endl;
        for (jsize i = 0; i < env->GetArrayLength(fieldsArray); ++i) {
            jobject fieldObj = env->GetObjectArrayElement(fieldsArray, i);
            jmethodID getNameMethod = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");
            if (getNameMethod == nullptr) {
                std::cerr << "Failed to find getName method in java.lang.Class!" << std::endl;
                return;
            }
            jstring fieldName = (jstring)env->CallObjectMethod(fieldObj, getNameMethod);
            const char* fieldNameStr = env->GetStringUTFChars(fieldName, nullptr);
            std::cout << "  Field Name: " << fieldNameStr << std::endl;
            env->ReleaseStringUTFChars(fieldName, fieldNameStr);
        }
    }
}