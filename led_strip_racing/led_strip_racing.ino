/**
 * @file led_strip_race.ino
 * @author Christian Corsetti (c.corsetti.01@gmail.com)
 * @brief Funny led race with Arduino and WS2812b led strip. Made in Fablab Valsamoggia.
 * @version 0.0.0
 * @date 29-04-2019
 * 
 * @copyright MIT License
 * Copyright (c) [year] [fullname]
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * @note CHANGES
 * @parblock
 *  29-04-2019 - Christian Corsetti:
 *      - Add multiple players.
 * 
 *  01-05-2019 - Christian Corsetti:
 *      - Bug fix: remove third "ghost" player when there are only two players.
 * 
 *  03-05-2019 - Christian Corsetti:
 *      - Add comments and clean code.
 * **FROZEN HERE V0.0.0**
 * @endparblock
 */
/*----------------------------------------------------------------------------*/
/*                                   INCLUDE                                  */
/*----------------------------------------------------------------------------*/
#include <Adafruit_NeoPixel.h>
/*----------------------------------------------------------------------------*/
/*                                   DEFINE                                   */
/*----------------------------------------------------------------------------*/
/// Led strip pin. A 500 <= R <= 1K in series should be used.
#define PIN_LED A0
/// Player 1 Pin switch. Internal pull-up; place the switch between PIN and GND.
#define PIN_P1 7
/// Player 1 Pin switch. Internal pull-up; place the switch between PIN and GND.
#define PIN_P2 6
/// Player 1 Pin switch. Internal pull-up; place the switch between PIN and GND.
#define PIN_P3 5
/// Player 1 Pin switch. Internal pull-up; place the switch between PIN and GND.
#define PIN_P4 4
/// Speaker audio output. a C = 2.2uF should be placed in series between
/// PIN and 8 Ohm speaker.
#define PIN_AUDIO 3 
/// Max number of players. Don't exceed 8.
#define MAX_PLAYERS 4
/// Led in the strip. A 5m 60led/m is the best :).
#define NPIXELS  300
/// Player 1 color. RED
#define COLOR1 track.Color(255, 0, 0)
/// Player 2 color. GREEN
#define COLOR2 track.Color(0, 255, 0)
/// Player 3 color. BLUE
#define COLOR3 track.Color(0, 0, 255)
/// Player 4 color. YELLOW
#define COLOR4 track.Color(255, 255, 0)
/// Player 5 color. PURPLE
#define COLOR5 track.Color(178, 102, 255)
/// Player 6 color. PINK
#define COLOR6 track.Color(204, 0, 102)
/// Player 7 color. ORANGE
#define COLOR7 track.Color(204, 102, 0)
/// Player 8 color. LIGHT BLUE
#define COLOR8 track.Color(153, 255, 255)

/// MIDI Array.
int win_music[] = {2637, 2637, 0, 2637, 0, 2093, 2637, 0, 3136};
/// 127 is the normal gravity. Values under this value means there is a hill,
/// while values above 127 means there is a downhill.
byte gravity_map[NPIXELS];
/// Lenght of start beep.
int TBEEP = 3;
/// It's pretty self-explanatory.
float speeds[MAX_PLAYERS];
/// It's pretty self-explanatory.
float dists[MAX_PLAYERS];
/// It's pretty self-explanatory.
float loops[MAX_PLAYERS];
/// It's pretty self-explanatory.
float flag_sws[MAX_PLAYERS];
/// It's pretty self-explanatory.
int PIN[MAX_PLAYERS];
/// It's pretty self-explanatory.
uint32_t COLORS[MAX_PLAYERS];
/// Total laps race.
byte loop_max = 5;
/// Acceleration value. When a player pushs the button, his speed increases
/// with this value. Otherwise his speed decreases. 
/// An high value means more speed and power :)
float ACEL = 0.2;
/// Simulate the friction between asphalt and car's "wheels". Car speed 
/// decreases every time with this value.
float friction = 0.012;
/// Weight force simulation. Used in loop and ramp "gravity" simulation.
float g_force = 0.004;//0.003; 
/// Neopixel class for the led strip.
Adafruit_NeoPixel track = Adafruit_NeoPixel(NPIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);
/// It's pretty self-explanatory. (in milliseconds).
int tdelay = 5;
//==============================================================================
/**
 * @brief  Set the gravity array for a ramp.
 * @note   
 * @param  ramp_high: high of the ramp; set the "steep".
 * @param  ramp_startl: start led strip for the hill.
 * @param  ramp_centerl: center led of the ramp.
 * @param  ramp_endl: last led of the ramp.
 * @retval None
 */
