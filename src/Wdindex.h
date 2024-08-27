#ifndef __F12_INDEX_H__
#define __F12_INDEX_H__
#include <Arduino.h>
class Wdindex {
    public:
      String processor(const String& var) {
        if(var == "PDCAL")
            return F("WdESP32");
        return String();
      }
        
};
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>%web_title%</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            background-color: #f4f4f4;
        }
        .container {
            text-align: center;
            background-color: #ffffff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
        button {
            color: #ffffff;
            background-color: #2d63c8;
            font-size: 19px;
            border: 1px solid #2d63c8;
            padding: 15px 50px;
            cursor: pointer;
            border-radius: 5px;
            margin: 10px;
            transition: background-color 0.3s, color 0.3s;
        }
        button:hover {
            color: #2d63c8;
            background-color: #ffffff;
        }
        a {
            text-decoration: none;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1 id="device-name"></h1>
        <p></p>
        <h2 id="firmware-version"></h2>
        <p></p>
        <h2 id="build-time"></h2>
        <p></p>
        <a href="/setup">
            <button type="button" name="myButton_setup">设备配置</button>
        </a>
        <a href="/update">
            <button type="button" name="myButton_update">更新固件</button>
        </a>
    </div>
    <script>
        // Assuming these variables are available in your JavaScript context
        var Chinese_Name = "设备名称"; // Replace with actual name
        var deviceNo = "%deviceNo%"; // Replace with actual device number
        var ver_buildTime = "2024-08-22"; // Replace with actual build time
         var firmware_version = "1.0.0"; // Replace with actual firmware version
        document.getElementById('firmware-version').innerText = firmware_version;
        document.getElementById('device-name').innerText = Chinese_Name + deviceNo;
        document.getElementById('build-time').innerText = ver_buildTime;
    </script>
</body>
</html>
)rawliteral";

#endif /* __F12_INDEX_H__ */
//以下内容由get_html_str(void)函数生成
//     <script>
//         // Assuming these variables are available in your JavaScript context
//         var Chinese_Name = "设备名称"; // Replace with actual name
//         var deviceNo = "123456"; // Replace with actual device number
//         var ver_buildTime = "2024-08-22"; // Replace with actual build time
//          var firmware_version = "1.0.0"; // Replace with actual firmware version
//         document.getElementById('firmware-version').innerText = firmware_version;
//         document.getElementById('device-name').innerText = Chinese_Name + deviceNo;
//         document.getElementById('build-time').innerText = ver_buildTime;
//     </script>
// </body>
// </html>