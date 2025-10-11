const uint8_t PIN_TRIG = PA0;
const uint8_t PIN_ECHO = PA1;

unsigned long get_cm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);  //microsecond是微秒，millisecond是毫秒
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 23200);  //这是测量从现在起到PIN_ECHO上出现高电平的时间,23200us超时（这个时候距离是4米）,返回的结果也是微秒

  return duration / 58.82;    // 340m/s，一来一回，2x=vt,x=vt/2,x(cm)=(340m/s)*(t(10^-6s))/2*100
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Serial.begin(115200);
  Serial.println("超声测距仪_测试");
  Serial.println("--------------");
  Serial.println("");
}

void loop() {
  unsigned long cm = get_cm();
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");
  delay(500);
}
