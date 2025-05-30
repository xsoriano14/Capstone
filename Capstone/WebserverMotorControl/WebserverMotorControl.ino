#include <WiFi.h>
#include <ESP32Servo.h>
 
Servo myservo, mybrushless;  

const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
String Mode = "None";
String Status = "off";
String Position = "0"; // 0 = off
// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output16 = 16;
const int output17 = 17;
void setup() {

///////////////////////////////////////////////////////

  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
  mybrushless.setPeriodHertz(50);
	myservo.attach(16, 1000, 2500); // attaches the servo on pin 18 to the servo object
	mybrushless.attach(17, 1000, 2500);   
  
  ///////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////

  delay(3000); // delay to plug power in 
  myservo.write(0);
  mybrushless.write(0);
  
  delay(2000); // delay to wait for confirmation beeps
  
  ///////////////////////////////////////////////////////
  // Initialize the output variables as outputs
  pinMode(2, OUTPUT);
  // Set outputs to LOW
  digitalWrite(2, LOW);
  

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();



}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
          
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              Status = "on";
              digitalWrite(2, HIGH);
              digitalWrite(output16, HIGH);
              digitalWrite(output17, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              Status = "off";
              digitalWrite(2, LOW);
              digitalWrite(output16, LOW);
              digitalWrite(output17, LOW);
            }else if (header.indexOf("GET /Mode1/pos0") >= 0) {
              Serial.println("vertical pos0");
             // output26State = "off";
              Mode = "Mode1";
              digitalWrite(2, HIGH);
              myservo.write(0);
              mybrushless.write(0);
            }else if (header.indexOf("GET /Mode1/pos1") >= 0) {
              Serial.println("vertical pos1");
             // output26State = "off";
              Mode = "Mode1";
              digitalWrite(2, LOW);
              myservo.write(10);
              mybrushless.write(20);
            }else if (header.indexOf("GET /Mode1/pos2") >= 0) {
              Serial.println("vertical pos2");
             // output26State = "off";
              Mode = "Mode1";
              digitalWrite(2, HIGH);
              myservo.write(20);
              mybrushless.write(25);
            }else if (header.indexOf("GET /Mode1/pos3") >= 0) {
              Serial.println("vertical pos3");
             // output26State = "off";
              Mode = "Mode1";
              digitalWrite(2, LOW);
              myservo.write(25);
              mybrushless.write(35); 
            }else if (header.indexOf("GET /Mode1/pos4") >= 0) {
              Serial.println("vertical pos3");
             // output26State = "off";
              Mode = "Mode1";
              digitalWrite(2, LOW);
              myservo.write(5);
              mybrushless.write(25);
            }else if (header.indexOf("GET /Mode2") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              Mode = "Mode2";
              digitalWrite(2, HIGH);
              digitalWrite(output16, HIGH);
              digitalWrite(output17, HIGH);
            }else if (header.indexOf("GET /Mode1") >= 0) {
             // output26State = "off";
              Mode = "Mode1";
              digitalWrite(2, LOW);
              myservo.write(0);
              mybrushless.write(0);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"en\">");
            client.println("<head>");
            client.println("    <meta charset=\"UTF-8\">");
            client.println("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("    <title>ESP32 Control</title>");
            client.println("    <style>");
            client.println("        body {");
            client.println("            font-family: Arial, sans-serif;");
            client.println("            background-color: #f0f0f0;");
            client.println("            margin: 0;");
            client.println("            padding: 0;");
            client.println("        }");
            client.println("        header {");
            client.println("            background-color: #333;");
            client.println("            color: white;");
            client.println("            padding: 10px 0;");
            client.println("            text-align: center;");
            client.println("        }");
            client.println("        main {");
            client.println("            padding: 20px;");
            client.println("            text-align: center;");
            client.println("        }");
            client.println("        footer {");
            client.println("            background-color: #333;");
            client.println("            color: white;");
            client.println("            text-align: center;");
            client.println("            padding: 10px 0;");
            client.println("            position: fixed;");
            client.println("            width: 100%;");
            client.println("            bottom: 0;");
            client.println("        }");
            client.println("        .button-container {");
            client.println("            margin-top: 20px;");
            client.println("        }");
            client.println("        .button {");
            client.println("            padding: 10px 20px;");
            client.println("            margin: 5px;");
            client.println("            font-size: 16px;");
            client.println("            cursor: pointer;");
            client.println("            border: none;");
            client.println("            border-radius: 5px;");
            client.println("        }");
            client.println("        .mode1 {");
            client.println("            background-color: #4CAF50;");
            client.println("            color: white;");
            client.println("        }");
            client.println("        .mode2 {");
            client.println("            background-color: #2196F3;");
            client.println("            color: white;");
            client.println("        }");
            client.println("        .on-off {");
            client.println("            background-color: #f44336;");
            client.println("            color: white;");
            client.println("        }");
            client.println("    </style>");
            client.println("</head>");
            client.println("<body>");
            client.println("    <header>");
            client.println("        <h1>Angry Birdie User Interface</h1>");
            client.println("    </header>");
            client.println("    <main>");
            client.println("        <h2>Control Modes</h2>");
            client.println("        <p>Select the mode or toggle the On/Off state:</p>");
            client.println("        <div class=\"button-container\">");
            client.println("        <p id=\"status\">Current Mode: " + Mode +" | Status: " + Status +"</p>");
            if(Status == "off"){
              client.println("       <a href=\"/26/on\"><button class=\"button on-off\">On</button></a>");
            } else {
              client.println("       <a href=\"/26/off\"><button class=\"button on-off\">Off</button></a>");
            }
            if(Mode == "Mode1"){
              client.println("       <a href=\"/Mode2\"><button class=\"button mode2\">Mode 2 - Dynamic Real Time Shot Selection</button></a>");
              client.println("       <a href=\"/Mode1/pos0\"><button class=\"button mode2/ \">pos0</button></a>");
              client.println("       <a href=\"/Mode1/pos1\"><button class=\"button mode2/pos1\">pos1</button></a>");
              client.println("       <a href=\"/Mode1/pos2\"><button class=\"button mode2/pos2\">pos2</button></a>");
              client.println("       <a href=\"/Mode1/pos3\"><button class=\"button mode2/pos3\">pos3</button></a>");
              client.println("       <a href=\"/Mode1/pos4\"><button class=\"button mode2/pos4\">pos3</button></a>");
            } else {
              client.println("       <a href=\"/Mode1\"><button class=\"button mode1\">Mode 1 - Fixed Shot Preset</button>");
            }
            //client.println("            <button class=\"button mode1\" onclick=\"setMode('Mode 1')\">Mode 1 - Fixed Shot Preset</button>");
            //client.println("            <button class=\"button mode2\" onclick=\"setMode('Mode 2')\">Mode 2 - Dynamic Real Time Shot Selection</button>");
            //client.println("            <button class=\"button on-off\" onclick=\"toggleOnOff()\">On/Off</button>");
            client.println("        </div>");
            client.println("    </main>");
            client.println("    <footer>");
            client.println("        <p>&copy; 2025 Hypershuttle Innovations</p>");
            client.println("    </footer>");
            client.println("    <script>");
            client.println("        function setMode(mode) {");
            client.println("            fetch(`/setMode?mode=${mode}`)");
            client.println("                .then(response => response.text())");
            client.println("                .then(data => {");
            client.println("                    document.getElementById('status').innerText = data;");
            client.println("                });");
            client.println("        }");
            client.println("        function toggleOnOff() {");
            client.println("            fetch('/toggleOnOff')");
            client.println("                .then(response => response.text())");
            client.println("                .then(data => {");
            client.println("                    document.getElementById('status').innerText = data;");
            client.println("                });");
            client.println("        }");
            client.println("    </script>");
            client.println("</body>");
            client.println("</html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}