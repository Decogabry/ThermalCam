# DIY Cheap Thermal Camera

A real thermal camera for under $40, built from three parts.

No display, no battery, no app: the ESP32 creates its own Wi-Fi network and serves a web page, and your phone becomes the viewfinder. It works the same on iPhone, Android and a laptop, with no router and no cloud.

![Thermal camera](images/thermalcam.jpg)

Full build guide on Instructables: [DIY Cheap Thermal Camera](INSTRUCTABLES_LINK)

## Features

- Live thermal image, with bilinear interpolation and sharpening done in the browser
- Center temperature, plus min and max markers
- Tap anywhere on the image to add up to 5 measuring points
- Target distance and real-world size of one thermal pixel
- Photos and video saved straight to your phone
- Ironbow and Rainbow palettes
- Emissivity correction, rotation and mirroring
- Layout that adapts to phone and desktop

The sensor only outputs 768 raw temperature readings. Everything you see is computed on the phone.

## Hardware

| Part | Notes | Price |
|---|---|---|
| ESP32-C3 Supermini | any ESP32 works, only the I2C pins change | ~$3 |
| MLX90640ESF-BAB | 32×24 pixels, 55°×35° field of view, small 4-pin breakout | ~$32 |
| VL53L0X | time-of-flight distance sensor, optional | ~$3 |
| 4 jumper wires | both sensors share the same I2C lines | |

## Wiring

| ESP32-C3 | MLX90640 | VL53L0X |
|---|---|---|
| 3V3 | VIN | VIN |
| GND | GND | GND |
| GPIO 8 | SDA | SDA |
| GPIO 9 | SCL | SCL |

On a classic ESP32 use GPIO 21 (SDA) and GPIO 22 (SCL), and change the two pins in `config.h`.

I2C addresses: MLX90640 `0x33`, VL53L0X `0x29`.

## Software

**Board:** ESP32C3 Dev Module, with *USB CDC On Boot* set to *Enabled*.

**Libraries** (from the Arduino Library Manager):
- WebSockets by Markus Sattler
- Adafruit MLX90640
- Adafruit VL53L0X

**Files:**
- `ThermalCam.ino`: reads the sensors and sends the frames over Wi-Fi
- `config.h`: I2C pins, Wi-Fi name, refresh rate
- `page.h`: the web page served to the phone, with all the image processing

After *Code → Download ZIP*, rename the extracted folder from `ThermalCam-main` to `ThermalCam`: the Arduino IDE requires the folder to have the same name as the `.ino` file.

## Usage

1. Upload the sketch and open the serial monitor at 115200 to check that both sensors are found.
2. On your phone, join the Wi-Fi network **ThermalCam-xxxxxx** (open, no password).
3. Open **http://192.168.4.1** or **http://thermal.local**.

## Limitations

32×24 pixels is not a FLIR. It clearly shows a hand, a warm component on a board, a radiator or a hot breaker, but not fine detail. Shiny metal reads much colder than it really is: put a piece of electrical tape on it and measure the tape.

## License

MIT. See [LICENSE](LICENSE).

Made by Gabriele De Cosimo, 2026.
