#ifndef __ESP32_FTP_CLIENT__
#define __ESP32_FTP_CLIENT__

#define MAX_RETRIES 5

class ESP32_FTP {
public:
    ESP32_FTP(const String& username,const String& password,const String& address,unsigned int timeout);
    ESP32_FTP(const String& username,const String& password,const String& address,unsigned int timeout,unsigned int port);
    ~ESP32_FTP();

    bool isConnected();

    void esp32Connect();
    void esp32Close();

    void SystemType();
    void SystemState();

    void esp32Dir();
    void esp32Ls();
    void esp32Pwd();
    void esp32Cd(const char* dir);
    void esp32Get(const char* file);
    void esp32Put(const char* file,const char* data);
    void esp32Put(const char* file,String data);
    void esp32Append(const char* file,const char* data);
    void esp32Append(const char* file,String data);

    void esp32Delete(const char* file);

    void esp32MDir(const char* dir);
    void esp32RDir(const char* dir);
    // force remove directory Tree
    void esp32RDirT(const char* dir);

private:
    // enter passive mode 0 sucess 1 error
    int passiveEnter();
    // put receive string into
    void passiveRev();
    void passiveClose();

    void waiting();
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