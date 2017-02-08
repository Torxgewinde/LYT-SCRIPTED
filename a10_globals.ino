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

/* Details for state CONSTANT_COLOR
   Note that RGB and White LEDs are not supposed to be lit at the same time */
uint8_t g_red=1, g_green=1, g_blue=255;

/* Hostname */
String g_hostname = "LYT8266";

/* remote URL */
String g_remoteurl = "http://lyt.naaa.de";

/* Storage for most recent logging messages */
std::deque<String> log_messages;

#define LOG_LENGTH 100

/* logging messages, delete old entries */
void Log(String text) {
  log_messages.push_back(text);
  log_messages.pop_front();

  Serial.println(text);
}

/* only allow to write to flash if commando was set */
bool enableUpdates = false;
