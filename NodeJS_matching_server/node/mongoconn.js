const mongoOptions = { useUnifiedTopology : true };

let connectionString = process.env.MongoConnectionString;
if(connectionString == null)
{
    connectionString = 'Mongodb connection string env missing';
}

let dbName = process.env.MongoDataBaseName;
if(dbName == null)
{
    dbName = 'Mongodb db name string env missing';
}

let collectionName = process.env.MongoCollectionName;
if(collectionName == null)
{
    collectionName = 'Mongo collection name string env missing';
}

const MongoClient = require('mongodb').MongoClient;

let dbCollection;

module.exports = {
    connectToServer: function ()  {
        MongoClient.connect(connectionString, mongoOptions, (err, mongoClient) => {
    
            if (err != null) {
                console.error(err);
                return;
            }

            const db = mongoClient.db(dbName);
            dbCollection = db.collection(collectionName);
        });
    },

    getDbCollection: function () {
        return dbCollection;
    },
};