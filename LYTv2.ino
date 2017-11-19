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
- Arduino 1.8.2

Hardware: 
- LYT8266 Lamp

Arduino Settings:
- Generic ESP8266 Module
- CPU Frequency 80 Mhz
- Flash Size 1 M (64k SPIFFS)
- Flash Mode is DIO
- Crystal Frequency is 26 MHz
  Due to the limited SPIFFS, the Javascript libraries are fetched from CDN or
  hosted at your site if you do not like CDNs
  
*******************************************************************************/
