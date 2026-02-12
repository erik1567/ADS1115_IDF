idf_component_register(SRCS "ADS1115.c"
                       INCLUDE_DIRS "."
                       REQUIRES driver esp_driver_i2c log  # I2C driver API
                       )