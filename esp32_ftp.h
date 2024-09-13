#ifndef __ESP32_FTP_CLIENT__
#define __ESP32_FTP_CLIENT__

const char QUIT[] PROGMEM = "QUIT";

class ESP32_FTP {
public:
    ESP32_FTP();
    ~ESP32_FTP();

    bool isConnected();

    void esp32Connect(const char* username,const char* password,const char* address);
    void esp32Quit();

    void esp32Dir();
    void esp32Ls();
    void esp32Pwd();
    int  esp32Cd(const char* dir);
    int  esp32Get(const char* file);
    int  esp32Put(const char* file);
private:
    // respone code
    int response();

private:
    const  char* username;
    const  char* password;
    const  char* address;
    unsigned int port;
    // is connected flag
    bool  flag;
};


#endif