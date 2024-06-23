#pragma once
#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>

#include <src/sdk/game/classloader/classloader.hpp>

template<typename T>
class ThreadSafeClassFactory
{
private:
    std::unordered_map<std::thread::id, T*> Instances;
    std::mutex Mutex;

public:
    T* GetInstance()
	{
       // std::lock_guard Lock(Mutex);
        std::lock_guard<std::mutex> Lock(Mutex);
        std::thread::id CallingThreadID = std::this_thread::get_id();

        auto FoundInstance = Instances.find(CallingThreadID);

        if (FoundInstance != Instances.end()) {
            return FoundInstance->second;
        }

        T* NewInstance = new T();
        Instances[CallingThreadID] = NewInstance;
        return NewInstance;
    }

    void DeleteInstance()
	{
        std::lock_guard<std::mutex> Lock(Mutex);
        std::thread::id CallingThreadID = std::this_thread::get_id();

        auto FoundInstance = Instances.find(CallingThreadID);

        if (FoundInstance != Instances.end()) {
            delete FoundInstance->second;
            Instances.erase(CallingThreadID);
        }
    }
};

inline ThreadSafeClassFactory<ClassLoader> ClassLoaderFactory;