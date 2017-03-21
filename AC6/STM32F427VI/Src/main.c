/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

#include "ad7779.h"
#include "platform_drivers.h"
#include "string.h"

#define ARM_MATH_CM4
#include "core_cm4.h"

#include "arm_common_tables.h"

#define __FPU_PRESENT             1U       /*!< FPU present */

#include <stdlib.h>
#include <math.h>
#include "arm_math.h"
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include "arm_math.h"

#include <time.h>

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef hspi4;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;
DMA_HandleTypeDef hdma_spi3_rx;
DMA_HandleTypeDef hdma_spi3_tx;
DMA_HandleTypeDef hdma_spi4_rx;
DMA_HandleTypeDef hdma_spi4_tx;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

#define FREESMARTEEG_OPENBCI                (1<<0)
#define FREESMARTEEG_PROCESSING             (1<<1)
#define FREESMARTEEG_CRC                    (1<<2)
#define FREESMARTEEG_TEXT                   (1<<3)
#define FREESMARTEEG_SAMPLECOUNT            (1<<4)
#define FREESMARTEEG_BLINK                  (1<<5)

#define FREESMARTEEG_FFT                    (1<<6)
#define FREESMARTEEG_DIF                    (1<<7)

#define FREESMARTEEG_TEST_COUNTER           (1<<8)

#define FREESMARTEEG_TEXT_UART1             (1<<9)
#define FREESMARTEEG_OPENVIBE_TELNET        (1<<10)

#define FREESMARTEEG_ARDUINO_MEGA_DISPLAY   (1<<11)

#define FREESMARTEEG_OPENVIBE_TELNET_PACKET (1<<12)

#define FREESMARTEEG_OPENBCI_TELNET_PACKET  (1<<13)

#define FREESMARTEEG_OPENVIBE_FREEEEG32     (1<<14)

#define FREESMARTEEG_CHANNELS_STATE         (1<<15)

#define FREESMARTEEG_FFT_DISPLAY            (1<<16)

#define FREESMARTEEG_TEST_BLINK             (1<<17)

#define FREESMARTEEG_STABLE_CRC             (1<<18)

//#define FREESMARTEEG_OUT FREESMARTEEG_CRC
//#define FREESMARTEEG_OUT FREESMARTEEG_TEXT
//#define FREESMARTEEG_OUT FREESMARTEEG_PROCESSING
//#define FREESMARTEEG_OUT FREESMARTEEG_OPENBCI
//#define FREESMARTEEG_OUT FREESMARTEEG_SAMPLECOUNT
//#define FREESMARTEEG_OUT FREESMARTEEG_FFT
//#define FREESMARTEEG_OUT FREESMARTEEG_DIF
//#define FREESMARTEEG_OUT FREESMARTEEG_TEST_COUNTER
//#define FREESMARTEEG_OUT (FREESMARTEEG_DIF | FREESMARTEEG_TEXT)
//#define FREESMARTEEG_OUT (FREESMARTEEG_DIF | FREESMARTEEG_OPENBCI)
//#define FREESMARTEEG_OUT FREESMARTEEG_TEXT_UART1
//#define FREESMARTEEG_OUT FREESMARTEEG_OPENVIBE_TELNET
//#define FREESMARTEEG_OUT FREESMARTEEG_ARDUINO_MEGA_DISPLAY
//#define FREESMARTEEG_OUT (FREESMARTEEG_ARDUINO_MEGA_DISPLAY | FREESMARTEEG_OPENBCI)
//#define FREESMARTEEG_OUT FREESMARTEEG_OPENVIBE_TELNET_PACKET
//#define FREESMARTEEG_OUT FREESMARTEEG_OPENBCI_TELNET_PACKET
#define FREESMARTEEG_OUT FREESMARTEEG_OPENVIBE_FREEEEG32
//#define FREESMARTEEG_OUT FREESMARTEEG_CHANNELS_STATE
//#define FREESMARTEEG_OUT FREESMARTEEG_FFT_DISPLAY
//#define FREESMARTEEG_OUT FREESMARTEEG_TEST_BLINK
//#define FREESMARTEEG_OUT FREESMARTEEG_STABLE_CRC


#define FREESMARTEEG_SEND_UART1  (1<<0)
#define FREESMARTEEG_SEND_UART2  (1<<1)

#define FREESMARTEEG_SEND FREESMARTEEG_SEND_UART1
//#define FREESMARTEEG_SEND FREESMARTEEG_SEND_UART2

#define FREESMARTEEG_SHOW_CRC_NO        (1<<0)
#define FREESMARTEEG_SHOW_CRC_PIN       (1<<1)
#define FREESMARTEEG_SHOW_CRC_NOERRORS  (1<<2)
#define FREESMARTEEG_SHOW_CRC_SOMETHING (1<<3)

//#define FREESMARTEEG_SHOW_CRC FREESMARTEEG_SHOW_CRC_PIN
#define FREESMARTEEG_SHOW_CRC (FREESMARTEEG_SHOW_CRC_PIN | FREESMARTEEG_SHOW_CRC_SOMETHING)
//#define FREESMARTEEG_SHOW_CRC (FREESMARTEEG_SHOW_CRC_PIN | FREESMARTEEG_SHOW_CRC_NOERRORS)
//#define FREESMARTEEG_SHOW_CRC FREESMARTEEG_SHOW_CRC_NO

#define SPI_DMA 1
//#define SPI_DMA 0


#define LCD_RAM   *(uint16_t *)((uint32_t)0x60020000)  //disp Data ADDR
#define LCD_REG   *(uint16_t *)((uint32_t)0x60000000)    //disp Reg  ADDR

#define LCD_NOOP            0x00   /* No Operation*/
#define LCD_SWRESET             0x01   /* Software Reset */
#define LCD_RDDPM               0x0A   /* Read Display Power Mode */
#define LCD_RDDMADCTL           0x0B   /* Read Display MADCTL */
#define LCD_RDDCOLMOD           0x0C   /* Read Display Pixel Format */
#define LCD_RDDIM               0x0D   /* Read Display Image Format */
#define LCD_RDDSM               0x0E   /* Read Display Signal Mode */
#define LCD_RDDSDR              0x0F   /* Read Display Self-Diagnostic Result */
#define LCD_SPLIN               0x10   /* Enter Sleep Mode */
#define LCD_SLEEP_OUT           0x11   /* Sleep out register */
#define LCD_PTLON               0x12   /* Partial Mode ON */
#define LCD_NORMAL_MODE_ON      0x13   /* Normal Display Mode ON */
#define LCD_DINVOFF             0x20   /* Display Inversion OFF */
#define LCD_DINVON              0x21   /* Display Inversion ON */
#define LCD_GAMMA               0x26   /* Gamma register */
#define LCD_DISPLAY_OFF         0x28   /* Display off register */
#define LCD_DISPLAY_ON          0x29   /* Display on register */
#define LCD_COLUMN_ADDR         0x2A   /* Column address register */
#define LCD_PAGE_ADDR           0x2B   /* Page address register */
#define LCD_GRAM                0x2C   /* GRAM register */
#define LCD_RGBSET              0x2D   /* Color SET */
#define LCD_RAMRD               0x2E   /* Memory Read */
#define LCD_PLTAR               0x30   /* Partial Area */
#define LCD_VSCRDEF             0x33   /* Vertical Scrolling Definition */
#define LCD_TEOFF               0x34   /* Tearing Effect Line OFF */
#define LCD_TEON                0x35   /* Tearing Effect Line ON */
#define LCD_MAC                 0x36   /* Memory Access Control register*/
#define LCD_VSCRSADD            0x37   /* Vertical Scrolling Start Address */
#define LCD_IDMOFF              0x38   /* Idle Mode OFF */
#define LCD_IDMON               0x39   /* Idle Mode ON */
#define LCD_PIXEL_FORMAT        0x3A   /* Pixel Format register */
#define LCD_WRITE_MEM_CONTINUE  0x3C   /* Write Memory Continue */
#define LCD_READ_MEM_CONTINUE   0x3E   /* Read Memory Continue */
#define LCD_SET_TEAR_SCANLINE   0x44   /* Set Tear Scanline */
#define LCD_GET_SCANLINE        0x45   /* Get Scanline */
#define LCD_READ_DDB_START      0xA1   /* Read DDB start */

/* Level 2 Commands */
#define LCD_CMDACCPRTC          0xB0   /* Command Access Protect  */
#define LCD_FRMCTR              0xB3   /* Frame Memory Access and Interface setting  */
#define LCD_DMFMCTR             0xB4   /* Display Mode and Frame Memory Write Mode
setting */
#define LCD_DEVCODERD           0xBF   /* Device code read */
#define LCD_PANEL_DRV_CTL       0xC0   /* Panel Driving Setting */
#define LCD_NORMAL_TIMING_WR    0xC1   /* Display Timing Setting for Normal Mode  */
#define LCD_PARTIAL_TIMING_WR   0xC2   /* Display Timing Setting for Partial Mode  */
#define LCD_IDLE_TIMING_WR      0xC3   /* Display Timing Setting for Idle Mode  */
#define LCD_FR_INV_CTL          0xC5   /* Frame rate and Inversion Control  */
#define LCD_INTERFACE           0xC6   /* Interface Control */
#define LCD_GAMMAWR            0xC8   /* Gamma Setting */
#define LCD_POWER               0xD0   /* POWER CONTROL */
#define LCD_VCOM               0xD1   /* VCOM Control */
#define LCD_NORMAL_PWR_WR       0xD2   /* Power Setting for Normal Mode  */
#define LCD_PARTIAL_PWR_WR      0xD3   /* Power Setting for Partial Mode  */
#define LCD_IDLE_PWR_WR         0xD4   /* Power Setting for Idle Mode  */
#define LCD_NVMEMWR             0xE0   /* NV Memory Write  */
#define LCD_NVMEMCTRL           0xE1   /* NV Memory Control */
#define LCD_NVMEMRD             0xE2   /* NV Memory Status */
#define LCD_NVMEMPRT            0xE3   /* NV Memory Protection  */
#define LCD_EEPROMWR_ENABLE      0xE8   /* EEPROM Write Enable  */
#define LCD_EEPROMWR_DISABLE    0xE9   /* EEPROM Write Disable  */
#define LCD_EEPROMWR         0xEA   /* EEPROM Word Write */
#define LCD_EEPROMRD            0xEB   /* EEPROM Word Read   */
#define LCD_EEPROM_ADR_SET       0xEC   /* EEPROM Address Set */
#define LCD_EEPROM_ERASE        0xED   /* EEPROM Erase */
#define LCD_EEPROM_ERASE_ALL    0xEE   /* EEPROM Erase All  */

//RTC_TimeTypeDef sTime;

__IO ITStatus UartReady = RESET;

/* defines number of us per second */
#define US_IN_SECOND            ((uint32_t)1000000)

/* Systick Start */
#define TimerCount_Start()  do{\
              SysTick->LOAD  =  0xFFFFFF  ; /* set reload register */\
              SysTick->VAL  =  0  ;     /* Clear Counter */    \
              SysTick->CTRL  =  0x5 ;     /* Enable Counting*/   \
              }while(0)

/* Systick Stop and retrieve CPU Clocks count */
#define TimerCount_Stop(Value)  do {\
                SysTick->CTRL  =0;  /* Disable Counting */         \
                Value = SysTick->VAL;/* Load the SysTick Counter Value */\
                Value = 0xFFFFFF - Value;/* Capture Counts in CPU Cycles*/\
                }while(0)
#define FIR_PROCESS 0
#define FFT_PROCESS 1

#define Float32 0
#define Q15     1
#define Q31     2
#define LPF     0
#define HPF     1

#define FFT_INVERSE_FLAG        ((uint8_t)0)
#define FFT_Normal_OUTPUT_FLAG  ((uint8_t)1)
#define BUFFER_SIZE       240
#define NB_SAMPLES        64
#define SAMPLES           2048      /* 256 real party and 256 imaginary parts */

//extern
uint32_t _aADC1ConvertedValue_s [SAMPLES];
//extern
uint32_t _uhADCxConvertedValue  ;
//extern
uint32_t _nb_cycles             ;

#define FFT_Length_Tab 1024
//#define FFT_Length_Tab 128

//extern int FFT_SIZE_CHOOSE;

float32_t aFFT_Input_f32[FFT_Length_Tab*2];
float32_t aFFT_Output_f32 [FFT_Length_Tab];

float32_t FFT_Input_Q15_f[FFT_Length_Tab*2];
q15_t aFFT_Input_Q15[FFT_Length_Tab*2];
q15_t FFT_Output_Q15[FFT_Length_Tab];

float32_t FFT_Input_Q31_f[FFT_Length_Tab*2];
q31_t aFFT_Input_Q31[FFT_Length_Tab*2];
q31_t FFT_Output_Q31[FFT_Length_Tab];


//#define uint8_ad_number ((1 + 3) * 1)
//#define uint8_ad_adc_number (1)
//#define uint8_ad_adc_number (2)
//#define uint8_ad_adc_number (3)
#define uint8_ad_adc_number (4)
//#define uint8_ad_chan_number (1)
//#define uint8_ad_chan_number (2)
#define uint8_ad_chan_number (8)
//#define uint8_accel_chan_number (1)
//#define uint8_ad_number ((1 + 3) * 8)
#define uint8_ad_number ((1 + 3) * uint8_ad_chan_number)
#define uint8_accel_chan_number (3)
#define average_number (1)
//#define average_number (2)
//#define average_number (4)
//#define average_number (8)
//#define average_number (10)
//GPIO_TypeDef * led_ports[] = {LED_YELLOW_GPIO_Port,LED_RED_GPIO_Port,LED_GREEN_GPIO_Port,LED_BLUE_GPIO_Port};
//uint16_t led_pins[] = {LED_YELLOW_Pin,LED_RED_Pin,LED_GREEN_Pin,LED_BLUE_Pin};

GPIO_TypeDef * led_ports[] = {LED_RED_GPIO_Port,LED_GREEN_GPIO_Port,LED_BLUE_GPIO_Port,LED_YELLOW_GPIO_Port};
uint16_t led_pins[] = {LED_RED_Pin,LED_GREEN_Pin,LED_BLUE_Pin,LED_YELLOW_Pin};
//GPIO_TypeDef * led_ports[] = {LED_RED_GPIO_Port,LED_GREEN_GPIO_Port};
//uint16_t led_pins[] = {LED_RED_Pin,LED_GREEN_Pin};

GPIO_TypeDef * reset_ports[] = {AD_RESET_GPIO_Port,AD2_RESET_GPIO_Port,AD3_RESET_GPIO_Port,AD4_RESET_GPIO_Port};
uint16_t reset_pins[] = {AD_RESET_Pin,AD2_RESET_Pin,AD3_RESET_Pin,AD4_RESET_Pin};

int32_t samples_int32_average[average_number][uint8_ad_adc_number][uint8_ad_chan_number];
uint8_t datas_out[uint8_ad_adc_number][uint8_ad_number];
uint8_t datas[uint8_ad_adc_number][uint8_ad_number];
uint8_t data1[uint8_ad_number];
uint8_t data2[uint8_ad_number];
uint8_t data3[uint8_ad_number];
uint8_t data4[uint8_ad_number];

uint8_t aTxBuffer[uint8_ad_number];
uint8_t aRxBuffer[uint8_ad_number];

uint8_t crc8_01;
uint8_t crc8_23;
uint8_t crc8_45;
uint8_t crc8_67;

uint8_t crc8fromad[4];
uint8_t crc8[4];
uint8_t crcBuffer[4][1+3+3+1];
//uint8_t crcBuffer[8/2][3+3];
uint8_t crc8ok[4];
uint8_t channelnumberok[8];

long int c000 = 0;
long int c_0 = 0;
long int c_1 = 0;
long int c_2 = 0;
long int c_3 = 0;
long int c_00 = 0;
long int c_01 = 0;
long int c_02 = 0;
long int c_03 = 0;
long int c00 = 0;
long int c01 = 0;
long int c0 = 0;
long int c1 = 0;
long int c2 = 0;
long int c3 = 0;
long int c4 = 0;
long int c5 = 0;
long int c6 = 0;
long int c7 = 0;

char str_in[] = "%1.6f";
char str_out[] = "0000000000000000000";

uint8_t uicount=0;

uint32_t last_index_fill_adc_buffer=0;

int32_t last_stddev_count=0;

uint8_t ui8SampleNumber=0;
uint32_t ui32SampleNumber=0;

//const uint8_t uint8_data_number_print = 200;
//const uint8_t uint8_data_number_print2 = 200;
#define uint8_data_number_print  200
#define uint8_data_number_print2  200
uint8_t dataBuffer_print2[uint8_data_number_print2];
uint8_t dataBuffer_print[uint8_data_number_print];

time_t time1;
#define times_max 20
time_t times[times_max];
long stddevmaxs[times_max];
int times_count=0;
float frac_secs1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_SPI4_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void print_hex(int v, int num_places);
void print2_hex(int v, int num_places);
void print_binary(int v, int num_places );
void print_symbol(uint8_t v);
void print_text(const char * t);
void print_line();
void print2_line();
void print_text_line(const char * t);

//void FFT_PROCESSING_F32Process(uint32_t FFT_Length);
//void FFT_PROCESSING_Q15Process(uint32_t FFT_Length);
//void FFT_PROCESSING_Q31Process(uint32_t FFT_Length);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

//void rtc_read_frac_s(time_t * t, float * secfrac)
//{
//    RTC_HandleTypeDef RtcHandle;
//    RTC_DateTypeDef dateStruct;
//    RTC_TimeTypeDef timeStruct;
//    struct tm timeinfo;
//
//    RtcHandle.Instance = RTC;
//
//    HAL_RTC_GetTime(&RtcHandle, &timeStruct, FORMAT_BIN);
//    HAL_RTC_GetDate(&RtcHandle, &dateStruct, FORMAT_BIN);
//
//    // Setup a tm structure based on the RTC
//    timeinfo.tm_wday = dateStruct.WeekDay;
//    timeinfo.tm_mon  = dateStruct.Month - 1;
//    timeinfo.tm_mday = dateStruct.Date;
//    timeinfo.tm_year = dateStruct.Year + 100;
//    timeinfo.tm_hour = timeStruct.Hours;
//    timeinfo.tm_min  = timeStruct.Minutes;
//    timeinfo.tm_sec  = timeStruct.Seconds;
//
//    // Convert to timestamp
//    *t = mktime(&timeinfo);
//
//// Standard granularity is ~4ms (1/256sec)
//// Import mbed-dev & modify rtc_api.c's rtc_init() with these values for ~1ms (1/1024sec) granularity:
////    RtcHandle.Init.AsynchPrediv   = 0x1f;
////    RtcHandle.Init.SynchPrediv    = (rtc_freq / 0x20) - 1;
//
//    *secfrac = ((float)(timeStruct.SecondFraction-timeStruct.SubSeconds)) / ((float)(timeStruct.SecondFraction+1));
//    return;
//}

void print_hex(int v, int num_places)
{
  const uint32_t uint8_data_number = (num_places % 4 == 0 ? num_places / 4 : num_places / 4 + 1);
//  uint8_t dataBuffer[uint8_data_number];
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
  int mask=0, n, num_nibbles, digit, nibbles_max;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask; // truncate v to specified number of places

    num_nibbles = num_places / 4;
    if ((num_places % 4) != 0)
    {
        ++num_nibbles;
    }
    nibbles_max=num_nibbles;

    do
    {
        digit = ((v >> (num_nibbles-1) * 4)) & 0x0f;
        if(digit == 0)  dataBuffer_print[ nibbles_max-num_nibbles ] = '0';
        if(digit == 1)  dataBuffer_print[ nibbles_max-num_nibbles ] = '1';
        if(digit == 2)  dataBuffer_print[ nibbles_max-num_nibbles ] = '2';
        if(digit == 3)  dataBuffer_print[ nibbles_max-num_nibbles ] = '3';
        if(digit == 4)  dataBuffer_print[ nibbles_max-num_nibbles ] = '4';
        if(digit == 5)  dataBuffer_print[ nibbles_max-num_nibbles ] = '5';
        if(digit == 6)  dataBuffer_print[ nibbles_max-num_nibbles ] = '6';
        if(digit == 7)  dataBuffer_print[ nibbles_max-num_nibbles ] = '7';
        if(digit == 8)  dataBuffer_print[ nibbles_max-num_nibbles ] = '8';
        if(digit == 9)  dataBuffer_print[ nibbles_max-num_nibbles ] = '9';
        if(digit == 10) dataBuffer_print[ nibbles_max-num_nibbles ] = 'A';
        if(digit == 11) dataBuffer_print[ nibbles_max-num_nibbles ] = 'B';
        if(digit == 12) dataBuffer_print[ nibbles_max-num_nibbles ] = 'C';
        if(digit == 13) dataBuffer_print[ nibbles_max-num_nibbles ] = 'D';
        if(digit == 14) dataBuffer_print[ nibbles_max-num_nibbles ] = 'E';
        if(digit == 15) dataBuffer_print[ nibbles_max-num_nibbles ] = 'F';
    } while(--num_nibbles);
//    HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//    HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);

    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer_print, uint8_data_number) != HAL_OK)
    {
      Error_Handler();
    }
//    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
    {
    }
}

void print2_hex(int v, int num_places)
{
  const uint32_t uint8_data_number = (num_places % 4 == 0 ? num_places / 4 : num_places / 4 + 1);
//  uint8_t dataBuffer[uint8_data_number];
  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
  {
  }
  int mask=0, n, num_nibbles, digit, nibbles_max;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask; // truncate v to specified number of places

    num_nibbles = num_places / 4;
    if ((num_places % 4) != 0)
    {
        ++num_nibbles;
    }
    nibbles_max=num_nibbles;

    do
    {
        digit = ((v >> (num_nibbles-1) * 4)) & 0x0f;
        if(digit == 0)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '0';
        if(digit == 1)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '1';
        if(digit == 2)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '2';
        if(digit == 3)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '3';
        if(digit == 4)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '4';
        if(digit == 5)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '5';
        if(digit == 6)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '6';
        if(digit == 7)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '7';
        if(digit == 8)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '8';
        if(digit == 9)  dataBuffer_print2[ nibbles_max-num_nibbles ] = '9';
        if(digit == 10) dataBuffer_print2[ nibbles_max-num_nibbles ] = 'A';
        if(digit == 11) dataBuffer_print2[ nibbles_max-num_nibbles ] = 'B';
        if(digit == 12) dataBuffer_print2[ nibbles_max-num_nibbles ] = 'C';
        if(digit == 13) dataBuffer_print2[ nibbles_max-num_nibbles ] = 'D';
        if(digit == 14) dataBuffer_print2[ nibbles_max-num_nibbles ] = 'E';
        if(digit == 15) dataBuffer_print2[ nibbles_max-num_nibbles ] = 'F';
    } while(--num_nibbles);
//    HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//    HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);

    if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer_print2, uint8_data_number) != HAL_OK)
    {
      Error_Handler();
    }
//    while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
    {
    }
}

void print_binary(int v, int num_places  )
{
  const uint32_t uint8_data_number = num_places;
//  uint8_t dataBuffer[uint8_data_number];
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
    int mask=0, n;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask;  // truncate v to specified number of places

    while(num_places)
    {

        if (v & (0x0001 << (num_places-1)))
        {
//             Serial.print("1");
             dataBuffer_print[ uint8_data_number-num_places ] = '1';
        }
        else
        {
//             Serial.print("0");
          dataBuffer_print[ uint8_data_number-num_places ] = '0';
         }

        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
//            Serial.print("_");
        }
    }
//    HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//    HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer_print, uint8_data_number) != HAL_OK)
    {
      Error_Handler();
    }
//    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
    {
    }
}
void print2_binary(int v, int num_places  )
{
  const uint32_t uint8_data_number = num_places;
//  uint8_t dataBuffer[uint8_data_number];
  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
  {
  }
    int mask=0, n;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask;  // truncate v to specified number of places

    while(num_places)
    {

        if (v & (0x0001 << (num_places-1)))
        {
//             Serial.print("1");
             dataBuffer_print2[ uint8_data_number-num_places ] = '1';
        }
        else
        {
//             Serial.print("0");
          dataBuffer_print2[ uint8_data_number-num_places ] = '0';
         }

        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
//            Serial.print("_");
        }
    }
//    HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//    HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
    if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer_print2, uint8_data_number) != HAL_OK)
    {
      Error_Handler();
    }
//    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
    {
    }
}
void print_symbol(uint8_t v)
{
  const uint8_t uint8_data_number = 1;
//  uint8_t dataBuffer[uint8_data_number];
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
  dataBuffer_print[ 0 ] = v;
//  HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer, uint8_data_number);
//  HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
//  HAL_UART_Transmit_IT(&huart1, (uint8_t*)dataBuffer, uint8_data_number);
  if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer_print, uint8_data_number) != HAL_OK)
  {
    Error_Handler();
  }
//  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
}
void print2_symbol(uint8_t v)
{
  const uint8_t uint8_data_number = 1;
//  uint8_t dataBuffer[uint8_data_number];
  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
  {
  }
  dataBuffer_print2[ 0 ] = v;
//  HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer, uint8_data_number);
//  HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
//  HAL_UART_Transmit_IT(&huart1, (uint8_t*)dataBuffer, uint8_data_number);
  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer_print2, uint8_data_number) != HAL_OK)
  {
    Error_Handler();
  }
//  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
}
void print_text(const char * t)
{
  int text_length = strlen(t);
  const uint32_t uint8_data_number = text_length;
  uint8_t dataBuffer[uint8_data_number];
  for (int n=0; n<text_length; n++)
  {
    dataBuffer[n] = (uint8_t)(t[n]);
  }
//  HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//  HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
//  HAL_UART_Transmit_IT(&huart1, (uint8_t*)dataBuffer, uint8_data_number);
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
  if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
}
void print2_text(const char * t)
{
  int text_length = strlen(t);
  const uint32_t uint8_data_number = text_length;
  uint8_t dataBuffer[uint8_data_number];
  for (int n=0; n<text_length; n++)
  {
    dataBuffer[n] = (uint8_t)(t[n]);
  }
//  HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//  HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
//  HAL_UART_Transmit_IT(&huart1, (uint8_t*)dataBuffer, uint8_data_number);
  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
  {
  }
  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
  {
  }
}
void print_line()
{
  const uint8_t uint8_data_number = 2;
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
  dataBuffer_print[ 0 ] = '\r';
  dataBuffer_print[ 1 ] = '\n';
  //    HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//      HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
  if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer_print, uint8_data_number) != HAL_OK)
  {
    Error_Handler();
  }
//  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
}
void print2_line()
{
  const uint8_t uint8_data_number = 2;
  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
  {
  }
  dataBuffer_print2[ 0 ] = '\r';
  dataBuffer_print2[ 1 ] = '\n';
  //    HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//      HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer_print2, uint8_data_number) != HAL_OK)
  {
    Error_Handler();
  }
//  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
  {
  }
}
void print_text_line(const char * t)
{
  print_text(t);
  print_line();
}

uint8_t aui8_6b[6] = {0,1,2,3,4,5};
uint8_t aui8_7b[7] = {0,1,2,3,4,5,6};
uint8_t aui8_8b[8] = {0,1,2,3,4,5,6,7};
uint8_t aui8_8b_[8] = {1,2,3,4,5,6,7,8};

uint8_t crc8da_(uint8_t *data, uint8_t data_size)
{
//  Serial.println("");
//  for(int16_t byte_in_data = data_size - 1; byte_in_data >= 0; byte_in_data--)
//  {
//    print_binary(data[byte_in_data],8);    Serial.print(";");
//  }
//  Serial.println("");
  for(int16_t bit_in_data = data_size * 8 - 1; bit_in_data >= 7; bit_in_data--)
//  for(int16_t bit_in_data = data_size * 8 - 1; bit_in_data >= 8; bit_in_data--)
  {
//    uint16_t bit_in_leftmost_byte_in_data = bit_in_data / 8;
//    uint16_t leftmost_byte_in_data = bit_in_data / 8;
    if(data[bit_in_data / 8] & (1 << (bit_in_data % 8)))
    {
//      if(bit_in_data == 8)
      if(bit_in_data == 7)
      {
        if(data[0]<0x83)
        {
          return data[0];
        }
      }
      data[bit_in_data / 8] = data[bit_in_data / 8] ^ (1 << (bit_in_data % 8));
      data[(bit_in_data - 6) / 8] = data[(bit_in_data - 6) / 8] ^ (1 << ((bit_in_data - 6) % 8));
      data[(bit_in_data - 7) / 8] = data[(bit_in_data - 7) / 8] ^ (1 << ((bit_in_data - 7) % 8));
//      data[(bit_in_data - 8) / 8] = data[(bit_in_data - 8) / 8] ^ (1 << ((bit_in_data - 8) % 8));
    }
//    for(int16_t byte_in_data = data_size - 1; byte_in_data >= 0; byte_in_data--)
//    {
//      print_binary(data[byte_in_data],8);    Serial.print(";");
//    }
//    Serial.println("");
  }
  return data[0];
}

