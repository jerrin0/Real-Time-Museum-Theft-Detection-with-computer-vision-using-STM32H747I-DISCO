#define BLYNK_TEMPLATE_ID "******"
#define BLYNK_TEMPLATE_NAME "******"
#define BLYNK_AUTH_TOKEN "******"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "*****";
char pass[] = "*****";

String receivedText = ""; // Store received STM32 text

#define RXD2 16  // ESP32 RX (Connect to STM32 TX)
#define TXD2 17  // ESP32 TX (Connect to STM32 RX)
#define BUZZER_PIN 4 
void setup() {
    Serial.begin(115200);   // USB Serial Monitor
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // UART2 for STM32 communication
    pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output
    digitalWrite(BUZZER_PIN, LOW);
    WiFi.begin(ssid, pass);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    Serial.println("\n✅ WiFi Connected!");
}

void loop() {
    Serial2.flush(); // Clears the buffer (ESP32 uses Serial2 for STM32)
    
    if (Serial2.available()) {  
        receivedText = ""; // Clear old data
        while (Serial2.available()) {
            char c = Serial2.read();
            receivedText += c;
        }
        
        receivedText.trim(); // Remove extra spaces/newlines
        
        if (!receivedText.isEmpty()) {
            Serial.println("Received: " + receivedText);
            Blynk.virtualWrite(V0, receivedText); 

            if (receivedText == "Trespassing or Intruder") {
                Serial.println("⚠️ ALERT: " + receivedText);
                
               Blynk.logEvent("trespassing_or_intruder", "⚠️ Trespassing or Intruder Detected!");
                Blynk.virtualWrite(V1, "Location: Table1_STM32_No1");
                Blynk.virtualWrite(V2, "HURRY UP! to the Location.");
                 digitalWrite(BUZZER_PIN, HIGH);
                delay(500); 
                digitalWrite(BUZZER_PIN, LOW);
                delay(1000);
                digitalWrite(BUZZER_PIN, HIGH);
                delay(500); 
                digitalWrite(BUZZER_PIN, LOW);
                delay(1000);
                digitalWrite(BUZZER_PIN, HIGH);
                delay(500); 
                digitalWrite(BUZZER_PIN, LOW);
                delay(5000);
                Serial.println("🚨 Blynk Event Triggered!");
            }

            Blynk.virtualWrite(V1, " ");
            Blynk.virtualWrite(V2, " ");
        }
    }
    Blynk.run();
}
