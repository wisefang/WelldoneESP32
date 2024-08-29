#ifndef __04WD_INDEX_H__
#define __04WD_INDEX_H__
#include <Arduino.h>
#include "05WdJson.h"
class Wdindex:public WdJson 
{
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
        var Chinese_Name = "%ChineseName%"; // Replace with actual name
        var deviceNo = "%deviceNo%"; // Replace with actual device number
        var ver_buildTime = "%BuildTime%"; // Replace with actual build time
        var firmware_version = "%SoftwareVer%"; // Replace with actual firmware version
        document.getElementById('firmware-version').innerText = firmware_version;
        document.getElementById('device-name').innerText = Chinese_Name + deviceNo;
        document.getElementById('build-time').innerText = ver_buildTime;
    </script>
</body>
</html>
)rawliteral";
const char setup_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PDCAL_SETUP</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            text-align: center;
            background-color: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        h2 {
            color: #333;
        }
        b {
            font-size: 20px; /* Increased font size for labels */
        }
        #pdvalue, #No {
            font-size: 36px; /* Increased font size for divs */
            margin: 20px 0; /* Added margin for spacing */
            padding: 15px; /* Added padding for better appearance */
            border: 1px solid #ddd;
            border-radius: 6px; /* Slightly increased border radius */
            background-color: #fafafa;
        }
        button {
            background-color: #007bff;
            color: #fff;
            border: none;
            padding: 15px 30px; /* Increased button size */
            margin: 10px;
            border-radius: 6px; /* Slightly increased border radius */
            cursor: pointer;
            font-size: 18px; /* Increased font size for buttons */
        }
        button:hover {
            background-color: #0056b3;
        }
        input[type="text"] {
            padding: 15px;
            border: 1px solid #ddd;
            border-radius: 6px; /* Slightly increased border radius */
            font-size: 18px; /* Increased font size for input field */
        }
        a button {
            background-color: #6c757d;
        }
        a button:hover {
            background-color: #5a6268;
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>设备配置</h2>
        <p></p>
        <b>设备编号: </b>
        <div id="No"></div>
        <input type="text" maxlength="6" id="deviceNoInput" onkeyup="this.value=this.value.replace(/[^\d]/g,'')" />
        <button onclick="get()"> 修改编号 </button>
        <a href=\>
            <button>返回</button>
        </a>
    </div>
    <script>
        function set(p1) {
            var payload = p1; // 需要发送的内容
            var xhr = new XMLHttpRequest();
            var url = p1 >= 1000000 ? "/set?value=" + payload : "/set?DeviceNo=" + payload;
            xhr.open("GET", url, true);
            xhr.send();
        }

        function get() {
            var payload = document.getElementById("deviceNoInput").value;
            if (payload.length) { // 判断输入框是否为空
                set(payload);
            }
        }

        function fetchData(url, elementId) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById(elementId).innerHTML = this.responseText;
                }
            };
            xhttp.open("GET", url, true);
            xhttp.send();
        }

        setInterval(function () {
            fetchData("/pdvalue", "pdvalue");
            fetchData("/No", "No");
        }, 500);
    </script>
</body>
</html>)rawliteral";
#endif /* __INDEX_H__ */