uint8_t crc8da(uint8_t *data, uint8_t data_size)
{
//  Serial.println("");
//  for(int16_t byte_in_data = 0; byte_in_data < data_size; byte_in_data++)
//  {
//    print_binary(data[byte_in_data],8);    Serial.print(";");
//  }
//  Serial.println("");
//  data[0] = data[0] ^ 0xff;
//  for(int16_t byte_in_data = 0; byte_in_data < data_size; byte_in_data++)
//  {
//    print_binary(data[byte_in_data],8);    Serial.print(";");
//  }
//  Serial.println("");
  for(int16_t bit_in_data = (data_size * 8 - 1); bit_in_data >= (9 - 1); bit_in_data--)
  {
//    uint16_t bit_in_leftmost_byte_in_data = bit_in_data / 8;
//    uint16_t leftmost_byte_in_data = bit_in_data / 8;
    if(data[((data_size * 8 - 1) - bit_in_data) / 8] & (1 << (bit_in_data % 8)))
    {
      if(bit_in_data == (9 - 1))
      {
        if(data[(data_size - 1)] < 0x07)
        {
          return data[(data_size - 1)];
        }
      }
      data[(((data_size * 8 - 1) - (bit_in_data - 0))) / 8] =
       data[(((data_size * 8 - 1) - (bit_in_data - 0))) / 8] ^ (1 << ((bit_in_data - 0) % 8));
      data[(((data_size * 8 - 1) - (bit_in_data - 6))) / 8] =
       data[(((data_size * 8 - 1) - (bit_in_data - 6))) / 8] ^ (1 << ((bit_in_data - 6) % 8));
      data[(((data_size * 8 - 1) - (bit_in_data - 7))) / 8] =
       data[(((data_size * 8 - 1) - (bit_in_data - 7))) / 8] ^ (1 << ((bit_in_data - 7) % 8));
      data[(((data_size * 8 - 1) - (bit_in_data - 8))) / 8] =
       data[(((data_size * 8 - 1) - (bit_in_data - 8))) / 8] ^ (1 << ((bit_in_data - 8) % 8));
    }
//    for(int16_t byte_in_data = 0; byte_in_data < data_size; byte_in_data++)
//    {
//      print_binary(data[byte_in_data],8);    Serial.print(";");
//    }
//    Serial.println("");
  }
  return data[(data_size - 1)];
}

uint8_t crc8da_7(const uint8_t *addr)
{
/*  aui8_7b[6]=((addr[4] & 0xf0) >> 4) | ((addr[0] & 0xf0));
  aui8_7b[5]=addr[7];
  aui8_7b[4]=addr[6];
  aui8_7b[3]=addr[5];
  aui8_7b[2]=addr[3];
  aui8_7b[1]=addr[2];
  aui8_7b[0]=addr[1];
  return crc8da(aui8_7b,7);*/

/*  aui8_7b[6]=addr[7];
  aui8_7b[5]=addr[6];
  aui8_7b[4]=addr[5];
  aui8_7b[3]=addr[3];
  aui8_7b[2]=addr[2];
  aui8_7b[1]=addr[1];
  aui8_7b[0]=0xff;
  return crc8da(aui8_7b,7);*/

/*  aui8_7b[6]=addr[7];
  aui8_7b[5]=addr[6];
  aui8_7b[4]=addr[5];
  aui8_7b[3]=((addr[4] & 0xf0) >> 4) | ((addr[3] & 0x0f) << 4);
  aui8_7b[2]=((addr[3] & 0xf0) >> 4) | ((addr[2] & 0x0f) << 4);
  aui8_7b[1]=((addr[2] & 0xf0) >> 4) | ((addr[1] & 0x0f) << 4);
  aui8_7b[0]=((addr[1] & 0xf0) >> 4) | ((addr[0] & 0xf0));
  return crc8da(aui8_7b,7);*/

  aui8_8b_[0]=((addr[0] & 0xf0)) | ((addr[1] & 0xf0) >> 4);
  aui8_8b_[0] = aui8_8b_[0] ^ 0xff;
  aui8_8b_[1]=((addr[1] & 0x0f) << 4) | ((addr[2] & 0xf0) >> 4);
  aui8_8b_[2]=((addr[2] & 0x0f) << 4) | ((addr[3] & 0xf0) >> 4);
  aui8_8b_[3]=((addr[3] & 0x0f) << 4) | ((addr[4] & 0xf0) >> 4);
  aui8_8b_[4]=addr[5];
  aui8_8b_[5]=addr[6];
  aui8_8b_[6]=addr[7];
  aui8_8b_[7]=0x00;
  return crc8da(aui8_8b_,8);
//  return crc8ccitt(aui8_8b_,8);


//  aui8_8b[7]=addr[7];
/*  aui8_7b[6]=addr[6];
  aui8_7b[5]=addr[5];
  aui8_7b[4]=((addr[4] & 0xf0) >> 4) | ((addr[3] & 0x0f) << 4);
  aui8_7b[3]=((addr[3] & 0xf0) >> 4) | ((addr[2] & 0x0f) << 4);
  aui8_7b[2]=((addr[2] & 0xf0) >> 4) | ((addr[1] & 0x0f) << 4);
  aui8_7b[1]=((addr[1] & 0xf0) >> 4) | ((addr[0] & 0xf0));
  aui8_7b[0]=0xff;
  return crc8da(aui8_7b,7);*/

/*  aui8_7b[6]=addr[7];
  aui8_7b[5]=addr[6];
  aui8_7b[4]=addr[5];
  aui8_7b[3]=((addr[4] & 0xf0) >> 4) | ((addr[3] & 0x0f) << 4);
  aui8_7b[2]=((addr[3] & 0xf0) >> 4) | ((addr[2] & 0x0f) << 4);
  aui8_7b[1]=((addr[2] & 0xf0) >> 4) | ((addr[1] & 0x0f) << 4);
  aui8_7b[0]=((addr[1] & 0xf0) >> 4) | ((addr[0] & 0xf0));
  return crc8da(aui8_7b,7);*/
}


/**
  * @brief  This function Calculate FFT in Q15.
  * @param  FFT Length : 1024, 256, 64
  * @retval None
  */
void FFT_PROCESSING_Q15Process(uint32_t FFT_Length)
{

  arm_cfft_radix4_instance_q15  FFT_Q15_struct;

  q15_t maxValue;    /* Max FFT value is stored here */
  uint32_t maxIndex;    /* Index in Output array where max value is */

  uint32_t index_fill_input_buffer, index_fill_output_buffer, index_fill_adc_buffer = 0;
  uint32_t duration_us = 0x00;

  for (index_fill_adc_buffer = 0; index_fill_adc_buffer < FFT_Length*2; index_fill_adc_buffer ++)
  {

    int crc_ok = 0;
//    while (!crc_ok)
    {

      int drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
      while ((drdy_pin == 0))
      {
        drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
      }

      if(0)
      {
//        HAL_SPI_Receive(&hspi1, data, uint8_ad_number, 5000);
      }
      else
      {
        if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)aRxBuffer, uint8_ad_number) != HAL_OK)
  //        if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)data, uint8_ad_number) != HAL_OK)
  //        if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, aRxBuffer, uint8_ad_number) != HAL_OK)
        {
          // Transfer error in transmission process
          Error_Handler();
        }


        /*##-3- Wait for the end of the transfer ###################################*/
        /*  Before starting a new communication transfer, you need to check the current
            state of the peripheral; if it’s busy you need to wait for the end of current
            transfer before starting a new one.
            For simplicity reasons, this example is just waiting till the end of the
            transfer, but application may perform other tasks while transfer operation
            is ongoing. */
        while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
        {
        }

        for(int ad_data_byte = 0; ad_data_byte < uint8_ad_number; ad_data_byte ++)
        {
//          data[ad_data_byte] = aRxBuffer[ad_data_byte];
        }
      }

//      crc8_01=crc8da_7(&(data[0]));
//
//      if(!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//      if(
//        (((data[0]>=0b10000000)&&(data[0]<0b10010000))||((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//      &&(((data[4]>=0b10010000)&&(data[4]<0b10100000))||((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//      )
//      {
////        c0++;
//        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
//        crc_ok = 1;
//      }
//      else
//      {
//        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//      }

    }

//    _uhADCxConvertedValue = data[1]<<16 | data[2]<<8 | data[3];

    _aADC1ConvertedValue_s[index_fill_adc_buffer] = _uhADCxConvertedValue;
//    TIM2_Config();
  }
  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer += 2)
  {
    FFT_Input_Q15_f[(uint16_t)index_fill_input_buffer] = (float32_t)_uhADCxConvertedValue / (float32_t)4096.0;
    /* Imaginary part */
    FFT_Input_Q15_f[(uint16_t)(index_fill_input_buffer + 1)] = 0;

//    TIM2_Config();
  }

  arm_float_to_q15((float32_t *)&FFT_Input_Q15_f[0], (q15_t *)&aFFT_Input_Q15[0], FFT_Length*2);

  /* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
  arm_cfft_radix4_init_q15(&FFT_Q15_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);

//  TimerCount_Start();
  arm_cfft_radix4_q15(&FFT_Q15_struct, aFFT_Input_Q15);
//  TimerCount_Stop(nb_cycles);

//  GUI_Clear();
//  LCD_OUTPUT_Cycles(5, 305, nb_cycles);
//  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
//  LCD_OUTPUT_DURATION(120, 305, duration_us);

  /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
  arm_cmplx_mag_q15(aFFT_Input_Q15, FFT_Output_Q15, FFT_Length);

  /* Calculates maxValue and returns corresponding value */
  arm_max_q15(FFT_Output_Q15, FFT_Length, &maxValue, &maxIndex);
  maxValue = 0;

  if(0)
  {
    print_text("a_");
    for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer++)
    {
  //    GRAPH_DATA_YT_SetAlign(aGraph_Data[1], GRAPH_ALIGN_LEFT);
  //    GRAPH_DATA_YT_MirrorX (aGraph_Data[1], 1);
  //    GRAPH_DATA_YT_AddValue(aGraph_Data[1], aADC1ConvertedValue_s[index_fill_input_buffer] / 100 + 50);
      print_hex(_aADC1ConvertedValue_s[index_fill_input_buffer] / 100 + 50, 8);
      print_text(";");
    }
    print_line();
    print_text("f_");
  }
  print_hex(uicount++,8);
  print_text(":");
  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
  {
//    GRAPH_DATA_YT_SetAlign(aGraph_Data[0], GRAPH_ALIGN_LEFT);
//    GRAPH_DATA_YT_MirrorX (aGraph_Data[0], 1);
//    GRAPH_DATA_YT_AddValue(aGraph_Data[0], FFT_Output_Q15[index_fill_output_buffer] / 50 + 10);
    print_hex(FFT_Output_Q15[index_fill_output_buffer] / 50 + 10, 8);
    print_text(";");
  }
  print_line();

}

/**
  * @brief  This function Calculate FFT in F32.
  * @param  FFT Length : 1024, 256, 64
  * @retval None
  */
void FFT_PROCESSING_F32Process(uint32_t FFT_Length)
{
  arm_cfft_radix4_instance_f32  FFT_F32_struct;

  float32_t maxValue;    /* Max FFT value is stored here */
  uint32_t maxIndex;    /* Index in Output array where max value is */

  unsigned index_fill_input_buffer, index_fill_output_buffer, index_fill_adc_buffer = 0;
  uint32_t duration_us = 0x00;

  for (index_fill_adc_buffer = 0; index_fill_adc_buffer < FFT_Length*2; index_fill_adc_buffer ++)
  {

    int crc_ok = 0;
//    while (!crc_ok)
    {

      int drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
      while ((drdy_pin == 0))
      {
        drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
      }

      if(0)
      {
//        HAL_SPI_Receive(&hspi1, data, uint8_ad_number, 5000);
      }
      else
      {
        if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)aRxBuffer, uint8_ad_number) != HAL_OK)
  //        if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)data, uint8_ad_number) != HAL_OK)
  //        if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, aRxBuffer, uint8_ad_number) != HAL_OK)
        {
          // Transfer error in transmission process
          Error_Handler();
        }


        /*##-3- Wait for the end of the transfer ###################################*/
        /*  Before starting a new communication transfer, you need to check the current
            state of the peripheral; if it’s busy you need to wait for the end of current
            transfer before starting a new one.
            For simplicity reasons, this example is just waiting till the end of the
            transfer, but application may perform other tasks while transfer operation
            is ongoing. */
        while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
        {
        }

        for(int ad_data_byte = 0; ad_data_byte < uint8_ad_number; ad_data_byte ++)
        {
//          data[ad_data_byte] = aRxBuffer[ad_data_byte];
        }
      }

//      crc8_01=crc8da_7(&(data[0]));
//
//      if(!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//      if(
//        (((data[0]>=0b10000000)&&(data[0]<0b10010000))||((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//      &&(((data[4]>=0b10010000)&&(data[4]<0b10100000))||((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//      )
//      {
////        c0++;
//        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
//        crc_ok = 1;
//      }
//      else
//      {
//        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//      }

    }

//    _uhADCxConvertedValue = data[1]<<16 | data[2]<<8 | data[3];

    _aADC1ConvertedValue_s[index_fill_adc_buffer] = _uhADCxConvertedValue;
///    TIM2_Config();
  }
  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer += 2)
  {
    aFFT_Input_f32[(uint16_t)index_fill_input_buffer] = (float32_t)_uhADCxConvertedValue / (float32_t)4096.0;
    /* Imaginary part */
    aFFT_Input_f32[(uint16_t)(index_fill_input_buffer + 1)] = 0;
///    TIM2_Config();
  }
  /* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
  arm_cfft_radix4_init_f32(&FFT_F32_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);

//  TimerCount_Start();
  arm_cfft_radix4_f32(&FFT_F32_struct, aFFT_Input_f32);
//  TimerCount_Stop(nb_cycles);

///  GUI_Clear();
///  LCD_OUTPUT_Cycles(5, 305, nb_cycles);
//  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
///  LCD_OUTPUT_DURATION(120, 305, duration_us);

  /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
  arm_cmplx_mag_f32(aFFT_Input_f32, aFFT_Output_f32, FFT_Length);

  /* Calculates maxValue and returns corresponding value */
  arm_max_f32(aFFT_Output_f32, FFT_Length, &maxValue, &maxIndex);
  maxValue = 0;

  if(0)
  {
    print_text("a_");
    for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer++)
    {
  ///    GRAPH_DATA_YT_SetAlign(aGraph_Data[1], GRAPH_ALIGN_LEFT);
  ///    GRAPH_DATA_YT_MirrorX (aGraph_Data[1], 1);
  ///    GRAPH_DATA_YT_AddValue(aGraph_Data[1], aADC1ConvertedValue_s[index_fill_input_buffer] / 50 + 50);
      print_hex(_aADC1ConvertedValue_s[index_fill_input_buffer] / 50 + 50, 8);
      print_text(";");
    }
    print_line();
    print_text("f_");
  }
  print_hex(uicount++,8);
  print_text(":");
  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
  {
///    GRAPH_DATA_YT_SetAlign(aGraph_Data[0], GRAPH_ALIGN_LEFT);
///    GRAPH_DATA_YT_MirrorX (aGraph_Data[0], 1);
///    GRAPH_DATA_YT_AddValue(aGraph_Data[0], aFFT_Output_f32[index_fill_output_buffer] + 10);
    print_hex((int)aFFT_Output_f32[index_fill_output_buffer] + 10, 8);
    print_text(";");
  }
  print_line();
}

/**
  * @brief  This function Calculate FFT in Q31.
  * @param  FFT Length : 1024, 256, 64
  * @retval None
  */
void FFT_PROCESSING_Q31Process(uint32_t FFT_Length)
{
//  arm_cfft_radix4_instance_q31  FFT_Q31_struct;
  arm_cfft_instance_q31  FFT_Q31_struct;

  q31_t maxValue;    /* Max FFT value is stored here */
  uint32_t maxIndex;    /* Index in Output array where max value is */

  uint32_t index_fill_input_buffer, index_fill_output_buffer, index_fill_adc_buffer = 0;
  uint32_t duration_us = 0x00;

  if(last_index_fill_adc_buffer==FFT_Length*2)
  {
    last_index_fill_adc_buffer--;
    for (index_fill_adc_buffer = 0; index_fill_adc_buffer < FFT_Length*2-1; index_fill_adc_buffer ++)
    {
      _aADC1ConvertedValue_s[index_fill_adc_buffer] = _aADC1ConvertedValue_s[index_fill_adc_buffer+1];
    }
  }

//  last_index_fill_adc_buffer = 0;

  for (index_fill_adc_buffer = last_index_fill_adc_buffer; index_fill_adc_buffer < FFT_Length*2; index_fill_adc_buffer ++)
//  for (index_fill_adc_buffer = 0; index_fill_adc_buffer < FFT_Length*2; index_fill_adc_buffer ++)
  {

//    int crc_ok = 0;
//    while (!crc_ok)
    {


      int drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
      while ((drdy_pin == 0))
      {
        drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
      }
//      int miso = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
//      if ((drdy_pin == 0))
      {   // ADC Converter ready ?
//        if (miso == 0)
        {   // ADC Converter ready ?

          if(0)
          {
//            HAL_SPI_Receive(&hspi1, data, uint8_ad_number, 5000);
          }
          else
          {
//            if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)aRxBuffer, uint8_ad_number) != HAL_OK)
//            if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)data, uint8_ad_number) != HAL_OK)
      //        if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, aRxBuffer, uint8_ad_number) != HAL_OK)
            {
              // Transfer error in transmission process
              Error_Handler();
            }


            /*##-3- Wait for the end of the transfer ###################################*/
            /*  Before starting a new communication transfer, you need to check the current
                state of the peripheral; if it’s busy you need to wait for the end of current
                transfer before starting a new one.
                For simplicity reasons, this example is just waiting till the end of the
                transfer, but application may perform other tasks while transfer operation
                is ongoing. */
            while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
            {
            }

//            for(int ad_data_byte = 0; ad_data_byte < uint8_ad_number; ad_data_byte ++)
//            {
//              data[ad_data_byte] = aRxBuffer[ad_data_byte];
//            }
          }

//          crc8_01=crc8da_7(&(data[0]));
//
//          if(
//              ((!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//             ||(!(((uint8_t)crc8_01)-((~((uint8_t)((data[0]<<4)|(data[4]&0x0f))))&0b00000111)))
//              )
//          &&(
//            (((data[0]>=0b10000000)&&(data[0]<0b10010000))||((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//          &&(((data[4]>=0b10010000)&&(data[4]<0b10100000))||((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//          ))
//          {
//            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
//          }
//          else
//          {
//            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//          }
          if(1)
          {
    //        c0++;
//            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
//            crc_ok = 1;
          }
          else
          {
//            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
          }

        }
      }
    }

//    _uhADCxConvertedValue = data[1]<<16 | data[2]<<8 | data[3];

    _aADC1ConvertedValue_s[index_fill_adc_buffer] = _uhADCxConvertedValue;
///    TIM2_Config();
  }
  last_index_fill_adc_buffer=index_fill_adc_buffer;
  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer += 2)
  {
    FFT_Input_Q31_f[(uint16_t)index_fill_input_buffer] = (float32_t)_uhADCxConvertedValue;// / (float32_t)4096.0;
    /* Imaginary part */
    FFT_Input_Q31_f[(uint16_t)(index_fill_input_buffer + 1)] = 0;

///    TIM2_Config();
  }

  arm_float_to_q31((float32_t *)&FFT_Input_Q31_f[0], (q31_t *)&aFFT_Input_Q31[0], FFT_Length*2);

  /* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
//  arm_cfft_radix4_init_q31(&FFT_Q31_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);
  arm_cfft_q31(&FFT_Q31_struct, aFFT_Input_Q31, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);

//  TimerCount_Start();
//  arm_cfft_radix4_q31(&FFT_Q31_struct, aFFT_Input_Q31);
//  TimerCount_Stop(nb_cycles);

///  GUI_Clear();
///  LCD_OUTPUT_Cycles(5, 305, nb_cycles);
//  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
///  LCD_OUTPUT_DURATION(120, 305, duration_us);

  /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
  arm_cmplx_mag_q31(aFFT_Input_Q31, FFT_Output_Q31, FFT_Length);

  /* Calculates maxValue and returns corresponding value */
  arm_max_q31(FFT_Output_Q31, FFT_Length, &maxValue, &maxIndex);
  maxValue = 0;

  if(0)
  {
    print_text("a_");
    for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer++)
    {
  ///    GRAPH_DATA_YT_SetAlign(aGraph_Data[1], GRAPH_ALIGN_LEFT);
  ///    GRAPH_DATA_YT_MirrorX (aGraph_Data[1], 1);
  ///    GRAPH_DATA_YT_AddValue(aGraph_Data[1], aADC1ConvertedValue_s[index_fill_input_buffer] / 50 + 50);
      print_hex(_aADC1ConvertedValue_s[index_fill_input_buffer] / 50 + 50, 24);
      print_text(";");
    }
    print_line();
    print_text("f_");
  }

  uicount++;
  print_hex(uicount,8);
  print_symbol(':');
  print2_hex(uicount,8);
  print2_symbol(':');
  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
