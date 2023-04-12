const express = require('express');

let port = process.env.PORT;
if(port == null){
    port = 4000;
}

const app = express();
const userRouter = require('./userrouter.js');

app.use('/api/v1', userRouter);
app.listen(port);