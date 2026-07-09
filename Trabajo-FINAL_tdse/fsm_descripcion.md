# Descripción de la Máquina de Estados Finitos (FSM) - SRAGV

La lógica central del Sistema Riego Automático con Gestión de Viento (SRAGV) se encuentra modelada como una Máquina de Estados Finitos no bloqueante dentro del módulo `task_system.c`. Esta tarea evalúa de forma periódica las condiciones del sistema y toma decisiones para comandar a los actuadores.

A continuación se detallan los 3 estados principales del sistema, las transiciones entre ellos y las salidas asociadas:

## 1. Lista de Estados

La FSM se compone de los siguientes estados (definidos en la enumeración `task_system_st_t`):
- **`MODO_NORMAL`**: Es el estado de ejecución por defecto. El sistema monitorea continuamente el viento y la luz, y habilita/inhibe los sectores de riego correspondientes de manera dinámica.
- **`MODO_SET_UP`**: Estado de configuración. Se detiene todo el riego temporalmente para permitir al usuario navegar por el menú del LCD y configurar los umbrales de viento u otras opciones.
- **`MODO_FALLA`**: Estado de error crítico o emergencia. Se activa si un componente falla (ej. error en la lectura del ADC) o si el viento supera el umbral crítico configurado. El sistema de riego se apaga por seguridad y se activan alarmas visuales/sonoras.

---

## 2. Eventos y Transiciones

El cambio entre estados ocurre de manera controlada (Event-Driven) en función de las variables ambientales y la interacción del usuario:

### De `MODO_NORMAL` a `MODO_SET_UP`
- **Condición (Trigger):** Se oprime el botón `BTN_ENTER` (detectado como el evento `EV_SYS_BTN_ENTER`).
- **Acción:** El sistema transiciona a `MODO_SET_UP`.

### De `MODO_SET_UP` a `MODO_NORMAL`
- **Condición (Trigger):** Se vuelve a oprimir el botón `BTN_ENTER` tras finalizar la configuración.
- **Acción:** Los parámetros se guardan automáticamente en la memoria EEPROM y el sistema vuelve a `MODO_NORMAL`.

### De `MODO_NORMAL` (o `MODO_SET_UP`) a `MODO_FALLA`
- **Condición (Trigger):** Ocurre un error de Hardware (falla el ADC) **O** la velocidad de viento reportada por el sensor supera el `Umbral Crítico`.
- **Acción:** El sistema aborta lo que estaba haciendo e ingresa forzosamente a `MODO_FALLA` por seguridad.

### De `MODO_FALLA` a `MODO_NORMAL`
- **Condición (Trigger):** Las condiciones de falla desaparecen físicamente (el ADC se recupera y el viento disminuye por debajo del umbral crítico) **Y** el usuario oprime el botón correspondiente o se envía el comando Bluetooth de reset (`EV_SYS_ERROR_CLEARED`).
- **Acción:** Las alarmas se desactivan y el sistema regresa a `MODO_NORMAL`.

---

## 3. Salidas (Outputs) en cada Estado

Cada estado define qué actuadores y componentes visuales deben estar operando:

### En `MODO_NORMAL`
- **LED RUN:** Encendido continuo (indicando operación saludable).
- **LED ALERTA:** Apagado.
- **Buzzer:** Apagado.
- **Sectores de Riego (N, S, E, W):** Se evalúan dinámicamente. 
  - Si es de noche (inhibición nocturna activa): Todos apagados.
  - Si el viento supera el *Umbral Moderado*: Se apaga puntualmente el sector opuesto de donde sopla el viento (Ej: si sopla del Norte, se apaga el Sur).
  - De lo contrario, se encienden los permitidos.

### En `MODO_SET_UP`
- **LED RUN:** Apagado.
- **LED ALERTA:** Apagado.
- **Buzzer:** Apagado.
- **Sectores de Riego (N, S, E, W):** Todos **suspendidos** forzosamente mientras el usuario cambia la configuración en el LCD.

### En `MODO_FALLA`
- **LED RUN:** Apagado.
- **LED ALERTA:** Encendido. *(Nota: La tarea `task_actuator` es la encargada de hacerlo parpadear a 1Hz de forma no bloqueante al detectar este estado)*.
- **Buzzer:** Encendido intermitente.
- **Sectores de Riego (N, S, E, W):** Todos **suspendidos** (aislados) por seguridad, evitando desperdicio de agua o daño físico al sistema de tuberías ante la tormenta.
