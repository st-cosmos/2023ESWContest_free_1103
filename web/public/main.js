var mapContainer = document.getElementById('map'), // 지도를 표시할 div 
mapOption = { 
    center: new kakao.maps.LatLng(37.630912, 127.079566), // 지도의 중심좌표
    level: 2 // 지도의 확대 레벨
};

var map = new kakao.maps.Map(mapContainer, mapOption); // 지도를 생성합니다

const imageSrc = "https://t1.daumcdn.net/localimg/localimages/07/mapapidoc/markerStar.png"; 
const imageSize = new kakao.maps.Size(24, 35); 
const markerImage = new kakao.maps.MarkerImage(imageSrc, imageSize); 

let isClickable = false;
let selectedDeviceId;

let targetPositionList = [];

// 지도에 클릭 이벤트를 등록합니다
// 지도를 클릭하면 마지막 파라미터로 넘어온 함수를 호출합니다
kakao.maps.event.addListener(map, 'click', (mouseEvent)=>{        
    if(isClickable) {
        // 클릭한 위도, 경도 정보를 가져옵니다 
        var latlng = mouseEvent.latLng;

        let marker = new kakao.maps.Marker({
            position: latlng,
            image : markerImage
        });
        marker.setMap(map);


        let info = new kakao.maps.InfoWindow({
            position : latlng, 
            content : `${selectedDeviceId} - Target`
        });
        info.open(map, marker);

        let path = drawPath(selectedDeviceId, latlng);
        axios.post('/Path', path);
        isClickable = false;
    }
});

const drawPath = (id, targetLatlng) => {
    let device = deviceList.find(device => device.id == id);
    let path = [
        new kakao.maps.LatLng(device.latitude, device.longitude),
        new kakao.maps.LatLng(37.630706, 127.079586),
        new kakao.maps.LatLng(37.630806, 127.079586),
        new kakao.maps.LatLng(37.630906, 127.079586),
        targetLatlng
    ];

    let polyline = new kakao.maps.Polyline({
        map: map,
        path: path,
        strokeWeight: 2,
        strokeColor: '#FF00FF',
        strokeOpacity: 0.8,
        strokeStyle: 'dashed'
    });

    let pathFormated = path.map(p => {
        return {
            "latitude" : p.Ma,
            "longitude" : p.La
        }
    });
    return {
        "id" : id,
        "status" : "Todo",
        "path" : pathFormated
    };
};

let deviceList = [];
const updateDevicePanel = () => {
    let devicePanel = document.getElementById("device-panel");
    devicePanel.innerHTML = ''; // clear element

    let dropdown = document.getElementById("dropdown");
    while (dropdown.options.length > 0) {
        dropdown.options[0].remove(); // Remove the first option in each iteration
    }

    deviceList.forEach(device => {
        devicePanel.innerHTML += `<div class="device-item">ID: ${device.id} | 위치: (${device.latitude.toFixed(5)}, ${device.longitude.toFixed(5)}) | ${device.numOfPeople}명</div>`;
        
        let newOption = document.createElement("option"); // Create a new option element
        newOption.value = device.id; // Set the value attribute of the option
        newOption.textContent = device.id; // Set the text content of the option
        dropdown.appendChild(newOption); // Append the option to the dropdown
        // console.log(device);
    });
};

let mapItemList = [];
const updateMarkers = () => {
    mapItemList.forEach(mapItem => {
        mapItem.marker.setMap(null);
        mapItem.info.open(null, mapItem.marker);
        mapItem.circle.setMap(null);
    });

    deviceList.forEach(device => {
        let position = new kakao.maps.LatLng(device.latitude, device.longitude);
        let marker = new kakao.maps.Marker({
            position: position
        });
        marker.setMap(map);

        let info = new kakao.maps.InfoWindow({
            position : position, 
            content : `${device.id} | ${device.numOfPeople} 명`
        });
        info.open(map, marker);

        let color;
        if(device.numOfPeople > 5) {
            color = "#eb8f8f";
        }
        else if(device.numOfPeople > 2) {
            color = "#edda8f";
        }
        else {
            color = "#8fed8f";
        }

        // 지도에 표시할 원을 생성합니다
        let circle = new kakao.maps.Circle({
            center : position,
            radius: 20, // 미터 단위의 원의 반지름입니다 
            fillColor: color, // 채우기 색깔입니다
            fillOpacity: 0.7,  // 채우기 불투명도 입니다
            strokeWeight: 0, // 선의 두께입니다
        }); 

        // 지도에 원을 표시합니다 
        circle.setMap(map);

        mapItemList = [];
        mapItemList.push({
            "marker" : marker,
            "info" : info,
            "circle" : circle
        });
    });
};

const updateDevice = () => {
    axios.get('/Device')
        .then((res)=>{
            // console.log(res.data);
            deviceList = res.data;
            updateDevicePanel();
            updateMarkers();
            
            // console.log(deviceList);
            let body = document.getElementById("main-section");
            body.style.display = 'none';
            body.style.display = 'grid';
        });
};

const setDevicePostion = () => {
    let dropdown = document.getElementById("dropdown");
    let selectedIndex = dropdown.selectedIndex;
    selectedDeviceId = dropdown.options[selectedIndex].value;
    isClickable = true;
};

let intervalId;
window.onload = () => {
    updateDevice();
    intervalId = setInterval(updateDevice, 1000);
};