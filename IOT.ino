  #include <DHT.h>
  #include <MQ135.h>
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>

  #define DHTPIN D2
  #define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);

  #define GASPIN A0
  MQ135 gasSensor = MQ135(GASPIN);
  #define BLYNK_TEMPLATE_ID "TMPL6z3EoeGLF"
  #define BLYNK_TEMPLATE_NAME "Cam Bien Do Am"
  #define BLYNK_AUTH_TOKEN "c-8J8ANHYqTjPzL_FIkV5LEXvl7pA3Xk"
  char auth[] = "c-8J8ANHYqTjPzL_FIkV5LEXvl7pA3Xk";
  char ssid[] = "SieuAiQuoc1";
  char pass[] ="Yeuhoanthaiha1";

  int humidityThreshold = 70;
  int ledStatus = LOW;

  void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    dht.begin();
    
    Blynk.virtualWrite(V4, humidityThreshold); // Gửi giá trị ngưỡng độ ẩm mặc định lên Slider

    // Thiết lập chân kết nối ảo V5 cho LED
    Blynk.virtualWrite(V5, ledStatus);
  }

  void loop() {
    Blynk.run();

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    float gasValue = gasSensor.getPPM();

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("% - Temperature: ");
    Serial.print(temperature);
    Serial.print("°C - Gas: ");
    Serial.print(gasValue);
    Serial.println(" ppm");

    // Gửi dữ liệu nhiệt độ, độ ẩm và nồng độ khí lên Blynk
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V3, gasValue);

    if (humidity > humidityThreshold) {
      ledStatus = HIGH;
    } else {
      ledStatus = LOW;
    }

    // Điều khiển LED thông qua chân kết nối ảo V5
    Blynk.virtualWrite(V5, ledStatus);

    delay(1000);
  }

  // Hàm để xử lý khi slider trên Blynk thay đổi giá trị
  BLYNK_WRITE(V4) {
    int sliderValue = param.asInt();
    humidityThreshold = sliderValue;
  }
