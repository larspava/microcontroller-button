#include <Arduino.h>

#define BUTTON_PIN 2 //pino do botão
#define INTERVAL 10000 // valor para 10 segundos 

// Enum com os estados do botão
typedef enum {
    STATE_OFF,
    STATE_ON,
    STATE_PROTECTED
} ButtonState;

// Struct para armazenar o estado do botão
typedef struct {
    ButtonState state;
    time_t press_time;
    time_t protected_start_time;
} Button;

Button button = {STATE_OFF, 0, 0}

// Simulação da leitura do botão (abstração)
bool read_button() {
    return digitalRead(BUTTON_PIN) == HIGH;
}

/**
 * Função para realizar a atualização do estado do botão
 * @param button posição em memória do struct criado Button.
 */
void update_button_state(Button *button) {
    bool is_pressed = read_button();
    time_t current_time = milis();

    switch (button->state) {
        case STATE_OFF:
            if (is_pressed) {
                button->state = STATE_ON;
                button->press_time = current_time;
                printf("Estado: LIGADO\n");
            }
            break;

        case STATE_ON:
            if (!is_pressed) {
                unsigned long press_duration = current_time - button->press_time;
                button->state = STATE_PROTECTED;
                button->protected_start_time = current_time;
                Serial.print("Tempo pressionado: ");
                Serial.print(press_duration / 1000.0);
                Serial.println(" segundos");
                Serial.println("Estado: PROTEGIDO");
            }
            break;

        case STATE_PROTECTED:
            if ((current_time - button->protected_start_time) >= INTERVAL) {
                button->state = STATE_OFF;
                printf("Estado: DESLIGADO\n");
            }
            break;
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {
    update_button_state(&button);
    delay(100); // Evita leituras excessivas
}