//  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
  {
///    GRAPH_DATA_YT_SetAlign(aGraph_Data[0], GRAPH_ALIGN_LEFT);
///    GRAPH_DATA_YT_MirrorX (aGraph_Data[0], 1);
///    GRAPH_DATA_YT_AddValue(aGraph_Data[0], FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10);
    print_hex(FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10, 8);
    print_symbol(':');
    print2_hex(FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10, 8);
    print2_symbol(':');
  }
  print_line();
  print2_line();

}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_SPI4_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */

  //START
  HAL_GPIO_WritePin(AD_START_GPIO_Port, AD_START_Pin, GPIO_PIN_RESET);

  //RESET
  HAL_GPIO_WritePin(AD_RESET_GPIO_Port, AD_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(AD_RESET_GPIO_Port, AD_RESET_Pin, GPIO_PIN_SET);
  //2RESET
  HAL_GPIO_WritePin(AD2_RESET_GPIO_Port, AD2_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(AD2_RESET_GPIO_Port, AD2_RESET_Pin, GPIO_PIN_SET);
  //3RESET
  HAL_GPIO_WritePin(AD3_RESET_GPIO_Port, AD3_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(AD3_RESET_GPIO_Port, AD3_RESET_Pin, GPIO_PIN_SET);
  //4RESET
  HAL_GPIO_WritePin(AD4_RESET_GPIO_Port, AD4_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(AD4_RESET_GPIO_Port, AD4_RESET_Pin, GPIO_PIN_SET);

//  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
//  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);

  //ESP RESET
//  GPIO_InitTypeDef GPIO_InitStruct;
//  GPIO_InitStruct.Pin = GPIO_PIN_11;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_RESET);
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_SET);
//  HAL_Delay(1);
//  GPIO_InitStruct.Pin = GPIO_PIN_11;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


//  if(1)
//  {
//    ad7779_init_param init_params[uint8_ad_adc_number];
    ad7779_dev *devices[uint8_ad_adc_number];
    ad7779_dev *device1;
    ad7779_init_param init_param;
    uint8_t i;

//    init_param.spi_dev = &hspi1;
    init_param.spi_dev.dev = &hspi1;
//    init_param.spi_dev.chip_select_port = AD_CS_GPIO_Port;
//    init_param.spi_dev.chip_select_pin = AD_CS_Pin;
    init_param.ctrl_mode = AD7779_SPI_CTRL;

    init_param.spi_crc_en = AD7779_DISABLE;
//    init_param.spi_crc_en = AD7779_ENABLE;

    if(FREESMARTEEG_OUT & FREESMARTEEG_TEXT)
    {
      init_param.spi_crc_en = AD7779_DISABLE;
    }

    for (i = AD7779_CH0; i <= AD7779_CH7; i++)
    {
//      init_param.state[i] = AD7779_DISABLE;
      init_param.state[i] = AD7779_ENABLE;
    }
//    init_param.state[0] = AD7779_DISABLE;
//    init_param.state[1] = AD7779_DISABLE;
//    init_param.state[2] = AD7779_DISABLE;
//    init_param.state[0] = AD7779_ENABLE;
//      init_param.state[1] = AD7779_ENABLE;
//      init_param.state[2] = AD7779_ENABLE;
//      init_param.state[3] = AD7779_ENABLE;
//      init_param.state[4] = AD7779_ENABLE;
//      init_param.state[1] = AD7779_ENABLE;

    for (i = AD7779_CH0; i <= AD7779_CH7; i++) {
//      init_param.gain[i] = AD7779_GAIN_1;
      init_param.gain[i] = AD7779_GAIN_8;
    }

    init_param.pwr_mode = AD7779_HIGH_RES;
//    init_param.dec_rate_int = 0xfff;//hr 0.5001221 kHz
    init_param.dec_rate_int = 0xfa0;//hr 0.512 kHz
//    init_param.dec_rate_int = 0xc80;//hr 0.640 kHz
//    init_param.dec_rate_int = 0xa00;//hr 0.800 kHz
//    init_param.dec_rate_int = 0x800;//hr 1 kHz
//    init_param.dec_rate_int = 0x400;//hr 2 kHz
//    init_param.dec_rate_int = 0x200;//hr 4 kHz
//    init_param.dec_rate_int = 0x100;//hr 8 kHz
//    init_param.dec_rate_int = 0x80;//hr 16 kHz
//    init_param.dec_rate_int = 0x40;//hr 32 kHz

//    init_param.pwr_mode = AD7779_LOW_PWR;
//    init_param.dec_rate_int = 0xfff;//lp 0.25006105 kHz
//    init_param.dec_rate_int = 0xfa0;//lp 0.256 kHz
//    init_param.dec_rate_int = 0xc80;//lp 0.320 kHz
//    init_param.dec_rate_int = 0xa00;//lp 0.400 kHz
//    init_param.dec_rate_int = 0x800;//lp 0.500 kHz
//    init_param.dec_rate_int = 0x400;//lp 1 kHz
//    init_param.dec_rate_int = 0x200;//lp 2 kHz
//    init_param.dec_rate_int = 0x100;//lp 4 kHz
//    init_param.dec_rate_int = 0x80;//lp 8 kHz
//    init_param.dec_rate_int = 0x40;//lp 16 kHz

//    init_param.dec_rate_int = 0x20;//lp 64 kHz

    init_param.dec_rate_dec = 0;
//    init_param.dec_rate_dec = 0xfff;

    init_param.ref_type = AD7779_EXT_REF;
//    init_param.ref_type = AD7779_INT_REF;

//    init_param.pwr_mode = AD7779_HIGH_RES;
//    init_param.pwr_mode = AD7779_LOW_PWR;

    init_param.dclk_div = AD7779_DCLK_DIV_1;
//    init_param.dclk_div = AD7779_DCLK_DIV_128;

    for (i = AD7779_CH0; i <= AD7779_CH7; i++) {
      init_param.sync_offset[i] = 0;
      init_param.offset_corr[i] = 0;
      init_param.gain_corr[i] = 0;
    }

//    init_param.spi_dev.dev = &hspi3;
    init_param.spi_dev.dev = &hspi1;
//    init_param.spi_dev.chip_select_port = AD2_CS_GPIO_Port;
//    init_param.spi_dev.chip_select_pin = AD2_CS_Pin;
//    init_param.spi_dev.dev = &hspi3;
//    init_param.spi_dev.chip_select_port = AD4_CS_GPIO_Port;
//    init_param.spi_dev.chip_select_pin = AD4_CS_Pin;
//    init_param.spi_dev.dev = &hspi3;
//    init_param.spi_dev.chip_select_port = AD3_CS_GPIO_Port;
//    init_param.spi_dev.chip_select_pin = AD3_CS_Pin;

//    init_param.dec_rate_int = 0x1000;//hr 0.5 kHz
//    init_param.dec_rate_int = 0x80;//hr 0.5 kHz
//    init_param.dec_rate_int = 0x40;//hr 0.5 kHz
//    init_param.dec_rate_int = 0x1000;//hr 0.5 kHz

//    HAL_GPIO_WritePin(init_param.spi_dev.chip_select_port, init_param.spi_dev.chip_select_pin, GPIO_PIN_SET);

    ad7779_setup(&device1, init_param);

    ad7779_spi_int_reg_write_mask(device1,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_SOURCE,AD7779_DISABLE);
    ad7779_spi_int_reg_write_mask(device1,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_UPDATE,AD7779_ENABLE);
    HAL_Delay(1);
    ad7779_spi_int_reg_write_mask(device1,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_UPDATE,AD7779_DISABLE);

//    ad7779_set_spi_op_mode(device, AD7779_SD_CONV);

    ad7779_dev *device2;
    ad7779_init_param init_param2 = init_param;

//    init_param2.spi_dev.dev = &hspi4;
    init_param2.spi_dev.dev = &hspi2;
//    init_param2.spi_dev.dev = &hspi1;
//    init_param2.spi_dev.chip_select_port = AD2_CS_GPIO_Port;
//    init_param2.spi_dev.chip_select_pin = AD2_CS_Pin;
//    init_param2.spi_dev.dev = &hspi3;
//    init_param2.spi_dev.chip_select_port = AD4_CS_GPIO_Port;
//    init_param2.spi_dev.chip_select_pin = AD4_CS_Pin;
//    init_param2.spi_dev.dev = &hspi3;
//    init_param2.spi_dev.chip_select_port = AD3_CS_GPIO_Port;
//    init_param2.spi_dev.chip_select_pin = AD3_CS_Pin;

//    init_param2.dec_rate_int = 0x1000;//hr 0.5 kHz

//    HAL_GPIO_WritePin(init_param2.spi_dev.chip_select_port, init_param2.spi_dev.chip_select_pin, GPIO_PIN_SET);

    ad7779_setup(&device2, init_param2);

    ad7779_spi_int_reg_write_mask(device2,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_SOURCE,AD7779_DISABLE);
    ad7779_spi_int_reg_write_mask(device2,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_UPDATE,AD7779_ENABLE);
    HAL_Delay(1);
    ad7779_spi_int_reg_write_mask(device2,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_UPDATE,AD7779_DISABLE);

    ad7779_dev *device3;
    ad7779_init_param init_param3 = init_param;

//    init_param3.spi_dev.dev = &hspi1;
    init_param3.spi_dev.dev = &hspi3;
//    init_param3.spi_dev.chip_select_port = AD3_CS_GPIO_Port;
//    init_param3.spi_dev.chip_select_pin = AD3_CS_Pin;
//    init_param3.spi_dev.dev = &hspi3;
//    init_param3.spi_dev.chip_select_port = AD4_CS_GPIO_Port;
//    init_param3.spi_dev.chip_select_pin = AD4_CS_Pin;
//    init_param3.spi_dev.dev = &hspi1;
//    init_param3.spi_dev.chip_select_port = AD2_CS_GPIO_Port;
//    init_param3.spi_dev.chip_select_pin = AD2_CS_Pin;
//    init_param3.spi_dev.dev = &hspi1;
//    init_param3.spi_dev.chip_select_port = AD_CS_GPIO_Port;
//    init_param3.spi_dev.chip_select_pin = AD_CS_Pin;

//    init_param3.dec_rate_int = 0x100;//hr 16 kHz
//    init_param3.dec_rate_int = 0x1000;//hr 1 kHz
//    init_param3.dec_rate_int = 0x800;//hr 1 kHz

//    HAL_GPIO_WritePin(init_param3.spi_dev.chip_select_port, init_param3.spi_dev.chip_select_pin, GPIO_PIN_SET);

    ad7779_setup(&device3, init_param3);

    ad7779_spi_int_reg_write_mask(device3,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_SOURCE,AD7779_DISABLE);
    ad7779_spi_int_reg_write_mask(device3,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_UPDATE,AD7779_ENABLE);
    HAL_Delay(1);
    ad7779_spi_int_reg_write_mask(device3,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_UPDATE,AD7779_DISABLE);

    ad7779_dev *device4;
    ad7779_init_param init_param4 = init_param;

//    init_param4.spi_dev.dev = &hspi2;
    init_param4.spi_dev.dev = &hspi4;
//    init_param4.spi_dev.dev = &hspi3;
//    init_param4.spi_dev.chip_select_port = AD4_CS_GPIO_Port;
//    init_param4.spi_dev.chip_select_pin = AD4_CS_Pin;
//    init_param4.spi_dev.dev = &hspi1;
//    init_param4.spi_dev.chip_select_port = AD2_CS_GPIO_Port;
//    init_param4.spi_dev.chip_select_pin = AD2_CS_Pin;
//    init_param4.spi_dev.dev = &hspi1;
//    init_param4.spi_dev.chip_select_port = AD_CS_GPIO_Port;
//    init_param4.spi_dev.chip_select_pin = AD_CS_Pin;

//    init_param4.dec_rate_int = 0x1000;//hr 1 kHz
//    init_param4.dec_rate_int = 0x800;//hr 1 kHz

//    HAL_GPIO_WritePin(init_param4.spi_dev.chip_select_port, init_param4.spi_dev.chip_select_pin, GPIO_PIN_SET);

    ad7779_setup(&device4, init_param4);

    ad7779_spi_int_reg_write_mask(device4,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_SOURCE,AD7779_DISABLE);
    ad7779_spi_int_reg_write_mask(device4,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_UPDATE,AD7779_ENABLE);
    HAL_Delay(1);
    ad7779_spi_int_reg_write_mask(device4,AD7779_REG_SRC_UPDATE,AD7779_SRC_LOAD_UPDATE,AD7779_DISABLE);

    devices[0]=device1;
    devices[1]=device2;
    devices[2]=device3;
    devices[3]=device4;
//    init_params[0]=init_param;
//    init_params[1]=init_param2;
//    init_params[2]=init_param3;
//    init_params[3]=init_param4;

//    ad7779_set_spi_op_mode(device2, AD7779_SD_CONV);

//    ad7779_set_spi_op_mode(0, AD7779_SD_CONV);
//  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    //  for(int ad_data_byte = 0; ad_data_byte < 100; ad_data_byte ++)
    if(1)
    {
        HAL_Delay(1);
        aTxBuffer[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
        aTxBuffer[1] = AD7779_SPI_SLAVE_MODE_EN;
      //  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
      //  HAL_SPI_Transmit(&hspi1, data, 2, 5000);
  //      if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)data, 2) != HAL_OK)
  //      if(SPI_DMA)
  //      {
  //        if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, data, 2) != HAL_OK)
  //        {
  //          Error_Handler();
  //        }
  //      }
  //      else
        {
          for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
          {
            aTxBuffer[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
            aTxBuffer[1] = AD7779_SPI_SLAVE_MODE_EN;

  //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
            if(HAL_SPI_TransmitReceive_DMA(devices[ad_adc]->spi_dev.dev, aTxBuffer, datas[ad_adc], 2) != HAL_OK)
  //          if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, 2, 5000) != HAL_OK)
            {
              Error_Handler();
            }
  //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);

            while (HAL_SPI_GetState(devices[ad_adc]->spi_dev.dev) != HAL_SPI_STATE_READY)
            {
            }

          }

////          HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//          if(HAL_SPI_TransmitReceive_DMA(device1->spi_dev.dev, aTxBuffer, data1, 2) != HAL_OK)
////          if(HAL_SPI_TransmitReceive(device->spi_dev.dev, aTxBuffer, data, 2, 5000) != HAL_OK)
//          {
//            Error_Handler();
//          }
////          HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//          while (HAL_SPI_GetState(device1->spi_dev.dev) != HAL_SPI_STATE_READY)
//          {
//          }
//
//          aTxBuffer[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
//          aTxBuffer[1] = AD7779_SPI_SLAVE_MODE_EN;
//
////          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//          if(HAL_SPI_TransmitReceive_DMA(device2->spi_dev.dev, aTxBuffer, data2, 2) != HAL_OK)
////          if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, 2, 5000) != HAL_OK)
//          {
//            Error_Handler();
//          }
////          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//          while (HAL_SPI_GetState(device2->spi_dev.dev) != HAL_SPI_STATE_READY)
//          {
//          }
//
//          aTxBuffer[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
//          aTxBuffer[1] = AD7779_SPI_SLAVE_MODE_EN;
//
////          HAL_GPIO_WritePin(device3->spi_dev.chip_select_port, device3->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//          if(HAL_SPI_TransmitReceive_DMA(device3->spi_dev.dev, aTxBuffer, data3, 2) != HAL_OK)
////          if(HAL_SPI_TransmitReceive(device3->spi_dev.dev, aTxBuffer, data3, 2, 5000) != HAL_OK)
//          {
//            Error_Handler();
//          }
////          HAL_GPIO_WritePin(device3->spi_dev.chip_select_port, device3->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//          while (HAL_SPI_GetState(device3->spi_dev.dev) != HAL_SPI_STATE_READY)
//          {
//          }
//
//          aTxBuffer[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
//          aTxBuffer[1] = AD7779_SPI_SLAVE_MODE_EN;
//
////          HAL_GPIO_WritePin(device4->spi_dev.chip_select_port, device4->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//          if(HAL_SPI_TransmitReceive_DMA(device4->spi_dev.dev, aTxBuffer, data4, 2) != HAL_OK)
////          if(HAL_SPI_TransmitReceive(device4->spi_dev.dev, aTxBuffer, data4, 2, 5000) != HAL_OK)
//          {
//            Error_Handler();
//          }
////          HAL_GPIO_WritePin(device4->spi_dev.chip_select_port, device4->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//          while (HAL_SPI_GetState(device4->spi_dev.dev) != HAL_SPI_STATE_READY)
//          {
//          }

        }
      //  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    }
        HAL_Delay(1);
    if(1)
    {
        aTxBuffer[0] = AD7779_REG_GEN_ERR_REG_1_EN;
        aTxBuffer[1] = AD7779_SPI_CRC_TEST_EN;
      //  HAL_SPI_Transmit(&hspi1, data, 2, 5000);
  //      if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)data, 2) != HAL_OK)
  //      if(SPI_DMA)
  //      {
  //        if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, data, 2) != HAL_OK)
  //        {
  //          Error_Handler();
  //        }
  //      }
  //      else
        {
          for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
          {
            aTxBuffer[0] = AD7779_REG_GEN_ERR_REG_1_EN;
            aTxBuffer[1] = AD7779_SPI_CRC_TEST_EN;

  //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
            if(HAL_SPI_TransmitReceive_DMA(devices[ad_adc]->spi_dev.dev, aTxBuffer, datas[ad_adc], 2) != HAL_OK)
  //          if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, 2, 5000) != HAL_OK)
            {
              Error_Handler();
            }
  //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);

            while (HAL_SPI_GetState(devices[ad_adc]->spi_dev.dev) != HAL_SPI_STATE_READY)
            {
            }

          }

////          HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//          if(HAL_SPI_TransmitReceive_DMA(device1->spi_dev.dev, aTxBuffer, data1, 2) != HAL_OK)
////          if(HAL_SPI_TransmitReceive(device->spi_dev.dev, aTxBuffer, data, 2, 5000) != HAL_OK)
//          {
//            Error_Handler();
//          }
////          HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//          while (HAL_SPI_GetState(device1->spi_dev.dev) != HAL_SPI_STATE_READY)
//          {
//          }
//
//          aTxBuffer[0] = AD7779_REG_GEN_ERR_REG_1_EN;
//          aTxBuffer[1] = AD7779_SPI_CRC_TEST_EN;
//
////          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//          if(HAL_SPI_TransmitReceive_DMA(device2->spi_dev.dev, aTxBuffer, data2, 2) != HAL_OK)
////          if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, 2, 5000) != HAL_OK)
//          {
//            Error_Handler();
//          }
////          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//          while (HAL_SPI_GetState(device2->spi_dev.dev) != HAL_SPI_STATE_READY)
//          {
//          }
//
//          aTxBuffer[0] = AD7779_REG_GEN_ERR_REG_1_EN;
//          aTxBuffer[1] = AD7779_SPI_CRC_TEST_EN;
//
////          HAL_GPIO_WritePin(device3->spi_dev.chip_select_port, device3->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//          if(HAL_SPI_TransmitReceive_DMA(device3->spi_dev.dev, aTxBuffer, data3, 2) != HAL_OK)
////          if(HAL_SPI_TransmitReceive(device3->spi_dev.dev, aTxBuffer, data3, 2, 5000) != HAL_OK)
//          {
//            Error_Handler();
//          }
////          HAL_GPIO_WritePin(device3->spi_dev.chip_select_port, device3->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//          while (HAL_SPI_GetState(device3->spi_dev.dev) != HAL_SPI_STATE_READY)
//          {
//          }
//
//          aTxBuffer[0] = AD7779_REG_GEN_ERR_REG_1_EN;
//          aTxBuffer[1] = AD7779_SPI_CRC_TEST_EN;
//
////          HAL_GPIO_WritePin(device4->spi_dev.chip_select_port, device4->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//          if(HAL_SPI_TransmitReceive_DMA(device4->spi_dev.dev, aTxBuffer, data4, 2) != HAL_OK)
////          if(HAL_SPI_TransmitReceive(device4->spi_dev.dev, aTxBuffer, data4, 2, 5000) != HAL_OK)
//          {
//            Error_Handler();
//          }
////          HAL_GPIO_WritePin(device4->spi_dev.chip_select_port, device4->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//          while (HAL_SPI_GetState(device4->spi_dev.dev) != HAL_SPI_STATE_READY)
//          {
//          }

        }
      //  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    }

    HAL_GPIO_WritePin(AD_START_GPIO_Port, AD_START_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(AD_START_GPIO_Port, AD_START_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(AD_START_GPIO_Port, AD_START_Pin, GPIO_PIN_RESET);

    for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
    {
      for(int ad_data_byte = 0; ad_data_byte < uint8_ad_number; ad_data_byte ++)
      {
        datas[ad_adc][ad_data_byte] = 0;
        aRxBuffer[ad_data_byte] = 0;
        aTxBuffer[ad_data_byte] = 0;
      }
    }

  //  if(0)
    if(FREESMARTEEG_OUT & FREESMARTEEG_FFT)
    while (1)
    {
      // data is in 11.21(q21) format for the 1024 point as there are 3 middle stages
      // data is in 9.23(q23) format for the 256 point as there are 2 middle stages
      // data is in 7.25(q25) format for the 64 point as there are 1 middle stage
      // data is in 5.27(q27) format for the 16 point as there are no middle stages
//      uint32_t FFT_Length = 16;
  //    uint32_t FFT_Length = 64;
  //    uint32_t FFT_Length = 256;
      //    uint32_t FFT_Length = 1024;
  //    FFT_PROCESSING_Q15Process(FFT_Length);
  //    FFT_PROCESSING_F32Process(FFT_Length);
////      FFT_PROCESSING_Q31Process(FFT_Length);
    }

    if(0)
    while (1)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
      HAL_Delay(40);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
      HAL_Delay(40);
    }

    long test_counter=0;
    long test_blink_counter=0;

//    const int sample_number_max = 480;
//    const int sample_number_max = 240;
//    const int sample_number_max = 15;
    const int sample_number_max = 128;
//    const int sample_number_max = 256;
//    const int sample_number_max = 512;
//    const int sample_number_max = 1024;
//    const int sample_number_max = 2048;
    int sample_freq_fft = 64;
    int fft_out=16;
    int fft_out_shift=5;
    int32_t sample_int[sample_number_max];
    uint8_t sample_byte[sample_number_max];
    int sample_number_now = 0;
//    int sample_average = 0;
    int64_t sample_sum=0;
    int32_t sample_max=0;
    int32_t sample_min=0;
    int32_t sample_max_last=0;
    int32_t sample_min_last=0;

//    int32_t samples_int[uint8_ad_adc_number][8][sample_number_max];
    int32_t samples_int_part[sample_number_max];
//    uint8_t samples_byte[uint8_ad_adc_number][8][sample_number_max];
    int32_t samples_max[uint8_ad_adc_number][8];//={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
    int32_t samples_min[uint8_ad_adc_number][8];//={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
    int32_t samples_max_min_dif[uint8_ad_adc_number][8];
    int32_t sample_max_min_dif_max=0;
    int32_t sample_max_min_dif_min=0;
    int32_t sample_max_new;
    int32_t sample_min_new;
    int32_t samples_max_new[uint8_ad_adc_number][8];
    int32_t samples_min_new[uint8_ad_adc_number][8];

    int32_t sample_int_out;
    uint8_t sample_byte_out;
//    double sample_int_byte_scale;
//    int sample_int_byte_shift;

    const uint32_t sample_data_number = sample_number_max;
    uint8_t sample_dataBuffer[sample_data_number];

    const uint32_t telnet_packet_data_number_max = 100;//32;
    const uint32_t telnet_data_number_max = 2 + 8 * 3 + 3 * 2 + 1;
//                const uint32_t uint8_data_number = 2 + 8 * 3 + 8 * 3 + 3 * 2 + 1;
    uint8_t telnet_dataBuffer[telnet_packet_data_number_max][telnet_data_number_max];

    int telnet_packet_data_number_now = 0;

    long data_counter=0;
    long stable_crc[uint8_ad_adc_number][8/2]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

//    for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
//    {
//      for(int ad_data_channel_pair = 0; ad_data_channel_pair < (uint8_ad_chan_number / 2); ad_data_channel_pair ++)
//      {
//        stable_crc[adc][ad_data_channel_pair]=0;
//      }
//    }

    if(1)
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */


    if(FREESMARTEEG_OUT & FREESMARTEEG_TEST_COUNTER)
    {
      test_counter++;
      print_hex(test_counter, 32);
      print_symbol(';');
      print2_hex(test_counter, 32);
      print2_symbol(';');
    }

    if(FREESMARTEEG_OUT & FREESMARTEEG_TEST_BLINK)
    {
      test_blink_counter++;
      if(test_blink_counter%1000<500)
      {
        HAL_GPIO_WritePin(led_ports[0], led_pins[0], GPIO_PIN_SET);
      }
      else
      {
        HAL_GPIO_WritePin(led_ports[0], led_pins[0], GPIO_PIN_RESET);

      }
    }

    //    data[0] = AD7779_REG_GENERAL_USER_CONFIG_1;
    //    data[1] = 0;
        if(0)
        {
          data1[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
          data1[1] = AD7779_SPI_SLAVE_MODE_EN;
      //    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//          HAL_SPI_Transmit(&hspi4, data1, 2, 5000);
          HAL_SPI_Transmit_DMA(&hspi4, data1, 2);
      //    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
          data1[0] = AD7779_REG_GEN_ERR_REG_1_EN;
          data1[1] = AD7779_SPI_CRC_TEST_EN;
        //  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//          HAL_SPI_Transmit(&hspi4, data1, 2, 5000);
          HAL_SPI_Transmit_DMA(&hspi4, data1, 2);
        //  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
        }

        if(0)
        {
          int ad_adc = 3;
//          for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
          {
            aTxBuffer[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
            aTxBuffer[1] = AD7779_SPI_SLAVE_MODE_EN;
//            aTxBuffer[0]=0x80;
//            aTxBuffer[1]=0x00;
  //              HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_RESET);
  //              if(HAL_SPI_TransmitReceive(&hspi1, aTxBuffer, data, uint8_ad_number, 5000) != HAL_OK)
            if(HAL_SPI_TransmitReceive_DMA(devices[ad_adc]->spi_dev.dev, aTxBuffer, datas[ad_adc], 2) != HAL_OK)
  //                print2_symbol('1');
  //                if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, data1, uint8_ad_number) != HAL_OK)
  //                if(HAL_SPI_Receive_DMA(&hspi1, data1, uint8_ad_number) != HAL_OK)
  //              if(HAL_SPI_TransmitReceive(device->spi_dev.dev, aTxBuffer, data, uint8_ad_number, 5000) != HAL_OK)
            {
              // Transfer error in transmission process
              Error_Handler();
  //                  print2_symbol('?');
            }
            while (HAL_SPI_GetState(devices[ad_adc]->spi_dev.dev) != HAL_SPI_STATE_READY)
//                while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
            {
            }
          }

        }

    //    for(int ad_data_byte = 0; ad_data_byte < uint8_ad_number; ad_data_byte ++)
    //    {
    //      data[ad_data_byte] = 0;
    //    }

    //    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    //    HAL_Delay(50);
    //    HAL_SPI_TransmitReceive_DMA(&hspi1, data, data, 2);
    //    HAL_SPI_TransmitReceive(&hspi1, data, data, 2, 5000);


        if(0)
        {
          data1[0] = 0x80 | AD7779_REG_GENERAL_USER_CONFIG_1;
    //      data[0] = 0x80 | AD7779_REG_GENERAL_USER_CONFIG_3;
          data1[1] = 0;

          print_binary(data1[0], 8);    print_symbol(';');
          print_binary(data1[1], 8);    print_symbol(';');
          print_symbol(';');

    //      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//         spi_write_and_read(0, data, 2);
    //      spi_write_and_read(0, data, 2);
          HAL_SPI_TransmitReceive(&hspi1, data1, data1, 2, 5000);
      //    HAL_SPI_Transmit(&hspi1, data, 2, 5000);
    //      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

          print_binary(data1[0], 8);    print_symbol(';');
          print_binary(data1[1], 8);    print_symbol(';');
          print_line();

      //    print_line();

        }

    //    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    //    HAL_Delay(50);
    //    print_text_line("hello world!");
    //    print_symbol('!');
    //    print_binary(data[0], 8);
    //    print_line();
    //    HAL_UART_Transmit(&huart1, data, 1, 5000);

    //    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    //    HAL_Delay(200);
    //    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    //    HAL_Delay(200);

//        int drdy_pin = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5);

        int drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
//        int drdy2_pin = HAL_GPIO_ReadPin(AD2_DRDY_GPIO_Port, AD2_DRDY_Pin);
//        int drdy3_pin = HAL_GPIO_ReadPin(AD3_DRDY_GPIO_Port, AD3_DRDY_Pin);
//        int drdy4_pin = HAL_GPIO_ReadPin(AD4_DRDY_GPIO_Port, AD4_DRDY_Pin);
//        print2_hex(drdy_pin, 1);
//        print2_symbol(';');
        int drdy_count=0;
        while (drdy_pin == 0)
//        while (drdy2_pin == 0)
//        while (drdy3_pin == 0)
//        while (drdy4_pin == 0)
//        while ((drdy_pin == 0)||(drdy2_pin == 0)||(drdy3_pin == 0)||(drdy4_pin == 0))
//        while ((drdy_pin == 0)&&(drdy2_pin == 0)&&(drdy3_pin == 0)&&(drdy4_pin == 0))
//        while ((drdy_pin == 0)||(drdy2_pin == 0))
        {
          drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
//          drdy2_pin = HAL_GPIO_ReadPin(AD2_DRDY_GPIO_Port, AD2_DRDY_Pin);
//          drdy3_pin = HAL_GPIO_ReadPin(AD3_DRDY_GPIO_Port, AD3_DRDY_Pin);
//          drdy4_pin = HAL_GPIO_ReadPin(AD4_DRDY_GPIO_Port, AD4_DRDY_Pin);
//          if(!drdy_pin)
//          {
//            drdy_pin = HAL_GPIO_ReadPin(AD_DRDY_GPIO_Port, AD_DRDY_Pin);
//          }
//          if(!drdy2_pin)
//          {
//            drdy2_pin = HAL_GPIO_ReadPin(AD2_DRDY_GPIO_Port, AD2_DRDY_Pin);
//          }
//          if(!drdy3_pin)
//          {
//            drdy3_pin = HAL_GPIO_ReadPin(AD3_DRDY_GPIO_Port, AD3_DRDY_Pin);
//          }
//          if(!drdy4_pin)
//          {
//            drdy4_pin = HAL_GPIO_ReadPin(AD4_DRDY_GPIO_Port, AD4_DRDY_Pin);
//          }
//          print2_hex(drdy_pin, 1);
//          print2_symbol(';');
          drdy_count++;
//          if(drdy_count>2000)
//          {
//            drdy_pin=-1;
////            drdy_count=0;
////            print2_symbol(';');
////            print2_line();
//          }
        }

//        int drdy2_pin = HAL_GPIO_ReadPin(AD2_DRDY_GPIO_Port, AD2_DRDY_Pin);
//        while ((drdy2_pin == 0))
//        {
//          drdy2_pin = HAL_GPIO_ReadPin(AD2_DRDY_GPIO_Port, AD2_DRDY_Pin);
//        }

//        int miso = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
    //    if ((drdy_pin == 0))
        {   // ADC Converter ready ?
    //      if (miso == 0)
          {   // ADC Converter ready ?

    //        spi_write_and_read(0, data, uint8_ad_number);
    //        HAL_SPI_TransmitReceive_DMA(&hspi1, data, data, uint8_ad_number);
    //        HAL_SPI_TransmitReceive(&hspi1, data, data, uint8_ad_number, 5000);

    //        HAL_SPI_Receive(&hspi1, data, uint8_ad_number, 5000);
    //        HAL_SPI_Receive_DMA(&hspi1, data, uint8_ad_number);


            if(0)
            {
              /*##-2- Start the Full Duplex Communication process ########################*/
              /* While the SPI in TransmitReceive process, user can transmit data through
                 "aTxBuffer" buffer & receive data through "aRxBuffer" */
              //        HAL_SPI_TransmitReceive_DMA(&hspi1, data, data, uint8_ad_number);

      //        if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)aRxBuffer, uint8_ad_number) != HAL_OK)
    //          if(HAL_SPI_Receive_DMA(&hspi1, (uint8_t *)data, uint8_ad_number) != HAL_OK)
              aTxBuffer[0]=0x80;
              aTxBuffer[1]=0x00;
              if(SPI_DMA)
              {
                if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, data1, uint8_ad_number) != HAL_OK)
      //          if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, aRxBuffer, uint8_ad_number) != HAL_OK)
                {
                  // Transfer error in transmission process
                  Error_Handler();
                }
              }
              else
              {
                if(HAL_SPI_TransmitReceive(&hspi1, aTxBuffer, data1, uint8_ad_number, 5000) != HAL_OK)
      //          if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, aRxBuffer, uint8_ad_number) != HAL_OK)
                {
                  // Transfer error in transmission process
                  Error_Handler();
                }
              }


              /*##-3- Wait for the end of the transfer ###################################*/
              /*  Before starting a new communication transfer, you need to check the current
                  state of the peripheral; if it’s busy you need to wait for the end of current
                  transfer before starting a new one.
                  For simplicity reasons, this example is just waiting till the end of the
                  transfer, but application may perform other tasks while transfer operation
                  is ongoing. */
              while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
              {
              }

      //        for(int ad_data_byte = 0; ad_data_byte < uint8_ad_number; ad_data_byte ++)
      //        {
      //          data[ad_data_byte] = aRxBuffer[ad_data_byte];
      //        }

            }
            else
            {

//              while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
//              {
//              }

              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
//                print2_symbol('0');
                aTxBuffer[0]=0x80;
                aTxBuffer[1]=0x00;
  //              HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_RESET);
  //              if(HAL_SPI_TransmitReceive(&hspi1, aTxBuffer, data, uint8_ad_number, 5000) != HAL_OK)
                if(HAL_SPI_TransmitReceive_DMA(devices[ad_adc]->spi_dev.dev, aTxBuffer, datas[ad_adc], uint8_ad_number) != HAL_OK)
//                print2_symbol('1');
//                if(HAL_SPI_TransmitReceive_DMA(&hspi1, aTxBuffer, data1, uint8_ad_number) != HAL_OK)
//                if(HAL_SPI_Receive_DMA(&hspi1, data1, uint8_ad_number) != HAL_OK)
  //              if(HAL_SPI_TransmitReceive(device->spi_dev.dev, aTxBuffer, data, uint8_ad_number, 5000) != HAL_OK)
                {
                  // Transfer error in transmission process
                  Error_Handler();
//                  print2_symbol('?');
                }
//                print2_symbol('2');
  //              HAL_SPI_Receive(&hspi1, data, uint8_ad_number, 5000);
  //              HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_SET);

  //              while (HAL_SPI_GetState(device->spi_dev.dev) != HAL_SPI_STATE_READY)
  //              {
  //              }

              }

////              HAL_SPI_Receive(device.spi_dev.dev, data, uint8_ad_number, 5000);
//
//              aTxBuffer[0]=0x80;
//              aTxBuffer[1]=0x00;
////              HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_RESET);
////              if(HAL_SPI_TransmitReceive(&hspi1, aTxBuffer, data, uint8_ad_number, 5000) != HAL_OK)
//              if(HAL_SPI_TransmitReceive_DMA(device1->spi_dev.dev, aTxBuffer, data1, uint8_ad_number) != HAL_OK)
////              if(HAL_SPI_TransmitReceive(device->spi_dev.dev, aTxBuffer, data, uint8_ad_number, 5000) != HAL_OK)
//              {
//                // Transfer error in transmission process
//                Error_Handler();
//              }
////              HAL_SPI_Receive(&hspi1, data, uint8_ad_number, 5000);
////              HAL_GPIO_WritePin(device->spi_dev.chip_select_port, device->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
////              while (HAL_SPI_GetState(device->spi_dev.dev) != HAL_SPI_STATE_READY)
////              {
////              }
//
//              aTxBuffer[0]=0x80;
//              aTxBuffer[1]=0x00;
//
////              HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
////              if(HAL_SPI_Transmit(&hspi1, aTxBuffer, uint8_ad_number, 5000) != HAL_OK)
//              if(HAL_SPI_TransmitReceive_DMA(device2->spi_dev.dev, aTxBuffer, data2, uint8_ad_number) != HAL_OK)
////              if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, uint8_ad_number, 5000) != HAL_OK)
////              if(HAL_SPI_TransmitReceive(&hspi1, aTxBuffer, data2, uint8_ad_number, 5000) != HAL_OK)
//              {
//                // Transfer error in transmission process
//                Error_Handler();
//              }
////              HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
////              while (HAL_SPI_GetState(device2->spi_dev.dev) != HAL_SPI_STATE_READY)
////              {
////              }
//
//              aTxBuffer[0]=0x80;
//              aTxBuffer[1]=0x00;
//
////              HAL_GPIO_WritePin(device3->spi_dev.chip_select_port, device3->spi_dev.chip_select_pin, GPIO_PIN_RESET);
////              if(HAL_SPI_Transmit(&hspi1, aTxBuffer, uint8_ad_number, 5000) != HAL_OK)
//              if(HAL_SPI_TransmitReceive_DMA(device3->spi_dev.dev, aTxBuffer, data3, uint8_ad_number) != HAL_OK)
////              if(HAL_SPI_TransmitReceive(device3->spi_dev.dev, aTxBuffer, data3, uint8_ad_number, 5000) != HAL_OK)
////              if(HAL_SPI_TransmitReceive(&hspi1, aTxBuffer, data2, uint8_ad_number, 5000) != HAL_OK)
//              {
//                // Transfer error in transmission process
//                Error_Handler();
//              }
////              HAL_GPIO_WritePin(device3->spi_dev.chip_select_port, device3->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
////              while (HAL_SPI_GetState(device3->spi_dev.dev) != HAL_SPI_STATE_READY)
////              {
////              }
//
//              aTxBuffer[0]=0x80;
//              aTxBuffer[1]=0x00;
//
////              HAL_GPIO_WritePin(device4->spi_dev.chip_select_port, device4->spi_dev.chip_select_pin, GPIO_PIN_RESET);
////              if(HAL_SPI_Transmit(&hspi1, aTxBuffer, uint8_ad_number, 5000) != HAL_OK)
//              if(HAL_SPI_TransmitReceive_DMA(device4->spi_dev.dev, aTxBuffer, data4, uint8_ad_number) != HAL_OK)
////              if(HAL_SPI_TransmitReceive(device4->spi_dev.dev, aTxBuffer, data4, uint8_ad_number, 5000) != HAL_OK)
////              if(HAL_SPI_TransmitReceive(&hspi1, aTxBuffer, data2, uint8_ad_number, 5000) != HAL_OK)
//              {
//                // Transfer error in transmission process
//                Error_Handler();
//              }
////              HAL_GPIO_WritePin(device4->spi_dev.chip_select_port, device4->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
////              while (HAL_SPI_GetState(device4->spi_dev.dev) != HAL_SPI_STATE_READY)
////              {
////              }

              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                while (HAL_SPI_GetState(devices[ad_adc]->spi_dev.dev) != HAL_SPI_STATE_READY)
//                while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
                {
                }
              }
//              HAL_GPIO_WritePin(AD_START_GPIO_Port, AD_START_Pin, GPIO_PIN_SET);
//              HAL_Delay(1);
//              HAL_GPIO_WritePin(AD_START_GPIO_Port, AD_START_Pin, GPIO_PIN_RESET);


            }

            data_counter++;


            if(FREESMARTEEG_OUT & FREESMARTEEG_OPENBCI_TELNET_PACKET)
//            if(0)//openbci
            {

              crc8_01=crc8da_7(&(datas[0][0]));

              if(
                  ((!(((uint8_t)crc8_01)-((uint8_t)((datas[0][0]<<4)|(datas[0][4]&0x0f)))))
                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((datas[0][0]<<4)|(datas[0][4]&0x0f))))&0b00000111)))
                  )
              &&(
                (//((datas[0][0]>=0b10000000)&&(datas[0][0]<0b10010000))||
                    ((datas[0][0]>=0b00000000)&&(datas[0][0]<0b00010000)))
              &&(//((datas[0][4]>=0b10010000)&&(datas[0][4]<0b10100000))||
                  ((datas[0][4]>=0b00010000)&&(datas[0][4]<0b00100000)))
              ))
              {
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
              }
              else
              {
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
              }

//              crc8_01=crc8da_7(&(datas[1][0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((datas[1][0]<<4)|(datas[1][4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((datas[1][0]<<4)|(datas[1][4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((datas[1][0]>=0b10000000)&&(datas[1][0]<0b10010000))||
//                    ((datas[1][0]>=0b00000000)&&(datas[1][0]<0b00010000)))
//              &&(//((datas[1][4]>=0b10010000)&&(datas[1][4]<0b10100000))||
//                  ((datas[1][4]>=0b00010000)&&(datas[1][4]<0b00100000)))
//              ))
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//              }
//              else
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
//              }

//              crc8_01=crc8da_7(&(data[0]));
//              crc8_23=crc8da_7(&(data[8]));
//              crc8_45=crc8da_7(&(data[16]));
//              crc8_67=crc8da_7(&(data[24]));
//
//              if(((!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//                &&(!(((uint8_t)crc8_23)-((uint8_t)((data[8]<<4)|(data[12]&0x0f)))))
//                &&(!(((uint8_t)crc8_45)-((uint8_t)((data[16]<<4)|(data[20]&0x0f)))))
//                &&(!(((uint8_t)crc8_67)-((uint8_t)((data[24]<<4)|(data[28]&0x0f))))))
//              && (
//                (((data[0]>=0b10000000)&&(data[0]<0b10010000))||((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//              &&(((data[4]>=0b10010000)&&(data[4]<0b10100000))||((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//              &&(((data[8]>=0b10100000)&&(data[8]<0b10110000))||((data[8]>=0b00100000)&&(data[8]<0b00110000)))
//              &&(((data[12]>=0b10110000)&&(data[12]<0b11000000))||((data[12]>=0b00110000)&&(data[12]<0b01000000)))
//              &&(((data[16]>=0b11000000)&&(data[16]<0b11010000))||((data[16]>=0b01000000)&&(data[16]<0b01010000)))
//              &&(((data[20]>=0b11010000)&&(data[20]<0b11100000))||((data[20]>=0b01010000)&&(data[20]<0b01100000)))
//              &&(((data[24]>=0b11100000)&&(data[24]<0b11110000))||((data[24]>=0b01100000)&&(data[24]<0b01110000)))
//              &&(((data[28]>=0b11110000)&&(data[28]<=0b11111111))||((data[28]>=0b01110000)&&(data[28]<=0b01111111)))
//              ))
              if(1)
              {
//                const uint32_t uint8_data_number = 2 + 8 * 3 + 3 * 2 + 1;
//                const uint32_t uint8_data_number = 2 + 8 * 3 + 8 * 3 + 3 * 2 + 1;
//                uint8_t dataBuffer[uint8_data_number];

                telnet_dataBuffer[telnet_packet_data_number_now][0] = 0xA0;
                telnet_dataBuffer[telnet_packet_data_number_now][1] = ui8SampleNumber++;

                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                {
                  telnet_dataBuffer[telnet_packet_data_number_now][2 + ad_data_channel * 3 + 0] = datas[0][ad_data_channel * 4 + 1];
                  telnet_dataBuffer[telnet_packet_data_number_now][2 + ad_data_channel * 3 + 1] = datas[0][ad_data_channel * 4 + 2];
                  telnet_dataBuffer[telnet_packet_data_number_now][2 + ad_data_channel * 3 + 2] = datas[0][ad_data_channel * 4 + 3];
                }
//                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
//                {
//                  dataBuffer[2 + ad_data_channel * 3 + 0] = data2[ad_data_channel * 4 + 1];
//                  dataBuffer[2 + ad_data_channel * 3 + 1] = data2[ad_data_channel * 4 + 2];
//                  dataBuffer[2 + ad_data_channel * 3 + 2] = data2[ad_data_channel * 4 + 3];
//                }
                for(int accel_data_channel = 0; accel_data_channel < 3; accel_data_channel ++)
                {
                  telnet_dataBuffer[telnet_packet_data_number_now][2 + 8 * 3 + accel_data_channel * 2 + 0] = 0;
                  telnet_dataBuffer[telnet_packet_data_number_now][2 + 8 * 3 + accel_data_channel * 2 + 1] = 0;
                }
                telnet_dataBuffer[telnet_packet_data_number_now][2 + 8 * 3 + 3 * 2 + 0] = 0xC0;


                if((data_counter%telnet_data_number_max==0))
                {
                  if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART1)
                  {
                    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
                    {
                    }
                    if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)telnet_dataBuffer, telnet_packet_data_number_max*telnet_data_number_max) != HAL_OK)
                    {
                      Error_Handler();
                    }
                  }
                  if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART2)
                  {
                    while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
                    {
                    }
                    if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)telnet_dataBuffer, telnet_packet_data_number_max*telnet_data_number_max) != HAL_OK)
                    {
                      Error_Handler();
                    }
                  }
                  telnet_packet_data_number_now=0;
                }
                else
                {
                  telnet_packet_data_number_now++;
                }


