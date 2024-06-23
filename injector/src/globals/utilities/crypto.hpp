#pragma once
#include <sodium.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace Crypto
{
    inline std::string CalculateHash() 
    {
        char FilePathBuffer[MAX_PATH];
        GetModuleFileNameA(nullptr, FilePathBuffer, MAX_PATH);
        auto FilePath = std::string(FilePathBuffer);

        std::ifstream Binary(FilePath, std::ios::binary);
        if (!Binary)
        {
            abort();

            std::cerr << "Error: Unable to open file: " << FilePath << std::endl;
            return "";
        }

        if (sodium_init() == -1) 
        {
            abort();

            std::cerr << "Error: Sodium initialization failed" << std::endl;
            return "";
        }

        crypto_hash_sha256_state State;
        unsigned char Buffer[4096];
        crypto_hash_sha256_init(&State);
        while (Binary.read(reinterpret_cast<char*>(Buffer), sizeof(Buffer))) {
            crypto_hash_sha256_update(&State, Buffer, Binary.gcount());
        }

        unsigned char Hash[crypto_hash_sha256_BYTES];
        crypto_hash_sha256_final(&State, Hash);

        std::stringstream StringStream;
        for (int i = 0; i < crypto_hash_sha256_BYTES; ++i) {
            StringStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(Hash[i]);
        }

        return StringStream.str();
    }

    inline void DecryptAES(std::vector<unsigned char>* Decrypted, std::vector<unsigned char>& Cipher, std::vector<unsigned char>& IV, std::vector<unsigned char>& Key, bool* Status)
    {
        std::vector<unsigned char> DecryptedData(Cipher.size() - crypto_secretbox_MACBYTES, '\0');
        if (crypto_secretbox_open_easy(DecryptedData.data(), Cipher.data(), Cipher.size(), IV.data(), Key.data()) != 0) 
        {
            *Status = false;
            std::cout << "Decrypt Failure" << std::endl;
        }

        *Status = true;
        *Decrypted = DecryptedData;
    }

    inline void TryDecryptAES(std::vector<unsigned char>* Decrypted, std::vector<unsigned char>& Cipher, std::vector<std::vector<unsigned char>>& IV, std::vector<std::vector<unsigned char>>& Key, bool* Status)
    {
        std::vector<unsigned char> DecryptedData(Cipher.size() - crypto_secretbox_MACBYTES, '\0');
        for (int KeyIndex = 0; KeyIndex < Key.size(); KeyIndex++)
        {
            if (crypto_secretbox_open_easy(DecryptedData.data(), Cipher.data(), Cipher.size(), IV[KeyIndex].data(), Key[KeyIndex].data()) != 0)
            {
                *Status = false;
                std::cout << "Decrypt Failure" << std::endl;

                continue;
            }

            break;
        }

        *Status = true;
        *Decrypted = DecryptedData;
    }
}