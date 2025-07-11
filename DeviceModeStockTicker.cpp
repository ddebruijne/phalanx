#include "DeviceModeStockTicker.h"
#include "defines.h"
#include "constants.h"
#include <ArduinoJson.h>
#include <ESP_EEPROM.h>

bool DeviceModeStockTicker::Start() {

    EEPROM.get(0, saveData);

    display->ShiftText("Loading...");
    Serial.printf("Started Stock Ticker. Connecting...\n");

    client.setInsecure();
    UpdateSymbol();
    return true;
}

bool DeviceModeStockTicker::Stop() {
    return true;
}

void DeviceModeStockTicker::OnTick() {
    timeSinceLastUpdate += delayBetweenTicks;
    if (timeSinceLastUpdate >= delayBetweenUpdates) {
        // timesince=63900 - delaybetweenupd=63872 - delaybetweentick=100
        Serial.println("timesince=" + String(timeSinceLastUpdate) + " - delaybetweenupd=" + String(delayBetweenUpdates) + " - delaybetweentick=" + String(delayBetweenTicks));
        timeSinceLastUpdate = 0;
        Serial.println("Going to update stock");
        UpdateSymbol();
    }
}

bool DeviceModeStockTicker::UpdateSymbol() {
    String url;
    String symbol = saveData.stockSymbols[0];
    TwelveDataConstants::GetUrlForSymbol(url, symbol);

    if (!client.connect(TwelveDataConstants::apiBaseUrl, 443)) {
        Serial.println("Connection failed");
        return false;
    }

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
            "Host: " + TwelveDataConstants::apiBaseUrl + "\r\n" +
            "Connection: close\r\n\r\n");


    String payload = "";
    String line;
    while (client.connected() || client.available()) {
        line = client.readStringUntil('\n');

        line.trim(); // remove \r and whitespace

        if (line.startsWith("{") && line.endsWith("}")) {
            payload = line; // found the JSON
        }
    }

    // Parse JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.f_str());
        return false;
    }

    // Convert to float and round to 2 decimal places
    float priceValue = doc["price"];
    String price = String(priceValue, 2); // 2 = number of decimal places

    display->ShiftText(symbol + " " + price);
    Serial.println(symbol + " = " + price);
    return true;
}
