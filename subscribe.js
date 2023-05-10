const mqtt = require('mqtt')

const url = "ws://test.mosquitto.org"

const options = {
    clean: true,
    clientId: 'mqttjs_' + Math.random().toString(16)
}

console.log("connecting to server")

const client  = mqtt.connect(url, options)

client.subscribe("real_unique_topic")