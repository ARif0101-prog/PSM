#include  "WiFi.h" //perintah deklarasi memanggil library wifi

WiFiClient client; //mengubah perintah WiFiClient menjadi client

#include <DHTesp.h> //perintah deklarasi memanggil library DHTesp 

const int DHT_PIN = 15; //deklarasi pin dht

DHTesp dhtSensor; //mengubah perintah DHTesp menjadidhtSensor

String thingSpeakAddress = "api.thingspeak.com"; //alamat dari server thingspeak
String request_string; //variabel yang digunakan untuk memasukkan script upload data ke thingspeak

void setup()
{
  Serial.begin(115200);
  WiFi.disconnect(); //perintah disconnect wifi
  WiFi.begin("Wokwi-GUEST", "",6); //perintah untuk koneksi wifi esp32 dengan alamat ssid dan pasword wifi yang sudah ditentukan
  while (WiFi.status() != WL_CONNECTED) { //perintah tunggu sampai esp32 terkoneksi ke jaringan wifi
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22); //inisialisasi sensor dht
}


void loop()
{
  delay(2000);
  TempAndHumidity data = dhtSensor.getTempAndHumidity(); //perintah utnuk mengambil data suhu dan kelembaban 

  float t = data.temperature; //perintah memasukkan data suhu ke variabel t
  float h = data.humidity; //perintah memasukkan data kelembaban ke variabel h

  kirim_thingspeak(t, h); //perintah untuk menjalankan void kirim_thingspeak kemudian t, dan h untu memasukkan nilai variabel t dan h ke dalam variabel baru yang ada di void kirim_thingspeak 

  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void kirim_thingspeak(float suhu, float hum) {
  if (client.connect("api.thingspeak.com", 80)) { //perintah untuk koneksi dengan server thingspeak pada port 80
    request_string = "/update?"; //membuat script API untuk update data
    request_string += "key="; //script API untuk deklarasi variabel API key
    request_string += "4SFDANMFGLCU3P4B"; //API key dari channel yang dibuat
    request_string += "&"; //menambahakan simbol & ke script API untuk menambahkan perintah berikutnya ke dalam script
    request_string += "field1"; //field yang diguanakn untuk menampilkan datanya, apakah itu field1, field2 atau field ke-n
    request_string += "="; //menambahksn simbol = ke script API jadi jika digabungkan menjadi "field1="
    request_string += suhu; //variabel data yang akan dikirimkan ke field
    request_string += "&"; //menambahakan simbol & ke script API untuk menambahkan perintah berikutnya ke dalam script
    request_string += "field2"; //field yang diguanakn untuk menampilkan datanya, apakah itu field1, field2 atau field ke-n
    request_string += "="; //menambahksn simbol = ke script API jadi jika digabungkan menjadi "field1="
    request_string += hum; //variabel data yang akan dikirimkan ke field

    Serial.println(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n"); //menampilkan keterangan ke serial monitor
                 
    client.print(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n"); //memberikan keterangan ke library WiFiClient
    unsigned long timeout = millis();
    while (client.available() == 0) { //perintah memunculkan notifikasi apabila esp32 terputus dengan thingspeak
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) { //perintah untuk menampilkan line ketika esp32 terkoneksi dengan thingspeak
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

  }
}
