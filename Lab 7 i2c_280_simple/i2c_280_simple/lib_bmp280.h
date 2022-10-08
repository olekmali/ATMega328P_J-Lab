//* Simple BMP280 library - header file - 2022 *
#ifndef LIB_BMP280_H_
#define LIB_BMP280_H_

typedef struct
{
    float cTemp;
    float fTemp;
    float pressure;
} bmp_data_t;

void bmp_init(void);
void bmp_get_data(bmp_data_t* bmp_data);

#endif