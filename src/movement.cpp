#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <PCF8574.h>
#include <display.h>

#define ROTATE_MOTOR_1 12
#define ROTATE_MOTOR_2 13
#define CARD_MOTOR_1 27
#define CARD_MOTOR_2 14

#define HALL_EFFECT 15

void home()
{
    displayHomingUI();

    int hallEffectValue = analogRead(HALL_EFFECT);

    while (hallEffectValue > 1000 && hallEffectValue < 2500)
    {
        digitalWrite(ROTATE_MOTOR_1, HIGH);
        digitalWrite(ROTATE_MOTOR_2, LOW);

        hallEffectValue = analogRead(HALL_EFFECT);
    }

    digitalWrite(ROTATE_MOTOR_1, LOW);
    digitalWrite(ROTATE_MOTOR_2, LOW);
}

int measureSpeed()
{
    unsigned long start = millis();
    home();
    unsigned long end = millis();

    return end - start;
}

void shootCard()
{
    digitalWrite(CARD_MOTOR_1, HIGH);
    digitalWrite(CARD_MOTOR_2, LOW);
    delay(210);

    digitalWrite(CARD_MOTOR_1, LOW);
    digitalWrite(CARD_MOTOR_2, HIGH);
    delay(210);

    digitalWrite(CARD_MOTOR_1, LOW);
    digitalWrite(CARD_MOTOR_2, LOW);
}

void dealCards(int numPlayers, int numCards, int speed)
{
    int offsets[7] = {70, 80, 95, 100, 100, 85, 75};

    int cardInterval = speed / numPlayers - offsets[numPlayers - 2];

    for (int card = 0; card < numCards; card++)
    {
        for (int player = 0; player < numPlayers; player++)
        {
            shootCard();

            digitalWrite(ROTATE_MOTOR_1, HIGH);
            digitalWrite(ROTATE_MOTOR_2, LOW);
            delay(cardInterval);
            digitalWrite(ROTATE_MOTOR_1, LOW);
            digitalWrite(ROTATE_MOTOR_2, LOW);
            delay(100);
        }
    }

    home();
    delay(300);
}
