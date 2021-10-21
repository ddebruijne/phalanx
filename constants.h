#define GPIO_DATA 13
#define GPIO_CLOCK 14
#define GPIO_LATCH 15
#define TIMER_INTERVAL_uS 200 // 200 = safe value for 6 digits. You can go down to 150 for 4-digit one. Going too low will cause crashes.

uint8_t bnum[12] = {
    B11111100, // 0
    B01100000, // 1
    B11011010, // 2
    B11110010, // 3
    B01100110, // 4
    B10110110, // 5
    B10111110, // 6
    B11100000, // 7
    B11111110, // 8
    B11110110, // 9
    B00000001, // .
    B00000000  // blank
};

class letters
{
public:
    static const uint8_t h = B00101110;
    static const uint8_t E = B10011110;
    static const uint8_t L = B00011100;
    static const uint8_t l = B00001100;
    static const uint8_t o = B00111010;
    static const uint8_t dot = B00000001;
    static const uint8_t blank = B00000000;
};