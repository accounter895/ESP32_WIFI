#include "my_inmp441.h"
int16_t s_buffer[bufferlen];                 // 采样数据缓冲区

void My_INMP441::Init() {
   // The I2S config as per the example
   const i2s_config_t i2s_config = {
       .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Receive, not transfer
       .sample_rate = SAMPLE_RATE,                         // 采样频率16KHz
       .bits_per_sample = i2s_bits_per_sample_t(I2S_BITS_PER_SAMPLE_16BIT), // could only get it to work with 32bits
       .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, // although the SEL config should be left, it seems to transmit on right
       .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
       .intr_alloc_flags = 0,     // Interrupt level 1
       .dma_buf_count = 8,                           // 量化电平为8位(分辨率)
       .dma_buf_len = bufferlen,                     // samples per buffer
       .use_apll = false
   };
 
   // The pin config as per the setup
   const i2s_pin_config_t pin_config = {
       .bck_io_num = INMP441_BCK,   
       .ws_io_num = INMP441_WS,    
       .data_out_num = -1, // not used (only for speakers)
       .data_in_num = INMP441_SD   
   };
 
   // Configuring the I2S driver and pins.
   // This function must be called before any I2S driver read/write operations.
   err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
   if (err != ESP_OK) {
     Serial.printf("Failed installing driver: %d\n", err);
     while (true);
   }
   err = i2s_set_pin(I2S_PORT, &pin_config);
   if (err != ESP_OK) {
     Serial.printf("Failed setting pin: %d\n", err);
     while (true);
   }
   Serial.println("I2S driver installed.");
}

void My_INMP441::Read() {
    size_t bytes_read = 0;                       // 读取到的字节数
    esp_err_t inm_results = i2s_read(I2S_PORT, &s_buffer, bufferlen, &bytes_read, portMAX_DELAY);
    if (inm_results == ESP_OK)
    {
      int samples_read = bytes_read / 8;
      if (samples_read > 0) {
        float mean = 0;
        for (int i = 0; i < samples_read; ++i) {
          mean += (s_buffer[i]);
        }
        mean /= samples_read;
        Serial.printf("%f\n", mean);
      }
    }
    delay(100);
}