//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//              }
//              else
//              {
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
              }


            }//FREESMARTEEG_OPENBCI_TELNET_PACKET

            if(FREESMARTEEG_OUT & FREESMARTEEG_OPENVIBE_TELNET_PACKET)
            {

//              int crc_pair_ok[uint8_ad_adc_number][4];

              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                int crc_ok = 1;
                for(int ad_data_channel_pair = 0; ad_data_channel_pair < 4; ad_data_channel_pair ++)
                {
//                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 1];
//                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 2];
//                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 3];

                  uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

//                  uint8_t crc1=((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
//                      |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)));
//                  uint8_t chan_0=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b01110000)>>4);
//                  uint8_t chan_1=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b01110000)>>4);
//                  print2_hex(chan_0,8);
//                  print2_symbol(':');
//                  print2_hex(crc8_,8);
//                  print2_symbol('-');
//                  print2_hex(crc1,8);
////                  print2_symbol(';');
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(!(((uint8_t)crc8_)
//                      -((uint8_t)crc1))),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)!(((uint8_t)crc8_)
//                     -((~((uint8_t)crc1))&0b00000111)),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(chan_0==ad_data_channel_pair*2),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(chan_1==(ad_data_channel_pair*2+1)),8);
//                  print2_symbol(';');
//                  if(
//                      ((!(((uint8_t)crc8_)
//                          -((uint8_t)crc1)))
//                     ||(!(((uint8_t)crc8_)
//                         -((~((uint8_t)crc1))&0b00000111)))
//                      )
//                  &&(
//                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
//                        ((chan_0==ad_data_channel_pair)))
//                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
//                      ((chan_1==(ad_data_channel_pair+1))))
//                  ))
                  if(
                      ((!(((uint8_t)crc8_)
                          -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                              |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                     ||(!(((uint8_t)crc8_)
                         -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                             |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                      )
                  &&(
                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                        ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b11110000)>>4)==ad_data_channel_pair*2)))
                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                      ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b11110000)>>4)==ad_data_channel_pair*2+1)))
                  ))
                  {
//                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                  }
                  else
                  {
//                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                    crc_ok = 0;
                  }

                }
                if(crc_ok)
                {
//                  print2_symbol('1');
                  HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
  //                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
                }
                else
                {
//                  print2_symbol('0');
                  HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
  //                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

                }
//                print2_symbol(';');
              }

              int ad_adc = 0;
//              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                int ad_data_channel = 0;
//                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                {
//                  print2_binary(datas[ad_adc][ad_data_channel * 4 + 0], 8);          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
//                  print2_symbol(';');
                  datas_out[ad_adc][ad_data_channel * 4 + 0]=datas[ad_adc][ad_data_channel * 4 + 1]&0b10000000;
                  datas_out[ad_adc][ad_data_channel * 4 + 1]=datas[ad_adc][ad_data_channel * 4 + 1]&0b01111111;
                  datas_out[ad_adc][ad_data_channel * 4 + 2]=datas[ad_adc][ad_data_channel * 4 + 2];
                  datas_out[ad_adc][ad_data_channel * 4 + 3]=datas[ad_adc][ad_data_channel * 4 + 3];

//                  const int sample_number_max = 240;
//                  int sample[sample_number_max];
//                  int sample_number_last = -1;
//                  int sample_average = 0;
//
//                  int sample_int_out;
//                  uint8_t sample_byte_out;
//                  double sample_int_byte_scale;
//                  int sample_int_byte_shift;

                  sample_int_out = (int)(datas_out[ad_adc][ad_data_channel * 4 + 0]<<24)
                          +(int)(datas_out[ad_adc][ad_data_channel * 4 + 1]<<16)
                          +(int)(datas_out[ad_adc][ad_data_channel * 4 + 2]<<8)
                          +(int)(datas_out[ad_adc][ad_data_channel * 4 + 3]<<0);
                  if(sample_number_now==sample_number_max)
                  {
                    for(int sample_count=0;sample_count<sample_number_now-1;sample_count++)
                    {
                      sample_int[sample_count]=sample_int[sample_count+1];
                      sample_byte[sample_count]=sample_byte[sample_count+1];
                    }
                  }
                  else
                  {
                    sample_number_now++;
                  }
                  sample_int[sample_number_now-1]=sample_int_out;

//                  if((test_counter%sample_number_max==0)||(test_counter<sample_number_max))
                  {
                    sample_sum=0;

                    int sample_max_new=sample_int[0];
                    int sample_min_new=sample_int[0];

                    for(int sample_count=0;sample_count<sample_number_now;sample_count++)
                    {
                      sample_sum+=sample_int[sample_count];
                      if(sample_max_new<sample_int[sample_count])
                      {
                        sample_max_new=sample_int[sample_count];
                      }
                      if(sample_min_new>sample_int[sample_count])
                      {
                        sample_min_new=sample_int[sample_count];
                      }
                    }
//                    sample_average=sample_sum/sample_number_now;

                    if(((sample_max_last<sample_max_new)||(sample_min_last>sample_min_new))||
                        ((sample_max_last>sample_max_new+(sample_max+sample_min)/2)
                            ||(sample_min_last<sample_min_new-(sample_max+sample_min)/2)))
                    {
                      sample_max=sample_max_new;
                      sample_min=sample_min_new;
                    }

                    sample_max_last=sample_max;
                    sample_min_last=sample_min;

                  }


//                  sample_int_byte_shift = (1<<8)/(1<<32);
//                  sample_byte_out = sample_int_out + sample_int_byte_shift;
//                  sample_int_byte_scale = 1;//(1<<8)/(1<<32);
//                  sample_byte_out = (uint8_t)(sample_int_out * sample_int_byte_scale);
                  sample_byte_out = (uint8_t)((sample_int_out-sample_min)*255/(sample_max-sample_min+1));
                  sample_byte[sample_number_now-1]=sample_byte_out;

//                  print2_symbol(sample_byte_out);
//                  print2_hex(sample_byte_out, 8);//          print2_symbol(';');
//                  print2_line();

                  if((test_counter%sample_number_max==0)||(test_counter<sample_number_max))
                  {
                    for(int sample_count=0;sample_count<sample_number_now;sample_count++)
                    {

                      sample_dataBuffer[sample_count]=sample_byte[sample_count];
                    }

                    while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
                    {
                    }
                    if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)sample_dataBuffer, sample_data_number) != HAL_OK)
                    {
                      Error_Handler();
                    }
                  }
                }
              }

            }//FREESMARTEEG_OPENVIBE_TELNET_PACKET


            if(FREESMARTEEG_OUT & FREESMARTEEG_CHANNELS_STATE)
            {
              if(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_PIN)
              {
//              int crc_pair_ok[uint8_ad_adc_number][4];

                for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
                {
                  int crc_ok = 1;
                  for(int ad_data_channel_pair = 0; ad_data_channel_pair < 4; ad_data_channel_pair ++)
                  {
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 1];
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 2];
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 3];

                    uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

  //                  uint8_t crc1=((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
  //                      |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)));
  //                  uint8_t chan_0=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b01110000)>>4);
  //                  uint8_t chan_1=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b01110000)>>4);
  //                  print2_hex(chan_0,8);
  //                  print2_symbol(':');
  //                  print2_hex(crc8_,8);
  //                  print2_symbol('-');
  //                  print2_hex(crc1,8);
  ////                  print2_symbol(';');
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(!(((uint8_t)crc8_)
  //                      -((uint8_t)crc1))),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)!(((uint8_t)crc8_)
  //                     -((~((uint8_t)crc1))&0b00000111)),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_0==ad_data_channel_pair*2),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_1==(ad_data_channel_pair*2+1)),8);
  //                  print2_symbol(';');
  //                  if(
  //                      ((!(((uint8_t)crc8_)
  //                          -((uint8_t)crc1)))
  //                     ||(!(((uint8_t)crc8_)
  //                         -((~((uint8_t)crc1))&0b00000111)))
  //                      )
  //                  &&(
  //                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
  //                        ((chan_0==ad_data_channel_pair)))
  //                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
  //                      ((chan_1==(ad_data_channel_pair+1))))
  //                  ))
                    if(
                        ((!(((uint8_t)crc8_)
                            -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                                |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                       ||(!(((uint8_t)crc8_)
                           -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                               |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                        )
                    &&(
                      (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                          ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b11110000)>>4)==ad_data_channel_pair*2)))
                    &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                        ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b11110000)>>4)==ad_data_channel_pair*2+1)))
                    ))
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                    }
                    else
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                      crc_ok = 0;
                    }

                  }
                  if(crc_ok)
                  {
  //                  print2_symbol('1');
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
  //                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
                  }
                  else
                  {
  //                  print2_symbol('0');
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
  //                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

                  }
  //                print2_symbol(';');
                }
              }

              if(data_counter%sample_number_max==0)
              {
                sample_number_now=sample_number_max;
              }
              else
              {
                sample_number_now=data_counter%sample_number_max;
              }

//              int ad_adc = 0;
              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
//                int ad_data_channel = 0;
                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                {
//                  print2_binary(datas[ad_adc][ad_data_channel * 4 + 0], 8);          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
//                  print2_symbol(';');
//                  datas_out[ad_adc][ad_data_channel * 4 + 0]=datas[ad_adc][ad_data_channel * 4 + 1]&0b10000000;
//                  datas_out[ad_adc][ad_data_channel * 4 + 1]=datas[ad_adc][ad_data_channel * 4 + 1]&0b01111111;
//                  datas_out[ad_adc][ad_data_channel * 4 + 2]=datas[ad_adc][ad_data_channel * 4 + 2];
//                  datas_out[ad_adc][ad_data_channel * 4 + 3]=datas[ad_adc][ad_data_channel * 4 + 3];

//                  const int sample_number_max = 240;
//                  int sample[sample_number_max];
//                  int sample_number_last = -1;
//                  int sample_average = 0;
//
//                  int sample_int_out;
//                  uint8_t sample_byte_out;
//                  double sample_int_byte_scale;
//                  int sample_int_byte_shift;

//                  sample_int_out = (int)((datas_out[ad_adc][ad_data_channel * 4 + 0]<<24)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 1]<<16)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 2]<<8)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 3]<<0));

//                  int32_t newInt = (byteBuffer[2] << 24) | (byteBuffer[1] << 16) | byteBuffer[0] << 8;
//                  // depending on most significant byte, set positive or negative value
//                  if ((newInt & 0x00008000) > 0) {
//                    newInt |= 0x000000FF;
//                  } else {
//                    newInt &= 0xFFFFFF00;
//                  }

//                  sample_int_out = (datas[ad_adc][ad_data_channel * 4 + 3]<<24)
//                          |(datas[ad_adc][ad_data_channel * 4 + 2]<<16)
//                          |(datas[ad_adc][ad_data_channel * 4 + 1]<<8);
                  sample_int_out = (datas[ad_adc][ad_data_channel * 4 + 1]<<24)
                          |(datas[ad_adc][ad_data_channel * 4 + 2]<<16)
                          |(datas[ad_adc][ad_data_channel * 4 + 3]<<8);
                  if ((sample_int_out & 0x00008000) > 0) {
                    sample_int_out |= 0x000000FF;
                  } else {
                    sample_int_out &= 0xFFFFFF00;
                  }
//                  sample_int_out >>= 8;
//
//                  sample_int_out = (datas[ad_adc][ad_data_channel * 4 + 3]<<16)
//                          |(datas[ad_adc][ad_data_channel * 4 + 2]<<8)
//                          |(datas[ad_adc][ad_data_channel * 4 + 1]<<0);

//                  if(samples_number_now[ad_adc][ad_data_channel]==sample_number_max)
//                  {
//                    for(int sample_count=0;sample_count<samples_number_now[ad_adc][ad_data_channel]-1;sample_count++)
//                    {
//                      samples_int[ad_adc][ad_data_channel][sample_count]=samples_int[ad_adc][ad_data_channel][sample_count+1];
//                      samples_byte[ad_adc][ad_data_channel][sample_count]=samples_byte[ad_adc][ad_data_channel][sample_count+1];
//                    }
//                  }
//                  else
//                  {
//                    samples_number_now[ad_adc][ad_data_channel]++;
//                  }
//                  samples_int[ad_adc][ad_data_channel][sample_number_now-1]=sample_int_out;

//                  if((data_counter%sample_number_max==0))//||(data_counter<sample_number_max))
                  {
//                    sample_sum=0;

//                    int32_t sample_max_new=samples_int[ad_adc][ad_data_channel][0];
//                    int32_t sample_min_new=samples_int[ad_adc][ad_data_channel][0];
//
//                    for(int sample_count=0;sample_count<sample_number_now;sample_count++)
//                    {
////                      samples_sum+=samples_int[ad_adc][ad_data_channel][sample_count];
//                      if(sample_max_new<samples_int[ad_adc][ad_data_channel][sample_count])
//                      {
//                        sample_max_new=samples_int[ad_adc][ad_data_channel][sample_count];
//                      }
//                      if(sample_min_new>samples_int[ad_adc][ad_data_channel][sample_count])
//                      {
//                        sample_min_new=samples_int[ad_adc][ad_data_channel][sample_count];
//                      }
//                    }
////                    sample_average=sample_sum/sample_number_now;
//
////                    if(((sample_max_last<sample_max_new)||(sample_min_last>sample_min_new))||
////                        ((sample_max_last>sample_max_new+(sample_max+sample_min)/2)
////                            ||(sample_min_last<sample_min_new-(sample_max+sample_min)/2)))
//                    if(((samples_max[ad_adc][ad_data_channel]<sample_max_new)||
//                        (samples_min[ad_adc][ad_data_channel]>sample_min_new))||
//                        ((samples_max[ad_adc][ad_data_channel]>sample_max_new+
//                            (samples_max[ad_adc][ad_data_channel]+samples_min[ad_adc][ad_data_channel])/20)
//                            ||(samples_min[ad_adc][ad_data_channel]<sample_min_new-
//                                (samples_max[ad_adc][ad_data_channel]+samples_min[ad_adc][ad_data_channel])/20)))
//                    {
//                      samples_max[ad_adc][ad_data_channel]=sample_max_new;
//                      samples_min[ad_adc][ad_data_channel]=sample_min_new;
//                    }

                    if(data_counter==1)
                    {
                      samples_max[ad_adc][ad_data_channel]=sample_int_out;
                      samples_min[ad_adc][ad_data_channel]=sample_int_out;
                    }
                    if(data_counter%sample_number_max==1)
                    {
//                      samples_max_new[ad_adc][ad_data_channel]=samples_max[ad_adc][ad_data_channel];
//                      samples_min_new[ad_adc][ad_data_channel]=samples_min[ad_adc][ad_data_channel];
                      samples_max_new[ad_adc][ad_data_channel]=sample_int_out;
                      samples_min_new[ad_adc][ad_data_channel]=sample_int_out;
                    }
                    if(samples_max_new[ad_adc][ad_data_channel]<sample_int_out)
                    {
                      samples_max_new[ad_adc][ad_data_channel]=sample_int_out;
                    }
                    if(samples_min_new[ad_adc][ad_data_channel]>sample_int_out)
                    {
                      samples_min_new[ad_adc][ad_data_channel]=sample_int_out;
                    }
                    if(data_counter%sample_number_max==0)
                    {
//                    sample_average=sample_sum/sample_number_now;

//                    if(((sample_max_last<sample_max_new)||(sample_min_last>sample_min_new))||
//                        ((sample_max_last>sample_max_new+(sample_max+sample_min)/2)
//                            ||(sample_min_last<sample_min_new-(sample_max+sample_min)/2)))
//                      if(((samples_max[ad_adc][ad_data_channel]<sample_max_new)||
//                          (samples_min[ad_adc][ad_data_channel]>sample_min_new))||
//                          ((samples_max[ad_adc][ad_data_channel]>sample_max_new+
//                              (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])/20)
//                              ||(samples_min[ad_adc][ad_data_channel]<sample_min_new-
//                                  (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])/20)))
//                      {
//                        samples_max[ad_adc][ad_data_channel]=sample_max_new;
//                        samples_min[ad_adc][ad_data_channel]=sample_min_new;
//                      }
                      if(((samples_max[ad_adc][ad_data_channel]<samples_max_new[ad_adc][ad_data_channel]))
//                          ||((samples_max[ad_adc][ad_data_channel]>samples_max_new[ad_adc][ad_data_channel]+
//                              (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])/20))
                              )
                      {
                        samples_max[ad_adc][ad_data_channel]=samples_max_new[ad_adc][ad_data_channel];
                      }
                      if(((samples_min[ad_adc][ad_data_channel]>samples_min_new[ad_adc][ad_data_channel]))
//                          ||((samples_min[ad_adc][ad_data_channel]<samples_min_new[ad_adc][ad_data_channel]-
//                                  (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])/20))
                                  )
                      {
                        samples_min[ad_adc][ad_data_channel]=samples_min_new[ad_adc][ad_data_channel];
                      }
                    }
//                    samples_max_last[ad_adc][ad_data_channel]=sample_max;
//                    samples_min_last[ad_adc][ad_data_channel]=sample_min;

//                    int32_t sample_max_min_dif=(sample_max-sample_min);


                  }

//                  sample_int_byte_shift = (1<<8)/(1<<32);
//                  sample_byte_out = sample_int_out + sample_int_byte_shift;
//                  sample_int_byte_scale = 1;//(1<<8)/(1<<32);
//                  sample_byte_out = (uint8_t)(sample_int_out * sample_int_byte_scale);

//                  sample_byte_out = (uint8_t)(((int64_t)sample_int_out-(int64_t)samples_min[ad_adc][ad_data_channel])*255/((int64_t)samples_max[ad_adc][ad_data_channel]-(int64_t)samples_min[ad_adc][ad_data_channel]+1));
//                  samples_byte[ad_adc][ad_data_channel][sample_number_now-1]=sample_byte_out;

//                  sample_byte_out = (uint8_t)(((int64_t)sample_int_out-(int64_t)sample_min)*255/((int64_t)sample_max-(int64_t)sample_min+1));
//                  sample_byte[sample_number_now-1]=sample_byte_out;


//                  print2_symbol(datas_out[ad_adc][ad_data_channel * 4 + 3]);
//                  print2_symbol((uint8_t)((sample_int_out/(1<<0)+(255/4)*(1))));
//                  print2_symbol((uint8_t)(((sample_int_out-1000000)/1000000)));
                  if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART1)
                  {
//                    print_symbol(sample_byte_out);
                  }
