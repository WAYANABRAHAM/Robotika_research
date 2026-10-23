# 04 — Dashboard Blynk

Sketch `04_blynk_dashboard.ino` memetakan Blynk Virtual Pin V0 ke kendali motor ON/OFF. Buat datastream V0 bertipe Integer dengan rentang 0–1 dan hubungkan widget Switch ke V0.

## Kredensial lokal
Salin `secrets.example.h` menjadi `secrets.h` di folder ini; isi token Blynk dan Wi-Fi secara lokal. `secrets.h` diabaikan Git. Jangan commit token atau password.

Motor disetel OFF pada boot dan saat Blynk tersambung/tersambung ulang. Kecepatan ON saat ini tetap pada duty 205/255; belum ada slider atau RPM telemetry.
