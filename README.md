# Temperaturlampan
En 3D-printad lampa som ändrar färg beroende på yttertemperatur

Temperaturlampan består av en adresserbar WS2811 LED som ändrar färg beroende på yttertemperaturen. Datakällan är valfri större flygplats i världen. Via ett webbinterface matar man in den METAR-kod som flygplatsen har. (De vanligaste svenska flygplatserna finns listade på HTML-sidan)

Temperaturen visas även med siffror på OLED-skärmen.


Jag har printat lådan i träfilament och betsat den. Stjärnan är printad i vit PLA i vase mode

Själva stången som stjärnana sitter på är ett vanligt metallsugrör som finns på typ ICA. Lådan och stjärnan är 3D-printad och STL-filer finns bifogade.

De komponenter som behövs är:

- 3D-printad låda och lock
- Wemos D1 mini v3
- Wemos D1 mini OLED shield
- WS2811 LED
- 100 ohm motstånd
- Metallsugrör
- Dupontsladdar
- Smältlim för att få LEDen på plats i stjärnan
- Microusb-kabel
- Krympslang, framförallt för lödningarna uppe vid LEDen

![skiss](https://github.com/duelago/Temperaturlampan/assets/12539239/adb47f02-b3e3-48fd-9842-8540c615630b)



![temperaturlampan-tulip](https://github.com/duelago/Temperaturlampan/assets/12539239/87550a2e-41df-498f-b439-62fd7137fa43)


Julversion:
![temperaturlampan-stjarna](https://github.com/duelago/Temperaturlampan/assets/12539239/3af25f29-aa95-4592-a9a4-02e15cef8af8)


Instruktioner:

Användaren ansluter enheten med en vanlig microusb-kabel och mobilladdare. Första gången man ansluter lampan i hemmet dyker en ny accesspunkt upp som heter <b>Temperaturlampan</b> Anslut till denna via en captive portal som poppar upp, eller via IP-adress: http://192.168.4.1

Lampan får nu en IP-adress av DHCP-servern i hemmet. Användaren ansluter sin dator eller telefon till sitt hemmawifi igen och når nu lampan via denna nya IP-adress eller på adressen: http://temperaturlampan.local

I webbinterfacet ställer man in flygplats. Denna sparas i EEPROM och klarar alltså strömavbrott etc. 
