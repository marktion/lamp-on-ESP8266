#include <Arduino.h>
#include <Ticker.h>
#include "Aliyun.h"
#include <WiFiManager.h> //自动配网

Ticker t1; // 多任务
Ticker t2; // 计时多任务
// int ir = 0;//红外标记,高电平触发
int led = 1;      // 灯标记
int voicef = 0;   // 声音标记，低电平触发
int v_switch = 0; // 模式
int count;        // 延时计数每秒

// 计时秒
void tc(int t)
{
  count++;
  if (count > t)
  {
    t2.detach();
    count = 0;
  }
}
// 定时点亮
void timing_t1(int t)
{
  count++;
  Serial.println(count);
  if (count >= t)
  {
    digitalWrite(LED_BUILTIN, 0);
    digitalWrite(D2, 0);
    count = 0;
    t1.detach();
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0'; // 加上结束符，因为不清楚返回的数据是否带有结束符
  String message = String((char *)payload);
  StaticJsonDocument<192> doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: ")); // 反序列化失败
    Serial.println(error.f_str());
    return;
  }

  int params_Status = doc["params"]["Status"]; // 状态
  int params_S = doc["params"]["S"];           // 定时
  int params_switch = doc["params"]["switch"]; // 模式
  if (params_switch != 0)
  {
    v_switch = params_switch;
  }
  else
  {
    v_switch = 0;
  }
  switch (v_switch)
  {
  case 2: /*声控灯*/
    voicef = 1;
    break;
  case 1: // 定时模块
    digitalWrite(LED_BUILTIN, 1);
    digitalWrite(D2, 1);
    t1.attach(1, timing_t1, params_S);
    voicef = 0;
    t2.detach();
    count = 0;
    break;
  case 0:
    digitalWrite(LED_BUILTIN, !params_Status);
    digitalWrite(D2, !params_Status);
    voicef = 0;
    t2.detach();
    count = 0;
    break;
  default:

    break;
  }
  Serial.println(message);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // 设置LED引脚为输出
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  // ir = digitalRead(D2);
  // 建立WiFiManager对象
  WiFiManager wifiManager;
  // 自动连接WiFi。AutoConnectAP是连接ESP8266时的WiFi名称
  wifiManager.autoConnect("AutoConnectAP");
  connetMqtt();
  
}

void loop()
{
  // put your main code here, to run repeatedly:
  clent_loop();
  // 当发生变化则上报云平台
  if (led != digitalRead(LED_BUILTIN))
  {
    led = digitalRead(LED_BUILTIN);
    postLed(led);
  }
  if (voicef == 1)
  {
    if (digitalRead(D1) == 0) // 如果有声
    {
      if (count == 0) // 检查计时是否已启动
      {
        digitalWrite(LED_BUILTIN, 0);
        digitalWrite(D2, 0);
        t2.attach(1, tc, 7); // 启动计时
      }
      else if (6 >= count && count > 0)
      {
        count = 0; // 重新计时
      }
    }
    else if (count > 6) // 没声且超过6秒关灯
    {
      digitalWrite(LED_BUILTIN, 1);
      digitalWrite(D2, 1);
    }
  }
}
