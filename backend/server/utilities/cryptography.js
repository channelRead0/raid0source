const Crypto = require("crypto");
const Fs = require("fs");
const Sodium = require('sodium-native');

function SliceBuffer(Buffer)
{
    const BufferSliced = {};

    BufferSliced.AesKey = Buffer.slice(0, 32);
    BufferSliced.AesIV = Buffer.slice(Buffer.length - 24, Buffer.length);
    BufferSliced.Cipher = Buffer.slice(32, Buffer.length - 24);

   return BufferSliced;
}

function DecryptSecretBox(Cipher, Nonce, Key)
{
    let Decrypted = Buffer.alloc(Cipher.length - Sodium.crypto_secretbox_MACBYTES);

    if (!Sodium.crypto_secretbox_open_easy(Decrypted, Cipher, Nonce, Key))
    {
        console.log("Decryption Failure");
        return null;
    }

    return Decrypted.toString('utf-8');
}

function EncryptSecretBox(RawBuffer, Nonce, Key)
{
    const EncryptedBuffer = Buffer.alloc(RawBuffer.length + Sodium.crypto_secretbox_MACBYTES);

    Sodium.crypto_secretbox_easy(EncryptedBuffer, RawBuffer, Nonce, Key);

    return EncryptedBuffer;
}

module.exports = { SliceBuffer, DecryptSecretBox, EncryptSecretBox };