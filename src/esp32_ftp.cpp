#include <WiFi.h>
#include "esp32_ftp.h"

static NetworkClient client;

ESP32_FTP::ESP32_FTP(const char* username,const char* password,const char* address,unsigned int timeout) {
    this->username = username;
    this->password = password;
    this->address = address;
    this->timeout = timeout;
    this->port = 21;
}

ESP32_FTP::ESP32_FTP(const char* username,const char* password,const char* address,unsigned int timeout,unsigned int port) {
    this->username = username;
    this->password = password;
    this->address = address;
    this->timeout = timeout;
    this->port = port;
}

ESP32_FTP::~ESP32_FTP() {

}

void ESP32_FTP::esp32Connect() {
    if(!client.connect(address, port, timeout)) {
        Serial.println("Fail to connect FTP server ");
        return;
    }

    response();

    client.print(F("USER "));
    client.println(username);
    response();

    client.print(F("PASS "));
    client.println(password);
    delay(1000);
    response();
}

void ESP32_FTP::esp32Close() {
    client.println(F("QUIT"));
    delay(1000);

    response();
    
    client.stop();
    Serial.println("Disconnected from FTP server");
}


void ESP32_FTP::esp32Dir() {
    client.println(F"LIST");
    response();
}

void ESP32_FTP::esp32Ls() {
    esp32Dir();
}

void ESP32_FTP::esp32Pwd() {
    client.println(F"PWD");
    response();
}

int  ESP32_FTP::esp32Cd(const char* dir) {
    client.print(F("CWD"));
    client.println(dir);
    response();
}

int  ESP32_FTP::esp32Get(const char* file) {

}

int  ESP32_FTP::esp32Put(const char* file) {

}


void ESP32_FTP::response() {
    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println(response);
    }
}