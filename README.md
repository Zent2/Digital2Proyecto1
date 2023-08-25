# Digital2Proyecto1
Este proyecto presenta una catapulta automatizada que utiliza tecnología digital y sensores para ajustar automáticamente el ángulo de lanzamiento según el peso 
del objeto a lanzar y la distancia hacia el objetivo. Se emplean cuatro microcontroladores PIC16F887 y un ESP32 con comunicación I2C. Uno de los microcontroladores 
controla una pantalla LCD y un sensor de peso, otro maneja el sensor de distancia y un motor paso a paso para la orientación de la catapulta, mientras que el tercero
gestiona un sensor de sonido para activar la catapulta y un servomotor para ajustar el ángulo de la catapulta. Por último el microcontrolador maestro coordina las acciones
y se comunica de manera serial con el ESP32. Mientras que el ESP32 se conecta a Adafruit IO para la transferencia de datos en tiempo real y el control remoto.
