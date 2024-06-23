#include "pch.h"
#include "classbase.hpp"

void ClassBase::PromoteInstanceObject(JNIEnv* Env)
{
    if (Env->IsSameObject(InstanceObject, NULL)) {
        return;
    }

    this->UpdateInstanceObject(Env->NewGlobalRef(InstanceObject));
}

void ClassBase::UpdateInstanceObject(jobject UpdatedObject)
{
    this->InstanceObject = UpdatedObject;
}

jobject ClassBase::GetInstanceObject() const
{
    return InstanceObject;
}

