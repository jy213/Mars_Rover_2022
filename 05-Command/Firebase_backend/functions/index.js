const functions = require("firebase-functions");
const express = require("express");
const cors = require("cors");
// const { firebaseConfig } = require("firebase-functions");
const admin = require("firebase-admin")
// import "firebase/analytics";

const rover_app = express();
rover_app.use(cors());
const radar_app = express();
radar_app.use(cors());
const alien_app = express();
alien_app.use(cors());
const new_alien_app = express();
new_alien_app.use(cors());
const mannual_app = express();
mannual_app.use(cors());
const mode_app = express();
mode_app.use(cors());
const status_app = express();
status_app.use(cors());
const route_app = express();
route_app.use(cors())

var serviceAccount = require('./admin.json');
const { ref } = require("firebase-functions/v1/database");
admin.initializeApp({
credential: admin.credential.cert(serviceAccount),
databaseURL: "https://rover-back-end-default-rtdb.europe-west1.firebasedatabase.app",
authDomain: "rover-back-end.firebaseapp.com",
});
// const analytics = firebase.analytics();




var db = admin.database();
const ref_rover = db.ref("/rover_loc");
const ref_alien = db.ref("/alien_loc");
const ref_mannual = db.ref("/mannual_instruction");
const ref_mode = db.ref("/mode");
const ref_radar = db.ref("/radar_loc")
const ref_status = db.ref("/Server_Status")
const ref_route = db.ref("/Route_Planning")



route_app.get('/', async (req,res)=>{
    console.log('rover route')
    ref_route
    .once("value")
    .then((snapshot) => {
        var data = snapshot.val();  //Data is in JSON format.
        // new_data=JSON.stringify(data[data.length-1]);
        data_final = new Array;
        for (let i = 0; i < data.length; i++) {
          //console.log("json.parse: ",data_alien[i]);
          const tmp_data = JSON.stringify(data[i]); 
          const obj = JSON.parse(tmp_data);
          data_final.push(obj);
        }
        res.send(data_final);
    });
})


status_app.get('/', async (req,res)=>{
    console.log('status')
    ref_status
    .once("value")
    .then((snapshot) => {
        var data = snapshot.val();  //Data is in JSON format.
        console.log("status: ", data);
        status_data=JSON.stringify(data);
        const obj = JSON.parse(status_data);
        console.log(obj); 
        res.send(obj['Server_Status']);
    });
})

rover_app.get('/', async (req,res)=>{
    console.log('rover site')
    ref_rover
    .once("value")
    .then((snapshot) => {
        var data = snapshot.val();  //Data is in JSON format.
        console.log(data);
        new_data=JSON.stringify(data[data.length-1]);
        const obj = JSON.parse(new_data);
        console.log(obj); 
        res.send([obj['x_value'],obj['y_value']]);
    });
})

alien_app.get('/', async (req,res)=>{
    console.log('alien site')
    ref_alien
    .once("value")
    .then((snapshot) => {
        var data_alien = snapshot.val();  //Data is in JSON format.
        // new_data=JSON.stringify(data[data.length-1]);
        data_alien_final = new Array;
        for (let i = 0; i < data_alien.length; i++) {
          //console.log("json.parse: ",data_alien[i]);
          const tmp_data = JSON.stringify(data_alien[i]); 
          const obj = JSON.parse(tmp_data);
          data_alien_final.push(obj);
        }
        res.send(data_alien_final);
    });
})

new_alien_app.get('/', async (req,res)=>{
    ref_alien
    .once("value")
    .then((snapshot) => {
        var data_alien = snapshot.val();  //Data is in JSON format.
        console.log("data_fetched", data_alien)
        new_data=JSON.stringify(data_alien[data_alien.length-1]);
        console.log("newest_data", new_data)
        const obj = JSON.parse(new_data);
        res.send(obj);
    });
})

radar_app.get('/', async (req,res)=>{
    ref_radar
    .once("value")
    .then((snapshot) => {
        var data_radar = snapshot.val();  //Data is in JSON format.
        console.log("data_fetched", data_radar)
        new_data=JSON.stringify(data_radar);
        const obj = JSON.parse(new_data);
        res.send(obj);
    });
})

mannual_app.post('/', async function(req, res){
    console.log('mannual site')
    console.log("data_transmitted", req.body)
    direction=JSON.stringify(req.body);
    ref_mannual.push(direction)
});

mode_app.post('/', async function(req, res){
    console.log('switch mode')
    console.log("data_transmitted", req.body)
    mode=JSON.stringify(req.body);
    ref_mannual.push(mode)
});

exports.rover_1 = functions.https.onRequest(rover_app);
exports.alien_1 = functions.https.onRequest(alien_app);
exports.new_alien_app = functions.https.onRequest(new_alien_app);
exports.radar_loc = functions.https.onRequest(radar_app);
exports.mannual_app = functions.https.onRequest(mannual_app);
exports.mode_app = functions.https.onRequest(mode_app);
exports.status_app = functions.https.onRequest(status_app);
exports.route = functions.https.onRequest(route_app);




// exports.rover = functions.https.onRequest((req, res) => {
//     console.log('rover site')
//     ref_rover
//     .once("value")
//     .then((snapshot) => {
//         var data = snapshot.val();  //Data is in JSON format.
//         new_data=JSON.stringify(data[data.length-1]);
//         const obj = JSON.parse(new_data);
//         console.log(obj); 
//         res.send([obj['x-axis'],obj['y-axis']]);
//     });
// });

// exports.alien = functions.https.onRequest((req, res) => {
//     console.log('alien site')
//     ref_alien
//     .once("value")
//     .then((snapshot) => {
//         var data_alien = snapshot.val();  //Data is in JSON format.
//         // new_data=JSON.stringify(data[data.length-1]);
//         data_alien_final = new Array;
//         for (let i = 0; i < data_alien.length; i++) {
//           //console.log("json.parse: ",data_alien[i]);
//           const tmp_data = JSON.stringify(data_alien[i]); 
//           const obj = JSON.parse(tmp_data);
//           data_alien_final.push(obj);
//         }
//         res.send(data_alien_final);
//     });
// });


// var data_alien = snapshot.val();  //Data is in JSON format.
// // new_data=JSON.stringify(data[data.length-1]);
// data_alien_final = new Array;
// for (let i = 0; i < data_alien.length; i++) {
//   //console.log("json.parse: ",data_alien[i]);
//   const tmp_data = JSON.stringify(data_alien[i]); 
//   const obj = JSON.parse(tmp_data);
//   data_alien_final.push(obj);
//   console.log("push", obj);
//   console.log("array", data_alien_final);
// }

// port=8001;
// app.listen(port,(err)=>err?console.log(err):console.log(`Server Running on port ${port}`));

// app.get('/data', (req,res) => {
//     console.log('data site')
//     ref
//     .once("value")
//     .then((snapshot) => {
//       res.send(snapshot.val());
//     });
// });

// app.post('/', async (req,res) => {
//     const user = req.body;
//     ref.set({user});
//     // await admin.firestore().collection('users').add(user);
//     res.status(201).send();
// })

// exports.helloWorld = functions.https.onRequest((request, response) => {
//     functions.logger.info("Hello logs!", {structuredData: true});
//     response.send("Hello from Firebase!");
//   });

// exports.user = functions.https.onRequest(app);
// // Create and Deploy Your First Cloud Functions
// // https://firebase.google.com/docs/functions/write-firebase-functions
//
// exports.alien = functions.https.onRequest((req, res) => {
//     console.log('data site')
//     ref_rover
//     .once("value")
//     .then((snapshot) => {
//       res.send(snapshot.val());
//     });
// });

