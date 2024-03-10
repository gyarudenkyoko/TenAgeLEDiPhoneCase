#include <FastLED.h>

#define LED_PIN_WS2812B     2      // WS2812B LEDのデータピン
#define NUM_LEDS_WS2812B    9      // WS2812B LEDの数
#define LED_PIN_LED         4      // 粒LEDのデータピン

#define LED_TYPE_WS2812B    WS2812B
#define COLOR_ORDER_WS2812B GRB

CRGB leds[NUM_LEDS_WS2812B];

// 順番に点灯する色の配列
CRGB colors[] = {CRGB::GhostWhite, CRGB::DeepPink, CRGB::GreenYellow, CRGB::DeepSkyBlue, CRGB::DarkTurquoise, CRGB::Purple};
int colorIndex = 0; // 現在の色のインデックス

unsigned long previousMillis = 0;
const long interval = 1000; // 1秒間LEDが光る

void setup() {
  Serial.begin(9600); // シリアル通信を9600bpsで開始
  FastLED.addLeds<LED_TYPE_WS2812B, LED_PIN_WS2812B, COLOR_ORDER_WS2812B>(leds, NUM_LEDS_WS2812B); // WS2812B LEDストリップの初期化
  pinMode(LED_PIN_LED, OUTPUT); // 粒LEDのピンを出力に設定
}

void loop() {
  int sensorValue = analogRead(A0); // Cdsセルから値を読み取る
  Serial.println(sensorValue); // 値をシリアルモニタに出力
  // Cdsセルの値に応じて動作を制御する
  if (sensorValue <= 10) { // Cdsセルの値が10以下の場合
    unsigned long currentMillis = millis();
    // 1秒間WS2812B LEDと粒LEDを光らせる
    if (currentMillis - previousMillis <= interval) {
      // WS2812B LEDを点灯
      fill_solid(leds, NUM_LEDS_WS2812B, colors[colorIndex]); // 現在の色を設定
      FastLED.show(); // WS2812B LEDを表示

      // 粒LEDを点灯
      digitalWrite(LED_PIN_LED, HIGH);
    } else {
      // WS2812B LEDを消灯
      fill_solid(leds, NUM_LEDS_WS2812B, CRGB::Black); // LEDを消灯
      FastLED.show(); // WS2812B LEDを表示

      // 粒LEDを消灯
      digitalWrite(LED_PIN_LED, LOW);

      // 次の色に進む
      colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));

      previousMillis = currentMillis; // カウントをリセット
    }
  } else { // Cdsセルの値が10より大きい場合
    // WS2812B LEDをピンク色で点灯
    fill_solid(leds, NUM_LEDS_WS2812B, CRGB::DeepPink); // WS2812B LEDをピンク色に設定
    FastLED.show(); // WS2812B LEDを表示

    // 粒LEDを消灯
    digitalWrite(LED_PIN_LED, LOW);

    previousMillis = millis(); // カウントをリセット
  }
}
