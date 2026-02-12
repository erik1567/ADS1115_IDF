/* Apache License 2.0  | Author Contact: jdwifwaf@gmail.com */
#include "ADS1115.h"

static const char *ADS_TAG = "ADS1115";
static i2c_master_bus_handle_t bus_handle = NULL;
static uint8_t rw_buff[ADS_RW_BUFF_SIZE], w_buff[ADS_W_BUFF_SIZE];
static uint16_t reg_cfg = ADS1115_CFG_LS_COMP_MODE_TRAD | // Comparator is traditional
              ADS1115_CFG_LS_COMP_LAT_NON |   // Comparator is non-latching
              ADS1115_CFG_LS_COMP_POL_LOW |   // Alert is active low
              ADS1115_CFG_LS_COMP_QUE_DIS |   // Compator is disabled
              ADS1115_CFG_LS_DR_1600SPS |     // No. of samples to take
              ADS1115_CFG_MS_MODE_SS |
              ADS1115_CFG_MS_PGA_FSR_2_048V; 

static esp_err_t ADS1115_read_to_rwbuff(i2c_master_dev_handle_t dev_handle, uint8_t reg_adr);          // Move these to the header file if you need additional r/w capabilities
static esp_err_t ADS1115_write_reg(i2c_master_dev_handle_t dev_handle, uint16_t val, uint8_t reg);    // Move these to the header file if you need additional r/w capabilities
static esp_err_t i2c_handle_write(i2c_master_dev_handle_t dev_handle, uint8_t w_adr, uint8_t w_len, uint8_t *buff) ;
static esp_err_t i2c_handle_read(i2c_master_dev_handle_t dev_handle, uint8_t r_adr, uint8_t r_len, uint8_t *buff);

//inline implementations
extern inline esp_err_t ADS1115_request_single_ended_AIN0(i2c_master_dev_handle_t dev_handle);
extern inline esp_err_t ADS1115_request_single_ended_AIN1(i2c_master_dev_handle_t dev_handle);
extern inline esp_err_t ADS1115_request_single_ended_AIN2(i2c_master_dev_handle_t dev_handle);
extern inline esp_err_t ADS1115_request_single_ended_AIN3(i2c_master_dev_handle_t dev_handle);

extern inline esp_err_t ADS1115_request_diff_AIN0_AIN1(i2c_master_dev_handle_t dev_handle);
extern inline esp_err_t ADS1115_request_diff_AIN0_AIN3(i2c_master_dev_handle_t dev_handle);
extern inline esp_err_t ADS1115_request_diff_AIN1_AIN3(i2c_master_dev_handle_t dev_handle);
extern inline esp_err_t ADS1115_request_diff_AIN2_AIN3(i2c_master_dev_handle_t dev_handle);

static inline esp_err_t ADS1115_set_lo_thresh(i2c_master_dev_handle_t dev_handle, uint16_t value);
static inline esp_err_t ADS1115_set_hi_thresh(i2c_master_dev_handle_t dev_handle, uint16_t value);

esp_err_t ADS1115_initialize(int SDA_GPIO_PIN, int SCL_GPIO_PIN)
{
    if(bus_handle!= NULL){
        ESP_LOGW(ADS_TAG, "ADS1115_initialize called more than once. This may cause memory leaks and other issues. Please call ADS1115_initialize only once per device.");
        return ESP_OK;
    }
    if(SDA_GPIO_PIN < 0 || SCL_GPIO_PIN < 0)
        return ESP_ERR_INVALID_ARG;
    esp_log_level_set(ADS_TAG, ADS1115_DEBUG_LEVEL);
    i2c_master_bus_config_t i2c_mst_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = I2C_PORT_NUM_0,
    .scl_io_num = SCL_GPIO_PIN,
    .sda_io_num = SDA_GPIO_PIN,
    .glitch_ignore_cnt = 7,
    };
    return i2c_new_master_bus(&i2c_mst_config, &bus_handle);
}

esp_err_t ADS1115_add_device(uint8_t dev_addr, i2c_master_dev_handle_t *dev_handle)
{
    if(bus_handle == NULL){
        ESP_LOGE(ADS_TAG, "ADS1115_add_device called before ADS1115_initialization. Please call ADS1115_initialize before adding devices.");
        return ESP_ERR_INVALID_STATE;
    }
    if(dev_handle == NULL){
        ESP_LOGE(ADS_TAG, "dev_handle pointer is NULL. Please provide a valid pointer to store the device handle.");
        return ESP_ERR_INVALID_ARG;
    }
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = 10000,
    };
    return i2c_master_bus_add_device(bus_handle, &dev_cfg, dev_handle);
}

