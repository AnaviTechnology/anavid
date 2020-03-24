# anavid
Standalone daemon process for controlling ANAVI Light pHAT over MQTT

# Installation

* Install Hassbian on Raspberry Pi

* Run the command below and enable I2C from *Interfacing Options*:

```bash
sudo raspi-config
```

* Install mosquitto, doxygen, wiringpi, piGPIO and OpenSSL:

```bash
sudo apt-get update
sudo apt-get install -y mosquitto mosquitto-clients wiringpi pigpio libssl-dev doxygen
```

* Enable anonymous connection to the MQTT broker by editting the following setting in `/etc/mosquitto/mosquitto.conf`:

```
allow_anonymous true
```

* Start Mosquitto and enable its automatic launch at boot:

```bash
sudo systemctl enable mosquitto.service
sudo systemctl start mosquitto.service
```

* Install Paho (library for MQTT clients)

```bash
cd ~
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make html
make
sudo make install
```

* Build and install anavid

```bash
cd ~
git clone https://github.com/AnaviTechnology/anavid.git
cd anavid
make
sudo make install
```

* Create configuration file and adjust the settings:

```bash
sudo cp config/anavilightphat.ini /etc/anavilight.ini
```

* Start the system service

```bash
sudo systemctl start anavi
```

* Retrieve device ID

```bash
anavid -m
```

* Replace DEVICEID and add the following lines to configuration.yaml

```yaml
# MQTT Broker (aka Mosquitto)
mqtt:
  discovery: true
  broker: hassbian.local
  port: 1883

# ANAVI Light pHAT
light:
  - platform: mqtt_json
    name: "ANAVI Light pHAT"
    command_topic: "DEVICEID/action/rgbled"
    brightness: true
    rgb: true
```

* Run the following command to restart Home Assistant and reload configurations:

```bash
sudo systemctl restart home-assistant@homeassistant.service
```

# Integration with Amazon Echo and Alexa

* Add the following settings to your configuration.yaml:

```yaml
emulated_hue:
  type: alexa
  expose_by_default: true
```

* Run the following command to restart Home Assistant and reload configurations:

```bash
sudo systemctl restart home-assistant@homeassistant.service
```

* Ask alexa to discover new devices


* Say "Alexa, turn on ANAVI Light pHAT" to turn on the lights


* Say "Alexa, turn off ANAVI Light pHAT" to turn off the lights

# MQTT Commands

**NOTE:** Replace **<replace-with-your-device-id>** with the actual machine ID!

* Change the colors of the 12V RGB LED strip to **red**:

```bash
mosquitto_pub -h 192.168.4.250 -d -p 1883 -t "<replace-with-your-device-id>/action/rgbled" -m "{ \"state\": \"ON\", \"color\": { \"r\":255, \"g\":0, \"b\":0 } }"
```

* Change the colors of the 12V RGB LED strip to **yellow**:

```bash
mosquitto_pub -h 192.168.4.250 -d -p 1883 -t "<replace-with-your-device-id>/action/rgbled" -m "{ \"state\": \"ON\", \"color\": { \"r\":200, \"g\":200, \"b\":0 } }"
```

* Set the 12V RGB LED strip to full brightness:

```bash
mosquitto_pub -h 192.168.4.250 -d -p 1883 -t "<replace-with-your-device-id>/action/rgbled" -m "{ \"state\": \"ON\", \"color\": { \"r\":255, \"g\":255, \"b\":255 } }"
```

* Turn off the 12V RGB LED strip:

```bash
mosquitto_pub -h 192.168.4.250 -d -p 1883 -t "<replace-with-your-device-id>/action/rgbled" -m "{ \"state\": \"OFF\" }"
```
