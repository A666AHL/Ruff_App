var mqtt = require('mqtt');
var express = require("express");
var app = express();
var hostName = '127.0.0.1'; //http服务的提供服务ip
var port = 1883;
//有两个需要用到的源 npm install mqtt  npm install mosca
app.get("/app", function(req, res) {
    req.setTimeout(200); 
    var client = mqtt.connect('mqtt://127.0.0.1:1883', {
        username: 'abc',
        password: 'abc',
        clientId: 'ap'
    });
    client.on('connect', function() {
        client.subscribe('IOT', { qos: 1 });
    });
    client.on('message', function(topic, message) { 
	   console.log(message.toString())
    });