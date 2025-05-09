#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <PCF8574.h>
#include <movement.h>

// PCF8574 buttons extender
PCF8574 buttons(0x20);

#define PLAY_BUTTON 1
#define RIGHT_BUTTON 2
#define OPTIONS_BUTTON 0
#define PLUS_BUTTON 6
#define MINUS_BUTTON 4
#define HOME_BUTTON 5

// OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 22, 21);

unsigned int blinkMillis;
unsigned int buttonMillis;

int selected = 0;

struct Option
{
    int value;
    int minValue;
    int maxValue;
    const uint8_t *font;
};

Option options[2] = {
    {2, 2, 6, u8g2_font_ncenB24_tr},  // Number of Players // Clamped at 6 players, as going higher is inconsistent, and depends more on the card properties than the timing.
    {3, 1, 20, u8g2_font_ncenB18_tr}, // Number of Cards
};

void displayInit()
{
    buttons.begin();
    u8g2.begin();
}

void displayMainUI()
{
    char numPlayersChar[2];
    itoa(options[0].value, numPlayersChar, 10);

    char numCardsChar[3];
    itoa(options[1].value, numCardsChar, 10);

    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 60, "Players");

    int numPlayersX = u8g2.getStrWidth("Players") / 2 - u8g2.getStrWidth(numPlayersChar);

    u8g2.setFont(options[0].font);
    u8g2.drawStr(numPlayersX, 40, numPlayersChar);

    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(85, 60, "Cards");

    int numCardsX = u8g2.getStrWidth("Cards") / 2 - u8g2.getStrWidth(numCardsChar) + 85;

    u8g2.setFont(options[1].font);
    u8g2.drawStr(numCardsX, 40, numCardsChar);
}

void displayHomingUI()
{
    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.setFontPosCenter();
    u8g2.drawStr(24, 32, "Homing...");

    u8g2.sendBuffer();
}

void buttonActions(int speed)
{
    static bool plusButtonState;
    static bool minusButtonState;
    static bool rightButtonState;
    static bool playButtonState;
    static bool homeButtonState;
    unsigned int currentMillis = millis();

    if ((currentMillis - buttonMillis) >= 50)
    {
        buttonMillis = currentMillis;

        if (plusButtonState != buttons.readButton(PLUS_BUTTON))
        {
            if (plusButtonState)
            {
                options[selected].value++;

                options[selected].value = constrain(options[selected].value, options[selected].minValue, options[selected].maxValue);
            }
            plusButtonState = !plusButtonState;
        }

        if (minusButtonState != buttons.readButton(MINUS_BUTTON))
        {
            if (minusButtonState)
            {
                options[selected].value--;

                options[selected].value = constrain(options[selected].value, options[selected].minValue, options[selected].maxValue);
            }
            minusButtonState = !minusButtonState;
        }

        if (rightButtonState != buttons.readButton(RIGHT_BUTTON))
        {
            if (rightButtonState)
            {
                selected++;

                if (selected > 1)
                    selected = 0;

                options[selected].font = u8g2_font_ncenB24_tr;

                for (int i = 0; i < 2; i++)
                {
                    if (i == selected)
                        continue;

                    options[i].font = u8g2_font_ncenB18_tr;
                }
            }
            rightButtonState = !rightButtonState;
        }

        if (playButtonState != buttons.readButton(PLAY_BUTTON))
        {
            if (playButtonState)
            {
                delay(300);
                dealCards(options[0].value, options[1].value, speed);
            }
            playButtonState = !playButtonState;
        }

        if (homeButtonState != buttons.readButton(HOME_BUTTON))
        {
            if (homeButtonState)
            {
                delay(300);
                home();
            }
            homeButtonState = !homeButtonState;
        }
    }
}

void display(int speed)
{
    buttonActions(speed);

    u8g2.clearBuffer();

    displayMainUI();

    u8g2.sendBuffer();
}
