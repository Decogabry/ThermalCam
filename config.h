#pragma once

// ------------------------------------------------------------------ Wi-Fi
// the SSID gets the last three bytes of the MAC appended, so two cameras
// in the same room never clash: "ThermalCam-4F2A91"
#define AP_NAME       "ThermalCam"

// open network by default: on a device with no keyboard a password is
// friction without real protection, and there is nothing private to steal.
// To lock it down, uncomment and pick at least 8 characters.
// #define AP_PASS    "thermal123"

#define MDNS_NAME     "thermal"       // also reachable at http://thermal.local
#define WS_PORT       81              // WebSocket port (the page is on 80)

// -------------------------------------------------------------------- I2C
// ESP32-C3 Supermini. Classic ESP32: 21 and 22
#define PIN_SDA       8
#define PIN_SCL       9

// the MLX90640 needs 1 MHz to keep up at 16 Hz, the VL53L0X tops out
// around 400 kHz: the firmware switches between the two
#define I2C_FAST      1000000
#define I2C_SLOW      400000

// ---------------------------------------------------------------- sensors
#define MLX_ADDR      0x33
#define MLX_RATE      MLX90640_16_HZ  // 8/16/32 Hz: higher is noisier
#define COLS          32
#define ROWS          24
#define NPIX          (COLS * ROWS)   // 768

#define TOF_ADDR      0x29
#define TOF_MAX_MM    2200            // beyond this the VL53L0X is unreliable

// ----------------------------------------------------------------- packet
// header: magic, cols, rows, ambient temp, frame counter, distance
// then NPIX int16 in hundredths of a degree, little-endian
#define HDR           10