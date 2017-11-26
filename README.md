# anavid
Standalone daemon process for controlling ANAVI Light pHAT over MQTT

# Installation

* Install wiringpi

```
sudo apt-get update
sudo apt-get install -y wiringpi
```

* Install Paho (library for MQTT clients)

```
cd ~
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c.git
make
sudo make install
```

* Install pigpio

```
git clone https://github.com/joan2937/pigpio.git
cd pigpio
make
sudo make install
```

* Build and install

```
cd anavid
make
sudo make install
```

# MQTT Commands

* Change the colors of the 12V RGB LED strip:

```
mosquitto_pub -h hassbian.local -d -p 1883 -t "replace-with-your-device-id/action/rgbled" -m "{ \"red\":255, \"green\":0, \"blue\":0 }"
```
