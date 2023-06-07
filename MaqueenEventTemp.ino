/*
Copyright (c) 2018 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/
#include <Maqueen.h>
Maqueen bot;

//NewPing.h and Adafruit_Microbit.h are inlcuded the library header
//but still need to be invoked here.
Adafruit_Microbit microbit;

void setup() {

  Serial.begin(115200);

  Serial.println("nRF5x Die Temperature Plotting");

  microbit.BTLESerial.setLocalName("microbit");
  microbit.BTLESerial.begin();

  // Start LED matrix driver after radio (required)
  microbit.begin();
  bot.begin();
}

#define OVERSAMPLE 50

void loop() {
  microbit.BTLESerial.poll();

  // Take 'OVERSAMPLES' measurements and average them!
  float avgtemp = 0;
  for (int i = 0; i < OVERSAMPLE; i++) {
    int32_t temp;
    do {
      temp = microbit.getDieTemp();
    } while (temp == 0);  // re run until we get valid data
    avgtemp += temp;
    delay(1);
  }
  avgtemp /= OVERSAMPLE;

  Serial.print("Temperature (C): ");
  Serial.println(avgtemp);  // Float value since temp is in 0.25°C steps
  if (avgtemp > 37) {
    bot.motorRun(M1, CW, 50);  //Motor Number, direction, speed (PWM)
    bot.motorRun(M2, CW, 50);  //Motor Number, direction, speed (PWM)
  }
  // Send just the raw reading over bluetooth
  microbit.BTLESerial.println(avgtemp);

  delay(1000);
}
