const express = require('express');
const mongo = require('../utilities/mongo');
const admininfo = require('../data/admins.json');
const utilities = require('../utilities/utilities');
const {Key, ExistingKey} = require('../data/keyclass')
const fs = require("fs");
const crypto = require("crypto");

const dllpath = ('../data/cheat.dll');
const version = 0.01;
const server = express();
const mongoinstance = mongo.connect();
const keycollection = mongoinstance.collection('keys');

const API_LOGGED_IN= -33819032
const API_EXPIRED_KEY= -33819008
const API_BAD_REQUEST= -33818976
const API_OUTDATED = -33818944
const API_UNAUTHORIZED= -33818912
const API_INVALID_KEY= -33818880

module.exports = server.post('/', async (req, res) =>
{
    try
    {
        let { key, hwid, mtd } = req.body;
        const response = { };

        if (!key || !hwid || !mtd)
        {
            response.status = "bad request";
            res.status(400).json(response);
            return;
        }

        const encrypted_key_buffer = new Uint8Array(key);
        const encrypted_hwid_buffer = new Uint8Array(hwid);

        const aeskey = Buffer.from(new Uint8Array(encrypted_key_buffer.slice(0, 16)));
        const aesiv = Buffer.from(new Uint8Array(encrypted_hwid_buffer.slice(0, 16)));

        const encryptedkey = Buffer.from(new Uint8Array(encrypted_key_buffer.slice(16)));
        const encryptedhwid = Buffer.from(new Uint8Array(encrypted_hwid_buffer.slice(16)));

        key = utilities.aescbcdecrypt(encryptedkey, aeskey, aesiv);
        hwid = utilities.aescbcdecrypt(encryptedhwid, aeskey, aesiv);

        console.log(key);

        /// DO DECRYPTION HERE

        const keydocument = await keycollection.findOne({ key: key });

        if (mtd !== version)
        {
            response.status = utilities.aescbcencrypt("outdated", aeskey, aesiv);
            response.statuslx = utilities.aescbcencrypt(API_OUTDATED.toString(), aeskey, aesiv);
            res.status(400).json(response);
            return;
        }

        if (!keydocument)
        {
            response.status = utilities.aescbcencrypt("invalid key", aeskey, aesiv);
            response.statuslx = utilities.aescbcencrypt(API_INVALID_KEY.toString(), aeskey, aesiv);
            res.status(404).json(response);
            return;
        }

        const foundkey = new ExistingKey(keydocument);

        if (foundkey.authentication.blacklisted)
        {
            response.status = utilities.aescbcencrypt("bad request", aeskey, aesiv);
            response.statuslx = utilities.aescbcencrypt(API_BAD_REQUEST.toString(), aeskey, aesiv);
            res.status(400).json(response);
        }

        if (foundkey.logging.bound_hwid === "")
        {
            foundkey.logging.bound_hwid = hwid;
            await foundkey.update_db();
        }

        if (foundkey.logging.bound_hwid !== hwid)
        {
            response.status = utilities.aescbcencrypt("unauthorized", aeskey, aesiv);
            response.statuslx = utilities.aescbcencrypt(API_UNAUTHORIZED.toString(), aeskey, aesiv);
            res.status(401).json(response);
            return;
        }

        if (!foundkey.authentication.activated)
        {
            foundkey.activate();
            await foundkey.update_db();
        }

        foundkey.logging.last_ip = req.ip || req.connection.remoteAddress;
        await foundkey.update_db();

        const remainingtime = foundkey.get_remaining_time();

        if (!remainingtime)
        {
            response.status = utilities.aescbcencrypt("expired key", aeskey, aesiv);
            response.statuslx = utilities.aescbcencrypt(API_EXPIRED_KEY.toString(), aeskey, aesiv);
            res.status(403).json(response);
            return;
        }

        const encrypteddll = await utilities.aescbcencryptdll(aeskey, aesiv);

        response.statuslb = encrypteddll.buffer;
        response.status = utilities.aescbcencrypt("success", aeskey, aesiv);
        response.statuslx = utilities.aescbcencrypt(API_LOGGED_IN.toString(), aeskey, aesiv);
        response.statusl = utilities.aescbcencrypt(foundkey.authentication.expiration_time.toString(), aeskey, aesiv);
        response.statusll = utilities.aescbcencrypt("asjkjsl!HJansbqajJUIQn", aeskey, aesiv);

        res.status(200).json(response);
    }
    catch(error)
    {
        res.status(400).json('bad request');
        console.log("$> " + error);
    }
});