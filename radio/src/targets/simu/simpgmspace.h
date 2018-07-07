/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _SIMPGMSPACE_H_
#define _SIMPGMSPACE_H_

#ifndef __GNUC__
#include <windows.h>
#define sleep(x) Sleep(x)
#define strcasecmp  _stricmp
#define strncasecmp _tcsnicmp
#define chdir  _chdir
#define getcwd _getcwd
#else
#include <unistd.h>
#define sleep(x) usleep(1000*x)
#endif

#ifdef SIMU_EXCEPTIONS
extern char * main_thread_error;
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#if defined(_MSC_VER) || !defined(__GNUC__)
#define write_backtrace(output)
#else
#include <execinfo.h>
inline void write_backtrace(char *output)
{
  void *buf[16];
  char **s;
  int n = backtrace(buf, 16);
  s = backtrace_symbols(buf, n);
  if (s) {
    for (int i=0; i<n; ++i) {
      sprintf(output+strlen(output), "%02i: %s\n", i, s[i]);
    }
  }
}
#endif
void sig(int sgn)
{
  main_thread_error = (char *)malloc(2048);
  sprintf(main_thread_error,"Signal %d caught\n", sgn);
  write_backtrace(main_thread_error);
  throw std::exception();
}
#define assert(x) do { if (!(x)) { main_thread_error = (char *)malloc(2048); sprintf(main_thread_error, "Assert failed, %s:%d: %s\n", __FILE__, __LINE__, #x); write_backtrace(main_thread_error); throw std::exception(); } } while(0)
#else
#include <assert.h>
#endif

#include <inttypes.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <errno.h>

#undef min
#undef max

#define APM
#define __REV

typedef const unsigned char pm_uchar;
typedef const char pm_char;
typedef const uint16_t pm_uint16_t;
typedef const uint8_t pm_uint8_t;
typedef const int16_t pm_int16_t;
typedef const int8_t pm_int8_t;

#if defined(STM32)
extern GPIO_TypeDef gpioa, gpiob, gpioc, gpiod, gpioe, gpiof, gpiog, gpioh, gpioi, gpioj;
extern TIM_TypeDef tim1, tim2, tim3, tim4, tim5, tim6, tim7, tim8, tim9, tim10;
extern USART_TypeDef Usart0, Usart1, Usart2, Usart3, Usart4;
extern RCC_TypeDef rcc;
extern DMA_Stream_TypeDef dma1_stream0, dma1_stream1, dma1_stream2, dma1_stream3, dma1_stream5, dma1_stream7, dma2_stream1, dma2_stream2, dma2_stream5, dma2_stream6, dma2_stream7;
extern DMA_TypeDef dma2;
extern SysTick_Type systick;
#undef SysTick
#define SysTick (&systick)
#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOD
#undef GPIOE
#undef GPIOF
#undef GPIOG
#undef GPIOH
#undef GPIOI
#undef GPIOJ
#define GPIOA (&gpioa)
#define GPIOB (&gpiob)
#define GPIOC (&gpioc)
#define GPIOD (&gpiod)
#define GPIOE (&gpioe)
#define GPIOF (&gpiof)
#define GPIOG (&gpiog)
#define GPIOH (&gpioh)
#define GPIOI (&gpioi)
#define GPIOJ (&gpioj)
#undef TIM1
#undef TIM2
#undef TIM3
#undef TIM4
#undef TIM5
#undef TIM6
#undef TIM7
#undef TIM8
#undef TIM9
#undef TIM10
#define TIM1 (&tim1)
#define TIM2 (&tim2)
#define TIM3 (&tim3)
#define TIM4 (&tim4)
#define TIM5 (&tim4)
#define TIM6 (&tim4)
#define TIM7 (&tim4)
#define TIM8 (&tim8)
#define TIM9 (&tim9)
#define TIM10 (&tim10)
#undef USART0
#undef USART1
#undef USART2
#undef USART3
#define USART0 (&Usart0)
#define USART1 (&Usart1)
#define USART2 (&Usart2)
#define USART3 (&Usart3)
#undef RCC
#define RCC (&rcc)
#undef DMA1_Stream0
#undef DMA1_Stream1
#undef DMA1_Stream2
#undef DMA1_Stream3
#undef DMA1_Stream5
#undef DMA1_Stream7
#undef DMA2_Stream1
#undef DMA2_Stream2
#undef DMA2_Stream5
#undef DMA2_Stream6
#undef DMA2_Stream7
#define DMA1_Stream0 (&dma1_stream0)
#define DMA1_Stream1 (&dma1_stream1)
#define DMA1_Stream2 (&dma1_stream2)
#define DMA1_Stream3 (&dma1_stream3)
#define DMA1_Stream5 (&dma1_stream5)
#define DMA1_Stream7 (&dma1_stream7)
#define DMA2_Stream1 (&dma2_stream1)
#define DMA2_Stream2 (&dma2_stream2)
#define DMA2_Stream5 (&dma2_stream5)
#define DMA2_Stream6 (&dma2_stream6)
#define DMA2_Stream7 (&dma2_stream7)
#undef DMA2
#define DMA2 (&dma2)
#elif defined(PCBSKY9X)
extern Pmc pmc;
#undef PMC
#define PMC (&pmc)
extern Ssc ssc;
#undef SSC
#define SSC (&ssc)
extern Pio Pioa, Piob, Pioc;
extern Twi Twio;
extern Dacc dacc;
extern Usart Usart0;
extern Adc Adc0;
#undef ADC
#define ADC (&Adc0)
#undef USART0
#define USART0 (&Usart0)
#undef USART1
#define USART1 (&Usart0)
#undef USART2
#define USART2 (&Usart0)
#undef USART3
#define USART3 (&Usart0)
#undef PIOA
#define PIOA (&Pioa)
#undef PIOB
#define PIOB (&Piob)
#undef PIOC
#define PIOC (&Pioc)
#undef TWI0
#define TWI0 (&Twio)
#undef DACC
#define DACC (&dacc)
extern Pwm pwm;
#undef PWM
#define PWM (&pwm)
#endif

