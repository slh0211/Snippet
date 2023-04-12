const redis = require('redis');

let redisAddress = process.env.RedisAddress;
if(redisAddress == null)
{
    redisAddress = 'redis address env missing';
}

let redisPort = process.env.RedisAddress;
if(redisPort == null)
{
    redisPort = 'redis port env missing';
}

const redisClient = redis.createClient(redisPort, redisAddress);

module.exports = {
    connectToRedis: function (){
        redisClient.on('error', (e) => console.log(e));
        (async ()=> {
            await redisClient.connect();
        })();
    },

    getRedisClient: function (){
        return redisClient;
    },
};