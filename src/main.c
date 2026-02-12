#include <ADS1115.h>
ads1115_t ADS1, ADS2;
void app_main() {
    ADS1115_initialize(8, 9);
    ADS1115_add_device(0x48, &ADS1);
    ADS1115_add_device(0x4A, &ADS2);
    while(1){
        uint16_t result1 = ADS1115_read_pin(ADS1, 0), result2 = ADS1115_read_pin(ADS1, 1), result3 = ADS1115_read_pin(ADS1, 2), result4 = ADS1115_read_pin(ADS1, 3);
        uint16_t result5 = ADS1115_read_pin(ADS2, 0), result6 = ADS1115_read_pin(ADS2, 1), result7 = ADS1115_read_pin(ADS2, 2), result8 = ADS1115_read_pin(ADS2, 3);
        printf("%d;%d;%d;%d;%d;%d;%d;%d\n", result1, result2, result3, result4, result5, result6, result7, result8);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }  
}
