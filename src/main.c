#include <ADS1115.h>
ads1115_t ADS1, ADS2, ADS3, ADS4;
void app_main() {
    ADS1115_initialize(8, 9);
    ADS1115_add_device(0x48, &ADS1);
    ADS1115_add_device(0x4A, &ADS2);
    ADS1115_add_device(0x4B, &ADS3);
    ADS1115_add_device(0x4C, &ADS4);
    while(1){
        uint16_t result1 = ADS1115_read_pin(ADS1, 0), result2 = ADS1115_read_pin(ADS1, 1), result3 = ADS1115_read_pin(ADS1, 2), result4 = ADS1115_read_pin(ADS1, 3);
        uint16_t result5 = ADS1115_read_pin(ADS2, 0), result6 = ADS1115_read_pin(ADS2, 1), result7 = ADS1115_read_pin(ADS2, 2), result8 = ADS1115_read_pin(ADS2, 3);
        uint16_t result9 = ADS1115_read_pin(ADS3, 0), result10 = ADS1115_read_pin(ADS3, 1), result11 = ADS1115_read_pin(ADS3, 2), result12 = ADS1115_read_pin(ADS3, 3);
        uint16_t result13 = ADS1115_read_pin(ADS4, 0), result14 = ADS1115_read_pin(ADS4, 1), result15 = ADS1115_read_pin(ADS4, 2), result16 = ADS1115_read_pin(ADS4, 3);
        printf("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\n", result1, result2, result3, result4, result5, result6, result7, result8, result9, result10, result11, result12, result13, result14, result15, result16);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }  
}
