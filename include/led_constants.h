/**
 * @file led_constants.h
 * @author Jarosław Herman
 * @version 1.0
 * @date 2023-02-14
 */

//--------------------------------------------------------------------------------

#ifndef LED_MANAGER_H
#define LED_MANAGER_H

//--------------------------------------------------------------------------------

#include <FastLED.h>

//--------------------------------------------------------------------------------

#define LED_PIN D4
#define NUM_LEDS 30
#define BRIGHTNESS 128
#define LED_TYPE WS2812B // type of LED strip

//--------------------------------------------------------------------------------

// RGB values for different moods
const CRGB Relax = CRGB(50, 153, 153);        // green-blue
const CRGB Concentrate = CRGB(255, 102, 102); // bright red-pink
const CRGB Energize = CRGB(255, 153, 51);     // yellow-orange

// Define RGB values for each sunrise color
const CRGB dimRed = CRGB(255, 50, 0);
const CRGB orange = CRGB(255, 150, 0);
const CRGB yellow = CRGB(255, 255, 0);
const CRGB lightYellow = CRGB(255, 255, 100);
const CRGB lightYellowGreen = CRGB(200, 255, 100);
const CRGB brightWhite = CRGB(255, 255, 255);

const CRGB TurnOff = CRGB(0,0,0);

const CRGB colors[] = {dimRed, dimRed, orange, yellow, yellow, lightYellow,
                         lightYellowGreen, brightWhite, brightWhite};

//--------------------------------------------------------------------------------

#endif