#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

gpio_num_t usedPin = GPIO_NUM_NC;
const gpio_num_t gpioPins[] = {GPIO_NUM_1, GPIO_NUM_0, GPIO_NUM_6, GPIO_NUM_5, GPIO_NUM_4};

gpio_num_t check_which_data_pin() {
    for (gpio_num_t pin : gpioPins) {
      gpio_reset_pin(pin);
      gpio_set_direction(pin, GPIO_MODE_INPUT);
      int level = gpio_get_level(pin);
      if (level == 0){
        return pin;
      }
    }
    return GPIO_NUM_NC;
}

void setup() {
  Serial.begin(115200);
  while (usedPin == GPIO_NUM_NC){
    usedPin = check_which_data_pin();
  }

  Serial.print("Angeschlossener Pin gefunden: ");
  switch (usedPin) {
    case GPIO_NUM_1:
      Serial.println(1);
      break;
    case GPIO_NUM_0:
      Serial.println(0);
      break;
    case GPIO_NUM_6:
      Serial.println(6);
      break;
    case GPIO_NUM_5:
      Serial.println(5);
      break;
    case GPIO_NUM_4:
      Serial.println(4);
      break;
    default:
      Serial.println("Irgendetwas geht schief");
      break;
  }
}

void loop() {
  Serial.println("Funktion erfolgreich beendet!");
  delay(5000);
}
