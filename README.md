# Robotika_Research

Kumpulan program dan dokumentasi eksperimen robotika menggunakan ESP32 dan Kit iMCLab.

## Isi repositori

- `on_off_Motor.ino` — kendali motor DC dan LED melalui Serial Monitor (`1` = ON, `0` = OFF).
- `on_off_Motor_blynk_wifi.ino` — kendali ON/OFF melalui Blynk Virtual Pin V0.
- `on_off_Motor_OTA.ino` — kendali Blynk dengan pembaruan firmware lokal melalui ArduinoOTA.
- `Tes_IMCLab_pilih_port_otomatis.ipynb` — contoh Python/pySerial untuk mendeteksi port dan mengirim perintah serial.
- `Test_Port_Serial.ipynb` — contoh dasar pengiriman perintah serial dari Python.
- `Rangkuman_Riset_Modul_Robotika_iMCLab.pdf` — rangkuman riset dan analisis modul.

## Kredensial

Jangan simpan token Blynk atau password Wi-Fi di repositori. Sketch Blynk membaca konfigurasi dari `secrets.h` bila file lokal itu tersedia; bila tidak, sketch memakai `secrets.example.h` yang hanya berisi placeholder. Untuk penggunaan pribadi:

1. Salin `secrets.example.h` menjadi `secrets.h`.
2. Isi kredensial baru secara lokal.
3. Jangan pernah menjalankan `git add -f secrets.h` atau mengunggah file tersebut.

`secrets.h` sudah masuk `.gitignore`. Token yang pernah terekspos sebaiknya diregenerasi di Blynk Console sebelum digunakan lagi.

## Pengujian dan cara menjalankan

- Sketch ESP32 harus dikompilasi satu per satu sebagai sketch terpisah; setiap `.ino` adalah program mandiri dengan `setup()` dan `loop()` sendiri.
- Sketch dasar memakai Serial Monitor 115200 bps. Kirim karakter `1` untuk ON dan `0` untuk OFF.
- Sketch Blynk memerlukan datastream Virtual Pin V0 bertipe Integer dengan rentang 0–1 dan widget Switch yang terhubung ke V0.
- Sketch OTA perlu diunggah pertama kali melalui USB. ArduinoOTA pada sketch ini adalah jalur update lokal; Blynk tetap menjadi jalur kontrol motor.
- Notebook Python memerlukan `pyserial`. Tutup Serial Monitor sebelum notebook membuka port yang sama.

File notebook menyertakan output historis. Output tersebut bukan hasil uji ulang dalam proses penyiapan repositori ini.