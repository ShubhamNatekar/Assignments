
const vehicleRoutes = (app, fs) => {

    // variables
    const dataPath = './data/vins.json';

    // helper methods
    const readFile = (callback, returnJson = false, filePath = dataPath, encoding = 'utf8') => {
        fs.readFile(filePath, encoding, (err, data) => {
            if (err) {
                throw err;
            }

            callback(returnJson ? JSON.parse(data) : data);
        });
    };

    const writeFile = (fileData, callback, filePath = dataPath, encoding = 'utf8') => {

        fs.writeFile(filePath, fileData, encoding, (err) => {
            if (err) {
                throw err;
            }

            callback();
        });
    };

    // READ
    app.get('/vehicles', (req, res) => {
        fs.readFile(dataPath, 'utf8', (err, data) => {
            if (err) {
                throw err;
            }

            res.send(JSON.parse(data));
        });
    });
   app.post('/vehicles/:vin', (req, res, next) => {
   console.log('vehicles: ' + req.params.vin + ' Request received');
   let parsedContent = JSON.parse(req.query);
   let payload = req.body.payload; // your json data
   //res.status(201).send('success');
});


    // CREATE
    app.post('/vehicles', (req, res) => {

        readFile(data => {
            const newVehicleId = Object.keys(data).length + 1;

            // add the new vehicle
            data[newVehicleId.toString()] = req.body;

            writeFile(JSON.stringify(data, null, 2), () => {
                res.status(200).send('new vehicle added');
            });
        },
            true);
    });


    // UPDATE
    app.put('/vehicles/:id', (req, res) => {

        readFile(data => {

            // add the new vehicle
            const vehicleId = req.params["id"];
            data[vehicleId] = req.body;

            writeFile(JSON.stringify(data, null, 2), () => {
                res.status(200).send(`vehicles id:${vehicleId} updated`);
            });
        },
            true);
    });


    // DELETE
    app.delete('/vehicles/:id', (req, res) => {

        readFile(data => {

            // add the new vehicle
            const vehicleId = req.params["id"];
            delete data[vehicleId];

            writeFile(JSON.stringify(data, null, 2), () => {
                res.status(200).send(`vehicles id:${vehicleId} removed`);
            });
        },
            true);
    });
};

module.exports = vehicleRoutes;