void set_ramp(byte ramp_high, byte ramp_startl, byte ramp_centerl, byte ramp_endl)
{
    float k;

    k = (float)ramp_high / (ramp_centerl - ramp_startl);
    // Set the hill leds
    for (int i = 0; i < (ramp_centerl - ramp_startl); i++)
    {
        gravity_map[ramp_startl + i] = 127 - i * k;
    }
    gravity_map[ramp_centerl] = 127;

    k = (float)ramp_high / (ramp_endl - ramp_centerl);
    // Set downhill leds
    for (int i = 0; i < (ramp_endl - ramp_centerl); i++)
    {
        gravity_map[ramp_centerl + i + 1] = 127 + ramp_high - i * k;
    }
}
/**
 * @brief  Set the gravity array for a death loop.
 * @note   
 * @param  high: high of the ramp; set the "steep".
 * @param  start: start led strip for the hill.
 * @param  center: center led of the loop.
 * @param  end: last led of the loop.
 * @retval None
 */
void set_loop(byte high, byte start, byte center, byte end)
{
    float k;

    k = (float)high / (center - start);
   
    for (int i = 0; i < (center - start); i++)
    {
        gravity_map[start + i] = 127 - i * ();
    }
    gravity_map[center] = 255;

    k = (float)high / (end - center);
    for (int i = 0; i < (end - center); i++)
    {
        gravity_map[center + i + 1] = 127 + high - i * k;
    }
}
/**
 * @brief  Car one: color GREEN.
 * @note   the number of led increase with the laps.
 * @retval None
 */
void draw_car1(void)
{
    for (int i = 0; i <= loops[0]; i++)
    {
        track.setPixelColor(((word)dists[0] % NPIXELS) + i,
                            track.Color(0, 255 - i * 20, 0));
    }
}
/**
 * @brief  Car two: color RED.
 * @note   the number of led increase with the laps.
 * @retval None
 */
void draw_car2(void)
{
    for (int i = 0; i <= loops[1]; i++)
    {
        track.setPixelColor(((word)dists[1] % NPIXELS) + i,
                            track.Color(255 - i * 20, 0, 0));
    }
}
/**
 * @brief  Car two: color BLUE.
 * @note   the number of led increase with the laps.
 * @retval None
 */
void draw_car3(void)
{
    for (int i = 0; i <= loops[2]; i++)
    {
        track.setPixelColor(((word)dists[2] % NPIXELS) + i,
                            track.Color(0, 0, 255 - i * 20));
    }
}
/**
 * @brief  Car two: color YELLOW.
 * @note   the number of led increase with the laps.
 * @retval None
 */
void draw_car4(void)
{
    for (int i = 0; i <= loops[3]; i++)
    {
        track.setPixelColor(((word)dists[3] % NPIXELS) + i,
                            track.Color(255 - i * 20, 255 - i * 20, 0));
    }
}
/// Functions pointer array. Used for call draw_cars function with a loop. 
/// Increases the execution speed.
void (*draw_cars[MAX_PLAYERS])(void);

void setup()
{
    for (int i = 0; i < NPIXELS; i++)
    {
        gravity_map[i] = 127;
    };
    track.begin();
    pinMode(PIN_P1, INPUT_PULLUP);
    pinMode(PIN_P2, INPUT_PULLUP);
    pinMode(PIN_P3, INPUT_PULLUP);
    pinMode(PIN_P4, INPUT_PULLUP);

    set_ramp(28, 80, 101, 124);
    set_ramp(26, 135, 158, 178);
    //set_loop(10, 20, 50, 80);
    //for (int i = 0; i < NPIXELS; i++)
    //{
      //  track.setPixelColor(i, track.Color(125, 0, (45 - gravity_map[i]) / 8)); //127
    //}

    PIN[0] = PIN_P1;
    PIN[1] = PIN_P2;
    PIN[2] = PIN_P3;
    PIN[3] = PIN_P4;

    COLORS[0] = COLOR2;
    COLORS[1] = COLOR1;
    COLORS[2] = COLOR3;
    COLORS[3] = COLOR5;

    loops[0] = 0;
    loops[1] = 0;
    loops[2] = 0;
    loops[3] = 0;

    draw_cars[0] = draw_car1;
    draw_cars[1] = draw_car2;
    draw_cars[2] = draw_car3;
    draw_cars[3] = draw_car4;

    //randomSeed(analogRead(1));
    
    track.show();
    start_race();
}

