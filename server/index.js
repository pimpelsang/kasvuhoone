const express = require('express'),
	app = express(),
	bodyParser = require('body-parser'),
	port = process.env.PORT || 8080;

app.use(bodyParser.json())
app.use((req, res) => {
	res.setHeader('Content-Type', 'text/plain')
  	res.write('You sent:\n')
  	res.end(JSON.stringify(req.body, null, 2))
})

app.listen(port, () => console.log('Example app listening on port'+port))