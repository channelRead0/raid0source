#include "pch.h"
#include "authentication.hpp"
#include <cheat/network/request.hpp>

AUTH_STATUS Authentication::Authenticate()
{
	VMProtectBeginUltra("Authentication");

	sodium_init();

	json Request;
	Request["LB"] = Key;
	Request["LC"] = HardwareUtilities::GetHardwareIdentifier();
	Request["LA"] = Crypto::CalculateHash();

	std::vector<std::vector<unsigned char>> DecryptKeys;
	std::vector<std::vector<unsigned char>> DecryptIVs;

	for (auto& [Key, Value] : Request.items())
	{
		const auto Data = Value.get<std::string>();

		std::vector<unsigned char> AesKey(crypto_secretbox_KEYBYTES);
		randombytes_buf(AesKey.data(), AesKey.size());
		std::vector<unsigned char> AesIV(crypto_secretbox_NONCEBYTES);
		randombytes_buf(AesIV.data(), AesIV.size());

		std::vector<unsigned char> Cipher(Data.size() + crypto_secretbox_MACBYTES);
		crypto_secretbox_easy(Cipher.data(), reinterpret_cast<const unsigned char*>(Data.data()), Data.size(), AesIV.data(), AesKey.data());

		std::vector<unsigned char> Final;
		Final.insert(Final.end(), AesKey.begin(), AesKey.end());
		Final.insert(Final.end(), Cipher.begin(), Cipher.end());
		Final.insert(Final.end(), AesIV.begin(), AesIV.end());

		DecryptKeys.push_back(AesKey);
		DecryptIVs.push_back(AesIV);

		Value = Final;
	}

	auto Response = ::Request("http://localhost:80/authenticate", Request.dump()).Post();

	json ResponseObject;
	if (!JsonUtilities::TryParse(&ResponseObject, Response)) 
	{
		//std::cout << "external server issu" << std::endl;
		return UNKNOWN;
	}

	auto DllBufferRaw = ResponseObject["LB"]["data"].get<std::vector<unsigned char>>();

	bool DecryptStatus = false;
	std::vector<unsigned char> DecryptedBuffer;
	Crypto::TryDecryptAES(&DecryptedBuffer, DllBufferRaw, DecryptIVs, DecryptKeys, &DecryptStatus);

	Buffer DllBuffer(DecryptedBuffer.data(), DecryptedBuffer.size());

	VMProtectEnd();

	return {};
}
