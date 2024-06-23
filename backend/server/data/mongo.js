const { MongoClient } = require('mongodb');

const uri = 'mongodb+srv://busyy2007:zM8nlFwCEzkPJKFK@raid0.0cu14dw.mongodb.net/?retryWrites=true&w=majority&appName=raid0';
const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });

async function connect() {
  try {
    await client.connect();
    console.log('Connected to MongoDB');
    return client.db(); 
  } catch (err) {
    console.error('Error connecting to MongoDB:', err);
    throw err;
  }
}

module.exports = connect;