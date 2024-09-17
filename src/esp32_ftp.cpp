#include <WiFi.h>
#include "esp32_ftp.h"

static NetworkClient client;
static NetworkClient dclient;

ESP32_FTP::ESP32_FTP(const String& username,const String& password,const String& address,unsigned int timeout) {
    this->username = username;
    this->password = password;
    this->address = address;
    this->timeout = timeout;
    this->port = 21;
}

ESP32_FTP::ESP32_FTP(const String& username,const String& password,const String& address,unsigned int timeout,unsigned int port) {
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
    client.println(F("LIST"));
    response();
}

void ESP32_FTP::esp32Ls() {
    esp32Dir();
}

void ESP32_FTP::esp32Pwd() {
    client.println(F("PWD"));
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

void ESP32_FTP::passiveEnter() {
    client.println(F("PASV"));
    delay(1000);
    String pr = client.readStringUntil('\n');
    Serial.println(pr);

// -----------------------------------------
    // need change here
    int s = pasvResponse.indexOf('(') + 1;
    int e = pasvResponse.indexOf(')');
    String pd = pr.substring(s, e);

    int h1 = pd.indexOf(',');
    int h2 = pd.indexOf(',', h1 + 1);
    int h3 = pd.indexOf(',', h2 + 1);
    int h4 = pd.indexOf(',', h3 + 1);
    int p1 = pd.indexOf(',', h4 + 1);
    int p2 = pd.indexOf(',', p5 + 1);
// -----------------------------------------

    String host =  pd.substring(0, h1) + "." +
                   pd.substring(h1 + 1, h2) + "." +
                   pd.substring(h2 + 1, h3) + "." +
                   pd.substring(h3 + 1, h4);

    int dp = (pd.substring(h4 + 1, p1).toInt() * 256) + pd.substring(p1 + 1, p2).toInt();

    // establish connection

    if(!dclient.connect(host, dp, timeout)) {
        Serial.println("Fail to connect FTP Data server ");
        return;
    }

}

void ESP32_FTP::passiveClose() {
    dclient.close();
    response();
}

void ESP32_FTP::response() {
    unsigned long n = millis();
    // connect error
    while ( !client.available() && millis() < n + timeout ) {
        delay(10);
    }

    if (!client.available()) {
        Serial.println("ftp server out of time ");
    }
    else {
        String response = client.readStringUntil('\n');
        Serial.println(response);
    }
}