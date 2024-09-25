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
    if(!client.connect(address.c_str(), port, timeout)) {
        Serial.println("Fail to connect FTP server ");
        return;
    }

    response();

    client.print(F("USER "));
    client.println(username);
    response();

    client.print(F("PASS "));
    client.println(password);
    response();
}

void ESP32_FTP::esp32Close() {
    client.println(F("QUIT"));
    delay(1000);

    response();
    
    client.stop();
    Serial.println("FTP server Close");
}

void ESP32_FTP::esp32Dir() {
    // connect error
    if (passiveEnter()) {
        return;
    }
    
    client.println(F("LIST"));
    delay(timeout);
    response();


    //receive and print data
    // do  {
        passiveRev(recv);
    // }
    // while (dclient.connected());

    passiveClose();
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

int ESP32_FTP::passiveEnter() {

    response();

    client.println(F("PASV"));
    
    // waiting
    while ( !client.available()) {
        delay(1000);
    }

    String pr = client.readStringUntil('\n');
    Serial.println(pr);

// -----------------------------------------
    // need change here
    int s = pr.indexOf('(') + 1;
    int e = pr.indexOf(')');
    String pd = pr.substring(s, e);

    int h1 = pd.indexOf(',');
    int h2 = pd.indexOf(',', h1 + 1);
    int h3 = pd.indexOf(',', h2 + 1);
    int h4 = pd.indexOf(',', h3 + 1);
    int p1 = pd.indexOf(',', h4 + 1);
    int p2 = pd.indexOf(',', p1 + 1);
// -----------------------------------------

    // String host =  pd.substring(0, h1) + "." +
    //                pd.substring(h1 + 1, h2) + "." +
    //                pd.substring(h2 + 1, h3) + "." +
    //                pd.substring(h3 + 1, h4);

    int dp = (pd.substring(h4 + 1, p1).toInt() * 256) + pd.substring(p1 + 1, p2).toInt();

    // establish connection

    if(!dclient.connect(address.c_str(), dp, timeout)) {
        Serial.println("Fail to connect FTP Data server ");
        return 1;
    }

    return 0;
}

void ESP32_FTP::passiveRev(String& recv) {
    // waiting
    // while (!dclient.available()) {
    //     delay(100);
    // }
    // String r;
    // r = dclient.readStringUntil('\n');
    // Serial.println(r);
    while (dclient.connected()) {
        while (dclient.available()) {
            String line = dclient.readStringUntil('\n');
            Serial.println(line);
        }
    }
}

void ESP32_FTP::passiveClose() {
    dclient.stop();
    Serial.println("Ftp data Server close");
    response();
}

void ESP32_FTP::response() {
    unsigned long n = millis();
    // wait connection 
    while ( !client.available() && millis() < n + timeout ) {
        delay(100);
    }
  
    // transfer maybe out of time read the last package
    while (client.available()) {
        Serial.println(client.readStringUntil('\n'));
    }
}