#ifndef __ESP32_FTP_CLIENT__
#define __ESP32_FTP_CLIENT__

class ESP32_FTP {
public:
    ESP32_FTP(const char* username,const char* password,const char* address,unsigned int timeout);
    ESP32_FTP(const char* username,const char* password,const char* address,unsigned int timeout,unsigned int port);
    ~ESP32_FTP();

    bool isConnected();

    void esp32Connect();
    void esp32Close();

    void esp32Dir();
    void esp32Ls();
    void esp32Pwd();
    int  esp32Cd(const char* dir);
    int  esp32Get(const char* file);
    int  esp32Put(const char* file);
private:
    // respone code
    void response();

private:

    const  char* username;
    const  char* password;
    const  char* address;
    unsigned int port;
    unsigned int timeout;
    // is connected flag
};


#endif