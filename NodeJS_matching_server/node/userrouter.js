const express = require('express');
const uuid4 = require('uuid4');

const router = express.Router();
router.use(express.json())

const dbo = require('./mongoconn.js');
dbo.connectToServer();

const redisdbo = require('./redisconn.js');
redisdbo.connectToRedis();

let expirationDuration = process.env.RedisExpirationDuration;
if(expirationDuration == null)
{
    expirationDuration = 1;
}

var ObjectId = require('mongodb').ObjectId;

const crypto = require('crypto');

let currentSessionsAndUsers = [];

let gamePort = parseInt(process.env.GamePort);
if(gamePort == null)
{
    gamePort = 4000;
}

let secret = process.env.SharedSecret;
if(secret == null)
{
    secret = 'shared secret env missing';
}

// create a user
router.post("/users/", async (req, res) =>{
    const usersCollection = dbo.getDbCollection();
    const query = { username : req.body.username };
    const foundUser = await usersCollection.findOne(query);

    if(foundUser){
        res.status(409);
        res.set("Content-Type", "text/plain");
        res.send('Duplicate User');
        return;
    }

    const newUser = {
        username: req.body.username,
        password: req.body.password,
        avatar: req.body.avatar,
    }

    await usersCollection.insertOne(newUser);

    res.status(200);
    res.set("Content-Type", "application/json");
    res.send({id : newUser._id, username : newUser.username, password : newUser.password, avatar: newUser.avatar});
});

// login to create an active session
router.post("/login", async (req, res) =>{
    const usersCollection = dbo.getDbCollection();
    const query = { username : req.body.username };
    const foundUser = await usersCollection.findOne(query);

    if(!foundUser){
        res.status(400);
        res.set("Content-Type", "text/plain");
        res.send('Bad Username');
        return;
    }

    if(foundUser.password !== req.body.password){
        res.status(403);
        res.set("Content-Type", "text/plain");
        res.send('Bad Password');
        return;
    }

    let currentUserIsOn = currentSessionsAndUsers.find(el => el.userId === foundUser._id.toString());
    
    const sessionId = uuid4();
    const redisClient = redisdbo.getRedisClient();

    // Delete the sessionId and userId pair on redis if there is an ongoing session for the current user
    if(!currentUserIsOn){
        const newSession = {
            sessionId : sessionId,
            userId : foundUser._id.toString()
        }

        currentSessionsAndUsers.push(newSession);
    }
    else{
        await redisClient.del(`sessions:${currentUserIsOn.sessionId}`)
                .catch((err)=> {
                    if(err != null){
                        console.error(err);
                        res.sendStatus(500);
                        return;
                    }
        });

        currentUserIsOn.sessionId = sessionId;
    }

    await redisClient.set(`sessions:${sessionId}`, foundUser._id.toString(), {EX : expirationDuration})
            .catch((err)=> {
                if(err != null){
                    console.error(err);
                    res.sendStatus(500);
                    return;
                }
        });

    res.status(200);
    res.set("Content-Type", "application/json");
    res.send({session : sessionId});
});

// search user using encoded path
router.get("/users/:id", async (req, res) =>{
    if(!req.body.session){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('No Session');
        return;
    }

    if(!uuid4.valid(req.body.session)){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('Bad Session');
        return;
    }

    const redisClient = redisdbo.getRedisClient();
    const foundSessionOwner = await redisClient
    .get(`sessions:${req.body.session}`)
    .catch((err)=>{
        if(err != null){
            console.error(err);
            res.sendStatus(404);
            return;
        }
    });


    if(!foundSessionOwner){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('Old Session');
        return;
    }
    
    
    if(!ObjectId.isValid(req.params.id)){
        res.status(404);
        res.set("Content-Type", "text/plain");
        res.send('Bad id/username');
        return;
    }
    
    const usersCollection = dbo.getDbCollection();
    const query = { _id : new ObjectId(req.params.id) };
    const foundUser = await usersCollection.findOne(query);

    res.status(200);
    res.set("Content-Type", "application/json");

    if(foundSessionOwner === foundUser._id.toString()){
        res.send({id : foundUser._id, username : foundUser.username, password : foundUser.password, avatar: foundUser.avatar});
    }
    else{
        res.send({id : foundUser._id, username : foundUser.username, avatar: foundUser.avatar});
    }
});

