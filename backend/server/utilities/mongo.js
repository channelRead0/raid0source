const MongoClient = require('mongodb');
//OLD
function Connect()
{
    try
    {
        const Client = new MongoClient.MongoClient
        (
            'mongodb+srv://busyy2007:zM8nlFwCEzkPJKFK@raid0.0cu14dw.mongodb.net/?retryWrites=true&w=majority&appName=raid0'
        );

        Client.connect();

        console.log('Connected to MongoDB');

        return Client.db('raid0');
    } catch (Exception)
    {
        console.error('Error connecting to MongoDB:', Exception);
        throw Exception;
    }
}

function Disconnect()
{
    try
    {
        const Client = new MongoClient.MongoClient
        (
            'mongodb+srv://busyy2007:zM8nlFwCEzkPJKFK@raid0.0cu14dw.mongodb.net/?retryWrites=true&w=majority&appName=raid0'
        );

        Client.close();

        console.log('Disconnected from MongoDB');

        return Client.db('raid0');
    } catch (Exception)
    {
        console.error('Error connecting to MongoDB:', Exception);
        throw Exception;
    }
}


module.exports = { Connect, Disconnect };