//                  if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART2)
//                  {
//                    print2_hex(sample_byte_out, 8);          print2_symbol(';');
//                    print2_hex(sample_int_out, 32);          print2_symbol(';');
//                    print2_hex(sample_min, 32);          print2_symbol(';');
//                    print2_hex(sample_max, 32);          print2_symbol(';');
//                    print2_line();
//                  }
                  if((data_counter%sample_number_max==0))
                  {

//                    print2_hex(sample_byte_out, 8);          print2_symbol(';');
//                    print2_hex(sample_int_out, 32);          print2_symbol(';');
//                    print2_hex(sample_min, 32);          print2_symbol(';');
//                    print2_hex(sample_max, 32);          print2_symbol(';');
//                    print2_hex(sample_max-sample_min, 32);          print2_symbol(';');
//                    print2_line();

//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])>>16, 8);          print2_symbol(';');
//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])>>8, 16);          print2_symbol(';');
//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])>>8, 24);          print2_symbol(';');
//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel]), 32);          print2_symbol(';');
//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel]), 24);          print2_symbol(';');

//                    print_symbol(sample_byte_out);

                    samples_max_min_dif[ad_adc][ad_data_channel]=
                        (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel]);

                    if(data_counter==sample_number_max)
                    {
                      sample_max_min_dif_max=samples_max_min_dif[ad_adc][ad_data_channel];
                      sample_max_min_dif_min=samples_max_min_dif[ad_adc][ad_data_channel];
                    }
                    else
                    {
                      if(sample_max_min_dif_max<samples_max_min_dif[ad_adc][ad_data_channel])
                      {
                        sample_max_min_dif_max=samples_max_min_dif[ad_adc][ad_data_channel];
                      }
                      if(sample_max_min_dif_min>samples_max_min_dif[ad_adc][ad_data_channel])
                      {
                        sample_max_min_dif_min=samples_max_min_dif[ad_adc][ad_data_channel];
                      }
                    }
                    print2_hex(samples_max_min_dif[ad_adc][ad_data_channel], 32);          print2_symbol(';');
                    print2_hex(sample_max_min_dif_min, 32);          print2_symbol(';');
                    print2_hex(sample_max_min_dif_max, 32);          print2_symbol(';');
                    print2_symbol(';');

                  }

                }
              }
              if((data_counter%sample_number_max==0))
              {
//                print2_symbol(';');
//                print2_hex(sample_max_min_dif_max>>16, 8);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min>>16, 8);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_max>>8, 16);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min>>8, 16);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_max>>8, 24);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min>>8, 24);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_max, 24);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min, 24);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_max, 32);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min, 32);          print2_symbol(';');
                print2_line();

                const uint32_t uint8_data_number = 2 + 8 * 3 * 4 + 3 * 2 + 1;
//                const uint32_t uint8_data_number = 2 + 8 * 3 + 8 * 3 + 3 * 2 + 1;
                uint8_t dataBuffer[uint8_data_number];

                dataBuffer[0] = 0xA0;
//                dataBuffer[0] = 0x00;
                dataBuffer[1] = ui8SampleNumber++;

                for(int ad_adc = 0; ad_adc < 4; ad_adc ++)
                {
                  for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                  {
                    int32_t sample_max_min_dif=samples_max_min_dif[ad_adc][ad_data_channel];
                    /*convert to long rainbow RGB*/
                    uint8_t r;
                    uint8_t g;
                    uint8_t b;
                    double f=(double)sample_max_min_dif/(double)0xffffff;
                    double f_min=(double)sample_max_min_dif_min/(double)0xffffff;
                    double f_max=(double)sample_max_min_dif_max/(double)0xffffff;
//                    double f=(double)(sample_max_min_dif>>8)/(double)0xffff;
//                    double f=(double)(sample_max_min_dif>>16)/(double)0xff;
                    f=(log(f)-log(f_min))/(log(f_max)-log(f_min));
                    double a=(1-f)/0.2;
                    int  X=floor(a);
                    int  Y=floor(255*(a-X));
                    switch(X)
                    {
                        case 0: r=255;g=Y;b=0;break;
                        case 1: r=255-Y;g=255;b=0;break;
                        case 2: r=0;g=255;b=Y;break;
                        case 3: r=0;g=255-Y;b=255;break;
                        case 4: r=Y;g=0;b=255;break;
                        case 5: r=255;g=0;b=255;break;
                    }

                    dataBuffer[2 + ad_adc * 24 + ad_data_channel * 3 + 0] = r;
                    dataBuffer[2 + ad_adc * 24 + ad_data_channel * 3 + 1] = g;
                    dataBuffer[2 + ad_adc * 24 + ad_data_channel * 3 + 2] = b;
                  }
                }
//                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
//                {
//                  dataBuffer[2 + ad_data_channel * 3 + 0] = data2[ad_data_channel * 4 + 1];
//                  dataBuffer[2 + ad_data_channel * 3 + 1] = data2[ad_data_channel * 4 + 2];
//                  dataBuffer[2 + ad_data_channel * 3 + 2] = data2[ad_data_channel * 4 + 3];
//                }
                for(int accel_data_channel = 0; accel_data_channel < 3; accel_data_channel ++)
                {
                  dataBuffer[2 + 8 * 3 * 4 + accel_data_channel * 2 + 0] = 0;
                  dataBuffer[2 + 8 * 3 * 4 + accel_data_channel * 2 + 1] = 0;
                }
                dataBuffer[2 + 8 * 3 * 4 + 3 * 2 + 0] = 0xC0;

//                dataBuffer[2]=100;
//                dataBuffer[3]=101;
//                dataBuffer[4]=102;

//                for(int data_out = 0; data_out < uint8_data_number; data_out ++)
//                {
//                  print2_hex(dataBuffer[data_out], 8);          print2_symbol(';');
//                }
//                print2_line();

//                if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART1)
                {
                  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
                  {
                  }
                  if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
                  {
                    Error_Handler();
                  }
                }
//                if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART2)
//                {
//                  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
//                  {
//                  }
//                  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
//                  {
//                    Error_Handler();
//                  }
//                }

              }

            }//FREESMARTEEG_OUT & FREESMARTEEG_СHANNELS_STATE

            if(FREESMARTEEG_OUT & FREESMARTEEG_FFT_DISPLAY)
            {
              if(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_PIN)
              {
//              int crc_pair_ok[uint8_ad_adc_number][4];

                for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
                {
                  int crc_ok = 1;
                  int crc_something_ok = 0;
                  for(int ad_data_channel_pair = 0; ad_data_channel_pair < 4; ad_data_channel_pair ++)
                  {
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 1];
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 2];
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 3];

                    uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

  //                  uint8_t crc1=((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
  //                      |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)));
  //                  uint8_t chan_0=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b01110000)>>4);
  //                  uint8_t chan_1=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b01110000)>>4);
  //                  print2_hex(chan_0,8);
  //                  print2_symbol(':');
  //                  print2_hex(crc8_,8);
  //                  print2_symbol('-');
  //                  print2_hex(crc1,8);
  ////                  print2_symbol(';');
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(!(((uint8_t)crc8_)
  //                      -((uint8_t)crc1))),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)!(((uint8_t)crc8_)
  //                     -((~((uint8_t)crc1))&0b00000111)),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_0==ad_data_channel_pair*2),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_1==(ad_data_channel_pair*2+1)),8);
  //                  print2_symbol(';');
  //                  if(
  //                      ((!(((uint8_t)crc8_)
  //                          -((uint8_t)crc1)))
  //                     ||(!(((uint8_t)crc8_)
  //                         -((~((uint8_t)crc1))&0b00000111)))
  //                      )
  //                  &&(
  //                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
  //                        ((chan_0==ad_data_channel_pair)))
  //                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
  //                      ((chan_1==(ad_data_channel_pair+1))))
  //                  ))
                    if(
                        ((!(((uint8_t)crc8_)
                            -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                                |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                       ||(!(((uint8_t)crc8_)
                           -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                               |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                        )
                        &&(
                          (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                              ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]
                                                &((FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_NOERRORS)?0b11110000:0b01110000))>>4)==
                                                    ad_data_channel_pair*2)))
                        &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                            ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]
                                              &((FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_NOERRORS)?0b11110000:0b01110000))>>4)==
                                                  ad_data_channel_pair*2+1)))
                        ))
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                      crc_something_ok = 1;
                    }
                    else
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                      crc_ok = 0;
                    }

                  }
                  if((crc_ok)||((crc_something_ok)&&(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_SOMETHING)))
//                  if(crc_ok)
                  {
  //                  print2_symbol('1');
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
  //                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
                  }
                  else
                  {
  //                  print2_symbol('0');
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
  //                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

                  }
  //                print2_symbol(';');
                }
              }

//              if(data_counter%sample_number_max==0)
//              {
//                sample_number_now=sample_number_max;
//              }
//              else
//              {
//                sample_number_now=data_counter%sample_number_max;
//              }

//              int ad_adc = 0;
              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
//                int ad_data_channel = 0;
                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                {
//                  print2_binary(datas[ad_adc][ad_data_channel * 4 + 0], 8);          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
//                  print2_symbol(';');
//                  datas_out[ad_adc][ad_data_channel * 4 + 0]=datas[ad_adc][ad_data_channel * 4 + 1]&0b10000000;
//                  datas_out[ad_adc][ad_data_channel * 4 + 1]=datas[ad_adc][ad_data_channel * 4 + 1]&0b01111111;
//                  datas_out[ad_adc][ad_data_channel * 4 + 2]=datas[ad_adc][ad_data_channel * 4 + 2];
//                  datas_out[ad_adc][ad_data_channel * 4 + 3]=datas[ad_adc][ad_data_channel * 4 + 3];

//                  const int sample_number_max = 240;
//                  int sample[sample_number_max];
//                  int sample_number_last = -1;
//                  int sample_average = 0;
//
//                  int sample_int_out;
//                  uint8_t sample_byte_out;
//                  double sample_int_byte_scale;
//                  int sample_int_byte_shift;

//                  sample_int_out = (int)((datas_out[ad_adc][ad_data_channel * 4 + 0]<<24)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 1]<<16)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 2]<<8)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 3]<<0));

//                  int32_t newInt = (byteBuffer[2] << 24) | (byteBuffer[1] << 16) | byteBuffer[0] << 8;
//                  // depending on most significant byte, set positive or negative value
//                  if ((newInt & 0x00008000) > 0) {
//                    newInt |= 0x000000FF;
//                  } else {
//                    newInt &= 0xFFFFFF00;
//                  }

//                  sample_int_out = (datas[ad_adc][ad_data_channel * 4 + 3]<<24)
//                          |(datas[ad_adc][ad_data_channel * 4 + 2]<<16)
//                          |(datas[ad_adc][ad_data_channel * 4 + 1]<<8);
                  sample_int_out = (datas[ad_adc][ad_data_channel * 4 + 1]<<24)
                          |(datas[ad_adc][ad_data_channel * 4 + 2]<<16)
                          |(datas[ad_adc][ad_data_channel * 4 + 3]<<8);
                  if ((sample_int_out & 0x00008000) > 0) {
                    sample_int_out |= 0x000000FF;
                  } else {
                    sample_int_out &= 0xFFFFFF00;
                  }
//                  sample_int_out >>= 8;
//
//                  sample_int_out = (datas[ad_adc][ad_data_channel * 4 + 3]<<16)
//                          |(datas[ad_adc][ad_data_channel * 4 + 2]<<8)
//                          |(datas[ad_adc][ad_data_channel * 4 + 1]<<0);

//                  if(samples_number_now[ad_adc][ad_data_channel]==sample_number_max)
//                  {
//                    for(int sample_count=0;sample_count<samples_number_now[ad_adc][ad_data_channel]-1;sample_count++)
//                    {
//                      samples_int[ad_adc][ad_data_channel][sample_count]=samples_int[ad_adc][ad_data_channel][sample_count+1];
//                      samples_byte[ad_adc][ad_data_channel][sample_count]=samples_byte[ad_adc][ad_data_channel][sample_count+1];
//                    }
//                  }
//                  else
//                  {
//                    samples_number_now[ad_adc][ad_data_channel]++;
//                  }
                  if((ad_adc==0)&&(ad_data_channel==0))
//                  if((ad_adc==1)&&(ad_data_channel==4))
                  {
                    if(sample_number_now==sample_number_max)
                    {
                      for(int sample_count=0;sample_count<sample_number_now-1;sample_count++)
                      {
                        samples_int_part[sample_count]=samples_int_part[sample_count+1];
                      }
                    }
                    else
                    {
                      sample_number_now++;
                    }
                    samples_int_part[sample_number_now-1]=sample_int_out;
                  }
//                  samples_int[ad_adc][ad_data_channel][sample_number_now-1]=sample_int_out;

                  if(0)
//                  if((data_counter%sample_number_max==0))//||(data_counter<sample_number_max))
                  {
//                    sample_sum=0;

//                    int32_t sample_max_new=samples_int[ad_adc][ad_data_channel][0];
//                    int32_t sample_min_new=samples_int[ad_adc][ad_data_channel][0];
//
//                    for(int sample_count=0;sample_count<sample_number_now;sample_count++)
//                    {
////                      samples_sum+=samples_int[ad_adc][ad_data_channel][sample_count];
//                      if(sample_max_new<samples_int[ad_adc][ad_data_channel][sample_count])
//                      {
//                        sample_max_new=samples_int[ad_adc][ad_data_channel][sample_count];
//                      }
//                      if(sample_min_new>samples_int[ad_adc][ad_data_channel][sample_count])
//                      {
//                        sample_min_new=samples_int[ad_adc][ad_data_channel][sample_count];
//                      }
//                    }
////                    sample_average=sample_sum/sample_number_now;
//
////                    if(((sample_max_last<sample_max_new)||(sample_min_last>sample_min_new))||
////                        ((sample_max_last>sample_max_new+(sample_max+sample_min)/2)
////                            ||(sample_min_last<sample_min_new-(sample_max+sample_min)/2)))
//                    if(((samples_max[ad_adc][ad_data_channel]<sample_max_new)||
//                        (samples_min[ad_adc][ad_data_channel]>sample_min_new))||
//                        ((samples_max[ad_adc][ad_data_channel]>sample_max_new+
//                            (samples_max[ad_adc][ad_data_channel]+samples_min[ad_adc][ad_data_channel])/20)
//                            ||(samples_min[ad_adc][ad_data_channel]<sample_min_new-
//                                (samples_max[ad_adc][ad_data_channel]+samples_min[ad_adc][ad_data_channel])/20)))
//                    {
//                      samples_max[ad_adc][ad_data_channel]=sample_max_new;
//                      samples_min[ad_adc][ad_data_channel]=sample_min_new;
//                    }

                    if(data_counter==1)
                    {
                      samples_max[ad_adc][ad_data_channel]=sample_int_out;
                      samples_min[ad_adc][ad_data_channel]=sample_int_out;
                    }
                    if(data_counter%sample_number_max==1)
                    {
//                      samples_max_new[ad_adc][ad_data_channel]=samples_max[ad_adc][ad_data_channel];
//                      samples_min_new[ad_adc][ad_data_channel]=samples_min[ad_adc][ad_data_channel];
                      samples_max_new[ad_adc][ad_data_channel]=sample_int_out;
                      samples_min_new[ad_adc][ad_data_channel]=sample_int_out;
                    }
                    if(samples_max_new[ad_adc][ad_data_channel]<sample_int_out)
                    {
                      samples_max_new[ad_adc][ad_data_channel]=sample_int_out;
                    }
                    if(samples_min_new[ad_adc][ad_data_channel]>sample_int_out)
                    {
                      samples_min_new[ad_adc][ad_data_channel]=sample_int_out;
                    }
                    if(data_counter%sample_number_max==0)
                    {
//                    sample_average=sample_sum/sample_number_now;

//                    if(((sample_max_last<sample_max_new)||(sample_min_last>sample_min_new))||
//                        ((sample_max_last>sample_max_new+(sample_max+sample_min)/2)
//                            ||(sample_min_last<sample_min_new-(sample_max+sample_min)/2)))
//                      if(((samples_max[ad_adc][ad_data_channel]<sample_max_new)||
//                          (samples_min[ad_adc][ad_data_channel]>sample_min_new))||
//                          ((samples_max[ad_adc][ad_data_channel]>sample_max_new+
//                              (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])/20)
//                              ||(samples_min[ad_adc][ad_data_channel]<sample_min_new-
//                                  (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])/20)))
//                      {
//                        samples_max[ad_adc][ad_data_channel]=sample_max_new;
//                        samples_min[ad_adc][ad_data_channel]=sample_min_new;
//                      }
                      if(((samples_max[ad_adc][ad_data_channel]<samples_max_new[ad_adc][ad_data_channel]))
//                          ||((samples_max[ad_adc][ad_data_channel]>samples_max_new[ad_adc][ad_data_channel]+
//                              (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])/20))
                              )
                      {
                        samples_max[ad_adc][ad_data_channel]=samples_max_new[ad_adc][ad_data_channel];
                      }
                      if(((samples_min[ad_adc][ad_data_channel]>samples_min_new[ad_adc][ad_data_channel]))
//                          ||((samples_min[ad_adc][ad_data_channel]<samples_min_new[ad_adc][ad_data_channel]-
//                                  (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])/20))
                                  )
                      {
                        samples_min[ad_adc][ad_data_channel]=samples_min_new[ad_adc][ad_data_channel];
                      }
                    }
//                    samples_max_last[ad_adc][ad_data_channel]=sample_max;
//                    samples_min_last[ad_adc][ad_data_channel]=sample_min;

//                    int32_t sample_max_min_dif=(sample_max-sample_min);


                  }

//                  sample_int_byte_shift = (1<<8)/(1<<32);
//                  sample_byte_out = sample_int_out + sample_int_byte_shift;
//                  sample_int_byte_scale = 1;//(1<<8)/(1<<32);
//                  sample_byte_out = (uint8_t)(sample_int_out * sample_int_byte_scale);

//                  sample_byte_out = (uint8_t)(((int64_t)sample_int_out-(int64_t)samples_min[ad_adc][ad_data_channel])*255/((int64_t)samples_max[ad_adc][ad_data_channel]-(int64_t)samples_min[ad_adc][ad_data_channel]+1));
//                  samples_byte[ad_adc][ad_data_channel][sample_number_now-1]=sample_byte_out;

//                  sample_byte_out = (uint8_t)(((int64_t)sample_int_out-(int64_t)sample_min)*255/((int64_t)sample_max-(int64_t)sample_min+1));
//                  sample_byte[sample_number_now-1]=sample_byte_out;


//                  print2_symbol(datas_out[ad_adc][ad_data_channel * 4 + 3]);
//                  print2_symbol((uint8_t)((sample_int_out/(1<<0)+(255/4)*(1))));
//                  print2_symbol((uint8_t)(((sample_int_out-1000000)/1000000)));
                  if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART1)
                  {
//                    print_symbol(sample_byte_out);
                  }
//                  if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART2)
//                  {
//                    print2_hex(sample_byte_out, 8);          print2_symbol(';');
//                    print2_hex(sample_int_out, 32);          print2_symbol(';');
//                    print2_hex(sample_min, 32);          print2_symbol(';');
//                    print2_hex(sample_max, 32);          print2_symbol(';');
//                    print2_line();
//                  }
                  if(0)
                  if((data_counter%sample_number_max==0))
                  {

//                    print2_hex(sample_byte_out, 8);          print2_symbol(';');
//                    print2_hex(sample_int_out, 32);          print2_symbol(';');
//                    print2_hex(sample_min, 32);          print2_symbol(';');
//                    print2_hex(sample_max, 32);          print2_symbol(';');
//                    print2_hex(sample_max-sample_min, 32);          print2_symbol(';');
//                    print2_line();

//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])>>16, 8);          print2_symbol(';');
//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])>>8, 16);          print2_symbol(';');
//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel])>>8, 24);          print2_symbol(';');
//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel]), 32);          print2_symbol(';');
//                    print2_hex((samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel]), 24);          print2_symbol(';');

//                    print_symbol(sample_byte_out);

                    samples_max_min_dif[ad_adc][ad_data_channel]=
                        (samples_max[ad_adc][ad_data_channel]-samples_min[ad_adc][ad_data_channel]);

                    if(data_counter==sample_number_max)
                    {
                      sample_max_min_dif_max=samples_max_min_dif[ad_adc][ad_data_channel];
                      sample_max_min_dif_min=samples_max_min_dif[ad_adc][ad_data_channel];
                    }
                    else
                    {
                      if(sample_max_min_dif_max<samples_max_min_dif[ad_adc][ad_data_channel])
                      {
                        sample_max_min_dif_max=samples_max_min_dif[ad_adc][ad_data_channel];
                      }
                      if(sample_max_min_dif_min>samples_max_min_dif[ad_adc][ad_data_channel])
                      {
                        sample_max_min_dif_min=samples_max_min_dif[ad_adc][ad_data_channel];
                      }
                    }
//                    print2_hex(samples_max_min_dif[ad_adc][ad_data_channel], 32);          print2_symbol(';');
//                    print2_hex(sample_max_min_dif_min, 32);          print2_symbol(';');
//                    print2_hex(sample_max_min_dif_max, 32);          print2_symbol(';');
//                    print2_symbol(';');

                  }

                }
              }
              if((data_counter%sample_freq_fft==0))
//              if((data_counter%sample_number_max==0))
              {

                uint32_t FFT_Length = (int)(sample_number_max/2);
//                uint32_t FFT_Length = 64;//sample_number_max/2;
//                arm_cfft_radix4_instance_q31  FFT_Q31_struct;
//                arm_cfft_radix4_instance_q15  FFT_Q15_struct;
                arm_cfft_instance_q31  FFT_Q31_struct;
                FFT_Q31_struct.fftLen = FFT_Length;
                FFT_Q31_struct.pTwiddle = twiddleCoef_1024_q31;
                FFT_Q31_struct.pBitRevTable = armBitRevTable;
                FFT_Q31_struct.bitRevLength = 1024;

                arm_cfft_instance_q15  FFT_Q15_struct;
                FFT_Q15_struct.fftLen = FFT_Length;
                FFT_Q15_struct.pTwiddle = twiddleCoef_1024_q15;
                FFT_Q15_struct.pBitRevTable = armBitRevTable;
                FFT_Q15_struct.bitRevLength = 1024;

                q31_t maxValue;    /* Max FFT value is stored here */
                uint32_t maxIndex;    /* Index in Output array where max value is */

                uint32_t index_fill_input_buffer, index_fill_output_buffer, index_fill_adc_buffer = 0;
                uint32_t duration_us = 0x00;

//                _uhADCxConvertedValue = ;

//                _aADC1ConvertedValue_s[index_fill_adc_buffer] = _uhADCxConvertedValue;
            ///    TIM2_Config();
//              }
//              last_index_fill_adc_buffer=index_fill_adc_buffer;
                for (int index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer += 2)
                {
//                  FFT_Input_Q31_f[(uint16_t)index_fill_input_buffer] = (float32_t)samples_int[1][4][index_fill_input_buffer];//_uhADCxConvertedValue;// / (float32_t)4096.0;
//                  /* Imaginary part */
//                  FFT_Input_Q31_f[(uint16_t)(index_fill_input_buffer + 1)] = 0;

                  aFFT_Input_Q31[(uint16_t)index_fill_input_buffer] = samples_int_part[index_fill_input_buffer];//_uhADCxConvertedValue;// / (float32_t)4096.0;
//                  aFFT_Input_Q31[(uint16_t)index_fill_input_buffer] = samples_int[1][4][index_fill_input_buffer];//_uhADCxConvertedValue;// / (float32_t)4096.0;
                  /* Imaginary part */
                  aFFT_Input_Q31[(uint16_t)(index_fill_input_buffer + 1)] = 0;

//                  FFT_Input_Q15_f[(uint16_t)index_fill_input_buffer] = (float32_t)samples_int[1][4][index_fill_input_buffer];//_uhADCxConvertedValue;// / (float32_t)4096.0;
//                  /* Imaginary part */
//                  FFT_Input_Q15_f[(uint16_t)(index_fill_input_buffer + 1)] = 0;

//                  aFFT_Input_Q15[(uint16_t)index_fill_input_buffer] = (int16_t)(samples_int_part[index_fill_input_buffer]>>8);//_uhADCxConvertedValue;// / (float32_t)4096.0;
//                  /* Imaginary part */
//                  aFFT_Input_Q15[(uint16_t)(index_fill_input_buffer + 1)] = 0;

              ///    TIM2_Config();
                }

//                arm_float_to_q31((float32_t *)&FFT_Input_Q31_f[0], (q31_t *)&aFFT_Input_Q31[0], FFT_Length*2);
//                arm_float_to_q15((float32_t *)&FFT_Input_Q15_f[0], (q15_t *)&aFFT_Input_Q15[0], FFT_Length*2);

                /* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
//                arm_cfft_radix4_init_q31(&FFT_Q31_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);
//                arm_cfft_radix4_init_q15(&FFT_Q15_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);
//                arm_cfft_q31(&FFT_Q31_struct, aFFT_Input_Q31, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);
                arm_cfft_q31(&FFT_Q31_struct, aFFT_Input_Q31, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);
//                arm_cfft_q15(&FFT_Q15_struct, aFFT_Input_Q15, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG);
//                if ( arm_cfft_radix4_init_q31 ( &FFT_Q31_struct, FFT_Length, FFT_INVERSE_FLAG, FFT_Normal_OUTPUT_FLAG) != ARM_MATH_SUCCESS )
//                     return 1;

              //  TimerCount_Start();
//                arm_cfft_radix4_q31(&FFT_Q31_struct, aFFT_Input_Q31);
//                arm_cfft_radix4_q15(&FFT_Q15_struct, aFFT_Input_Q15);
              //  TimerCount_Stop(nb_cycles);

              ///  GUI_Clear();
              ///  LCD_OUTPUT_Cycles(5, 305, nb_cycles);
              //  duration_us = (uint32_t)(((uint64_t)US_IN_SECOND * (nb_cycles)) / SystemCoreClock);
              ///  LCD_OUTPUT_DURATION(120, 305, duration_us);

                /* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
                arm_cmplx_mag_q31(aFFT_Input_Q31, FFT_Output_Q31, FFT_Length);
//                arm_cmplx_mag_q15(aFFT_Input_Q15, FFT_Output_Q15, FFT_Length);

                /* Calculates maxValue and returns corresponding value */
//                arm_max_q31(FFT_Output_Q31, FFT_Length, &maxValue, &maxIndex);
//                arm_max_q15(FFT_Output_Q15, FFT_Length, &maxValue, &maxIndex);
//                maxValue = 0;

                if(0)
                {
                  print_text("a_");
                  for (index_fill_input_buffer = 0; index_fill_input_buffer < FFT_Length*2; index_fill_input_buffer++)
                  {
                ///    GRAPH_DATA_YT_SetAlign(aGraph_Data[1], GRAPH_ALIGN_LEFT);
                ///    GRAPH_DATA_YT_MirrorX (aGraph_Data[1], 1);
                ///    GRAPH_DATA_YT_AddValue(aGraph_Data[1], aADC1ConvertedValue_s[index_fill_input_buffer] / 50 + 50);
                    print_hex(_aADC1ConvertedValue_s[index_fill_input_buffer] / 50 + 50, 24);
                    print_text(";");
                  }
                  print_line();
                  print_text("f_");
                }

//                if(data_counter==sample_number_max)
//                {
//                  sample_max_min_dif_max=maxValue;
//                  sample_max_min_dif_min=maxValue;
//                }
//                else
//                {
//                  if(sample_max_min_dif_max<maxValue)
//                  {
//                    sample_max_min_dif_max=maxValue;
//                  }
//                  if(sample_max_min_dif_min>maxValue)
//                  {
//                    sample_max_min_dif_min=maxValue;
//                  }
//                }
                for (index_fill_output_buffer = 0+fft_out_shift; index_fill_output_buffer < fft_out+fft_out_shift; index_fill_output_buffer++)
                {
                  if(data_counter==sample_freq_fft)
//                  if(data_counter==sample_number_max)
                  {
                    sample_max_min_dif_max=FFT_Output_Q31[index_fill_output_buffer];
                    sample_max_min_dif_min=FFT_Output_Q31[index_fill_output_buffer];
                  }
                  else
                  {
                    if(sample_max_min_dif_max<FFT_Output_Q31[index_fill_output_buffer])
                    {
                      sample_max_min_dif_max=FFT_Output_Q31[index_fill_output_buffer];
                    }
                    if(sample_max_min_dif_min>FFT_Output_Q31[index_fill_output_buffer])
                    {
                      sample_max_min_dif_min=FFT_Output_Q31[index_fill_output_buffer];
                    }
                  }
//                  if(data_counter==sample_freq_fft)
////                  if(data_counter==sample_number_max)
//                  {
//                    sample_max_min_dif_max=FFT_Output_Q15[index_fill_output_buffer];
//                    sample_max_min_dif_min=FFT_Output_Q15[index_fill_output_buffer];
//                  }
//                  else
//                  {
//                    if(sample_max_min_dif_max<FFT_Output_Q15[index_fill_output_buffer])
//                    {
//                      sample_max_min_dif_max=FFT_Output_Q15[index_fill_output_buffer];
//                    }
//                    if(sample_max_min_dif_min>FFT_Output_Q15[index_fill_output_buffer])
//                    {
//                      sample_max_min_dif_min=FFT_Output_Q15[index_fill_output_buffer];
//                    }
//                  }

                }


                uicount++;
////                print_hex(uicount,8);
////                print_symbol(':');
//                print2_hex(uicount,8);
//                print2_symbol(':');
                for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
              //  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
                {
              ///    GRAPH_DATA_YT_SetAlign(aGraph_Data[0], GRAPH_ALIGN_LEFT);
              ///    GRAPH_DATA_YT_MirrorX (aGraph_Data[0], 1);
              ///    GRAPH_DATA_YT_AddValue(aGraph_Data[0], FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10);
//                  print_hex(FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10, 8);
//                  print_symbol(':');
//                  print2_hex(FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10, 8);
//                  print2_hex(FFT_Output_Q31[index_fill_output_buffer], 32);
//                  print2_symbol(':');
                }
//                print_line();
//                print2_line();

//                print2_symbol(';');
//                print2_hex(sample_max_min_dif_max>>16, 8);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min>>16, 8);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_max>>8, 16);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min>>8, 16);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_max>>8, 24);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min>>8, 24);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_max, 24);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min, 24);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_max, 32);          print2_symbol(';');
//                print2_hex(sample_max_min_dif_min, 32);          print2_symbol(';');
//                print2_line();

                const uint32_t uint8_data_number = 2 + 8 * 3 * 4 + 3 * 2 + 1;
//                const uint32_t uint8_data_number = 2 + 8 * 3 + 8 * 3 + 3 * 2 + 1;
                uint8_t dataBuffer[uint8_data_number];

                dataBuffer[0] = 0xA0;
//                dataBuffer[0] = 0x00;
                dataBuffer[1] = ui8SampleNumber++;

//                double f=(double)sample_max_min_dif/(double)0xffffff;
                double f_min=(double)sample_max_min_dif_min;
                if(f_min<=0)
                {
                  f_min=0.1;
                }
                double f_max=(double)sample_max_min_dif_max;
                if(f_max<=0)
                {
                  f_max=0.1;
                }
                if(f_max==f_min)
                {
                  f_max=f_min+1;
                }

                for (index_fill_output_buffer = 0+fft_out_shift; index_fill_output_buffer < fft_out+fft_out_shift; index_fill_output_buffer++)
//                for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
//                for(int ad_adc = 0; ad_adc < 4; ad_adc ++)
                {
//                  for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                  {
//                    int32_t sample_max_min_dif=samples_max_min_dif[ad_adc][ad_data_channel];
                    /*convert to long rainbow RGB*/
                    uint8_t r;
                    uint8_t g;
                    uint8_t b;
                    double f=(double)FFT_Output_Q31[index_fill_output_buffer];///f_max;
//                    double f=(double)FFT_Output_Q15[index_fill_output_buffer];///f_max;
                    if(f<=0)
                    {
                      f=0.1;
                    }
//                    print2_hex((int)(f), 32);          print2_symbol(';');
//                    print2_hex((int)(f_min), 32);          print2_symbol(';');
//                    print2_hex((int)(f_max), 32);          print2_symbol(';');
                    f=(log(f-f_min)-log(1))/(log(f_max-f_min)-log(1));
//                    print2_hex((int)(f*0xffffffff), 32);          print2_symbol(';');
//                    print2_hex((int)(log(f)*0xff), 32);          print2_symbol(';');
//                    print2_hex((int)(log(f_min)*0xff), 32);          print2_symbol(';');
//                    print2_hex((int)(log(f_max)*0xff), 32);          print2_symbol(';');
//                    print2_hex((int)((log(f_max)-log(f_min))*0xff), 32);          print2_symbol(';');
//                    print2_hex((int)((log(f)-log(f_min))*0xff), 32);          print2_symbol(';');
//                    print2_line();

//                    double f=(double)FFT_Output_Q31[index_fill_output_buffer]/f_max;
//                    double f=((double)FFT_Output_Q31[index_fill_output_buffer]-f_min)/(f_max-f_min);
//                    f=((double)FFT_Output_Q31[index_fill_output_buffer]-f_min)/(f_max-f_min);
//                    f=((double)FFT_Output_Q15[index_fill_output_buffer]-f_min)/(f_max-f_min);
//                    double f=(double)sample_max_min_dif/(double)0xffffff;
//                    double f_min=(double)sample_max_min_dif_min/(double)0xffffff;
//                    double f_max=(double)sample_max_min_dif_max/(double)0xffffff;
//                    double f=(double)(sample_max_min_dif>>8)/(double)0xffff;
//                    double f=(double)(sample_max_min_dif>>16)/(double)0xff;

                    uint8_t gray = f*255;

                    dataBuffer[2 + index_fill_output_buffer + 0] = gray;

//                    double a=(1-f)/0.2;
//                    int  X=floor(a);
//                    int  Y=floor(255*(a-X));
//                    switch(X)
//                    {
//                        case 0: r=255;g=Y;b=0;break;
//                        case 1: r=255-Y;g=255;b=0;break;
//                        case 2: r=0;g=255;b=Y;break;
//                        case 3: r=0;g=255-Y;b=255;break;
//                        case 4: r=Y;g=0;b=255;break;
//                        case 5: r=255;g=0;b=255;break;
//                    }

//                    dataBuffer[2 + index_fill_output_buffer * 3 + 0] = r;
//                    dataBuffer[2 + index_fill_output_buffer * 3 + 1] = g;
//                    dataBuffer[2 + index_fill_output_buffer * 3 + 2] = b;
//                    dataBuffer[2 + ad_adc * 24 + ad_data_channel * 3 + 0] = r;
//                    dataBuffer[2 + ad_adc * 24 + ad_data_channel * 3 + 1] = g;
//                    dataBuffer[2 + ad_adc * 24 + ad_data_channel * 3 + 2] = b;
                  }
                }
