const express = require('express');
const mongo = require('../utilities/mongo');
const admininfo = require('../data/admins.json');
const utilities = require('../utilities/utilities');
const {Key, ExistingKey} = require('../data/keyclass')

const server = express();
const mongoinstance = mongo.connect();
const keycollection = mongoinstance.collection('keys');

module.exports = server.post('/', async (req, res) =>
{
    try
    {
        const { key, hwid } = req.body;
        const response = { };

        if (!key || !hwid)
        {
            response.status = "bad request";
            res.status(400).json(response);
            return;
        }

        /// DO DECRYPTION HERE

        const keydocument = await keycollection.findOne({ key: key });

        if (!keydocument)
        {
            response.status = "invalid key";
            res.status(404).json(response);
            return;
        }

        const foundkey = new ExistingKey(keydocument);

        if (foundkey.authentication.blacklisted)
        {
            response.status = "bad request";
            res.status(400).json(response);
        }

        if (foundkey.logging.bound_hwid === "")
        {
            foundkey.logging.bound_hwid = hwid;
            await foundkey.update_db();
        }

        if (foundkey.logging.bound_hwid !== hwid)
        {
            response.status = "unauthorized";
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
            response.status = "expired key";
            res.status(403).json(response);
            return;
        }

        response.status = "sucess";
        response.long = remainingtime;
        response.string = foundkey.get_remaining_time_string();
        res.status(200).json(response);
    }
    catch(error)
    {
        res.status(400).json('bad request');
        console.log("$> " + error);
    }
});