#include <ADS1115.h>
/*
#define I2C_PORT_NUM_0 -1
#define I2C_MASTER_SCL_IO 9
#define I2C_MASTER_SDA_IO 8
#define ADS_RW_BUFF_SIZE 2

void app_main() {
    esp_log_level_set("I2C_MASTER_DEMO", ESP_LOG_ERROR);
uint8_t rw_buff[ADS_RW_BUFF_SIZE], w_buff[ADS_RW_BUFF_SIZE], apr_buff;
uint16_t ADC_result=0;
vTaskDelay(5);
i2c_master_bus_config_t i2c_mst_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = I2C_PORT_NUM_0,
    .scl_io_num = I2C_MASTER_SCL_IO,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .glitch_ignore_cnt = 7,
};
i2c_master_bus_handle_t bus_handle;

ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

i2c_device_config_t dev_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = 0x48,
    .scl_speed_hz = 10000,
};
i2c_master_dev_handle_t dev_handle;

ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

memset(&apr_buff, 0, 1);
w_buff[0] = 0x85; //pointer to config register
w_buff[1] = 0x84; //config data
apr_buff = 0x01;

uint8_t dev_addr = 0x48<<1 | 0; 

i2c_operation_job_t ADS_write_func[] = {
    {
        .command = I2C_MASTER_CMD_START
        },
    {
        .command = I2C_MASTER_CMD_WRITE,
        .write = {
            .ack_check = true,
            .data = (uint8_t *)&dev_addr,
            .total_bytes = 1,
            },
        },
    {
        .command = I2C_MASTER_CMD_WRITE,
        .write = {
            .ack_check = true,
            .data = (uint8_t *)&apr_buff,
            .total_bytes = 1,
            },
        },
    {
        .command = I2C_MASTER_CMD_WRITE,
        .write = {
            .ack_check = true,
            .data =w_buff,
            .total_bytes = 2,
            },
        },
    {
        .command = I2C_MASTER_CMD_STOP,
    }
};
vTaskDelay(1000/portTICK_PERIOD_MS);
uint8_t config_data[3];
config_data[0] = 0x01;  // Pointer to Config Register
config_data[1] = 0xC4;  // MSB (Start single shot, +/-4.096V range, Single mode)
config_data[2] = 0x84;  // LSB (128SPS, etc.)

// 2. Transmit the buffer
ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, config_data, 3, -1));
vTaskDelay(1000/portTICK_PERIOD_MS);
memset(w_buff, 0, ADS_RW_BUFF_SIZE);
w_buff[0] = 0x01; //pointer to conversion register
ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle,(uint8_t *) &apr_buff, 1, rw_buff, ADS_RW_BUFF_SIZE, -1));
if((rw_buff[0] & 0x80) == 0){
    printf("Configuration successful: 0x%02X 0x%02X\n", rw_buff[0], rw_buff[1]);
    apr_buff = 0x00;
}
while(1){
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle,(uint8_t *) &apr_buff, 1, rw_buff, ADS_RW_BUFF_SIZE, -1));
    printf("Wrote data: 0x%02X\n",apr_buff);
    printf("Received data: MSB 0x%02X LSB 0x%02X\n", rw_buff[0], rw_buff[1]);
    ADC_result = (rw_buff[0] << 8) | rw_buff[1];
    printf("ADC Result: %d\n", ADC_result);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
//ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, rw_buff, ADS_RW_BUFF_SIZE, -1));
//ESP_ERROR_CHECK(i2c_master_receive(dev_handle, rw_buff, ADS_RW_BUFF_SIZE, -1));
}
*/