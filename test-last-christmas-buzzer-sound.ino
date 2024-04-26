// Arduino note for complete Last Christmas by Wham

const int tempo = 120; // beats per minute

// Notes for Last Christmas
const int notes[] = {
  262, 392, 440, 349, // Frequencies for C4, G4, A4, F4
  262, 392, 440, 349,
  262, 392, 440, 349,
  523, 784, 880, 698, // Frequencies for C5, G5, A5, F5

  262, 392, 440, 349,
  262, 392, 440, 349,
  262, 392, 440, 349,
  523, 784, 880, 698,

  262, 392, 440, 349,
  262, 392, 440, 349,
  262, 392, 440, 349,
  523, 784, 880, 698,

  262, 392, 440, 349,
  262, 392, 440, 349,
  262, 392, 440, 349,
  523, 784, 880, 698,
};

// Durations for Last Christmas
const int durations[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,

  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,

  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,

  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
};

const int pin = 5; // Pin D5

void setup() {
  // Set the tempo
  Serial.begin(9600);
  Serial.println("Ready to play Last Christmas by Wham...");

  // Configure the tone pin
  pinMode(pin, OUTPUT);
}

void loop() {
  // Check if pin D5 is high
  if (digitalRead(pin) == HIGH) {
    // Play each note in the sequence
    for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++) {
      tone(pin, notes[i], durations[i] * 60000 / tempo);
      delay(durations[i] * 60000 / tempo);
    }
    // Wait a short while before playing the sequence again
    delay(1000);
  }
}
