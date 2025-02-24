#ifndef MY_INMP441_H
#define MY_INMP441_H

#include "Arduino.h"
#include "driver/i2s.h"

#endif

#define SAMPLE_RATE (44100) // 采样率为44Khz
#define bufferlen 64        // 缓冲区长度

#define INMP441_WS 15       //字时钟WS
#define INMP441_BCK 2       //位时钟BCLK
#define INMP441_SD 13       //串行数据SD

const i2s_port_t I2S_PORT = I2S_NUM_0;      // I2S外设端口号0

class My_INMP441 {
public:
    esp_err_t err;              // 错误码
    void Init();            // I2S初始化
    void Read();            // 读取数据
};