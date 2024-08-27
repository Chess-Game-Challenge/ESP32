#include <Arduino.h>
#include <FastLED.h>

#define LED_COUNT 128
CRGB leds[LED_COUNT];

#define ROW_PINS (uint8_t[]){2, 3, 4, 5, 6, 7, 8, 9}
#define COL_PINS (uint8_t[]){10, 11, 12, 13, 14, 15, 16, 17}

enum Color {
    BLACK,
    WHITE
};

enum Type { //TODO
    PAWN,           // Peão
    BISHOP,         // Bispo
    KNIGHT,         // Cavalo
    ROOK,           // Torre
    QUEEN,          // Rainha
    KING            // Rei
};

struct Piece {
public:
    Color color;
    byte line;
    byte column;
    bool dead;
    bool moving;
};

Piece *refBoard[64];
Piece *moving = nullptr;

void setup() {
    CFastLED::addLeds<NEOPIXEL, 1>(leds, LED_COUNT);

    uint8_t hue = 0;

    for (auto &led: leds) {
        hue += 1;
        hue = hue % 360;

        led.setHSV(hue, 100, 100);
    }

    FastLED.show();
}

void loop() {
    for (int i = 0; i < 8; ++i) {
        digitalWrite(ROW_PINS[i], HIGH);

        for (int j = 0; j < 8; ++j) {
            uint16_t value = analogRead(COL_PINS[j]);
            uint8_t pos = (i + 1) * (j + 1) - 1;

            Piece *piece = refBoard[pos];

            if (value >= 502 && value <= 522) {
                if (piece != nullptr) {
                    piece->moving = true;
                    moving = piece;
                }
            } else if (moving != nullptr) {
                if (piece != nullptr ) { //TODO: Verificar se polo trocou
                    //TODO: Comer peça
                    //piece->dead = true;
                }

                if (piece == nullptr) {
                    refBoard[pos] = moving;
                    moving->moving = false;

                    refBoard[(moving->line + 1) * (moving->column + 1) - 1] = nullptr;
                }
            }
        }

        digitalWrite(ROW_PINS[i], LOW);
    }
}
