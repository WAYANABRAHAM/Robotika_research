# 01 — Kendali motor ON/OFF

Sketch ESP32 dasar untuk mengendalikan motor DC dan LED melalui Serial Monitor.

## Hardware mapping
- GPIO 27/26: arah motor melalui driver.
- GPIO 12: PWM enable, 30 kHz, resolusi 8-bit.
- GPIO 2: LED indikator.

## Cara uji
1. Pastikan motor memakai driver dan catu yang sesuai; jangan hubungkan motor langsung ke GPIO.
2. Upload sketch ini ke ESP32.
3. Buka Serial Monitor pada 115200 baud.
4. Kirim `1` untuk ON dan `0` untuk OFF.

Saat boot, sketch memanggil `motorOff()` sehingga output mulai dalam keadaan aman. Kecepatan tidak diukur; pin RPM tidak dipakai pada modul ini.
