"""Interactive host-side serial controller for the iMCLab PWM sketch.

Commands sent to the ESP32: u/+ (increase), d/- (decrease), 1 (set 205),
0 (stop), p (print duty), q (quit). This runs on the PC, not on the ESP32.
"""
from __future__ import annotations

import argparse
import time

import serial
from serial.tools import list_ports

BAUDRATE_DEFAULT = 115200
ESP32_VID_PID = {
    (0x10C4, 0xEA60),  # Silicon Labs CP210x
    (0x10C4, 0xEA70),  # Silicon Labs CP2105
    (0x1A86, 0x7523),  # WCH CH340
    (0x1A86, 0x55D4),  # WCH CH9102
    (0x0403, 0x6001),  # FTDI FT232
    (0x0403, 0x6015),  # FTDI FT231X
    (0x303A, 0x1001),  # Espressif USB JTAG/Serial
}
ESP32_KEYWORDS = (
    "cp210", "ch340", "ch341", "ch910", "ftdi", "ft232",
    "usb-serial", "usb serial", "usb uart", "espressif", "esp32",
)
VALID_COMMANDS = {"u", "+", "d", "-", "1", "0", "p"}


def detect_esp32_port() -> str | None:
    """Return a likely ESP32 USB-UART port; return None if ambiguous/not found."""
    scored = []
    for port in list_ports.comports():
        text = " ".join((port.description or "", port.manufacturer or "", port.hwid or "")).lower()
        if "bluetooth" in text:
            continue
        score = 100 if (port.vid, port.pid) in ESP32_VID_PID else 0
        if any(keyword in text for keyword in ESP32_KEYWORDS):
            score += 50
        if score:
            scored.append((score, port.device))
    scored.sort(reverse=True)
    if not scored:
        return None
    if len(scored) > 1 and scored[0][0] == scored[1][0]:
        return None
    return scored[0][1]


def send_command(connection: serial.Serial, command: str) -> None:
    """Validate and send exactly one ASCII command byte."""
    command = command.strip().lower()
    if command not in VALID_COMMANDS:
        raise ValueError("Command must be u/+, d/-, 1, 0, or p")
    connection.write(command.encode("ascii"))
    connection.flush()
    time.sleep(0.05)
    if connection.in_waiting:
        reply = connection.readline().decode("utf-8", errors="replace").strip()
        if reply:
            print("ESP32:", reply)


def run_interactive(port: str | None, baudrate: int) -> int:
    port = port or detect_esp32_port()
    if not port:
        print("ESP32 port not detected. Re-run with --port COMx.")
        return 2

    connection = None
    try:
        connection = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)  # ESP32 may reset when the serial port opens.
        send_command(connection, "0")  # Start in the safe OFF state.
        print(f"Connected to {port} at {baudrate} baud.")
        print("Commands: u/+ up, d/- down, 1=205, 0=OFF, p=status, q=quit")
        while True:
            command = input("> ").strip().lower()
            if command == "q":
                break
            try:
                send_command(connection, command)
            except ValueError as error:
                print(error)
    except serial.SerialException as error:
        print(f"Serial connection failed: {error}")
        return 1
    finally:
        if connection is not None:
            try:
                send_command(connection, "0")
            except (serial.SerialException, ValueError):
                pass
            connection.close()
            print("Serial port closed; OFF command sent when possible.")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", help="Serial port, e.g. COM11; auto-detect if omitted")
    parser.add_argument("--baud", type=int, default=BAUDRATE_DEFAULT)
    args = parser.parse_args()
    return run_interactive(args.port, args.baud)


if __name__ == "__main__":
    raise SystemExit(main())
