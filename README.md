# Temperaturlampan
En 3D-printad lampa som ändrar färg beroende på yttertemperatur

Lampan fungerar fristående, men vill du ha en lampa som du integrerar i ett befintligt hemautomatiseringsprojekt så kan du flasha WLED på hårdvaran istället för att köra min kod. 
Kolla här: https://kno.wled.ge/

För min kod flashar du filen metar.ino  i Arduino IDE eller så laddar du ner .bin-filen och flashar direkt med exempelvis NodeMCUflasher

Temperaturlampan består av en adresserbar WS2812 LED som ändrar färg beroende på yttertemperaturen. Datakällan är valfri större flygplats i världen. Via ett webbinterface matar man in den METAR-kod som flygplatsen har. (De vanligaste svenska flygplatserna finns listade på HTML-sidan)

Temperaturen visas även med siffror på OLED-skärmen.


Jag har printat lådan i träfilament och betsat den. Stjärnan är printad i vit PLA i vase mode

Själva stången som stjärnan sitter på är ett vanligt metallsugrör som finns på ICA eller amazon.se. Lådan och stjärnan är 3D-printad och STL-filer finns bifogade.

De komponenter som behövs är:

- 3D-printad låda och lock
- Wemos D1 mini v3 (varianten med skruvhål)  https://www.aliexpress.com/item/4000420770002.html
- Wemos D1 mini OLED shield https://www.aliexpress.com/item/32872809298.html
- WS2812 LED https://www.aliexpress.com/item/1005001863273661.html
- WS2812 LED (alternativ i Sverige) https://www.electrokit.com/produkt/led-rgb-5mm-adresserbar-ws2812-diffus-5-pack/
- 100 ohm motstånd
- Metallsugrör
- Dupontsladdar
- Smältlim för att få LEDen på plats i stjärnan
- Superlim 
- Microusb-kabel
- Krympslang, framförallt för lödningarna uppe vid LEDen

<h1>Kopplingsschema</h1>


![skiss](https://github.com/duelago/Temperaturlampan/assets/12539239/a163f34b-9bc2-4a24-a90c-1ac9b98ec7bc)




<h1>3D-printad lampa</h1>

![temperaturlampan-tulip](https://github.com/duelago/Temperaturlampan/assets/12539239/1e27401d-4797-4640-9fd7-3ef83dfc9c0f)


<h1>Julversion</h1>

![temperaturlampan-stjarna](https://github.com/duelago/Temperaturlampan/assets/12539239/859001ad-1024-4838-bd3d-e470d4a57013)


Denna versionen av Wemos D1 mini passar 

![Screenshot 2023-10-04 at 19 12 29](https://github.com/duelago/Temperaturlampan/assets/12539239/45842c62-73e3-4499-badd-159bfa92e052)


<h1>Instruktioner</h1>

Användaren ansluter enheten med en vanlig microusb-kabel och mobilladdare. Första gången man ansluter lampan i hemmet dyker en ny accesspunkt upp som heter <b>Temperaturlampan</b> Anslut till denna via en captive portal som poppar upp, eller via IP-adress: http://192.168.4.1

Lampan får nu en IP-adress av DHCP-servern i hemmet. Användaren ansluter sin dator eller telefon till sitt hemmawifi igen och når nu lampan via denna nya IP-adress eller på adressen: http://temperaturlampan.local

I webbinterfacet ställer man in flygplats. Denna sparas i EEPROM och klarar alltså strömavbrott etc. 

![ezgif com-crop](https://github.com/duelago/Temperaturlampan/assets/12539239/41c63c9f-87a0-4780-b7ca-e82fb4401fc8)

Beställde en tredje typ av LED som har denna pinouten

![Screenshot 2023-11-28 at 10 53 52](https://github.com/duelago/Temperaturlampan/assets/12539239/9fed205d-eeeb-43c3-9d69-fbd2a2076dd6)