#if defined(EEPROM_SIZE)
extern uint8_t eeprom[EEPROM_SIZE];
#else
extern uint8_t * eeprom;
#endif

#if defined(CPUARM)
extern void startPdcUsartReceive() ;
extern uint32_t txPdcUsart( uint8_t *buffer, uint32_t size );
extern uint32_t txPdcPending();
extern void rxPdcUsart( void (*pChProcess)(uint8_t x) );
#else
#define PIOA 0
#define PIOB 0
#define PIOC 0
#endif

#define loop_until_bit_is_set( port, bitnum) \
  while ( 0/*! ( (port) & (1 << (bitnum)) )*/ ) ;

#define pgm_read_byte(address_short) (*(uint8_t*)(address_short))
#define pgm_read_word(address_short) (*(uint16_t*)(address_short))
#define pgm_read_adr(address_short) *address_short
#define pgm_read_stringP(adr) ((adr))
#define PSTR(adr) adr
#define _delay_us(a)
#define _delay_ms(a)
#define cli()
#define sei()
#define strcpy_P strcpy
#define memcpy_P memcpy

#define PORTA dummyport
#define PORTB portb
#define PORTC portc
#define PORTD dummyport
#define PORTE dummyport
#define PORTF dummyport
#define PORTG dummyport
#define PORTH porth
#define PORTL dummyport
#define DDRA  dummyport
#define DDRB  dummyport
#define DDRC  dummyport
#define DDRD  dummyport
#define DDRE  dummyport
#define DDRF  dummyport
#define DDRG  dummyport
#define EEMEM

#define UCSR0B dummyport
#define UDRIE0 dummyport
#define TXB80 dummyport
#define TXCIE0 dummyport
#define TXEN0 dummyport
#define RXEN0 dummyport
#define DDE0 dummyport
#define PORTE0 dummyport
#define RXCIE0 dummyport
#define OCR0A dummyport
#define OCR1A dummyport16
#define OCR1B dummyport16
#define OCR1C dummyport16
#define OCR2 dummyport
#define OCR3A dummyport16
#define OCR3B dummyport16
#define OCR4A dummyport
#define OCR5A dummyport
#define TCCR0A dummyport
#define TCCR1A dummyport
#define TCCR1B dummyport
#define TCCR1C dummyport
#define COM1B0 dummyport
#define COM0A0 dummyport
#define TCNT1 dummyport16
#define TCNT1L dummyport
#define TCNT5 dummyport16
#define ICR1 dummyport16
#define TIFR dummyport
#define TIFR1 dummyport
#define ETIFR dummyport

