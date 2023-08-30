'''
주기적으로 돌면서
1. 사람 수 카운트 해서 보내기
2. 내 위치 보내기
3. 어디로 가야할지 확인하기
'''
import cv2
import numpy as np
import time
import serial
import json
import requests

TIME_INTERVAL = 5
BASE_URL = 'http://localhost:9999' # 상황에 맞게 바꾸기
DEVICE_ID = "C001"

lat_prev = None
lng_prev = None

# Load YOLO model
def load_yolo():
    net = cv2.dnn.readNet("yolov4-tiny.weights", "yolov4-tiny.cfg")
    classes = []
    with open("coco.names", "r") as f:
        classes = [line.strip() for line in f.readlines()]
    layers_names = net.getLayerNames()
    output_layers = [layers_names[i - 1] for i in net.getUnconnectedOutLayers()]
    return net, classes, output_layers

def init_camera():
    # Create a VideoCapture object to access the webcam
    cap = cv2.VideoCapture(0)

    # Check if the webcam is opened correctly
    if not cap.isOpened():
        print("Error: Webcam cannot be accessed.")
        return None
    
    else:
        return cap

def get_frame(cap):
    # Read a frame from the webcam
    ret, frame = cap.read()

    if not ret:
        print("Error: Failed to read frame.")
        return None
    
    else:
        return frame

# Detect people using YOLO model
def detect_people_yolo(frame, net, output_layers, confidence_threshold=0.5, nms_threshold=0.3):
    height, width, channels = frame.shape

    blob = cv2.dnn.blobFromImage(frame, scalefactor=1/255.0, size=(416, 416), swapRB=True, crop=False)
    net.setInput(blob)
    outs = net.forward(output_layers)

    class_ids = []
    confidences = []
    boxes = []

    for out in outs:
        for detection in out:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]

            if confidence > confidence_threshold and class_id == 0:
                center_x = int(detection[0] * width)
                center_y = int(detection[1] * height)
                w = int(detection[2] * width)
                h = int(detection[3] * height)
                x = int(center_x - w / 2)
                y = int(center_y - h / 2)
                
                boxes.append([x, y, w, h])
                confidences.append(float(confidence))

    indices = cv2.dnn.NMSBoxes(boxes, confidences, confidence_threshold, nms_threshold)
    num_of_people = len(indices)
    
    return num_of_people

def init_serial():
    py_serial = serial.Serial(
        port='/dev/ttyUSB1',
        baudrate=9600,
    )
    # print(py_serial)
    time.sleep(0.3)
    return py_serial

def get_position(py_serial):
    if py_serial:
        cmd = json.dumps({"method":"get", "resource":"position"})
        py_serial.write(cmd.encode())
        for i in range(5):
            if py_serial.readable():
                response = py_serial.readline()
                print(response.decode())
                data = json.loads(response)
                return data
        return None
    else: # for test purpose
        return {
            "latitude" : 37.630912,
            "longitude" : 127.079566,
        }

def go_to_position(py_serial, position):
    if position["latitude"] != lat_prev or position["longitude"] != lng_prev:
        cmd = json.dumps({
                "method":"set",
                "resource":"position",
                "latitude":position["latitude"],
                "longitude":position["longitude"],
            })
        py_serial.write(cmd.encode())
        
        lat_prev = position["latitude"]
        lng_prev = position["longitude"]

def move_around(py_serial):
    cmd = json.dumps({
            "method":"set",
            "resource":"moveAround"
        })
    py_serial.write(cmd.encode())

def send_position(data):
    url = f"{BASE_URL}/Device"
    # print(data)
    _ = requests.post(url, json=data)

def check_path():
    url = f"{BASE_URL}/Path?id={DEVICE_ID}"
    res = requests.get(url)
    return res.json()

def update_path(data):
    url = f"{BASE_URL}/Path"
    # print(data)
    _ = requests.post(url, json=data)

def is_in_range(current, target):
    if current["latitude"] in range(target["latitude"] - 0.00005, target["latitude"] + 0.00005) and current["longitude"] in range(target["longitude"] - 0.00005, target["longitude"] + 0.00005):
        return True
    else:
        return False

def main():
    cap = init_camera()
    net, _, output_layers = load_yolo()
    # arduino = init_serial()
    arduino = None

    while True:
        ## 사람 수 세기
        frame = get_frame(cap)
        if frame is not None:
            num_of_people = detect_people_yolo(frame, net, output_layers)
            # print(num_of_people)
        
        ## 위치 값 읽어와서 서버로 보내기
        position = get_position(arduino)
        if position is not None:
            data = {
                "id" : "C0001",
                "latitude" : position["latitude"],
                "longitude" : position["longitude"],
                "numOfPeople" : num_of_people,
            }
            send_position(data)

        ## 가야할 곳 확인해서 1) 있으면 가기 2) 없으면 주변 서성이기
        target_path = check_path()
        # print(target_path)
        if len(target_path.keys()):
            if len(target_path["path"]):
                target_path["status"] = "Inprogress"
                
                if is_in_range(position, target_path["path"][0]):
                    target_path["path"].remove(target_path["path"][0])
                    update_path(target_path)
                else:
                    go_to_position(arduino, target_path["path"][0])
            else:
                target_path["status"] = "Done"
                move_around(arduino)
        else:
            move_around(arduino)
        time.sleep(TIME_INTERVAL)

if __name__ == '__main__':
    main()