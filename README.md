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
- Superlim (om du gör varianten med stjärnan)
- Microusb-kabel
- Krympslang, framförallt för lödningarna uppe vid LEDen

<h1>Kopplingsschema</h1>

![skiss](https://github.com/duelago/Temperaturlampan/assets/12539239/7f917df6-da84-4c0b-8d06-351bf7535b82)




<h1>3D-printad lampa</h1>

![temperaturlampan-tulip](https://github.com/duelago/Temperaturlampan/assets/12539239/1e27401d-4797-4640-9fd7-3ef83dfc9c0f)


<h1>Julversion</h1>

![temperaturlampan-stjarna](https://github.com/duelago/Temperaturlampan/assets/12539239/859001ad-1024-4838-bd3d-e470d4a57013)


<h1>Instruktioner</h1>

Användaren ansluter enheten med en vanlig microusb-kabel och mobilladdare. Första gången man ansluter lampan i hemmet dyker en ny accesspunkt upp som heter <b>Temperaturlampan</b> Anslut till denna via en captive portal som poppar upp, eller via IP-adress: http://192.168.4.1

Lampan får nu en IP-adress av DHCP-servern i hemmet. Användaren ansluter sin dator eller telefon till sitt hemmawifi igen och når nu lampan via denna nya IP-adress eller på adressen: http://temperaturlampan.local

I webbinterfacet ställer man in flygplats. Denna sparas i EEPROM och klarar alltså strömavbrott etc. 
