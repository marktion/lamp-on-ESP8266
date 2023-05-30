#include <Arduino.h>
#include "Aliyun.h"

WiFiClient espClient;
PubSubClient client(espClient);
using experimental::crypto::SHA256; // 哈希算法

String signHmacSha256(String deviceId, String productKey, String deviceName, String deviceSecret, uint64_t timestamp)
{
    const char *key = deviceSecret.c_str();
    String data = "clientId" + deviceId + "deviceName" + deviceName + "productKey" + productKey + "timestamp" + timestamp;
    Serial.print("sha256:");
    Serial.println(data);
    return SHA256::hmac(data, key, strlen(key), SHA256::NATURAL_LENGTH);
}

void connetMqtt()
{

    Serial.println("start connect mqtt ....");
    client.setKeepAlive(60); // 注意：PubSubClient库的默认keepalive是15s,而官方要求(30~1200s)最小30s，否则会拒绝连接
    client.setServer(serverUrl.c_str(), serverPort);

    String deviceId = String(ESP.getChipId()); // 设备芯片唯一序列号
    uint64_t timestamp = micros64();

    String clientId = deviceId + "|securemode=3,signmethod=hmacsha256,timestamp=" + timestamp + "|";
    String password = signHmacSha256(deviceId, productKey, deviceName, deviceSecret, timestamp);
    String username = deviceName + "&" + productKey;
    Serial.print("clientId:");
    Serial.println(clientId);
    Serial.print("username:");
    Serial.println(username);
    Serial.print("password:");
    Serial.println(password);

    client.connect(clientId.c_str(), username.c_str(), password.c_str());

    while (!client.connected())
    {
        /* code */
        delay(1000);
        client.connect(clientId.c_str(), username.c_str(), password.c_str());
        Serial.println("try connect mqtt...");
    }
    Serial.println("ok, mqtt connected!");
    client.subscribe(subTopic.c_str()); // 订阅主题
    client.setCallback(callback);       // 接收订阅的内容
}

void clent_loop(){
    if (client.connected())
    {
        client.loop(); // 心跳以及消息回调等
    }
}

void postLed(int led){

    String post = "{\"params\":{\"Status\":" + String(!led)+"},\"version\":\"1.0.0\"}";
    client.publish(pubTopic.c_str(), post.c_str());
    Serial.println("post success:");
    Serial.println(post);
}
