The code for node with GPS module attached to it is pretty much done. It passes the latitude, longitude and number of currently available satellites to the central unit. The central unit is not using it in any way yet.

I wish it could display markers on a google map in "real time" as you collect the data but there's some technical obstacles:  
- Google Maps API requires internet connection  
- If smartphone (with mobile data) was going to be used then it can't just be connected to the central unit and use mobile data at the same time...

One solution I thought about is to use additional module like Sim800L which would just upload the GPS data straight to internet (e.g. server hosted by me, which would provide that GPS data when requested by a smartphone for example).

![](https://github.com/michalmonday/WiFi-Locator/blob/master/pics/locator_node_with_gps_schematic.png)
