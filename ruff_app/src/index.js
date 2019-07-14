'use strict';
// 光感器
var lightSensor;
var mqtt = require('mqtt');
var client = mqtt.connect('mqtt://XXXXXXXXX:1883');
// 温度，光强，湿度
var data = {Temperature:"", Illuminance:"", Humidity:""};

$.ready(function (error) {
    if (error) {
        console.log(error);
        return;
    }
	
	$('#sound-01').on('sound', function() {
		// console.log('sound detected');
		Blinks();
	});
	// Blinks();
	
	lightSensor = $('#gy-30');
	// 每秒测一下光强
    setInterval(showLightIntensity, 1000);
    // 每秒测一下温度、湿度
	setInterval(showTemperatureHumidity, 1000);
    // 订阅“IOT”主题
	client.on('connect', function () {
	  client.subscribe('IOT');
	});
    // 每5秒发送一下光强、湿度、温度
	setInterval(send, 5000);
	
});


$.end(function () {
	// client.close();
});

function send() {
	  client.publish('IOT', JSON.stringify(data));
	  console.log('sended!!!');
}

function showLightIntensity() {
    //从光照传感器获取光照强度
    lightSensor.getIlluminance(function (error, value) {
        if (error) {
            console.error(error);
            return;
        }
        data.Illuminance = value.toString();
        console.log('illuminance: ' + value);
    });
}

function showTemperatureHumidity() {
	$('#dht11').getTemperature(function (error, temperature) {
		if (error) {
			console.error(error);
			return;
		}
		data.Temperature = temperature.toString();
		console.log('temperature', temperature);
	});

	$('#dht11').getRelativeHumidity(function (error, humidity) {
		if (error) {
			console.error(error);
			return;
		}
		data.Humidity = humidity.toString();
		console.log('humidity', humidity);
	});
}

// 红灯闪一下
function Blink() {
    if (!$('#led-r').isOn()) {
        $('#led-r').turnOn();
    }
    else {
        $('#led-r').turnOff();
    }
}

// 红灯闪连续3下
function Blinks() {
    $('#led-r').turnOn();
    setTimeout(Blink, 100);
    setTimeout(Blink, 200);
    setTimeout(Blink, 300);
    setTimeout(Blink, 400);
}