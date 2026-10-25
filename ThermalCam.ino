/*
  DIY Cheap Thermal Camera
  ========================
  A real thermal camera for under $40, built from three parts.
  No display, no battery, no app: the ESP32 creates its own Wi-Fi
  network and serves a web page, and your phone becomes the screen.

  Hardware
    ESP32-C3 Supermini      ~$3
    MLX90640 (32x24, 55deg) ~$32   I2C address 0x33
    VL53L0X (distance)      ~$3    I2C address 0x29
    4 wires

  Wiring: both sensors share the same two lines.
    3V3 -> VIN on both        GND -> GND on both
    GPIO 8 -> SDA on both     GPIO 9 -> SCL on both
    (classic ESP32: GPIO 21 and 22)

  Use
    1. upload, open the serial monitor at 115200
    2. join the Wi-Fi network "ThermalCam-xxxxxx" (open, no password)
    3. open http://192.168.4.1   (or http://thermal.local)

  Features
    live image, bilinear interpolation and unsharp mask on the phone
    center spot, min/max markers, tap to add up to 5 measuring points
    target distance and real-world size of one thermal pixel
    photo and video saved to the phone, Ironbow and Rainbow palettes
    emissivity correction, rotation, mirroring, adaptive phone/desktop layout

  Board       ESP32C3 Dev Module, USB CDC On Boot: Enabled
  Libraries   WebSockets (Markus Sattler)
              Adafruit MLX90640
              Adafruit VL53L0X
  Tabs        ThermalCam.ino, config.h, page.h

  The sensor only outputs 768 raw temperature readings. Everything you
  see, palettes, smoothing, markers, is computed in the browser.

  License: MIT. Built by Gabriele De Cosimo, 2026.
*/ì

#include <Wire.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Adafruit_MLX90640.h>
#include "Adafruit_VL53L0X.h"

#include "config.h"
#include "page.h"

Adafruit_MLX90640 mlx;
Adafruit_VL53L0X  tof;
WebServer         http(80);
WebSocketsServer  ws(WS_PORT);

float    frame[NPIX];
uint8_t  pkt[HDR + NPIX * 2];
uint16_t seq    = 0;
float    ta     = 22.0;
bool     tofOk  = false;
uint16_t distMm = 0;

// --------------------------------------------------------------- sensors
void startThermal() {
  if (!mlx.begin(MLX_ADDR, &Wire)) {
    Serial.println("MLX90640 not found. Check SDA/SCL and 3V3.");
    while (1) delay(1000);
  }
  mlx.setMode(MLX90640_CHESS);
  mlx.setResolution(MLX90640_ADC_18BIT);
  mlx.setRefreshRate(MLX_RATE);
}

void startDistance() {
  // the VL53L0X cannot run at 1 MHz: bring the bus down for it
  Wire.setClock(I2C_SLOW);
  if (tof.begin(TOF_ADDR, false, &Wire)) {
    tof.startRangeContinuous(50);
    tofOk = true;
  }
  Wire.setClock(I2C_FAST);
}

void readDistance() {
  if (!tofOk) return;
  Wire.setClock(I2C_SLOW);
  if (tof.isRangeComplete()) {
    uint16_t d = tof.readRangeResult();
    // out of range or a non-reflecting surface: the sensor returns
    // out-of-scale values, which are discarded instead of shown
    distMm = (d > 0 && d < TOF_MAX_MM) ? d : 0;
  }
  Wire.setClock(I2C_FAST);
}

// --------------------------------------------------------------- network
void startNetwork() {
  // unique SSID from the MAC, so several cameras can coexist
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char ssid[32];
  snprintf(ssid, sizeof(ssid), "%s-%02X%02X%02X", AP_NAME, mac[3], mac[4], mac[5]);

  WiFi.mode(WIFI_AP);

  bool up;
#ifdef AP_PASS
  up = WiFi.softAP(ssid, AP_PASS, 1, 0, 4);
#else
  up = WiFi.softAP(ssid, NULL, 1, 0, 4);
#endif

  if (!up) {
    Serial.println("Access point failed to start. Power cycle the board.");
    return;
  }

  WiFi.setTxPower(WIFI_POWER_11dBm);   // lower current peaks, plenty of range
  WiFi.setSleep(false);                // steady latency for the frame stream

  if (MDNS.begin(MDNS_NAME)) MDNS.addService("http", "tcp", 80);

  Serial.println();
  Serial.println("-----------------------------------");
  Serial.print  ("  Wi-Fi network : "); Serial.println(ssid);
#ifdef AP_PASS
  Serial.print  ("  Password      : "); Serial.println(AP_PASS);
#else
  Serial.println("  Password      : none (open network)");
#endif
  Serial.print  ("  Open          : http://"); Serial.println(WiFi.softAPIP());
  Serial.print  ("  or            : http://"); Serial.print(MDNS_NAME); Serial.println(".local");
  Serial.println("-----------------------------------");
  Serial.println("  Thermal  : MLX90640 ok");
  Serial.print  ("  Distance : "); Serial.println(tofOk ? "VL53L0X ok" : "not detected");
  Serial.println();
}

// ---------------------------------------------------------------- packet
void buildPacket() {
  pkt[0] = 0x54;          // 'T'
  pkt[1] = 0x4D;          // 'M'
  pkt[2] = COLS;
  pkt[3] = ROWS;

  int16_t t = (int16_t)lroundf(ta * 100.0f);
  pkt[4] = t & 0xFF;
  pkt[5] = (t >> 8) & 0xFF;
  pkt[6] = seq & 0xFF;
  pkt[7] = (seq >> 8) & 0xFF;
  pkt[8] = distMm & 0xFF;
  pkt[9] = (distMm >> 8) & 0xFF;
  seq++;

  for (int i = 0; i < NPIX; i++) {
    float x = frame[i];
    if (x < -273) x = -273;
    if (x >  600) x =  600;
    int16_t c = (int16_t)lroundf(x * 100.0f);   // hundredths of a degree
    pkt[HDR + i * 2]     = c & 0xFF;
    pkt[HDR + i * 2 + 1] = (c >> 8) & 0xFF;
  }
}

// ----------------------------------------------------------------- setup
void setup() {
  Serial.begin(115200);
  delay(2000);

  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(I2C_FAST);

  startThermal();
  startDistance();
  startNetwork();

  http.on("/", []() { http.send_P(200, "text/html", PAGE); });
  // iOS checks connectivity here: answering keeps the captive portal
  // window from opening, which would break the WebSocket
  http.on("/hotspot-detect.html", []() { http.send(200, "text/html", "<HTML>Success</HTML>"); });
  http.on("/generate_204",        []() { http.send(204, "text/plain", ""); });
  http.begin();

  ws.begin();
}

// ------------------------------------------------------------------ loop
void loop() {
  http.handleClient();
  ws.loop();

  if (ws.connectedClients() == 0) { delay(5); return; }
  if (mlx.getFrame(frame) != 0) return;

  if ((seq & 15) == 0) ta = mlx.getTa(false);   // ambient: once per 16 frames
  if ((seq & 3)  == 0) readDistance();          // distance: 4x per second

  buildPacket();
  ws.broadcastBIN(pkt, sizeof(pkt));
}