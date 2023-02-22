/**
 * @file main.cpp
 * @author Jarosław Herman
 * @version 1.0
 * @date 2023-02-14
 */

//--------------------------------------------------------------------------------

#include <BlynkSimpleEsp8266.h>
#include <time_manager.h>
#include <led_constants.h>
#include <credentials.h>

//--------------------------------------------------------------------------------

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = WIFI_SSID_HOME2;
char pass[] = WIFI_PASSWD_HOME2;

int ledOnHour{};
int ledOnMinute{};

//--------------------------------------------------------------------------------

enum Mood
{
  RELAX,
  CONCENTRATE,
  ENERGIZE,
  SLIDER,
  TURNOFF,
  SUNRISE
};

Mood mood = RELAX;

//--------------------------------------------------------------------------------

CRGB leds[NUM_LEDS];


int realMinutes{};
int numSteps{};   // 36 => 1 minute; 1440 - 40min
int stepDelay = 166;
int sunriseAllowed{};

//--------------------------------------------------------------------------------

int hour_flag{};

int red_val, green_val, blue_val;

const int timezone = 1 * 3600;
const int dst = 0;
TimeManager myTimeObject;

//--------------------------------------------------------------------------------

BLYNK_WRITE(V3)
{
  red_val = param[0].asInt();
  mood = SLIDER;
}

BLYNK_WRITE(V4)
{
  green_val = param[0].asInt();
  mood = SLIDER;
}

BLYNK_WRITE(V5)
{
  blue_val = param[0].asInt();
  mood = SLIDER;
}

BLYNK_WRITE(V6)
{
  if (param[0].asInt() == 1)
  {
    mood = RELAX;
    Serial.println("(V6) mood = RELAX");
  }
  else
  {
    mood = TURNOFF;
    Serial.println("(V6) mood = TURNOFF");
  }
}

BLYNK_WRITE(V7)
{
  if (param[0].asInt() == 1)
  {
    mood = CONCENTRATE;
    Serial.println("(V7) mood = CONCENTRATE");
  }
  else
  {
    mood = TURNOFF;
    Serial.println("(V7) mood = TURNOFF");
  }
}

BLYNK_WRITE(V8)
{
  if (param[0].asInt() == 1)
  {
    mood = ENERGIZE;
    Serial.println("(V8) mood = ENERGIZE");
  }
  else
  {
    mood = TURNOFF;
    Serial.println("(V8) mood = TURNOFF");
  }
}

BLYNK_WRITE(V9)
{
  if (param[0].asInt() == 1)
  {
    sunriseAllowed = 1;
    Serial.println("(V9) SUNRISE ALLOWED");
  }
  else
  {
    sunriseAllowed = 0;
    mood = TURNOFF;
    Serial.println("(V9) SUNRISE NOT ALLOWED");
  }
}

BLYNK_WRITE(V10)
{
  ledOnHour = param[0].asInt();
  Serial.print("Led on hour set to ");
  Serial.print(ledOnHour);
  Serial.print("\n");
}

BLYNK_WRITE(V11)
{
  ledOnMinute = param[0].asInt();
  Serial.print("Led on minute set to ");
  Serial.print(ledOnMinute);
  Serial.print("\n");
}

BLYNK_WRITE(V12)
{
  realMinutes = param[0].asInt();
  Serial.print("Sunrise duration set to ");
  Serial.print(realMinutes);
  Serial.print("\n");
}

//--------------------------------------------------------------------------------

void light_leds(const CRGB moody)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = moody;
  }
}

//--------------------------------------------------------------------------------

void setLedTime(int _ledOnHour, int _ledOnMinute)
{
  myTimeObject.updateTime();
  if (sunriseAllowed == 1)
  {
    if (myTimeObject.getHour() == _ledOnHour && myTimeObject.getMinute() == _ledOnMinute)
    {
      mood = SUNRISE;
      Serial.println("Time-triggered mood = SUNRISE");
    }
  }
}

//--------------------------------------------------------------------------------

void make_sunrise()
{
    unsigned long startTime = millis(); // Start time

    // Loop through all the colors
    for (int i = 0; i < (sizeof(colors) / sizeof(CRGB)); i++)
    {
        CRGB color1 = colors[i];
        CRGB color2 = colors[i + 1];

        // Loop through the steps between two colors
        for (int j = 0; j < numSteps; j++)
        {
            // Interpolate between two colors
            CRGB color = color1.lerp8(color2, j * 255 / numSteps);
            for (int k = 0; k < NUM_LEDS; k++)
            {
                leds[k] = color;
            }
            delay(stepDelay);
            FastLED.show();
        }
    }

    unsigned long endTime = millis(); // End time
    unsigned long elapsedTime = endTime - startTime;
    Serial.print("Sunrise loop finished. Elapsed time: ");
    Serial.print(elapsedTime);
    Serial.println(" ms");
    sunriseAllowed = 0;
    mood = TURNOFF;
}

//--------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  configTime(timezone, dst, "pool.ntp.org","myTimeObject.nist.gov");
  Serial.println("\nWaiting for Internet time");
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

//--------------------------------------------------------------------------------

void loop()
{
  Blynk.run();
  switch (mood)
  {
    case RELAX:
      light_leds(Relax);
      break;
    case CONCENTRATE:
      light_leds(Concentrate);
      break;
    case ENERGIZE:
      light_leds(Energize);
      break;
    case TURNOFF:
      light_leds(TurnOff);
    case SLIDER:
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].setRGB(red_val, green_val, blue_val);
      }
      break;
    case SUNRISE:
      Serial.println("Sunrise execution");
      Serial.print("Numsteps: ");
      Serial.print(numSteps);
      make_sunrise();
      break;
  }
  FastLED.show();
  setLedTime(ledOnHour, ledOnMinute);
  numSteps = realMinutes*38;
}