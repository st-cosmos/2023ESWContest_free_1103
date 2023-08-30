void initGps() {
    gps.begin(9600); // Hardware Tx1, RX0 Pin
}

void getCurrentPosition(double& latValue, double& lngValue) {
    if (gps.available()){
        String string = gps.readStringUntil('$');
        GPosition lat, lng;
        int ret = getLatAndLng(string, lat, lng);
        if (ret) {
            // printGPosition(lat); Serial.print(" | "); printGPosition(lng); Serial.println("");
            latValue = getGPositionValue(lat);
            lngValue = getGPositionValue(lng);
            // Serial.print("Target:\t\t"); Serial.print(LAT_TARGET, 5); Serial.print("\t|\t"); Serial.println(LNG_TARGET, 5); 
            // Serial.print("Current:\t"); Serial.print(latValue, 5); Serial.print("\t|\t"); Serial.println(lngValue, 5); 
        }
    }    
}

void splitString(char* input, char* delimiter, char** output, int max_parts = MAX_STRING_PARTS) {
    int count = 0;
    char* token = strtok(input, delimiter);
    
    while (token != NULL && count < max_parts) {
        output[count++] = token;
        token = strtok(NULL, delimiter);
    }
}

void convertRawToGPos(GPosition& pos, double raw_value) {
    pos.degree = (int) (raw_value / 100);
    pos.minute = ((int) floor(raw_value)) % 100;
    pos.second = (raw_value - floor(raw_value)) * 60;
}

int getLatAndLng(String& string, GPosition& lat, GPosition& lng) {
    char* parts[MAX_STRING_PARTS];
    splitString(string.c_str(), ",", parts, MAX_STRING_PARTS);
    if(!String(parts[0]).compareTo("GPGGA")) {
        /*
        * TODO: resolve toDouble error issue
        */
        //Serial.println(string_to_print);
        
        double lat_raw = String(parts[2]).toDouble();
        double lng_raw = String(parts[4]).toDouble();

        if(lat_raw > 0) convertRawToGPos(lat, lat_raw);
        if(lng_raw > 100) convertRawToGPos(lng, lng_raw);

        return 1;
    }
    else return 0;
}

// void printGPosition(GPosition& pos) {
//     Serial.print(pos.degree); Serial.print("d ");
//     Serial.print(pos.minute); Serial.print("' ");
//     Serial.print(pos.second,5); Serial.print("''");
// }

double getGPositionValue(GPosition& pos) {
    return pos.degree + ((double) pos.minute / 60) + ((double) pos.second / 3600);
}