#define SPDR dummyport
#define SPSR dummyport
#define SPIF dummyport
#define SPCR dummyport

#define TIMSK  dummyport
#define TIMSK1 dummyport
#define TIMSK3 dummyport
#define TIMSK4 dummyport
#define TIMSK5 dummyport
#define ETIMSK  dummyport
#define ETIMSK1 dummyport

#define UCSZ02 dummyport
#define UCSR0C dummyport
#define UCSZ01 dummyport
#define UCSZ00 dummyport
#define UCSR0A dummyport
#define RXC0 dummyport

#define UDR0 dummyport
#define OCIE1A dummyport
#define OCIE1B dummyport
#define OCIE1C dummyport
#define OCIE4A dummyport
#define OCIE5A dummyport

#define OUT_B_LIGHT   7
#define INP_E_ElevDR  2
#define INP_E_Trainer 5
#define INP_E_Gear    4
#define INP_C_ThrCt   6
#define INP_C_AileDR  7
#define INP_E_ID2     6

#define WDRF    0
#define WGM10   0
#define WGM12   0
#define COM1B1  0
#define FOC1B   0
#define CS10    0
#define DOR0    0
#define UPE0    0
#define FE0     0

#define ISR(x, ...)  void x()

#if !defined(_MSC_VER) && defined(__GNUC__)
#define asm(...)
#endif

#if defined(CPUARM)
extern uint32_t Master_frequency;
#define NVIC_EnableIRQ(x)
#define NVIC_DisableIRQ(x)
#define NVIC_SetPriority(...)
#define NVIC_SystemReset() exit(0)
#define __disable_irq()
#define __enable_irq()
#endif

extern uint8_t portb, portc, porth, dummyport;
extern uint16_t dummyport16;
extern uint8_t main_thread_running;
extern char * main_thread_error;

#define getADC()
#define GET_ADC_IF_MIXER_NOT_RUNNING()
#define getADC_bandgap()

#define SIMU_SLEEP(x) do { if (!main_thread_running) return; sleep(x/*ms*/); } while (0)
#define SIMU_SLEEP_NORET(x) do { sleep(x/*ms*/); } while (0)

uint64_t simuTimerMicros(void);

void simuInit();
void StartSimu(bool tests=true, const char * sdPath = 0, const char * settingsPath = 0);
void StopSimu();

void simuSetKey(uint8_t key, bool state);
void simuSetTrim(uint8_t trim, bool state);
void simuSetSwitch(uint8_t swtch, int8_t state);

void StartEepromThread(const char *filename="eeprom.bin");
void StopEepromThread();
#if defined(SIMU_AUDIO) && defined(CPUARM)
  void StartAudioThread(int volumeGain = 10);
  void StopAudioThread(void);
#else
  #define StartAudioThread(dummy)
  #define StopAudioThread()
#endif

#if !defined(CPUARM)
#define wdt_disable(...)  sleep(1/*ms*/)
#define wdt_enable(...)   sleep(1/*ms*/)
#define wdt_reset()       sleep(1/*ms*/)
#endif

void * simuMain(void * args = NULL);
extern uint8_t MCUCSR, MCUSR, MCUCR;

// simulate CoOS

#if defined(STM32F4)
#define CFG_CPU_FREQ            (168000000)
#elif defined(STM32)
#define CFG_CPU_FREQ            (120000000)
#else
#define CFG_CPU_FREQ            (36000000)
#endif
#define CFG_SYSTICK_FREQ        (500)

#define OS_MutexID pthread_mutex_t
extern OS_MutexID audioMutex;
#define OS_FlagID uint32_t
#define OS_TID pthread_t
#define OS_TCID uint32_t
#define OS_STK uint32_t
#define OS_EventID sem_t *

typedef unsigned char      U8;
typedef unsigned int       U32;
typedef unsigned long long U64;
typedef void               (*FUNCPtr)(void*);
typedef U8                 StatusType;

#define E_OK           (0)
#define E_CREATE_FAIL  (StatusType)-1

#define CoInitOS(...)
#define CoStartOS(...)

