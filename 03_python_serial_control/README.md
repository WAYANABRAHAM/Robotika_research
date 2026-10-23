# 03 — Kontrol Serial dengan Python

Program Python berjalan di komputer, bukan sebagai MicroPython di ESP32. Komputer mengirim byte perintah melalui USB-Serial ke sketch di `02_kecepatan_pwm_serial`.

## Setup
```bash
python -m pip install pyserial
```

Jalankan `python tes_imclab_naik_turun.py` untuk deteksi port dan kontrol interaktif. Jika deteksi port gagal, tentukan port manual, misalnya `python tes_imclab_naik_turun.py --port COM11`.

Perintah: `u/+`, `d/-`, `1`, `0`, `p`, dan `q`. Program mengirim `0` saat koneksi dibuka dan saat keluar bila port masih tersedia. Tutup Serial Monitor saat Python memakai port yang sama.

Notebook di folder ini menyediakan contoh deteksi port otomatis, serial dasar, dan kontrol PWM. Notebook tidak menjalankan motor otomatis saat dibuka.
