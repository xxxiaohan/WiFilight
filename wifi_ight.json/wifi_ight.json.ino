#include <ESP8266WiFi.h>          // 添加 ESP8266WiFi 库
const char* ssid = "11";   // Wi-Fi 名称
const char* password = "asdfghjkl";   // Wi-Fi 密码
WiFiServer server(80);           // 添加 WiFiServer 对象，用于侦听来自客户端的连接

void setup() {
  Serial.begin(115200);          // 初始化序列通信
  delay(10);

  pinMode(2, OUTPUT);  // 设置 LED 为输出
  digitalWrite(2, 0); // 初始化 LED 灭

  WiFi.begin(ssid, password);  // 连接 Wi-Fi 网络
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  server.begin();               // 开启服务器

  Serial.println(WiFi.localIP());//打印IP地址
}

void loop() {
  WiFiClient client = server.available();   // 等待连接或请求

  if (client) {   // 如果客户端连接成功
    String currentLine = "";  // 等待客户端发送信息
    while (client.connected()) {
      if (client.available()) {   // 如果有信息可读
        char c = client.read();   // 读取客户端发送的信息
        Serial.write(c);          // 输出客户端发送的信息
        if (c == '\n') {          // 如果收到回车键
          if (currentLine.length() == 0) { // 如果收到空行
           client.println("Content-type:text/html");
           client.println();
           client.println("<html>");
           client.println("<head>");
           client.println("<title>LED Control</title>");
           client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
           client.println("<style>");
           client.println("body { font-family: Arial, sans-serif; background-color: #F5F5F5; }");
           client.println(".container { max-width: 500px; margin: auto; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; }");
           client.println("h1 { text-align: center; font-size: 40px; margin: 0; }");
           client.println("p { text-align: center; font-size: 35px; margin-bottom: 30px; }");
           client.println("form { text-align: center; }");
           client.println("button { font-size: 30px; border: none; border-radius: 5px; padding: 20px 40px; cursor: pointer; }");
           client.println(".on { background-color: green ; color: white; }");
           client.println(".off { background-color: orange ; color: white; }");
           client.println("</style>");
           client.println("</head>");
           client.println("<body>");
           client.println("<div class=\"container\">");
           client.println("<h1>LED 控制器</h1>");

          if (digitalRead(LED_BUILTIN) == LOW) {
            client.println("<p>当前灯光状态：开启</p>");
            client.println("<form method=\"get\" action=\"/turnoff\">");
            client.println("<button class=\"off\" type=\"submit\">关闭灯光</button>");
} 
          else {
             client.println("<p>当前灯光状态：关闭</p>");
             client.println("<form method=\"get\" action=\"/turnon\">");
             client.println("<button class=\"on\" type=\"submit\">开启灯光</button>");
}
          client.println("</form>");
          client.println("</div>");
          client.println("</body>");
          client.println("</html>");
            break;
          } else {                // 如果不是空行则继续读取
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /turnon")) {   // 开启 LED
          digitalWrite(LED_BUILTIN, LOW);
        }
        if (currentLine.endsWith("GET /turnoff")) {  // 关闭 LED
          digitalWrite(LED_BUILTIN, HIGH);
        }
      }
    }
    // 断开客户端连接
    client.stop();
    Serial.println("Client disconnected.");
  }
}