// search user using username
router.get("/users", async (req, res) =>{
    if(!req.query.username){
        res.status(400)
        res.set("Content-Type", "text/plain")
        res.send('No Username');
        return;
    }
    
    if(!req.body.session){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('No Session');
        return;
    }

    if(!uuid4.valid(req.body.session)){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('Bad Session');
        return;
    }

    const redisClient = redisdbo.getRedisClient();
    const foundSessionOwner = await redisClient
    .get(`sessions:${req.body.session}`)
    .catch((err)=>{
        if(err != null){
            console.error(err);
            res.sendStatus(404);
            return;
        }
    });

    if(!foundSessionOwner){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('Old Session');
        return;
    }

    const query = { username : req.query.username };
    const usersCollection = dbo.getDbCollection();
    const foundUser = await usersCollection.findOne(query);

    if(!foundUser){
        res.status(404);
        res.set("Content-Type", "text/plain");
        res.send('Bad id/username');
        return;
    }

    res.status(200);
    res.set("Content-Type", "application/json");

    if(foundSessionOwner === foundUser._id.toString()){
        res.send({id : foundUser._id, username : foundUser.username, password : foundUser.password, avatar: foundUser.avatar});
    }
    else{
        res.send({id : foundUser._id, username : foundUser.username, avatar: foundUser.avatar});
    }
});

// update user info using id
router.put("/users/:id", async (req, res) =>{
    if(!req.body.session){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('No Session');
        return;
    }

    if(!uuid4.valid(req.body.session)){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('Bad Session');
        return;
    }

    const redisClient = redisdbo.getRedisClient();
    const foundSessionOwner = await redisClient
    .get(`sessions:${req.body.session}`)
    .catch((err)=>{
        if(err != null){
            console.error(err);
            res.sendStatus(404);
            return;
        }
    });

    if(!foundSessionOwner){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('Old Session');
        return;
    }

    if(!ObjectId.isValid(req.params.id)){
        res.status(404);
        res.set("Content-Type", "text/plain");
        res.send('Bad id/username');
        return;
    }
    
    const query = { _id : new ObjectId(req.params.id) };
    const usersCollection = dbo.getDbCollection();
    const foundUser = await usersCollection.findOne(query);
    
    if(!foundUser){
        res.status(404);
        res.set("Content-Type", "text/plain");
        res.send('Bad id');
        return;
    }

    if(foundSessionOwner !== foundUser._id.toString()){
        res.status(403);
        res.set("Content-Type", "text/plain");
        res.send('Update on different user');
        return;
    }

    const setCommand = {$set : { username : req.body.username, password : req.body.password, avatar : req.body.avatar }}
    await usersCollection.findOneAndUpdate(query, setCommand);

    res.status(200);
    res.set("Content-Type", "application/json");
    res.send({id : req.params.id, username : req.body.username, password : req.body.password, avatar: req.body.avatar});
});

router.post("/connect", async (req, res) =>{
    if(!req.body.session){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('No Session');
        return;
    }

    if(!uuid4.valid(req.body.session)){
        res.status(401);
        res.set("Content-Type", "text/plain");
        res.send('Bad Session');
        return;
    }

    if(!req.body.game_type){
        res.status(400);
        res.set("Content-Type", "text/plain");
        res.send('No game type');
        return;
    }

    const currentUser = currentSessionsAndUsers.find(el => el.sessionId === req.body.session);
    const query = { _id : new ObjectId(currentUser.userId) };
    const usersCollection = dbo.getDbCollection();
    const foundUser = await usersCollection.findOne(query);

    
    const plaintextToken = foundUser.username + foundUser.avatar + req.body.game_type + secret;
    const outputToken = crypto.createHash('sha256').update(plaintextToken).digest('base64');

    res.status(200);
    res.set("Content-Type", "application/json");
    res.send({username : foundUser.username, avatar: foundUser.avatar, game_port: gamePort, token: outputToken});
});

module.exports = router;


