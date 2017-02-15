/*******************************************************************************
#                                                                              #
#      LYT-SCRIPTED, an alternative firmware for LYT8266 RGBW LED lamps        #
#                                                                              #
#                                                                              #
#      Copyright (C) 2017 Tom Stöveken                                         #
#                                                                              #
# This program is free software; you can redistribute it and/or modify         #
# it under the terms of the GNU General Public License as published by         #
# the Free Software Foundation; version 2 of the License.                      #
#                                                                              #
# This program is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with this program; if not, write to the Free Software                  #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    #
#                                                                              #
*******************************************************************************/

#define RED_PIN 13
#define GREEN_PIN 12
#define BLUE_PIN 14
#define WHITE_PIN 2
#define POWER_ENABLE_LED 15

/******************************************************************************
Description.: set the LED, if RGB are set to the same value the white LEDs are
              used. Operating White and RGB LEDs is not OK according to
              the manufacturer (operating all at once would damage the bulb).
Input Value.: brightness level for RGB, if R=G=B the white LEDs are used
Return Value: -
******************************************************************************/
void setLeds(uint8_t red, uint8_t green, uint8_t blue) {
  if(red == green && green == blue) {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
    analogWrite(WHITE_PIN, red);
  } else {
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
    analogWrite(WHITE_PIN, 0);
  }
  
  //without delay I had issues when setting all values 0
  delay(1);
}

/******************************************************************************
Description.: prepare the LEDs, switch on to show bulb is working and provide
              light
Input Value.: -
Return Value: -
******************************************************************************/
void setup_LEDs() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(WHITE_PIN, OUTPUT);
  pinMode(POWER_ENABLE_LED, OUTPUT);
  
  analogWriteFreq(500);
  analogWriteRange(255);
  
  setLeds(g_red, g_green, g_blue);
  digitalWrite(POWER_ENABLE_LED, 1);
}

/******************************************************************************
Description.: set the RGBW LEDs as defined by state and global colors variables
Input Value.: -
Return Value: -
******************************************************************************/
void loop_LEDs() {  
  switch(state) {
    case BOOTUP:
      setLeds(255,255,255);
      break;
      
    case CONSTANTCOLOR:
    case REMOTEURL:
      setLeds(g_red,g_green,g_blue);
      break;

    default:
      ;
  }
}
