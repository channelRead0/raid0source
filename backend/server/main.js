const express = require('express');
const bodyparser = require('body-parser');
const morgan = require('morgan');
const ratelimit = require('express-rate-limit');

const createkey = require('./endpoints/createkey');
const validatetime = require('./endpoints/validatetime');
const keyinfo = require('./endpoints/keyinfo');
const authenticate = require('./endpoints/authenticate');
const authenticateB = require('./endpoints/authenticateB');

const server = express();
server.use(bodyparser.json());
server.use(morgan(':method :url :status :response-time ms - :res[content-length]'));
server.use(morgan('dev'));

const ratelimiter = ratelimit
(
{
  windowMs: 15 * 60 * 1000,
  max: 100,
});



server.use(ratelimiter);
server.use('/createkey', createkey);
server.use('/validatetime', validatetime);
server.use('/keyinfo', keyinfo);
server.use('/authenticate', authenticate);
server.use('/authenticate-b', authenticateB);

server.use((err, req, res, next) =>
{
    console.error(err.stack);
    res.status(500).send('Internal Server Error');
});

server.listen(80, () => {
    console.log(`$> Server is listening on port 8080`);
});