#include <WiFi.h>
#include <ESP32Servo.h>
#include "dynamic_mode.h"

Servo myservo, myservoHORIZONTAL, mybrushless;  

const char* ssid     = "ESP32-HyperShuttleInnovations";
const char* password = "123456789x";
bool mode2active;// Set web server port number to 80
bool fast = true;
int mode2iterations = 0;
int birdieCountGlobal = 1;

WiFiServer server(80);

// Variable to store the HTTP request
String header;
String Mode = "None";
String Status = "off";
String FAST_SLOW = "Fast";
String Position = "0"; // 0 = off
String Difficulty = "";
// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output16 = 16;
const int output17 = 17;

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

///////////////////////////////////////////////////////

void position0();
/*
void position1_slow();
void position2_slow();
void position3_slow();
void position4_slow();
void position5_slow();
void position6_slow();
void position7_slow();
void position8_slow();
void position9_slow();
void position10_slow();
void position11_slow();
void position12_slow();*/
void position1_fast();
void position2_fast();
void position3_fast();
void position4_fast();
void position5_fast();
void position6_fast();
void position7_fast();
void position8_fast();
void position9_fast();
void position10_fast();
void position11_fast();
void position12_fast();
void run_dynamic_mode(int rounds);


///////////////////////////////////////////////////////

