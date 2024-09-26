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
    client.print(F("USER "));
    client.println(username);

    client.print(F("PASS "));
    client.println(password);

    waiting();
    response();
}

void ESP32_FTP::esp32Close() {
    client.println(F("QUIT"));
    delay(1000);

    waiting();
    response();
    
    client.stop();
    Serial.println("FTP server Close");
}

void ESP32_FTP::esp32Dir() {
    esp32Ls();
}

void ESP32_FTP::esp32Ls() {
    waiting();
    response();
    // connect error
    if (passiveEnter()) {
        Serial.println("Max retries reached, aborting connection in esp32Ls function");
        return;
    }
    
    client.println(F("LIST"));
    // Serial.println(client.readStringUntil('\n'));

    passiveRev();
    passiveClose();
}

void ESP32_FTP::esp32Pwd() {
    client.println(F("PWD"));
    waiting();
    response();
}

void  ESP32_FTP::esp32Cd(const char* dir) {
    client.print(F("CWD"));
    client.println(dir);
    waiting();
    response();
}

void  ESP32_FTP::esp32Get(const char* file) {
    waiting();
    response();
    // connect error
    if (passiveEnter()) {
        Serial.println("Max retries reached, aborting connection in esp32Get function");
        return;
    }

    client.print(F("RETR "));
    client.println(file);

    // String response = client.readStringUntil('\n');
    // Serial.println(response);

    // if (!response.startsWith("150")) {
    //     Serial.println("Failed to get file download");
    //     return;
    // }
    passiveRev();
    passiveClose();
}

void  ESP32_FTP::esp32Put(const char* file,const char* data) {
    waiting();
    response();
    if (passiveEnter()) {
        Serial.println("Max retries reached, aborting connection in esp32Put function");
        return;
    }

    client.print(F("STOR "));
    client.println(file);
    
    // String response = client.readStringUntil('\n');
    // Serial.println(response);

    // if (!response.startsWith("150")) {
    //     Serial.println("Failed to start file upload");
    //     return;
    // }   

    dclient.print(data);
    passiveClose();

    Serial.println("File upload completed");
}

void  ESP32_FTP::esp32Put(const char* file,String data) {
    waiting();
    response();
    if (passiveEnter()) {
        Serial.println("Max retries reached, aborting connection in esp32Put function");
        return;
    }

    client.print(F("STOR "));
    client.println(file);

    // String response = client.readStringUntil('\n');
    // Serial.println(response);

    dclient.print(data);

    passiveClose();
    Serial.println("File upload completed");
}

void  ESP32_FTP::esp32Append(const char* file,const char* data) {
    waiting();
    response();
    if (passiveEnter()) {
        Serial.println("Max retries reached, aborting connection in esp32Append function");
        return;
    }

    client.print(F("APPE "));
    client.println(file);

    dclient.print(data);

    passiveClose();
    Serial.println("File Append completed");
}

void ESP32_FTP::esp32Append(const char* file,String data) {
    waiting();
    response();
    if (passiveEnter()) {
        Serial.println("Max retries reached, aborting connection in esp32Append function");
        return;
    }

    client.print(F("APPE "));
    client.println(file);

    dclient.print(data);

    passiveClose();
    Serial.println("File Append completed");
}


int ESP32_FTP::passiveEnter() {

    client.println(F("PASV"));

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

    int dp = (pd.substring(h4 + 1, p1).toInt() * 256) + pd.substring(p1 + 1, p2).toInt();

    int rc = 0;  // retry counter
    // establish connection
    // connect until it work
    while(!dclient.connect(address.c_str(), dp, timeout)) {
        delay(100);

        if (++rc > MAX_RETRIES) {
            return 1;
        }
    }

    return 0;
}

void ESP32_FTP::passiveRev() {
    // waiting
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
}

void ESP32_FTP::waiting() {
    // wait connection 
    while (!client.available()) {
        delay(100);
    }
}

void ESP32_FTP::response() {
    // transfer maybe out of time read the last package
    while (client.available()) {
        Serial.println(client.readStringUntil('\n'));
    }
}