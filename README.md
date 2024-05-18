# Whamageddonlamp

<b>EDIT:</> Nu även i <b>sommarversion.</b> Kompilera filen <b>bangolf.ino</b> om ni vill ha koll på SM i Bangolf 2012 i Kalmar och när den spelas på Mix Megapol

En 3D-printad lampa som ändrar färg beroende på yttertemperatur och som specialfeature blinkar när Last Christmas med Wham spelas på Mix Megapol. Lampan spelar även några toner av Last Christmas med hjälp av en störig buzzer. 

Kompilera filen whamageddonlamp.ino  i Arduino IDE eller så laddar du ner .bin-filen och flashar direkt med exempelvis NodeMCUflasher

Lampan består av en adresserbar WS2812 LED som ändrar färg beroende på yttertemperaturen. Datakällan är valfri större flygplats i världen. Via ett webbinterface matar man in den METAR-kod som flygplatsen har. (De vanligaste svenska flygplatserna finns listade på HTML-sidan)

Temperaturen visas även med siffror på OLED-skärmen och "allt" väder presenteras i webbinterfacet.
Lampan kollar spellistan mot Mix Megapol var 30 sekund för att upplysa dig om när 'Last Christmas' spelas. Vid träff börjar lampan blinka. En buzzer krämar dessutom ur sig några välkända noter. Oljudet hörs dock bara en gång per spelning av låten. Blinkandet pågår tills låten är slut.  

Här är en video som visar hur det funkar (utan buzzern) : https://youtu.be/staYxQRvpsY

Jag har printat lådan i träfilament och betsat den. Stjärnan är printad i vit PLA i vase mode.
Lådan finns att printa i två olika versioner och STEP/STL-filer finns att ladda ner.  Välj om du vill ha versionen med eller utan fästen för M 2.5 skruvar. Den med skruv är bättre, men har du inga M 2.5 liggandes så funkar versionen utan skruv om du säkrar Wemosen med smältlim. 

Själva stången som stjärnan sitter på är ett vanligt metallsugrör som finns på ICA eller amazon.se. 
De komponenter som behövs är:

- 3D-printad låda och lock
- Wemos D1 mini v3 (varianten med skruvhål)  https://www.aliexpress.com/item/4000420770002.html
- Wemos D1 mini OLED shield https://www.aliexpress.com/item/32804426981.html
- WS2812 LED https://www.aliexpress.com/item/1005001863273661.html
- WS2812 LED (alternativ i Sverige) https://www.electrokit.com/produkt/led-rgb-5mm-adresserbar-ws2812-diffus-5-pack/
- 100 ohm motstånd
- Passiv Buzzer https://www.aliexpress.com/item/1005006213298069.html
- Metallsugrör https://www.amazon.se/Alfresco-metallsugr%C3%B6r-%C3%A5teranv%C3%A4ndbara-reng%C3%B6ringsborstar-cocktail/dp/B0BKT4334T/ref=sr_1_17
- Dupontsladdar
- Smältlim för att få LEDen på plats i stjärnan
- Superlim 
- Microusb-kabel
- Krympslang (för lödningarna vid LEDen)
- 2 st M 2.5 skruvar (Funkar även utan om du printar den versionen av lådan)

<h1>Kopplingsschema</h1>


![templampan](https://github.com/duelago/Temperaturlampan/assets/12539239/ebb60627-cd37-4e9d-8fff-efc2866f2104)






<h1>3D-printad lampa</h1>

![whamageddonlampan](https://github.com/duelago/Temperaturlampan/assets/12539239/6cb68040-5377-46a6-b174-873620f04d1d)


<h1>Julversion</h1>

![temperaturlampan-stjarna](https://github.com/duelago/Temperaturlampan/assets/12539239/859001ad-1024-4838-bd3d-e470d4a57013)


Denna versionen av Wemos D1 mini med skruvhål passar i lådan 

![Screenshot 2023-10-04 at 19 12 29](https://github.com/duelago/Temperaturlampan/assets/12539239/45842c62-73e3-4499-badd-159bfa92e052)


<h1>Instruktioner</h1>

Jag printar mina lådor och lock i PLA med 0.6 nozzle och 0.3 mm lagerhöjd. Själva stjärnan printar jag i vase mode med 0.4 nozzle och 0.2 mm lagerhöjd i vit PLA. Lådan går garanterat fint att printa med 0.4 nozzle även den, men det tar naturligtvis längre tid.

I övrigt är det bara att titta på kopplingsschemat och löda kablarna på plats och vara uppmärksam på vilka ben på LEDen som ska kopplas var. Jag använder lite smältlim för att få LEDen på plats i själva stjärnan. Glöm inte krympslang för att separera de tre kontaktytorna till LEDen. I julversionen limmar jag ihop de två delarna av stjärnan med superlim och med "tulpanen" limmar jag den lilla adaptern med superlim.

Användaren ansluter enheten med en vanlig microusb-kabel och mobilladdare. Första gången man ansluter lampan i hemmet dyker en ny accesspunkt upp som heter <b>Whamageddonlamp</b> Anslut till denna via den captive portal som poppar upp, eller via IP-adress: http://192.168.4.1

Lampan får nu en IP-adress av DHCP-servern i hemmet. Användaren ansluter sin dator eller telefon till sitt hemmawifi igen och når nu lampan via denna nya IP-adress eller på adressen: http://whamageddonlamp.local Funkar inte det så använd appen Fing eller liknande på din telefon för att lista ut vilken IP-adress lampan har fått. 

I webbinterfacet ställer man in flygplats. Denna sparas i EEPROM och klarar alltså strömavbrott etc. 

![ezgif com-crop](https://github.com/duelago/Temperaturlampan/assets/12539239/41c63c9f-87a0-4780-b7ca-e82fb4401fc8)


