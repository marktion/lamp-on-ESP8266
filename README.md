# lamp-on-ESP8266
## 基于esp8266的智能灯设计
 
**技术栈**：ESP8266、C++（PIO+Arduino）、Android Studio（Java）、阿里云IoT、MQTT、LM393声音传感器、WiFi通信   

**项目描述**：  
- 设计并实现低成本智能灯系统，支持**手机远程控制**、**声音触发夜间模式**及**自定义定时开关**功能，只需一块esp8266即可解决传统灯具交互问题。  
- **系统架构**： 
 ![image](https://github.com/marktion/lamp-on-ESP8266/blob/main/image/系统架构.jpg)
  - **设备端**：基于NodeMCU（ESP8266）开发，集成声音传感器与继电器模块，通过LM393动态检测声音阈值，利用Ticker库实现多线程任务调度，保障夜间模式（声音触发亮灯/6秒延迟关灯）与定时模式的精准控制。  
  - **云端**：部署阿里云IoT平台，设计物模型（开关/模式/定时属性），通过MQTT协议实现设备与云端双向通信，支持实时状态同步与指令下发。  
  - **用户端**：基于Android Studio开发控制APP，集成Retrofit调用阿里云OpenAPI，实现灯具状态可视化、远程操控及模式切换，优化WiFi配置流程至APP内一键完成。  
  ![image](https://github.com/marktion/lamp-on-ESP8266/blob/main/image/APP界面白.jpg)
  ![image](https://github.com/marktion/lamp-on-ESP8266/blob/main/image/APP界面黑.jpg)
- 使用VSCode+PlatformIO搭建嵌入式开发环境
- 实现物联网设备“端-云-APP”全链路开发


**原理图**


  ![image](https://github.com/marktion/lamp-on-ESP8266/blob/main/image/原理图.jpg)


**实物**


  ![image](https://github.com/marktion/lamp-on-ESP8266/blob/main/image/9218027298a594a2b296e83ff6d86ac.jpg)

---  