void start_race()
{
    for (int i = 0; i < NPIXELS; i++)
    {
        track.setPixelColor(i, track.Color(0, 0, 0));
    }
    track.show();
    delay(2000);
    // Start the Semaphore. TODO: Add start button in v1.0.1
    // Red light 
    track.setPixelColor(5, track.Color(0, 0, 0));
    track.setPixelColor(6, track.Color(0, 0, 0));
    track.setPixelColor(4, track.Color(255, 0, 0));
    track.setPixelColor(3, track.Color(255, 0, 0));
    track.show();
    tone(PIN_AUDIO, 400);
    delay(2000);
    noTone(PIN_AUDIO);
    // Yellow Light
    track.setPixelColor(8, track.Color(0, 0, 0));
    track.setPixelColor(7, track.Color(0, 0, 0));
    track.setPixelColor(6, track.Color(255, 255, 0));
    track.setPixelColor(5, track.Color(255, 255, 0));
    track.show();
    tone(PIN_AUDIO, 600);
    delay(2000);
    noTone(PIN_AUDIO);
    // Green Light
    track.setPixelColor(8, track.Color(0, 255, 0));
    track.setPixelColor(7, track.Color(0, 255, 0));
    track.show();
    tone(PIN_AUDIO, 1200);
    delay(2000);
    noTone(PIN_AUDIO);
}

void winner_fx()
{
    int msize = sizeof(win_music) / sizeof(int);
    for (int note = 0; note < msize; note++)
    {
        tone(PIN_AUDIO, win_music[note], 200);
        delay(230);
        noTone(PIN_AUDIO);
    }
}

void loop()
{
    for(int i=0;i<NPIXELS;i++)
    {
        track.setPixelColor(i, track.Color(0,0,0));
    }
    // Color the ramp and loops
    // Viusalize the leds with gravity not equal to 127.
    //for (int i = 0; i < NPIXELS; i++)
    //{
      //  track.setPixelColor(i, track.Color(0, 0, (127 - gravity_map[i]) / 8));
    //}
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if((flag_sws[i] == 1) && (digitalRead(PIN[i]) == 0))
        {
            flag_sws[i] = 0;
            speeds[i] += ACEL;
        }
        if ((flag_sws[i] == 0) && (digitalRead(PIN[i]) == 1))
        {
            flag_sws[i] = 1;
        }
        if ((gravity_map[(word)dists[i] % NPIXELS]) < 127)
            speeds[i] -= g_force * (127 - (gravity_map[(word)dists[i] % NPIXELS]));
        if ((gravity_map[(word)dists[i] % NPIXELS]) > 127)
            speeds[i] += g_force * ((gravity_map[(word)dists[i] % NPIXELS]) - 127);

        speeds[i] -= speeds[i] * friction;
        dists[i] += speeds[i];
    }
    for (int j = 0; j < MAX_PLAYERS; j++)
    {
        if (dists[j] > NPIXELS * loops[j])
        {
            loops[j]++;
            tone(PIN_AUDIO, 600);
            TBEEP = 2;
        }
    }

    for (int j = 0; j < MAX_PLAYERS; j++)
    {
        if(loops[j] > loop_max)
        {
            for (int i = 0; i < NPIXELS; i++)
            {
                track.setPixelColor(i, COLORS[j]);
            }
            track.show();
            winner_fx();
            for (int k = 0; k < MAX_PLAYERS; k++)
            {
                loops[k] = 0;
                dists[k] = 0;
                speeds[k] = 0;
            }
            start_race();
            break;
        }
    }
    // TODO: use a random function for player display.
    for (int j = 0; j < MAX_PLAYERS; j++)
    {
        (*draw_cars[j])();
    }

    track.show();
    delay(tdelay);

    if (TBEEP > 0)
    {
        TBEEP -= 1;
        if (TBEEP == 0)
        {
            noTone(PIN_AUDIO);
        }
    }
}

int display_lcd_laps()
{
    //to do
}

int display_lcd_time()
{
    //to do
}
