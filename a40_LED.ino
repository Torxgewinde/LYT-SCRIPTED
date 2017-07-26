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

#include <Ticker.h>

#define RED_PIN 13
#define GREEN_PIN 12
#define BLUE_PIN 14
#define WHITE_PIN 2
#define POWER_ENABLE_LED 15

// how often will the animation ticker call the callback, defined in ms
#define ANI_RES 10

/******************************************************************************
Description.: The visual impression of the LEDs is not linear like the PWM,
              thus to make the range match the visual impression better
              a gamma correction is usual. Here, just one color channel
              is adjusted, which fixes the most important issues. Dependencies
              of one color with the others is not improved this way, so color
              shifts in mixed colors are still possible.
              Credit for the LUT values goes to
              http://rgb-123.com/ws2812-color-output/
Input Value.: brightness level for one color (0 .. 255)
Return Value: gamma corrected value (0 .. 255)
******************************************************************************/
uint8_t gamma_correction(uint8_t value) {
  uint8_t gammaE[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
    2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
    6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
    11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
    19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
    29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
    40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
    90, 91, 93, 94, 95, 96, 98, 99,100,102,103,104,106,107,109,110,
    111,113,114,116,117,119,120,121,123,124,126,128,129,131,132,134,
    135,137,138,140,142,143,145,146,148,150,151,153,155,157,158,160,
    162,163,165,167,169,170,172,174,176,178,179,181,183,185,187,189,
    191,193,194,196,198,200,202,204,206,208,210,212,214,216,218,220,
    222,224,227,229,231,233,235,237,239,241,244,246,248,250,252,255};
  
  return gammaE[value];
}

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
    analogWrite(WHITE_PIN, gamma_correction(red));
  } else {
    analogWrite(RED_PIN, gamma_correction(red));
    analogWrite(GREEN_PIN, gamma_correction(green));
    analogWrite(BLUE_PIN, gamma_correction(blue));
    analogWrite(WHITE_PIN, 0);
  }
  
  //without delay I had issues when setting all values 0
  //delay(1);
}

/******************************************************************************
Description.: animation function performs a fade animation
Input Value.: a pointer to context variables required for the animation
Return Value: -
******************************************************************************/
struct _animation_context {
  uint8_t next_red;
  uint8_t next_green;
  uint8_t next_blue;

  uint8_t previous_red;
  uint8_t previous_green;
  uint8_t previous_blue;

  uint8_t current_red;
  uint8_t current_green;
  uint8_t current_blue;

  bool busy;
  uint32_t ani_time;
  uint32_t ani_duration;

  Ticker animationTicker;
} animation_context;

void animation(_animation_context *ctx) {
  float progress;

  ctx->ani_time += ANI_RES;
  progress = _min(1.0, (float)ctx->ani_time / (float)ctx->ani_duration);

  ctx->current_red = ctx->previous_red + (ctx->next_red - ctx->previous_red) * progress;
  ctx->current_green = ctx->previous_green + (ctx->next_green - ctx->previous_green) * progress;
  ctx->current_blue = ctx->previous_blue + (ctx->next_blue - ctx->previous_blue) * progress;

  setLeds(ctx->current_red, ctx->current_green, ctx->current_blue);

  if( progress >= 1.0 ) {
    // stop timer, we reached 100%
    ctx->animationTicker.detach();
    ctx->ani_time = 0;
    ctx->previous_red = ctx->next_red;
    ctx->previous_green = ctx->next_green;
    ctx->previous_blue = ctx->next_blue;
    ctx->busy = false;
  }
}

/******************************************************************************
Description.: set the LEDs, not immediatly but fading from previous color
              to the specified within the specified time
Input Value.: brightness level for RGB, if R=G=B the white LEDs are used
              fade_time is the time of animation in milliseconds
Return Value: -
******************************************************************************/
void setLedsAnimated(uint8_t red, uint8_t green, uint8_t blue, uint32_t duration) {

  // guard against restarting the animation unless new target color specified
  if( (red == animation_context.next_red) && (green == animation_context.next_green) && (blue == animation_context.next_blue) ) {
    // leave if we already currently animate towards or already have set desired color
    return;
  }

  // if animation is still running
  if(animation_context.busy) {
    // already/still busy: stop timer first
    animation_context.animationTicker.detach();

    // restart animation from current colors
    animation_context.previous_red = animation_context.current_red;
    animation_context.previous_green = animation_context.current_green;
    animation_context.previous_blue = animation_context.current_blue;
    
    animation_context.busy = false;
  }

  animation_context.ani_duration = duration;
  animation_context.ani_time = 0;

  // animate towards this color
  animation_context.next_red = red;
  animation_context.next_green = green;
  animation_context.next_blue = blue;

  animation_context.animationTicker.attach_ms(ANI_RES, animation, &animation_context);
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

  animation_context.next_red = 0;
  animation_context.next_green = 0;
  animation_context.next_blue = 0;
  
  animation_context.previous_red = 0;
  animation_context.previous_green = 0;
  animation_context.previous_blue = 0;
  
  animation_context.current_red = 0;
  animation_context.current_green = 0;
  animation_context.current_blue = 0;

  animation_context.busy = false;

  setLedsAnimated(g_red, g_green, g_blue, 1000);
  
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
      setLeds(g_red, g_green, g_blue);
      break;
      
    case CONSTANTCOLOR:
    case REMOTEURL:
      setLedsAnimated(g_red, g_green, g_blue, 1000);
      //setLeds(g_red, g_green, g_blue);
      break;

    default:
      ;
  }
}
