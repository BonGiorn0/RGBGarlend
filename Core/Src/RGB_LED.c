#include "RGB_LED.h"
#include "math.h"

#define RED_MASK 		0xFF0000
#define GREEN_MASK 		0x00FF00
#define BLUE_MASK 		0x0000FF
#define GAMMA 			2.8
#define RED_INTENSITY 	1200.0
#define GREEN_INTENSITY 2000.0
#define BLUE_INTENSITY 	1200.0

void RGB_LED_Init(RGB_LED_t *led, TIM_HandleTypeDef *tim,
		uint32_t r_tim_channel, uint32_t g_tim_channel, uint32_t b_tim_channel)
{
	led->tim = tim;
	led->red_tim_channel = r_tim_channel;
	led->green_tim_channel = g_tim_channel;
	led->blue_tim_channel = b_tim_channel;
	led->hex_color = 0;
	led->on = 0;
}

static void RGB_LED_Update(RGB_LED_t *led)
{
	float r_brightness = (float)((led->hex_color & RED_MASK) >> 16) / 255.f;
	float g_brightness = (float)((led->hex_color & GREEN_MASK) >> 8) / 255.f;
	float b_brightness = (float)(led->hex_color & BLUE_MASK) / 255.f;

	//g_brightness = scaleColor(g_brightness, 2000.f, 1200.f); // Scale to match the highest intensity (GREEN_INTENSITY)

	uint32_t r_pulse =
			(uint32_t)(pow(r_brightness, GAMMA) * led->tim->Init.Period);
	uint32_t g_pulse =
			(uint32_t)(pow(g_brightness, GAMMA) * led->tim->Init.Period);
	uint32_t b_pulse =
			(uint32_t)(pow(b_brightness, GAMMA) * led->tim->Init.Period);

	__HAL_TIM_SET_COMPARE(led->tim, led->red_tim_channel, r_pulse);
	__HAL_TIM_SET_COMPARE(led->tim, led->green_tim_channel, g_pulse);
	__HAL_TIM_SET_COMPARE(led->tim, led->blue_tim_channel, b_pulse);

}
void RGB_LED_SetColor(RGB_LED_t *led, uint32_t hex_color)
{
	led->hex_color = hex_color;
	if(led->on)
		RGB_LED_Update(led);
}

void RGB_LED_On(RGB_LED_t *led)
{
	led->on = 1;
	RGB_LED_Update(led);
	//RGB_LED_SetColor(led, led->hex_color);
}

void RGB_LED_Off(RGB_LED_t *led)
{
	led->on = 0;
	__HAL_TIM_SET_COMPARE(led->tim, led->red_tim_channel, 0);
	__HAL_TIM_SET_COMPARE(led->tim, led->green_tim_channel, 0);
	__HAL_TIM_SET_COMPARE(led->tim, led->blue_tim_channel, 0);

}

uint32_t interpolateColors(uint32_t color1, uint32_t color2, float ratio) {
      uint8_t r1 = (color1 >> 16) & 0xFF;
      uint8_t g1 = (color1 >> 8) & 0xFF;
      uint8_t b1 = color1 & 0xFF;

      uint8_t r2 = (color2 >> 16) & 0xFF;
      uint8_t g2 = (color2 >> 8) & 0xFF;
      uint8_t b2 = color2 & 0xFF;

      uint8_t r = r1 + ratio * (r2 - r1);
      uint8_t g = g1 + ratio * (g2 - g1);
      uint8_t b = b1 + ratio * (b2 - b1);

      return (r << 16) | (g << 8) | b;
  }

void RGB_LED_Gradient(RGB_LED_t *led, uint32_t col1, uint32_t col2, float duration, float step)
{
	for(float t = 0; t < 1; t += step)
	{
		uint32_t color = interpolateColors(col1, col2, t);
		RGB_LED_SetColor(led, color);
		HAL_Delay(duration / step);
	}
}
