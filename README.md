**Visualization for Code of Music**

`client/` contains p5.js sketch w/tone.js
`server/` contains basic node.js `ws` server and `osc` mediator.
`src/` and pretty much everything else is openFrameworks land.
`ofxOSC` is used in OF to receive messages.

Note to self:

-Start server before client.
-Ports are hardcoded atm.
-Make a shell script that just runs everything.