void setup() {

///////////////////////////////////////////////////////

  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservoHORIZONTAL.setPeriodHertz(50);
  mybrushless.setPeriodHertz(50);
	myservo.attach(16, 1000, 2500); // attaches the servo on pin 18 to the servo object
  myservoHORIZONTAL.attach(18,1000,2500);
	mybrushless.attach(17, 1000, 2500);   
  
  ///////////////////////////////////////////////////////
  
  ///////////////////////////////////////////////////////

  delay(3000); // delay to plug power in 
  myservo.write(0);
  myservoHORIZONTAL.write(120);
  mybrushless.write(0);
  
  delay(2000); // delay to wait for confirmation beeps
  
  ///////////////////////////////////////////////////////
  // Initialize the output variables as outputs
  pinMode(2, OUTPUT);
  // Set outputs to LOW
  digitalWrite(2, LOW);
  

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");


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
             
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, HIGH);
              position0();

              /*myservo.write(0);
              myservoHORIZONTAL.write(120);
              mybrushless.write(0);
              */
            }else if (header.indexOf("GET /Mode2/pos0") >= 0) {
        
              Mode = "Mode2";
              mode2active = false;
              digitalWrite(2, HIGH);
              position0();
                                                        Serial.println("000000000");

              /*
              myservo.write(0);
              myservoHORIZONTAL.write(120);
              mybrushless.write(0);
              */
            }else if (header.indexOf("GET /Mode1/slow") >= 0) {
             
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, HIGH);
              fast = false;
              FAST_SLOW = "Slow";
             
            }else if (header.indexOf("GET /Mode1/fast") >= 0) {
             
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, HIGH);
              fast = true;
              FAST_SLOW = "Fast";
             
            }else if (header.indexOf("GET /Mode1/pos10") >= 0) {

              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position10_fast();
                                    Serial.println("dynamic 10 fast");

              }else{
                position10_slow();
                                    Serial.println("dynamic 10 slow");

              }
                            Serial.println("10101010101010");

              /*
              myservo.write(35);
              mybrushless.write(0);
              myservoHORIZONTAL.write(140);
              */

            }else if (header.indexOf("GET /Mode1/pos11") >= 0) {
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);

              if(fast){
                position11_fast();
                                    Serial.println("dynamic 11 fast");

              }else{
                position11_slow();
                                    Serial.println("dynamic 11 slow");

              }                             Serial.println("11111111111");


            }else if (header.indexOf("GET /Mode1/pos12") >= 0) {
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              Serial.println("12");
              if(fast){
                position12_fast();
                                    Serial.println("dynamic 12 fast");

              }else{
                position12_slow();
                                    Serial.println("dynamic 12 slow");

              }
                             Serial.println("12121212121212");


            }else if (header.indexOf("GET /Mode1/pos1") >= 0) {

              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position1_fast();
                                    Serial.println("dynamic 1 fast");

              }else{
                position1_slow();
                                    Serial.println("dynamic 1 slow");

              }                                                        
              Serial.println("111");

              /*
              myservo.write(10);
              myservoHORIZONTAL.write(120);
              mybrushless.write(25);
              */
            }else if (header.indexOf("GET /Mode1/pos2") >= 0) {

              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, HIGH);
              if(fast){
                position2_fast();
                                    Serial.println("dynamic 2 fast");

              }else{
                position2_slow();
                                    Serial.println("dynamic 2 slow");

              }

                                                        Serial.println("22222222");

              /*
              myservo.write(20);
              myservoHORIZONTAL.write(120);
              mybrushless.write(25);
              */
            }else if (header.indexOf("GET /Mode1/pos3") >= 0) {

              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position3_fast();
                                    Serial.println("dynamic 3 fast");

              }else{
                position3_slow();
                                    Serial.println("dynamic 3 slow");

              }
                                                                      Serial.println("333333333");

              /*
              myservo.write(25);
              myservoHORIZONTAL.write(120);
              mybrushless.write(30); 
              */
            }else if (header.indexOf("GET /Mode1/pos4") >= 0) { //////////////////////////////////////////////////////

              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position4_fast();
                                    Serial.println("dynamic 4 fast");

              }else{
                position4_slow();
                                    Serial.println("dynamic 4 slow");

              }                                                        Serial.println("4444444");

              /*         
              myservo.write(25);
              mybrushless.write(30);
              myservoHORIZONTAL.write(140);
              */
            }else if (header.indexOf("GET /Mode1/pos5") >= 0) {
 
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position5_fast();
                                    Serial.println("dynamic 5 fast");

              }else{
                position5_slow();
                                    Serial.println(" 5 slow");

              }                                                        Serial.println("5555555");

              /*          
              myservo.write(10);
              mybrushless.write(25);
              myservoHORIZONTAL.write(100);
              */

            }else if (header.indexOf("GET /Mode1/pos6") >= 0) {
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position6_fast();
                                    Serial.println("dynamic 6 fast");

              }else{
                position6_slow();
                                    Serial.println("dynamic 6 slow");

              }                                                        Serial.println("6666666");

              /*
              myservo.write(20);
              mybrushless.write(25);
              myservoHORIZONTAL.write(100);
              */

            }else if (header.indexOf("GET /Mode1/pos7") >= 0) {
             
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position7_fast();
                                    Serial.println("dynamic 7 fast");

              }else{
                position7_slow();
                                    Serial.println("dynamic 7 slow");

              }                                          Serial.println("77777777777777777");
        
              /*
              myservo.write(25);
              mybrushless.write(30);
              myservoHORIZONTAL.write(100);
              */
            }else if (header.indexOf("GET /Mode1/pos8") >= 0) {

              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position8_fast();
                                    Serial.println("dynamic 8 fast");

              }else{
                position8_slow();
                                    Serial.println("8 slow");

              }                                                        Serial.println("88888888");

              /*
              myservo.write(10);
              mybrushless.write(25);
              myservoHORIZONTAL.write(140);
              */

            }else if (header.indexOf("GET /Mode1/pos9") >= 0) {
           
              Mode = "Mode1";
              mode2active = false;
              digitalWrite(2, LOW);
              if(fast){
                position9_fast();
                 Serial.println("9 fast");
                                    Serial.println("dynamic 9 fast");

              }else{
                position9_slow();
                 Serial.println("9 slow");
                                     Serial.println("9 slow");

              }                                                        Serial.println("999999999999999999");

              /*
              myservo.write(20);
              mybrushless.write(25);
              myservoHORIZONTAL.write(140);
              */
            }else if (header.indexOf("GET /Mode2running") >= 0) {
                Mode = "Mode2";
                mode2active = true;
              
                run_dynamic_mode(1);
              
              

              digitalWrite(2, HIGH);
              
            }else if(header.indexOf("GET /difficulty/easy") >= 0) {
              Serial.println("Difficulty set to EASY");
              Difficulty = "EASY";
                              Mode = "Mode2";

              set_mode(3);
            }
            else if (header.indexOf("GET /difficulty/medium") >= 0) {
              Serial.println("Difficulty set to MEDIUM");
              Difficulty = "MEDIUM";
                              Mode = "Mode2";

              set_mode(2);
            }
            else if (header.indexOf("GET /difficulty/hard") >= 0) {
              Serial.println("Difficulty set to HARD");
              Difficulty = "HARD";
                              Mode = "Mode2";

              set_mode(1);            
            }else if (header.indexOf("GET /setBirdies?count=") >= 0) {
              int paramStart = header.indexOf("GET /setBirdies?count=") + strlen("GET /setBirdies?count=");
              int paramEnd = header.indexOf(" ", paramStart);  // End of URL
              if (paramEnd > paramStart) {
                String countStr = header.substring(paramStart, paramEnd);
                int birdieCount = countStr.toInt();
                Serial.print("Set birdie count to: ");
                Serial.println(birdieCount);
                birdieCountGlobal = birdieCount;  // Store it or use it
              }
            }else if (header.indexOf("GET /Mode2") >= 0) {
              Mode = "Mode2";
              mode2active = true;
            
              // run_dynamic_mode(1);
            
            

            digitalWrite(2, HIGH);
              
            }else if (header.indexOf("GET /Mode1") >= 0) {
              Mode = "Mode1";
              mode2active = false;
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
            client.println("            max-height: calc(100vh - 140px);"); // space for header and footer
            client.println("            overflow-y: auto;");
            client.println("        }");
            client.println("        footer {");
            client.println("            background-color: #333;");
            client.println("            color: white;");
            client.println("            text-align: center;");
            client.println("            padding: 10px 0;");
            client.println("            width: 100%;");
            client.println("            margin-top: 20px;");
            client.println("        }");
            client.println("        .button-container {");
            client.println("            margin-top: 20px;");
            client.println("        }");

            client.println("        .button {");
            client.println("            width: 90%;");
            client.println("            max-width: 400px;");
            client.println("            box-sizing: border-box;");
            client.println("            padding: 12px;");
            client.println("            margin: 10px auto;");
            client.println("            font-size: 16px;");
            client.println("            cursor: pointer;");
            client.println("            border: 2px solid #000;");
            client.println("            border-radius: 5px;");
            client.println("            display: block;");
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
            client.println("<script>");
           client.println("function sendRepeatedRequest(path, count, delayMs) {");
client.println("  if (!confirm(`Are you sure you want to launch ${count} birdie(s)?`)) {");
client.println("    return; // user cancelled");
client.println("  }");
client.println("  for (let i = 0; i < count; i++) {");
client.println("    setTimeout(() => {");
client.println("      fetch(path).then(response => {");
client.println("        console.log(`Request ${i + 1} to ${path} done`);");
client.println("      }).catch(error => {");
client.println("        console.error('Error:', error);");
client.println("      });");
client.println("    }, i * delayMs);");
client.println("  }");
client.println("}");
            client.println("</script>");
            client.println("</head>");
            client.println("<body>");
            client.println("    <header>");
            client.println("        <h1>Angry Birdie User Interface</h1>");
            client.println("    </header>");
            client.println("    <main>");
            client.println("        <h2>Control Modes</h2>");
            client.println("        <p>Select the mode or toggle the On/Off state:</p>");
            client.println("        <div class=\"button-container\">");
            client.println("        <p id=\"status\">Current Mode: " + Mode +" | Status: " + Status +" | FAST/SLOW: " + FAST_SLOW +" | Difficulty: " + Difficulty +"</p>");
  
            // Add Difficulty Buttons (only enabled if not Mode2)
           /* if (Mode != "Mode2") {
              client.println("        <p><b>Select Difficulty:</b></p>");
              client.println("        <button class=\"button\" onclick=\"location.href='/difficulty/easy'\">Easy</button>");
              client.println("        <button class=\"button\" onclick=\"location.href='/difficulty/medium'\">Medium</button>");
              client.println("        <button class=\"button\" onclick=\"location.href='/difficulty/hard'\">Hard</button>");
            } else {
              client.println("        <p><b>Select Difficulty:</b></p>");
              client.println("        <button class=\"button\" disabled>Easy</button>");
              client.println("        <button class=\"button\" disabled>Medium</button>");
              client.println("        <button class=\"button\" disabled>Hard</button>");
            }  */

            if(Mode == "Mode1"){
              client.println("       <a href=\"/Mode2\"><button class=\"button mode2\">Mode 2 - Dynamic Real Time Shot Selection</button></a>");
              client.println("       <a href=\"/Mode1/pos0\"><button class=\"button mode2/ \">OFF</button></a>");
              if(fast){
                client.println("       <a href=\"/Mode1/slow\"><button class=\"button mode2/ \">SLOW TOGGLE</button></a>");
              }else{
                client.println("       <a href=\"/Mode1/fast\"><button class=\"button mode2/ \">FAST TOGGLE</button></a>");
              }
              client.println("       <a href=\"/Mode1/pos1\"><button class=\"button mode2/pos1\">1</button></a>");
              client.println("       <a href=\"/Mode1/pos2\"><button class=\"button mode2/pos2\">2</button></a>");
              client.println("       <a href=\"/Mode1/pos3\"><button class=\"button mode2/pos3\">3</button></a>");
             
              
              client.println("       <a href=\"/Mode1/pos4\"><button class=\"button on-off\">4</button></a>");
              client.println("       <a href=\"/Mode1/pos5\"><button class=\"button on-off\">5</button></a>"); 
              client.println("       <a href=\"/Mode1/pos6\"><button class=\"button on-off\">6</button></a>");

              client.println("       <a href=\"/Mode1/pos7\"><button class=\"button mode2\">7</button></a>");
              client.println("       <a href=\"/Mode1/pos8\"><button class=\"button mode2\">8</button></a>");
              client.println("       <a href=\"/Mode1/pos9\"><button class=\"button mode2\">9</button></a>");
              client.println("       <a href=\"/Mode1/pos10\"><button class=\"button on-off\">10</button></a>");
              client.println("       <a href=\"/Mode1/pos11\"><button class=\"button on-off\">11</button></a>");
              client.println("       <a href=\"/Mode1/pos12\"><button class=\"button on-off\">12</button></a>");

            } else if(Mode == "Mode2") {
              client.println("       <a href=\"/Mode1\"><button class=\"button mode1\">Mode 1 - Fixed Shot Preset</button>");
               client.println("        <p><b>Select Difficulty:</b></p>");
               /*
              client.println("        <button class=\"button\" onclick=\"location.href='/difficulty/easy'\">Easy</button>");
              client.println("        <button class=\"button\" onclick=\"location.href='/difficulty/medium'\">Medium</button>");
              client.println("        <button class=\"button\" onclick=\"location.href='/difficulty/hard'\">Hard</button>");
              */
              client.println("       <a href=\"/difficulty/easy\"><button class=\"button mode2/ \">Easy</button></a>");
              client.println("       <a href=\"/difficulty/medium\"><button class=\"button mode2/ \">Medium</button></a>");
              client.println("       <a href=\"/difficulty/hard\"><button class=\"button mode2/ \">Hard</button></a>");
              

              client.println("<div style='margin-top: 30px;'>");
              client.println("<form action=\"/setBirdies\" method=\"GET\" onsubmit=\"return validateInput()\">");
              client.println("  <label for=\"birdieCount\">Birdie Count (1–30):</label>");
              client.println("  <input type=\"number\" id=\"birdieCount\" name=\"count\" min=\"1\" max=\"30\" required>");
              //client.println("  <input type=\"submit\" value=\"Go\">");
              client.println("</form>");
              client.println("</div>");

              client.println("<script>");
              client.println("function validateInput() {");
              client.println("  var count = document.getElementById('birdieCount').value;");
              client.println("  if (count < 1 || count > 30) {");
              client.println("    alert('Please enter a number between 1 and 30.');");
              client.println("    return false;");
              client.println("  }");
              client.println("  alert('Birdies are set to ' + count);");
              client.println("  return true;");
              client.println("}");
              client.println("</script>");


              client.println("       <a href=\"/Mode2/pos0\"><button class=\"button mode2/ \">OFFMODE2</button></a>");
              client.println("       <p><button class=\"button\" onclick=\"sendRepeatedRequest('/Mode2running', " + String(birdieCountGlobal) + ", 1000)\">Mode 2 rep</button></p>");

              //client.println("       <p><button class=\"button\" onclick=\"sendRepeatedRequest('/Mode2running', 24, 1000)\">Mode 2 rep</button></p>");
            }else {
              client.println("       <a href=\"/Mode1\"><button class=\"button mode1\">Mode 1 - Fixed Shot Preset</button>");
              client.println("       <a href=\"/Mode2\"><button class=\"button mode2\">Mode 2 - Dynamic Real Time Shot Selection</button></a>");

            }
            
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
            client.println("function sendBirdieCount() {");
            client.println("  const count = document.getElementById('birdieCount').value;");
            client.println("  if (count) {");
            client.println("    fetch(`/setBirdies?count=${count}`)");
            client.println("      .then(response => response.text())");
            client.println("      .then(data => {");
            client.println("        alert(`Set to launch ${count} birdies.`);");
            client.println("      });");
            client.println("  } else {");
            client.println("    alert('Please enter a valid number');");
            client.println("  }");
            client.println("}");
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

void position0()
{
  myservo.write(0);
  myservoHORIZONTAL.write(120);
  mybrushless.write(0);
}

void position1_fast()
{
  myservo.write(10);
  myservoHORIZONTAL.write(150);
  mybrushless.write(0);
}

void position2_fast()
{
  myservo.write(10);
  myservoHORIZONTAL.write(130);
  mybrushless.write(0);
}

void position3_fast()
{
  myservo.write(10);
  myservoHORIZONTAL.write(110);
  mybrushless.write(0);
}
void position4_fast()
{
  myservo.write(10);
  myservoHORIZONTAL.write(90);
  mybrushless.write(0);
}

void position5_fast()
{
  myservo.write(20);
  myservoHORIZONTAL.write(150);
  mybrushless.write(0);
}
void position6_fast()
{
  myservo.write(20);
  myservoHORIZONTAL.write(130);
  mybrushless.write(0);
}

void position7_fast()
{
  myservo.write(20);
  myservoHORIZONTAL.write(110);
  mybrushless.write(0);
}

void position8_fast()
{
  myservo.write(20);
  myservoHORIZONTAL.write(90);
  mybrushless.write(0);
}

void position9_fast()
{
  myservo.write(30);
  myservoHORIZONTAL.write(150);
  mybrushless.write(0);
}

void position10_fast()
{
  myservo.write(30);
  myservoHORIZONTAL.write(130);
  mybrushless.write(0);
}

void position11_fast()
{
  myservo.write(30);
  myservoHORIZONTAL.write(110);
  mybrushless.write(0);
}

void position12_fast()
{
  myservo.write(30);
  myservoHORIZONTAL.write(90);
  mybrushless.write(0);
}

void position1_slow()
{
  myservo.write(10);
  myservoHORIZONTAL.write(150);
  mybrushless.write(0);
}

void position2_slow()
{
  myservo.write(10);
  myservoHORIZONTAL.write(130);
  mybrushless.write(0);
}

void position3_slow()
{
  myservo.write(10);
  myservoHORIZONTAL.write(110);
  mybrushless.write(0);
}
void position4_slow()
{
  myservo.write(10);
  myservoHORIZONTAL.write(90);
  mybrushless.write(0);
}

void position5_slow()
{
  myservo.write(20);
  myservoHORIZONTAL.write(150);
  mybrushless.write(0);
}
void position6_slow()
{
  myservo.write(20);
  myservoHORIZONTAL.write(130);
  mybrushless.write(0);
}

void position7_slow()
{
  myservo.write(20);
  myservoHORIZONTAL.write(110);
  mybrushless.write(0);
}

void position8_slow()
{
  myservo.write(20);
  myservoHORIZONTAL.write(90);
  mybrushless.write(0);
}

void position9_slow()
{
  myservo.write(30);
  myservoHORIZONTAL.write(150);
  mybrushless.write(0);
}

void position10_slow()
{
  myservo.write(30);
  myservoHORIZONTAL.write(130);
  mybrushless.write(0);
}

void position11_slow()
{
  myservo.write(30);
  myservoHORIZONTAL.write(110);
  mybrushless.write(0);
}

void position12_slow()
{
  myservo.write(30);
  myservoHORIZONTAL.write(90);
  mybrushless.write(0);
}

void execute_shot(int position, bool isfast)
{
  switch (position) {
    case 1:
        if(isfast){
          position1_fast();
          Serial.println("dynamic 1 fast");
        }else{
          position1_slow();
                    Serial.println("dynamic 1 slow");

        }
        break;
    case 2:
        if(isfast){
          position2_fast();
                    Serial.println("dynamic 2 fast");

        }else{
          position2_slow();
                              Serial.println("dynamic 2 slow");

        }        
        break;
    case 3:
        if(isfast){
          position3_fast();
                    Serial.println("dynamic 3 fast");

        }else{
          position3_slow();
                              Serial.println("dynamic 3 slow");

        }        
        break;
    case 4:
        if(isfast){
          position4_fast();
                    Serial.println("dynamic 4 fast");

        }else{
          position4_slow();
                              Serial.println("dynamic 4 slow");

        }          
        break;
    case 5:
        if(isfast){
          position5_fast();
                    Serial.println("dynamic 5 fast");

        }else{
          position5_slow();
                              Serial.println("dynamic 5 slow");

        }        
        break;
    case 6:
        if(isfast){
          position6_fast();
                    Serial.println("dynamic 6 fast");

        }else{
          position6_slow();
                              Serial.println("dynamic 6 slow");

        }                
        break;
    case 7:
        if(isfast){
          position7_fast();
                    Serial.println("dynamic 7 fast");

        }else{
          position7_slow();
                              Serial.println("dynamic 7 slow");

        }        
        break;
    case 8:
        if(isfast){
          position8_fast();
                    Serial.println("dynamic 8 fast");

        }else{
          position8_slow();
                              Serial.println("dynamic 8 slow");

        }                
        break;
    case 9:
        if(isfast){
          position9_fast();
                    Serial.println("dynamic 9 fast");

        }else{
          position9_slow();
                              Serial.println("dynamic 9 slow");

        }         
        break;
    case 10:
        if(isfast){
          position10_fast();
                    Serial.println("dynamic 10 fast");

        }else{
          position10_slow();
                              Serial.println("dynamic 10 slow");

        }         
        break;
    case 11:
        if(isfast){
          position11_fast();
                    Serial.println("dynamic 11 fast");

        }else{
          position11_slow();
                              Serial.println("dynamic 11 slow");

        }         
        break;
    case 12:
        if(isfast){
          position12_fast();
                    Serial.println("dynamic 12 fast");

        }else{
          position12_slow();
                              Serial.println("dynamic 12 slow");

        }         
        break;
    default:
        position0();
        break;
  }
}

bool random_true_false() {
    int random_number = rand() % 100;  // Random number between 0 and 99
    if (random_number < 100) { //// hcange back to 60
        return true;  // 60% chance
    } else {
        return false; // 40% chance
    }
}

void run_dynamic_mode(int rounds) {
    for (int i = 0; i < rounds; i++) {
        int next_shot = select_next_shot();
        
        add_to_history(next_shot);
        delay(10);
        execute_shot(next_shot, random_true_false());
        delay(30);
        

        Serial.print("Round ");
        Serial.print(i + 1);
        Serial.print(": Shot to location ");
        Serial.print(next_shot);
        Serial.print(" | History: [");

        for (int j = 0; j < HISTORY_SIZE; j++) {
            Serial.print(shot_history[j]);
            if (j < HISTORY_SIZE - 1) Serial.print(", ");
        }

        Serial.println("]");

    }
}
