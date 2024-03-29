# Mematikan dan Menyalakan OLED pada ESP8266
Program ini merupakan contoh sederhana tentang cara mematikan dan menyalakan OLED pada ESP8266 menggunakan sebuah tombol. Program ini juga menampilkan waktu NTP pada OLED yang telah diperbarui setiap 5 menit sekali.

## Persiapan
Untuk menjalankan program ini, Anda memerlukan:

## ESP8266 dengan OLED 0.96" terpasang
* Tombol
* Kabel jumper
* Perangkat untuk mengupload program ke ESP8266, seperti kabel USB-to-Serial atau modul ESP8266

## Koneksi Fisik
1. Hubungkan OLED ke ESP8266 sesuai dengan petunjuk yang terdapat pada OLED.
2. Hubungkan tombol ke ESP8266 dengan menghubungkan pin tombol ke salah satu pin digital ESP8266. Misalnya, jika Anda menghubungkan tombol ke pin D1, maka Anda perlu menambahkan const int buttonPin = D1; pada program.
3. Hubungkan ESP8266 ke perangkat yang akan Anda gunakan untuk mengupload program.

## Cara Menggunakan
1. Upload program ke ESP8266 menggunakan perangkat yang telah disiapkan.
2. Buka serial monitor pada perangkat yang Anda gunakan untuk mengupload program.
3. Tekan tombol untuk mematikan atau menyalakan OLED. Anda akan melihat waktu NTP yang diperbarui muncul pada OLED setiap 5 menit sekali jika OLED menyala.

## Flowchart
```mermaid
graph LR
  A[Start] --> B[Setup]
  B --> C[Connect to WiFi]
  C --> D[Initialize OLED]
  D --> E[Configure button pin]
  E --> F[Initialize lastUpdateTime]

  F --> G{Is OLED On?}
  G --> |Yes| H[Get NTP Time]
  H --> I{Is Time Updated?}
  I --> |Yes| J[Convert Unix Time]
  J --> K[Display Time on OLED]
  K --> L[Wait 100ms]
  I --> |No| L

  G --> |No| L[Turn off OLED]
  L --> M[Check Button Status]
  M --> N{Button Pressed?}
  N --> |Yes| O[Toggle OLED Status]
  N --> |No| L
  O --> L
  L --> O

  B --> P[Loop]
  P --> L

  subgraph NTP Functions
    H --> Q[Send NTP Packet]
    Q --> R[Delay 1000ms]
    R --> S{Packet Received?}
    S --> |Yes| T[Read Packet]
    T --> U[Calculate Time]
    U --> J
    S --> |No| R
  end
```

## Catatan
Pastikan Anda mengubah konstanta SSID dan PASSWORD sesuai dengan nama jaringan WiFi dan password yang Anda gunakan.
Jika Anda menggunakan pin digital yang berbeda dari D1 untuk menghubungkan tombol, pastikan Anda mengubah konstanta buttonPin sesuai dengan pin yang Anda gunakan.
