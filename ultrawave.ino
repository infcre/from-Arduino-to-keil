const uint8_t PIN_TRIG = PA0;
const uint8_t PIN_ECHO = PA1;

unsigned long dist_cm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);          // >10 µs 的触发脉冲
  digitalWrite(PIN_TRIG, LOW);

  // 测高电平持续时间（单位 µs）
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 26000); // 超时 26 ms≈4.5 m

  // 换算成 cm（声速 340 m/s → 29 µs/cm，往返除以 2）
  return duration / 58;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Serial.begin(115200);
  Serial.println("超声测距仪_测试");
}

void loop() {
  unsigned long cm = dist_cm();
  if (cm == 0) cm = 999;          // 超时标记为“超量程”
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");
  delay(200);                     // 刷新周期 200 ms
}
