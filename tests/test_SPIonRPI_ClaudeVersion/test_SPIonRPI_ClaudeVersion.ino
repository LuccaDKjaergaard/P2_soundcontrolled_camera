#include <SD.h>
#include <SPI.h>

// SPI0 - SD card
#define SD_SCK  18
#define SD_MOSI 19
#define SD_MISO 16
#define SD_CS   17

// SPI1 - ADC
#define ADC_SCK  10
#define ADC_MISO 12
#define ADC_CS   13

#define PIN_ISR_TIMER 5
#define PIN_ISR_SOUND 6

#define BACKLOGSIZE  40000
#define FRONTLOGSIZE 60000

uint16_t backlog[BACKLOGSIZE];
uint16_t frontlog[FRONTLOGSIZE];
unsigned int backlogCnt = 0;
unsigned int frontlogCnt = 0;

volatile bool soundDetected = false;
volatile bool writeToSD = false;

#define PATH "/adc_out.txt"


// ── Setup ─────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    while (!Serial);

    InitADC();  // init SPI1 first — doesn't touch SPI0
    InitSD();   // init SPI0 + SD filesystem
    InitISR();

    Serial.print("Size of backlog: ");
    Serial.print(sizeof(backlog));
    Serial.println(" bytes.");
    Serial.print("Size of frontlog: ");
    Serial.print(sizeof(frontlog));
    Serial.println(" bytes.");
}


// ── Loop ──────────────────────────────────────────────────────

void loop() {
    if (writeToSD) {
        // Pause interrupts and finish any in-progress ADC transaction
        detachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER));
        detachInterrupt(digitalPinToInterrupt(PIN_ISR_SOUND));
        digitalWrite(ADC_CS, HIGH);  // ensure ADC deselected

        Serial.print("Writing to SD card...");
        WriteToSD();                 // SD library manages SPI0 CS internally
        Serial.println("done.");

        Reset();

        attachInterrupt(digitalPinToInterrupt(PIN_ISR_SOUND), ISR_SOUND, RISING);
        attachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER), ISR_TIMER, RISING);
    }
}


// ── SD ────────────────────────────────────────────────────────

void InitSD() {
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    SPI.setRX(SD_MISO);
    SPI.setTX(SD_MOSI);
    SPI.setSCK(SD_SCK);

    Serial.println("Attempting SD.begin()...");
    bool result = SD.begin(SD_CS, SPI);
    Serial.print("SD.begin() returned: ");
    Serial.println(result ? "true" : "false");

    if (!result) {
        // Try probing the SPI bus manually
        Serial.println("Probing SPI0 bus...");
        SPI.begin();
        SPI.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
        digitalWrite(SD_CS, LOW);
        uint8_t resp = SPI.transfer(0xFF);
        digitalWrite(SD_CS, HIGH);
        SPI.endTransaction();
        Serial.print("SPI probe byte (expect 0xFF at idle): 0x");
        Serial.println(resp, HEX);
    }
}

void WriteToSD() {
    File file = SD.open(PATH, FILE_WRITE);
    if (!file) {
        Serial.println("ERROR: Could not open file for writing.");
        return;
    }

    // Write backlog in circular-buffer order
    for (unsigned int i = backlogCnt; i < BACKLOGSIZE; i++) {
        file.print("B["); file.print(i); file.print("]: ");
        file.println(backlog[i]);
    }
    for (unsigned int i = 0; i < backlogCnt; i++) {
        file.print("B["); file.print(i); file.print("]: ");
        file.println(backlog[i]);
    }

    // Write frontlog
    for (unsigned int i = 0; i < FRONTLOGSIZE; i++) {
        file.print("F["); file.print(i); file.print("]: ");
        file.println(frontlog[i]);
    }

    file.close();
}


// ── ADC ───────────────────────────────────────────────────────

void InitADC() {
    pinMode(ADC_CS, OUTPUT);
    digitalWrite(ADC_CS, HIGH);  // deselect

    SPI1.setRX(ADC_MISO);
    SPI1.setSCK(ADC_SCK);
    SPI1.begin();
    Serial.println("SPI1 (ADC) ready.");
}

uint16_t ReadADC() {
    // beginTransaction/endTransaction are called per-transfer so the
    // bus is properly owned even when called from an ISR context
    SPI1.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));
    digitalWrite(ADC_CS, LOW);

    uint8_t highByte = SPI1.transfer(0x00);
    uint8_t lowByte  = SPI1.transfer(0x00);

    digitalWrite(ADC_CS, HIGH);
    SPI1.endTransaction();

    uint16_t adcValue = ((highByte << 8) | lowByte);
    adcValue = (adcValue >> 1) & 0x0FFF;
    return adcValue;
}


// ── ISR ───────────────────────────────────────────────────────

void InitISR() {
    pinMode(PIN_ISR_SOUND, INPUT);
    pinMode(PIN_ISR_TIMER, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_ISR_SOUND), ISR_SOUND, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER), ISR_TIMER, RISING);
}

void ISR_SOUND() {
    soundDetected = true;
}

void ISR_TIMER() {
    if (soundDetected) {
        if (frontlogCnt < FRONTLOGSIZE) {
            frontlog[frontlogCnt++] = ReadADC();
        } else {
            writeToSD = true;
        }
    } else {
        if (backlogCnt >= BACKLOGSIZE) backlogCnt = 0;
        backlog[backlogCnt++] = ReadADC();
    }
}


// ── Reset ─────────────────────────────────────────────────────

void Reset() {
    frontlogCnt = 0;
    backlogCnt  = 0;
    memset(backlog,  0, sizeof(backlog));
    memset(frontlog, 0, sizeof(frontlog));
    soundDetected = false;
    writeToSD     = false;
}