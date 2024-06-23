#pragma once

class ClientData
{
public:
	struct
	{
		DWORD LLW = false;
		DWORD LLLW = false;
		bool LLLLW = false;
		bool LLLLI = false;
		bool LLLLC = false;
	};
};

class Molly
{
private:

public:
	Molly();

	bool AttachJNI() const;
	bool DetachJNI() const;

	bool Authenticate() const;
	bool InitializeGame() const;
	void DestroyGame() const;
	void DoIPC() const;

	JavaVM* GetJvm() const;
	JNIEnv* GetJniEnv() const;
	jvmtiEnv* GetJvmtiEnv() const;

	void CreateModules() const;
	void DestroyModules() const;

	void CreateHooks() const;
	void DeleteHooks() const;

	~Molly();
};

extern JavaVM* JvmPtrM;
extern JNIEnv* JniEnvPtrM;
extern jvmtiEnv* JvmtiPtrM;
extern std::unique_ptr<Molly> InstancePtr;
extern std::unique_ptr<ClassLoader> ClassLoaderPtr;
