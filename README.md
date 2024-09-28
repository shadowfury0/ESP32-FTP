# ESP32 FTP

### This is a esp32 ftp client package for arduino
* is only for the ftp client to access ftp server on esp32
* WIFI must be connected

### First declare the ftp instance 
```
ESP32_FTP ftp("username","password","address",timeout);
ESP32_FTP ftp("username","password","address",timeout，port);
```
* must be use esp32Connect() to initialize instance
* end with esp32Close() to release instance

### API Function
include "esp32_ftp.h"
```
esp32Dir();
esp32Ls();
esp32Pwd();
esp32Cd(const char* dir);
esp32Get(const char* file);
esp32Put(const char* file,const char* data);
esp32Put(const char* file,String data);
esp32Append(const char* file,const char* data);
esp32Append(const char* file,String data);
esp32Delete(const char* file);
esp32MDir(const char* dir);
esp32RDir(const char* dir);
esp32RDirT(const char* dir);
```

* Note that in PASV mode, it is necessary to immediately connect to receive data