module.exports = {
  apps : [{
    name: 'CS261_Assignment6',
    script: 'assignment6.js',
    instances: 1,
    autorestart: true,
    max_memory_restart: '1G',
    watch: false,
    env: {
      PORT: 3100,
      MongoConnectionString: 'mongodb://localhost:27017',
      MongoDataBaseName: 'assignment6',
      MongoCollectionName: 'users',
      RedisAddress: '127.0.0.1',
      RedisPort: 6379,
      RedisExpirationDuration: 10,
      GamePort: 4200,
      SharedSecret: "CS261S21"
    }
  }],
};