//                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
//                {
//                  dataBuffer[2 + ad_data_channel * 3 + 0] = data2[ad_data_channel * 4 + 1];
//                  dataBuffer[2 + ad_data_channel * 3 + 1] = data2[ad_data_channel * 4 + 2];
//                  dataBuffer[2 + ad_data_channel * 3 + 2] = data2[ad_data_channel * 4 + 3];
//                }
//                for(int accel_data_channel = 0; accel_data_channel < 3; accel_data_channel ++)
//                {
//                  dataBuffer[2 + fft_out * 1 + accel_data_channel * 2 + 0] = 0;
//                  dataBuffer[2 + fft_out * 1 + accel_data_channel * 2 + 1] = 0;
////                  dataBuffer[2 + 8 * 3 * 4 + accel_data_channel * 2 + 0] = 0;
////                  dataBuffer[2 + 8 * 3 * 4 + accel_data_channel * 2 + 1] = 0;
//                }
//                dataBuffer[2 + fft_out * 1 + 3 * 2 + 0] = 0xC0;
                dataBuffer[2 + fft_out * 1 + 0] = 0xC0;

//                dataBuffer[2]=100;
//                dataBuffer[3]=101;
//                dataBuffer[4]=102;

//                for(int data_out = 0; data_out < uint8_data_number; data_out ++)
//                {
//                  print2_hex(dataBuffer[data_out], 8);          print2_symbol(';');
//                }
//                print2_line();

//                if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART1)
                {
                  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
                  {
                  }
                  if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
                  {
                    Error_Handler();
                  }
                }
//                if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART2)
//                {
//                  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
//                  {
//                  }
//                  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
//                  {
//                    Error_Handler();
//                  }
//                }

              }

            }//FREESMARTEEG_OUT & FREESMARTEEG_FFT_DISPLAY



            if(FREESMARTEEG_OUT & FREESMARTEEG_ARDUINO_MEGA_DISPLAY)
            {
              if(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_PIN)
              {
//              int crc_pair_ok[uint8_ad_adc_number][4];

                for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
                {
                  int crc_ok = 1;
                  for(int ad_data_channel_pair = 0; ad_data_channel_pair < 4; ad_data_channel_pair ++)
                  {
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 1];
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 2];
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 3];

                    uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

  //                  uint8_t crc1=((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
  //                      |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)));
  //                  uint8_t chan_0=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b01110000)>>4);
  //                  uint8_t chan_1=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b01110000)>>4);
  //                  print2_hex(chan_0,8);
  //                  print2_symbol(':');
  //                  print2_hex(crc8_,8);
  //                  print2_symbol('-');
  //                  print2_hex(crc1,8);
  ////                  print2_symbol(';');
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(!(((uint8_t)crc8_)
  //                      -((uint8_t)crc1))),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)!(((uint8_t)crc8_)
  //                     -((~((uint8_t)crc1))&0b00000111)),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_0==ad_data_channel_pair*2),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_1==(ad_data_channel_pair*2+1)),8);
  //                  print2_symbol(';');
  //                  if(
  //                      ((!(((uint8_t)crc8_)
  //                          -((uint8_t)crc1)))
  //                     ||(!(((uint8_t)crc8_)
  //                         -((~((uint8_t)crc1))&0b00000111)))
  //                      )
  //                  &&(
  //                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
  //                        ((chan_0==ad_data_channel_pair)))
  //                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
  //                      ((chan_1==(ad_data_channel_pair+1))))
  //                  ))
                    if(
                        ((!(((uint8_t)crc8_)
                            -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                                |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                       ||(!(((uint8_t)crc8_)
                           -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                               |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                        )
                    &&(
                      (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                          ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b11110000)>>4)==ad_data_channel_pair*2)))
                    &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                        ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b11110000)>>4)==ad_data_channel_pair*2+1)))
                    ))
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                    }
                    else
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                      crc_ok = 0;
                    }

                  }
                  if(crc_ok)
                  {
  //                  print2_symbol('1');
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
  //                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
                  }
                  else
                  {
  //                  print2_symbol('0');
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
  //                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

                  }
  //                print2_symbol(';');
                }
              }


              int ad_adc = 0;
//              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                int ad_data_channel = 0;
//                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                {
//                  print2_binary(datas[ad_adc][ad_data_channel * 4 + 0], 8);          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
//                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
//                  print2_symbol(';');
//                  datas_out[ad_adc][ad_data_channel * 4 + 0]=datas[ad_adc][ad_data_channel * 4 + 1]&0b10000000;
//                  datas_out[ad_adc][ad_data_channel * 4 + 1]=datas[ad_adc][ad_data_channel * 4 + 1]&0b01111111;
//                  datas_out[ad_adc][ad_data_channel * 4 + 2]=datas[ad_adc][ad_data_channel * 4 + 2];
//                  datas_out[ad_adc][ad_data_channel * 4 + 3]=datas[ad_adc][ad_data_channel * 4 + 3];

//                  const int sample_number_max = 240;
//                  int sample[sample_number_max];
//                  int sample_number_last = -1;
//                  int sample_average = 0;
//
//                  int sample_int_out;
//                  uint8_t sample_byte_out;
//                  double sample_int_byte_scale;
//                  int sample_int_byte_shift;

//                  sample_int_out = (int)((datas_out[ad_adc][ad_data_channel * 4 + 0]<<24)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 1]<<16)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 2]<<8)
//                          |(datas_out[ad_adc][ad_data_channel * 4 + 3]<<0));

//                  int32_t newInt = (byteBuffer[2] << 24) | (byteBuffer[1] << 16) | byteBuffer[0] << 8;
//                  // depending on most significant byte, set positive or negative value
//                  if ((newInt & 0x00008000) > 0) {
//                    newInt |= 0x000000FF;
//                  } else {
//                    newInt &= 0xFFFFFF00;
//                  }

                  sample_int_out = (datas[ad_adc][ad_data_channel * 4 + 3]<<24)
                          |(datas[ad_adc][ad_data_channel * 4 + 2]<<16)
                          |(datas[ad_adc][ad_data_channel * 4 + 1]<<8);
                  if ((sample_int_out & 0x00008000) > 0) {
                    sample_int_out |= 0x000000FF;
                  } else {
                    sample_int_out &= 0xFFFFFF00;
                  }
                  sample_int_out >>= 8;
//
//                  sample_int_out = (datas[ad_adc][ad_data_channel * 4 + 3]<<16)
//                          |(datas[ad_adc][ad_data_channel * 4 + 2]<<8)
//                          |(datas[ad_adc][ad_data_channel * 4 + 1]<<0);

                  if(sample_number_now==sample_number_max)
                  {
                    for(int sample_count=0;sample_count<sample_number_now-1;sample_count++)
                    {
                      sample_int[sample_count]=sample_int[sample_count+1];
                      sample_byte[sample_count]=sample_byte[sample_count+1];
                    }
                  }
                  else
                  {
                    sample_number_now++;
                  }
                  sample_int[sample_number_now-1]=sample_int_out;

                  if((data_counter%sample_number_max==0)||(data_counter<sample_number_max))
                  {
                    sample_sum=0;

                    int32_t sample_max_new=sample_int[0];
                    int32_t sample_min_new=sample_int[0];

                    for(int sample_count=0;sample_count<sample_number_now;sample_count++)
                    {
                      sample_sum+=sample_int[sample_count];
                      if(sample_max_new<sample_int[sample_count])
                      {
                        sample_max_new=sample_int[sample_count];
                      }
                      if(sample_min_new>sample_int[sample_count])
                      {
                        sample_min_new=sample_int[sample_count];
                      }
                    }
//                    sample_average=sample_sum/sample_number_now;

                    if(((sample_max_last<sample_max_new)||(sample_min_last>sample_min_new))||
                        ((sample_max_last>sample_max_new+(sample_max+sample_min)/2)
                            ||(sample_min_last<sample_min_new-(sample_max+sample_min)/2)))
                    {
                      sample_max=sample_max_new;
                      sample_min=sample_min_new;
                    }

                    sample_max_last=sample_max;
                    sample_min_last=sample_min;

                  }

//                  sample_int_byte_shift = (1<<8)/(1<<32);
//                  sample_byte_out = sample_int_out + sample_int_byte_shift;
//                  sample_int_byte_scale = 1;//(1<<8)/(1<<32);
//                  sample_byte_out = (uint8_t)(sample_int_out * sample_int_byte_scale);

                  sample_byte_out = (uint8_t)(((int64_t)sample_int_out-(int64_t)sample_min)*255/((int64_t)sample_max-(int64_t)sample_min+1));
                  sample_byte[sample_number_now-1]=sample_byte_out;


//                  print2_symbol(datas_out[ad_adc][ad_data_channel * 4 + 3]);
//                  print2_symbol((uint8_t)((sample_int_out/(1<<0)+(255/4)*(1))));
//                  print2_symbol((uint8_t)(((sample_int_out-1000000)/1000000)));
                  if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART1)
                  {
                    print_symbol(sample_byte_out);
                  }
//                  if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART2)
//                  {
//                    print2_hex(sample_byte_out, 8);          print2_symbol(';');
//                    print2_hex(sample_int_out, 32);          print2_symbol(';');
//                    print2_hex(sample_min, 32);          print2_symbol(';');
//                    print2_hex(sample_max, 32);          print2_symbol(';');
//                    print2_line();
//                  }
                }
              }

            }//FREESMARTEEG_OUT & FREESMARTEEG_ARDUINO_MEGA_DISPLAY


            if(FREESMARTEEG_OUT & FREESMARTEEG_DIF)
            {

//              crc8_01=crc8da_7(&(datas[0][0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((datas[0][0]<<4)|(datas[0][4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((datas[0][0]<<4)|(datas[0][4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((data1[0]>=0b10000000)&&(data1[0]<0b10010000))||
//                    ((datas[0][0]>=0b00000000)&&(datas[0][0]<0b00010000)))
//              &&(//((data1[4]>=0b10010000)&&(data1[4]<0b10100000))||
//                  ((datas[0][4]>=0b00010000)&&(datas[0][4]<0b00100000)))
//              ))
//              {
//                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
//              }
//              else
//              {
//                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//              }

//              crc8_01=crc8da_7(&(data2[0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((data2[0]<<4)|(data2[4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((data2[0]<<4)|(data2[4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (((data2[0]>=0b10000000)&&(data2[0]<0b10010000))||((data2[0]>=0b00000000)&&(data2[0]<0b00010000)))
//              &&(((data2[4]>=0b10010000)&&(data2[4]<0b10100000))||((data2[4]>=0b00010000)&&(data2[4]<0b00100000)))
//              ))
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//              }
//              else
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
//              }

//              int FFT_Length = 8;
//              int FFT_Length = 16;
              int FFT_Length = 32;
//              int FFT_Length = 48;
//              int FFT_Length = 64;
//              int FFT_Length = 80;
//              int FFT_Length = 128;
//              int FFT_Length = 256;
              int multmax = 0;
              int bufferspeed=1;
//              int bufferspeed=FFT_Length/4;
              int index_fill_adc_buffer = last_index_fill_adc_buffer;
              if(last_index_fill_adc_buffer==FFT_Length*2)
              {
                last_index_fill_adc_buffer--;
                for (index_fill_adc_buffer = 0; index_fill_adc_buffer < FFT_Length*2-bufferspeed; index_fill_adc_buffer ++)
                {
                  _aADC1ConvertedValue_s[index_fill_adc_buffer] = _aADC1ConvertedValue_s[index_fill_adc_buffer+bufferspeed];
                }
              }
              else
              {
                index_fill_adc_buffer++;
              }
              _uhADCxConvertedValue = datas[0][1]<<16 | datas[0][2]<<8 | datas[0][3];
              _aADC1ConvertedValue_s[last_index_fill_adc_buffer] = _uhADCxConvertedValue;
              for(int mult=0;mult<multmax;mult++)
              {
                if(last_index_fill_adc_buffer-mult>=0)
                {
                  _aADC1ConvertedValue_s[last_index_fill_adc_buffer-mult] = _uhADCxConvertedValue;
                }
              }
              last_index_fill_adc_buffer=index_fill_adc_buffer;

              if(last_index_fill_adc_buffer==FFT_Length*2)
              {
                double median=0;
                for (int index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length*2; index_fill_output_buffer++)
                {
                  median+=(double)(_aADC1ConvertedValue_s[index_fill_output_buffer]);
                }
                median/=(double)(FFT_Length*2);
                double stddev_=0;
                for (int index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length*2; index_fill_output_buffer++)
                {
                  stddev_+=(((double)(_aADC1ConvertedValue_s[index_fill_output_buffer]))-median)*(((double)(_aADC1ConvertedValue_s[index_fill_output_buffer]))-median);
                }
                double stddevmax=0;
                double stddev=0;
                if(stddev_)
                {
                  stddev_=(double)sqrt(((double)(stddev_))/((double)(FFT_Length*2)));

                  for (int index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length*2; index_fill_output_buffer++)
                  {
                    stddev=(double)((((double)(_aADC1ConvertedValue_s[index_fill_output_buffer]))-median)/stddev_);
                    if(stddevmax<stddev)
                    {
                      stddevmax=stddev;
                    }
                  }

//                  stddev=(double)((((double)(_aADC1ConvertedValue_s[FFT_Length]))-median)/stddev_);
                }

//                double stddevh=2.08;

                double stddevh=4.0;

//                double stddevh=3.0;
//                double stddevh=2.9;
//                double stddevh=2.7;
//                double stddevh=2.6;
//                double stddevh=2.2;
//                double stddevh=2.15;
//                double stddevh=2.06;
                if(stddevmax>stddevh)
                {

//                if(stddev>stddevh)
                  if(last_stddev_count<=0)
                  {
                    last_stddev_count=FFT_Length*3;
  //                  last_stddev_count=FFT_Length*2;
  //                  last_stddev_count=250;
  //                  last_stddev_count=500;
  //                  last_stddev_count=1000;
  //                  last_index_fill_adc_buffer = 0;
                    uicount++;

                    if(times_count==times_max)
                    {
                      int times_count1;
                      for(times_count1=0;times_count1<times_max-1;times_count1++)
                      {
                        times[times_count1]=times[times_count1+1];
                        stddevmaxs[times_count1]=stddevmaxs[times_count1+1];
                      }
                      times_count=times_count1;
                    }
                    stddevmaxs[times_count]=stddevmax;
                    rtc_read_frac_s(&time1, &frac_secs1);
                    times[times_count]=time1;
                    times_count++;
                    double times_stddev=0;
                    for(int times_stddev_int=0;times_stddev_int<times_count-1;times_stddev_int++)
                    {
  //                    times_stddev=stddevmaxs[times_stddev_int]
                      times_stddev+=(times[times_stddev_int]-times[times_stddev_int+1])*(times[times_stddev_int]-times[times_stddev_int+1]);
                    }
                    times_stddev=sqrt(times_stddev/times_count);

                    if(1)
                    {
                      //                print_hex(uicount,8);
                      //                print_symbol(':');
                      print2_hex(uicount,8);
                      print2_symbol(':');
                      //                for (int index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
                                    //  for (index_fill_output_buffer = 0; index_fill_output_buffer < FFT_Length; index_fill_output_buffer++)
                      //                {
                      //                  FFT_Output_Q31[index_fill_output_buffer]=_aADC1ConvertedValue_s[index_fill_output_buffer];
                                    ///    GRAPH_DATA_YT_SetAlign(aGraph_Data[0], GRAPH_ALIGN_LEFT);
                                    ///    GRAPH_DATA_YT_MirrorX (aGraph_Data[0], 1);
                                    ///    GRAPH_DATA_YT_AddValue(aGraph_Data[0], FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10);
                      //                  print_hex(FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10, 8);
                      //                  print_symbol(':');
                      //                  print2_hex(FFT_Output_Q31[index_fill_output_buffer] / 5376212 + 10, 8);
                      //                  print2_symbol(':');
                      //                }


                      print2_hex((long)median, 32);
                      print2_symbol(':');
                      print2_hex((long)_aADC1ConvertedValue_s[FFT_Length], 32);
                      print2_symbol(':');
                      print2_hex((long)stddev_, 32);
                      print2_symbol(':');
                      print2_hex((long)(stddevmax*1000), 32);

                      print2_symbol(':');
                      print2_hex((long)time1, 32);
                      print2_symbol(':');
                      print2_hex((long)(times_stddev*1000), 32);
                      print2_symbol(':');
                      print2_hex((long)(60/times_stddev), 32);

      //                print_line();
                      print2_line();
                    }

                    if(stddev>stddevh)
                    {
  //                    print_hex(uicount,8);
  //                    print_symbol(':');
  //                    print_hex((stddevmax*1000), 16);
  ////                    print_symbol(':');
  ////                    print_hex((long)time1, 32);
  //                    print_symbol(':');
  //                    print_hex((long)(60/times_stddev), 16);
  //                    print_line();
                      print_symbol((stddevmax*10));
                      HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
                    }
                    else
                    {
                      HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
//                      print_symbol((stddevmax*1));
                      print_symbol(0);
                    }

                  }
                  else
                  {
                    HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
//                    print_symbol((stddevmax*10));
                    print_symbol(0);
                  }
                }
                else
                {
                  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
//                  print_symbol((stddevmax*10));
                  print_symbol(0);
                }
//                last_stddev_count/=2;
                last_stddev_count--;
              }
            }//FREESMARTEEG_DIF

            if(FREESMARTEEG_OUT & FREESMARTEEG_SAMPLECOUNT)
            {
              ui32SampleNumber++;
//              if(!(ui32SampleNumber%250))
//              if(!(ui32SampleNumber%500))
              if(!(ui32SampleNumber%512))
//              if(!(ui32SampleNumber%4000))
//              if(!(ui32SampleNumber%1000))
//              if(!(ui32SampleNumber%0x1000))
//              if(!(ui32SampleNumber%0x100))
//              if(!(ui32SampleNumber%0x10000))
//              if(!(ui32SampleNumber%0x20000))
              {
                print_hex(ui32SampleNumber,32);
                print_line();

                print2_hex(ui32SampleNumber,32);
                print2_line();
//                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
              }
              else
              {
//                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
              }
            }//FREESMARTEEG_SAMPLECOUNT

            if(FREESMARTEEG_OUT & FREESMARTEEG_CRC)
