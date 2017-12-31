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

//determine array length
#define LENGTH_OF(x) (sizeof(x)/sizeof(x[0]))

/* current state of the device */
enum STATES {UNDEF=0, CONSTANTCOLOR, REMOTEURL, REMOTEURL_POSTPONED, BOOTUP, FLASH, DISCO};
struct {
  STATES state;
  String human_readable_string;
  String state_as_string;
} state_map[] = {
  { UNDEF, "Undefined State", "undef" },
  { CONSTANTCOLOR,"Static Color Mode", "constantcolor" },
  { REMOTEURL, "Polling Remote Address", "remotecontrol" },
  { REMOTEURL_POSTPONED, "Waiting before polling", "remotecontrol_postponed" },
  { BOOTUP, "Booting up...", "bootup" },
  { FLASH, "Flashing Firmware...", "flash" },
  { DISCO, "Disco Mode", "disco" }
};
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

bool g_send_WLAN_keep_alive_packet;

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