OS_TID CoCreateTask(FUNCPtr task, void *argv, uint32_t parameter, void * stk, uint32_t stksize);
U64 CoGetOSTime(void);
//#define CoCreateTaskEx(...)            0

#define CoCreateMutex(...)             PTHREAD_MUTEX_INITIALIZER
#define CoEnterMutexSection(m)         pthread_mutex_lock(&(m))
#define CoLeaveMutexSection(m)         pthread_mutex_unlock(&(m))

inline OS_EventID CoCreateSem(uint16_t initCnt, uint16_t maxCnt, uint8_t /*sortType*/)
{
  OS_EventID semaphore = (OS_EventID)malloc(sizeof(sem_t));
  sem_init(semaphore, maxCnt, initCnt);
  return semaphore;
}
inline int CoDelSem(OS_EventID id, uint8_t /*opt*/)
{
  int ret = sem_destroy(id);
  free(id);
  return ret;
}
#define CoPostSem(__sem)               sem_post((__sem))
#define isr_PostSem(__sem)             sem_post((__sem))
#define CoPendSem(__sem, __to)         sem_wait((__sem))
#define CoAcceptSem(__sem)             sem_trywait((__sem))

// TODO: real flags (use semaphores?)
#define CoCreateFlag(...)              E_OK
#define CoSetFlag(...)                 E_OK
#define CoClearFlag(...)               E_OK
#define CoWaitForSingleFlag(...)       E_OK
#define CoSetTmrCnt(...)
#define CoEnterISR(...)
#define CoExitISR(...)
#define CoStartTmr(...)
#define CoTickDelay(x)                 sleep(2*(x))

#define UART_Stop(...)
#define UART3_Stop(...)
#define USART_GetITStatus(...)         0
#define USART_ClearFlag(...)

#if defined(STM32)
inline void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct) { }
#define TIM_DeInit(...)
#define TIM_SetCompare2(...)
#define TIM_ClearFlag(...)
#define TIM_Cmd(...)
#define TIM_ITConfig(...)
#define GPIO_SetBits(GPIOx, pin) GPIOx->BSRRL |= pin
#define GPIO_ResetBits(GPIOx, pin) GPIOx->BSRRL &= ~pin
#define GPIO_ReadInputDataBit(GPIOx, pin) (GPIOx->BSRRL & pin)
#define RCC_AHB1PeriphClockCmd(...)
#define RCC_APB2PeriphClockCmd(...)
inline void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct) { }
inline void SPI_CalculateCRC(SPI_TypeDef* SPIx, FunctionalState NewState) { }
inline void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState) { }
inline FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG) { return RESET; }
inline uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx) { return 0; }
inline void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data) { }
inline void DMA_DeInit(DMA_Stream_TypeDef* DMAy_Streamx) { }
inline void DMA_Init(DMA_Stream_TypeDef* DMAy_Streamx, DMA_InitTypeDef* DMA_InitStruct) { }
inline void DMA_ITConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT, FunctionalState NewState) { }
inline void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct) { }
inline void DMA_Cmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState) { }
inline FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG) { return RESET; }
inline ITStatus DMA_GetITStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT) { return RESET; }
inline void DMA_ClearITPendingBit(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT) { }
inline void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionalState NewState) { }
inline void UART3_Configure(uint32_t baudrate, uint32_t masterClock) { }
inline void NVIC_Init(NVIC_InitTypeDef *) { }
#endif

inline void delay_01us(int dummy) { }
#define configure_pins(...)

#if defined(SDCARD) && !defined(SKIP_FATFS_DECLARATION) && !defined(SIMU_DISKIO)
  #define SIMU_USE_SDCARD
#endif

#define sdMountPoll()
#define sdPoll10ms()
#define sd_card_ready()  (true)
#if !defined(SIMU_DISKIO)
  #define sdMounted()      (true)
#endif

#if defined(SIMU_USE_SDCARD)
  void simuFatfsSetPaths(const char * sdPath, const char * settingsPath);
#else
  #define simuFatfsSetPaths(...)
#endif

#if defined(TRACE_SIMPGMSPACE)
  #undef TRACE_SIMPGMSPACE
  #define TRACE_SIMPGMSPACE   TRACE
#else
  #define TRACE_SIMPGMSPACE(...)
#endif

#endif // _SIMPGMSPACE_H_
