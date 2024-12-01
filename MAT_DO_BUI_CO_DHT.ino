int thietbi = 2;  //Thứ tự của 2 thiết bị

#include <DHT.h>

#define DHTPIN 27    // Chân kết nối với cảm biến DHT22
#define DHTTYPE DHT22 // Loại cảm biến DHT

DHT dht(DHTPIN, DHTTYPE);
#include <GP2YDustSensor.h>

const uint8_t SHARP_LED_PIN = 26; // GPIO14, chân điều khiển LED của cảm biến
const uint8_t SHARP_VO_PIN = 39; // Chân ADC để đọc giá trị từ cảm biến
GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

const int mq7Pin = 36;   // Chân đọc tín hiệu từ cảm biến MQ-7

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6dbEVGXjq"
#define BLYNK_TEMPLATE_NAME "QUAN TRAC MOI TRUONG"
#define BLYNK_AUTH_TOKEN "Le1D9S670yA5DYLq-f9FuBK3621A8W8B"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Inception Escape Room";
char pass[] = "IloveInception";
float humidity,temperature,dustDensity,co_concentration;
void setup() {
  Serial.begin(115200);
  dht.begin();
  dustSensor.begin();
  dustSensor.setCalibrationFactor(0.55);//hiệu chỉnh độ chíng xác
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
}
void gui(){
  if(thietbi == 1){
  Blynk.virtualWrite(V0,temperature);
  Blynk.virtualWrite(V1,humidity);
  Blynk.virtualWrite(V2,dustDensity);
  Blynk.virtualWrite(V3,co_concentration);
  }
  if(thietbi == 2){
  Blynk.virtualWrite(V10,temperature);
  Blynk.virtualWrite(V11,humidity);
  Blynk.virtualWrite(V12,dustDensity);
  Blynk.virtualWrite(V13,co_concentration);
  }
}
void loop() {
  Blynk.run();
  // Đọc giá trị từ cảm biến DHT22
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Kiểm tra lỗi đọc cảm biến DHT22
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
  dustDensity = dustSensor.getDustDensity();
  
  Serial.print("Dust Density: ");
  Serial.print(dustDensity);
  Serial.println(" µg/m³");

  // Đọc giá trị từ cảm biến MQ-7
  int mq7Value = analogRead(mq7Pin);  // Đọc giá trị từ chân ADC

  // Chuyển đổi giá trị đọc được thành điện áp
  float mq7Voltage = mq7Value * (3.3 / 4095.0);

  // Áp dụng công thức tính toán nồng độ khí CO (điện áp)
  // Lưu ý: Bạn cần hiệu chỉnh công thức này dựa trên đặc điểm cụ thể của cảm biến MQ-7.
   co_concentration = mq7Voltage * 100; // Thay đổi công thức này nếu cần thiết.

  // In giá trị điện áp và nồng độ khí CO ra Serial Monitor
  Serial.print("CO Voltage: ");
  Serial.print(mq7Voltage);
  Serial.print(" V, CO Concentration: ");
  Serial.print(co_concentration);
  Serial.println(" ppm");
  gui();
  
  delay(500); // Chờ 2 phút trước khi đo lại
}
