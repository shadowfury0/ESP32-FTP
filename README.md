# ESP32 FTP

### This is a esp32 ftp client package for arduino
* is only for the ftp client to access ftp server on esp32

### Attention for the connect input args
* This attribute can't modify in ESP32_FTP Class 
* But it can be modified externally
* Be careful not to make any changes these parameters outside 
* For efficient design
```
    const char* username;
    const char* password;
    const char* address;
```
