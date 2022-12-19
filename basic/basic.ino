#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Konstanta untuk paket NTP
const uint8_t NTP_PACKET_SIZE = 48;

// Konstanta untuk koneksi WiFi
const char* SSID = "your_wifi_ssid";
const char* PASSWORD = "your_wifi_password";

// Konstanta untuk OLED 0.96
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Variabel global untuk menyimpan data yang diterima dari server NTP
byte packetBuffer[NTP_PACKET_SIZE];

// Variabel global untuk menyimpan waktu NTP terakhir yang diperbarui
time_t lastUpdateTime = 0;

// Interval waktu antara setiap pembaruan waktu NTP (dalam milidetik)
const long UPDATE_INTERVAL = 5 * 60 * 1000;

// Fungsi untuk mengirimkan permintaan waktu NTP ke server NTP yang telah ditentukan
void sendNTPpacket(IPAddress& address)
{
  // Menyiapkan paket NTP
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // Mengirimkan paket NTP
  WiFiUDP udp;
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

// Fungsi untuk mengambil waktu NTP
time_t getNTPTime() {
  // Konfigurasi server NTP yang akan digunakan
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 3600;
  const int daylightOffset_sec = 3600;

  WiFiUDP udp;
  udp.begin(2390);

  IPAddress timeServerIP;
  WiFi.hostByName(ntpServer, timeServerIP);

  sendNTPpacket(timeServerIP);
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("No packet yet");
  }
  else {
    Serial.print("packet received, length=");
    Serial.println(cb);

    udp.read(packetBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;

    return epoch;
  }
}

// Fungsi untuk mengkonversi waktu Unix ke dalam format tanggal dan waktu
void convertUnixTime(time_t t, char* dateStr, int dateStrSize, char* timeStr, int timeStrSize) {
  struct tm* ptm = gmtime(&t);
  strftime(dateStr, dateStrSize, "%d-%m-%Y", ptm);
  strftime(timeStr, timeStrSize, "%H:%M:%S", ptm);
}

void setup() {
  Serial.begin(115200);

  // Inisialisasi OLED 0.96
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Koneksi ke WiFi
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Menyinkronisasi waktu NTP pertama kali
  lastUpdateTime = getNTPTime();
}

void loop() {
  // Mengambil waktu saat ini (dalam milidetik)
  unsigned long currentTime = millis();

  // Jika sudah lewat waktu pembaruan waktu NTP, maka sinkronisasi waktu NTP
  if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
    lastUpdateTime = getNTPTime();
  }

  // Mengkonversi waktu NTP terakhir yang diperbarui ke dalam format tanggal dan waktu
  char dateStr[11];
  char timeStr[9];
  convertUnixTime(lastUpdateTime, dateStr, 11, timeStr, 9);

  // Menampilkan waktu NTP terakhir yang diperbarui pada OLED 0.96
  display.clearDisplay();
  display.println(dateStr);
  display.println(timeStr);
  display.display();

  // Delay selama 1 detik
  delay(1000);
}
