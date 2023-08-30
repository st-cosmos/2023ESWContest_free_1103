import cv2
import numpy as np
from app import load_yolo, detect_people_yolo

def main():
    net, classes, output_layers = load_yolo()

    # Create a VideoCapture object to access the webcam
    cap = cv2.VideoCapture(0)

    # Check if the webcam is opened correctly
    if not cap.isOpened():
        print("Error: Webcam cannot be accessed.")
        return

    # Read a frame from the webcam
    ret, frame = cap.read()

    if not ret:
        print("Error: Failed to read frame.")
        return

    # Detect people and get the count
    num_of_people = detect_people_yolo(frame, net, output_layers)
    print("Number of people:", num_of_people)

    # Release the VideoCapture
    cap.release()

if __name__ == "__main__":
    main()
