// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Imports
const express = require('express');
const mqtt = require('mqtt');
const cors = require("cors");

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Server Vars
const exprPort = 3000
var instanceList = []; //not used yet
const activeInstances = new Map();
const testInstanceConfig = {
  instanceId: "inid001",
  maxParts: 5,
  roles:{
    agent:{
      match: "agent.*",
      publish: [],
      subscribe: ["/comm/{id}/in"]
    },
    frontend:{
      match: "front.*",
      publish: [],
      subscribe: ["/comm/{id}/in"]
    },
    MCU:{
      match: "mcu.*",
      publish: ["/comm/{id}/out"],
      subscribe: ["/comm/{id}/in"]
    },

  }
}; // this is purely for debugging

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Functions
function createInstance(config)
{
  const instance = {
    instanceId: config.instanceId,
    config,
    state: "RUNNING",
    participants: [],
  };

  const topics = {
    root: `instance/${config.instanceId}`,
    comm: `instance/${config.instanceId}/comm`,
    events: `instance/${config.instanceId}/events`,
    system: `instance/${config.instanceId}/system`
  };

  instance.topics = topics;

  activeInstances.set(config.instanceId, instance);

  return instance;
}



// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// HTTP / HTTPS implementation
var app = express();
app.use(express.json());
app.use(cors());

app.get('/', (req, res) => {
  res.send('Hello World!')
})


app.post("/join", (req, res) => {
  console.log("JOIN HIT");
  const token = req.body.token;
  const instanceId = req.body.instanceId;
  
  console.log("join req", req)

  //Needs "validateToken()" eventually.

  res.json({
    success: true,
    instanceId,
  });
  
  
})


app.get("/api", (req, res) => {
  console.log("access attempt")
  res.json({ message: "Hello from Express!" });
});

app.listen(exprPort, () => {
  console.log(`Listening at http://localhost:${exprPort}`)
})







// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MQTT Broker
const broker = mqtt.connect('mqtt://localhost:1883');

broker.on('connect', () => {
  console.log('Connected to broker');
  broker.subscribe('test/topic');
});

broker.on('message', (topic, message) => {
  console.log(topic, message.toString());
});

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// MQTT Implementation
