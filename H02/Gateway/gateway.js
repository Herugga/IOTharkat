var parameters = process.argv;
var flags = {'-s': null, '-p': null, '-a': null}

// handle the command line parameters
if (parameters.length > 2)
{
	console.log(parameters.length)
	if (parameters.length % 2 != 0)
	{
		console.log("Incorrect parameters.")
		process.exit()
	}
	else
	{
		for(var i = 2; i < parameters.length-1; i++)
		{
			flags[parameters[i]] = parameters[i+1];
			i++;
		}
	}	
}
else
{
	console.log("Serialportname required.")
	process.exit();
}


// initializing the requirements for serial port communication
var portname = flags['-s'];
var serial = require("serialport");
var serialport = new serial(portname, {
	baudrate: 9600,
	parser: serial.parsers.readline("\n")
});
var serialopen = false;

// initializing the requirements for the tcp-connection to server
var net = require('net');
// default, changed if different port given as command line parameter
var servport = 6969;
if (flags['-p'] != null)
{
	servport = flags['-p'];
}
// default, changed if different address is given as command line parameter
var servhost = 'localhost';
if (flags['-'] != null)
{
	servhost = flags['-a']
}

var respport;


// Opening serial port
serialport.on('open', function(){
	console.log("Serial port " + portname + " opened");
	serialopen = true;
});

serialport.on('error', function(err){
	console.log('Serialport error: ', err.message);
})

// When receiving data, format it correctly and send to server
serialport.on('data', function(data){
	var dataInJSON = parseMessage(data);
	console.log(data);
	sendData(dataInJSON);
})



function parseMessage(data)
{
	// Implement your message parsing and converting to JSON format
}


function sendData(data)
{
	var c = net.createConnection(servport, servhost);
	c.on('connect', function(){
		c.write(data);
	})
}
