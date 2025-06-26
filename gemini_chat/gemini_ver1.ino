//Generated cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // Install this library from Library Manager

// WiFi credentials
const char* ssid = "Rajnarain";
const char* password = "narain12";

// Gemini API Key
const char* apiKey = "AIzaSyAuPSFfZub15iyZC8CTf426Tgssw_jLBUo";  // Replace with your actual API key

// Gemini API endpoint (replace with the correct one, model may change!)
const char* geminiEndpoint = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=";

// Global Variables
String receivedData = "";  // To store data received from the serial monitor
bool dataAvailable = false;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Type your message and press Enter to send it to Gemini.");
}

void loop() {
  // Check for serial data
  if (Serial.available() > 0) {
    receivedData = Serial.readStringUntil('\n');  // Read until newline character
    receivedData.trim(); // Remove leading/trailing whitespace
    Serial.print("You said: ");
    Serial.println(receivedData);
    dataAvailable = true;
  }

  // If data is available, send it to Gemini
  if (dataAvailable) {
    String response = sendToGemini(receivedData);
    Serial.print("Gemini says: ");
    Serial.println(response);
    receivedData = "";  // Clear the buffer
    dataAvailable = false;
  }
}


String sendToGemini(String prompt) {
  HTTPClient http;

  // Construct the API URL
  String apiURL = String(geminiEndpoint) + apiKey;

  // Construct the JSON payload
  String jsonPayload = createJsonPayload(prompt);

  // Start HTTP request
  http.begin(apiURL);
  http.addHeader("Content-Type", "application/json");

  // Send the POST request
  int httpCode = http.POST(jsonPayload);

  String response = "";

  if (httpCode > 0) {
    // HTTP request was successful
    Serial.printf("HTTP code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      // Get the response payload
      String payload = http.getString();
      Serial.println("Raw JSON Payload: " + payload);  // Print the raw JSON for debugging

      // Parse the JSON response
      DynamicJsonDocument doc(4096);  // Allocate memory for the JSON document (adjust size as needed)
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        response = "Error parsing JSON response.";
      } else {
        // Extract the text from the response
        if (doc["candidates"][0]["content"]["parts"][0].containsKey("text")) {
          response = doc["candidates"][0]["content"]["parts"][0]["text"].as<String>();
        } else {
          response = "No 'text' field found in the response.";
        }
      }
    } else {
      response = "HTTP request failed. Code: " + String(httpCode);
    }
  } else {
    // HTTP request failed
    response = "HTTP request failed. Error: " + http.errorToString(httpCode);
    Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  // Free resources
  http.end();
  return response;
}


String createJsonPayload(String prompt) {
  // Use ArduinoJson to create the JSON payload
  DynamicJsonDocument doc(2048); // Adjust size as needed

  // Construct the JSON structure based on Gemini API documentation.  This is the *crucial* part.
  // The exact structure might vary slightly depending on the specific Gemini model version.  Check the API docs.
  // This example uses the "parts" format.

  JsonArray contents = doc.createNestedArray("contents");
  JsonObject content = contents.createNestedObject();
  JsonArray parts = content.createNestedArray("parts");
  JsonObject part = parts.createNestedObject();
  part["text"] = prompt;


  // Serialize the JSON document to a string
  String jsonString;
  serializeJson(doc, jsonString);
  Serial.print("JSON Payload: ");
  Serial.println(jsonString);

  return jsonString;
}


