# Manual de Conexionado Físico (SRAGV)

Este manual documenta de forma exhaustiva el cableado hacia la protoboard basándose estrictamente en el plano esquemático de los conectores de extensión (Morpho y Arduino) de la placa **STM32 NUCLEO-F103RB**.

---

### 1. BLOQUE DISPLAY LCD 16x2 (Modo 4-bits)
El LCD se mapeó al Puerto C para mantener el código limpio. Como el pin `PC7` no se encuentra en el Morpho derecho, se toma del conector Arduino CN5/CN9.
*Nota: Todos los pines de datos van directo a los pines del LCD. Recordar conectar el ánodo del Backlight (A) a 5V con una resistencia de 220Ω y el cátodo (K) a GND.*

| Componente LCD | Puerto STM32 | Conector Físico | Pin / Etiqueta | Notas de Conexión |
| :--- | :--- | :--- | :--- | :--- |
| **RS** (Register Select)| `PC4` | **CN10** (Morpho) | Pin 34 | - |
| **EN** (Enable) | `PC5` | **CN10** (Morpho) | Pin 6 | - |
| **D4** (Data 4) | `PC6` | **CN10** (Morpho) | Pin 4 | - |
| **D5** (Data 5) | `PC7` | **CN9** (Arduino) | Pin 2 (`D9`) | ¡Atención! Está en el header Arduino. |
| **D6** (Data 6) | `PC8` | **CN10** (Morpho) | Pin 2 | - |
| **D7** (Data 7) | `PC9` | **CN10** (Morpho) | Pin 1 | - |
| **VO** (Contraste) | - | Protoboard | Potenciómetro | Pin central de un pote de 10k (extremos a VCC/GND). |
| **VDD / VSS** | - | **CN6** / Protoboard | `5V` y `GND` | Alimentación lógica del display. |

---

### 2. BLOQUE BOTONES (UP, DOWN, ENTER)
*Nota sobre el plano:* Aunque consultaste por el CN9, analizando el esquemático oficial de la Nucleo, los pines `PB0`, `PB1` y `PB2` se encuentran distribuidos entre los conectores CN8 y CN10. 
*Conexión:* Pulsador a GND y usar pull-up interna del micro (o resistencia externa de 10k a 3.3V).

| Componente | Puerto STM32 | Conector Físico | Pin / Etiqueta | Notas de Conexión |
| :--- | :--- | :--- | :--- | :--- |
| **Botón UP** | `PB0` | **CN8** (Arduino) | Pin 4 (`A3`) | - |
| **Botón DOWN** | `PB1` | **CN10** (Morpho) | Pin 24 | - |
| **Botón ENTER** | `PB2` | **CN10** (Morpho) | Pin 21 | - |

---

### 3. BLOQUE SENSORES ANALÓGICOS
Perfectamente alineados con el header analógico de Arduino.

| Componente | Puerto STM32 | Conector Físico | Pin / Etiqueta | Notas de Conexión |
| :--- | :--- | :--- | :--- | :--- |
| **Velocidad Viento** | `PA0` | **CN8** (Arduino) | Pin 1 (`A0`) | Salida analógica del Joystick X. |
| **Dirección Viento** | `PA1` | **CN8** (Arduino) | Pin 2 (`A1`) | Salida analógica del Joystick Y. |
| **Sensor de Luz (LDR)**| `PA4` | **CN8** (Arduino) | Pin 3 (`A2`) | Punto medio del divisor resistivo (LDR + Resistencia a GND). |

---

### 4. BLOQUE ALERTAS Y SECTORES
*Conexión de LEDs:* El pin del STM32 va al ánodo del LED, y el cátodo a GND mediante una resistencia de 330Ω.
*Buzzer:* Si es pasivo usar un transistor NPN para manejar la corriente; si es módulo activo conectar directo la señal.

| Componente | Puerto STM32 | Conector Físico | Pin / Etiqueta | Notas de Conexión |
| :--- | :--- | :--- | :--- | :--- |
| **LED RUN** | `PB10` | **CN9** (Arduino) | Pin 7 (`D6`) | - |
| **LED ALERTA** | `PB11` | **CN10** (Morpho) | Pin 18 | - |
| **Sector Norte (N)** | `PA6` | **CN5** (Arduino) | Pin 5 (`D12`) | - |
| **Sector Sur (S)** | `PA7` | **CN5** (Arduino) | Pin 4 (`D11`) | - |
| **Sector Este (E)** | `PB6` | **CN5** (Arduino) | Pin 3 (`D10`) | - |
| **Sector Oeste (W)**| `PB7` | **CN7** (Morpho) | Pin 21 | - |
| **Buzzer** | `PB12` | **CN10** (Morpho) | Pin 16 | Activo durante falla. |

---

### 5. BLOQUE BLUETOOTH / LOGGER (HM-10)
Comunicación serial cruzada (TX del micro al RX del módulo, y RX del micro al TX del módulo).

| Componente | Puerto STM32 | Conector Físico | Pin / Etiqueta | Notas de Conexión |
| :--- | :--- | :--- | :--- | :--- |
| **TX (Transmite)** | `PA2` | **CN9** (Arduino) | Pin 2 (`D1`) | Conectar al pin **RXD** del módulo Bluetooth. |
| **RX (Recibe)** | `PA3` | **CN9** (Arduino) | Pin 1 (`D0`) | Conectar al pin **TXD** del módulo Bluetooth. |

---

### 6. BLOQUE ALIMENTACIÓN GENERAL (Power)
Para llevar las líneas de voltaje a las tiras de la protoboard.

| Señal | Conectores Disponibles | Pines Específicos | Notas de Conexión |
| :--- | :--- | :--- | :--- |
| **+5V** | **CN6** (Arduino) | Pin 5 (`5V`) | Usar para el LCD y el módulo Bluetooth (si requiere 5V). |
| **+3.3V** | **CN6** (Arduino) | Pin 4 (`3V3`) | Usar para el divisor del LDR y los Joysticks (ADC a 3.3V max). |
| **GND** | **CN6** / **CN5** (Arduino) | CN6: Pines 6 y 7. CN5: Pin 7 | Unificar **todas** las tierras en la protoboard. |
