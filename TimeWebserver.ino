#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char *ssid     = "speedy@bde0";
const char *password = "HURUFKECIL";

ESP8266WebServer server(80);
String tanggalWaktu = "";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


void handleRoot() {
  String html = "<html><body><style>h1{text-align: center;}</style><h1 id='angka'>";
  html += tanggalWaktu;
  html += "</h1><script>setInterval(function() { fetch('/data').then(response => response.text()).then(data => { document.getElementById('angka').innerText = data; }); }, 1000);</script></body></html>";
  server.send(200, "text/html", html);
}

void handleData() {
  String data = tanggalWaktu;
  server.send(200, "text/plain", data);
}

void setup() {
  Serial.begin(115200);

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(28800);

  // Mengatur rute URL dan menangani permintaan GET
  server.on("/", handleRoot);
  server.on("/data", handleData);

  // Memulai server web
  server.begin();
  Serial.print("Web server started whith ip ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
  // Mengubah detik setiap 1 detik
  tanggalWaktu = getDateTime();
  delay(1000);
}

String getDateTime(){
  timeClient.update();
  //Get a time structure
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  String formattedTime = timeClient.getFormattedTime(); //18:11:39
  String weekDay = weekDays[timeClient.getDay()]; //Saturday
  int monthDay = ptm->tm_mday; // 21
  int currentMonth = ptm->tm_mon+1; // number of month
  String currentMonthName = months[currentMonth-1]; //March
  int currentYear = ptm->tm_year+1900; //2023
  String date= weekDay + ", " + String(monthDay) + " "+ currentMonthName + " " + String(currentYear);
  String dateTime = date + " (" + formattedTime + ")";
  return dateTime;
}
