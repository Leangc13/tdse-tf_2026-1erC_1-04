# Mapa de Pines del Proyecto SRAGV

A continuación se detalla el conexionado físico (Pinout) diseñado para la placa de desarrollo **STM32 NUCLEO-F103RB**. Toda esta asignación se encuentra mapeada en el archivo `Core/Inc/board.h`.

## 1. Tabla de Entradas (Inputs)

| Componente | Puerto / Pin STM32 | Estado Lógico Activo | Notas |
| :--- | :--- | :--- | :--- |
| **Botón UP** | `GPIOB`, `PIN_0` | `RESET` (Bajo) | Asume resistencia pull-up interna/externa. |
| **Botón DOWN** | `GPIOB`, `PIN_1` | `RESET` (Bajo) | Asume resistencia pull-up interna/externa. |
| **Botón ENTER** | `GPIOB`, `PIN_2` | `RESET` (Bajo) | Asume resistencia pull-up interna/externa. |
| **DIP Switch 1** | `GPIOC`, `PIN_0` | `RESET` (Bajo) | Configuración del sistema. |
| **DIP Switch 2** | `GPIOC`, `PIN_1` | `RESET` (Bajo) | Configuración del sistema. |
| **DIP Switch 3** | `GPIOC`, `PIN_2` | `RESET` (Bajo) | Configuración del sistema. |
| **DIP Switch 4** | `GPIOC`, `PIN_3` | `RESET` (Bajo) | Configuración del sistema. |
| **ADC: Velocidad Viento** (Joystick X) | `GPIOA`, `PIN_0` | Analógico (ADC1_IN0) | Equivalente al conector Arduino A0. |
| **ADC: Dirección Viento** (Joystick Y) | `GPIOA`, `PIN_1` | Analógico (ADC1_IN1) | Equivalente al conector Arduino A1. |
| **ADC: Sensor Luz** (LDR) | `GPIOA`, `PIN_4` | Analógico (ADC1_IN4) | Equivalente al conector Arduino A2. |


## 2. Tabla de Salidas (Outputs)

| Componente | Puerto / Pin STM32 | Estado Lógico Activo | Notas |
| :--- | :--- | :--- | :--- |
| **LED_PWR** | `GPIOA`, `PIN_5` | `SET` (Alto) | **LD2 integrado** en la placa Nucleo. |
| **LED_RUN** | `GPIOB`, `PIN_10` | `SET` (Alto) | Indicador de ciclo de ejecución. |
| **LED_ALERTA** | `GPIOB`, `PIN_11` | `SET` (Alto) | Enciende parpadeando durante `MODO_FALLA`. |
| **Sector Norte (N)** | `GPIOA`, `PIN_6` | `SET` (Alto) | Válvula / LED del sector de riego. |
| **Sector Sur (S)** | `GPIOA`, `PIN_7` | `SET` (Alto) | Válvula / LED del sector de riego. |
| **Sector Este (E)** | `GPIOB`, `PIN_6` | `SET` (Alto) | Válvula / LED del sector de riego. |
| **Sector Oeste (W)** | `GPIOB`, `PIN_7` | `SET` (Alto) | Válvula / LED del sector de riego. |
| **Buzzer** | `GPIOB`, `PIN_12` | `SET` (Alto) | Alarma sonora, se activa en `MODO_FALLA`. |
| **LCD RS** | `GPIOC`, `PIN_4` | `N/A` | Selección de Registro del LCD. |
| **LCD EN** | `GPIOC`, `PIN_5` | `N/A` | Enable del LCD. |
| **LCD D4** | `GPIOC`, `PIN_6` | `N/A` | Bus de datos (4-bits). |
| **LCD D5** | `GPIOC`, `PIN_7` | `N/A` | Bus de datos (4-bits). |
| **LCD D6** | `GPIOC`, `PIN_8` | `N/A` | Bus de datos (4-bits). |
| **LCD D7** | `GPIOC`, `PIN_9` | `N/A` | Bus de datos (4-bits). |

---
**Nota sobre I2C y UART:** Adicionalmente, recordar que los pines I2C1 (ahora de uso exclusivo para la memoria EEPROM) se configuran automáticamente en PB8 (SCL) y PB9 (SDA), y el USART2 (Bluetooth) en PA2 (TX) y PA3 (RX) según la topología default de Nucleo.
