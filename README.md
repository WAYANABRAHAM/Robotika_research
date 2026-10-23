# Robotika_Research — Kit iMCLab

Program dan dokumentasi praktikum robotika ESP32 disusun per modul seperti struktur berikut:

```text
01_on_off_motor/
02_kecepatan_pwm_serial/
03_python_serial_control/
04_blynk_dashboard/
05_ota_firmware/
ETS/
```

## Modul

1. **01_on_off_motor** — perintah Serial `1` menyalakan motor, `0` mematikan.
2. **02_kecepatan_pwm_serial** — duty PWM dapat dinaikkan/diturunkan melalui Serial (`u/+`, `d/-`). Ini kontrol open-loop, bukan pembacaan RPM.
3. **03_python_serial_control** — program Python host-side memakai pySerial; bukan MicroPython pada ESP32.
4. **04_blynk_dashboard** — switch Blynk mengirim nilai Virtual Pin V0 (Integer 0/1).
5. **05_ota_firmware** — Blynk untuk kendali motor dan ArduinoOTA untuk pembaruan firmware lokal. Jalur OTA ini bukan Blynk.Air.

Setiap modul punya README sendiri. Satu sketch `.ino` adalah satu program; buka/compile tiap modul secara terpisah.

## Kredensial

Berkas `secrets.example.h` hanya berisi placeholder. Untuk penggunaan lokal, salin menjadi `secrets.h` dalam folder modul yang sesuai dan isi nilai baru. `secrets.h` diabaikan Git; jangan pernah commit token Blynk, password Wi-Fi, atau password OTA. Token yang pernah dibagikan sebelumnya sebaiknya diregenerasi sebelum dipakai kembali.

## Keselamatan dan batas uji

Motor harus memakai driver/catu daya yang sesuai—jangan hubungkan motor DC langsung ke GPIO ESP32. Firmware memulai dengan motor OFF dan OTA mematikan motor saat update mulai. Hasil compile tidak membuktikan motor bergerak, perangkat online di Blynk, atau transfer OTA berhasil; status pengujian dicatat di `ETS/README.md`.
