String serData;

#define XDIR 4
#define XSTEP 5
#define YDIR 6
#define YSTEP 7
#define SOL 8    // Solenoid
#define PHOTO 10      // Sensor
#define LMITS 9
#define RIGHT OUTPUT  // Motor
#define LEFT LOW

bool P_in;          // Sensor
bool SW;
bool Xhome = 0;
bool Yhome = 0;

void setup() {

  // Sensor
  pinMode(PHOTO, INPUT);
  pinMode(LMITS, INPUT);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  Serial.println("Arduino is ready!");
}

void loop() {
  while (Serial.available() > 0) {
    char rec = Serial.read();
    serData += rec;

    if (rec == 'r') {
      Serial.println(serData);
      mov_step(XSTEP, RIGHT, 11, 1500);
      serData = "";
      delay(1);
    }
    else if (rec == 'R') {
      Serial.println(serData);
      mov_step(XSTEP, RIGHT, 20, 1500);
      serData = "";
      delay(1);
    }
    else if (rec == 'l') {
      Serial.println(serData);
      mov_step(XSTEP, LEFT, 11, 1500);
      serData = "";
      delay(1);
    }
    else if (rec == 'L') {
      Serial.println(serData);
      mov_step(XSTEP, LEFT, 20, 1500);
      serData = "";
      delay(1);
    }
    else if (rec == 'd') {
      Serial.println(serData);
      mov_step(YSTEP, LEFT, 5, 3000);
      serData = "";
      delay(1);
    }
    else if (rec == 'D') {
      Serial.println(serData);
      mov_step(YSTEP, LEFT, 10, 3000);
      serData = "";
      delay(1);
    }
    else if (rec == 'p') {
      Serial.println(serData);
      push();
      serData = "";
      delay(1);
    }
    else
    {
      Serial.println("Unknown");
      serData = "";
    }
  }
  delay(10);
}

int mov_step(int num, int dir, int tic, int vel) {

  digitalWrite(num - 1, dir);

  for (int x = 0; x < tic; x++) {
    digitalWrite(num, HIGH);
    delayMicroseconds(vel);
    digitalWrite(num, LOW);
    delayMicroseconds(vel);
  }
  return 0;
}
void push() {
  digitalWrite(SOL, HIGH);
  delay(60);
  digitalWrite(SOL, LOW);
  delay(100);
}

void home_init() {   //Find Home position
  while (Xhome == 0) { // Home
    SW = digitalRead(LMITS);
    if (Xhome == 0) {
      if (SW == 1)
        mov_step(XSTEP, RIGHT, 1, 1500);
      else if (SW == 0) {      // SW ON
        mov_step(XSTEP, LEFT, 30, 1500);
        Xhome = 1;
      }
    }
  }
  while (Yhome == 0) {
    P_in = digitalRead(PHOTO);
    if (P_in == 1)
      mov_step(YSTEP, RIGHT, 1, 1500 * 2);
    else if (P_in == 0) {    // Paper Detected
      mov_step(YSTEP, LEFT, 30, 1500 * 2);
      Yhome = 1;
    }
  }
}
