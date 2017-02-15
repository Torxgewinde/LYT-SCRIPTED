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

#include <ArduinoJson.h>
#include <deque>

/* global variables */
enum STATES {UNDEF=0, CONSTANTCOLOR=1, REMOTEURL=2, BOOTUP=3, FLASH=4};
STATES state = UNDEF;

/* Variables for RGB (and implicitly white) color
   Note that RGB and White LEDs are not supposed to be lit at the same time */
uint8_t g_red, g_green, g_blue;

/* Hostname, this name will show up in DHCP requests */
String g_hostname;

/* remote URL, this will be used to poll a color value */
String g_remoteurl;

/* an optional delay in seconds before switching from bootup to remoteURL */
uint16_t g_delay_before_going_remotecontrolled;

/* Storage for most recent logging messages */
std::deque<String> log_messages;

#define LOG_LENGTH 100

/* logging messages, maintain length of entries */
void Log(String text) {
  log_messages.push_back(text);
  log_messages.pop_front();

  Serial.println(text);
}

/* only allow to write to flash if commando was set */
bool enableUpdates = false;
