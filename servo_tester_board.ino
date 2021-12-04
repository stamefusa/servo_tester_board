#include <Servo.h>

#define VOLUME A7
#define BUTTON 4
#define LED_1 12
#define LED_2 9
#define LED_3 6
#define LED_4 3
#define SERVO_1 11
#define SERVO_2 8
#define SERVO_3 5
#define SERVO_4 2

int deg = 0;
int target = 0;
int button = 0, button_pre = 0;

Servo s[4];

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);

  s[0].attach(SERVO_1);
  s[1].attach(SERVO_2);
  s[2].attach(SERVO_3);
  s[3].attach(SERVO_4);

  // サーボは90度を初期値とする
  for (int i = 0; i < 4; i++) {
    s[i].write(90);
  }

  Serial.begin(9600);
}

void loop() {
  int volume = analogRead(VOLUME);
  deg = getDeg(volume);
  //Serial.println(volume);

  //String disp_str = "raw: " + String(volume) + " | deg: " + String(deg);
  //Serial.println(disp_str);

  button = digitalRead(BUTTON);
  if (button == LOW && button_pre == HIGH) {
    Serial.println("ON");
    target = (target + 1) % 5;
  }
  Serial.println(target);
  handleLed(target);
  handleServo(target, deg);
  
  button_pre = button;
  delay(140);
}

/**
   可変抵抗の値から角度を取得
*/
int getDeg(int volume) {
  int volDeg = 0;

  // 可変抵抗が非線形なので角度を区切って変換する
  if (volume < 153) {
    volDeg = 0;
  } else if (volume >= 153 && volume < 514) {
    volDeg = map(volume, 153, 514, 0, 45);
  } else if (volume >= 514 && volume < 891) {
    volDeg = map(volume, 514, 891, 45, 90);
  } else if (volume >= 891 && volume < 951) {
    volDeg = map(volume, 891, 951, 90, 135);
  } else if (volume >= 951 && volume < 989) {
    volDeg = map(volume, 951, 989, 135, 180);
  } else if (volume >= 989) {
    volDeg = 180;
  }

  return volDeg;
}

/**
   ボタンを押すごとにLEDを順番に点灯
*/
void handleLed(int state) {
  if (state == 0) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  } else if (state == 1) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  } else if (state == 2) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  } else if (state == 3) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, LOW);
  } else if (state == 4) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, HIGH);
  }
}

/**
   サーボの制御
*/
void handleServo(int raw_target, int raw_deg) {
  int target = 0;

  if (raw_target == 0) {
    return;
  } else {
    target = raw_target - 1;
  }

  // サーボへの過負荷保護のため、可動範囲は10-170度とする
  int deg = 0;
  if (raw_deg < 10) {
    deg = 0;
  } else if (raw_deg > 170) {
    deg = 170;
  } else {
    deg = raw_deg;
  }

  s[target].write(deg);
}
