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

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   

/******************************************************************************
Description.: prepare the WiFi settings, uses WiFi Manager Library
Input Value.: -
Return Value: -
******************************************************************************/
void setup_wifi() {
  WiFiManager wifiManager;

  //reset settings - for testing
  //wifiManager.resetSettings();
  
  wifiManager.setConfigPortalTimeout(300);
  wifiManager.autoConnect("LYT8266", "addiwau+1");

  WiFi.setOutputPower(20.5);
}

/******************************************************************************
Description.: no permanent task necessary
Input Value.: -
Return Value: -
******************************************************************************/
void loop_wifi() {
  //nothing to do here
}
