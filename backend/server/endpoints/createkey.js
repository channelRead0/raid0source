const express = require('express');
const mongo = require('../utilities/mongo');
const admininfo = require('../data/admins.json');
const utilities = require('../utilities/utilities');
const {Key, ExistingKey} = require('../data/keyclass')

const server = express();

module.exports = server.post('/', async (req, res) =>
{
    try
    {
        const { user, password, duration } = req.body;
        const { admin_username, admin_password } = admininfo;

        if (!user || !password || !duration || duration < 0)
        {
            res.status(400).json('bad request');
            return;
        }

        if (admin_username !== user || admin_password !== utilities.getsha512(password))
        {
            res.status(401).json('unauthorized');
            return;
        }

        const CreatedKey = new Key(duration);
        await CreatedKey.insertdb();

        res.status(201).json(CreatedKey);
    }
    catch(error)
    {
        res.status(400).json('bad request');
        console.log("$> " + error); 
    }
});