//            if(0)//crc
            {
              /*
              for(int channelpair=0;channelpair<4;channelpair++)
              {
//                for(int channelinpair=0;channelinpair<2;channelinpair++)
//                {
//                  for(int byteofchannelinpair=0;byteofchannelinpair<3;byteofchannelinpair++)
//                  {
//                    crcBuffer[channelpair][channelinpair*3+byteofchannelinpair]=data[channelpair*8+channelinpair*4+byteofchannelinpair+1];
//                  }
//                }
                crcBuffer[channelpair][0]=((data[channelpair*8+0] & 0xf0)) | ((data[channelpair*8+1] & 0xf0) >> 4);
                crcBuffer[channelpair][1]=((data[channelpair*8+1] & 0x0f) << 4) | ((data[channelpair*8+2] & 0xf0) >> 4);
                crcBuffer[channelpair][2]=((data[channelpair*8+2] & 0x0f) << 4) | ((data[channelpair*8+3] & 0xf0) >> 4);
                crcBuffer[channelpair][3]=((data[channelpair*8+3] & 0x0f) << 4) | ((data[channelpair*8+4] & 0xf0) >> 4);
                crcBuffer[channelpair][4]=data[channelpair*8+5];
                crcBuffer[channelpair][5]=data[channelpair*8+6];
                crcBuffer[channelpair][6]=data[channelpair*8+7];
                crcBuffer[channelpair][7]=0x00;


                crc8fromad[channelpair]=((data[channelpair*8]&0x0f)<<4) | (data[channelpair*8+4]&0x0f);
                crc8[channelpair]=ad7779_compute_crc8(crcBuffer[channelpair], 8);

                crcBuffer[channelpair][0] = crcBuffer[channelpair][0] ^ 0xff;
                crc8[channelpair]=crc8da(crcBuffer[channelpair], 8);

//                crc8[channelpair]=crc8da_7(&data[channelpair*8]);

                if(crc8fromad[channelpair]==crc8[channelpair])
                {
                  crc8ok[channelpair]=1;
                }
                else
                {
                  crc8ok[channelpair]=0;
                }
              }
              for(int channel=0;channel<8;channel++)
              {
                if((data[channel*4]&0x70)>>4==channel)
                {
                  channelnumberok[channel]=1;
                }
                else
                {
                  channelnumberok[channel]=0;
                }
              }
              */
             crc8_01=crc8da_7(&(data1[0]));
             crc8_23=crc8da_7(&(data1[8]));
             crc8_45=crc8da_7(&(data1[16]));
             crc8_67=crc8da_7(&(data1[24]));
             int crcok00=0;
//             int crcok0=0;
//             int crcok1=0;
//             int crcok2=0;
//             int crcok3=0;

            /*    print_hex(crc8_01,8);    Serial.print(";");
                print_hex(crc8_23,8);    Serial.print(";");
                print_hex(crc8_45,8);    Serial.print(";");
                print_hex(crc8_67,8);    Serial.print(";");
                Serial.println("");

                print_hex(((b00<<4)|(b04&0x0f)),8);    Serial.print(";");
                print_hex(((b08<<4)|(b12&0x0f)),8);    Serial.print(";");
                print_hex(((b16<<4)|(b20&0x0f)),8);    Serial.print(";");
                print_hex(((b24<<4)|(b28&0x0f)),8);    Serial.print(";");
                Serial.println("");*/

            // repeat=1;

             if(
              (
               (((data1[0]>=0b10000000)&&(data1[0]<0b10010000))||((data1[0]>=0b00000000)&&(data1[0]<0b00010000)))
             &&(((data1[4]>=0b10010000)&&(data1[4]<0b10100000))||((data1[4]>=0b00010000)&&(data1[4]<0b00100000)))
             &&(((data1[8]>=0b10100000)&&(data1[8]<0b10110000))||((data1[8]>=0b00100000)&&(data1[8]<0b00110000)))
             &&(((data1[12]>=0b10110000)&&(data1[12]<0b11000000))||((data1[12]>=0b00110000)&&(data1[12]<0b01000000)))
             &&(((data1[16]>=0b11000000)&&(data1[16]<0b11010000))||((data1[16]>=0b01000000)&&(data1[16]<0b01010000)))
             &&(((data1[20]>=0b11010000)&&(data1[20]<0b11100000))||((data1[20]>=0b01010000)&&(data1[20]<0b01100000)))
             &&(((data1[24]>=0b11100000)&&(data1[24]<0b11110000))||((data1[24]>=0b01100000)&&(data1[24]<0b01110000)))
             &&(((data1[28]>=0b11110000)&&(data1[28]<=0b11111111))||((data1[28]>=0b01110000)&&(data1[28]<=0b01111111)))
             ))
//              if((crc8ok[0]&&crc8ok[1]&&crc8ok[2]&&crc8ok[3]) &&
//                 (channelnumberok[0]&&channelnumberok[1]&&channelnumberok[2]&&channelnumberok[3]&&
//                  channelnumberok[4]&&channelnumberok[5]&&channelnumberok[6]&&channelnumberok[7]))
             {
           //   repeat=0;
               c000++;
             }

              if(((!(((uint8_t)crc8_01)-((uint8_t)((data1[0]<<4)|(data1[4]&0x0f)))))
                 &&(!(((uint8_t)crc8_23)-((uint8_t)((data1[8]<<4)|(data1[12]&0x0f)))))
                 &&(!(((uint8_t)crc8_45)-((uint8_t)((data1[16]<<4)|(data1[20]&0x0f)))))
                 &&(!(((uint8_t)crc8_67)-((uint8_t)((data1[24]<<4)|(data1[28]&0x0f))))))
              && (
                (((data1[0]>=0b10000000)&&(data1[0]<0b10010000))||((data1[0]>=0b00000000)&&(data1[0]<0b00010000)))
              &&(((data1[4]>=0b10010000)&&(data1[4]<0b10100000))||((data1[4]>=0b00010000)&&(data1[4]<0b00100000)))
              &&(((data1[8]>=0b10100000)&&(data1[8]<0b10110000))||((data1[8]>=0b00100000)&&(data1[8]<0b00110000)))
              &&(((data1[12]>=0b10110000)&&(data1[12]<0b11000000))||((data1[12]>=0b00110000)&&(data1[12]<0b01000000)))
              &&(((data1[16]>=0b11000000)&&(data1[16]<0b11010000))||((data1[16]>=0b01000000)&&(data1[16]<0b01010000)))
              &&(((data1[20]>=0b11010000)&&(data1[20]<0b11100000))||((data1[20]>=0b01010000)&&(data1[20]<0b01100000)))
              &&(((data1[24]>=0b11100000)&&(data1[24]<0b11110000))||((data1[24]>=0b01100000)&&(data1[24]<0b01110000)))
              &&(((data1[28]>=0b11110000)&&(data1[28]<=0b11111111))||((data1[28]>=0b01110000)&&(data1[28]<=0b01111111)))
              ))
//              if((crc8ok[0]&&crc8ok[1]&&crc8ok[2]&&crc8ok[3]) &&
//                 (channelnumberok[0]&&channelnumberok[1]&&channelnumberok[2]&&channelnumberok[3]&&
//                  channelnumberok[4]&&channelnumberok[5]&&channelnumberok[6]&&channelnumberok[7]))
              {
                crcok00=1;
            //   repeat=0;
                c00++;

                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);

                if(0)
                {
                  for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                  {
                    print2_binary(data1[ad_data_channel * 4 + 0], 8);          print_symbol(';');
          //          print_hex(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
          //          print_hex(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
          //          print_hex(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
                    print2_hex(data1[ad_data_channel * 4 + 1], 8);          print_symbol(';');
                    print2_hex(data1[ad_data_channel * 4 + 2], 8);          print_symbol(';');
                    print2_hex(data1[ad_data_channel * 4 + 3], 8);          print_symbol(';');
                    print2_symbol(';');
                  }
                  print2_line();
                }

              }
              else
              {
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
              }
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);

              c01++;

              if((!(((uint8_t)crc8_01)-((uint8_t)((data1[0]<<4)|(data1[4]&0x0f)))))
              &&(
                (((data1[0]>=0b10000000)&&(data1[0]<0b10010000))||((data1[0]>=0b00000000)&&(data1[0]<0b00010000)))
              &&(((data1[4]>=0b10010000)&&(data1[4]<0b10100000))||((data1[4]>=0b00010000)&&(data1[4]<0b00100000)))
              ))
              {
//                crcok0=1;
                c0++;
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

                if(0)
                {
                  const uint32_t uint8_data_number = 3+4;
                  uint8_t dataBuffer[uint8_data_number];
                  dataBuffer[0] = 0xff;
                  dataBuffer[1] = 0xfe;
                  dataBuffer[2] = 0xfd;
                  dataBuffer[3] = data1[0];
                  dataBuffer[4] = data1[1];
                  dataBuffer[5] = data1[2];
                  dataBuffer[6] = data1[3];
                //  HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//                  HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
                  if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
                  {
                    Error_Handler();
                  }
                  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
                  {
                  }
                }

              }
              else
              {
                if((data1[0]&0x0f)==0)
                {
                  c_0++;
                }
                if((data1[4]&0x0f)==((~crc8_01)&0x07))
                {
                  c_00++;
                }
              }
              if((!(((uint8_t)crc8_23)-((uint8_t)((data1[8]<<4)|(data1[12]&0x0f)))))
              &&(
                (((data1[8]>=0b10100000)&&(data1[8]<0b10110000))||((data1[8]>=0b00100000)&&(data1[8]<0b00110000)))
              &&(((data1[12]>=0b10110000)&&(data1[12]<0b11000000))||((data1[12]>=0b00110000)&&(data1[12]<0b01000000)))
              ))
              {
//                crcok1=1;
                c1++;
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
              }
              else
              {
                if((data1[8]&0x0f)==0)
                {
                  c_1++;
                }
                if((data1[12]&0x0f)==((~crc8_23)&0x07))
                {

                  c_01++;
                }
              }
              if((!(((uint8_t)crc8_45)-((uint8_t)((data1[16]<<4)|(data1[20]&0x0f)))))
              &&(
                (((data1[16]>=0b11000000)&&(data1[16]<0b11010000))||((data1[16]>=0b01000000)&&(data1[16]<0b01010000)))
              &&(((data1[20]>=0b11010000)&&(data1[20]<0b11100000))||((data1[20]>=0b01010000)&&(data1[20]<0b01100000)))
              ))
              {
//                crcok2=1;
                c2++;
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
              }
              else
              {
                if((data1[16]&0x0f)==0)
                {
                  c_2++;
                }
                if((data1[20]&0x0f)==((~crc8_45)&0x07))
                {
                  c_02++;
                }
              }
              if((!(((uint8_t)crc8_67)-((uint8_t)((data1[24]<<4)|(data1[28]&0x0f)))))
              &&(
                (((data1[24]>=0b11100000)&&(data1[24]<0b11110000))||((data1[24]>=0b01100000)&&(data1[24]<0b01110000)))
              &&(((data1[28]>=0b11110000)&&(data1[28]<=0b11111111))||((data1[28]>=0b01110000)&&(data1[28]<=0b01111111)))
              ))
              {
//                crcok3=1;
                c3++;
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
              }
              else
              {
                if((data1[24]&0x0f)==0)
                {
                  c_3++;
                }
                if((data1[28]&0x0f)==((~crc8_67)&0x07))
                {
                  c_03++;
                }
              }
              if(1)
                if(!crcok00)
              {
//                  if(!crcok0)
//                  {
//                    print_binary((crc8_01&0x0f),4);
//                    print_text("_");
//                    print_binary((data[4]&0x0f),4);
//                    print_text(";\n");
//                  }
//                  if(!crcok1)
//                  {
//                    print_binary((crc8_23&0x0f),4);
//                    print_text("_");
//                    print_binary((data[12]&0x0f),4);
//                    print_text(";\n");
//                  }
//                  if(!crcok2)
//                  {
//                    print_binary((crc8_45&0x0f),4);
//                    print_text("_");
//                    print_binary((data[20]&0x0f),4);
//                    print_text(";\n");
//                  }
//                  if(!crcok3)
//                  {
//                    print_binary((crc8_67&0x0f),4);
//                    print_text("_");
//                    print_binary((data[28]&0x0f),4);
//                    print_text(";\n");
//                  }

//                if(!crcok0)
//                {
//                  print_binary((crc8_01&0x0f),4);
//                  print_text("_");
//                  print_binary((data[4]&0x0f),4);
//                  print_text(";");
//                }
//                else
//                {
//                  print_text("_________");
//                  print_text(";");
//                }
//                if(!crcok1)
//                {
//                  print_binary((crc8_23&0x0f),4);
//                  print_text("_");
//                  print_binary((data[12]&0x0f),4);
//                  print_text(";");
//                }
//                else
//                {
//                  print_text("_________");
//                  print_text(";");
//                }
//                if(!crcok2)
//                {
//                  print_binary((crc8_45&0x0f),4);
//                  print_text("_");
//                  print_binary((data[20]&0x0f),4);
//                  print_text(";");
//                }
//                else
//                {
//                  print_text("_________");
//                  print_text(";");
//                }
//                if(!crcok3)
//                {
//                  print_binary((crc8_67&0x0f),4);
//                  print_text("_");
//                  print_binary((data[28]&0x0f),4);
//                  print_text(";;");
//                }
//                else
//                {
//                  print_text("_________");
//                  print_text(";;");
//                }
//                print_hex((crc8_01&0x0f),4);
//                print_text("_");
//                print_hex((data[4]&0x0f),4);
//                print_text(";");
//                print_hex((crc8_23&0x0f),4);
//                print_text("_");
//                print_hex((data[12]&0x0f),4);
//                print_text(";");
//                print_hex((crc8_45&0x0f),4);
//                print_text("_");
//                print_hex((data[20]&0x0f),4);
//                print_text(";");
//                print_hex((crc8_67&0x0f),4);
//                print_text("_");
//                print_hex((data[28]&0x0f),4);
//                print_text(";;");
                print2_hex((c01*4-(c0+c1+c2+c3))-(c_00+c_01+c_02+c_03),8);
                print2_text(";;");
                print2_hex((c01*4-(c0+c1+c2+c3))-(c_0+c_1+c_2+c_3),8);
                print2_text(";;");
                print2_hex(c_0,8);
                print2_text(";");
                print2_hex(c_1,8);
                print2_text(";");
                print2_hex(c_2,8);
                print2_text(";");
                print2_hex(c_3,8);
                print2_text(";;");
                print2_hex(c01-c000,8);
                print2_text(";");
                print2_hex(0xff*((float)(c000))/((float)(c01)),8);
                print2_text(";;");
                print2_hex(0xff*((float)(c00))/((float)(c01)),8);
                print2_text(";;");
                print2_hex(0xff*((float)(c0))/((float)(c01)),8);
                print2_text(";");
                print2_hex(0xff*((float)(c1))/((float)(c01)),8);
                print2_text(";");
                print2_hex(0xff*((float)(c2))/((float)(c01)),8);
                print2_text(";");
                print2_hex(0xff*((float)(c3))/((float)(c01)),8);
                print2_text(";");
                print2_line();
              }
              if(0)
              {
                sprintf(str_out, str_in, 100*((float)(c00))/((float)(c01)));
                print2_text(str_out);
                print2_text("; ");
                sprintf(str_out, str_in, 100*((float)(c0))/((float)(c01)));
                print2_text(str_out);
                print2_text("; ");
                sprintf(str_out, str_in, 100*((float)(c1))/((float)(c01)));
                print2_text(str_out);
                print2_text("; ");
                sprintf(str_out, str_in, 100*((float)(c2))/((float)(c01)));
                print2_text(str_out);
                print2_text("; ");
                sprintf(str_out, str_in, 100*((float)(c3))/((float)(c01)));
                print2_text(str_out);
                print2_text("; ");
                print2_line();
              }
    //            Serial.print(100*((float)(c00))/((float)(c01)),6);    Serial.print("; ");
    //            Serial.print(100*((float)(c0))/((float)(c01)),6);    Serial.print("; ");
    //            Serial.print(100*((float)(c1))/((float)(c01)),6);    Serial.print("; ");
    //            Serial.print(100*((float)(c2))/((float)(c01)),6);    Serial.print("; ");
    //            Serial.print(100*((float)(c3))/((float)(c01)),6);    Serial.print("; ");
            //    Serial.print(100*((float)(c0))/((float)(c0+c1+c2+c3)),6);    Serial.print("; ");
            //    Serial.print(100*((float)(c1))/((float)(c0+c1+c2+c3)),6);    Serial.print("; ");
            //    Serial.print(100*((float)(c2))/((float)(c0+c1+c2+c3)),6);    Serial.print("; ");
            //    Serial.print(100*((float)(c3))/((float)(c0+c1+c2+c3)),6);    Serial.print("; ");
    //            Serial.println("");



            }//text==3//crc

            if(FREESMARTEEG_OUT & FREESMARTEEG_TEXT)
//            if(0)
            {

//              int crc_pair_ok[uint8_ad_adc_number][4];

              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                int crc_ok = 1;
                for(int ad_data_channel_pair = 0; ad_data_channel_pair < 4; ad_data_channel_pair ++)
                {
//                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 1];
//                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 2];
//                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 3];

                  uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

//                  uint8_t crc1=((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
//                      |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)));
//                  uint8_t chan_0=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b01110000)>>4);
//                  uint8_t chan_1=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b01110000)>>4);
//                  print2_hex(chan_0,8);
//                  print2_symbol(':');
//                  print2_hex(crc8_,8);
//                  print2_symbol('-');
//                  print2_hex(crc1,8);
////                  print2_symbol(';');
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(!(((uint8_t)crc8_)
//                      -((uint8_t)crc1))),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)!(((uint8_t)crc8_)
//                     -((~((uint8_t)crc1))&0b00000111)),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(chan_0==ad_data_channel_pair*2),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(chan_1==(ad_data_channel_pair*2+1)),8);
//                  print2_symbol(';');
//                  if(
//                      ((!(((uint8_t)crc8_)
//                          -((uint8_t)crc1)))
//                     ||(!(((uint8_t)crc8_)
//                         -((~((uint8_t)crc1))&0b00000111)))
//                      )
//                  &&(
//                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
//                        ((chan_0==ad_data_channel_pair)))
//                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
//                      ((chan_1==(ad_data_channel_pair+1))))
//                  ))
                  if(
                      ((!(((uint8_t)crc8_)
                          -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                              |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                     ||(!(((uint8_t)crc8_)
                         -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                             |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                      )
                  &&(
                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                        ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b11110000)>>4)==ad_data_channel_pair*2)))
                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                      ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b11110000)>>4)==ad_data_channel_pair*2+1)))
                  ))
                  {
//                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                  }
                  else
                  {
//                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                    crc_ok = 0;
                  }

                }
                if(crc_ok)
                {
//                  print2_symbol('1');
                  HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
  //                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
                }
                else
                {
//                  print2_symbol('0');
                  HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
  //                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//                  ad7779_do_spi_soft_reset(devices[ad_adc]);
//                  HAL_GPIO_WritePin(reset_ports[ad_adc], reset_pins[ad_adc], GPIO_PIN_RESET);
//                  HAL_Delay(1);
//                  HAL_GPIO_WritePin(reset_ports[ad_adc], reset_pins[ad_adc], GPIO_PIN_SET);
//
//                  ad7779_setup(&devices[ad_adc], init_params[ad_adc]);
//
//                    {
//                      aTxBuffer[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
//                      aTxBuffer[1] = AD7779_SPI_SLAVE_MODE_EN;
//
//            //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//                      if(HAL_SPI_TransmitReceive_DMA(devices[ad_adc]->spi_dev.dev, aTxBuffer, datas[ad_adc], 2) != HAL_OK)
//            //          if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, 2, 5000) != HAL_OK)
//                      {
//                        Error_Handler();
//                      }
//            //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//                      while (HAL_SPI_GetState(devices[ad_adc]->spi_dev.dev) != HAL_SPI_STATE_READY)
//                      {
//                      }
//
//                    }
//
//                    {
//                      aTxBuffer[0] = AD7779_REG_GEN_ERR_REG_1_EN;
//                      aTxBuffer[1] = AD7779_SPI_CRC_TEST_EN;
//
//            //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//                      if(HAL_SPI_TransmitReceive_DMA(devices[ad_adc]->spi_dev.dev, aTxBuffer, datas[ad_adc], 2) != HAL_OK)
//            //          if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, 2, 5000) != HAL_OK)
//                      {
//                        Error_Handler();
//                      }
//            //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//                      while (HAL_SPI_GetState(devices[ad_adc]->spi_dev.dev) != HAL_SPI_STATE_READY)
//                      {
//                      }
//
//                    }

                }
//                print2_symbol(';');
              }



//              crc8_01=crc8da_7(&(data[0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((data[0]<<4)|(data[4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
//                    ((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//              &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
//                  ((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//              ))
//              {
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
////                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
//              }
//              else
//              {
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
////                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//              }
//
//              crc8_01=crc8da_7(&(data2[0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((data2[0]<<4)|(data2[4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((data2[0]<<4)|(data2[4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((data2[0]>=0b10000000)&&(data2[0]<0b10010000))||
//                    ((data2[0]>=0b00000000)&&(data2[0]<0b00010000)))
//              &&(//((data2[4]>=0b10010000)&&(data2[4]<0b10100000))||
//                  ((data2[4]>=0b00010000)&&(data2[4]<0b00100000)))
//              ))
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//              }
//              else
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//              }
//
//              crc8_01=crc8da_7(&(data3[0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((data3[0]<<4)|(data3[4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((data3[0]<<4)|(data3[4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((data3[0]>=0b10000000)&&(data3[0]<0b10010000))||
//                    ((data3[0]>=0b00000000)&&(data3[0]<0b00010000)))
//              &&(//((data3[4]>=0b10010000)&&(data3[4]<0b10100000))||
//                  ((data3[4]>=0b00010000)&&(data3[4]<0b00100000)))
//              ))
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
//              }
//              else
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
//              }
//
//              crc8_01=crc8da_7(&(data4[0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((data4[0]<<4)|(data4[4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((data4[0]<<4)|(data4[4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((data4[0]>=0b10000000)&&(data4[0]<0b10010000))||
//                    ((data4[0]>=0b00000000)&&(data4[0]<0b00010000)))
//              &&(//((data4[4]>=0b10010000)&&(data4[4]<0b10100000))||
//                  ((data4[4]>=0b00010000)&&(data4[4]<0b00100000)))
//              ))
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
//              }
//              else
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
//              }

//              int ad_adc = 3;
              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
//                int ad_data_channel = 6;
                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                {
//                  print2_binary(datas[ad_adc][ad_data_channel * 4 + 0], 8);          print2_symbol(';');
                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
                  print2_hex(datas[ad_adc][ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
      //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
                  print2_symbol(';');
                }
                print2_symbol(';');
              }

//              for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
//              {
////                print2_binary(data[ad_data_channel * 4 + 0], 8);          print2_symbol(';');
//                print2_hex(data[ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
////                print2_hex(data[ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
////                print2_hex(data[ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
////                print2_hex(data[ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
//                print2_symbol(';');
//              }
//              print2_symbol(';');
//
//              for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
//              {
////                print2_binary(data2[ad_data_channel * 4 + 0], 8);          print2_symbol(';');
//                print2_hex(data2[ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
////                print2_hex(data2[ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
////                print2_hex(data2[ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
////                print2_hex(data2[ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
//                print2_symbol(';');
//              }
//              print2_symbol(';');
//
//              for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
//              {
////                print2_binary(data2[ad_data_channel * 4 + 0], 8);          print2_symbol(';');
//                print2_hex(data3[ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
////                print2_hex(data3[ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
////                print2_hex(data3[ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
////                print2_hex(data3[ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
//                print2_symbol(';');
//              }
//              print2_symbol(';');
//
//              for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
//              {
////                print2_binary(data2[ad_data_channel * 4 + 0], 8);          print2_symbol(';');
//                print2_hex(data4[ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
////                print2_hex(data4[ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
////                print2_hex(data4[ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
////                print2_hex(data4[ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 1], 8);          print_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 2], 8);          print_symbol(';');
//    //            print_binary(data[ad_data_channel * 4 + 3], 8);          print_symbol(';');
//                print2_symbol(';');
//              }

              print2_symbol(';');
              print2_line();
            }
            if(FREESMARTEEG_OUT & FREESMARTEEG_TEXT_UART1)
//            if(0)
            {

//              int crc_pair_ok[uint8_ad_adc_number][4];

              if(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_PIN)
              {

                for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
                {
                  int crc_ok = 1;
                  int crc_something_ok = 0;
                  for(int ad_data_channel_pair = 0; ad_data_channel_pair < (uint8_ad_chan_number / 2); ad_data_channel_pair ++)
                  {

                    uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

  //                  uint8_t crc1=((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
  //                      |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)));
  //                  uint8_t chan_0=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b01110000)>>4);
  //                  uint8_t chan_1=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b01110000)>>4);
  //                  print2_hex(chan_0,8);
  //                  print2_symbol(':');
  //                  print2_hex(crc8_,8);
  //                  print2_symbol('-');
  //                  print2_hex(crc1,8);
  ////                  print2_symbol(';');
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(!(((uint8_t)crc8_)
  //                      -((uint8_t)crc1))),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)!(((uint8_t)crc8_)
  //                     -((~((uint8_t)crc1))&0b00000111)),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_0==ad_data_channel_pair*2),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_1==(ad_data_channel_pair*2+1)),8);
  //                  print2_symbol(';');
  //                  if(
  //                      ((!(((uint8_t)crc8_)
  //                          -((uint8_t)crc1)))
  //                     ||(!(((uint8_t)crc8_)
  //                         -((~((uint8_t)crc1))&0b00000111)))
  //                      )
  //                  &&(
  //                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
  //                        ((chan_0==ad_data_channel_pair)))
  //                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
  //                      ((chan_1==(ad_data_channel_pair+1))))
  //                  ))
                    if(
                        ((!(((uint8_t)crc8_)
                            -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                                |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                       ||(!(((uint8_t)crc8_)
                           -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                               |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                        )
                    &&(
                      (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                          ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]
                                            &((FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_NOERRORS)?0b11110000:0b01110000))>>4)==
                                                ad_data_channel_pair*2)))
                    &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                        ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]
                                          &((FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_NOERRORS)?0b11110000:0b01110000))>>4)==
                                              ad_data_channel_pair*2+1)))
                    ))
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                      crc_something_ok = 1;
                    }
                    else
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                      crc_ok = 0;
                    }
                  }
                  if((crc_ok)||((crc_something_ok)&&(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_SOMETHING)))
                  {
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
                  }
                  else
                  {
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
                  }
                }
              }

              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                for(int ad_data_channel = 0; ad_data_channel < uint8_ad_chan_number; ad_data_channel ++)
                {
                  print_hex(datas[ad_adc][ad_data_channel * 4 + 0], 8);//          print2_symbol(';');
                  print_hex(datas[ad_adc][ad_data_channel * 4 + 1], 8);//          print2_symbol(';');
                  print_hex(datas[ad_adc][ad_data_channel * 4 + 2], 8);//          print2_symbol(';');
                  print_hex(datas[ad_adc][ad_data_channel * 4 + 3], 8);//          print2_symbol(';');
                  print_symbol(';');
                }
                print_symbol(';');
              }
              print_symbol(';');

              print_line();

            }
            if(FREESMARTEEG_OUT & FREESMARTEEG_STABLE_CRC)
//            if(0)
            {

//              int crc_pair_ok[uint8_ad_adc_number][4];

//              if(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_PIN)
              {

                for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
                {
                  int crc_ok = 1;
                  int crc_something_ok = 0;
                  for(int ad_data_channel_pair = 0; ad_data_channel_pair < (uint8_ad_chan_number / 2); ad_data_channel_pair ++)
                  {

                    uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

                    if(
                        ((!(((uint8_t)crc8_)
                            -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                                |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                       ||(!(((uint8_t)crc8_)
                           -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                               |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                        )
                    &&(
                      (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                          ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]
                                            &((FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_NOERRORS)?0b11110000:0b01110000))>>4)==
                                                ad_data_channel_pair*2)))
                    &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                        ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]
                                          &((FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_NOERRORS)?0b11110000:0b01110000))>>4)==
                                              ad_data_channel_pair*2+1)))
                    ))
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                      crc_something_ok = 1;
                      stable_crc[ad_adc][ad_data_channel_pair]++;
                    }
                    else
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                      crc_ok = 0;
                    }
                  }
                  if((crc_ok)||((crc_something_ok)&&(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_SOMETHING)))
                  {
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
                  }
                  else
                  {
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
                  }
                }
              }
              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                for(int ad_data_channel_pair = 0; ad_data_channel_pair < (uint8_ad_chan_number / 2); ad_data_channel_pair ++)
                {
//                  print_hex(0xff*((float)(data_counter))/((float)(stable_crc[ad_adc][ad_data_channel_pair])),8);
                  print_hex(((data_counter))-((stable_crc[ad_adc][ad_data_channel_pair])),8);
                  print_symbol(';');
                }
              }

              print_line();

            }
            if(FREESMARTEEG_OUT & FREESMARTEEG_OPENVIBE_TELNET)
//            if(0)
            {

//              int crc_pair_ok[uint8_ad_adc_number][4];

              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                int crc_ok = 1;
                for(int ad_data_channel_pair = 0; ad_data_channel_pair < 4; ad_data_channel_pair ++)
                {

                  uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

//                  uint8_t crc1=((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
//                      |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)));
//                  uint8_t chan_0=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b01110000)>>4);
//                  uint8_t chan_1=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b01110000)>>4);
//                  print2_hex(chan_0,8);
//                  print2_symbol(':');
//                  print2_hex(crc8_,8);
//                  print2_symbol('-');
//                  print2_hex(crc1,8);
////                  print2_symbol(';');
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(!(((uint8_t)crc8_)
//                      -((uint8_t)crc1))),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)!(((uint8_t)crc8_)
//                     -((~((uint8_t)crc1))&0b00000111)),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(chan_0==ad_data_channel_pair*2),8);
//                  print2_symbol(',');
//                  print2_hex((uint8_t)(chan_1==(ad_data_channel_pair*2+1)),8);
//                  print2_symbol(';');
//                  if(
//                      ((!(((uint8_t)crc8_)
//                          -((uint8_t)crc1)))
//                     ||(!(((uint8_t)crc8_)
//                         -((~((uint8_t)crc1))&0b00000111)))
//                      )
//                  &&(
//                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
//                        ((chan_0==ad_data_channel_pair)))
//                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
//                      ((chan_1==(ad_data_channel_pair+1))))
//                  ))
                  if(
                      ((!(((uint8_t)crc8_)
                          -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                              |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                     ||(!(((uint8_t)crc8_)
                         -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                             |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                      )
                  &&(
                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                        ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b11110000)>>4)==ad_data_channel_pair*2)))
                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                      ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b11110000)>>4)==ad_data_channel_pair*2+1)))
                  ))
                  {
//                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                  }
                  else
                  {
//                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                    crc_ok = 0;
                  }
                }
                if(crc_ok)
                {
                  HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
                }
                else
                {
                  HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);

//                  //                  ad7779_do_spi_soft_reset(devices[ad_adc]);
//                                    HAL_GPIO_WritePin(reset_ports[ad_adc], reset_pins[ad_adc], GPIO_PIN_RESET);
//                                    HAL_Delay(1);
//                                    HAL_GPIO_WritePin(reset_ports[ad_adc], reset_pins[ad_adc], GPIO_PIN_SET);
//
//                                    ad7779_setup(&devices[ad_adc], init_params[ad_adc]);
//
//                                      {
//                                        aTxBuffer[0] = AD7779_REG_GENERAL_USER_CONFIG_3;
//                                        aTxBuffer[1] = AD7779_SPI_SLAVE_MODE_EN;
//
//                              //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//                                        if(HAL_SPI_TransmitReceive_DMA(devices[ad_adc]->spi_dev.dev, aTxBuffer, datas[ad_adc], 2) != HAL_OK)
//                              //          if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, 2, 5000) != HAL_OK)
//                                        {
//                                          Error_Handler();
//                                        }
//                              //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//                                        while (HAL_SPI_GetState(devices[ad_adc]->spi_dev.dev) != HAL_SPI_STATE_READY)
//                                        {
//                                        }
//
//                                      }
//
//                                      {
//                                        aTxBuffer[0] = AD7779_REG_GEN_ERR_REG_1_EN;
//                                        aTxBuffer[1] = AD7779_SPI_CRC_TEST_EN;
//
//                              //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_RESET);
//                                        if(HAL_SPI_TransmitReceive_DMA(devices[ad_adc]->spi_dev.dev, aTxBuffer, datas[ad_adc], 2) != HAL_OK)
//                              //          if(HAL_SPI_TransmitReceive(device2->spi_dev.dev, aTxBuffer, data2, 2, 5000) != HAL_OK)
//                                        {
//                                          Error_Handler();
//                                        }
//                              //          HAL_GPIO_WritePin(device2->spi_dev.chip_select_port, device2->spi_dev.chip_select_pin, GPIO_PIN_SET);
//
//                                        while (HAL_SPI_GetState(devices[ad_adc]->spi_dev.dev) != HAL_SPI_STATE_READY)
//                                        {
//                                        }
//
//                                      }


                }
              }

              int ad_adc = 0;
