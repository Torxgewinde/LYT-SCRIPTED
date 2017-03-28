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
********************************************************************************

Compiler:
- Arduino 1.6.12

Hardware: 
- LYT8266 Lamp

Arduino Settings:
- Generic ESP8266 Module
- CPU Frequency 80 Mhz
- Flash Size 1 M (64k SPIFFS)
  Due to the limited SPIFFS, the Javascript libraries are fetched from CDN or
  hosted at your site if you do not like CDNs

Note: HTTP Updater did not work reliable after a while, after adding three
      delays it finally works now very reliable. The change is in the file:
      .arduino15/packages/esp8266/hardware/esp8266/2.3.0/cores/esp8266/Updater.cpp

      ...
      bool UpdaterClass::_writeBuffer(){

      if(!_async) yield();
      delay(10);
      bool result = ESP.flashEraseSector(_currentAddress/FLASH_SECTOR_SIZE);
      if(!_async) yield();
      delay(10);
      if (result) {
         result = ESP.flashWrite(_currentAddress, (uint32_t*) _buffer, _bufferLen);
      }
      if(!_async) yield();
      delay(10);

      if (!result) {
      ...

*******************************************************************************/