bool ADS1115_get_conversion_state(i2c_master_dev_handle_t   dev_handle)
{
    ADS1115_read_to_rwbuff(dev_handle, ADS1115_REG_CFG);
    return (rw_buff[0] & 0x80) ? true : false;
}

int16_t ADS1115_get_conversion(i2c_master_dev_handle_t dev_handle)
{
    ADS1115_read_to_rwbuff(dev_handle, ADS1115_REG_CONV);
    return (int16_t)BYTES_INT(rw_buff[0],rw_buff[1]);

}

int16_t ADS1115_read_pin(i2c_master_dev_handle_t dev_handle, uint8_t pin)
{
    int16_t result=0;
    if(pin > 3)
        return 0;

    esp_err_t err;
    switch (pin)
    {
    case 0:
        err = ADS1115_request_single_ended_AIN0(dev_handle);
        break;
    case 1:
        err = ADS1115_request_single_ended_AIN1(dev_handle);
        break;
    case 2:
        err = ADS1115_request_single_ended_AIN2(dev_handle);
        break;
    case 3:
        err = ADS1115_request_single_ended_AIN3(dev_handle);
        break;
    default:
        return ESP_ERR_INVALID_ARG;
    }
    if(err != ESP_OK)
        return err;

    while(!ADS1115_get_conversion_state(dev_handle)){
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    result = ADS1115_get_conversion(dev_handle);
    return result;
}
uint16_t ADS1115_get_config(i2c_master_dev_handle_t dev_handle)
{
    ADS1115_read_to_rwbuff(dev_handle, ADS1115_REG_CFG);
    return BYTES_INT(rw_buff[0], rw_buff[1]);
}
esp_err_t ADS1115_set_config(i2c_master_dev_handle_t dev_handle, uint16_t config)
{
    reg_cfg = config;
    return ADS1115_write_reg(dev_handle, config, ADS1115_REG_CFG);
}
esp_err_t ADS1115_request_by_definition(i2c_master_dev_handle_t dev_handle, uint8_t def)
{
    reg_cfg &= ADS1115_CFG_MS_MUX_OMASK;
    reg_cfg |= (def << 8) & 0xFF00;
    reg_cfg |= ADS1115_CFG_MS_OS_ACTIVE & 0xFF00;
    return ADS1115_write_reg(dev_handle, reg_cfg , ADS1115_REG_CFG);
}

esp_err_t ADS1115_set_thresh_by_definition(i2c_master_dev_handle_t dev_handle, uint8_t thresh, uint16_t val)
{
    if(thresh != ADS1115_REG_LO_THRESH || thresh != ADS1115_REG_HI_THRESH)
        return ESP_ERR_INVALID_ARG;

    return ADS1115_write_reg(dev_handle, val, thresh);
}

static esp_err_t ADS1115_read_to_rwbuff(i2c_master_dev_handle_t dev_handle, uint8_t reg_adr)
{
    return i2c_handle_read(dev_handle, reg_adr, 2, rw_buff);
}

static esp_err_t ADS1115_write_reg(i2c_master_dev_handle_t dev_handle, uint16_t val, uint8_t reg)
{
    rw_buff[0] = (uint8_t)(val >> 8) & 0xFF;
    rw_buff[1] = (uint8_t)val & 0xFF;

    return i2c_handle_write(dev_handle, reg, 2, rw_buff);
}

static esp_err_t i2c_handle_write(i2c_master_dev_handle_t dev_handle, uint8_t w_adr, uint8_t w_len, uint8_t *buff)
{
    memset(w_buff, 0, ADS_W_BUFF_SIZE);
    w_buff[0] = w_adr; //pointer to config register(Address Pointer Register)
    memcpy(&w_buff[1], buff, w_len);

    return i2c_master_transmit(dev_handle, w_buff, ADS_W_BUFF_SIZE, pdMS_TO_TICKS(500));
}

static esp_err_t i2c_handle_read(i2c_master_dev_handle_t dev_handle, uint8_t r_adr, uint8_t r_len, uint8_t *buff)
{
    //ADS115 needs the Address Pointer Register to be set before reading, so we write the register address first, then read the data from that register
    uint8_t addr = r_adr;
    return i2c_master_transmit_receive(dev_handle, &addr, 1, buff, r_len, pdMS_TO_TICKS(500));
}