//              for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
              {
                int ad_data_channel = 0;
//                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                {
//                  datas_out[ad_adc][ad_data_channel * 4 + 0]=datas[ad_adc][ad_data_channel * 4 + 3];
//                  datas_out[ad_adc][ad_data_channel * 4 + 1]=datas[ad_adc][ad_data_channel * 4 + 2];
//                  datas_out[ad_adc][ad_data_channel * 4 + 2]=datas[ad_adc][ad_data_channel * 4 + 1]&0b01111111;
//                  datas_out[ad_adc][ad_data_channel * 4 + 3]=datas[ad_adc][ad_data_channel * 4 + 1]&0b10000000;

//                  if(datas[ad_adc][ad_data_channel * 4 + 1]&0b10000000)
//                  {
//                    datas_out[ad_adc][ad_data_channel * 4 + 0]=0b11111111;
//                  }
//                  else
//                  {
//                    datas_out[ad_adc][ad_data_channel * 4 + 0]=0b00000000;
//                  }
//                  datas_out[ad_adc][ad_data_channel * 4 + 1]=datas[ad_adc][ad_data_channel * 4 + 1]&0b01111111;
//                  datas_out[ad_adc][ad_data_channel * 4 + 2]=datas[ad_adc][ad_data_channel * 4 + 2];
//                  datas_out[ad_adc][ad_data_channel * 4 + 3]=datas[ad_adc][ad_data_channel * 4 + 3];
                  datas_out[ad_adc][ad_data_channel * 4 + 0]=datas[ad_adc][ad_data_channel * 4 + 1];
                  datas_out[ad_adc][ad_data_channel * 4 + 1]=datas[ad_adc][ad_data_channel * 4 + 2];
                  datas_out[ad_adc][ad_data_channel * 4 + 2]=datas[ad_adc][ad_data_channel * 4 + 3];
                  datas_out[ad_adc][ad_data_channel * 4 + 3]=0;
                }
              }
              while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
//              while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
              {
              }
              if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)datas_out, 4) != HAL_OK)
//              if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)datas_out, 4*8*4) != HAL_OK)
              {
                Error_Handler();
              }
//              while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
//              {
//              }
            }//FREESMARTEEG_OPENVIBE_TELNET

            if(FREESMARTEEG_OUT & FREESMARTEEG_BLINK)
//            if(0)
            {
    //        int ad_data_channel = 0;
              int ad_data_channel = 0;
    //          if((data[(ad_data_channel * 4)] & 0b01110000) == 0)
    /*          if(((data[((ad_data_channel + 0) * 4) + 2] & 0b01110000) >> 4 == ad_data_channel + 1 + 0)
              && ((data[((ad_data_channel + 1) * 4) + 2] & 0b01110000) >> 4 == ad_data_channel + 1 + 1)
              && ((data[((ad_data_channel + 2) * 4) + 2] & 0b01110000) >> 4 == ad_data_channel + 1 + 2)
              && ((data[((ad_data_channel + 3) * 4) + 2] & 0b01110000) >> 4 == ad_data_channel + 1 + 3)
              && ((data[((ad_data_channel + 4) * 4) + 2] & 0b01110000) >> 4 == ad_data_channel + 1 + 4)
              )*/
              if(((data1[((ad_data_channel + 0) * 4)] & 0b01110000) >> 4 == ad_data_channel + 0)
              && ((data1[((ad_data_channel + 1) * 4)] & 0b01110000) >> 4 == ad_data_channel + 1)
              && ((data1[((ad_data_channel + 2) * 4)] & 0b01110000) >> 4 == ad_data_channel + 2)
              && ((data1[((ad_data_channel + 3) * 4)] & 0b01110000) >> 4 == ad_data_channel + 3)
              && ((data1[((ad_data_channel + 4) * 4)] & 0b01110000) >> 4 == ad_data_channel + 4)
              && ((data1[((ad_data_channel + 5) * 4)] & 0b01110000) >> 4 == ad_data_channel + 5)
              && ((data1[((ad_data_channel + 6) * 4)] & 0b01110000) >> 4 == ad_data_channel + 6)
              && ((data1[((ad_data_channel + 7) * 4)] & 0b01110000) >> 4 == ad_data_channel + 7)
              )
              {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
              }
              else
              {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
              }
            }

            if(FREESMARTEEG_OUT & FREESMARTEEG_OPENBCI)
//            if(0)//openbci
            {

              if(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_PIN)
              {
                crc8_01=crc8da_7(&(datas[0][0]));

                if(
                    ((!(((uint8_t)crc8_01)-((uint8_t)((datas[0][0]<<4)|(datas[0][4]&0x0f)))))
                   ||(!(((uint8_t)crc8_01)-((~((uint8_t)((datas[0][0]<<4)|(datas[0][4]&0x0f))))&0b00000111)))
                    )
                &&(
                  (//((datas[0][0]>=0b10000000)&&(datas[0][0]<0b10010000))||
                      ((datas[0][0]>=0b00000000)&&(datas[0][0]<0b00010000)))
                &&(//((datas[0][4]>=0b10010000)&&(datas[0][4]<0b10100000))||
                    ((datas[0][4]>=0b00010000)&&(datas[0][4]<0b00100000)))
                ))
                {
                  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
                }
                else
                {
                  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
                }
              }

//              crc8_01=crc8da_7(&(datas[1][0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((datas[1][0]<<4)|(datas[1][4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((datas[1][0]<<4)|(datas[1][4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((datas[1][0]>=0b10000000)&&(datas[1][0]<0b10010000))||
//                    ((datas[1][0]>=0b00000000)&&(datas[1][0]<0b00010000)))
//              &&(//((datas[1][4]>=0b10010000)&&(datas[1][4]<0b10100000))||
//                  ((datas[1][4]>=0b00010000)&&(datas[1][4]<0b00100000)))
//              ))
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//              }
//              else
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
//              }

//              crc8_01=crc8da_7(&(data[0]));
//              crc8_23=crc8da_7(&(data[8]));
//              crc8_45=crc8da_7(&(data[16]));
//              crc8_67=crc8da_7(&(data[24]));
//
//              if(((!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//                &&(!(((uint8_t)crc8_23)-((uint8_t)((data[8]<<4)|(data[12]&0x0f)))))
//                &&(!(((uint8_t)crc8_45)-((uint8_t)((data[16]<<4)|(data[20]&0x0f)))))
//                &&(!(((uint8_t)crc8_67)-((uint8_t)((data[24]<<4)|(data[28]&0x0f))))))
//              && (
//                (((data[0]>=0b10000000)&&(data[0]<0b10010000))||((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//              &&(((data[4]>=0b10010000)&&(data[4]<0b10100000))||((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//              &&(((data[8]>=0b10100000)&&(data[8]<0b10110000))||((data[8]>=0b00100000)&&(data[8]<0b00110000)))
//              &&(((data[12]>=0b10110000)&&(data[12]<0b11000000))||((data[12]>=0b00110000)&&(data[12]<0b01000000)))
//              &&(((data[16]>=0b11000000)&&(data[16]<0b11010000))||((data[16]>=0b01000000)&&(data[16]<0b01010000)))
//              &&(((data[20]>=0b11010000)&&(data[20]<0b11100000))||((data[20]>=0b01010000)&&(data[20]<0b01100000)))
//              &&(((data[24]>=0b11100000)&&(data[24]<0b11110000))||((data[24]>=0b01100000)&&(data[24]<0b01110000)))
//              &&(((data[28]>=0b11110000)&&(data[28]<=0b11111111))||((data[28]>=0b01110000)&&(data[28]<=0b01111111)))
//              ))
              if(1)
              {
                const uint32_t uint8_data_number = 2 + 8 * 3 + 3 * 2 + 1;
//                const uint32_t uint8_data_number = 2 + 8 * 3 + 8 * 3 + 3 * 2 + 1;
                uint8_t dataBuffer[uint8_data_number];

                dataBuffer[0] = 0xA0;
                dataBuffer[1] = ui8SampleNumber++;

                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
                {
                  dataBuffer[2 + ad_data_channel * 3 + 0] = datas[0][ad_data_channel * 4 + 1];
                  dataBuffer[2 + ad_data_channel * 3 + 1] = datas[0][ad_data_channel * 4 + 2];
                  dataBuffer[2 + ad_data_channel * 3 + 2] = datas[0][ad_data_channel * 4 + 3];
                }
//                for(int ad_data_channel = 0; ad_data_channel < 8; ad_data_channel ++)
//                {
//                  dataBuffer[2 + ad_data_channel * 3 + 0] = data2[ad_data_channel * 4 + 1];
//                  dataBuffer[2 + ad_data_channel * 3 + 1] = data2[ad_data_channel * 4 + 2];
//                  dataBuffer[2 + ad_data_channel * 3 + 2] = data2[ad_data_channel * 4 + 3];
//                }
                for(int accel_data_channel = 0; accel_data_channel < 3; accel_data_channel ++)
                {
                  dataBuffer[2 + 8 * 3 + accel_data_channel * 2 + 0] = 0;
                  dataBuffer[2 + 8 * 3 + accel_data_channel * 2 + 1] = 0;
                }
                dataBuffer[2 + 8 * 3 + 3 * 2 + 0] = 0xC0;

                if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART1)
                {
                  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
                  {
                  }
                  if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
                  {
                    Error_Handler();
                  }
                }
                if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART2)
                {
                  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
                  {
                  }
                  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
                  {
                    Error_Handler();
                  }
                }


//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//              }
//              else
//              {
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
              }


            }//FREESMARTEEG_OPENBCI

            if(FREESMARTEEG_OUT & FREESMARTEEG_OPENVIBE_FREEEEG32)
//            if(0)//openbci
            {

              if(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_PIN)
              {
//              int crc_pair_ok[uint8_ad_adc_number][4];

                for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
                {
                  int crc_ok = 1;
                  int crc_something_ok = 0;
                  for(int ad_data_channel_pair = 0; ad_data_channel_pair < (uint8_ad_chan_number / 2); ad_data_channel_pair ++)
                  {
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 1];
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 2];
  //                  datas[ad_adc][ad_data_channel_pair * 2 * 4 + 3];

                    uint8_t crc8_=crc8da_7(&(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]));

  //                  uint8_t crc1=((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
  //                      |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)));
  //                  uint8_t chan_0=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]&0b01110000)>>4);
  //                  uint8_t chan_1=((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0b01110000)>>4);
  //                  print2_hex(chan_0,8);
  //                  print2_symbol(':');
  //                  print2_hex(crc8_,8);
  //                  print2_symbol('-');
  //                  print2_hex(crc1,8);
  ////                  print2_symbol(';');
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(!(((uint8_t)crc8_)
  //                      -((uint8_t)crc1))),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)!(((uint8_t)crc8_)
  //                     -((~((uint8_t)crc1))&0b00000111)),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_0==ad_data_channel_pair*2),8);
  //                  print2_symbol(',');
  //                  print2_hex((uint8_t)(chan_1==(ad_data_channel_pair*2+1)),8);
  //                  print2_symbol(';');
  //                  if(
  //                      ((!(((uint8_t)crc8_)
  //                          -((uint8_t)crc1)))
  //                     ||(!(((uint8_t)crc8_)
  //                         -((~((uint8_t)crc1))&0b00000111)))
  //                      )
  //                  &&(
  //                    (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
  //                        ((chan_0==ad_data_channel_pair)))
  //                  &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
  //                      ((chan_1==(ad_data_channel_pair+1))))
  //                  ))
                    if(
                        ((!(((uint8_t)crc8_)
                            -((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                                |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f)))))
                       ||(!(((uint8_t)crc8_)
                           -((~((uint8_t)((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]<<4)
                               |(datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]&0x0f))))&0b00000111)))
                        )
                    &&(
                      (//((data[0]>=0b10000000)&&(data[0]<0b10010000))||
                          ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 0]
                                            &((FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_NOERRORS)?0b11110000:0b01110000))>>4)==
                                                ad_data_channel_pair*2)))
                    &&(//((data[4]>=0b10010000)&&(data[4]<0b10100000))||
                        ((((datas[ad_adc][ad_data_channel_pair * 2 * 4 + 4]
                                          &((FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_NOERRORS)?0b11110000:0b01110000))>>4)==
                                              ad_data_channel_pair*2+1)))
                    ))
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 1;
                      crc_something_ok = 1;
                    }
                    else
                    {
  //                    crc_pair_ok[ad_adc][ad_data_channel_pair] = 0;
                      crc_ok = 0;
                    }
                  }
                  if((crc_ok)||((crc_something_ok)&&(FREESMARTEEG_SHOW_CRC & FREESMARTEEG_SHOW_CRC_SOMETHING)))
                  {
  //                  print2_symbol('1');
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
  //                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
                  }
                  else
                  {
  //                  print2_symbol('0');
                    HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
  //                  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

                  }
  //                print2_symbol(';');
                }
              }

//              crc8_01=crc8da_7(&(datas[0][0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((datas[0][0]<<4)|(datas[0][4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((datas[0][0]<<4)|(datas[0][4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((datas[0][0]>=0b10000000)&&(datas[0][0]<0b10010000))||
//                    ((datas[0][0]>=0b00000000)&&(datas[0][0]<0b00010000)))
//              &&(//((datas[0][4]>=0b10010000)&&(datas[0][4]<0b10100000))||
//                  ((datas[0][4]>=0b00010000)&&(datas[0][4]<0b00100000)))
//              ))
//              {
//                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
//              }
//              else
//              {
//                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//              }

//              crc8_01=crc8da_7(&(datas[1][0]));
//
//              if(
//                  ((!(((uint8_t)crc8_01)-((uint8_t)((datas[1][0]<<4)|(datas[1][4]&0x0f)))))
//                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((datas[1][0]<<4)|(datas[1][4]&0x0f))))&0b00000111)))
//                  )
//              &&(
//                (//((datas[1][0]>=0b10000000)&&(datas[1][0]<0b10010000))||
//                    ((datas[1][0]>=0b00000000)&&(datas[1][0]<0b00010000)))
//              &&(//((datas[1][4]>=0b10010000)&&(datas[1][4]<0b10100000))||
//                  ((datas[1][4]>=0b00010000)&&(datas[1][4]<0b00100000)))
//              ))
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
//              }
//              else
//              {
////                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
//              }

//              crc8_01=crc8da_7(&(data[0]));
//              crc8_23=crc8da_7(&(data[8]));
//              crc8_45=crc8da_7(&(data[16]));
//              crc8_67=crc8da_7(&(data[24]));
//
//              if(((!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//                &&(!(((uint8_t)crc8_23)-((uint8_t)((data[8]<<4)|(data[12]&0x0f)))))
//                &&(!(((uint8_t)crc8_45)-((uint8_t)((data[16]<<4)|(data[20]&0x0f)))))
//                &&(!(((uint8_t)crc8_67)-((uint8_t)((data[24]<<4)|(data[28]&0x0f))))))
//              && (
//                (((data[0]>=0b10000000)&&(data[0]<0b10010000))||((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//              &&(((data[4]>=0b10010000)&&(data[4]<0b10100000))||((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//              &&(((data[8]>=0b10100000)&&(data[8]<0b10110000))||((data[8]>=0b00100000)&&(data[8]<0b00110000)))
//              &&(((data[12]>=0b10110000)&&(data[12]<0b11000000))||((data[12]>=0b00110000)&&(data[12]<0b01000000)))
//              &&(((data[16]>=0b11000000)&&(data[16]<0b11010000))||((data[16]>=0b01000000)&&(data[16]<0b01010000)))
//              &&(((data[20]>=0b11010000)&&(data[20]<0b11100000))||((data[20]>=0b01010000)&&(data[20]<0b01100000)))
//              &&(((data[24]>=0b11100000)&&(data[24]<0b11110000))||((data[24]>=0b01100000)&&(data[24]<0b01110000)))
//              &&(((data[28]>=0b11110000)&&(data[28]<=0b11111111))||((data[28]>=0b01110000)&&(data[28]<=0b01111111)))
//              ))

//              if(average_number>1)
//              {
//                for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
//                {
//                  for(int ad_data_channel = 0; ad_data_channel < uint8_ad_chan_number; ad_data_channel ++)
//                  {
//                    int32_t sample_int32_out = (datas[ad_adc][ad_data_channel * 4 + 1]<<24)
//                            |(datas[ad_adc][ad_data_channel * 4 + 2]<<16)
//                            |(datas[ad_adc][ad_data_channel * 4 + 3]<<8);
//                    if ((sample_int32_out & 0x80000000) > 0) {
//                      sample_int32_out |= 0x000000FF;
//                    } else {
//                      sample_int32_out &= 0xFFFFFF00;
//                    }
////                    samples_int32_average[(data_counter-1)%average_number][ad_adc][ad_data_channel]=sample_int32_out;
//                    samples_int32_average[(data_counter-1)%average_number][ad_adc][ad_data_channel]=sample_int32_out/average_number;
//                  }
//                }
//                if((data_counter-1)%average_number==average_number-1)
//                {
//                  for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
//                  {
//                    for(int ad_data_channel = 0; ad_data_channel < uint8_ad_chan_number; ad_data_channel ++)
//                    {
//                      int64_t sample_int64_out_sum=0;
//                      for(int average = 0; average < average_number; average ++)
//                      {
//                        sample_int64_out_sum+=samples_int32_average[average][ad_adc][ad_data_channel];
//                      }
//                      int32_t sample_int32_out = sample_int64_out_sum / average_number;
////                      if(
////                          (datas[ad_adc][ad_data_channel * 4 + 1] == (uint8_t)((sample_int32_out>>8)&0xff))
////                        &&(datas[ad_adc][ad_data_channel * 4 + 2] == (uint8_t)((sample_int32_out>>16)&0xff))
////                        &&(datas[ad_adc][ad_data_channel * 4 + 3] == (uint8_t)((sample_int32_out>>24)&0xff))
////                        )
////                      {
////                        HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_SET);
////                      }
////                      else
////                      {
////                        HAL_GPIO_WritePin(led_ports[ad_adc], led_pins[ad_adc], GPIO_PIN_RESET);
////                      }
////                      print2_hex(datas[ad_adc][ad_data_channel * 4 + 1],8);
////                      print2_symbol(';');
////                      print2_hex((uint8_t)((sample_int32_out>>8)&0xff),8);
////                      print2_symbol(';');
////                      print2_symbol(';');
////                      print2_hex(datas[ad_adc][ad_data_channel * 4 + 2],8);
////                      print2_symbol(';');
////                      print2_hex((uint8_t)((sample_int32_out>>16)&0xff),8);
////                      print2_symbol(';');
////                      print2_symbol(';');
////                      print2_hex(datas[ad_adc][ad_data_channel * 4 + 3],8);
////                      print2_symbol(';');
////                      print2_hex((uint8_t)((sample_int32_out>>24)&0xff),8);
////                      print2_symbol(';');
////                      print2_symbol(';');
//                      datas[ad_adc][ad_data_channel * 4 + 3] = (uint8_t)((sample_int32_out>>8)&0xff);
//                      datas[ad_adc][ad_data_channel * 4 + 2] = (uint8_t)((sample_int32_out>>16)&0xff);
//                      datas[ad_adc][ad_data_channel * 4 + 1] = (uint8_t)((sample_int32_out>>24)&0xff);
//                    }
////                    print2_symbol(';');
//                  }
////                  print2_line();
//                }
//              }
//              if(((data_counter-1)%average_number==average_number-1) || (average_number<=1))
              if(1)
              {
                const uint32_t uint8_data_number = 2 + uint8_ad_chan_number * 3 * uint8_ad_adc_number + uint8_accel_chan_number * 2 + 1;
                uint8_t dataBuffer[uint8_data_number];

                dataBuffer[0] = 0xA0;
                dataBuffer[1] = ui8SampleNumber++;

                for(int ad_adc = 0; ad_adc < uint8_ad_adc_number; ad_adc ++)
                {
                  for(int ad_data_channel = 0; ad_data_channel < uint8_ad_chan_number; ad_data_channel ++)
                  {
                    dataBuffer[2 + uint8_ad_chan_number * 3 * ad_adc + ad_data_channel * 3 + 0] = datas[ad_adc][ad_data_channel * 4 + 1];
                    dataBuffer[2 + uint8_ad_chan_number * 3 * ad_adc + ad_data_channel * 3 + 1] = datas[ad_adc][ad_data_channel * 4 + 2];
                    dataBuffer[2 + uint8_ad_chan_number * 3 * ad_adc + ad_data_channel * 3 + 2] = datas[ad_adc][ad_data_channel * 4 + 3];
                  }
                }
                for(int accel_data_channel = 0; accel_data_channel < uint8_accel_chan_number; accel_data_channel ++)
                {
                  dataBuffer[2 + uint8_ad_chan_number * 3 * uint8_ad_adc_number + accel_data_channel * 2 + 0] = 0;
                  dataBuffer[2 + uint8_ad_chan_number * 3 * uint8_ad_adc_number + accel_data_channel * 2 + 1] = 0;
                }
                dataBuffer[2 + uint8_ad_chan_number * 3 * uint8_ad_adc_number + uint8_accel_chan_number * 2 + 0] = 0xC0;

                if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART1)
                {
                  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
                  {
                  }
                  if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
                  {
                    Error_Handler();
                  }
                }
                if(FREESMARTEEG_SEND & FREESMARTEEG_SEND_UART2)
                {
                  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
                  {
                  }
                  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
                  {
                    Error_Handler();
                  }
                }


//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//              }
//              else
//              {
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
              }


            }//FREESMARTEEG_OPENVIBE_FREEEEG32

//            if(1)//crc processing
            if(FREESMARTEEG_OUT==FREESMARTEEG_PROCESSING)
            {

              crc8_01=crc8da_7(&(data1[0]));

              if(
                  ((!(((uint8_t)crc8_01)-((uint8_t)((data1[0]<<4)|(data1[4]&0x0f)))))
                 ||(!(((uint8_t)crc8_01)-((~((uint8_t)((data1[0]<<4)|(data1[4]&0x0f))))&0b00000111)))
                  )
              &&(
                (//((data1[0]>=0b10000000)&&(data1[0]<0b10010000))||
                    ((data1[0]>=0b00000000)&&(data1[0]<0b00010000)))
              &&(//((data1[4]>=0b10010000)&&(data1[4]<0b10100000))||
                  ((data1[4]>=0b00010000)&&(data1[4]<0b00100000)))
              ))
              {
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
              }
              else
              {
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
              }

//              crc8_01=crc8da_7(&(data[0]));
//              if((!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//              &&(
//                (((data[0]>=0b10000000)&&(data[0]<0b10010000))||((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//              &&(((data[4]>=0b10010000)&&(data[4]<0b10100000))||((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//              ))
//              {

//              crc8_01=crc8da_7(&(data[0]));
//              crc8_23=crc8da_7(&(data[8]));
//              crc8_45=crc8da_7(&(data[16]));
//              crc8_67=crc8da_7(&(data[24]));
//
//              if(((!(((uint8_t)crc8_01)-((uint8_t)((data[0]<<4)|(data[4]&0x0f)))))
//                &&(!(((uint8_t)crc8_23)-((uint8_t)((data[8]<<4)|(data[12]&0x0f)))))
//                &&(!(((uint8_t)crc8_45)-((uint8_t)((data[16]<<4)|(data[20]&0x0f)))))
//                &&(!(((uint8_t)crc8_67)-((uint8_t)((data[24]<<4)|(data[28]&0x0f))))))
//              && (
//                (((data[0]>=0b10000000)&&(data[0]<0b10010000))||((data[0]>=0b00000000)&&(data[0]<0b00010000)))
//              &&(((data[4]>=0b10010000)&&(data[4]<0b10100000))||((data[4]>=0b00010000)&&(data[4]<0b00100000)))
//              &&(((data[8]>=0b10100000)&&(data[8]<0b10110000))||((data[8]>=0b00100000)&&(data[8]<0b00110000)))
//              &&(((data[12]>=0b10110000)&&(data[12]<0b11000000))||((data[12]>=0b00110000)&&(data[12]<0b01000000)))
//              &&(((data[16]>=0b11000000)&&(data[16]<0b11010000))||((data[16]>=0b01000000)&&(data[16]<0b01010000)))
//              &&(((data[20]>=0b11010000)&&(data[20]<0b11100000))||((data[20]>=0b01010000)&&(data[20]<0b01100000)))
//              &&(((data[24]>=0b11100000)&&(data[24]<0b11110000))||((data[24]>=0b01100000)&&(data[24]<0b01110000)))
//              &&(((data[28]>=0b11110000)&&(data[28]<=0b11111111))||((data[28]>=0b01110000)&&(data[28]<=0b01111111)))
//              ))
              if(1)
              {

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

                if(1)
                {
                  const uint32_t uint8_data_number = 3+4;
                  uint8_t dataBuffer[uint8_data_number];
                  dataBuffer[0] = 0xff;
                  dataBuffer[1] = 0xfe;
                  dataBuffer[2] = 0xfd;
                  dataBuffer[3] = data1[0];
                  dataBuffer[4] = data1[1];
                  dataBuffer[5] = data1[2];
                  dataBuffer[6] = data1[3];
                //  HAL_UART_Transmit_DMA(&huart1, dataBuffer, uint8_data_number);
//                  HAL_UART_Transmit(&huart1, (uint8_t*)dataBuffer, uint8_data_number, 5000);
                  if(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)dataBuffer, uint8_data_number) != HAL_OK)
                  {
                    Error_Handler();
                  }
                  while (HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY)
                  {
                  }
                }

//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//              }
//              else
//              {
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
//                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
              }
            }





    //        HAL_Delay(1000/250);
//            HAL_Delay(50);
          }
        }
    //    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI3 init function */
static void MX_SPI3_Init(void)
{

  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI4 init function */
static void MX_SPI4_Init(void)
{

  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
//  huart1.Init.BaudRate = 2000000;
//  huart1.Init.BaudRate = 1509433;
  huart1.Init.BaudRate = 930232;
//  huart1.Init.BaudRate = 462427;
//  huart1.Init.BaudRate = 115273;
//  huart1.Init.BaudRate = 1500000;
//  huart1.Init.BaudRate = 921600;
//  huart1.Init.BaudRate = 460800;
//  huart1.Init.BaudRate = 230400;
//  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
//  huart2.Init.BaudRate = 2000000;
  huart2.Init.BaudRate = 930232;
//  huart2.Init.BaudRate = 462427;
//  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, AD4_RESET_Pin|AD4_START_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_YELLOW_Pin|AD_START_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, AD_RESET_Pin|LED_RED_Pin|LED_GREEN_Pin|AD3_START_Pin 
                          |AD3_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, AD2_START_Pin|AD2_RESET_Pin|LED_BLUE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : AD4_RESET_Pin AD4_START_Pin */
  GPIO_InitStruct.Pin = AD4_RESET_Pin|AD4_START_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_YELLOW_Pin AD_START_Pin */
  GPIO_InitStruct.Pin = LED_YELLOW_Pin|AD_START_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : AD_DRDY_Pin */
  GPIO_InitStruct.Pin = AD_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(AD_DRDY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : AD_RESET_Pin LED_RED_Pin LED_GREEN_Pin AD3_START_Pin 
                           AD3_RESET_Pin */
  GPIO_InitStruct.Pin = AD_RESET_Pin|LED_RED_Pin|LED_GREEN_Pin|AD3_START_Pin 
                          |AD3_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : AD2_DRDY_Pin */
  GPIO_InitStruct.Pin = AD2_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(AD2_DRDY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : AD2_START_Pin AD2_RESET_Pin LED_BLUE_Pin */
  GPIO_InitStruct.Pin = AD2_START_Pin|AD2_RESET_Pin|LED_BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : ESP_RESET_Pin */
  GPIO_InitStruct.Pin = ESP_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ESP_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : AD3_DRDY_Pin */
  GPIO_InitStruct.Pin = AD3_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(AD3_DRDY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : AD4_DRDY_Pin */
  GPIO_InitStruct.Pin = AD4_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(AD4_DRDY_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
