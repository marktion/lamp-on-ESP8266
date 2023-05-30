#include <Arduino.h>
#include <ArduinoJson.h>
#include <Crypto.h>//密码
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


const String productKey = "阿里云物联网云平台密钥";                        // 替换
const String deviceName = "设备名称";                              // 替换
const String deviceSecret = "设备密钥"; // 替换
const String subTopic = "/sys/" + productKey + "/" + deviceName + "/thing/service/property/set";
const String pubTopic = "/sys/" + productKey + "/" + deviceName + "/thing/event/property/post";
const String serverUrl = "地址.mqtt.iothub.aliyuncs.com";
const int serverPort = 1883;

//开始连接云平台
void connetMqtt();

//保持连接状态
void clent_loop();

//哈希256
String signHmacSha256(String deviceId, String productKey, String deviceName, String deviceSecret, uint64_t timestamp);

//消息回调
void callback(char *topic, byte *payload, unsigned int length);

//上报状态
void postLed(int led);