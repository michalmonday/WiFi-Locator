# WiFi-Locator
Locating Wi-Fi Access Points with Signal Strength (RSSI) using Esp8266 and NEO-M8N modules. 
> (NEO-M8N GPS functionality is not implemented yet, central unit gathers the GPS data but is not doing anything with it)  

**Parts used:**  
- 4x Wemos D1 Mini boards (£10) - [Link](https://www.ebay.co.uk/itm/D1-Mini-NodeMCU-Lua-ESP8266-ESP-12-WeMos-D1-Mini-WIFI-4M-Bytes-Module/272253589592)  
- 4x AAA batteries holders (£2) - [Link](https://www.ebay.co.uk/itm/1-8-AAA-Battery-Holder-Box-with-Switch-PP3-Clip-Wire-Solder-Tags-JR-Lead/121729965773)  
- 4x Mini SMD switches (£1) - [Link](https://www.aliexpress.com/item/10PCS-YT2024Y-MSS22D18-MINI-Miniature-SMD-Slide-Switch-2P2T-6Pin-for-DIY-Electronic-Accessories-High-Quality/32826876349.html)  
- NEO-M8N GPS module (£7, optional) - [Link](https://www.aliexpress.com/item/Free-Shipping-GPSV3-M8N-ublox-NEO-M8N-001-eighth-Beidou-GPS-module-APM-MWC-flight-control/32822894266.html)  
> GPS module is not required for it to work, it is not implemented yet anyway  
- NodeMcu (£2) - [Link](https://www.aliexpress.com/item/1PCS-Wireless-module-CH340-NodeMcu-V3-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266/32802874451.html)  
- SH1106 I2C display (£5) - [Link](https://www.ebay.co.uk/itm/1-3-4-Pin-SH1106-I2C-IIC-128X64-OLED-LED-Display-Module-Board-For-Arduino-RB/183492452441)  
- Buttons (£3 or less) - [Link](https://www.ebay.co.uk/itm/140Pcs-momentary-tactile-push-button-micro-SMD-SMT-tact-switches-14-types/183062874271) 
- Wires (thin or solid core + micro usb cables/connectors) [Link 30 awg - thin wire £6](https://www.aliexpress.com/item/30-AWG-Wrapping-Wire-0-25mm-Tin-Plated-Copper-8-Colored-Wire-Wrap-Insulation-Test-Cable/32907507087.html)  [Link 22 awg - wider wire £1.5](https://www.ebay.co.uk/itm/10m-1-0-6mm-Equipment-Wire-22-23-AWG-Solid-Copper-Core-11-Colours-WP-010000/230775068955)  
> The micro usb cable, connectors and breakout can be used for conveniently disconnecting/connecting GPS module from one of the Wemos D1 Mini nodes.  
> Thin wires are more comfortable for soldering small elements than the "solid core" ones, I'd recommend 30 awg cables with high wire diameter (e.g. 0.25mm) relative to overal diameter (e.g. 0.55mm).
- Hot glue  
- Soldering equipment  (station/iron + holder)  


**Libraries used:**  
[ESP8266 core for Arduino](https://github.com/esp8266/Arduino)  
[ThingPulse ESP8266 OLED SSD1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)  
[LinkedList](https://github.com/ivanseidel/LinkedList)  
[TinyGPSplus for Neo-M8N](https://github.com/SensorsIot/TinyGPSplus-for-Neo-M8N)  

See separate folders for wiring images.  


![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/all_parts.jpg)

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/placement_measure.jpg)

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/test_behind.jpg)
