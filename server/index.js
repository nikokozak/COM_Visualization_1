const osc = require('osc');
const WebSocket = require('ws');

const udpPort = new osc.UDPPort({
  localAddress: "0.0.0.0",
  localPort: 57575,
  remoteAddress: "0.0.0.0",
  remotePort: 55555
})

let udpPortReady = false;
udpPort.on('ready', () => {
  console.log(`Listening to UDP on port ${udpPort.options.localPort}`)
  udpPortReady = true;
})

// Create WebSocket server
const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', (ws) => {
  console.log('Client connected');
  
  ws.on('message', (data) => {
    const message = JSON.parse(data);
    //console.log(message);
    
    if (message.frequencyData) {
      udpPort.send({
        address: "/fft",
        args: message.frequencyData.map(value => ({
          type: "f",  // float type
          value: value
        }))
      });
    } else if (message.key) {
      udpPort.send({
        address: "/keypress",
        args: [{
          type: "s",  // string type
          value: message.key
        }]
      });
    }
  });
});

udpPort.on('message', (oscMessage) => { console.log(oscMessage) })

udpPort.on('error', (oscError) => { console.log(oscError) })

udpPort.open();
