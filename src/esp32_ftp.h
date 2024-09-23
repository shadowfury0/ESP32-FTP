#ifndef __ESP32_FTP_CLIENT__
#define __ESP32_FTP_CLIENT__

class ESP32_FTP {
public:
    ESP32_FTP(const String& username,const String& password,const String& address,unsigned int timeout);
    ESP32_FTP(const String& username,const String& password,const String& address,unsigned int timeout,unsigned int port);
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
    // enter passive mode 0 sucess 1 error
    int passiveEnter();
    // put receive string into
    void passiveRev(String& recv);
    void passiveClose();
    // respone code
    void response();

private:
    unsigned int timeout;
    unsigned int port;

    String username;
    String password;
    String address;
    // is connected flag
};


#endif