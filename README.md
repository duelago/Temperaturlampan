# Whamageddon lamp


A 3D-printed lamp that changes color depending on the outside temperature and, as a special feature, blinks when Last Christmas by Wham is played on the radio station. You can also monitor other songs by setting the song in the web interface. The lamp also plays a few notes from Last Christmas using an annoying buzzer when the song is detected. The API supports many european radio stations, so just check the code and you can easily figure out how to change it to your local radio station. The default setting is Mix Megapol in Sweden

You can compile the file whamageddonlamp.ino in Arduino IDE or download the .bin file (with Mix Megapol) and flash it directly with, for example, NodeMCUflasher.

The lamp consists of an addressable WS2812 LED that changes color based on the outside temperature. The data source can be any major airport in the world. Via a web interface, you input the METAR code of the airport. (The most common Swedish airports are listed on the HTML page.)

The temperature is also displayed on the OLED screen, and all weather data is presented in the web interface. The lamp checks the radio stations playlist every 30 seconds to notify you when Last Christmas is played. When detected, the lamp starts blinking. A buzzer also plays a few familiar notes, but the sound is only heard once per song play. The blinking continues until the song ends.

Here’s a video showing how it works (without the buzzer): https://youtu.be/staYxQRvpsY

I printed the case in wood filament and stained it. The star is printed in white PLA in vase mode. Other lamp designs are availabe (tulip, ball and "bambulab") The case is available in two versions, and STEP/STL files can be downloaded. Choose whether you want the version with or without mounts for M 2.5 screws. The version with screws is better, but if you don’t have any M 2.5 screws, the version without screws works if you secure the Wemos with hot glue.

The pole that the star sits on is a regular metal straw available at Amazon or a big supermarket. The components needed are:

- 3D-printed case and lid
- Wemos D1 mini v3 (the version with screw holes) https://www.aliexpress.com/item/4000420770002.html
- Wemos D1 mini OLED shield https://www.aliexpress.com/item/32804426981.html
- WS2812 LED https://www.aliexpress.com/item/1005001863273661.html
- 100-ohm resistor
- Passive buzzer https://www.aliexpress.com/item/1005006213298069.html
- Metal straw https://www.amazon.se/Alfresco-metallsugr%C3%B6r-%C3%A5teranv%C3%A4ndbara-reng%C3%B6ringsborstar-cocktail/dp/B0BKT4334T/ref=sr_1_17
- Dupont wires
- Hot glue to secure the LED in the star
- Super glue
- Micro USB cable
- Heat shrink tubing (for the soldering at the LED)
- 2 M 2.5 screws (It also works without them if you print the version without screw mounts)

<h1>Wiring diagram</h1>


![templampan](https://github.com/duelago/Temperaturlampan/assets/12539239/ebb60627-cd37-4e9d-8fff-efc2866f2104)






<h1>3D-printed lamp</h1>

![whamageddonlampan](https://github.com/duelago/Temperaturlampan/assets/12539239/6cb68040-5377-46a6-b174-873620f04d1d)


<h1>Christmas version</h1>

![temperaturlampan-stjarna](https://github.com/duelago/Temperaturlampan/assets/12539239/859001ad-1024-4838-bd3d-e470d4a57013)


This version of the Wemos D1 fits. V3 with holes for screws.

![Screenshot 2023-10-04 at 19 12 29](https://github.com/duelago/Temperaturlampan/assets/12539239/45842c62-73e3-4499-badd-159bfa92e052)


<h1>Instructions</h1>

I print my boxes and lids in PLA using a 0.6 nozzle and 0.3 mm layer height. I print the star itself in vase mode with a 0.4 nozzle and 0.2 mm layer height in white PLA. The box can definitely be printed with a 0.4 nozzle as well, but it will naturally take longer.

Other than that, just follow the wiring diagram and solder the wires in place, paying attention to which pins on the LED should be connected where. I use a bit of hot glue to secure the LED inside the star. Don't forget to use heat shrink tubing to separate the three contact points of the LED. In the Christmas version, I glue the two parts of the star together with super glue, and for the "tulip," I glue the small adapter with super glue as well.

The user connects the device with a standard micro USB cable and phone charger. The first time you connect the lamp at home, a new access point named Whamageddonlamp will appear. Connect to it via the captive portal that pops up, or through the IP address: http://192.168.4.1.

Add the lamp in WifiManager to your local wifi network, with your private wifi password. The lamp will then receive an IP address from the DHCP server at home. The user can reconnect their computer or phone to their home WiFi and access the lamp through this new IP address or via the address: http://whamageddonlamp.local. If that doesn't work, use the Fing app or a similar one on your phone to find out which IP address the lamp has been assigned.

In the web interface, you can set the airport and the song you want to monitor. These are stored in EEPROM, so they are preserved even during power outages, etc.


![Screenshot 2024-05-22 at 08 39 23](https://github.com/duelago/Temperaturlampan/assets/12539239/358260d7-782f-43a1-8a2a-4314bc805bc0)



![ezgif com-crop](https://github.com/duelago/Temperaturlampan/assets/12539239/41c63c9f-87a0-4780-b7ca-e82fb4401fc8)


