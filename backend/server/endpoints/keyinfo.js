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
        const { user, password, key } = req.body;
        const { admin_username, admin_password } = admininfo;

        if (!user || !password || !key)
        {
            res.status(400).json('bad request');
            return;
        }

        if (admin_username !== user || admin_password !== utilities.getsha512(password))
        {
            res.status(401).json('unauthorized');
            return;
        }

        const keydocument = await keycollection.findOne({ key: key });

        if (!keydocument)
        {
            res.status(404).json('invalid key');
            return;
        }

        const foundkey = new ExistingKey(keydocument);
        const response = foundkey;

        if (!foundkey.get_remaining_time()){
            response.authentication.expired = !!foundkey.authentication.activated;
        }
        else
        {
            response.authentication.expired = false;
            response.authentication.remaining_time = foundkey.get_remaining_time_string();
            response.authentication.remaining_time_long = foundkey.get_remaining_time();
        }

        res.status(200).json(response);
    }
    catch(error)
    {
        res.status(400).json('bad request');
        console.log("$> " + error);
    }
});