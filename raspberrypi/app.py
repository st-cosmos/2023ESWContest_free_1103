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
        cmd = "GET Position\r\n"
        py_serial.write(cmd.encode())
        for i in range(5):
            if py_serial.readable():
                response = py_serial.readline()
                print(response.decode())
                data = json.loads(response)

                if data["status"]:
                    return data
                else:
                    return None
        return None
    else:
        return {
            "status" : True,
            "latitude" : 37.630912,
            "longitude" : 127.079566,
        }

def send_position(data):
    url = f"{BASE_URL}/Device"
    # print(data)
    _ = requests.post(url, json=data)

def check_position():
    url = f"{BASE_URL}/Position"
    res = requests.get(url)
    return res.json()

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
        
        ## 위치 값 읽어오기
        position = get_position(arduino)
        if position is not None:
            data = {
                "numOfPeople" : num_of_people,
                "latitude" : position["latitude"],
                "longitude" : position["longitude"],
                "status" : 0
            }
        else:
            data = {
                "numOfPeople" : num_of_people,
                "latitude" : 0.,
                "longitude" : 0.,
                "status" : 1
            }
        send_position(data)

        target_position = check_position()
        # print(target_position)
        time.sleep(5)

if __name__ == '__main__':
    main()