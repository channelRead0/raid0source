const mongoclient = require('mongodb').MongoClient;
const utilities = require('../utilities/utilities');

class Key
{
    constructor(duration)
    {
        this._id = utilities.getkey(16);
        this.key = utilities.getkey(16);
        this.authentication =
        {
            blacklisted: false,
            valid_duation: duration,
            activated: false,
            activation_time: 0,
            activation_time_string: "",
            expiration_time: 0,
            expiration_time_string: 0,
            others: [],
        };
        this.logging =
        {
            last_ip: "",
            bound_hwid: "",
        };
        this.client = {
            configs: [],
        };
    }

    get_json() {
        return JSON.stringify(this);
    }

    async insertdb()
    {
        const client = new mongoclient
        (
            'mongodb+srv://busyy2007:zM8nlFwCEzkPJKFK@raid0.0cu14dw.mongodb.net/?retryWrites=true&w=majority&appName=raid0',
            {
            }
        );

        try
        {
            await client.connect();
            const database = client.db('raid0');
            const collection = database.collection('keys');
            const result = await collection.insertOne(this);

            console.log(`Key saved with ID: ${result.insertedId}`);
        } finally {
            await client.close();
        }
    }
}

class ExistingKey
{
    constructor(data)
    {
        this._id = data._id;
        this.key = data.key;
        this.authentication =
            {
                blacklisted: data.authentication.blacklisted,
                valid_duation: data.authentication.valid_duation,
                activated: data.authentication.activated,
                activation_time: data.authentication.activation_time,
                activation_time_string: data.authentication.activation_time_string,
                expiration_time: data.authentication.expiration_time,
                expiration_time_string: data.authentication.expiration_time_string,
                others: data.authentication.others,
            };
        this.logging =
            {
                last_ip: data.logging.last_ip,
                bound_hwid: data.logging.bound_hwid,
            };
        this.client = {
            configs: data.client.configs,
        };
    }

    update_ip(ip_address) {
        this.logging.last_ip = ip_address;
    }

    update_hwid(hwid){
        this.logging.bound_hwid = hwid;
    }

    blacklist(status) {
        this.authentication.blacklisted = status;
    }

    add_config(config){
        this.client.configs.push(config);
    }

    activate()
    {
        const current_time = new Date();
        const expire_time = new Date();

        expire_time.setTime(current_time.getTime() + parseInt(this.authentication.valid_duation));

        this.authentication.activated = true;
        this.authentication.activation_time = current_time.getTime();
        this.authentication.activation_time_string = utilities.formatdate(current_time);
        this.authentication.expiration_time = expire_time.getTime();
        this.authentication.expiration_time_string = utilities.formatdate(expire_time);
    }

    extend_time(extension)
    {
        const current_time = new Date();
        const expire_time = new Date();

        expire_time.setTime(this.authentication.expiration_time + parseInt(this.authentication.valid_duation));

        this.authentication.expiration_time = expire_time.getTime();
        this.authentication.expiration_time_string = utilities.formatdate(expire_time);
    }

    get_remaining_time()
    {
        const current_time = new Date();

        if (current_time.getTime() > this.authentication.expiration_time) {
            return false;
        }

        return this.authentication.expiration_time - current_time.getTime();
    }

    get_remaining_time_string()
    {
        const current_time = new Date();

        if (current_time.getTime() > this.authentication.expiration_time) {
            return false;
        }

        return utilities.mstotimestring(this.authentication.expiration_time - current_time.getTime());
    }

    get_json() {
        return (this);
    }

    async update_db()
    {
        if (!this._id) {
            console.error('Cannot update: _id is not set.');
            return;
        }

        const client = new mongoclient
        (
            'mongodb+srv://busyy2007:zM8nlFwCEzkPJKFK@raid0.0cu14dw.mongodb.net/?retryWrites=true&w=majority&appName=raid0',
            {
            }
        );

        try {
            await client.connect();
            const database = client.db('raid0');
            const collection = database.collection('keys');

            const filter = { _id: this._id };

            const updated =
            {
                $set:
                {
                    key : this.key,
                    authentication :
                    {
                        blacklisted: this.authentication.blacklisted,
                        valid_duation: this.authentication.valid_duation,
                        activated: this.authentication.activated,
                        activation_time: this.authentication.activation_time,
                        activation_time_string: this.authentication.activation_time_string,
                        expiration_time: this.authentication.expiration_time,
                        expiration_time_string: this.authentication.expiration_time_string,
                        others: this.authentication.others,
                    },
                    logging :
                    {
                        last_ip: this.logging.last_ip,
                        bound_hwid: this.logging.bound_hwid,
                    },
                    client : {
                        configs: this.client.configs,
                    }
                }
            };

            const result = await collection.updateOne(filter, updated);

            console.log(`Key with ID ${this._id} updated. Matched ${result.matchedCount} document(s).`);
        } finally {
            await client.close();
        }
    }
}

module.exports = { Key, ExistingKey };