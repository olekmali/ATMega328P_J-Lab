//* Simple BMP280 library - implementaiton file - 2022 *
#include "lib_bmp280.h"
#include "bios_twi_master.h"

/* BMP280 register address */
#define BMP280_ADDR                 0x76

#define BMP280_REGISTER_DIG24B      0x88
#define BMP280_REGISTER_CHIPID      0xD0
#define BMP280_REGISTER_VERSION     0xD1
#define BMP280_REGISTER_SOFTRESET   0xE0
#define BMP280_REGISTER_CAL26B      0xE1 /* R calibration = 0xE1-0xF0 */
#define BMP280_REGISTER_STATUS      0xF3
#define BMP280_REGISTER_CONTROL     0xF4
#define BMP280_REGISTER_CONFIG      0xF5
#define BMP280_REGISTER_DATA8B      0xF7
#define BMP280_REGISTER_PRESSUREDATA 0xF7
#define BMP280_REGISTER_TEMPDATA    0xFA


// calibration data retrieved from the BMP280 sensor
static unsigned int dig_T1;
static int dig_T2, dig_T3;
static unsigned int dig_P1;
static int dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

void bmp_init(void)
{
    // see https://github.com/ControlEverythingCommunity/BMP280/blob/master/C/BMP280.c
    ret_code_t error_code;
    uint8_t data[24] = {0};
    data[0] = BMP280_REGISTER_DIG24B;
    error_code = tw_master_transmit(BMP280_ADDR, data, 1, true);
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }

    error_code = tw_master_receive(BMP280_ADDR, data, 24);
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }

    // temp coefficients
    dig_T1 = data[1] * 256 + data[0];
    dig_T2 = data[3] * 256 + data[2];
    dig_T3 = data[5] * 256 + data[4];

    // pressure coefficients
    dig_P1 = data[7] * 256 + data[6];
    dig_P2 = data[9] * 256 + data[8];
    dig_P3 = data[11]* 256 + data[10];
    dig_P4 = data[13]* 256 + data[12];
    dig_P5 = data[15]* 256 + data[14];
    dig_P6 = data[17]* 256 + data[16];
    dig_P7 = data[19]* 256 + data[18];
    dig_P8 = data[21]* 256 + data[20];
    dig_P9 = data[23]* 256 + data[22];

    // Select control measurement register(0xF4)
    // Normal mode, temp and pressure over sampling rate = 1(0x27)
    uint8_t config[2] = {0};
    config[0] = BMP280_REGISTER_CONTROL;
    config[1] = 0x27; // From data sheet: normal mode, temp and pressure over sampling rate = 1
    error_code = tw_master_transmit(BMP280_ADDR, config, 2, true);
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }

    // Select config register(0xF5)
    // Stand_by time = 1000 ms(0xA0)
    config[0] = BMP280_REGISTER_CONFIG;
    config[1] = 0xA0; // From data sheet: stand_by time = 1000ms
    error_code = tw_master_transmit(BMP280_ADDR, config, 2, true);
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }
}

void bmp_get_data(bmp_data_t* bmp_data) {
    ret_code_t error_code;
    uint8_t data[8];

    data[0] = BMP280_REGISTER_DATA8B;
    error_code = tw_master_transmit(BMP280_ADDR, data, 1, true);
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }
    error_code = tw_master_receive(BMP280_ADDR, data, 8);
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }

    // Convert pressure and temperature data to 19-bits
    long int adc_p = (((long int)data[0] * 65536) + ((long int)data[1] * 256) + (long int)(data[2] & 0xF0)) / 16;
    long int adc_t = (((long int)data[3] * 65536) + ((long int)data[4] * 256) + (long int)(data[5] & 0xF0)) / 16;

    // Temperature offset calculations
    float var1 = (((float)adc_t) / 16384.0F - ((float)dig_T1) / 1024.0F) * ((float)dig_T2);
    float var2 = ((((float)adc_t) / 131072.0F - ((float)dig_T1) / 8192.0F) *(((float)adc_t)/131072.0F - ((float)dig_T1)/8192.0F)) * ((float)dig_T3);
    float t_fine = (long int)(var1 + var2);
    bmp_data->cTemp = (var1 + var2) / 5120.0F;
    bmp_data->fTemp = bmp_data->cTemp * 1.8F + 32.0F;

    // Pressure offset calculations
    var1 = ((float)t_fine / 2.0F) - 64000.0;
    var2 = var1 * var1 * ((float)dig_P6) / 32768.0F;
    var2 = var2 + var1 * ((float)dig_P5) * 2.0F;
    var2 = (var2 / 4.0F) + (((float)dig_P4) * 65536.0F);
    var1 = (((float) dig_P3) * var1 * var1 / 524288.0F + ((float) dig_P2) * var1) / 524288.0F;
    var1 = (1.0F + var1 / 32768.0F) * ((float)dig_P1);
    float p = 1048576.0 - (float)adc_p;
    p = (p - (var2 / 4096.0F)) * 6250.0F / var1;
    var1 = ((float) dig_P9) * p * p / 2147483648.0F;
    var2 = p * ((float) dig_P8) / 32768.0F;
    bmp_data->pressure = (p + (var1 + var2 + ((float)dig_P7)) / 16.0F) / 100.0F;
}
