#pragma once
#include <cstring>
#include <WTypesbase.h>

class Buffer
{
private:
	unsigned char* DataBytes = nullptr;
	int DataSize = 0;
	bool Populated = false;
public:
	Buffer(unsigned char* Bytes, int Size)
	{
		VMProtectBeginUltra("Buffer Handling");

		this->DataBytes = new unsigned char[Size];
		this->DataSize = Size;
		this->Populated = true;

		std::memcpy(DataBytes, Bytes, Size);

		VMProtectEnd();
	}

	int GetSize()
	{
		return DataSize;
	}

	LPVOID GetBuffer()
	{
		VMProtectBeginUltra("Buffer Handling B");

		if (!Populated) {
			return nullptr;
		}

		return reinterpret_cast<LPVOID>(DataBytes);

		VMProtectEnd();
	}

	void FreeData()
	{
		VMProtectBeginUltra("Buffer Handling C");

		if (Populated) {
			delete DataBytes;
		}

		Populated = false;

		VMProtectEnd();
	}

	~Buffer()
	{
		VMProtectBeginUltra("Buffer Handling D");

		if (!Populated) {
			return;
		}

		delete DataBytes;

		VMProtectEnd();
	}
};