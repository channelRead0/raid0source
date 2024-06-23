const crypto = require("crypto");
const fs = require("fs");
function getsha512(rawstring)
{
    const sha512 = crypto.createHash('sha512')

    sha512.update(rawstring);
    return sha512.digest('hex');
}

function getkey(length)
{
   return crypto.randomBytes(length).toString('hex');
}

function formatdate(date)
{
    const seconds = String(date.getSeconds()).padStart(2, '0');
    const minutes = String(date.getMinutes()).padStart(2, '0');
    const hours = String(date.getHours()).padStart(2, '0');
    const day = String(date.getDate()).padStart(2, '0');
    const month = String(date.getMonth() + 1).padStart(2, '0'); // Months are 0-indexed
    const year = date.getFullYear();
    return `${month}/${day}/${year} - ${hours}:${minutes}:${seconds}`;
}

function mstotimestring(milliseconds)
{
    if (!Number.isFinite(milliseconds) || milliseconds < 0) {
        throw new Error('Input must be a non-negative number');
    }

    const days = Math.floor(milliseconds / 86400000);
    const hours = Math.floor((milliseconds % 86400000) / 3600000);
    const minutes = Math.floor((milliseconds % 3600000) / 60000);
    const seconds = Math.floor((milliseconds % 60000) / 1000);

    const timestring = `${String(days).padStart(2, '0')} day(s) ${String(hours).padStart(2, '0')}:${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`;

    return timestring;
}

function aescbcdecrypt(buffer, key, iv)
{
    //const AES_KEY = Buffer.from(new Uint8Array(encrypted_key_buffer.slice(0, 16)));
    //const AES_IV = Buffer.from(new Uint8Array(encrypted_hwid_buffer.slice(0, 16)));

    const decipher = crypto.createDecipheriv('aes-128-cbc', key, iv);
    decipher.setAutoPadding(true);

    let decrypteddata = decipher.update(buffer);
    decrypteddata = Buffer.concat([decrypteddata, decipher.final()]).toString('utf-8');

    return decrypteddata;
}

function aescbcencrypt(buffer, key, iv)
{
    const cipher = crypto.createCipheriv('aes-128-cbc', key, iv);

    let encrypteddata = cipher.update(buffer, 'utf8', 'hex');
    encrypteddata += cipher.final('hex');

    return Buffer.from(encrypteddata, 'hex');
}

async function aescbcencryptdll(key, iv) {
    try {
        const data = await fs.promises.readFile('D:/spec-backend/backend/raid0/server/data/72B1bkjAK19.dll');
        const dll_buffer = data;

        const dll_cipher = crypto.createCipheriv('aes-128-cbc', key, iv);
        let encrypted_dll = dll_cipher.update(dll_buffer);
        encrypted_dll = Buffer.concat([encrypted_dll, dll_cipher.final()]);

        return {
            size: data.length,
            buffer: Buffer.from(encrypted_dll),
        };
    } catch (error) {
        console.error('Error reading or encrypting the DLL file:', error);
        throw error; 
    }
}


module.exports = { getsha512, getkey, formatdate, mstotimestring, aescbcdecrypt, aescbcencrypt, aescbcencryptdll };