// import other routes
const vehicleRoutes = require('./vehicles');

const appRouter = (app, fs) => {

    // default route
    app.get('/', (req, res) => {
        res.send('welcome to my api-server');
    });

    // // other routes
    vehicleRoutes(app, fs);

};

module.exports = appRouter;
