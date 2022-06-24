
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>



// init tft lib
MCUFRIEND_kbv tft;
// colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// tft wiring
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
// tft calibration
const int LEFT=115,RT=924,TOP=84,BOT=892;



// init touchscreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// defines what is a touch and what is not
#define MINPRESSURE 200
#define MAXPRESSURE 1000


int hour = 0;
int minute = 0;
float sec = 0.0;

bool stopped = true;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars


void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(0);            //PORTRAIT
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.fillRect(0, 160, 120, 160, GREEN);
  tft.fillRect(120, 160, 120, 160, RED);
}


void loop(void) {
  bool down = Touch_getXY(); // has the screen been touched
  //Serial.print(pixel_x); Serial.print(" "); Serial.println(pixel_y);
  
  tft.setCursor(57, 60);
  tft.print(String(hour)+":"+String(minute)+":"+String(sec));
  
  if ((160 < pixel_y) && (pixel_y < 320)) {

    // bottom left
    if ((0 < pixel_x) && (pixel_x < 120)) { 
      Serial.println("Left touched!");
      stopped = false;
    }
    // bottom right
    if ((120 < pixel_x) && (pixel_x < 240)) {
      Serial.println("Right touched");
      stopped = true;
    }
    
  }

  if (stopped == false) {
    sec = sec + 0.1;
    if (sec > 59) {
      sec = 0;
      minute = minute+1;
    }
    if (minute > 59) {
      minute = 0;
      hour = hour+1;
    }

    delay(100); // delay 0.1 sec 
  }

  // reset the x and y
  pixel_x = -500;
  pixel_y = -500;
}




bool Touch_getXY(void) {
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, LEFT, RT, 0, tft.width());
        pixel_y = map(p.y, TOP, BOT, 0, tft.height());
    }
    return pressed;
}
