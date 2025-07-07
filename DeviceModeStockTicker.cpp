#include "DeviceModeStockTicker.h"
#include "defines.h"
#include "constants.h"

bool DeviceModeStockTicker::Start() {

    // display->ShiftText("Started Stock Ticker. Connecting...");
    // Serial.printf("Started Stock Ticker. Connecting...\n");

    // std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    // client->setInsecure();
    // client->setTimeout(5000);

    // String uri = String(FinnhubConstants::apiBaseUrl) + String(FinnhubConstants::stockUrl) + String("META") + String(FinnhubConstants::tokenSuffix) + String(FinnhubApiKey);
    // Serial.printf("uri: %s\n", uri.c_str());
    // if (httpClient.begin(*client, uri, 443)) {
    //     int httpCode = httpClient.GET();
    //     if (httpCode > 0) {
    //         // HTTP header has been send and Server response header has been handled
    //         Serial.printf("GET... code: %d\n", httpCode);

    //         // file found at server
    //         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
    //             display->ShiftText(httpClient.getString());
    //             String payload = httpClient.getString();
    //             // String payload = https.getString(1024);  // optionally pre-reserve string to avoid reallocations in chunk mode
    //             Serial.println(payload);
    //         }
    //     } else {
    //         display->ShiftText(httpClient.errorToString(httpCode).c_str());
    //         Serial.printf("GET... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
    //     }

    //     httpClient.end();
    // } else {
    //     display->ShiftText("Could not begin");
    //     Serial.printf("Could not begin\n");
    // }
    // return true;
    return false;
}

bool DeviceModeStockTicker::Stop() {
    return true;
}

void DeviceModeStockTicker::OnTick() {

}