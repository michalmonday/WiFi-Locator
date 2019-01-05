Node is responsible for sending RSSI values to the central unit. At the top of the code there's "NODE_ID" which ranges from 0 to 3.  
0 - Front  
1 - Back  
2 - Left  
3 - Right  

The code should be uploaded to each node and the user needs to change the ID depending on which node is being currently programmed. (The ID is sent to central unit together with RSSI so it knows which node provided the info)  

After powering on the node its' blue LED should blink. When it finds the "locator_ap" (central unit) and connects to it using "12345678" password the LED should be steadily on. If the connection is dropped the LED should blink quickly.

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/locator_node_schematic.png)

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/switch.jpg)

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/node_back.jpg)

