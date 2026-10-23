# 02 — Kecepatan PWM melalui Serial

Sketch `02_kecepatan_pwm_serial.ino` menyesuaikan duty PWM melalui perintah serial; perubahan duty bersifat open-loop, bukan pengukuran RPM.

## Hardware mapping
GPIO 27/26 = arah driver, GPIO 12 = PWM enable, GPIO 2 = indikator. Frekuensi PWM 30 kHz, resolusi 8-bit (0–255).

## Perintah Serial Monitor (115200 baud)
- `u` atau `+`: duty naik 25.
- `d` atau `-`: duty turun 25.
- `1`: set duty awal 205.
- `0`: stop (duty 0).
- `p`: tampilkan duty saat ini.

Mulai dengan roda/motor dalam keadaan aman. Nilai duty bukan hasil ukur kecepatan aktual.
