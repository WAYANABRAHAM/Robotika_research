# ETS — Hasil Penataan dan Verifikasi

Folder ini menyimpan laporan final yang sama dengan salinan pada path pengguna `...\robotika\robotika\ETS`.

## Build firmware

Board: ESP32 Dev Module (`esp32:esp32:esp32`), Arduino-ESP32 core 2.0.11. Empat sketch dikompilasi terpisah.

| Modul | Status | Flash | RAM global |
|---|---|---:|---:|
| 01_on_off_motor | Lulus | 268.045 byte (20%) | 21.448 byte (6%) |
| 02_kecepatan_pwm_serial | Lulus | 268.609 byte (20%) | 21.448 byte (6%) |
| 04_blynk_dashboard | Lulus | 736.789 byte (56%) | 43.976 byte (13%) |
| 05_ota_firmware | Lulus | 775.493 byte (59%) | 48.104 byte (14%) |

Skrip `tes_imclab_naik_turun.py` lulus syntax lint saat ditulis. Notebook tidak dijalankan pada perangkat. Tidak ada uji fisik motor, koneksi cloud Blynk, atau upload OTA dalam proses ini.

## Dokumen

- `Laporan_Penataan_ETS.docx` — ringkasan modul, perubahan kode, hasil build, dan batas pengujian.
- `Laporan_Penataan_ETS.pdf` — versi PDF untuk dibaca/cetak.

Identitas mahasiswa dibiarkan kosong. Jangan tambahkan token Blynk, Wi-Fi, atau kata sandi OTA ke laporan atau repositori.
