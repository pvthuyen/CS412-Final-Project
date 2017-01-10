var express = require('express');
var app = express();
var fs = require("fs");
var multer = require('multer');
var net = require('net');

var HOST = '127.0.0.1';
var PORT = 6969;

//console.log(multer);

var upload = multer({ dest: '/tmp/'});

// set the view engine to ejs
app.set('view engine', 'ejs');

app.get('/', function(req, res) {
  res.sendFile(__dirname + '/index.html');
})

// File input field name is simply 'file'
app.post('/file_upload', upload.single('file'), function(req, res) {
  var file = __dirname + '/core/data/queries/' + req.file.filename + '.jpg';
  var outputFileName;
  fs.rename(req.file.path, file, function(err) {
    if (err) {
      console.log(err);
      res.send(500);
    }
    else {
      var client = new net.Socket();
      client.connect(PORT, HOST, function() {

        console.log('CONNECTED TO: ' + HOST + ':' + PORT);
        // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client 
        client.write(req.file.filename + '.jpg');

      });

      // Add a 'data' event handler for the client socket
      // data is what the server sent to this socket
      client.on('data', function(data) {

        console.log('DATA: ' + data);
          // Close the client socket completely
          client.destroy();
          outputFileName = data;
        });

      // Add a 'close' event handler for the client socket
      client.on('close', function() {
        console.log('Connection closed');
        res.writeHead(302, {
          'Location': '/result?filename=' + outputFileName
          //add other headers here...
        });
        res.end();
      });
  }
});
});

// display result images
app.get('/result', function(req, res) {
   var array = fs.readFileSync(__dirname + '/core/data/rankedlists/' + req.query.filename).toString().split("\n");
   // var imageLists = '<ul>';
   // for (var i=0; i<array.length; i++) {
   //    imageLists += '<li><img src="' + array[i] + '.jpg' + '"></li>';
   // }
   // imageLists += '</ul>';

   // res.writeHead(200, {'Content-type':'text/html'});
   // res.end(imageLists);
   res.render('result', {images: array});
}
)  


app.use(express.static(__dirname + '/core/data/images'));

var server = app.listen(8081, function () {
 var host = server.address().address
 var port = server.address().port

 console.log("Example app listening at http://%s:%s", host, port)
})