Node is responsible for sending RSSI values to the central unit. At the top of the code there's "NODE_ID" which ranges from 0 to 3.  
0 - Front  
1 - Back  
2 - Left  
3 - Right  

The code should be uploaded to each node and the user needs to change the ID depending on which node is being currently programmed. (The ID is sent to central unit together with RSSI so it knows which node provided the info)  

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/locator_node_schematic.png)

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/switch.jpg)

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/node_back.jpg)

