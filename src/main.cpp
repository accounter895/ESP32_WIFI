#include <Arduino.h>
#include <Wifi.h>
#include <WiFiAP.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "my_inmp441.h"

// 1. Replace with your network credentials
const char* ssid = "Redmi Note 12 Turbo";   // 若用手机热点，需要调整为2.4G频段
const char* password = "12345678";

// 2. Set your Doubao API key
const char* doubao_api_key = "b1c9428f-5f56-43e3-bd18-19050e53c221";

// 3. Send request to OpenAI API
String inputText = "你好，深度求索！";
String apiUrl = "https://ark.cn-beijing.volces.com/api/v3/chat/completions";
String answer;
My_INMP441 my_inmp441;
// put function declarations here:
String getGPTAnswer(String);

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  my_inmp441.Init();

  // Connect to Wi-Fi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  answer = getGPTAnswer(inputText);
  Serial.println("Answer: " + answer);
  Serial.println("Enter a prompt:");
}

void loop() {
  my_inmp441.Read();
  delay(100);
  /*
  if (Serial.available()) {
    inputText = Serial.readStringUntil('\r');//  \r表示结束符为回车符
    // inputText.trim();
    Serial.println("\n Input:"+inputText);

    answer = getGPTAnswer(inputText);
    Serial.println("Answer: " + answer);
    Serial.println("Enter a prompt:");
  }
  */
}

// put function definitions here:

String getGPTAnswer(String inputText) {
  HTTPClient http;
  http.setTimeout(65534);
  http.begin(apiUrl);
  http.addHeader("Content-Type", "application/json");
  String token_key = String("Bearer ") + doubao_api_key;
  http.addHeader("Authorization", token_key);
  String payload = String("{\"model\":\"ep-20250216202252-wprvx\",")
                  + "\"messages\":[{\"role\":\"system\","
                  + "\"content\":\"你是个农业专家,接下来的农业问题你必须用中文回答,且字数限制在200字以内\"},"
                  + "{\"role\":\"user\","
                  + "\"content\":\"" + inputText + "\"}],"
                  + "\"temperature\": 0.3}";

  int httpResponseCode = http.POST(payload);
  if (httpResponseCode == 200) {
    String response = http.getString();
    http.end();
    Serial.println(response);

    // Parse JSON response
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, response);
    String outputText = jsonDoc["choices"][0]["message"]["content"];
    return outputText;
    // Serial.println(outputText);
  } else {
    http.end();
    Serial.printf("Error %i \n", httpResponseCode);
    return "<error>";
  }
}
