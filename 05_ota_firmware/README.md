# 05 — OTA firmware

Sketch `05_ota_firmware.ino` memakai Blynk untuk kendali motor dan ArduinoOTA untuk transfer firmware melalui jaringan lokal. Ini bukan implementasi Blynk.Air.

## Kredensial lokal
Salin `secrets.example.h` menjadi `secrets.h`; isi token Blynk, Wi-Fi, dan kata sandi OTA secara lokal. Jangan commit `secrets.h`.

Upload pertama perlu USB. Setelah itu komputer dan ESP32 harus berada pada jaringan yang dapat saling menjangkau. OTA mulai setelah Wi-Fi memperoleh alamat IP, meskipun koneksi Blynk Cloud belum tersedia. Motor dimatikan pada awal update dan ketika koneksi hilang.
