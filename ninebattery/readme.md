
# Ninebot ES2/4 battery integration

Use your spare ninebot battery as powerbank!

- Monitoring of battery stats
- Home Assistant integration

Connection is pretty simple
- External battery pinout is printed on the battery
- RX needs pullup resistor (INPUT_PULLUP does not work yet), TX is ok. Pullup works with both 3.3 and 5 volts
- Connect GND of the battery and ESP's
- Optionally, ESP can be powered from battery itself via dc-dc buck converter

Preview: 
  
  ![image](https://github.com/alexbilevskiy/esphome-custom/assets/14160356/af5e3bc1-6cd4-4391-9002-331e714e146a)


Credits
- [etransport/ninebot-docs](https://github.com/etransport/ninebot-docs/wiki/protocol) for protocol documentation
- [LeoDJ/ESPHome_Nodes](https://github.com/LeoDJ/ESPHome_Nodes/tree/main/CM1106_CO2_Sensor) for custom uart sensor example
- https://esphome.io/

Code looks like shit but if it works, it works :)

This is third reincarnation of (1) native esp8266 project https://github.com/alexbilevskiy/ninebattery-mqtt, (2) go + esphome serial server https://github.com/alexbilevskiy/ninebot-mqtt
