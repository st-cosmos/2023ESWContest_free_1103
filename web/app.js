const ip = require('ip');
const express = require('express');
const cors = require('cors');
const path = require('path');

const app = express();
app.use(express.static('public'));
app.use(express.json());
app.use(cors());

const IP_ADDRESS = ip.address();
const PORT = 9999;

app.get('/admin', (req, res)=>{
    res.sendFile(path.join(__dirname, 'public/index.html'));
});

let deviceList = [
    
];

// let deviceList = [
//     {
//         "id" : "C001",
//         "latitude" : 37.630212,
//         "longitude" : 127.079566,
//         "numOfPeople" : 5,
//     },
//     {
//         "id" : "C002",
//         "latitude" : 37.630706,
//         "longitude" : 127.079666,
//         "numOfPeople" : 5,
//     },
// ];

app.get('/Device', (req, res)=>{
    // console.log('GET', deviceList);
    res.send(deviceList);
});

app.post('/Device', (req, res)=>{
    let data = req.body;
    // console.log(data);

    let index = deviceList.findIndex(device => device.id == data.id);
    if(index >= 0) {
        deviceList[index].latitude = data.latitude;
        deviceList[index].longitude = data.longitude;
        deviceList[index].numOfPeople = data.numOfPeople;
    }
    else {
        deviceList.push(data);
    }
    res.send(data);
});

let pathList = [

];

// let pathList = [
//     {
//         "id" : "C001",
//         "status" : "Todo",
//         "path" : [
//             {
//                 "latitude" : 37.630706,
//                 "longitude" : 127.079586
//             },
//             {
//                 "latitude" : 37.630806,
//                 "longitude" : 127.079586
//             },
//             {
//                 "latitude" : 37.630906,
//                 "longitude" : 127.079586
//             },
//         ]
//     }
// ]

app.get('/Path', (req, res)=>{
    let id = req.query.id;
    let path = pathList.find(path => path.id == id);
    
    if(path == undefined) {
        res.status(404).send({});
    }
    else {
        res.send(path);
    }
});

app.post('/Path', (req, res)=>{
    let data = req.body;
    // console.log(data);

    let index = pathList.findIndex(path => path.id == path.id);
    if(index > 0) {
        pathList[index].status = data.status;
        pathList[index].path = data.path;
    }
    else {
        pathList.push(data);
    }
    console.log(pathList);
    res.send(data);
});

app.listen(PORT, ()=>console.log(`Server is running | http://${IP_ADDRESS}:${PORT}`));
