/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "Campos5_55"
#define IO_KEY       "aio_AJns15r2QYcWEnj9uSPYIXzPHVKW"

#define WIFI_SSID "iPhone de Campos"
#define WIFI_PASS "Teamo555"

#include "AdafruitIO_WiFi.h"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
