#include <SoftwareSerial.h>
#define LAT_RANGE_MAX 37.630968
#define LAT_RANGE_MIN 37.630855
#define LAT_TARGET 37.630912

#define LNG_RANGE_MAX 127.079620
#define LNG_RANGE_MIN 127.079512
#define LNG_TARGET 127.079566

#define MAX_STRING_PARTS 6

#define PIN_GPS_RX 3
#define PIN_GPS_TX 4

#define INDOOR_TEST

SoftwareSerial gps(PIN_GPS_RX, PIN_GPS_TX);

typedef struct {
    int degree;
    int minute;
    double second;
} GPosition;

void setup() {
    Serial.begin(9600);
    Serial.println("Start Serial...");

    gps.begin(9600); // Hardware Tx1, RX0 Pin
}

void loop() {
#ifdef INDOOR_TEST
    if (true) {
        String string = "GPGGA,094710.00,3737.79895,N,12704.79369,E,1,05,2.51,38.1,M,18.6,M,,*69";
#else
    if (gps.available()){
        String string = gps.readStringUntil('$');
#endif
        GPosition lat, lng;
        int ret = getLatAndLng(string, lat, lng);
        if (ret) {
            printGPosition(lat); Serial.print(" | "); printGPosition(lng); Serial.println("");
            double latValue = getGPositionValue(lat);
            double lngValue = getGPositionValue(lng);
            Serial.print("Target:\t\t"); Serial.print(LAT_TARGET, 5); Serial.print("\t|\t"); Serial.println(LNG_TARGET, 5); 
            Serial.print("Current:\t"); Serial.print(latValue, 5); Serial.print("\t|\t"); Serial.println(lngValue, 5); 

            if(latValue >LAT_RANGE_MIN && latValue < LAT_RANGE_MAX && lngValue > LNG_RANGE_MIN && lngValue < LNG_RANGE_MAX) {
                Serial.println("*** STOP ***");
            }
            else {
                Serial.print("*** GO *** ");
                if(latValue - LAT_TARGET > 0) Serial.print("to S,");
                else if(latValue - LAT_TARGET < 0) Serial.print("to N,");
                else Serial.print("to ,");

                if(lngValue - LNG_TARGET > 0) Serial.println("to W,");
                else if(lngValue - LNG_TARGET < 0) Serial.println("to E,");
                else Serial.println("to ,");
            }
        }
    }
    // do not use delay
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
    char* string_to_print;
    strcpy(string_to_print, string.c_str());

    char* string_to_parse;
    strcpy(string_to_parse, string.c_str());

    char* parts[MAX_STRING_PARTS];
    splitString(string_to_parse, ",", parts, MAX_STRING_PARTS);

    if(!String(parts[0]).compareTo("GPGGA")) {
        /*
        * TODO: resolve toDouble error issue
        */
        Serial.println(string_to_print);
        
        double lat_raw = String(parts[2]).toDouble();
        double lng_raw = String(parts[4]).toDouble();

        if(lat_raw > 0) convertRawToGPos(lat, lat_raw);
        if(lng_raw > 100) convertRawToGPos(lng, lng_raw);

        return 1;
    }
    else return 0;
}

void printGPosition(GPosition& pos) {
    Serial.print(pos.degree); Serial.print("d ");
    Serial.print(pos.minute); Serial.print("' ");
    Serial.print(pos.second,5); Serial.print("''");
}

double getGPositionValue(GPosition& pos) {
    return pos.degree + ((double) pos.minute / 60) + ((double) pos.second / 3600);
}
