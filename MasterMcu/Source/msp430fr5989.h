/********************************************************************
*
* Standard register and bit definitions for the Texas Instruments
* MSP430 microcontroller.
*
* This file supports assembler and C development for
* MSP430FR5989 devices.
*
* Texas Instruments, Version 1.1
*
* Rev. 1.0, Setup
* Rev. 1.1, ESI: Renamed bit ESIVCC2 to ESIVMIDEN, renamed bit ESIVSS to ESISHTSM
*
*
********************************************************************/

#ifndef __MSP430FR5989
#define __MSP430FR5989

#define __MSP430_HEADER_VERSION__ 1157

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif

#if (((__TID__ >> 8) & 0x7F) != 0x2b)     /* 0x2b = 43 dec */
#error msp430fr5989.h file for use with ICC430/A430 only
#endif


#ifdef __IAR_SYSTEMS_ICC__
#include "in430.h"
#pragma language=save
#pragma language=extended

#define DEFC(name, address) __no_init volatile unsigned char name @ address;
#define DEFW(name, address) __no_init volatile unsigned short name @ address;

#define DEFCW(name, address) __no_init union \
{ \
  struct \
  { \
    volatile unsigned char  name##_L; \
    volatile unsigned char  name##_H; \
  }; \
  volatile unsigned short   name; \
} @ address;

#define READ_ONLY_DEFCW(name, address) __no_init union \
{ \
  struct \
  { \
    volatile READ_ONLY unsigned char  name##_L; \
    volatile READ_ONLY unsigned char  name##_H; \
  }; \
  volatile READ_ONLY unsigned short   name; \
} @ address;


#if __REGISTER_MODEL__ == __REGISTER_MODEL_REG20__
#define __ACCESS_20BIT_REG__  void __data20 * volatile
#else
#define __ACCESS_20BIT_REG__  volatile unsigned short  /* only short access from C is allowed in small memory model */
#endif

#define DEFA(name, address) __no_init union \
{ \
  struct \
  { \
    volatile unsigned char  name##_L; \
    volatile unsigned char  name##_H; \
  }; \
  struct \
  { \
    volatile unsigned short name##L; \
    volatile unsigned short name##H; \
  }; \
  __ACCESS_20BIT_REG__ name; \
} @ address;

#endif  /* __IAR_SYSTEMS_ICC__  */


#ifdef __IAR_SYSTEMS_ASM__
#define DEFC(name, address) sfrb name = address;
#define DEFW(name, address) sfrw name = address;

#define DEFCW(name, address) sfrbw name, name##_L, name##_H, address;
sfrbw macro name, name_L, name_H, address;
sfrb name_L = address;
sfrb name_H = address+1;
sfrw name   = address;
      endm

#define READ_ONLY_DEFCW(name, address) const_sfrbw name, name##_L, name##_H, address;
const_sfrbw macro name, name_L, name_H, address;
const sfrb name_L = address;
const sfrb name_H = address+1;
const sfrw name   = address;
      endm

#define DEFA(name, address) sfrba name, name##L, name##H, name##_L, name##_H, address;
sfrba macro name, nameL, nameH, name_L, name_H, address;
sfrb name_L = address;
sfrb name_H = address+1;
sfrw nameL  = address;
sfrw nameH  = address+2;
sfrl name   = address;
      endm

#endif /* __IAR_SYSTEMS_ASM__*/

#ifdef __cplusplus
#define READ_ONLY
#else
#define READ_ONLY const
#endif

/************************************************************
* STANDARD BITS
************************************************************/

#define BIT0                (0x0001u)
#define BIT1                (0x0002u)
#define BIT2                (0x0004u)
#define BIT3                (0x0008u)
#define BIT4                (0x0010u)
#define BIT5                (0x0020u)
#define BIT6                (0x0040u)
#define BIT7                (0x0080u)
#define BIT8                (0x0100u)
#define BIT9                (0x0200u)
#define BITA                (0x0400u)
#define BITB                (0x0800u)
#define BITC                (0x1000u)
#define BITD                (0x2000u)
#define BITE                (0x4000u)
#define BITF                (0x8000u)

/************************************************************
* STATUS REGISTER BITS
************************************************************/

#define C                   (0x0001u)
#define Z                   (0x0002u)
#define N                   (0x0004u)
#define V                   (0x0100u)
#define GIE                 (0x0008u)
#define CPUOFF              (0x0010u)
#define OSCOFF              (0x0020u)
#define SCG0                (0x0040u)
#define SCG1                (0x0080u)

/* Low Power Modes coded with Bits 4-7 in SR */

#ifndef __IAR_SYSTEMS_ICC__ /* Begin #defines for assembler */
#define LPM0                (CPUOFF)
#define LPM1                (SCG0+CPUOFF)
#define LPM2                (SCG1+CPUOFF)
#define LPM3                (SCG1+SCG0+CPUOFF)
#define LPM4                (SCG1+SCG0+OSCOFF+CPUOFF)
/* End #defines for assembler */

#else /* Begin #defines for C */
#define LPM0_bits           (CPUOFF)
#define LPM1_bits           (SCG0+CPUOFF)
#define LPM2_bits           (SCG1+CPUOFF)
#define LPM3_bits           (SCG1+SCG0+CPUOFF)
#define LPM4_bits           (SCG1+SCG0+OSCOFF+CPUOFF)

#include "in430.h"

#define LPM0      __bis_SR_register(LPM0_bits)         /* Enter Low Power Mode 0 */
#define LPM0_EXIT __bic_SR_register_on_exit(LPM0_bits) /* Exit Low Power Mode 0 */
#define LPM1      __bis_SR_register(LPM1_bits)         /* Enter Low Power Mode 1 */
#define LPM1_EXIT __bic_SR_register_on_exit(LPM1_bits) /* Exit Low Power Mode 1 */
#define LPM2      __bis_SR_register(LPM2_bits)         /* Enter Low Power Mode 2 */
#define LPM2_EXIT __bic_SR_register_on_exit(LPM2_bits) /* Exit Low Power Mode 2 */
#define LPM3      __bis_SR_register(LPM3_bits)         /* Enter Low Power Mode 3 */
#define LPM3_EXIT __bic_SR_register_on_exit(LPM3_bits) /* Exit Low Power Mode 3 */
#define LPM4      __bis_SR_register(LPM4_bits)         /* Enter Low Power Mode 4 */
#define LPM4_EXIT __bic_SR_register_on_exit(LPM4_bits) /* Exit Low Power Mode 4 */
#endif /* End #defines for C */

/************************************************************
* CPU
************************************************************/
#define __MSP430_HAS_MSP430XV2_CPU__  /* Definition to show that it has MSP430XV2 CPU */
#define __MSP430FR5XX_6XX_FAMILY__

/************************************************************
* PERIPHERAL FILE MAP
************************************************************/

/************************************************************
* ADC12_B
************************************************************/
#define __MSP430_HAS_ADC12_B__         /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_ADC12_B__ 0x0800
#define ADC12_B_BASE __MSP430_BASEADDRESS_ADC12_B__

#define  ADC12CTL0_            (0x0800u)  /* ADC12 B Control 0 */
DEFCW(   ADC12CTL0           , ADC12CTL0_)
#define  ADC12CTL1_            (0x0802u)  /* ADC12 B Control 1 */
DEFCW(   ADC12CTL1           , ADC12CTL1_)
#define  ADC12CTL2_            (0x0804u)  /* ADC12 B Control 2 */
DEFCW(   ADC12CTL2           , ADC12CTL2_)
#define  ADC12CTL3_            (0x0806u)  /* ADC12 B Control 3 */
DEFCW(   ADC12CTL3           , ADC12CTL3_)
#define  ADC12LO_              (0x0808u)  /* ADC12 B Window Comparator High Threshold */
DEFCW(   ADC12LO             , ADC12LO_)
#define  ADC12HI_              (0x080Au)  /* ADC12 B Window Comparator High Threshold */
DEFCW(   ADC12HI             , ADC12HI_)
#define  ADC12IFGR0_           (0x080Cu)  /* ADC12 B Interrupt Flag 0 */
DEFCW(   ADC12IFGR0          , ADC12IFGR0_)
#define  ADC12IFGR1_           (0x080Eu)  /* ADC12 B Interrupt Flag 1 */
DEFCW(   ADC12IFGR1          , ADC12IFGR1_)
#define  ADC12IFGR2_           (0x0810u)  /* ADC12 B Interrupt Flag 2 */
DEFCW(   ADC12IFGR2          , ADC12IFGR2_)
#define  ADC12IER0_            (0x0812u)  /* ADC12 B Interrupt Enable 0 */
DEFCW(   ADC12IER0           , ADC12IER0_)
#define  ADC12IER1_            (0x0814u)  /* ADC12 B Interrupt Enable 1 */
DEFCW(   ADC12IER1           , ADC12IER1_)
#define  ADC12IER2_            (0x0816u)  /* ADC12 B Interrupt Enable 2 */
DEFCW(   ADC12IER2           , ADC12IER2_)
#define  ADC12IV_              (0x0818u)  /* ADC12 B Interrupt Vector Word */
DEFCW(   ADC12IV             , ADC12IV_)

#define ADC12MCTL0_            (0x0820u)  /* ADC12 Memory Control 0 */
DEFCW(  ADC12MCTL0           , ADC12MCTL0_)
#define ADC12MCTL1_            (0x0822u)  /* ADC12 Memory Control 1 */
DEFCW(  ADC12MCTL1           , ADC12MCTL1_)
#define ADC12MCTL2_            (0x0824u)  /* ADC12 Memory Control 2 */
DEFCW(  ADC12MCTL2           , ADC12MCTL2_)
#define ADC12MCTL3_            (0x0826u)  /* ADC12 Memory Control 3 */
DEFCW(  ADC12MCTL3           , ADC12MCTL3_)
#define ADC12MCTL4_            (0x0828u)  /* ADC12 Memory Control 4 */
DEFCW(  ADC12MCTL4           , ADC12MCTL4_)
#define ADC12MCTL5_            (0x082Au)  /* ADC12 Memory Control 5 */
DEFCW(  ADC12MCTL5           , ADC12MCTL5_)
#define ADC12MCTL6_            (0x082Cu)  /* ADC12 Memory Control 6 */
DEFCW(  ADC12MCTL6           , ADC12MCTL6_)
#define ADC12MCTL7_            (0x082Eu)  /* ADC12 Memory Control 7 */
DEFCW(  ADC12MCTL7           , ADC12MCTL7_)
#define ADC12MCTL8_            (0x0830u)  /* ADC12 Memory Control 8 */
DEFCW(  ADC12MCTL8           , ADC12MCTL8_)
#define ADC12MCTL9_            (0x0832u)  /* ADC12 Memory Control 9 */
DEFCW(  ADC12MCTL9           , ADC12MCTL9_)
#define ADC12MCTL10_           (0x0834u)  /* ADC12 Memory Control 10 */
DEFCW(  ADC12MCTL10          , ADC12MCTL10_)
#define ADC12MCTL11_           (0x0836u)  /* ADC12 Memory Control 11 */
DEFCW(  ADC12MCTL11          , ADC12MCTL11_)
#define ADC12MCTL12_           (0x0838u)  /* ADC12 Memory Control 12 */
DEFCW(  ADC12MCTL12          , ADC12MCTL12_)
#define ADC12MCTL13_           (0x083Au)  /* ADC12 Memory Control 13 */
DEFCW(  ADC12MCTL13          , ADC12MCTL13_)
#define ADC12MCTL14_           (0x083Cu)  /* ADC12 Memory Control 14 */
DEFCW(  ADC12MCTL14          , ADC12MCTL14_)
#define ADC12MCTL15_           (0x083Eu)  /* ADC12 Memory Control 15 */
DEFCW(  ADC12MCTL15          , ADC12MCTL15_)
#define ADC12MCTL16_           (0x0840u)  /* ADC12 Memory Control 16 */
DEFCW(  ADC12MCTL16          , ADC12MCTL16_)
#define ADC12MCTL17_           (0x0842u)  /* ADC12 Memory Control 17 */
DEFCW(  ADC12MCTL17          , ADC12MCTL17_)
#define ADC12MCTL18_           (0x0844u)  /* ADC12 Memory Control 18 */
DEFCW(  ADC12MCTL18          , ADC12MCTL18_)
#define ADC12MCTL19_           (0x0846u)  /* ADC12 Memory Control 19 */
DEFCW(  ADC12MCTL19          , ADC12MCTL19_)
#define ADC12MCTL20_           (0x0848u)  /* ADC12 Memory Control 20 */
DEFCW(  ADC12MCTL20          , ADC12MCTL20_)
#define ADC12MCTL21_           (0x084Au)  /* ADC12 Memory Control 21 */
DEFCW(  ADC12MCTL21          , ADC12MCTL21_)
#define ADC12MCTL22_           (0x084Cu)  /* ADC12 Memory Control 22 */
DEFCW(  ADC12MCTL22          , ADC12MCTL22_)
#define ADC12MCTL23_           (0x084Eu)  /* ADC12 Memory Control 23 */
DEFCW(  ADC12MCTL23          , ADC12MCTL23_)
#define ADC12MCTL24_           (0x0850u)  /* ADC12 Memory Control 24 */
DEFCW(  ADC12MCTL24          , ADC12MCTL24_)
#define ADC12MCTL25_           (0x0852u)  /* ADC12 Memory Control 25 */
DEFCW(  ADC12MCTL25          , ADC12MCTL25_)
#define ADC12MCTL26_           (0x0854u)  /* ADC12 Memory Control 26 */
DEFCW(  ADC12MCTL26          , ADC12MCTL26_)
#define ADC12MCTL27_           (0x0856u)  /* ADC12 Memory Control 27 */
DEFCW(  ADC12MCTL27          , ADC12MCTL27_)
#define ADC12MCTL28_           (0x0858u)  /* ADC12 Memory Control 28 */
DEFCW(  ADC12MCTL28          , ADC12MCTL28_)
#define ADC12MCTL29_           (0x085Au)  /* ADC12 Memory Control 29 */
DEFCW(  ADC12MCTL29          , ADC12MCTL29_)
#define ADC12MCTL30_           (0x085Cu)  /* ADC12 Memory Control 30 */
DEFCW(  ADC12MCTL30          , ADC12MCTL30_)
#define ADC12MCTL31_           (0x085Eu)  /* ADC12 Memory Control 31 */
DEFCW(  ADC12MCTL31          , ADC12MCTL31_)
#define ADC12MCTL_          ADC12MCTL /* ADC12 Memory Control */
#ifndef __IAR_SYSTEMS_ICC__
#define ADC12MCTL           ADC12MCTL0 /* ADC12 Memory Control (for assembler) */
#else
#define ADC12MCTL           ((char*) &ADC12MCTL0) /* ADC12 Memory Control (for C) */
#endif

#define  ADC12MEM0_             (0x0860u)  /* ADC12 Conversion Memory 0 */
DEFCW(   ADC12MEM0            , ADC12MEM0_)
#define  ADC12MEM1_             (0x0862u)  /* ADC12 Conversion Memory 1 */
DEFCW(   ADC12MEM1            , ADC12MEM1_)
#define  ADC12MEM2_             (0x0864u)  /* ADC12 Conversion Memory 2 */
DEFCW(   ADC12MEM2            , ADC12MEM2_)
#define  ADC12MEM3_             (0x0866u)  /* ADC12 Conversion Memory 3 */
DEFCW(   ADC12MEM3            , ADC12MEM3_)
#define  ADC12MEM4_             (0x0868u)  /* ADC12 Conversion Memory 4 */
DEFCW(   ADC12MEM4            , ADC12MEM4_)
#define  ADC12MEM5_             (0x086Au)  /* ADC12 Conversion Memory 5 */
DEFCW(   ADC12MEM5            , ADC12MEM5_)
#define  ADC12MEM6_             (0x086Cu)  /* ADC12 Conversion Memory 6 */
DEFCW(   ADC12MEM6            , ADC12MEM6_)
#define  ADC12MEM7_             (0x086Eu)  /* ADC12 Conversion Memory 7 */
DEFCW(   ADC12MEM7            , ADC12MEM7_)
#define  ADC12MEM8_             (0x0870u)  /* ADC12 Conversion Memory 8 */
DEFCW(   ADC12MEM8            , ADC12MEM8_)
#define  ADC12MEM9_             (0x0872u)  /* ADC12 Conversion Memory 9 */
DEFCW(   ADC12MEM9            , ADC12MEM9_)
#define  ADC12MEM10_            (0x0874u)  /* ADC12 Conversion Memory 10 */
DEFCW(   ADC12MEM10           , ADC12MEM10_)
#define  ADC12MEM11_            (0x0876u)  /* ADC12 Conversion Memory 11 */
DEFCW(   ADC12MEM11           , ADC12MEM11_)
#define  ADC12MEM12_            (0x0878u)  /* ADC12 Conversion Memory 12 */
DEFCW(   ADC12MEM12           , ADC12MEM12_)
#define  ADC12MEM13_            (0x087Au)  /* ADC12 Conversion Memory 13 */
DEFCW(   ADC12MEM13           , ADC12MEM13_)
#define  ADC12MEM14_            (0x087Cu)  /* ADC12 Conversion Memory 14 */
DEFCW(   ADC12MEM14           , ADC12MEM14_)
#define  ADC12MEM15_            (0x087Eu)  /* ADC12 Conversion Memory 15 */
DEFCW(   ADC12MEM15           , ADC12MEM15_)
#define  ADC12MEM16_            (0x0880u)  /* ADC12 Conversion Memory 16 */
DEFCW(   ADC12MEM16           , ADC12MEM16_)
#define  ADC12MEM17_            (0x0882u)  /* ADC12 Conversion Memory 17 */
DEFCW(   ADC12MEM17           , ADC12MEM17_)
#define  ADC12MEM18_            (0x0884u)  /* ADC12 Conversion Memory 18 */
DEFCW(   ADC12MEM18           , ADC12MEM18_)
#define  ADC12MEM19_            (0x0886u)  /* ADC12 Conversion Memory 19 */
DEFCW(   ADC12MEM19           , ADC12MEM19_)
#define  ADC12MEM20_            (0x0888u)  /* ADC12 Conversion Memory 20 */
DEFCW(   ADC12MEM20           , ADC12MEM20_)
#define  ADC12MEM21_            (0x088Au)  /* ADC12 Conversion Memory 21 */
DEFCW(   ADC12MEM21           , ADC12MEM21_)
#define  ADC12MEM22_            (0x088Cu)  /* ADC12 Conversion Memory 22 */
DEFCW(   ADC12MEM22           , ADC12MEM22_)
#define  ADC12MEM23_            (0x088Eu)  /* ADC12 Conversion Memory 23 */
DEFCW(   ADC12MEM23           , ADC12MEM23_)
#define  ADC12MEM24_            (0x0890u)  /* ADC12 Conversion Memory 24 */
DEFCW(   ADC12MEM24           , ADC12MEM24_)
#define  ADC12MEM25_            (0x0892u)  /* ADC12 Conversion Memory 25 */
DEFCW(   ADC12MEM25           , ADC12MEM25_)
#define  ADC12MEM26_            (0x0894u)  /* ADC12 Conversion Memory 26 */
DEFCW(   ADC12MEM26           , ADC12MEM26_)
#define  ADC12MEM27_            (0x0896u)  /* ADC12 Conversion Memory 27 */
DEFCW(   ADC12MEM27           , ADC12MEM27_)
#define  ADC12MEM28_            (0x0898u)  /* ADC12 Conversion Memory 28 */
DEFCW(   ADC12MEM28           , ADC12MEM28_)
#define  ADC12MEM29_            (0x089Au)  /* ADC12 Conversion Memory 29 */
DEFCW(   ADC12MEM29           , ADC12MEM29_)
#define  ADC12MEM30_            (0x089Cu)  /* ADC12 Conversion Memory 30 */
DEFCW(   ADC12MEM30           , ADC12MEM30_)
#define  ADC12MEM31_            (0x089Eu)  /* ADC12 Conversion Memory 31 */
DEFCW(   ADC12MEM31           , ADC12MEM31_)
#define ADC12MEM_           ADC12MEM  /* ADC12 Conversion Memory */
#ifndef __IAR_SYSTEMS_ICC__
#define ADC12MEM            ADC12MEM0 /* ADC12 Conversion Memory (for assembler) */
#else
#define ADC12MEM            ((int*) &ADC12MEM0) /* ADC12 Conversion Memory (for C) */
#endif

/* ADC12CTL0 Control Bits */
#define ADC12SC             (0x0001u)  /* ADC12 Start Conversion */
#define ADC12ENC            (0x0002u)  /* ADC12 Enable Conversion */
#define ADC12ON             (0x0010u)  /* ADC12 On/enable */
#define ADC12MSC            (0x0080u)  /* ADC12 Multiple SampleConversion */
#define ADC12SHT00          (0x0100u)  /* ADC12 Sample Hold 0 Select Bit: 0 */
#define ADC12SHT01          (0x0200u)  /* ADC12 Sample Hold 0 Select Bit: 1 */
#define ADC12SHT02          (0x0400u)  /* ADC12 Sample Hold 0 Select Bit: 2 */
#define ADC12SHT03          (0x0800u)  /* ADC12 Sample Hold 0 Select Bit: 3 */
#define ADC12SHT10          (0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 0 */
#define ADC12SHT11          (0x2000u)  /* ADC12 Sample Hold 1 Select Bit: 1 */
#define ADC12SHT12          (0x4000u)  /* ADC12 Sample Hold 1 Select Bit: 2 */
#define ADC12SHT13          (0x8000u)  /* ADC12 Sample Hold 1 Select Bit: 3 */

/* ADC12CTL0 Control Bits */
#define ADC12SC_L           (0x0001u)  /* ADC12 Start Conversion */
#define ADC12ENC_L          (0x0002u)  /* ADC12 Enable Conversion */
#define ADC12ON_L           (0x0010u)  /* ADC12 On/enable */
#define ADC12MSC_L          (0x0080u)  /* ADC12 Multiple SampleConversion */

/* ADC12CTL0 Control Bits */
#define ADC12SHT00_H        (0x0001u)  /* ADC12 Sample Hold 0 Select Bit: 0 */
#define ADC12SHT01_H        (0x0002u)  /* ADC12 Sample Hold 0 Select Bit: 1 */
#define ADC12SHT02_H        (0x0004u)  /* ADC12 Sample Hold 0 Select Bit: 2 */
#define ADC12SHT03_H        (0x0008u)  /* ADC12 Sample Hold 0 Select Bit: 3 */
#define ADC12SHT10_H        (0x0010u)  /* ADC12 Sample Hold 1 Select Bit: 0 */
#define ADC12SHT11_H        (0x0020u)  /* ADC12 Sample Hold 1 Select Bit: 1 */
#define ADC12SHT12_H        (0x0040u)  /* ADC12 Sample Hold 1 Select Bit: 2 */
#define ADC12SHT13_H        (0x0080u)  /* ADC12 Sample Hold 1 Select Bit: 3 */

#define ADC12SHT0_0         (0*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 0 */
#define ADC12SHT0_1         (1*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 1 */
#define ADC12SHT0_2         (2*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 2 */
#define ADC12SHT0_3         (3*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 3 */
#define ADC12SHT0_4         (4*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 4 */
#define ADC12SHT0_5         (5*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 5 */
#define ADC12SHT0_6         (6*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 6 */
#define ADC12SHT0_7         (7*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 7 */
#define ADC12SHT0_8         (8*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 8 */
#define ADC12SHT0_9         (9*0x100u)  /* ADC12 Sample Hold 0 Select Bit: 9 */
#define ADC12SHT0_10        (10*0x100u) /* ADC12 Sample Hold 0 Select Bit: 10 */
#define ADC12SHT0_11        (11*0x100u) /* ADC12 Sample Hold 0 Select Bit: 11 */
#define ADC12SHT0_12        (12*0x100u) /* ADC12 Sample Hold 0 Select Bit: 12 */
#define ADC12SHT0_13        (13*0x100u) /* ADC12 Sample Hold 0 Select Bit: 13 */
#define ADC12SHT0_14        (14*0x100u) /* ADC12 Sample Hold 0 Select Bit: 14 */
#define ADC12SHT0_15        (15*0x100u) /* ADC12 Sample Hold 0 Select Bit: 15 */

#define ADC12SHT1_0         (0*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 0 */
#define ADC12SHT1_1         (1*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 1 */
#define ADC12SHT1_2         (2*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 2 */
#define ADC12SHT1_3         (3*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 3 */
#define ADC12SHT1_4         (4*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 4 */
#define ADC12SHT1_5         (5*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 5 */
#define ADC12SHT1_6         (6*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 6 */
#define ADC12SHT1_7         (7*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 7 */
#define ADC12SHT1_8         (8*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 8 */
#define ADC12SHT1_9         (9*0x1000u)  /* ADC12 Sample Hold 1 Select Bit: 9 */
#define ADC12SHT1_10        (10*0x1000u) /* ADC12 Sample Hold 1 Select Bit: 10 */
#define ADC12SHT1_11        (11*0x1000u) /* ADC12 Sample Hold 1 Select Bit: 11 */
#define ADC12SHT1_12        (12*0x1000u) /* ADC12 Sample Hold 1 Select Bit: 12 */
#define ADC12SHT1_13        (13*0x1000u) /* ADC12 Sample Hold 1 Select Bit: 13 */
#define ADC12SHT1_14        (14*0x1000u) /* ADC12 Sample Hold 1 Select Bit: 14 */
#define ADC12SHT1_15        (15*0x1000u) /* ADC12 Sample Hold 1 Select Bit: 15 */

/* ADC12CTL1 Control Bits */
#define ADC12BUSY           (0x0001u)    /* ADC12 Busy */
#define ADC12CONSEQ0        (0x0002u)    /* ADC12 Conversion Sequence Select Bit: 0 */
#define ADC12CONSEQ1        (0x0004u)    /* ADC12 Conversion Sequence Select Bit: 1 */
#define ADC12SSEL0          (0x0008u)    /* ADC12 Clock Source Select Bit: 0 */
#define ADC12SSEL1          (0x0010u)    /* ADC12 Clock Source Select Bit: 1 */
#define ADC12DIV0           (0x0020u)    /* ADC12 Clock Divider Select Bit: 0 */
#define ADC12DIV1           (0x0040u)    /* ADC12 Clock Divider Select Bit: 1 */
#define ADC12DIV2           (0x0080u)    /* ADC12 Clock Divider Select Bit: 2 */
#define ADC12ISSH           (0x0100u)    /* ADC12 Invert Sample Hold Signal */
#define ADC12SHP            (0x0200u)    /* ADC12 Sample/Hold Pulse Mode */
#define ADC12SHS0           (0x0400u)    /* ADC12 Sample/Hold Source Bit: 0 */
#define ADC12SHS1           (0x0800u)    /* ADC12 Sample/Hold Source Bit: 1 */
#define ADC12SHS2           (0x1000u)    /* ADC12 Sample/Hold Source Bit: 2 */
#define ADC12PDIV0          (0x2000u)    /* ADC12 Predivider Bit: 0 */
#define ADC12PDIV1          (0x4000u)    /* ADC12 Predivider Bit: 1 */

/* ADC12CTL1 Control Bits */
#define ADC12BUSY_L         (0x0001u)    /* ADC12 Busy */
#define ADC12CONSEQ0_L      (0x0002u)    /* ADC12 Conversion Sequence Select Bit: 0 */
#define ADC12CONSEQ1_L      (0x0004u)    /* ADC12 Conversion Sequence Select Bit: 1 */
#define ADC12SSEL0_L        (0x0008u)    /* ADC12 Clock Source Select Bit: 0 */
#define ADC12SSEL1_L        (0x0010u)    /* ADC12 Clock Source Select Bit: 1 */
#define ADC12DIV0_L         (0x0020u)    /* ADC12 Clock Divider Select Bit: 0 */
#define ADC12DIV1_L         (0x0040u)    /* ADC12 Clock Divider Select Bit: 1 */
#define ADC12DIV2_L         (0x0080u)    /* ADC12 Clock Divider Select Bit: 2 */

/* ADC12CTL1 Control Bits */
#define ADC12ISSH_H         (0x0001u)    /* ADC12 Invert Sample Hold Signal */
#define ADC12SHP_H          (0x0002u)    /* ADC12 Sample/Hold Pulse Mode */
#define ADC12SHS0_H         (0x0004u)    /* ADC12 Sample/Hold Source Bit: 0 */
#define ADC12SHS1_H         (0x0008u)    /* ADC12 Sample/Hold Source Bit: 1 */
#define ADC12SHS2_H         (0x0010u)    /* ADC12 Sample/Hold Source Bit: 2 */
#define ADC12PDIV0_H        (0x0020u)    /* ADC12 Predivider Bit: 0 */
#define ADC12PDIV1_H        (0x0040u)    /* ADC12 Predivider Bit: 1 */

#define ADC12CONSEQ_0        (0*0x0002u) /* ADC12 Conversion Sequence Select: 0 */
#define ADC12CONSEQ_1        (1*0x0002u) /* ADC12 Conversion Sequence Select: 1 */
#define ADC12CONSEQ_2        (2*0x0002u) /* ADC12 Conversion Sequence Select: 2 */
#define ADC12CONSEQ_3        (3*0x0002u) /* ADC12 Conversion Sequence Select: 3 */

#define ADC12SSEL_0          (0*0x0008u) /* ADC12 Clock Source Select: 0 */
#define ADC12SSEL_1          (1*0x0008u) /* ADC12 Clock Source Select: 1 */
#define ADC12SSEL_2          (2*0x0008u) /* ADC12 Clock Source Select: 2 */
#define ADC12SSEL_3          (3*0x0008u) /* ADC12 Clock Source Select: 3 */

#define ADC12DIV_0           (0*0x0020u) /* ADC12 Clock Divider Select: 0 */
#define ADC12DIV_1           (1*0x0020u) /* ADC12 Clock Divider Select: 1 */
#define ADC12DIV_2           (2*0x0020u) /* ADC12 Clock Divider Select: 2 */
#define ADC12DIV_3           (3*0x0020u) /* ADC12 Clock Divider Select: 3 */
#define ADC12DIV_4           (4*0x0020u) /* ADC12 Clock Divider Select: 4 */
#define ADC12DIV_5           (5*0x0020u) /* ADC12 Clock Divider Select: 5 */
#define ADC12DIV_6           (6*0x0020u) /* ADC12 Clock Divider Select: 6 */
#define ADC12DIV_7           (7*0x0020u) /* ADC12 Clock Divider Select: 7 */

#define ADC12SHS_0           (0*0x0400u) /* ADC12 Sample/Hold Source: 0 */
#define ADC12SHS_1           (1*0x0400u) /* ADC12 Sample/Hold Source: 1 */
#define ADC12SHS_2           (2*0x0400u) /* ADC12 Sample/Hold Source: 2 */
#define ADC12SHS_3           (3*0x0400u) /* ADC12 Sample/Hold Source: 3 */
#define ADC12SHS_4           (4*0x0400u) /* ADC12 Sample/Hold Source: 4 */
#define ADC12SHS_5           (5*0x0400u) /* ADC12 Sample/Hold Source: 5 */
#define ADC12SHS_6           (6*0x0400u) /* ADC12 Sample/Hold Source: 6 */
#define ADC12SHS_7           (7*0x0400u) /* ADC12 Sample/Hold Source: 7 */

#define ADC12PDIV_0          (0*0x2000u) /* ADC12 Clock predivider Select 0 */
#define ADC12PDIV_1          (1*0x2000u) /* ADC12 Clock predivider Select 1 */
#define ADC12PDIV_2          (2*0x2000u) /* ADC12 Clock predivider Select 2 */
#define ADC12PDIV_3          (3*0x2000u) /* ADC12 Clock predivider Select 3 */
#define ADC12PDIV__1         (0*0x2000u) /* ADC12 Clock predivider Select: /1 */
#define ADC12PDIV__4         (1*0x2000u) /* ADC12 Clock predivider Select: /4 */
#define ADC12PDIV__32        (2*0x2000u) /* ADC12 Clock predivider Select: /32 */
#define ADC12PDIV__64        (3*0x2000u) /* ADC12 Clock predivider Select: /64 */

/* ADC12CTL2 Control Bits */
#define ADC12PWRMD          (0x0001u)    /* ADC12 Power Mode */
#define ADC12DF             (0x0008u)    /* ADC12 Data Format */
#define ADC12RES0           (0x0010u)    /* ADC12 Resolution Bit: 0 */
#define ADC12RES1           (0x0020u)    /* ADC12 Resolution Bit: 1 */

/* ADC12CTL2 Control Bits */
#define ADC12PWRMD_L        (0x0001u)    /* ADC12 Power Mode */
#define ADC12DF_L           (0x0008u)    /* ADC12 Data Format */
#define ADC12RES0_L         (0x0010u)    /* ADC12 Resolution Bit: 0 */
#define ADC12RES1_L         (0x0020u)    /* ADC12 Resolution Bit: 1 */

#define ADC12RES_0          (0x0000u)    /* ADC12+ Resolution : 8 Bit */
#define ADC12RES_1          (0x0010u)    /* ADC12+ Resolution : 10 Bit */
#define ADC12RES_2          (0x0020u)    /* ADC12+ Resolution : 12 Bit */
#define ADC12RES_3          (0x0030u)    /* ADC12+ Resolution : reserved */

#define ADC12RES__8BIT      (0x0000u)    /* ADC12+ Resolution : 8 Bit */
#define ADC12RES__10BIT     (0x0010u)    /* ADC12+ Resolution : 10 Bit */
#define ADC12RES__12BIT     (0x0020u)    /* ADC12+ Resolution : 12 Bit */

/* ADC12CTL3 Control Bits */
#define ADC12CSTARTADD0     (0x0001u)    /* ADC12 Conversion Start Address Bit: 0 */
#define ADC12CSTARTADD1     (0x0002u)    /* ADC12 Conversion Start Address Bit: 1 */
#define ADC12CSTARTADD2     (0x0004u)    /* ADC12 Conversion Start Address Bit: 2 */
#define ADC12CSTARTADD3     (0x0008u)    /* ADC12 Conversion Start Address Bit: 3 */
#define ADC12CSTARTADD4     (0x0010u)    /* ADC12 Conversion Start Address Bit: 4 */
#define ADC12BATMAP         (0x0040u)    /* ADC12 Internal AVCC/2 select */
#define ADC12TCMAP          (0x0080u)    /* ADC12 Internal TempSensor select */
#define ADC12ICH0MAP        (0x0100u)    /* ADC12 Internal Channel 0 select */
#define ADC12ICH1MAP        (0x0200u)    /* ADC12 Internal Channel 1 select */
#define ADC12ICH2MAP        (0x0400u)    /* ADC12 Internal Channel 2 select */
#define ADC12ICH3MAP        (0x0800u)    /* ADC12 Internal Channel 3 select */

/* ADC12CTL3 Control Bits */
#define ADC12CSTARTADD0_L   (0x0001u)    /* ADC12 Conversion Start Address Bit: 0 */
#define ADC12CSTARTADD1_L   (0x0002u)    /* ADC12 Conversion Start Address Bit: 1 */
#define ADC12CSTARTADD2_L   (0x0004u)    /* ADC12 Conversion Start Address Bit: 2 */
#define ADC12CSTARTADD3_L   (0x0008u)    /* ADC12 Conversion Start Address Bit: 3 */
#define ADC12CSTARTADD4_L   (0x0010u)    /* ADC12 Conversion Start Address Bit: 4 */
#define ADC12BATMAP_L       (0x0040u)    /* ADC12 Internal AVCC/2 select */
#define ADC12TCMAP_L        (0x0080u)    /* ADC12 Internal TempSensor select */

/* ADC12CTL3 Control Bits */
#define ADC12ICH0MAP_H      (0x0001u)    /* ADC12 Internal Channel 0 select */
#define ADC12ICH1MAP_H      (0x0002u)    /* ADC12 Internal Channel 1 select */
#define ADC12ICH2MAP_H      (0x0004u)    /* ADC12 Internal Channel 2 select */
#define ADC12ICH3MAP_H      (0x0008u)    /* ADC12 Internal Channel 3 select */

#define ADC12CSTARTADD_0     ( 0*0x0001u) /* ADC12 Conversion Start Address: 0 */
#define ADC12CSTARTADD_1     ( 1*0x0001u) /* ADC12 Conversion Start Address: 1 */
#define ADC12CSTARTADD_2     ( 2*0x0001u) /* ADC12 Conversion Start Address: 2 */
#define ADC12CSTARTADD_3     ( 3*0x0001u) /* ADC12 Conversion Start Address: 3 */
#define ADC12CSTARTADD_4     ( 4*0x0001u) /* ADC12 Conversion Start Address: 4 */
#define ADC12CSTARTADD_5     ( 5*0x0001u) /* ADC12 Conversion Start Address: 5 */
#define ADC12CSTARTADD_6     ( 6*0x0001u) /* ADC12 Conversion Start Address: 6 */
#define ADC12CSTARTADD_7     ( 7*0x0001u) /* ADC12 Conversion Start Address: 7 */
#define ADC12CSTARTADD_8     ( 8*0x0001u) /* ADC12 Conversion Start Address: 8 */
#define ADC12CSTARTADD_9     ( 9*0x0001u) /* ADC12 Conversion Start Address: 9 */
#define ADC12CSTARTADD_10    (10*0x0001u) /* ADC12 Conversion Start Address: 10 */
#define ADC12CSTARTADD_11    (11*0x0001u) /* ADC12 Conversion Start Address: 11 */
#define ADC12CSTARTADD_12    (12*0x0001u) /* ADC12 Conversion Start Address: 12 */
#define ADC12CSTARTADD_13    (13*0x0001u) /* ADC12 Conversion Start Address: 13 */
#define ADC12CSTARTADD_14    (14*0x0001u) /* ADC12 Conversion Start Address: 14 */
#define ADC12CSTARTADD_15    (15*0x0001u) /* ADC12 Conversion Start Address: 15 */
#define ADC12CSTARTADD_16    (16*0x0001u) /* ADC12 Conversion Start Address: 16 */
#define ADC12CSTARTADD_17    (17*0x0001u) /* ADC12 Conversion Start Address: 17 */
#define ADC12CSTARTADD_18    (18*0x0001u) /* ADC12 Conversion Start Address: 18 */
#define ADC12CSTARTADD_19    (19*0x0001u) /* ADC12 Conversion Start Address: 19 */
#define ADC12CSTARTADD_20    (20*0x0001u) /* ADC12 Conversion Start Address: 20 */
#define ADC12CSTARTADD_21    (21*0x0001u) /* ADC12 Conversion Start Address: 21 */
#define ADC12CSTARTADD_22    (22*0x0001u) /* ADC12 Conversion Start Address: 22 */
#define ADC12CSTARTADD_23    (23*0x0001u) /* ADC12 Conversion Start Address: 23 */
#define ADC12CSTARTADD_24    (24*0x0001u) /* ADC12 Conversion Start Address: 24 */
#define ADC12CSTARTADD_25    (25*0x0001u) /* ADC12 Conversion Start Address: 25 */
#define ADC12CSTARTADD_26    (26*0x0001u) /* ADC12 Conversion Start Address: 26 */
#define ADC12CSTARTADD_27    (27*0x0001u) /* ADC12 Conversion Start Address: 27 */
#define ADC12CSTARTADD_28    (28*0x0001u) /* ADC12 Conversion Start Address: 28 */
#define ADC12CSTARTADD_29    (29*0x0001u) /* ADC12 Conversion Start Address: 29 */
#define ADC12CSTARTADD_30    (30*0x0001u) /* ADC12 Conversion Start Address: 30 */
#define ADC12CSTARTADD_31    (31*0x0001u) /* ADC12 Conversion Start Address: 31 */

/* ADC12MCTLx Control Bits */
#define ADC12INCH0          (0x0001u)    /* ADC12 Input Channel Select Bit 0 */
#define ADC12INCH1          (0x0002u)    /* ADC12 Input Channel Select Bit 1 */
#define ADC12INCH2          (0x0004u)    /* ADC12 Input Channel Select Bit 2 */
#define ADC12INCH3          (0x0008u)    /* ADC12 Input Channel Select Bit 3 */
#define ADC12INCH4          (0x0010u)    /* ADC12 Input Channel Select Bit 4 */
#define ADC12EOS            (0x0080u)    /* ADC12 End of Sequence */
#define ADC12VRSEL0         (0x0100u)    /* ADC12 VR Select Bit 0 */
#define ADC12VRSEL1         (0x0200u)    /* ADC12 VR Select Bit 1 */
#define ADC12VRSEL2         (0x0400u)    /* ADC12 VR Select Bit 2 */
#define ADC12VRSEL3         (0x0800u)    /* ADC12 VR Select Bit 3 */
#define ADC12DIF            (0x2000u)    /* ADC12 Differential mode (only for even Registers) */
#define ADC12WINC           (0x4000u)    /* ADC12 Comparator window enable */

/* ADC12MCTLx Control Bits */
#define ADC12INCH0_L        (0x0001u)    /* ADC12 Input Channel Select Bit 0 */
#define ADC12INCH1_L        (0x0002u)    /* ADC12 Input Channel Select Bit 1 */
#define ADC12INCH2_L        (0x0004u)    /* ADC12 Input Channel Select Bit 2 */
#define ADC12INCH3_L        (0x0008u)    /* ADC12 Input Channel Select Bit 3 */
#define ADC12INCH4_L        (0x0010u)    /* ADC12 Input Channel Select Bit 4 */
#define ADC12EOS_L          (0x0080u)    /* ADC12 End of Sequence */

/* ADC12MCTLx Control Bits */
#define ADC12VRSEL0_H       (0x0001u)    /* ADC12 VR Select Bit 0 */
#define ADC12VRSEL1_H       (0x0002u)    /* ADC12 VR Select Bit 1 */
#define ADC12VRSEL2_H       (0x0004u)    /* ADC12 VR Select Bit 2 */
#define ADC12VRSEL3_H       (0x0008u)    /* ADC12 VR Select Bit 3 */
#define ADC12DIF_H          (0x0020u)    /* ADC12 Differential mode (only for even Registers) */
#define ADC12WINC_H         (0x0040u)    /* ADC12 Comparator window enable */

#define ADC12INCH_0         (0x0000u)    /* ADC12 Input Channel 0 */
#define ADC12INCH_1         (0x0001u)    /* ADC12 Input Channel 1 */
#define ADC12INCH_2         (0x0002u)    /* ADC12 Input Channel 2 */
#define ADC12INCH_3         (0x0003u)    /* ADC12 Input Channel 3 */
#define ADC12INCH_4         (0x0004u)    /* ADC12 Input Channel 4 */
#define ADC12INCH_5         (0x0005u)    /* ADC12 Input Channel 5 */
#define ADC12INCH_6         (0x0006u)    /* ADC12 Input Channel 6 */
#define ADC12INCH_7         (0x0007u)    /* ADC12 Input Channel 7 */
#define ADC12INCH_8         (0x0008u)    /* ADC12 Input Channel 8 */
#define ADC12INCH_9         (0x0009u)    /* ADC12 Input Channel 9 */
#define ADC12INCH_10        (0x000Au)    /* ADC12 Input Channel 10 */
#define ADC12INCH_11        (0x000Bu)    /* ADC12 Input Channel 11 */
#define ADC12INCH_12        (0x000Cu)    /* ADC12 Input Channel 12 */
#define ADC12INCH_13        (0x000Du)    /* ADC12 Input Channel 13 */
#define ADC12INCH_14        (0x000Eu)    /* ADC12 Input Channel 14 */
#define ADC12INCH_15        (0x000Fu)    /* ADC12 Input Channel 15 */
#define ADC12INCH_16        (0x0010u)    /* ADC12 Input Channel 16 */
#define ADC12INCH_17        (0x0011u)    /* ADC12 Input Channel 17 */
#define ADC12INCH_18        (0x0012u)    /* ADC12 Input Channel 18 */
#define ADC12INCH_19        (0x0013u)    /* ADC12 Input Channel 19 */
#define ADC12INCH_20        (0x0014u)    /* ADC12 Input Channel 20 */
#define ADC12INCH_21        (0x0015u)    /* ADC12 Input Channel 21 */
#define ADC12INCH_22        (0x0016u)    /* ADC12 Input Channel 22 */
#define ADC12INCH_23        (0x0017u)    /* ADC12 Input Channel 23 */
#define ADC12INCH_24        (0x0018u)    /* ADC12 Input Channel 24 */
#define ADC12INCH_25        (0x0019u)    /* ADC12 Input Channel 25 */
#define ADC12INCH_26        (0x001Au)    /* ADC12 Input Channel 26 */
#define ADC12INCH_27        (0x001Bu)    /* ADC12 Input Channel 27 */
#define ADC12INCH_28        (0x001Cu)    /* ADC12 Input Channel 28 */
#define ADC12INCH_29        (0x001Du)    /* ADC12 Input Channel 29 */
#define ADC12INCH_30        (0x001Eu)    /* ADC12 Input Channel 30 */
#define ADC12INCH_31        (0x001Fu)    /* ADC12 Input Channel 31 */

#define ADC12VRSEL_0        (0*0x100u)    /* ADC12 Select Reference 0 */
#define ADC12VRSEL_1        (1*0x100u)    /* ADC12 Select Reference 1 */
#define ADC12VRSEL_2        (2*0x100u)    /* ADC12 Select Reference 2 */
#define ADC12VRSEL_3        (3*0x100u)    /* ADC12 Select Reference 3 */
#define ADC12VRSEL_4        (4*0x100u)    /* ADC12 Select Reference 4 */
#define ADC12VRSEL_5        (5*0x100u)    /* ADC12 Select Reference 5 */
#define ADC12VRSEL_6        (6*0x100u)    /* ADC12 Select Reference 6 */
#define ADC12VRSEL_7        (7*0x100u)    /* ADC12 Select Reference 7 */
#define ADC12VRSEL_8        (8*0x100u)    /* ADC12 Select Reference 8  */
#define ADC12VRSEL_9        (9*0x100u)    /* ADC12 Select Reference 9  */
#define ADC12VRSEL_10       (10*0x100u)   /* ADC12 Select Reference 10 */
#define ADC12VRSEL_11       (11*0x100u)   /* ADC12 Select Reference 11 */
#define ADC12VRSEL_12       (12*0x100u)   /* ADC12 Select Reference 12 */
#define ADC12VRSEL_13       (13*0x100u)   /* ADC12 Select Reference 13 */
#define ADC12VRSEL_14       (14*0x100u)   /* ADC12 Select Reference 14 */
#define ADC12VRSEL_15       (15*0x100u)   /* ADC12 Select Reference 15 */

/* ADC12HI Control Bits */

/* ADC12LO Control Bits */

/* ADC12IER0 Control Bits */
#define ADC12IE0          (0x0001u)  /* ADC12 Memory 0 Interrupt Enable */
#define ADC12IE1          (0x0002u)  /* ADC12 Memory 1 Interrupt Enable */
#define ADC12IE2          (0x0004u)  /* ADC12 Memory 2 Interrupt Enable */
#define ADC12IE3          (0x0008u)  /* ADC12 Memory 3 Interrupt Enable */
#define ADC12IE4          (0x0010u)  /* ADC12 Memory 4 Interrupt Enable */
#define ADC12IE5          (0x0020u)  /* ADC12 Memory 5 Interrupt Enable */
#define ADC12IE6          (0x0040u)  /* ADC12 Memory 6 Interrupt Enable */
#define ADC12IE7          (0x0080u)  /* ADC12 Memory 7 Interrupt Enable */
#define ADC12IE8          (0x0100u)  /* ADC12 Memory 8 Interrupt Enable */
#define ADC12IE9          (0x0200u)  /* ADC12 Memory 9 Interrupt Enable */
#define ADC12IE10          (0x0400u)  /* ADC12 Memory 10 Interrupt Enable */
#define ADC12IE11          (0x0800u)  /* ADC12 Memory 11 Interrupt Enable */
#define ADC12IE12          (0x1000u)  /* ADC12 Memory 12 Interrupt Enable */
#define ADC12IE13          (0x2000u)  /* ADC12 Memory 13 Interrupt Enable */
#define ADC12IE14          (0x4000u)  /* ADC12 Memory 14 Interrupt Enable */
#define ADC12IE15          (0x8000u)  /* ADC12 Memory 15 Interrupt Enable */

/* ADC12IER0 Control Bits */
#define ADC12IE0_L          (0x0001u)  /* ADC12 Memory 0 Interrupt Enable */
#define ADC12IE1_L          (0x0002u)  /* ADC12 Memory 1 Interrupt Enable */
#define ADC12IE2_L          (0x0004u)  /* ADC12 Memory 2 Interrupt Enable */
#define ADC12IE3_L          (0x0008u)  /* ADC12 Memory 3 Interrupt Enable */
#define ADC12IE4_L          (0x0010u)  /* ADC12 Memory 4 Interrupt Enable */
#define ADC12IE5_L          (0x0020u)  /* ADC12 Memory 5 Interrupt Enable */
#define ADC12IE6_L          (0x0040u)  /* ADC12 Memory 6 Interrupt Enable */
#define ADC12IE7_L          (0x0080u)  /* ADC12 Memory 7 Interrupt Enable */

/* ADC12IER0 Control Bits */
#define ADC12IE8_H          (0x0001u)  /* ADC12 Memory 8 Interrupt Enable */
#define ADC12IE9_H          (0x0002u)  /* ADC12 Memory 9 Interrupt Enable */
#define ADC12IE10_H         (0x0004u)  /* ADC12 Memory 10 Interrupt Enable */
#define ADC12IE11_H         (0x0008u)  /* ADC12 Memory 11 Interrupt Enable */
#define ADC12IE12_H         (0x0010u)  /* ADC12 Memory 12 Interrupt Enable */
#define ADC12IE13_H         (0x0020u)  /* ADC12 Memory 13 Interrupt Enable */
#define ADC12IE14_H         (0x0040u)  /* ADC12 Memory 14 Interrupt Enable */
#define ADC12IE15_H         (0x0080u)  /* ADC12 Memory 15 Interrupt Enable */

/* ADC12IER1 Control Bits */
#define ADC12IE16          (0x0001u)  /* ADC12 Memory 16 Interrupt Enable */
#define ADC12IE17          (0x0002u)  /* ADC12 Memory 17 Interrupt Enable */
#define ADC12IE18          (0x0004u)  /* ADC12 Memory 18 Interrupt Enable */
#define ADC12IE19          (0x0008u)  /* ADC12 Memory 19 Interrupt Enable */
#define ADC12IE20          (0x0010u)  /* ADC12 Memory 20 Interrupt Enable */
#define ADC12IE21          (0x0020u)  /* ADC12 Memory 21 Interrupt Enable */
#define ADC12IE22          (0x0040u)  /* ADC12 Memory 22 Interrupt Enable */
#define ADC12IE23          (0x0080u)  /* ADC12 Memory 23 Interrupt Enable */
#define ADC12IE24          (0x0100u)  /* ADC12 Memory 24 Interrupt Enable */
#define ADC12IE25          (0x0200u)  /* ADC12 Memory 25 Interrupt Enable */
#define ADC12IE26          (0x0400u)  /* ADC12 Memory 26 Interrupt Enable */
#define ADC12IE27          (0x0800u)  /* ADC12 Memory 27 Interrupt Enable */
#define ADC12IE28          (0x1000u)  /* ADC12 Memory 28 Interrupt Enable */
#define ADC12IE29          (0x2000u)  /* ADC12 Memory 29 Interrupt Enable */
#define ADC12IE30          (0x4000u)  /* ADC12 Memory 30 Interrupt Enable */
#define ADC12IE31          (0x8000u)  /* ADC12 Memory 31 Interrupt Enable */

/* ADC12IER1 Control Bits */
#define ADC12IE16_L         (0x0001u)  /* ADC12 Memory 16 Interrupt Enable */
#define ADC12IE17_L         (0x0002u)  /* ADC12 Memory 17 Interrupt Enable */
#define ADC12IE18_L         (0x0004u)  /* ADC12 Memory 18 Interrupt Enable */
#define ADC12IE19_L         (0x0008u)  /* ADC12 Memory 19 Interrupt Enable */
#define ADC12IE20_L         (0x0010u)  /* ADC12 Memory 20 Interrupt Enable */
#define ADC12IE21_L         (0x0020u)  /* ADC12 Memory 21 Interrupt Enable */
#define ADC12IE22_L         (0x0040u)  /* ADC12 Memory 22 Interrupt Enable */
#define ADC12IE23_L         (0x0080u)  /* ADC12 Memory 23 Interrupt Enable */

/* ADC12IER1 Control Bits */
#define ADC12IE24_H         (0x0001u)  /* ADC12 Memory 24 Interrupt Enable */
#define ADC12IE25_H         (0x0002u)  /* ADC12 Memory 25 Interrupt Enable */
#define ADC12IE26_H         (0x0004u)  /* ADC12 Memory 26 Interrupt Enable */
#define ADC12IE27_H         (0x0008u)  /* ADC12 Memory 27 Interrupt Enable */
#define ADC12IE28_H         (0x0010u)  /* ADC12 Memory 28 Interrupt Enable */
#define ADC12IE29_H         (0x0020u)  /* ADC12 Memory 29 Interrupt Enable */
#define ADC12IE30_H         (0x0040u)  /* ADC12 Memory 30 Interrupt Enable */
#define ADC12IE31_H         (0x0080u)  /* ADC12 Memory 31 Interrupt Enable */

/* ADC12IER2 Control Bits */
#define ADC12INIE           (0x0002u)    /* ADC12 Interrupt enable for the inside of window of the Window comparator */
#define ADC12LOIE           (0x0004u)    /* ADC12 Interrupt enable for lower threshold of the Window comparator */
#define ADC12HIIE           (0x0008u)    /* ADC12 Interrupt enable for upper threshold of the Window comparator */
#define ADC12OVIE           (0x0010u)    /* ADC12 ADC12MEMx Overflow interrupt enable */
#define ADC12TOVIE          (0x0020u)    /* ADC12 Timer Overflow interrupt enable */
#define ADC12RDYIE          (0x0040u)    /* ADC12 local buffered reference ready interrupt enable */

/* ADC12IER2 Control Bits */
#define ADC12INIE_L         (0x0002u)    /* ADC12 Interrupt enable for the inside of window of the Window comparator */
#define ADC12LOIE_L         (0x0004u)    /* ADC12 Interrupt enable for lower threshold of the Window comparator */
#define ADC12HIIE_L         (0x0008u)    /* ADC12 Interrupt enable for upper threshold of the Window comparator */
#define ADC12OVIE_L         (0x0010u)    /* ADC12 ADC12MEMx Overflow interrupt enable */
#define ADC12TOVIE_L        (0x0020u)    /* ADC12 Timer Overflow interrupt enable */
#define ADC12RDYIE_L        (0x0040u)    /* ADC12 local buffered reference ready interrupt enable */

/* ADC12IFGR0 Control Bits */
#define ADC12IFG0          (0x0001u)  /* ADC12 Memory 0 Interrupt Flag */
#define ADC12IFG1          (0x0002u)  /* ADC12 Memory 1 Interrupt Flag */
#define ADC12IFG2          (0x0004u)  /* ADC12 Memory 2 Interrupt Flag */
#define ADC12IFG3          (0x0008u)  /* ADC12 Memory 3 Interrupt Flag */
#define ADC12IFG4          (0x0010u)  /* ADC12 Memory 4 Interrupt Flag */
#define ADC12IFG5          (0x0020u)  /* ADC12 Memory 5 Interrupt Flag */
#define ADC12IFG6          (0x0040u)  /* ADC12 Memory 6 Interrupt Flag */
#define ADC12IFG7          (0x0080u)  /* ADC12 Memory 7 Interrupt Flag */
#define ADC12IFG8          (0x0100u)  /* ADC12 Memory 8 Interrupt Flag */
#define ADC12IFG9          (0x0200u)  /* ADC12 Memory 9 Interrupt Flag */
#define ADC12IFG10          (0x0400u)  /* ADC12 Memory 10 Interrupt Flag */
#define ADC12IFG11          (0x0800u)  /* ADC12 Memory 11 Interrupt Flag */
#define ADC12IFG12          (0x1000u)  /* ADC12 Memory 12 Interrupt Flag */
#define ADC12IFG13          (0x2000u)  /* ADC12 Memory 13 Interrupt Flag */
#define ADC12IFG14          (0x4000u)  /* ADC12 Memory 14 Interrupt Flag */
#define ADC12IFG15          (0x8000u)  /* ADC12 Memory 15 Interrupt Flag */

/* ADC12IFGR0 Control Bits */
#define ADC12IFG0_L         (0x0001u)  /* ADC12 Memory 0 Interrupt Flag */
#define ADC12IFG1_L         (0x0002u)  /* ADC12 Memory 1 Interrupt Flag */
#define ADC12IFG2_L         (0x0004u)  /* ADC12 Memory 2 Interrupt Flag */
#define ADC12IFG3_L         (0x0008u)  /* ADC12 Memory 3 Interrupt Flag */
#define ADC12IFG4_L         (0x0010u)  /* ADC12 Memory 4 Interrupt Flag */
#define ADC12IFG5_L         (0x0020u)  /* ADC12 Memory 5 Interrupt Flag */
#define ADC12IFG6_L         (0x0040u)  /* ADC12 Memory 6 Interrupt Flag */
#define ADC12IFG7_L         (0x0080u)  /* ADC12 Memory 7 Interrupt Flag */

/* ADC12IFGR0 Control Bits */
#define ADC12IFG8_H         (0x0001u)  /* ADC12 Memory 8 Interrupt Flag */
#define ADC12IFG9_H         (0x0002u)  /* ADC12 Memory 9 Interrupt Flag */
#define ADC12IFG10_H        (0x0004u)  /* ADC12 Memory 10 Interrupt Flag */
#define ADC12IFG11_H        (0x0008u)  /* ADC12 Memory 11 Interrupt Flag */
#define ADC12IFG12_H        (0x0010u)  /* ADC12 Memory 12 Interrupt Flag */
#define ADC12IFG13_H        (0x0020u)  /* ADC12 Memory 13 Interrupt Flag */
#define ADC12IFG14_H        (0x0040u)  /* ADC12 Memory 14 Interrupt Flag */
#define ADC12IFG15_H        (0x0080u)  /* ADC12 Memory 15 Interrupt Flag */

/* ADC12IFGR1 Control Bits */
#define ADC12IFG16          (0x0001u)  /* ADC12 Memory 16 Interrupt Flag */
#define ADC12IFG17          (0x0002u)  /* ADC12 Memory 17 Interrupt Flag */
#define ADC12IFG18          (0x0004u)  /* ADC12 Memory 18 Interrupt Flag */
#define ADC12IFG19          (0x0008u)  /* ADC12 Memory 19 Interrupt Flag */
#define ADC12IFG20          (0x0010u)  /* ADC12 Memory 20 Interrupt Flag */
#define ADC12IFG21          (0x0020u)  /* ADC12 Memory 21 Interrupt Flag */
#define ADC12IFG22          (0x0040u)  /* ADC12 Memory 22 Interrupt Flag */
#define ADC12IFG23          (0x0080u)  /* ADC12 Memory 23 Interrupt Flag */
#define ADC12IFG24          (0x0100u)  /* ADC12 Memory 24 Interrupt Flag */
#define ADC12IFG25          (0x0200u)  /* ADC12 Memory 25 Interrupt Flag */
#define ADC12IFG26          (0x0400u)  /* ADC12 Memory 26 Interrupt Flag */
#define ADC12IFG27          (0x0800u)  /* ADC12 Memory 27 Interrupt Flag */
#define ADC12IFG28          (0x1000u)  /* ADC12 Memory 28 Interrupt Flag */
#define ADC12IFG29          (0x2000u)  /* ADC12 Memory 29 Interrupt Flag */
#define ADC12IFG30          (0x4000u)  /* ADC12 Memory 30 Interrupt Flag */
#define ADC12IFG31          (0x8000u)  /* ADC12 Memory 31 Interrupt Flag */

/* ADC12IFGR1 Control Bits */
#define ADC12IFG16_L        (0x0001u)  /* ADC12 Memory 16 Interrupt Flag */
#define ADC12IFG17_L        (0x0002u)  /* ADC12 Memory 17 Interrupt Flag */
#define ADC12IFG18_L        (0x0004u)  /* ADC12 Memory 18 Interrupt Flag */
#define ADC12IFG19_L        (0x0008u)  /* ADC12 Memory 19 Interrupt Flag */
#define ADC12IFG20_L        (0x0010u)  /* ADC12 Memory 20 Interrupt Flag */
#define ADC12IFG21_L        (0x0020u)  /* ADC12 Memory 21 Interrupt Flag */
#define ADC12IFG22_L        (0x0040u)  /* ADC12 Memory 22 Interrupt Flag */
#define ADC12IFG23_L        (0x0080u)  /* ADC12 Memory 23 Interrupt Flag */

/* ADC12IFGR1 Control Bits */
#define ADC12IFG24_H        (0x0001u)  /* ADC12 Memory 24 Interrupt Flag */
#define ADC12IFG25_H        (0x0002u)  /* ADC12 Memory 25 Interrupt Flag */
#define ADC12IFG26_H        (0x0004u)  /* ADC12 Memory 26 Interrupt Flag */
#define ADC12IFG27_H        (0x0008u)  /* ADC12 Memory 27 Interrupt Flag */
#define ADC12IFG28_H        (0x0010u)  /* ADC12 Memory 28 Interrupt Flag */
#define ADC12IFG29_H        (0x0020u)  /* ADC12 Memory 29 Interrupt Flag */
#define ADC12IFG30_H        (0x0040u)  /* ADC12 Memory 30 Interrupt Flag */
#define ADC12IFG31_H        (0x0080u)  /* ADC12 Memory 31 Interrupt Flag */

/* ADC12IFGR2 Control Bits */
#define ADC12INIFG          (0x0002u)    /* ADC12 Interrupt Flag for the inside of window of the Window comparator */
#define ADC12LOIFG          (0x0004u)    /* ADC12 Interrupt Flag for lower threshold of the Window comparator */
#define ADC12HIIFG          (0x0008u)    /* ADC12 Interrupt Flag for upper threshold of the Window comparator */
#define ADC12OVIFG          (0x0010u)    /* ADC12 ADC12MEMx Overflow interrupt Flag */
#define ADC12TOVIFG         (0x0020u)    /* ADC12 Timer Overflow interrupt Flag */
#define ADC12RDYIFG         (0x0040u)    /* ADC12 local buffered reference ready interrupt Flag */

/* ADC12IFGR2 Control Bits */
#define ADC12INIFG_L        (0x0002u)    /* ADC12 Interrupt Flag for the inside of window of the Window comparator */
#define ADC12LOIFG_L        (0x0004u)    /* ADC12 Interrupt Flag for lower threshold of the Window comparator */
#define ADC12HIIFG_L        (0x0008u)    /* ADC12 Interrupt Flag for upper threshold of the Window comparator */
#define ADC12OVIFG_L        (0x0010u)    /* ADC12 ADC12MEMx Overflow interrupt Flag */
#define ADC12TOVIFG_L       (0x0020u)    /* ADC12 Timer Overflow interrupt Flag */
#define ADC12RDYIFG_L       (0x0040u)    /* ADC12 local buffered reference ready interrupt Flag */

/* ADC12IV Definitions */
#define ADC12IV_NONE        (0x0000u)    /* No Interrupt pending */
#define ADC12IV_ADC12OVIFG  (0x0002u)    /* ADC12OVIFG */
#define ADC12IV_ADC12TOVIFG (0x0004u)    /* ADC12TOVIFG */
#define ADC12IV_ADC12HIIFG  (0x0006u)    /* ADC12HIIFG */
#define ADC12IV_ADC12LOIFG  (0x0008u)    /* ADC12LOIFG */
#define ADC12IV_ADC12INIFG  (0x000Au)    /* ADC12INIFG */
#define ADC12IV_ADC12IFG0   (0x000Cu)    /* ADC12IFG0 */
#define ADC12IV_ADC12IFG1   (0x000Eu)    /* ADC12IFG1 */
#define ADC12IV_ADC12IFG2   (0x0010u)    /* ADC12IFG2 */
#define ADC12IV_ADC12IFG3   (0x0012u)    /* ADC12IFG3 */
#define ADC12IV_ADC12IFG4   (0x0014u)    /* ADC12IFG4 */
#define ADC12IV_ADC12IFG5   (0x0016u)    /* ADC12IFG5 */
#define ADC12IV_ADC12IFG6   (0x0018u)    /* ADC12IFG6 */
#define ADC12IV_ADC12IFG7   (0x001Au)    /* ADC12IFG7 */
#define ADC12IV_ADC12IFG8   (0x001Cu)    /* ADC12IFG8 */
#define ADC12IV_ADC12IFG9   (0x001Eu)    /* ADC12IFG9 */
#define ADC12IV_ADC12IFG10  (0x0020u)    /* ADC12IFG10 */
#define ADC12IV_ADC12IFG11  (0x0022u)    /* ADC12IFG11 */
#define ADC12IV_ADC12IFG12  (0x0024u)    /* ADC12IFG12 */
#define ADC12IV_ADC12IFG13  (0x0026u)    /* ADC12IFG13 */
#define ADC12IV_ADC12IFG14  (0x0028u)    /* ADC12IFG14 */
#define ADC12IV_ADC12IFG15  (0x002Au)    /* ADC12IFG15 */
#define ADC12IV_ADC12IFG16  (0x002Cu)    /* ADC12IFG16 */
#define ADC12IV_ADC12IFG17  (0x002Eu)    /* ADC12IFG17 */
#define ADC12IV_ADC12IFG18  (0x0030u)    /* ADC12IFG18 */
#define ADC12IV_ADC12IFG19  (0x0032u)    /* ADC12IFG19 */
#define ADC12IV_ADC12IFG20  (0x0034u)    /* ADC12IFG20 */
#define ADC12IV_ADC12IFG21  (0x0036u)    /* ADC12IFG21 */
#define ADC12IV_ADC12IFG22  (0x0038u)    /* ADC12IFG22 */
#define ADC12IV_ADC12IFG23  (0x003Au)    /* ADC12IFG23 */
#define ADC12IV_ADC12IFG24  (0x003Cu)    /* ADC12IFG24 */
#define ADC12IV_ADC12IFG25  (0x003Eu)    /* ADC12IFG25 */
#define ADC12IV_ADC12IFG26  (0x0040u)    /* ADC12IFG26 */
#define ADC12IV_ADC12IFG27  (0x0042u)    /* ADC12IFG27 */
#define ADC12IV_ADC12IFG28  (0x0044u)    /* ADC12IFG28 */
#define ADC12IV_ADC12IFG29  (0x0046u)    /* ADC12IFG29 */
#define ADC12IV_ADC12IFG30  (0x0048u)    /* ADC12IFG30 */
#define ADC12IV_ADC12IFG31  (0x004Au)    /* ADC12IFG31 */
#define ADC12IV_ADC12RDYIFG (0x004Cu)    /* ADC12RDYIFG */


/************************************************************
* AES256 Accelerator
************************************************************/
#define __MSP430_HAS_AES256__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_AES256__ 0x09C0
#define AES256_BASE __MSP430_BASEADDRESS_AES256__

#define  AESACTL0_              (0x09C0u)  /* AES accelerator control register 0 */
DEFCW(   AESACTL0             , AESACTL0_)
#define  AESACTL1_              (0x09C2u)  /* AES accelerator control register 1 */
DEFCW(   AESACTL1             , AESACTL1_)
#define  AESASTAT_              (0x09C4u)  /* AES accelerator status register */
DEFCW(   AESASTAT             , AESASTAT_)
#define  AESAKEY_               (0x09C6u)  /* AES accelerator key register */
DEFCW(   AESAKEY              , AESAKEY_)
#define  AESADIN_               (0x09C8u)  /* AES accelerator data in register */
DEFCW(   AESADIN              , AESADIN_)
#define  AESADOUT_              (0x09CAu)  /* AES accelerator data out register  */
DEFCW(   AESADOUT             , AESADOUT_)
#define  AESAXDIN_              (0x09CCu)  /* AES accelerator XORed data in register */
DEFCW(   AESAXDIN             , AESAXDIN_)
#define  AESAXIN_               (0x09CEu)  /* AES accelerator XORed data in register (no trigger)  */
DEFCW(   AESAXIN              , AESAXIN_)

/* AESACTL0 Control Bits */
#define AESOP0              (0x0001u)  /* AES Operation Bit: 0 */
#define AESOP1              (0x0002u)  /* AES Operation Bit: 1 */
#define AESKL0              (0x0004u)  /* AES Key length Bit: 0 */
#define AESKL1              (0x0008u)  /* AES Key length Bit: 1 */
#define AESTRIG             (0x0010u)  /* AES Trigger Select */
#define AESCM0              (0x0020u)  /* AES Cipher mode select Bit: 0 */
#define AESCM1              (0x0040u)  /* AES Cipher mode select Bit: 1 */
#define AESSWRST            (0x0080u)  /* AES Software Reset */
#define AESRDYIFG           (0x0100u)  /* AES ready interrupt flag */
#define AESERRFG            (0x0800u)  /* AES Error Flag */
#define AESRDYIE            (0x1000u)  /* AES ready interrupt enable*/
#define AESCMEN             (0x8000u)  /* AES DMA cipher mode enable*/

/* AESACTL0 Control Bits */
#define AESOP0_L            (0x0001u)  /* AES Operation Bit: 0 */
#define AESOP1_L            (0x0002u)  /* AES Operation Bit: 1 */
#define AESKL0_L            (0x0004u)  /* AES Key length Bit: 0 */
#define AESKL1_L            (0x0008u)  /* AES Key length Bit: 1 */
#define AESTRIG_L           (0x0010u)  /* AES Trigger Select */
#define AESCM0_L            (0x0020u)  /* AES Cipher mode select Bit: 0 */
#define AESCM1_L            (0x0040u)  /* AES Cipher mode select Bit: 1 */
#define AESSWRST_L          (0x0080u)  /* AES Software Reset */

/* AESACTL0 Control Bits */
#define AESRDYIFG_H         (0x0001u)  /* AES ready interrupt flag */
#define AESERRFG_H          (0x0008u)  /* AES Error Flag */
#define AESRDYIE_H          (0x0010u)  /* AES ready interrupt enable*/
#define AESCMEN_H           (0x0080u)  /* AES DMA cipher mode enable*/

#define AESOP_0             (0x0000u)  /* AES Operation: Encrypt */
#define AESOP_1             (0x0001u)  /* AES Operation: Decrypt (same Key) */
#define AESOP_2             (0x0002u)  /* AES Operation: Decrypt (frist round Key) */
#define AESOP_3             (0x0003u)  /* AES Operation: Generate first round Key */

#define AESKL_0             (0x0000u)  /* AES Key length: AES128 */
#define AESKL_1             (0x0004u)  /* AES Key length: AES192 */
#define AESKL_2             (0x0008u)  /* AES Key length: AES256 */
#define AESKL__128          (0x0000u)  /* AES Key length: AES128 */
#define AESKL__192          (0x0004u)  /* AES Key length: AES192 */
#define AESKL__256          (0x0008u)  /* AES Key length: AES256 */

#define AESCM_0             (0x0000u)  /* AES Cipher mode select: ECB */
#define AESCM_1             (0x0020u)  /* AES Cipher mode select: CBC */
#define AESCM_2             (0x0040u)  /* AES Cipher mode select: OFB */
#define AESCM_3             (0x0060u)  /* AES Cipher mode select: CFB */
#define AESCM__ECB          (0x0000u)  /* AES Cipher mode select: ECB */
#define AESCM__CBC          (0x0020u)  /* AES Cipher mode select: CBC */
#define AESCM__OFB          (0x0040u)  /* AES Cipher mode select: OFB */
#define AESCM__CFB          (0x0060u)  /* AES Cipher mode select: CFB */

/* AESACTL1 Control Bits */
#define AESBLKCNT0          (0x0001u)  /* AES Cipher Block Counter Bit: 0 */
#define AESBLKCNT1          (0x0002u)  /* AES Cipher Block Counter Bit: 1 */
#define AESBLKCNT2          (0x0004u)  /* AES Cipher Block Counter Bit: 2 */
#define AESBLKCNT3          (0x0008u)  /* AES Cipher Block Counter Bit: 3 */
#define AESBLKCNT4          (0x0010u)  /* AES Cipher Block Counter Bit: 4 */
#define AESBLKCNT5          (0x0020u)  /* AES Cipher Block Counter Bit: 5 */
#define AESBLKCNT6          (0x0040u)  /* AES Cipher Block Counter Bit: 6 */
#define AESBLKCNT7          (0x0080u)  /* AES Cipher Block Counter Bit: 7 */

/* AESACTL1 Control Bits */
#define AESBLKCNT0_L        (0x0001u)  /* AES Cipher Block Counter Bit: 0 */
#define AESBLKCNT1_L        (0x0002u)  /* AES Cipher Block Counter Bit: 1 */
#define AESBLKCNT2_L        (0x0004u)  /* AES Cipher Block Counter Bit: 2 */
#define AESBLKCNT3_L        (0x0008u)  /* AES Cipher Block Counter Bit: 3 */
#define AESBLKCNT4_L        (0x0010u)  /* AES Cipher Block Counter Bit: 4 */
#define AESBLKCNT5_L        (0x0020u)  /* AES Cipher Block Counter Bit: 5 */
#define AESBLKCNT6_L        (0x0040u)  /* AES Cipher Block Counter Bit: 6 */
#define AESBLKCNT7_L        (0x0080u)  /* AES Cipher Block Counter Bit: 7 */

/* AESASTAT Control Bits */
#define AESBUSY             (0x0001u)  /* AES Busy */
#define AESKEYWR            (0x0002u)  /* AES All 16 bytes written to AESAKEY */
#define AESDINWR            (0x0004u)  /* AES All 16 bytes written to AESADIN */
#define AESDOUTRD           (0x0008u)  /* AES All 16 bytes read from AESADOUT */
#define AESKEYCNT0          (0x0010u)  /* AES Bytes written via AESAKEY Bit: 0 */
#define AESKEYCNT1          (0x0020u)  /* AES Bytes written via AESAKEY Bit: 1 */
#define AESKEYCNT2          (0x0040u)  /* AES Bytes written via AESAKEY Bit: 2 */
#define AESKEYCNT3          (0x0080u)  /* AES Bytes written via AESAKEY Bit: 3 */
#define AESDINCNT0          (0x0100u)  /* AES Bytes written via AESADIN Bit: 0 */
#define AESDINCNT1          (0x0200u)  /* AES Bytes written via AESADIN Bit: 1 */
#define AESDINCNT2          (0x0400u)  /* AES Bytes written via AESADIN Bit: 2 */
#define AESDINCNT3          (0x0800u)  /* AES Bytes written via AESADIN Bit: 3 */
#define AESDOUTCNT0         (0x1000u)  /* AES Bytes read via AESADOUT Bit: 0 */
#define AESDOUTCNT1         (0x2000u)  /* AES Bytes read via AESADOUT Bit: 1 */
#define AESDOUTCNT2         (0x4000u)  /* AES Bytes read via AESADOUT Bit: 2 */
#define AESDOUTCNT3         (0x8000u)  /* AES Bytes read via AESADOUT Bit: 3 */

/* AESASTAT Control Bits */
#define AESBUSY_L           (0x0001u)  /* AES Busy */
#define AESKEYWR_L          (0x0002u)  /* AES All 16 bytes written to AESAKEY */
#define AESDINWR_L          (0x0004u)  /* AES All 16 bytes written to AESADIN */
#define AESDOUTRD_L         (0x0008u)  /* AES All 16 bytes read from AESADOUT */
#define AESKEYCNT0_L        (0x0010u)  /* AES Bytes written via AESAKEY Bit: 0 */
#define AESKEYCNT1_L        (0x0020u)  /* AES Bytes written via AESAKEY Bit: 1 */
#define AESKEYCNT2_L        (0x0040u)  /* AES Bytes written via AESAKEY Bit: 2 */
#define AESKEYCNT3_L        (0x0080u)  /* AES Bytes written via AESAKEY Bit: 3 */

/* AESASTAT Control Bits */
#define AESDINCNT0_H        (0x0001u)  /* AES Bytes written via AESADIN Bit: 0 */
#define AESDINCNT1_H        (0x0002u)  /* AES Bytes written via AESADIN Bit: 1 */
#define AESDINCNT2_H        (0x0004u)  /* AES Bytes written via AESADIN Bit: 2 */
#define AESDINCNT3_H        (0x0008u)  /* AES Bytes written via AESADIN Bit: 3 */
#define AESDOUTCNT0_H       (0x0010u)  /* AES Bytes read via AESADOUT Bit: 0 */
#define AESDOUTCNT1_H       (0x0020u)  /* AES Bytes read via AESADOUT Bit: 1 */
#define AESDOUTCNT2_H       (0x0040u)  /* AES Bytes read via AESADOUT Bit: 2 */
#define AESDOUTCNT3_H       (0x0080u)  /* AES Bytes read via AESADOUT Bit: 3 */

/************************************************************
* Capacitive_Touch_IO 0
************************************************************/
#define __MSP430_HAS_CAP_TOUCH_IO_0__ /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CAP_TOUCH_IO_0__ 0x0430
#define CAP_TOUCH_0_BASE __MSP430_BASEADDRESS_CAP_TOUCH_IO_0__

#define  CAPTIO0CTL_           (0x043Eu)  /* Capacitive_Touch_IO 0 control register */
DEFCW(   CAPTIO0CTL          , CAPTIO0CTL_)

#define  CAPSIO0CTL         CAPTIO0CTL  /* legacy define */

/************************************************************
* Capacitive_Touch_IO 1
************************************************************/
#define __MSP430_HAS_CAP_TOUCH_IO_1__ /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CAP_TOUCH_IO_1__ 0x0470
#define CAP_TOUCH_1_BASE __MSP430_BASEADDRESS_CAP_TOUCH_IO_1__

#define  CAPTIO1CTL_           (0x047Eu)  /* Capacitive_Touch_IO 1 control register */
DEFCW(   CAPTIO1CTL          , CAPTIO1CTL_)

#define  CAPSIO1CTL         CAPTIO1CTL  /* legacy define */

/* CAPTIOxCTL Control Bits */
#define CAPTIOPISEL0         (0x0002u)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPTIOPISEL1         (0x0004u)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPTIOPISEL2         (0x0008u)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPTIOPOSEL0         (0x0010u)    /* CapTouchIO Port Select Bit: 0 */
#define CAPTIOPOSEL1         (0x0020u)    /* CapTouchIO Port Select Bit: 1 */
#define CAPTIOPOSEL2         (0x0040u)    /* CapTouchIO Port Select Bit: 2 */
#define CAPTIOPOSEL3         (0x0080u)    /* CapTouchIO Port Select Bit: 3 */
#define CAPTIOEN             (0x0100u)    /* CapTouchIO Enable */
#define CAPTIO               (0x0200u)    /* CapTouchIO state */

/* CAPTIOxCTL Control Bits */
#define CAPTIOPISEL0_L      (0x0002u)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPTIOPISEL1_L      (0x0004u)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPTIOPISEL2_L      (0x0008u)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPTIOPOSEL0_L      (0x0010u)    /* CapTouchIO Port Select Bit: 0 */
#define CAPTIOPOSEL1_L      (0x0020u)    /* CapTouchIO Port Select Bit: 1 */
#define CAPTIOPOSEL2_L      (0x0040u)    /* CapTouchIO Port Select Bit: 2 */
#define CAPTIOPOSEL3_L      (0x0080u)    /* CapTouchIO Port Select Bit: 3 */

/* CAPTIOxCTL Control Bits */
#define CAPTIOEN_H          (0x0001u)    /* CapTouchIO Enable */
#define CAPTIO_H            (0x0002u)    /* CapTouchIO state */

/* Legacy defines */
#define CAPSIOPISEL0         (0x0002u)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPSIOPISEL1         (0x0004u)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPSIOPISEL2         (0x0008u)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPSIOPOSEL0         (0x0010u)    /* CapTouchIO Port Select Bit: 0 */
#define CAPSIOPOSEL1         (0x0020u)    /* CapTouchIO Port Select Bit: 1 */
#define CAPSIOPOSEL2         (0x0040u)    /* CapTouchIO Port Select Bit: 2 */
#define CAPSIOPOSEL3         (0x0080u)    /* CapTouchIO Port Select Bit: 3 */
#define CAPSIOEN             (0x0100u)    /* CapTouchIO Enable */
#define CAPSIO               (0x0200u)    /* CapTouchIO state */

/************************************************************
* Comparator E
************************************************************/
#define __MSP430_HAS_COMP_E__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_COMP_E__ 0x08C0
#define COMP_E_BASE __MSP430_BASEADDRESS_COMP_E__

#define  CECTL0_                (0x08C0u)  /* Comparator E Control Register 0 */
DEFCW(   CECTL0               , CECTL0_)
#define  CECTL1_                (0x08C2u)  /* Comparator E Control Register 1 */
DEFCW(   CECTL1               , CECTL1_)
#define  CECTL2_                (0x08C4u)  /* Comparator E Control Register 2 */
DEFCW(   CECTL2               , CECTL2_)
#define  CECTL3_                (0x08C6u)  /* Comparator E Control Register 3 */
DEFCW(   CECTL3               , CECTL3_)
#define  CEINT_                 (0x08CCu)  /* Comparator E Interrupt Register */
DEFCW(   CEINT                , CEINT_)
#define CEIV_                  (0x08CEu)  /* Comparator E Interrupt Vector Word */
READ_ONLY_DEFCW( CEIV              , CEIV_)

/* CECTL0 Control Bits */
#define CEIPSEL0            (0x0001u)  /* Comp. E Pos. Channel Input Select 0 */
#define CEIPSEL1            (0x0002u)  /* Comp. E Pos. Channel Input Select 1 */
#define CEIPSEL2            (0x0004u)  /* Comp. E Pos. Channel Input Select 2 */
#define CEIPSEL3            (0x0008u)  /* Comp. E Pos. Channel Input Select 3 */
//#define RESERVED            (0x0010u)  /* Comp. E */
//#define RESERVED            (0x0020u)  /* Comp. E */
//#define RESERVED            (0x0040u)  /* Comp. E */
#define CEIPEN              (0x0080u)  /* Comp. E Pos. Channel Input Enable */
#define CEIMSEL0            (0x0100u)  /* Comp. E Neg. Channel Input Select 0 */
#define CEIMSEL1            (0x0200u)  /* Comp. E Neg. Channel Input Select 1 */
#define CEIMSEL2            (0x0400u)  /* Comp. E Neg. Channel Input Select 2 */
#define CEIMSEL3            (0x0800u)  /* Comp. E Neg. Channel Input Select 3 */
//#define RESERVED            (0x1000u)  /* Comp. E */
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
#define CEIMEN              (0x8000u)  /* Comp. E Neg. Channel Input Enable */

/* CECTL0 Control Bits */
#define CEIPSEL0_L          (0x0001u)  /* Comp. E Pos. Channel Input Select 0 */
#define CEIPSEL1_L          (0x0002u)  /* Comp. E Pos. Channel Input Select 1 */
#define CEIPSEL2_L          (0x0004u)  /* Comp. E Pos. Channel Input Select 2 */
#define CEIPSEL3_L          (0x0008u)  /* Comp. E Pos. Channel Input Select 3 */
//#define RESERVED            (0x0010u)  /* Comp. E */
//#define RESERVED            (0x0020u)  /* Comp. E */
//#define RESERVED            (0x0040u)  /* Comp. E */
#define CEIPEN_L            (0x0080u)  /* Comp. E Pos. Channel Input Enable */
//#define RESERVED            (0x1000u)  /* Comp. E */
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */

/* CECTL0 Control Bits */
//#define RESERVED            (0x0010u)  /* Comp. E */
//#define RESERVED            (0x0020u)  /* Comp. E */
//#define RESERVED            (0x0040u)  /* Comp. E */
#define CEIMSEL0_H          (0x0001u)  /* Comp. E Neg. Channel Input Select 0 */
#define CEIMSEL1_H          (0x0002u)  /* Comp. E Neg. Channel Input Select 1 */
#define CEIMSEL2_H          (0x0004u)  /* Comp. E Neg. Channel Input Select 2 */
#define CEIMSEL3_H          (0x0008u)  /* Comp. E Neg. Channel Input Select 3 */
//#define RESERVED            (0x1000u)  /* Comp. E */
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
#define CEIMEN_H            (0x0080u)  /* Comp. E Neg. Channel Input Enable */

#define CEIPSEL_0           (0x0000u)  /* Comp. E V+ terminal Input Select: Channel 0 */
#define CEIPSEL_1           (0x0001u)  /* Comp. E V+ terminal Input Select: Channel 1 */
#define CEIPSEL_2           (0x0002u)  /* Comp. E V+ terminal Input Select: Channel 2 */
#define CEIPSEL_3           (0x0003u)  /* Comp. E V+ terminal Input Select: Channel 3 */
#define CEIPSEL_4           (0x0004u)  /* Comp. E V+ terminal Input Select: Channel 4 */
#define CEIPSEL_5           (0x0005u)  /* Comp. E V+ terminal Input Select: Channel 5 */
#define CEIPSEL_6           (0x0006u)  /* Comp. E V+ terminal Input Select: Channel 6 */
#define CEIPSEL_7           (0x0007u)  /* Comp. E V+ terminal Input Select: Channel 7 */
#define CEIPSEL_8           (0x0008u)  /* Comp. E V+ terminal Input Select: Channel 8 */
#define CEIPSEL_9           (0x0009u)  /* Comp. E V+ terminal Input Select: Channel 9 */
#define CEIPSEL_10          (0x000Au)  /* Comp. E V+ terminal Input Select: Channel 10 */
#define CEIPSEL_11          (0x000Bu)  /* Comp. E V+ terminal Input Select: Channel 11 */
#define CEIPSEL_12          (0x000Cu)  /* Comp. E V+ terminal Input Select: Channel 12 */
#define CEIPSEL_13          (0x000Du)  /* Comp. E V+ terminal Input Select: Channel 13 */
#define CEIPSEL_14          (0x000Eu)  /* Comp. E V+ terminal Input Select: Channel 14 */
#define CEIPSEL_15          (0x000Fu)  /* Comp. E V+ terminal Input Select: Channel 15 */

#define CEIMSEL_0           (0x0000u)  /* Comp. E V- Terminal Input Select: Channel 0 */
#define CEIMSEL_1           (0x0100u)  /* Comp. E V- Terminal Input Select: Channel 1 */
#define CEIMSEL_2           (0x0200u)  /* Comp. E V- Terminal Input Select: Channel 2 */
#define CEIMSEL_3           (0x0300u)  /* Comp. E V- Terminal Input Select: Channel 3 */
#define CEIMSEL_4           (0x0400u)  /* Comp. E V- Terminal Input Select: Channel 4 */
#define CEIMSEL_5           (0x0500u)  /* Comp. E V- Terminal Input Select: Channel 5 */
#define CEIMSEL_6           (0x0600u)  /* Comp. E V- Terminal Input Select: Channel 6 */
#define CEIMSEL_7           (0x0700u)  /* Comp. E V- Terminal Input Select: Channel 7 */
#define CEIMSEL_8           (0x0800u)  /* Comp. E V- terminal Input Select: Channel 8 */
#define CEIMSEL_9           (0x0900u)  /* Comp. E V- terminal Input Select: Channel 9 */
#define CEIMSEL_10          (0x0A00u)  /* Comp. E V- terminal Input Select: Channel 10 */
#define CEIMSEL_11          (0x0B00u)  /* Comp. E V- terminal Input Select: Channel 11 */
#define CEIMSEL_12          (0x0C00u)  /* Comp. E V- terminal Input Select: Channel 12 */
#define CEIMSEL_13          (0x0D00u)  /* Comp. E V- terminal Input Select: Channel 13 */
#define CEIMSEL_14          (0x0E00u)  /* Comp. E V- terminal Input Select: Channel 14 */
#define CEIMSEL_15          (0x0F00u)  /* Comp. E V- terminal Input Select: Channel 15 */

/* CECTL1 Control Bits */
#define CEOUT               (0x0001u)  /* Comp. E Output */
#define CEOUTPOL            (0x0002u)  /* Comp. E Output Polarity */
#define CEF                 (0x0004u)  /* Comp. E Enable Output Filter */
#define CEIES               (0x0008u)  /* Comp. E Interrupt Edge Select */
#define CESHORT             (0x0010u)  /* Comp. E Input Short */
#define CEEX                (0x0020u)  /* Comp. E Exchange Inputs */
#define CEFDLY0             (0x0040u)  /* Comp. E Filter delay Bit 0 */
#define CEFDLY1             (0x0080u)  /* Comp. E Filter delay Bit 1 */
#define CEPWRMD0            (0x0100u)  /* Comp. E Power mode Bit 0 */
#define CEPWRMD1            (0x0200u)  /* Comp. E Power mode Bit 1 */
#define CEON                (0x0400u)  /* Comp. E enable */
#define CEMRVL              (0x0800u)  /* Comp. E CEMRV Level */
#define CEMRVS              (0x1000u)  /* Comp. E Output selects between VREF0 or VREF1*/
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
//#define RESERVED            (0x8000u)  /* Comp. E */

/* CECTL1 Control Bits */
#define CEOUT_L             (0x0001u)  /* Comp. E Output */
#define CEOUTPOL_L          (0x0002u)  /* Comp. E Output Polarity */
#define CEF_L               (0x0004u)  /* Comp. E Enable Output Filter */
#define CEIES_L             (0x0008u)  /* Comp. E Interrupt Edge Select */
#define CESHORT_L           (0x0010u)  /* Comp. E Input Short */
#define CEEX_L              (0x0020u)  /* Comp. E Exchange Inputs */
#define CEFDLY0_L           (0x0040u)  /* Comp. E Filter delay Bit 0 */
#define CEFDLY1_L           (0x0080u)  /* Comp. E Filter delay Bit 1 */
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
//#define RESERVED            (0x8000u)  /* Comp. E */

/* CECTL1 Control Bits */
#define CEPWRMD0_H          (0x0001u)  /* Comp. E Power mode Bit 0 */
#define CEPWRMD1_H          (0x0002u)  /* Comp. E Power mode Bit 1 */
#define CEON_H              (0x0004u)  /* Comp. E enable */
#define CEMRVL_H            (0x0008u)  /* Comp. E CEMRV Level */
#define CEMRVS_H            (0x0010u)  /* Comp. E Output selects between VREF0 or VREF1*/
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
//#define RESERVED            (0x8000u)  /* Comp. E */

#define CEPWRMD_0          (0x0000u)  /* Comp. E Power mode 0 */
#define CEPWRMD_1          (0x0100u)  /* Comp. E Power mode 1 */
#define CEPWRMD_2          (0x0200u)  /* Comp. E Power mode 2 */
#define CEPWRMD_3          (0x0300u)  /* Comp. E Power mode 3*/

#define CEFDLY_0           (0x0000u)  /* Comp. E Filter delay 0 : 450ns */
#define CEFDLY_1           (0x0040u)  /* Comp. E Filter delay 1 : 900ns */
#define CEFDLY_2           (0x0080u)  /* Comp. E Filter delay 2 : 1800ns */
#define CEFDLY_3           (0x00C0u)  /* Comp. E Filter delay 3 : 3600ns */

/* CECTL2 Control Bits */
#define CEREF00             (0x0001u)  /* Comp. E Reference 0 Resistor Select Bit : 0 */
#define CEREF01             (0x0002u)  /* Comp. E Reference 0 Resistor Select Bit : 1 */
#define CEREF02             (0x0004u)  /* Comp. E Reference 0 Resistor Select Bit : 2 */
#define CEREF03             (0x0008u)  /* Comp. E Reference 0 Resistor Select Bit : 3 */
#define CEREF04             (0x0010u)  /* Comp. E Reference 0 Resistor Select Bit : 4 */
#define CERSEL              (0x0020u)  /* Comp. E Reference select */
#define CERS0               (0x0040u)  /* Comp. E Reference Source Bit : 0 */
#define CERS1               (0x0080u)  /* Comp. E Reference Source Bit : 1 */
#define CEREF10             (0x0100u)  /* Comp. E Reference 1 Resistor Select Bit : 0 */
#define CEREF11             (0x0200u)  /* Comp. E Reference 1 Resistor Select Bit : 1 */
#define CEREF12             (0x0400u)  /* Comp. E Reference 1 Resistor Select Bit : 2 */
#define CEREF13             (0x0800u)  /* Comp. E Reference 1 Resistor Select Bit : 3 */
#define CEREF14             (0x1000u)  /* Comp. E Reference 1 Resistor Select Bit : 4 */
#define CEREFL0             (0x2000u)  /* Comp. E Reference voltage level Bit : 0 */
#define CEREFL1             (0x4000u)  /* Comp. E Reference voltage level Bit : 1 */
#define CEREFACC            (0x8000u)  /* Comp. E Reference Accuracy */

/* CECTL2 Control Bits */
#define CEREF00_L           (0x0001u)  /* Comp. E Reference 0 Resistor Select Bit : 0 */
#define CEREF01_L           (0x0002u)  /* Comp. E Reference 0 Resistor Select Bit : 1 */
#define CEREF02_L           (0x0004u)  /* Comp. E Reference 0 Resistor Select Bit : 2 */
#define CEREF03_L           (0x0008u)  /* Comp. E Reference 0 Resistor Select Bit : 3 */
#define CEREF04_L           (0x0010u)  /* Comp. E Reference 0 Resistor Select Bit : 4 */
#define CERSEL_L            (0x0020u)  /* Comp. E Reference select */
#define CERS0_L             (0x0040u)  /* Comp. E Reference Source Bit : 0 */
#define CERS1_L             (0x0080u)  /* Comp. E Reference Source Bit : 1 */

/* CECTL2 Control Bits */
#define CEREF10_H           (0x0001u)  /* Comp. E Reference 1 Resistor Select Bit : 0 */
#define CEREF11_H           (0x0002u)  /* Comp. E Reference 1 Resistor Select Bit : 1 */
#define CEREF12_H           (0x0004u)  /* Comp. E Reference 1 Resistor Select Bit : 2 */
#define CEREF13_H           (0x0008u)  /* Comp. E Reference 1 Resistor Select Bit : 3 */
#define CEREF14_H           (0x0010u)  /* Comp. E Reference 1 Resistor Select Bit : 4 */
#define CEREFL0_H           (0x0020u)  /* Comp. E Reference voltage level Bit : 0 */
#define CEREFL1_H           (0x0040u)  /* Comp. E Reference voltage level Bit : 1 */
#define CEREFACC_H          (0x0080u)  /* Comp. E Reference Accuracy */

#define CEREF0_0            (0x0000u)  /* Comp. E Int. Ref.0 Select 0 : 1/32 */
#define CEREF0_1            (0x0001u)  /* Comp. E Int. Ref.0 Select 1 : 2/32 */
#define CEREF0_2            (0x0002u)  /* Comp. E Int. Ref.0 Select 2 : 3/32 */
#define CEREF0_3            (0x0003u)  /* Comp. E Int. Ref.0 Select 3 : 4/32 */
#define CEREF0_4            (0x0004u)  /* Comp. E Int. Ref.0 Select 4 : 5/32 */
#define CEREF0_5            (0x0005u)  /* Comp. E Int. Ref.0 Select 5 : 6/32 */
#define CEREF0_6            (0x0006u)  /* Comp. E Int. Ref.0 Select 6 : 7/32 */
#define CEREF0_7            (0x0007u)  /* Comp. E Int. Ref.0 Select 7 : 8/32 */
#define CEREF0_8            (0x0008u)  /* Comp. E Int. Ref.0 Select 0 : 9/32 */
#define CEREF0_9            (0x0009u)  /* Comp. E Int. Ref.0 Select 1 : 10/32 */
#define CEREF0_10           (0x000Au)  /* Comp. E Int. Ref.0 Select 2 : 11/32 */
#define CEREF0_11           (0x000Bu)  /* Comp. E Int. Ref.0 Select 3 : 12/32 */
#define CEREF0_12           (0x000Cu)  /* Comp. E Int. Ref.0 Select 4 : 13/32 */
#define CEREF0_13           (0x000Du)  /* Comp. E Int. Ref.0 Select 5 : 14/32 */
#define CEREF0_14           (0x000Eu)  /* Comp. E Int. Ref.0 Select 6 : 15/32 */
#define CEREF0_15           (0x000Fu)  /* Comp. E Int. Ref.0 Select 7 : 16/32 */
#define CEREF0_16           (0x0010u)  /* Comp. E Int. Ref.0 Select 0 : 17/32 */
#define CEREF0_17           (0x0011u)  /* Comp. E Int. Ref.0 Select 1 : 18/32 */
#define CEREF0_18           (0x0012u)  /* Comp. E Int. Ref.0 Select 2 : 19/32 */
#define CEREF0_19           (0x0013u)  /* Comp. E Int. Ref.0 Select 3 : 20/32 */
#define CEREF0_20           (0x0014u)  /* Comp. E Int. Ref.0 Select 4 : 21/32 */
#define CEREF0_21           (0x0015u)  /* Comp. E Int. Ref.0 Select 5 : 22/32 */
#define CEREF0_22           (0x0016u)  /* Comp. E Int. Ref.0 Select 6 : 23/32 */
#define CEREF0_23           (0x0017u)  /* Comp. E Int. Ref.0 Select 7 : 24/32 */
#define CEREF0_24           (0x0018u)  /* Comp. E Int. Ref.0 Select 0 : 25/32 */
#define CEREF0_25           (0x0019u)  /* Comp. E Int. Ref.0 Select 1 : 26/32 */
#define CEREF0_26           (0x001Au)  /* Comp. E Int. Ref.0 Select 2 : 27/32 */
#define CEREF0_27           (0x001Bu)  /* Comp. E Int. Ref.0 Select 3 : 28/32 */
#define CEREF0_28           (0x001Cu)  /* Comp. E Int. Ref.0 Select 4 : 29/32 */
#define CEREF0_29           (0x001Du)  /* Comp. E Int. Ref.0 Select 5 : 30/32 */
#define CEREF0_30           (0x001Eu)  /* Comp. E Int. Ref.0 Select 6 : 31/32 */
#define CEREF0_31           (0x001Fu)  /* Comp. E Int. Ref.0 Select 7 : 32/32 */

#define CERS_0              (0x0000u)  /* Comp. E Reference Source 0 : Off */
#define CERS_1              (0x0040u)  /* Comp. E Reference Source 1 : Vcc */
#define CERS_2              (0x0080u)  /* Comp. E Reference Source 2 : Shared Ref. */
#define CERS_3              (0x00C0u)  /* Comp. E Reference Source 3 : Shared Ref. / Off */

#define CEREF1_0            (0x0000u)  /* Comp. E Int. Ref.1 Select 0 : 1/32 */
#define CEREF1_1            (0x0100u)  /* Comp. E Int. Ref.1 Select 1 : 2/32 */
#define CEREF1_2            (0x0200u)  /* Comp. E Int. Ref.1 Select 2 : 3/32 */
#define CEREF1_3            (0x0300u)  /* Comp. E Int. Ref.1 Select 3 : 4/32 */
#define CEREF1_4            (0x0400u)  /* Comp. E Int. Ref.1 Select 4 : 5/32 */
#define CEREF1_5            (0x0500u)  /* Comp. E Int. Ref.1 Select 5 : 6/32 */
#define CEREF1_6            (0x0600u)  /* Comp. E Int. Ref.1 Select 6 : 7/32 */
#define CEREF1_7            (0x0700u)  /* Comp. E Int. Ref.1 Select 7 : 8/32 */
#define CEREF1_8            (0x0800u)  /* Comp. E Int. Ref.1 Select 0 : 9/32 */
#define CEREF1_9            (0x0900u)  /* Comp. E Int. Ref.1 Select 1 : 10/32 */
#define CEREF1_10           (0x0A00u)  /* Comp. E Int. Ref.1 Select 2 : 11/32 */
#define CEREF1_11           (0x0B00u)  /* Comp. E Int. Ref.1 Select 3 : 12/32 */
#define CEREF1_12           (0x0C00u)  /* Comp. E Int. Ref.1 Select 4 : 13/32 */
#define CEREF1_13           (0x0D00u)  /* Comp. E Int. Ref.1 Select 5 : 14/32 */
#define CEREF1_14           (0x0E00u)  /* Comp. E Int. Ref.1 Select 6 : 15/32 */
#define CEREF1_15           (0x0F00u)  /* Comp. E Int. Ref.1 Select 7 : 16/32 */
#define CEREF1_16           (0x1000u)  /* Comp. E Int. Ref.1 Select 0 : 17/32 */
#define CEREF1_17           (0x1100u)  /* Comp. E Int. Ref.1 Select 1 : 18/32 */
#define CEREF1_18           (0x1200u)  /* Comp. E Int. Ref.1 Select 2 : 19/32 */
#define CEREF1_19           (0x1300u)  /* Comp. E Int. Ref.1 Select 3 : 20/32 */
#define CEREF1_20           (0x1400u)  /* Comp. E Int. Ref.1 Select 4 : 21/32 */
#define CEREF1_21           (0x1500u)  /* Comp. E Int. Ref.1 Select 5 : 22/32 */
#define CEREF1_22           (0x1600u)  /* Comp. E Int. Ref.1 Select 6 : 23/32 */
#define CEREF1_23           (0x1700u)  /* Comp. E Int. Ref.1 Select 7 : 24/32 */
#define CEREF1_24           (0x1800u)  /* Comp. E Int. Ref.1 Select 0 : 25/32 */
#define CEREF1_25           (0x1900u)  /* Comp. E Int. Ref.1 Select 1 : 26/32 */
#define CEREF1_26           (0x1A00u)  /* Comp. E Int. Ref.1 Select 2 : 27/32 */
#define CEREF1_27           (0x1B00u)  /* Comp. E Int. Ref.1 Select 3 : 28/32 */
#define CEREF1_28           (0x1C00u)  /* Comp. E Int. Ref.1 Select 4 : 29/32 */
#define CEREF1_29           (0x1D00u)  /* Comp. E Int. Ref.1 Select 5 : 30/32 */
#define CEREF1_30           (0x1E00u)  /* Comp. E Int. Ref.1 Select 6 : 31/32 */
#define CEREF1_31           (0x1F00u)  /* Comp. E Int. Ref.1 Select 7 : 32/32 */

#define CEREFL_0            (0x0000u)  /* Comp. E Reference voltage level 0 : None */
#define CEREFL_1            (0x2000u)  /* Comp. E Reference voltage level 1 : 1.2V */
#define CEREFL_2            (0x4000u)  /* Comp. E Reference voltage level 2 : 2.0V  */
#define CEREFL_3            (0x6000u)  /* Comp. E Reference voltage level 3 : 2.5V  */

#define CEPD0               (0x0001u)  /* Comp. E Disable Input Buffer of Port Register .0 */
#define CEPD1               (0x0002u)  /* Comp. E Disable Input Buffer of Port Register .1 */
#define CEPD2               (0x0004u)  /* Comp. E Disable Input Buffer of Port Register .2 */
#define CEPD3               (0x0008u)  /* Comp. E Disable Input Buffer of Port Register .3 */
#define CEPD4               (0x0010u)  /* Comp. E Disable Input Buffer of Port Register .4 */
#define CEPD5               (0x0020u)  /* Comp. E Disable Input Buffer of Port Register .5 */
#define CEPD6               (0x0040u)  /* Comp. E Disable Input Buffer of Port Register .6 */
#define CEPD7               (0x0080u)  /* Comp. E Disable Input Buffer of Port Register .7 */
#define CEPD8               (0x0100u)  /* Comp. E Disable Input Buffer of Port Register .8 */
#define CEPD9               (0x0200u)  /* Comp. E Disable Input Buffer of Port Register .9 */
#define CEPD10              (0x0400u)  /* Comp. E Disable Input Buffer of Port Register .10 */
#define CEPD11              (0x0800u)  /* Comp. E Disable Input Buffer of Port Register .11 */
#define CEPD12              (0x1000u)  /* Comp. E Disable Input Buffer of Port Register .12 */
#define CEPD13              (0x2000u)  /* Comp. E Disable Input Buffer of Port Register .13 */
#define CEPD14              (0x4000u)  /* Comp. E Disable Input Buffer of Port Register .14 */
#define CEPD15              (0x8000u)  /* Comp. E Disable Input Buffer of Port Register .15 */

#define CEPD0_L             (0x0001u)  /* Comp. E Disable Input Buffer of Port Register .0 */
#define CEPD1_L             (0x0002u)  /* Comp. E Disable Input Buffer of Port Register .1 */
#define CEPD2_L             (0x0004u)  /* Comp. E Disable Input Buffer of Port Register .2 */
#define CEPD3_L             (0x0008u)  /* Comp. E Disable Input Buffer of Port Register .3 */
#define CEPD4_L             (0x0010u)  /* Comp. E Disable Input Buffer of Port Register .4 */
#define CEPD5_L             (0x0020u)  /* Comp. E Disable Input Buffer of Port Register .5 */
#define CEPD6_L             (0x0040u)  /* Comp. E Disable Input Buffer of Port Register .6 */
#define CEPD7_L             (0x0080u)  /* Comp. E Disable Input Buffer of Port Register .7 */

#define CEPD8_H             (0x0001u)  /* Comp. E Disable Input Buffer of Port Register .8 */
#define CEPD9_H             (0x0002u)  /* Comp. E Disable Input Buffer of Port Register .9 */
#define CEPD10_H            (0x0004u)  /* Comp. E Disable Input Buffer of Port Register .10 */
#define CEPD11_H            (0x0008u)  /* Comp. E Disable Input Buffer of Port Register .11 */
#define CEPD12_H            (0x0010u)  /* Comp. E Disable Input Buffer of Port Register .12 */
#define CEPD13_H            (0x0020u)  /* Comp. E Disable Input Buffer of Port Register .13 */
#define CEPD14_H            (0x0040u)  /* Comp. E Disable Input Buffer of Port Register .14 */
#define CEPD15_H            (0x0080u)  /* Comp. E Disable Input Buffer of Port Register .15 */

/* CEINT Control Bits */
#define CEIFG                (0x0001u)  /* Comp. E Interrupt Flag */
#define CEIIFG               (0x0002u)  /* Comp. E Interrupt Flag Inverted Polarity */
//#define RESERVED             (0x0004u)  /* Comp. E */
//#define RESERVED             (0x0008u)  /* Comp. E */
#define CERDYIFG             (0x0010u)  /* Comp. E Comparator_E ready interrupt flag */
//#define RESERVED             (0x0020u)  /* Comp. E */
//#define RESERVED             (0x0040u)  /* Comp. E */
//#define RESERVED             (0x0080u)  /* Comp. E */
#define CEIE                 (0x0100u)  /* Comp. E Interrupt Enable */
#define CEIIE                (0x0200u)  /* Comp. E Interrupt Enable Inverted Polarity */
//#define RESERVED             (0x0400u)  /* Comp. E */
//#define RESERVED             (0x0800u)  /* Comp. E */
#define CERDYIE              (0x1000u)  /* Comp. E Comparator_E ready interrupt enable */
//#define RESERVED             (0x2000u)  /* Comp. E */
//#define RESERVED             (0x4000u)  /* Comp. E */
//#define RESERVED             (0x8000u)  /* Comp. E */

/* CEINT Control Bits */
#define CEIFG_L             (0x0001u)  /* Comp. E Interrupt Flag */
#define CEIIFG_L            (0x0002u)  /* Comp. E Interrupt Flag Inverted Polarity */
//#define RESERVED             (0x0004u)  /* Comp. E */
//#define RESERVED             (0x0008u)  /* Comp. E */
#define CERDYIFG_L          (0x0010u)  /* Comp. E Comparator_E ready interrupt flag */
//#define RESERVED             (0x0020u)  /* Comp. E */
//#define RESERVED             (0x0040u)  /* Comp. E */
//#define RESERVED             (0x0080u)  /* Comp. E */
//#define RESERVED             (0x0400u)  /* Comp. E */
//#define RESERVED             (0x0800u)  /* Comp. E */
//#define RESERVED             (0x2000u)  /* Comp. E */
//#define RESERVED             (0x4000u)  /* Comp. E */
//#define RESERVED             (0x8000u)  /* Comp. E */

/* CEINT Control Bits */
//#define RESERVED             (0x0004u)  /* Comp. E */
//#define RESERVED             (0x0008u)  /* Comp. E */
//#define RESERVED             (0x0020u)  /* Comp. E */
//#define RESERVED             (0x0040u)  /* Comp. E */
//#define RESERVED             (0x0080u)  /* Comp. E */
#define CEIE_H              (0x0001u)  /* Comp. E Interrupt Enable */
#define CEIIE_H             (0x0002u)  /* Comp. E Interrupt Enable Inverted Polarity */
//#define RESERVED             (0x0400u)  /* Comp. E */
//#define RESERVED             (0x0800u)  /* Comp. E */
#define CERDYIE_H           (0x0010u)  /* Comp. E Comparator_E ready interrupt enable */
//#define RESERVED             (0x2000u)  /* Comp. E */
//#define RESERVED             (0x4000u)  /* Comp. E */
//#define RESERVED             (0x8000u)  /* Comp. E */

/* CEIV Definitions */
#define CEIV_NONE           (0x0000u)    /* No Interrupt pending */
#define CEIV_CEIFG          (0x0002u)    /* CEIFG */
#define CEIV_CEIIFG         (0x0004u)    /* CEIIFG */
#define CEIV_CERDYIFG       (0x000Au)    /* CERDYIFG */

/*************************************************************
* CRC Module
*************************************************************/
#define __MSP430_HAS_CRC__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CRC__ 0x0150
#define CRC_BASE __MSP430_BASEADDRESS_CRC__

#define  CRCDI_                 (0x0150u)  /* CRC Data In Register */
DEFCW(   CRCDI                , CRCDI_)
#define  CRCDIRB_               (0x0152u)  /* CRC data in reverse byte Register */
DEFCW(   CRCDIRB              , CRCDIRB_)
#define  CRCINIRES_             (0x0154u)  /* CRC Initialisation Register and Result Register */
DEFCW(   CRCINIRES            , CRCINIRES_)
#define  CRCRESR_               (0x0156u)  /* CRC reverse result Register */
DEFCW(   CRCRESR              , CRCRESR_)

/*************************************************************
* CRC Module
*************************************************************/
#define __MSP430_HAS_CRC32__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CRC32__ 0x0980
#define CRC32_BASE __MSP430_BASEADDRESS_CRC32__


//sfrl    CRC32DIL0              (0x0980)  /* CRC32 Data In */
#define CRC32DIW0_             (0x0980u)  /* CRC32 Data In */
DEFCW(  CRC32DIW0            , CRC32DIW0_)
#define CRC32DIW1_             (0x0982u)  /* CRC32 Data In */
DEFCW(  CRC32DIW1            , CRC32DIW1_)
#define CRC32DIB0           CRC32DIW0_L

//sfrl    CRC32DIRBL0            (0x0984)  /* CRC32 Data In Reversed Bit */
#define CRC32DIRBW1_           (0x0984u)  /* CRC32 Data In Reversed Bit */
DEFCW(  CRC32DIRBW1          , CRC32DIRBW1_)
#define CRC32DIRBW0_           (0x0986u)  /* CRC32 Data In Reversed Bit */
DEFCW(  CRC32DIRBW0          , CRC32DIRBW0_)
#define CRC32DIRBB0         CRC32DIRBW0_H

//sfrl    CRC32INIRESL0          (0x0988)  /* CRC32 Initialization and Result */
#define CRC32INIRESW0_         (0x0988u)  /* CRC32 Initialization and Result */
DEFCW(  CRC32INIRESW0        , CRC32INIRESW0_)
#define CRC32INIRESW1_         (0x098Au)  /* CRC32 Initialization and Result */
DEFCW(  CRC32INIRESW1        , CRC32INIRESW1_)
#define CRC32RESB0          CRC32INIRESW0_L
#define CRC32RESB1          CRC32INIRESW0_H
#define CRC32RESB2          CRC32INIRESW1_L
#define CRC32RESB3          CRC32INIRESW1_H

//sfrl    CRC32RESRL0            (0x098C)  /* CRC32 Result Reverse */
#define CRC32RESRW1_           (0x098Cu)  /* CRC32 Result Reverse */
DEFCW(  CRC32RESRW1          , CRC32RESRW1_)
#define CRC32RESRW0_           (0x098Eu)  /* CRC32 Result Reverse */
DEFCW(  CRC32RESRW0          , CRC32RESRW0_)
#define CRC32RESRB3         CRC32RESRW1_L
#define CRC32RESRB2         CRC32RESRW1_H
#define CRC32RESRB1         CRC32RESRW0_L
#define CRC32RESRB0         CRC32RESRW0_H

//sfrl    CRC16DIL0              (0x0990)  /* CRC16 Data Input */
#define CRC16DIW0_             (0x0990u)  /* CRC16 Data Input */
DEFCW(  CRC16DIW0            , CRC16DIW0_)
#define CRC16DIW1_             (0x0992u)  /* CRC16 Data Input */
DEFCW(  CRC16DIW1            , CRC16DIW1_)
#define CRC16DIB0           CRC16DIW0_L
//sfrl    CRC16DIRBL0            (0x0994)  /* CRC16 Data In Reverse */
#define CRC16DIRBW1_           (0x0994u)  /* CRC16 Data In Reverse */
DEFCW(  CRC16DIRBW1          , CRC16DIRBW1_)
#define CRC16DIRBW0_           (0x0996u)  /* CRC16 Data In Reverse */
DEFCW(  CRC16DIRBW0          , CRC16DIRBW0_)
#define CRC16DIRBB0         CRC16DIRBW0_L

//sfrl    CRC16INIRESL0          (0x0998)  /* CRC16 Init and Result */
#define CRC16INIRESW0_         (0x0998u)  /* CRC16 Init and Result */
DEFCW(  CRC16INIRESW0        , CRC16INIRESW0_)
#define CRC16INIRESB1       CRC16INIRESW0_H
#define CRC16INIRESB0       CRC16INIRESW0_L

//sfrl    CRC16RESRL0            (0x099E)  /* CRC16 Result Reverse */
#define CRC16RESRW0_           (0x099Eu)  /* CRC16 Result Reverse */
DEFCW(  CRC16RESRW0          , CRC16RESRW0_)
#define CRC16RESRW1_           (0x099Cu)  /* CRC16 Result Reverse */
DEFCW(  CRC16RESRW1          , CRC16RESRW1_)
#define CRC16RESRB1         CRC16RESRW0_L
#define CRC16RESRB0         CRC16RESRW0_H

/************************************************************
* CLOCK SYSTEM
************************************************************/
#define __MSP430_HAS_CS__              /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CS__ 0x0160
#define CS_BASE __MSP430_BASEADDRESS_CS__

#define  CSCTL0_                (0x0160u)  /* CS Control Register 0 */
DEFCW(   CSCTL0               , CSCTL0_)
#define  CSCTL1_                (0x0162u)  /* CS Control Register 1 */
DEFCW(   CSCTL1               , CSCTL1_)
#define  CSCTL2_                (0x0164u)  /* CS Control Register 2 */
DEFCW(   CSCTL2               , CSCTL2_)
#define  CSCTL3_                (0x0166u)  /* CS Control Register 3 */
DEFCW(   CSCTL3               , CSCTL3_)
#define  CSCTL4_                (0x0168u)  /* CS Control Register 4 */
DEFCW(   CSCTL4               , CSCTL4_)
#define  CSCTL5_                (0x016Au)  /* CS Control Register 5 */
DEFCW(   CSCTL5               , CSCTL5_)
#define  CSCTL6_                (0x016Cu)  /* CS Control Register 6 */
DEFCW(   CSCTL6               , CSCTL6_)

/* CSCTL0 Control Bits */

#define CSKEY                (0xA500u)    /* CS Password */
#define CSKEY_H              (0xA5)      /* CS Password for high byte access */

/* CSCTL1 Control Bits */
#define DCOFSEL0            (0x0002u)    /* DCO frequency select Bit: 0 */
#define DCOFSEL1            (0x0004u)    /* DCO frequency select Bit: 1 */
#define DCOFSEL2            (0x0008u)    /* DCO frequency select Bit: 2 */
#define DCORSEL             (0x0040u)    /* DCO range select. */

/* CSCTL1 Control Bits */
#define DCOFSEL0_L          (0x0002u)    /* DCO frequency select Bit: 0 */
#define DCOFSEL1_L          (0x0004u)    /* DCO frequency select Bit: 1 */
#define DCOFSEL2_L          (0x0008u)    /* DCO frequency select Bit: 2 */
#define DCORSEL_L           (0x0040u)    /* DCO range select. */

#define DCOFSEL_0           (0x0000u)    /* DCO frequency select: 0 */
#define DCOFSEL_1           (0x0002u)    /* DCO frequency select: 1 */
#define DCOFSEL_2           (0x0004u)    /* DCO frequency select: 2 */
#define DCOFSEL_3           (0x0006u)    /* DCO frequency select: 3 */
#define DCOFSEL_4           (0x0008u)    /* DCO frequency select: 4 */
#define DCOFSEL_5           (0x000Au)    /* DCO frequency select: 5 */
#define DCOFSEL_6           (0x000Cu)    /* DCO frequency select: 6 */
#define DCOFSEL_7           (0x000Eu)    /* DCO frequency select: 7 */

/* CSCTL2 Control Bits */
#define SELM0               (0x0001u)   /* MCLK Source Select Bit: 0 */
#define SELM1               (0x0002u)   /* MCLK Source Select Bit: 1 */
#define SELM2               (0x0004u)   /* MCLK Source Select Bit: 2 */
//#define RESERVED            (0x0004u)    /* RESERVED */
//#define RESERVED            (0x0008u)    /* RESERVED */
#define SELS0               (0x0010u)   /* SMCLK Source Select Bit: 0 */
#define SELS1               (0x0020u)   /* SMCLK Source Select Bit: 1 */
#define SELS2               (0x0040u)   /* SMCLK Source Select Bit: 2 */
//#define RESERVED            (0x0040u)    /* RESERVED */
//#define RESERVED            (0x0080u)    /* RESERVED */
#define SELA0               (0x0100u)   /* ACLK Source Select Bit: 0 */
#define SELA1               (0x0200u)   /* ACLK Source Select Bit: 1 */
#define SELA2               (0x0400u)   /* ACLK Source Select Bit: 2 */
//#define RESERVED            (0x0400u)    /* RESERVED */
//#define RESERVED            (0x0800u)    /* RESERVED */
//#define RESERVED            (0x1000u)    /* RESERVED */
//#define RESERVED            (0x2000u)    /* RESERVED */
//#define RESERVED            (0x4000u)    /* RESERVED */
//#define RESERVED            (0x8000u)    /* RESERVED */

/* CSCTL2 Control Bits */
#define SELM0_L             (0x0001u)   /* MCLK Source Select Bit: 0 */
#define SELM1_L             (0x0002u)   /* MCLK Source Select Bit: 1 */
#define SELM2_L             (0x0004u)   /* MCLK Source Select Bit: 2 */
//#define RESERVED            (0x0004u)    /* RESERVED */
//#define RESERVED            (0x0008u)    /* RESERVED */
#define SELS0_L             (0x0010u)   /* SMCLK Source Select Bit: 0 */
#define SELS1_L             (0x0020u)   /* SMCLK Source Select Bit: 1 */
#define SELS2_L             (0x0040u)   /* SMCLK Source Select Bit: 2 */
//#define RESERVED            (0x0040u)    /* RESERVED */
//#define RESERVED            (0x0080u)    /* RESERVED */
//#define RESERVED            (0x0400u)    /* RESERVED */
//#define RESERVED            (0x0800u)    /* RESERVED */
//#define RESERVED            (0x1000u)    /* RESERVED */
//#define RESERVED            (0x2000u)    /* RESERVED */
//#define RESERVED            (0x4000u)    /* RESERVED */
//#define RESERVED            (0x8000u)    /* RESERVED */

/* CSCTL2 Control Bits */
//#define RESERVED            (0x0004u)    /* RESERVED */
//#define RESERVED            (0x0008u)    /* RESERVED */
//#define RESERVED            (0x0040u)    /* RESERVED */
//#define RESERVED            (0x0080u)    /* RESERVED */
#define SELA0_H             (0x0001u)   /* ACLK Source Select Bit: 0 */
#define SELA1_H             (0x0002u)   /* ACLK Source Select Bit: 1 */
#define SELA2_H             (0x0004u)   /* ACLK Source Select Bit: 2 */
//#define RESERVED            (0x0400u)    /* RESERVED */
//#define RESERVED            (0x0800u)    /* RESERVED */
//#define RESERVED            (0x1000u)    /* RESERVED */
//#define RESERVED            (0x2000u)    /* RESERVED */
//#define RESERVED            (0x4000u)    /* RESERVED */
//#define RESERVED            (0x8000u)    /* RESERVED */

#define SELM_0              (0x0000u)   /* MCLK Source Select 0 */
#define SELM_1              (0x0001u)   /* MCLK Source Select 1 */
#define SELM_2              (0x0002u)   /* MCLK Source Select 2 */
#define SELM_3              (0x0003u)   /* MCLK Source Select 3 */
#define SELM_4              (0x0004u)   /* MCLK Source Select 4 */
#define SELM_5              (0x0005u)   /* MCLK Source Select 5 */
#define SELM_6              (0x0006u)   /* MCLK Source Select 6 */
#define SELM_7              (0x0007u)   /* MCLK Source Select 7 */
#define SELM__LFXTCLK       (0x0000u)   /* MCLK Source Select LFXTCLK */
#define SELM__VLOCLK        (0x0001u)   /* MCLK Source Select VLOCLK */
#define SELM__LFMODCLK      (0x0002u)   /* MCLK Source Select LFMODOSC */
#define SELM__LFMODOSC      (0x0002u)   /* MCLK Source Select LFMODOSC (legacy) */
#define SELM__DCOCLK        (0x0003u)   /* MCLK Source Select DCOCLK */
#define SELM__MODCLK        (0x0004u)   /* MCLK Source Select MODOSC */
#define SELM__MODOSC        (0x0004u)   /* MCLK Source Select MODOSC (legacy) */
#define SELM__HFXTCLK       (0x0005u)   /* MCLK Source Select HFXTCLK */

#define SELS_0              (0x0000u)   /* SMCLK Source Select 0 */
#define SELS_1              (0x0010u)   /* SMCLK Source Select 1 */
#define SELS_2              (0x0020u)   /* SMCLK Source Select 2 */
#define SELS_3              (0x0030u)   /* SMCLK Source Select 3 */
#define SELS_4              (0x0040u)   /* SMCLK Source Select 4 */
#define SELS_5              (0x0050u)   /* SMCLK Source Select 5 */
#define SELS_6              (0x0060u)   /* SMCLK Source Select 6 */
#define SELS_7              (0x0070u)   /* SMCLK Source Select 7 */
#define SELS__LFXTCLK       (0x0000u)   /* SMCLK Source Select LFXTCLK */
#define SELS__VLOCLK        (0x0010u)   /* SMCLK Source Select VLOCLK */
#define SELS__LFMODCLK      (0x0020u)   /* SMCLK Source Select LFMODOSC */
#define SELS__LFMODOSC      (0x0020u)   /* SMCLK Source Select LFMODOSC (legacy) */
#define SELS__DCOCLK        (0x0030u)   /* SMCLK Source Select DCOCLK */
#define SELS__MODCLK        (0x0040u)   /* SMCLK Source Select MODOSC */
#define SELS__MODOSC        (0x0040u)   /* SMCLK Source Select MODOSC (legacy) */
#define SELS__HFXTCLK       (0x0050u)   /* SMCLK Source Select HFXTCLK */

#define SELA_0              (0x0000u)   /* ACLK Source Select 0 */
#define SELA_1              (0x0100u)   /* ACLK Source Select 1 */
#define SELA_2              (0x0200u)   /* ACLK Source Select 2 */
#define SELA_3              (0x0300u)   /* ACLK Source Select 3 */
#define SELA_4              (0x0400u)   /* ACLK Source Select 4 */
#define SELA_5              (0x0500u)   /* ACLK Source Select 5 */
#define SELA_6              (0x0600u)   /* ACLK Source Select 6 */
#define SELA_7              (0x0700u)   /* ACLK Source Select 7 */
#define SELA__LFXTCLK       (0x0000u)   /* ACLK Source Select LFXTCLK */
#define SELA__VLOCLK        (0x0100u)   /* ACLK Source Select VLOCLK */
#define SELA__LFMODCLK      (0x0200u)   /* ACLK Source Select LFMODOSC */
#define SELA__LFMODOSC      (0x0200u)   /* ACLK Source Select LFMODOSC (legacy) */

/* CSCTL3 Control Bits */
#define DIVM0               (0x0001u)   /* MCLK Divider Bit: 0 */
#define DIVM1               (0x0002u)   /* MCLK Divider Bit: 1 */
#define DIVM2               (0x0004u)   /* MCLK Divider Bit: 2 */
//#define RESERVED            (0x0004u)    /* RESERVED */
//#define RESERVED            (0x0008u)    /* RESERVED */
#define DIVS0               (0x0010u)   /* SMCLK Divider Bit: 0 */
#define DIVS1               (0x0020u)   /* SMCLK Divider Bit: 1 */
#define DIVS2               (0x0040u)   /* SMCLK Divider Bit: 2 */
//#define RESERVED            (0x0040u)    /* RESERVED */
//#define RESERVED            (0x0080u)    /* RESERVED */
#define DIVA0               (0x0100u)   /* ACLK Divider Bit: 0 */
#define DIVA1               (0x0200u)   /* ACLK Divider Bit: 1 */
#define DIVA2               (0x0400u)   /* ACLK Divider Bit: 2 */
//#define RESERVED            (0x0400u)    /* RESERVED */
//#define RESERVED            (0x0800u)    /* RESERVED */
//#define RESERVED            (0x1000u)    /* RESERVED */
//#define RESERVED            (0x2000u)    /* RESERVED */
//#define RESERVED            (0x4000u)    /* RESERVED */
//#define RESERVED            (0x8000u)    /* RESERVED */

/* CSCTL3 Control Bits */
#define DIVM0_L             (0x0001u)   /* MCLK Divider Bit: 0 */
#define DIVM1_L             (0x0002u)   /* MCLK Divider Bit: 1 */
#define DIVM2_L             (0x0004u)   /* MCLK Divider Bit: 2 */
//#define RESERVED            (0x0004u)    /* RESERVED */
//#define RESERVED            (0x0008u)    /* RESERVED */
#define DIVS0_L             (0x0010u)   /* SMCLK Divider Bit: 0 */
#define DIVS1_L             (0x0020u)   /* SMCLK Divider Bit: 1 */
#define DIVS2_L             (0x0040u)   /* SMCLK Divider Bit: 2 */
//#define RESERVED            (0x0040u)    /* RESERVED */
//#define RESERVED            (0x0080u)    /* RESERVED */
//#define RESERVED            (0x0400u)    /* RESERVED */
//#define RESERVED            (0x0800u)    /* RESERVED */
//#define RESERVED            (0x1000u)    /* RESERVED */
//#define RESERVED            (0x2000u)    /* RESERVED */
//#define RESERVED            (0x4000u)    /* RESERVED */
//#define RESERVED            (0x8000u)    /* RESERVED */

/* CSCTL3 Control Bits */
//#define RESERVED            (0x0004u)    /* RESERVED */
//#define RESERVED            (0x0008u)    /* RESERVED */
//#define RESERVED            (0x0040u)    /* RESERVED */
//#define RESERVED            (0x0080u)    /* RESERVED */
#define DIVA0_H             (0x0001u)   /* ACLK Divider Bit: 0 */
#define DIVA1_H             (0x0002u)   /* ACLK Divider Bit: 1 */
#define DIVA2_H             (0x0004u)   /* ACLK Divider Bit: 2 */
//#define RESERVED            (0x0400u)    /* RESERVED */
//#define RESERVED            (0x0800u)    /* RESERVED */
//#define RESERVED            (0x1000u)    /* RESERVED */
//#define RESERVED            (0x2000u)    /* RESERVED */
//#define RESERVED            (0x4000u)    /* RESERVED */
//#define RESERVED            (0x8000u)    /* RESERVED */

#define DIVM_0              (0x0000u)    /* MCLK Source Divider 0 */
#define DIVM_1              (0x0001u)    /* MCLK Source Divider 1 */
#define DIVM_2              (0x0002u)    /* MCLK Source Divider 2 */
#define DIVM_3              (0x0003u)    /* MCLK Source Divider 3 */
#define DIVM_4              (0x0004u)    /* MCLK Source Divider 4 */
#define DIVM_5              (0x0005u)    /* MCLK Source Divider 5 */
#define DIVM__1             (0x0000u)    /* MCLK Source Divider f(MCLK)/1 */
#define DIVM__2             (0x0001u)    /* MCLK Source Divider f(MCLK)/2 */
#define DIVM__4             (0x0002u)    /* MCLK Source Divider f(MCLK)/4 */
#define DIVM__8             (0x0003u)    /* MCLK Source Divider f(MCLK)/8 */
#define DIVM__16            (0x0004u)    /* MCLK Source Divider f(MCLK)/16 */
#define DIVM__32            (0x0005u)    /* MCLK Source Divider f(MCLK)/32 */

#define DIVS_0              (0x0000u)    /* SMCLK Source Divider 0 */
#define DIVS_1              (0x0010u)    /* SMCLK Source Divider 1 */
#define DIVS_2              (0x0020u)    /* SMCLK Source Divider 2 */
#define DIVS_3              (0x0030u)    /* SMCLK Source Divider 3 */
#define DIVS_4              (0x0040u)    /* SMCLK Source Divider 4 */
#define DIVS_5              (0x0050u)    /* SMCLK Source Divider 5 */
#define DIVS__1             (0x0000u)    /* SMCLK Source Divider f(SMCLK)/1 */
#define DIVS__2             (0x0010u)    /* SMCLK Source Divider f(SMCLK)/2 */
#define DIVS__4             (0x0020u)    /* SMCLK Source Divider f(SMCLK)/4 */
#define DIVS__8             (0x0030u)    /* SMCLK Source Divider f(SMCLK)/8 */
#define DIVS__16            (0x0040u)    /* SMCLK Source Divider f(SMCLK)/16 */
#define DIVS__32            (0x0050u)    /* SMCLK Source Divider f(SMCLK)/32 */

#define DIVA_0              (0x0000u)    /* ACLK Source Divider 0 */
#define DIVA_1              (0x0100u)    /* ACLK Source Divider 1 */
#define DIVA_2              (0x0200u)    /* ACLK Source Divider 2 */
#define DIVA_3              (0x0300u)    /* ACLK Source Divider 3 */
#define DIVA_4              (0x0400u)    /* ACLK Source Divider 4 */
#define DIVA_5              (0x0500u)    /* ACLK Source Divider 5 */
#define DIVA__1             (0x0000u)    /* ACLK Source Divider f(ACLK)/1 */
#define DIVA__2             (0x0100u)    /* ACLK Source Divider f(ACLK)/2 */
#define DIVA__4             (0x0200u)    /* ACLK Source Divider f(ACLK)/4 */
#define DIVA__8             (0x0300u)    /* ACLK Source Divider f(ACLK)/8 */
#define DIVA__16            (0x0400u)    /* ACLK Source Divider f(ACLK)/16 */
#define DIVA__32            (0x0500u)    /* ACLK Source Divider f(ACLK)/32 */

/* CSCTL4 Control Bits */
#define LFXTOFF             (0x0001u)    /* High Frequency Oscillator 1 (XT1) disable */
#define SMCLKOFF            (0x0002u)    /* SMCLK Off */
#define VLOOFF              (0x0008u)    /* VLO Off */
#define LFXTBYPASS          (0x0010u)    /* LFXT bypass mode : 0: internal 1:sourced from external pin */
#define LFXTDRIVE0          (0x0040u)    /* LFXT Drive Level mode Bit 0 */
#define LFXTDRIVE1          (0x0080u)    /* LFXT Drive Level mode Bit 1 */
#define HFXTOFF             (0x0100u)    /* High Frequency Oscillator disable */
#define HFFREQ0             (0x0400u)    /* HFXT frequency selection Bit 1 */
#define HFFREQ1             (0x0800u)    /* HFXT frequency selection Bit 0 */
#define HFXTBYPASS          (0x1000u)    /* HFXT bypass mode : 0: internal 1:sourced from external pin */
#define HFXTDRIVE0          (0x4000u)    /* HFXT Drive Level mode Bit 0 */
#define HFXTDRIVE1          (0x8000u)    /* HFXT Drive Level mode Bit 1 */

/* CSCTL4 Control Bits */
#define LFXTOFF_L           (0x0001u)    /* High Frequency Oscillator 1 (XT1) disable */
#define SMCLKOFF_L          (0x0002u)    /* SMCLK Off */
#define VLOOFF_L            (0x0008u)    /* VLO Off */
#define LFXTBYPASS_L        (0x0010u)    /* LFXT bypass mode : 0: internal 1:sourced from external pin */
#define LFXTDRIVE0_L        (0x0040u)    /* LFXT Drive Level mode Bit 0 */
#define LFXTDRIVE1_L        (0x0080u)    /* LFXT Drive Level mode Bit 1 */

/* CSCTL4 Control Bits */
#define HFXTOFF_H           (0x0001u)    /* High Frequency Oscillator disable */
#define HFFREQ0_H           (0x0004u)    /* HFXT frequency selection Bit 1 */
#define HFFREQ1_H           (0x0008u)    /* HFXT frequency selection Bit 0 */
#define HFXTBYPASS_H        (0x0010u)    /* HFXT bypass mode : 0: internal 1:sourced from external pin */
#define HFXTDRIVE0_H        (0x0040u)    /* HFXT Drive Level mode Bit 0 */
#define HFXTDRIVE1_H        (0x0080u)    /* HFXT Drive Level mode Bit 1 */

#define LFXTDRIVE_0         (0x0000u)    /* LFXT Drive Level mode: 0 */
#define LFXTDRIVE_1         (0x0040u)    /* LFXT Drive Level mode: 1 */
#define LFXTDRIVE_2         (0x0080u)    /* LFXT Drive Level mode: 2 */
#define LFXTDRIVE_3         (0x00C0u)    /* LFXT Drive Level mode: 3 */

#define HFFREQ_0            (0x0000u)    /* HFXT frequency selection: 0 */
#define HFFREQ_1            (0x0400u)    /* HFXT frequency selection: 1 */
#define HFFREQ_2            (0x0800u)    /* HFXT frequency selection: 2 */
#define HFFREQ_3            (0x0C00u)    /* HFXT frequency selection: 3 */

#define HFXTDRIVE_0         (0x0000u)    /* HFXT Drive Level mode: 0 */
#define HFXTDRIVE_1         (0x4000u)    /* HFXT Drive Level mode: 1 */
#define HFXTDRIVE_2         (0x8000u)    /* HFXT Drive Level mode: 2 */
#define HFXTDRIVE_3         (0xC000u)    /* HFXT Drive Level mode: 3 */

/* CSCTL5 Control Bits */
#define LFXTOFFG            (0x0001u)    /* LFXT Low Frequency Oscillator Fault Flag */
#define HFXTOFFG            (0x0002u)    /* HFXT High Frequency Oscillator Fault Flag */
#define ENSTFCNT1           (0x0040u)    /* Enable start counter for XT1 */
#define ENSTFCNT2           (0x0080u)    /* Enable start counter for XT2 */

/* CSCTL5 Control Bits */
#define LFXTOFFG_L          (0x0001u)    /* LFXT Low Frequency Oscillator Fault Flag */
#define HFXTOFFG_L          (0x0002u)    /* HFXT High Frequency Oscillator Fault Flag */
#define ENSTFCNT1_L         (0x0040u)    /* Enable start counter for XT1 */
#define ENSTFCNT2_L         (0x0080u)    /* Enable start counter for XT2 */

/* CSCTL6 Control Bits */
#define ACLKREQEN           (0x0001u)    /* ACLK Clock Request Enable */
#define MCLKREQEN           (0x0002u)    /* MCLK Clock Request Enable */
#define SMCLKREQEN          (0x0004u)    /* SMCLK Clock Request Enable */
#define MODCLKREQEN         (0x0008u)    /* MODOSC Clock Request Enable */

/* CSCTL6 Control Bits */
#define ACLKREQEN_L         (0x0001u)    /* ACLK Clock Request Enable */
#define MCLKREQEN_L         (0x0002u)    /* MCLK Clock Request Enable */
#define SMCLKREQEN_L        (0x0004u)    /* SMCLK Clock Request Enable */
#define MODCLKREQEN_L       (0x0008u)    /* MODOSC Clock Request Enable */

/************************************************************
* DMA_X
************************************************************/
#define __MSP430_HAS_DMAX_3__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_DMAX_3__ 0x0500
#define DMA_BASE __MSP430_BASEADDRESS_DMAX_3__

#define  DMACTL0_               (0x0500u)    /* DMA Module Control 0 */
DEFCW(   DMACTL0              , DMACTL0_)
#define  DMACTL1_               (0x0502u)    /* DMA Module Control 1 */
DEFCW(   DMACTL1              , DMACTL1_)
#define  DMACTL2_               (0x0504u)    /* DMA Module Control 2 */
DEFCW(   DMACTL2              , DMACTL2_)
#define  DMACTL3_               (0x0506u)    /* DMA Module Control 3 */
DEFCW(   DMACTL3              , DMACTL3_)
#define  DMACTL4_               (0x0508u)    /* DMA Module Control 4 */
DEFCW(   DMACTL4              , DMACTL4_)
#define  DMAIV_                 (0x050Eu)    /* DMA Interrupt Vector Word */
DEFCW(   DMAIV                , DMAIV_)

#define  DMA0CTL_               (0x0510u)    /* DMA Channel 0 Control */
DEFCW(   DMA0CTL              , DMA0CTL_)
#define  DMA0SA_                (0x0512u)    /* DMA Channel 0 Source Address */
DEFA(    DMA0SA               , DMA0SA_)
#define  DMA0DA_                (0x0516u)    /* DMA Channel 0 Destination Address */
DEFA(    DMA0DA               , DMA0DA_)
#define  DMA0SZ_                (0x051Au)    /* DMA Channel 0 Transfer Size */
DEFW(    DMA0SZ               , DMA0SZ_)

#define  DMA1CTL_               (0x0520u)    /* DMA Channel 1 Control */
DEFCW(   DMA1CTL              , DMA1CTL_)
#define  DMA1SA_                (0x0522u)    /* DMA Channel 1 Source Address */
DEFA(    DMA1SA               , DMA1SA_)
#define  DMA1DA_                (0x0526u)    /* DMA Channel 1 Destination Address */
DEFA(    DMA1DA               , DMA1DA_)
#define  DMA1SZ_                (0x052Au)    /* DMA Channel 1 Transfer Size */
DEFW(    DMA1SZ               , DMA1SZ_)

#define  DMA2CTL_               (0x0530u)    /* DMA Channel 2 Control */
DEFCW(   DMA2CTL              , DMA2CTL_)
#define  DMA2SA_                (0x0532u)    /* DMA Channel 2 Source Address */
DEFA(    DMA2SA               , DMA2SA_)
#define  DMA2DA_                (0x0536u)    /* DMA Channel 2 Destination Address */
DEFA(    DMA2DA               , DMA2DA_)
#define  DMA2SZ_                (0x053Au)    /* DMA Channel 2 Transfer Size */
DEFW(    DMA2SZ               , DMA2SZ_)

/* DMACTL0 Control Bits */
#define DMA0TSEL0           (0x0001u)    /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1           (0x0002u)    /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2           (0x0004u)    /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3           (0x0008u)    /* DMA channel 0 transfer select bit 3 */
#define DMA0TSEL4           (0x0010u)    /* DMA channel 0 transfer select bit 4 */
#define DMA1TSEL0           (0x0100u)    /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1           (0x0200u)    /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2           (0x0400u)    /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3           (0x0800u)    /* DMA channel 1 transfer select bit 3 */
#define DMA1TSEL4           (0x1000u)    /* DMA channel 1 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA0TSEL0_L         (0x0001u)    /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1_L         (0x0002u)    /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2_L         (0x0004u)    /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3_L         (0x0008u)    /* DMA channel 0 transfer select bit 3 */
#define DMA0TSEL4_L         (0x0010u)    /* DMA channel 0 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA1TSEL0_H         (0x0001u)    /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1_H         (0x0002u)    /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2_H         (0x0004u)    /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3_H         (0x0008u)    /* DMA channel 1 transfer select bit 3 */
#define DMA1TSEL4_H         (0x0010u)    /* DMA channel 1 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA2TSEL0           (0x0001u)    /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1           (0x0002u)    /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2           (0x0004u)    /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3           (0x0008u)    /* DMA channel 2 transfer select bit 3 */
#define DMA2TSEL4           (0x0010u)    /* DMA channel 2 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA2TSEL0_L         (0x0001u)    /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1_L         (0x0002u)    /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2_L         (0x0004u)    /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3_L         (0x0008u)    /* DMA channel 2 transfer select bit 3 */
#define DMA2TSEL4_L         (0x0010u)    /* DMA channel 2 transfer select bit 4 */

/* DMACTL4 Control Bits */
#define ENNMI               (0x0001u)    /* Enable NMI interruption of DMA */
#define ROUNDROBIN          (0x0002u)    /* Round-Robin DMA channel priorities */
#define DMARMWDIS           (0x0004u)    /* Inhibited DMA transfers during read-modify-write CPU operations */

/* DMACTL4 Control Bits */
#define ENNMI_L             (0x0001u)    /* Enable NMI interruption of DMA */
#define ROUNDROBIN_L        (0x0002u)    /* Round-Robin DMA channel priorities */
#define DMARMWDIS_L         (0x0004u)    /* Inhibited DMA transfers during read-modify-write CPU operations */

/* DMAxCTL Control Bits */
#define DMAREQ              (0x0001u)    /* Initiate DMA transfer with DMATSEL */
#define DMAABORT            (0x0002u)    /* DMA transfer aborted by NMI */
#define DMAIE               (0x0004u)    /* DMA interrupt enable */
#define DMAIFG              (0x0008u)    /* DMA interrupt flag */
#define DMAEN               (0x0010u)    /* DMA enable */
#define DMALEVEL            (0x0020u)    /* DMA level sensitive trigger select */
#define DMASRCBYTE          (0x0040u)    /* DMA source byte */
#define DMADSTBYTE          (0x0080u)    /* DMA destination byte */
#define DMASRCINCR0         (0x0100u)    /* DMA source increment bit 0 */
#define DMASRCINCR1         (0x0200u)    /* DMA source increment bit 1 */
#define DMADSTINCR0         (0x0400u)    /* DMA destination increment bit 0 */
#define DMADSTINCR1         (0x0800u)    /* DMA destination increment bit 1 */
#define DMADT0              (0x1000u)    /* DMA transfer mode bit 0 */
#define DMADT1              (0x2000u)    /* DMA transfer mode bit 1 */
#define DMADT2              (0x4000u)    /* DMA transfer mode bit 2 */

/* DMAxCTL Control Bits */
#define DMAREQ_L            (0x0001u)    /* Initiate DMA transfer with DMATSEL */
#define DMAABORT_L          (0x0002u)    /* DMA transfer aborted by NMI */
#define DMAIE_L             (0x0004u)    /* DMA interrupt enable */
#define DMAIFG_L            (0x0008u)    /* DMA interrupt flag */
#define DMAEN_L             (0x0010u)    /* DMA enable */
#define DMALEVEL_L          (0x0020u)    /* DMA level sensitive trigger select */
#define DMASRCBYTE_L        (0x0040u)    /* DMA source byte */
#define DMADSTBYTE_L        (0x0080u)    /* DMA destination byte */

/* DMAxCTL Control Bits */
#define DMASRCINCR0_H       (0x0001u)    /* DMA source increment bit 0 */
#define DMASRCINCR1_H       (0x0002u)    /* DMA source increment bit 1 */
#define DMADSTINCR0_H       (0x0004u)    /* DMA destination increment bit 0 */
#define DMADSTINCR1_H       (0x0008u)    /* DMA destination increment bit 1 */
#define DMADT0_H            (0x0010u)    /* DMA transfer mode bit 0 */
#define DMADT1_H            (0x0020u)    /* DMA transfer mode bit 1 */
#define DMADT2_H            (0x0040u)    /* DMA transfer mode bit 2 */

#define DMASWDW             (0*0x0040u)  /* DMA transfer: source word to destination word */
#define DMASBDW             (1*0x0040u)  /* DMA transfer: source byte to destination word */
#define DMASWDB             (2*0x0040u)  /* DMA transfer: source word to destination byte */
#define DMASBDB             (3*0x0040u)  /* DMA transfer: source byte to destination byte */

#define DMASRCINCR_0        (0*0x0100u)  /* DMA source increment 0: source address unchanged */
#define DMASRCINCR_1        (1*0x0100u)  /* DMA source increment 1: source address unchanged */
#define DMASRCINCR_2        (2*0x0100u)  /* DMA source increment 2: source address decremented */
#define DMASRCINCR_3        (3*0x0100u)  /* DMA source increment 3: source address incremented */

#define DMADSTINCR_0        (0*0x0400u)  /* DMA destination increment 0: destination address unchanged */
#define DMADSTINCR_1        (1*0x0400u)  /* DMA destination increment 1: destination address unchanged */
#define DMADSTINCR_2        (2*0x0400u)  /* DMA destination increment 2: destination address decremented */
#define DMADSTINCR_3        (3*0x0400u)  /* DMA destination increment 3: destination address incremented */

#define DMADT_0             (0*0x1000u)  /* DMA transfer mode 0: Single transfer */
#define DMADT_1             (1*0x1000u)  /* DMA transfer mode 1: Block transfer */
#define DMADT_2             (2*0x1000u)  /* DMA transfer mode 2: Burst-Block transfer */
#define DMADT_3             (3*0x1000u)  /* DMA transfer mode 3: Burst-Block transfer */
#define DMADT_4             (4*0x1000u)  /* DMA transfer mode 4: Repeated Single transfer */
#define DMADT_5             (5*0x1000u)  /* DMA transfer mode 5: Repeated Block transfer */
#define DMADT_6             (6*0x1000u)  /* DMA transfer mode 6: Repeated Burst-Block transfer */
#define DMADT_7             (7*0x1000u)  /* DMA transfer mode 7: Repeated Burst-Block transfer */

/* DMAIV Definitions */
#define DMAIV_NONE           (0x0000u)    /* No Interrupt pending */
#define DMAIV_DMA0IFG        (0x0002u)    /* DMA0IFG*/
#define DMAIV_DMA1IFG        (0x0004u)    /* DMA1IFG*/
#define DMAIV_DMA2IFG        (0x0006u)    /* DMA2IFG*/

#define DMA0TSEL_0          (0*0x0001u)  /* DMA channel 0 transfer select 0:  DMA_REQ (sw) */
#define DMA0TSEL_1          (1*0x0001u)  /* DMA channel 0 transfer select 1:  */
#define DMA0TSEL_2          (2*0x0001u)  /* DMA channel 0 transfer select 2:  */
#define DMA0TSEL_3          (3*0x0001u)  /* DMA channel 0 transfer select 3:  */
#define DMA0TSEL_4          (4*0x0001u)  /* DMA channel 0 transfer select 4:  */
#define DMA0TSEL_5          (5*0x0001u)  /* DMA channel 0 transfer select 5:  */
#define DMA0TSEL_6          (6*0x0001u)  /* DMA channel 0 transfer select 6:  */
#define DMA0TSEL_7          (7*0x0001u)  /* DMA channel 0 transfer select 7:  */
#define DMA0TSEL_8          (8*0x0001u)  /* DMA channel 0 transfer select 8:  */
#define DMA0TSEL_9          (9*0x0001u)  /* DMA channel 0 transfer select 9:  */
#define DMA0TSEL_10         (10*0x0001u) /* DMA channel 0 transfer select 10: */
#define DMA0TSEL_11         (11*0x0001u) /* DMA channel 0 transfer select 11: */
#define DMA0TSEL_12         (12*0x0001u) /* DMA channel 0 transfer select 12: */
#define DMA0TSEL_13         (13*0x0001u) /* DMA channel 0 transfer select 13: */
#define DMA0TSEL_14         (14*0x0001u) /* DMA channel 0 transfer select 14: */
#define DMA0TSEL_15         (15*0x0001u) /* DMA channel 0 transfer select 15: */
#define DMA0TSEL_16         (16*0x0001u) /* DMA channel 0 transfer select 16: */
#define DMA0TSEL_17         (17*0x0001u) /* DMA channel 0 transfer select 17: */
#define DMA0TSEL_18         (18*0x0001u) /* DMA channel 0 transfer select 18: */
#define DMA0TSEL_19         (19*0x0001u) /* DMA channel 0 transfer select 19: */
#define DMA0TSEL_20         (20*0x0001u) /* DMA channel 0 transfer select 20: */
#define DMA0TSEL_21         (21*0x0001u) /* DMA channel 0 transfer select 21: */
#define DMA0TSEL_22         (22*0x0001u) /* DMA channel 0 transfer select 22: */
#define DMA0TSEL_23         (23*0x0001u) /* DMA channel 0 transfer select 23: */
#define DMA0TSEL_24         (24*0x0001u) /* DMA channel 0 transfer select 24: */
#define DMA0TSEL_25         (25*0x0001u) /* DMA channel 0 transfer select 25: */
#define DMA0TSEL_26         (26*0x0001u) /* DMA channel 0 transfer select 26: */
#define DMA0TSEL_27         (27*0x0001u) /* DMA channel 0 transfer select 27: */
#define DMA0TSEL_28         (28*0x0001u) /* DMA channel 0 transfer select 28: */
#define DMA0TSEL_29         (29*0x0001u) /* DMA channel 0 transfer select 29: */
#define DMA0TSEL_30         (30*0x0001u) /* DMA channel 0 transfer select 30: previous DMA channel DMA2IFG */
#define DMA0TSEL_31         (31*0x0001u) /* DMA channel 0 transfer select 31: ext. Trigger (DMAE0) */

#define DMA1TSEL_0          (0*0x0100u)  /* DMA channel 1 transfer select 0:  DMA_REQ (sw) */
#define DMA1TSEL_1          (1*0x0100u)  /* DMA channel 1 transfer select 1:  */
#define DMA1TSEL_2          (2*0x0100u)  /* DMA channel 1 transfer select 2:  */
#define DMA1TSEL_3          (3*0x0100u)  /* DMA channel 1 transfer select 3:  */
#define DMA1TSEL_4          (4*0x0100u)  /* DMA channel 1 transfer select 4:  */
#define DMA1TSEL_5          (5*0x0100u)  /* DMA channel 1 transfer select 5:  */
#define DMA1TSEL_6          (6*0x0100u)  /* DMA channel 1 transfer select 6:  */
#define DMA1TSEL_7          (7*0x0100u)  /* DMA channel 1 transfer select 7:  */
#define DMA1TSEL_8          (8*0x0100u)  /* DMA channel 1 transfer select 8:  */
#define DMA1TSEL_9          (9*0x0100u)  /* DMA channel 1 transfer select 9:  */
#define DMA1TSEL_10         (10*0x0100u) /* DMA channel 1 transfer select 10: */
#define DMA1TSEL_11         (11*0x0100u) /* DMA channel 1 transfer select 11: */
#define DMA1TSEL_12         (12*0x0100u) /* DMA channel 1 transfer select 12: */
#define DMA1TSEL_13         (13*0x0100u) /* DMA channel 1 transfer select 13: */
#define DMA1TSEL_14         (14*0x0100u) /* DMA channel 1 transfer select 14: */
#define DMA1TSEL_15         (15*0x0100u) /* DMA channel 1 transfer select 15: */
#define DMA1TSEL_16         (16*0x0100u) /* DMA channel 1 transfer select 16: */
#define DMA1TSEL_17         (17*0x0100u) /* DMA channel 1 transfer select 17: */
#define DMA1TSEL_18         (18*0x0100u) /* DMA channel 1 transfer select 18: */
#define DMA1TSEL_19         (19*0x0100u) /* DMA channel 1 transfer select 19: */
#define DMA1TSEL_20         (20*0x0100u) /* DMA channel 1 transfer select 20: */
#define DMA1TSEL_21         (21*0x0100u) /* DMA channel 1 transfer select 21: */
#define DMA1TSEL_22         (22*0x0100u) /* DMA channel 1 transfer select 22: */
#define DMA1TSEL_23         (23*0x0100u) /* DMA channel 1 transfer select 23: */
#define DMA1TSEL_24         (24*0x0100u) /* DMA channel 1 transfer select 24: */
#define DMA1TSEL_25         (25*0x0100u) /* DMA channel 1 transfer select 25: */
#define DMA1TSEL_26         (26*0x0100u) /* DMA channel 1 transfer select 26: */
#define DMA1TSEL_27         (27*0x0100u) /* DMA channel 1 transfer select 27: */
#define DMA1TSEL_28         (28*0x0100u) /* DMA channel 1 transfer select 28: */
#define DMA1TSEL_29         (29*0x0100u) /* DMA channel 1 transfer select 29: */
#define DMA1TSEL_30         (30*0x0100u) /* DMA channel 1 transfer select 30: previous DMA channel DMA0IFG */
#define DMA1TSEL_31         (31*0x0100u) /* DMA channel 1 transfer select 31: ext. Trigger (DMAE0) */

#define DMA2TSEL_0          (0*0x0001u)  /* DMA channel 2 transfer select 0:  DMA_REQ (sw) */
#define DMA2TSEL_1          (1*0x0001u)  /* DMA channel 2 transfer select 1:  */
#define DMA2TSEL_2          (2*0x0001u)  /* DMA channel 2 transfer select 2:  */
#define DMA2TSEL_3          (3*0x0001u)  /* DMA channel 2 transfer select 3:  */
#define DMA2TSEL_4          (4*0x0001u)  /* DMA channel 2 transfer select 4:  */
#define DMA2TSEL_5          (5*0x0001u)  /* DMA channel 2 transfer select 5:  */
#define DMA2TSEL_6          (6*0x0001u)  /* DMA channel 2 transfer select 6:  */
#define DMA2TSEL_7          (7*0x0001u)  /* DMA channel 2 transfer select 7:  */
#define DMA2TSEL_8          (8*0x0001u)  /* DMA channel 2 transfer select 8:  */
#define DMA2TSEL_9          (9*0x0001u)  /* DMA channel 2 transfer select 9:  */
#define DMA2TSEL_10         (10*0x0001u) /* DMA channel 2 transfer select 10: */
#define DMA2TSEL_11         (11*0x0001u) /* DMA channel 2 transfer select 11: */
#define DMA2TSEL_12         (12*0x0001u) /* DMA channel 2 transfer select 12: */
#define DMA2TSEL_13         (13*0x0001u) /* DMA channel 2 transfer select 13: */
#define DMA2TSEL_14         (14*0x0001u) /* DMA channel 2 transfer select 14: */
#define DMA2TSEL_15         (15*0x0001u) /* DMA channel 2 transfer select 15: */
#define DMA2TSEL_16         (16*0x0001u) /* DMA channel 2 transfer select 16: */
#define DMA2TSEL_17         (17*0x0001u) /* DMA channel 2 transfer select 17: */
#define DMA2TSEL_18         (18*0x0001u) /* DMA channel 2 transfer select 18: */
#define DMA2TSEL_19         (19*0x0001u) /* DMA channel 2 transfer select 19: */
#define DMA2TSEL_20         (20*0x0001u) /* DMA channel 2 transfer select 20: */
#define DMA2TSEL_21         (21*0x0001u) /* DMA channel 2 transfer select 21: */
#define DMA2TSEL_22         (22*0x0001u) /* DMA channel 2 transfer select 22: */
#define DMA2TSEL_23         (23*0x0001u) /* DMA channel 2 transfer select 23: */
#define DMA2TSEL_24         (24*0x0001u) /* DMA channel 2 transfer select 24: */
#define DMA2TSEL_25         (25*0x0001u) /* DMA channel 2 transfer select 25: */
#define DMA2TSEL_26         (26*0x0001u) /* DMA channel 2 transfer select 26: */
#define DMA2TSEL_27         (27*0x0001u) /* DMA channel 2 transfer select 27: */
#define DMA2TSEL_28         (28*0x0001u) /* DMA channel 2 transfer select 28: */
#define DMA2TSEL_29         (29*0x0001u) /* DMA channel 2 transfer select 29: */
#define DMA2TSEL_30         (30*0x0001u) /* DMA channel 2 transfer select 30: previous DMA channel DMA1IFG */
#define DMA2TSEL_31         (31*0x0001u) /* DMA channel 2 transfer select 31: ext. Trigger (DMAE0) */

#define DMA0TSEL__DMAREQ     (0*0x0001u)  /* DMA channel 0 transfer select 0:  DMA_REQ (sw) */
#define DMA0TSEL__TA0CCR0    (1*0x0001u)  /* DMA channel 0 transfer select 1:  TA0CCR0 */
#define DMA0TSEL__TA0CCR2    (2*0x0001u)  /* DMA channel 0 transfer select 2:  TA0CCR2 */
#define DMA0TSEL__TA1CCR0    (3*0x0001u)  /* DMA channel 0 transfer select 3:  TA1CCR0 */
#define DMA0TSEL__TA1CCR2    (4*0x0001u)  /* DMA channel 0 transfer select 4:  TA1CCR2 */
#define DMA0TSEL__TA2CCR0    (5*0x0001u)  /* DMA channel 0 transfer select 3:  TA2CCR0 */
#define DMA0TSEL__TA3CCR0    (6*0x0001u)  /* DMA channel 0 transfer select 4:  TA3CCR0 */
#define DMA0TSEL__TB0CCR0    (7*0x0001u)  /* DMA channel 0 transfer select 7:  TB0CCR0 */
#define DMA0TSEL__TB0CCR2    (8*0x0001u)  /* DMA channel 0 transfer select 8:  TB0CCR2 */
#define DMA0TSEL__RES9       (9*0x0001u)  /* DMA channel 0 transfer select 9:  RES9 */
#define DMA0TSEL__RES10      (10*0x0001u) /* DMA channel 0 transfer select 10: RES10 */
#define DMA0TSEL__AES_Trigger_0 (11*0x0001u) /* DMA channel 0 transfer select 11: AES Trigger 0 */
#define DMA0TSEL__AES_Trigger_1 (12*0x0001u) /* DMA channel 0 transfer select 12: AES Trigger 1 */
#define DMA0TSEL__AES_Trigger_2 (13*0x0001u) /* DMA channel 0 transfer select 13: AES Trigger 2 */
#define DMA0TSEL__UCA0RXIFG  (14*0x0001u) /* DMA channel 0 transfer select 14: UCA0RXIFG */
#define DMA0TSEL__UCA0TXIFG  (15*0x0001u) /* DMA channel 0 transfer select 15: UCA0TXIFG */
#define DMA0TSEL__UCA1RXIFG  (16*0x0001u) /* DMA channel 0 transfer select 16: UCA1RXIFG */
#define DMA0TSEL__UCA1TXIFG  (17*0x0001u) /* DMA channel 0 transfer select 17: UCA1TXIFG */
#define DMA0TSEL__UCB0RXIFG0 (18*0x0001u) /* DMA channel 0 transfer select 18: UCB0RXIFG0 */
#define DMA0TSEL__UCB0TXIFG0 (19*0x0001u) /* DMA channel 0 transfer select 19: UCB0TXIFG0 */
#define DMA0TSEL__UCB0RXIFG1 (20*0x0001u) /* DMA channel 0 transfer select 20: UCB0RXIFG1 */
#define DMA0TSEL__UCB0TXIFG1 (21*0x0001u) /* DMA channel 0 transfer select 21: UCB0TXIFG1 */
#define DMA0TSEL__UCB0RXIFG2 (22*0x0001u) /* DMA channel 0 transfer select 22: UCB0RXIFG2 */
#define DMA0TSEL__UCB0TXIFG2 (23*0x0001u) /* DMA channel 0 transfer select 23: UCB0TXIFG2 */
#define DMA0TSEL__UCB1RXIFG0 (24*0x0001u) /* DMA channel 0 transfer select 24: UCB1RXIFG0 */
#define DMA0TSEL__UCB1TXIFG0 (25*0x0001u) /* DMA channel 0 transfer select 25: UCB1TXIFG0 */
#define DMA0TSEL__ADC12IFG   (26*0x0001u) /* DMA channel 0 transfer select 26: ADC12IFG */
#define DMA0TSEL__RES27      (27*0x0001u) /* DMA channel 0 transfer select 27: RES27 */
//#define DMA0TSEL__RES28      (28*0x0001u) /* DMA channel 0 transfer select 28: RES28 */
#define DMA0TSEL__ESI        (28*0x0001u) /* DMA channel 0 transfer select 28: ESI */
#define DMA0TSEL__MPY        (29*0x0001u) /* DMA channel 0 transfer select 29: MPY */
#define DMA0TSEL__DMA2IFG    (30*0x0001u) /* DMA channel 0 transfer select 30: previous DMA channel DMA2IFG */
#define DMA0TSEL__DMAE0      (31*0x0001u) /* DMA channel 0 transfer select 31: ext. Trigger (DMAE0) */

#define DMA1TSEL__DMAREQ     (0*0x0100u)  /* DMA channel 1 transfer select 0:  DMA_REQ (sw) */
#define DMA1TSEL__TA0CCR0    (1*0x0100u)  /* DMA channel 1 transfer select 1:  TA0CCR0 */
#define DMA1TSEL__TA0CCR2    (2*0x0100u)  /* DMA channel 1 transfer select 2:  TA0CCR2 */
#define DMA1TSEL__TA1CCR0    (3*0x0100u)  /* DMA channel 1 transfer select 3:  TA1CCR0 */
#define DMA1TSEL__TA1CCR2    (4*0x0100u)  /* DMA channel 1 transfer select 4:  TA1CCR2 */
#define DMA1TSEL__TA2CCR0    (5*0x0100u)  /* DMA channel 1 transfer select 5:  TA2CCR0 */
#define DMA1TSEL__TA3CCR0    (6*0x0100u)  /* DMA channel 1 transfer select 6:  TA3CCR0 */
#define DMA1TSEL__TB0CCR0    (7*0x0100u)  /* DMA channel 1 transfer select 7:  TB0CCR0 */
#define DMA1TSEL__TB0CCR2    (8*0x0100u)  /* DMA channel 1 transfer select 8:  TB0CCR2 */
#define DMA1TSEL__RES9       (9*0x0100u)  /* DMA channel 1 transfer select 9:  RES9 */
#define DMA1TSEL__RES10      (10*0x0100u) /* DMA channel 1 transfer select 10: RES10 */
#define DMA1TSEL__AES_Trigger_0 (11*0x0100u) /* DMA channel 1 transfer select 11: AES Trigger 0 */
#define DMA1TSEL__AES_Trigger_1 (12*0x0100u) /* DMA channel 1 transfer select 12: AES Trigger 1 */
#define DMA1TSEL__AES_Trigger_2 (13*0x0100u) /* DMA channel 1 transfer select 13: AES Trigger 2 */
#define DMA1TSEL__UCA0RXIFG  (14*0x0100u) /* DMA channel 1 transfer select 14: UCA0RXIFG */
#define DMA1TSEL__UCA0TXIFG  (15*0x0100u) /* DMA channel 1 transfer select 15: UCA0TXIFG */
#define DMA1TSEL__UCA1RXIFG  (16*0x0100u) /* DMA channel 1 transfer select 16: UCA1RXIFG */
#define DMA1TSEL__UCA1TXIFG  (17*0x0100u) /* DMA channel 1 transfer select 17: UCA1TXIFG */
#define DMA1TSEL__UCB0RXIFG0 (18*0x0100u) /* DMA channel 1 transfer select 18: UCB0RXIFG0 */
#define DMA1TSEL__UCB0TXIFG0 (19*0x0100u) /* DMA channel 1 transfer select 19: UCB0TXIFG0 */
#define DMA1TSEL__UCB0RXIFG1 (20*0x0100u) /* DMA channel 1 transfer select 20: UCB0RXIFG1 */
#define DMA1TSEL__UCB0TXIFG1 (21*0x0100u) /* DMA channel 1 transfer select 21: UCB0TXIFG1 */
#define DMA1TSEL__UCB0RXIFG2 (22*0x0100u) /* DMA channel 1 transfer select 22: UCB0RXIFG2 */
#define DMA1TSEL__UCB0TXIFG2 (23*0x0100u) /* DMA channel 1 transfer select 23: UCB0TXIFG2 */
#define DMA1TSEL__UCB1RXIFG0 (24*0x0100u) /* DMA channel 1 transfer select 24: UCB1RXIFG0 */
#define DMA1TSEL__UCB1TXIFG0 (25*0x0100u) /* DMA channel 1 transfer select 25: UCB1TXIFG0 */
#define DMA1TSEL__ADC12IFG   (26*0x0100u) /* DMA channel 1 transfer select 26: ADC12IFG */
#define DMA1TSEL__RES27      (27*0x0100u) /* DMA channel 1 transfer select 27: RES27 */
#define DMA1TSEL__ESI        (28*0x0100u) /* DMA channel 1 transfer select 28: ESI */
#define DMA1TSEL__MPY        (29*0x0100u) /* DMA channel 1 transfer select 29: MPY */
#define DMA1TSEL__DMA0IFG    (30*0x0100u) /* DMA channel 1 transfer select 30: previous DMA channel DMA0IFG */
#define DMA1TSEL__DMAE0      (31*0x0100u) /* DMA channel 1 transfer select 31: ext. Trigger (DMAE0) */

#define DMA2TSEL__DMAREQ     (0*0x0001u)  /* DMA channel 2 transfer select 0:  DMA_REQ (sw) */
#define DMA2TSEL__TA0CCR0    (1*0x0001u)  /* DMA channel 2 transfer select 1:  TA0CCR0 */
#define DMA2TSEL__TA0CCR2    (2*0x0001u)  /* DMA channel 2 transfer select 2:  TA0CCR2 */
#define DMA2TSEL__TA1CCR0    (3*0x0001u)  /* DMA channel 2 transfer select 3:  TA1CCR0 */
#define DMA2TSEL__TA1CCR2    (4*0x0001u)  /* DMA channel 2 transfer select 4:  TA1CCR2 */
#define DMA2TSEL__TA2CCR0    (5*0x0001u)  /* DMA channel 2 transfer select 5:  TA2CCR0 */
#define DMA2TSEL__TA3CCR0    (6*0x0001u)  /* DMA channel 2 transfer select 6:  TA3CCR0 */
#define DMA2TSEL__TB0CCR0    (7*0x0001u)  /* DMA channel 2 transfer select 7:  TB0CCR0 */
#define DMA2TSEL__TB0CCR2    (8*0x0001u)  /* DMA channel 2 transfer select 8:  TB0CCR2 */
#define DMA2TSEL__RES9       (9*0x0001u)  /* DMA channel 2 transfer select 9:  RES9 */
#define DMA2TSEL__RES10      (10*0x0001u) /* DMA channel 2 transfer select 10: RES10 */
#define DMA2TSEL__AES_Trigger_0 (11*0x0001u) /* DMA channel 2 transfer select 11: AES Trigger 0 */
#define DMA2TSEL__AES_Trigger_1 (12*0x0001u) /* DMA channel 2 transfer select 12: AES Trigger 1 */
#define DMA2TSEL__AES_Trigger_2 (13*0x0001u) /* DMA channel 2 transfer select 13: AES Trigger 2 */
#define DMA2TSEL__UCA0RXIFG  (14*0x0001u) /* DMA channel 2 transfer select 14: UCA0RXIFG */
#define DMA2TSEL__UCA0TXIFG  (15*0x0001u) /* DMA channel 2 transfer select 15: UCA0TXIFG */
#define DMA2TSEL__UCA1RXIFG  (16*0x0001u) /* DMA channel 2 transfer select 16: UCA1RXIFG */
#define DMA2TSEL__UCA1TXIFG  (17*0x0001u) /* DMA channel 2 transfer select 17: UCA1TXIFG */
#define DMA2TSEL__UCB0RXIFG0 (18*0x0001u) /* DMA channel 2 transfer select 18: UCB0RXIFG0 */
#define DMA2TSEL__UCB0TXIFG0 (19*0x0001u) /* DMA channel 2 transfer select 19: UCB0TXIFG0 */
#define DMA2TSEL__UCB0RXIFG1 (20*0x0001u) /* DMA channel 2 transfer select 20: UCB0RXIFG1 */
#define DMA2TSEL__UCB0TXIFG1 (21*0x0001u) /* DMA channel 2 transfer select 21: UCB0TXIFG1 */
#define DMA2TSEL__UCB0RXIFG2 (22*0x0001u) /* DMA channel 2 transfer select 22: UCB0RXIFG2 */
#define DMA2TSEL__UCB0TXIFG2 (23*0x0001u) /* DMA channel 2 transfer select 23: UCB0TXIFG2 */
#define DMA2TSEL__UCB1RXIFG0 (24*0x0001u) /* DMA channel 2 transfer select 24: UCB1RXIFG0 */
#define DMA2TSEL__UCB1TXIFG0 (25*0x0001u) /* DMA channel 2 transfer select 25: UCB1TXIFG0 */
#define DMA2TSEL__ADC12IFG   (26*0x0001u) /* DMA channel 2 transfer select 26: ADC12IFG */
#define DMA2TSEL__RES27      (27*0x0001u) /* DMA channel 2 transfer select 27: RES27 */
#define DMA2TSEL__ESI        (28*0x0001u) /* DMA channel 2 transfer select 28: ESI */
#define DMA2TSEL__MPY        (29*0x0001u) /* DMA channel 2 transfer select 29: MPY */
#define DMA2TSEL__DMA1IFG    (30*0x0001u) /* DMA channel 2 transfer select 30: previous DMA channel DMA1IFG */
#define DMA2TSEL__DMAE0      (31*0x0001u) /* DMA channel 2 transfer select 31: ext. Trigger (DMAE0) */

/************************************************************
* EXTENDED SCAN INTERFACE
************************************************************/
#define __MSP430_HAS_ESI__                /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_ESI__ 0x0D00
#define ESI_BASE __MSP430_BASEADDRESS_ESI__

#define ESIDEBUG1_             (0x0D00u)      /* ESI debug register 1 */
DEFCW(  ESIDEBUG1            , ESIDEBUG1_)
#define ESIDEBUG2_             (0x0D02u)      /* ESI debug register 2 */
DEFCW(  ESIDEBUG2            , ESIDEBUG2_)
#define ESIDEBUG3_             (0x0D04u)      /* ESI debug register 3 */
DEFCW(  ESIDEBUG3            , ESIDEBUG3_)
#define ESIDEBUG4_             (0x0D06u)      /* ESI debug register 4 */
DEFCW(  ESIDEBUG4            , ESIDEBUG4_)
#define ESIDEBUG5_             (0x0D08u)      /* ESI debug register 5 */
DEFCW(  ESIDEBUG5            , ESIDEBUG5_)
#define ESICNT0_               (0x0D10u)      /* ESI PSM counter 0 */
DEFCW(  ESICNT0              , ESICNT0_)
#define ESICNT1_               (0x0D12u)      /* ESI PSM counter 1 */
DEFCW(  ESICNT1              , ESICNT1_)
#define ESICNT2_               (0x0D14u)      /* ESI PSM counter 2 */
DEFCW(  ESICNT2              , ESICNT2_)
#define ESICNT3_               (0x0D16u)      /* ESI oscillator counter register */
DEFCW(  ESICNT3              , ESICNT3_)
#define ESIIV_                 (0x0D1Au)      /* ESI interrupt vector */
DEFCW(  ESIIV                , ESIIV_)
#define ESIINT1_               (0x0D1Cu)      /* ESI interrupt register 1 */
DEFCW(  ESIINT1              , ESIINT1_)
#define ESIINT2_               (0x0D1Eu)      /* ESI interrupt register 2 */
DEFCW(  ESIINT2              , ESIINT2_)
#define ESIAFE_                (0x0D20u)      /* ESI AFE control register */
DEFCW(  ESIAFE               , ESIAFE_)
#define ESIPPU_                (0x0D22u)      /* ESI PPU control register */
DEFCW(  ESIPPU               , ESIPPU_)
#define ESITSM_                (0x0D24u)      /* ESI TSM control register */
DEFCW(  ESITSM               , ESITSM_)
#define ESIPSM_                (0x0D26u)      /* ESI PSM control register */
DEFCW(  ESIPSM               , ESIPSM_)
#define ESIOSC_                (0x0D28u)      /* ESI oscillator control register*/
DEFCW(  ESIOSC               , ESIOSC_)
#define ESICTL_                (0x0D2Au)      /* ESI control register */
DEFCW(  ESICTL               , ESICTL_)
#define  ESITHR1_              (0x0D2Cu)      /* ESI PSM Counter Threshold 1 register */
DEFCW(   ESITHR1             , ESITHR1_)
#define  ESITHR2_              (0x0D2Eu)      /* ESI PSM Counter Threshold 2 register */
DEFCW(   ESITHR2             , ESITHR2_)
#define ESIDAC1R0_             (0x0D40u)      /* ESI DAC1 register 0 */
DEFCW(  ESIDAC1R0            , ESIDAC1R0_)
#define ESIDAC1R1_             (0x0D42u)      /* ESI DAC1 register 1 */
DEFCW(  ESIDAC1R1            , ESIDAC1R1_)
#define ESIDAC1R2_             (0x0D44u)      /* ESI DAC1 register 2 */
DEFCW(  ESIDAC1R2            , ESIDAC1R2_)
#define ESIDAC1R3_             (0x0D46u)      /* ESI DAC1 register 3 */
DEFCW(  ESIDAC1R3            , ESIDAC1R3_)
#define ESIDAC1R4_             (0x0D48u)      /* ESI DAC1 register 4 */
DEFCW(  ESIDAC1R4            , ESIDAC1R4_)
#define ESIDAC1R5_             (0x0D4Au)      /* ESI DAC1 register 5 */
DEFCW(  ESIDAC1R5            , ESIDAC1R5_)
#define ESIDAC1R6_             (0x0D4Cu)      /* ESI DAC1 register 6 */
DEFCW(  ESIDAC1R6            , ESIDAC1R6_)
#define ESIDAC1R7_             (0x0D4Eu)      /* ESI DAC1 register 7 */
DEFCW(  ESIDAC1R7            , ESIDAC1R7_)
#define ESIDAC2R0_             (0x0D50u)      /* ESI DAC2 register 0 */
DEFCW(  ESIDAC2R0            , ESIDAC2R0_)
#define ESIDAC2R1_             (0x0D52u)      /* ESI DAC2 register 1 */
DEFCW(  ESIDAC2R1            , ESIDAC2R1_)
#define ESIDAC2R2_             (0x0D54u)      /* ESI DAC2 register 2 */
DEFCW(  ESIDAC2R2            , ESIDAC2R2_)
#define ESIDAC2R3_             (0x0D56u)      /* ESI DAC2 register 3 */
DEFCW(  ESIDAC2R3            , ESIDAC2R3_)
#define ESIDAC2R4_             (0x0D58u)      /* ESI DAC2 register 4 */
DEFCW(  ESIDAC2R4            , ESIDAC2R4_)
#define ESIDAC2R5_             (0x0D5Au)      /* ESI DAC2 register 5 */
DEFCW(  ESIDAC2R5            , ESIDAC2R5_)
#define ESIDAC2R6_             (0x0D5Cu)      /* ESI DAC2 register 6 */
DEFCW(  ESIDAC2R6            , ESIDAC2R6_)
#define ESIDAC2R7_             (0x0D5Eu)      /* ESI DAC2 register 7 */
DEFCW(  ESIDAC2R7            , ESIDAC2R7_)
#define ESITSM0_               (0x0D60u)      /* ESI TSM 0 */
DEFCW(  ESITSM0              , ESITSM0_)
#define ESITSM1_               (0x0D62u)      /* ESI TSM 1 */
DEFCW(  ESITSM1              , ESITSM1_)
#define ESITSM2_               (0x0D64u)      /* ESI TSM 2 */
DEFCW(  ESITSM2              , ESITSM2_)
#define ESITSM3_               (0x0D66u)      /* ESI TSM 3 */
DEFCW(  ESITSM3              , ESITSM3_)
#define ESITSM4_               (0x0D68u)      /* ESI TSM 4 */
DEFCW(  ESITSM4              , ESITSM4_)
#define ESITSM5_               (0x0D6Au)      /* ESI TSM 5 */
DEFCW(  ESITSM5              , ESITSM5_)
#define ESITSM6_               (0x0D6Cu)      /* ESI TSM 6 */
DEFCW(  ESITSM6              , ESITSM6_)
#define ESITSM7_               (0x0D6Eu)      /* ESI TSM 7 */
DEFCW(  ESITSM7              , ESITSM7_)
#define ESITSM8_               (0x0D70u)      /* ESI TSM 8 */
DEFCW(  ESITSM8              , ESITSM8_)
#define ESITSM9_               (0x0D72u)      /* ESI TSM 9 */
DEFCW(  ESITSM9              , ESITSM9_)
#define ESITSM10_              (0x0D74u)      /* ESI TSM 10 */
DEFCW(  ESITSM10             , ESITSM10_)
#define ESITSM11_              (0x0D76u)      /* ESI TSM 11 */
DEFCW(  ESITSM11             , ESITSM11_)
#define ESITSM12_              (0x0D78u)      /* ESI TSM 12 */
DEFCW(  ESITSM12             , ESITSM12_)
#define ESITSM13_              (0x0D7Au)      /* ESI TSM 13 */
DEFCW(  ESITSM13             , ESITSM13_)
#define ESITSM14_              (0x0D7Cu)      /* ESI TSM 14 */
DEFCW(  ESITSM14             , ESITSM14_)
#define ESITSM15_              (0x0D7Eu)      /* ESI TSM 15 */
DEFCW(  ESITSM15             , ESITSM15_)
#define ESITSM16_              (0x0D80u)      /* ESI TSM 16 */
DEFCW(  ESITSM16             , ESITSM16_)
#define ESITSM17_              (0x0D82u)      /* ESI TSM 17 */
DEFCW(  ESITSM17             , ESITSM17_)
#define ESITSM18_              (0x0D84u)      /* ESI TSM 18 */
DEFCW(  ESITSM18             , ESITSM18_)
#define ESITSM19_              (0x0D86u)      /* ESI TSM 19 */
DEFCW(  ESITSM19             , ESITSM19_)
#define ESITSM20_              (0x0D88u)      /* ESI TSM 20 */
DEFCW(  ESITSM20             , ESITSM20_)
#define ESITSM21_              (0x0D8Au)      /* ESI TSM 21 */
DEFCW(  ESITSM21             , ESITSM21_)
#define ESITSM22_              (0x0D8Cu)      /* ESI TSM 22 */
DEFCW(  ESITSM22             , ESITSM22_)
#define ESITSM23_              (0x0D8Eu)      /* ESI TSM 23 */
DEFCW(  ESITSM23             , ESITSM23_)
#define ESITSM24_              (0x0D90u)      /* ESI TSM 24 */
DEFCW(  ESITSM24             , ESITSM24_)
#define ESITSM25_              (0x0D92u)      /* ESI TSM 25 */
DEFCW(  ESITSM25             , ESITSM25_)
#define ESITSM26_              (0x0D94u)      /* ESI TSM 26 */
DEFCW(  ESITSM26             , ESITSM26_)
#define ESITSM27_              (0x0D96u)      /* ESI TSM 27 */
DEFCW(  ESITSM27             , ESITSM27_)
#define ESITSM28_              (0x0D98u)      /* ESI TSM 28 */
DEFCW(  ESITSM28             , ESITSM28_)
#define ESITSM29_              (0x0D9Au)      /* ESI TSM 29 */
DEFCW(  ESITSM29             , ESITSM29_)
#define ESITSM30_              (0x0D9Cu)      /* ESI TSM 30 */
DEFCW(  ESITSM30             , ESITSM30_)
#define ESITSM31_              (0x0D9Eu)      /* ESI TSM 31 */
DEFCW(  ESITSM31             , ESITSM31_)

/* ESIIV Control Bits */

#define ESIIV_NONE          (0x0000u)      /* No ESI Interrupt Pending */
#define ESIIV_ESIIFG1       (0x0002u)      /* rising edge of the ESISTOP(tsm) */
#define ESIIV_ESIIFG0       (0x0004u)      /* ESIOUT0 to ESIOUT3 conditions selected by ESIIFGSETx bits */
#define ESIIV_ESIIFG8       (0x0006u)      /* ESIOUT4 to ESIOUT7 conditions selected by ESIIFGSET2x bits */
#define ESIIV_ESIIFG3       (0x0008u)      /* ESICNT1 counter conditions selected with the ESITHR1 and ESITHR2 registers */
#define ESIIV_ESIIFG6       (0x000Au)      /* PSM transitions to a state with a Q7 bit */
#define ESIIV_ESIIFG5       (0x000Cu)      /* PSM transitions to a state with a Q6 bit */
#define ESIIV_ESIIFG4       (0x000Eu)      /* ESICNT2 counter conditions selected with the ESIIS2x bits */
#define ESIIV_ESIIFG7       (0x0010u)      /* ESICNT0 counter conditions selected with the ESIIS0x bits */
#define ESIIV_ESIIFG2       (0x0012u)      /* start of a TSM sequence */

/* ESIINT1 Control Bits */
#define ESIIFGSET22         (0x8000u)      /* ESIIFG8 interrupt flag source */
#define ESIIFGSET21         (0x4000u)      /* ESIIFG8 interrupt flag source */
#define ESIIFGSET20         (0x2000u)      /* ESIIFG8 interrupt flag source */
#define ESIIFGSET12         (0x1000u)      /* ESIIFG0 interrupt flag source */
#define ESIIFGSET11         (0x0800u)      /* ESIIFG0 interrupt flag source */
#define ESIIFGSET10         (0x0400u)      /* ESIIFG0 interrupt flag source */
#define ESIIE8              (0x0100u)      /* Interrupt enable */
#define ESIIE7              (0x0080u)      /* Interrupt enable */
#define ESIIE6              (0x0040u)      /* Interrupt enable */
#define ESIIE5              (0x0020u)      /* Interrupt enable */
#define ESIIE4              (0x0010u)      /* Interrupt enable */
#define ESIIE3              (0x0008u)      /* Interrupt enable */
#define ESIIE2              (0x0004u)      /* Interrupt enable */
#define ESIIE1              (0x0002u)      /* Interrupt enable */
#define ESIIE0              (0x0001u)      /* Interrupt enable */

/* ESIINT1 Control Bits */
#define ESIIE7_L            (0x0080u)      /* Interrupt enable */
#define ESIIE6_L            (0x0040u)      /* Interrupt enable */
#define ESIIE5_L            (0x0020u)      /* Interrupt enable */
#define ESIIE4_L            (0x0010u)      /* Interrupt enable */
#define ESIIE3_L            (0x0008u)      /* Interrupt enable */
#define ESIIE2_L            (0x0004u)      /* Interrupt enable */
#define ESIIE1_L            (0x0002u)      /* Interrupt enable */
#define ESIIE0_L            (0x0001u)      /* Interrupt enable */

/* ESIINT1 Control Bits */
#define ESIIFGSET22_H       (0x0080u)      /* ESIIFG8 interrupt flag source */
#define ESIIFGSET21_H       (0x0040u)      /* ESIIFG8 interrupt flag source */
#define ESIIFGSET20_H       (0x0020u)      /* ESIIFG8 interrupt flag source */
#define ESIIFGSET12_H       (0x0010u)      /* ESIIFG0 interrupt flag source */
#define ESIIFGSET11_H       (0x0008u)      /* ESIIFG0 interrupt flag source */
#define ESIIFGSET10_H       (0x0004u)      /* ESIIFG0 interrupt flag source */
#define ESIIE8_H            (0x0001u)      /* Interrupt enable */

#define ESIIFGSET2_0        (0x0000u)      /* ESIIFG8 is set when ESIOUT4 is set */
#define ESIIFGSET2_1        (0x2000u)      /* ESIIFG8 is set when ESIOUT4 is reset */
#define ESIIFGSET2_2        (0x4000u)      /* ESIIFG8 is set when ESIOUT5 is set */
#define ESIIFGSET2_3        (0x6000u)      /* ESIIFG8 is set when ESIOUT5 is reset */
#define ESIIFGSET2_4        (0x8000u)      /* ESIIFG8 is set when ESIOUT6 is set */
#define ESIIFGSET2_5        (0xA000u)      /* ESIIFG8 is set when ESIOUT6 is reset */
#define ESIIFGSET2_6        (0xC000u)      /* ESIIFG8 is set when ESIOUT7 is set */
#define ESIIFGSET2_7        (0xE000u)      /* ESIIFG8 is set when ESIOUT7 is reset */
#define ESIIFGSET1_0        (0x0000u)      /* ESIIFG0 is set when ESIOUT0 is set */
#define ESIIFGSET1_1        (0x0400u)      /* ESIIFG0 is set when ESIOUT0 is reset */
#define ESIIFGSET1_2        (0x0800u)      /* ESIIFG0 is set when ESIOUT1 is set */
#define ESIIFGSET1_3        (0x0C00u)      /* ESIIFG0 is set when ESIOUT1 is reset */
#define ESIIFGSET1_4        (0x1000u)      /* ESIIFG0 is set when ESIOUT2 is set */
#define ESIIFGSET1_5        (0x1400u)      /* ESIIFG0 is set when ESIOUT2 is reset */
#define ESIIFGSET1_6        (0x1800u)      /* ESIIFG0 is set when ESIOUT3 is set */
#define ESIIFGSET1_7        (0x1C00u)      /* ESIIFG0 is set when ESIOUT3 is reset */

/* ESIINT2 Control Bits */
#define ESIIS21             (0x4000u)      /* SIFIFG4 interrupt flag source */
#define ESIIS20             (0x2000u)      /* SIFIFG4 interrupt flag source */
#define ESIIS01             (0x0800u)      /* SIFIFG7 interrupt flag source */
#define ESIIS00             (0x0400u)      /* SIFIFG7 interrupt flag source */
#define ESIIFG8             (0x0100u)      /* ESIIFG8 interrupt pending */
#define ESIIFG7             (0x0080u)      /* ESIIFG7 interrupt pending */
#define ESIIFG6             (0x0040u)      /* ESIIFG6 interrupt pending */
#define ESIIFG5             (0x0020u)      /* ESIIFG5 interrupt pending */
#define ESIIFG4             (0x0010u)      /* ESIIFG4 interrupt pending */
#define ESIIFG3             (0x0008u)      /* ESIIFG3 interrupt pending */
#define ESIIFG2             (0x0004u)      /* ESIIFG2 interrupt pending */
#define ESIIFG1             (0x0002u)      /* ESIIFG1 interrupt pending */
#define ESIIFG0             (0x0001u)      /* ESIIFG0 interrupt pending */

/* ESIINT2 Control Bits */
#define ESIIFG7_L           (0x0080u)      /* ESIIFG7 interrupt pending */
#define ESIIFG6_L           (0x0040u)      /* ESIIFG6 interrupt pending */
#define ESIIFG5_L           (0x0020u)      /* ESIIFG5 interrupt pending */
#define ESIIFG4_L           (0x0010u)      /* ESIIFG4 interrupt pending */
#define ESIIFG3_L           (0x0008u)      /* ESIIFG3 interrupt pending */
#define ESIIFG2_L           (0x0004u)      /* ESIIFG2 interrupt pending */
#define ESIIFG1_L           (0x0002u)      /* ESIIFG1 interrupt pending */
#define ESIIFG0_L           (0x0001u)      /* ESIIFG0 interrupt pending */

/* ESIINT2 Control Bits */
#define ESIIS21_H           (0x0040u)      /* SIFIFG4 interrupt flag source */
#define ESIIS20_H           (0x0020u)      /* SIFIFG4 interrupt flag source */
#define ESIIS01_H           (0x0008u)      /* SIFIFG7 interrupt flag source */
#define ESIIS00_H           (0x0004u)      /* SIFIFG7 interrupt flag source */
#define ESIIFG8_H           (0x0001u)      /* ESIIFG8 interrupt pending */

#define ESIIS2_0            (0x0000u)      /* SIFIFG4 interrupt flag source: SIFCNT2 */
#define ESIIS2_1            (0x2000u)      /* SIFIFG4 interrupt flag source: SIFCNT2 MOD 4 */
#define ESIIS2_2            (0x4000u)      /* SIFIFG4 interrupt flag source: SIFCNT2 MOD 256 */
#define ESIIS2_3            (0x6000u)      /* SIFIFG4 interrupt flag source: SIFCNT2 decrements from 01h to 00h */
#define ESIIS0_0            (0x0000u)      /* SIFIFG7 interrupt flag source: SIFCNT0 */
#define ESIIS0_1            (0x0400u)      /* SIFIFG7 interrupt flag source: SIFCNT0 MOD 4 */
#define ESIIS0_2            (0x0800u)      /* SIFIFG7 interrupt flag source: SIFCNT0 MOD 256 */
#define ESIIS0_3            (0x0C00u)      /* SIFIFG7 interrupt flag source: SIFCNT0 increments from FFFFh to 00h */

/* ESIAFE Control Bits */
#define ESIDAC2EN           (0x0800u)      /* Enable ESIDAC(tsm) control for DAC in AFE2 */
#define ESICA2EN            (0x0400u)      /* Enable ESICA(tsm) control for comparator in AFE2 */
#define ESICA2INV           (0x0200u)      /* Invert AFE2's comparator output */
#define ESICA1INV           (0x0100u)      /* Invert AFE1's comparator output */
#define ESICA2X             (0x0080u)      /* AFE2's comparator input select */
#define ESICA1X             (0x0040u)      /* AFE1's comparator input select */
#define ESICISEL            (0x0020u)      /* Comparator input select for AFE1 only */
#define ESICACI3            (0x0010u)      /* Comparator input select for AFE1 only */
#define ESISHTSM            (0x0008u)      /* Sample-and-hold ESIVSS select */
#define ESIVMIDEN           (0x0004u)      /* Mid-voltage generator */
#define ESISH               (0x0002u)      /* Sample-and-hold enable */
#define ESITEN              (0x0001u)      /* Excitation enable */

/* ESIAFE Control Bits */
#define ESICA2X_L           (0x0080u)      /* AFE2's comparator input select */
#define ESICA1X_L           (0x0040u)      /* AFE1's comparator input select */
#define ESICISEL_L          (0x0020u)      /* Comparator input select for AFE1 only */
#define ESICACI3_L          (0x0010u)      /* Comparator input select for AFE1 only */
#define ESISHTSM_L          (0x0008u)      /* Sample-and-hold ESIVSS select */
#define ESIVMIDEN_L         (0x0004u)      /* Mid-voltage generator */
#define ESISH_L             (0x0002u)      /* Sample-and-hold enable */
#define ESITEN_L            (0x0001u)      /* Excitation enable */

/* ESIAFE Control Bits */
#define ESIDAC2EN_H         (0x0008u)      /* Enable ESIDAC(tsm) control for DAC in AFE2 */
#define ESICA2EN_H          (0x0004u)      /* Enable ESICA(tsm) control for comparator in AFE2 */
#define ESICA2INV_H         (0x0002u)      /* Invert AFE2's comparator output */
#define ESICA1INV_H         (0x0001u)      /* Invert AFE1's comparator output */

#define ESIVSS              (0x0008u)      /* legacy define: Sample-and-hold ESIVSS select */
#define ESIVCC2             (0x0004u)      /* legacy define: Mid-voltage generator */

/* ESIPPU Control Bits */
#define ESITCHOUT1          (0x0200u)      /* Latched AFE1 comparator output for test channel 1 */
#define ESITCHOUT0          (0x0100u)      /* Lachted AFE1 comparator output for test channel 0 */
#define ESIOUT7             (0x0080u)      /* Latched AFE2 comparator output when ESICH3 input is selected */
#define ESIOUT6             (0x0040u)      /* Latched AFE2 comparator output when ESICH2 input is selected */
#define ESIOUT5             (0x0020u)      /* Latched AFE2 comparator output when ESICH1 input is selected */
#define ESIOUT4             (0x0010u)      /* Latched AFE2 comparator output when ESICH0 input is selected */
#define ESIOUT3             (0x0008u)      /* Latched AFE1 comparator output when ESICH3 input is selected */
#define ESIOUT2             (0x0004u)      /* Latched AFE1 comparator output when ESICH2 input is selected */
#define ESIOUT1             (0x0002u)      /* Latched AFE1 comparator output when ESICH1 input is selected */
#define ESIOUT0             (0x0001u)      /* Latched AFE1 comparator output when ESICH0 input is selected */

/* ESIPPU Control Bits */
#define ESIOUT7_L           (0x0080u)      /* Latched AFE2 comparator output when ESICH3 input is selected */
#define ESIOUT6_L           (0x0040u)      /* Latched AFE2 comparator output when ESICH2 input is selected */
#define ESIOUT5_L           (0x0020u)      /* Latched AFE2 comparator output when ESICH1 input is selected */
#define ESIOUT4_L           (0x0010u)      /* Latched AFE2 comparator output when ESICH0 input is selected */
#define ESIOUT3_L           (0x0008u)      /* Latched AFE1 comparator output when ESICH3 input is selected */
#define ESIOUT2_L           (0x0004u)      /* Latched AFE1 comparator output when ESICH2 input is selected */
#define ESIOUT1_L           (0x0002u)      /* Latched AFE1 comparator output when ESICH1 input is selected */
#define ESIOUT0_L           (0x0001u)      /* Latched AFE1 comparator output when ESICH0 input is selected */

/* ESIPPU Control Bits */
#define ESITCHOUT1_H        (0x0002u)      /* Latched AFE1 comparator output for test channel 1 */
#define ESITCHOUT0_H        (0x0001u)      /* Lachted AFE1 comparator output for test channel 0 */

/* ESITSM Control Bits */
#define ESICLKAZSEL         (0x4000u)      /* Functionality selection of ESITSMx bit5 */
#define ESITSMTRG1          (0x2000u)      /* TSM start trigger selection */
#define ESITSMTRG0          (0x1000u)      /* TSM start trigger selection */
#define ESISTART            (0x0800u)      /* TSM software start trigger */
#define ESITSMRP            (0x0400u)      /* TSM repeat modee */
#define ESIDIV3B2           (0x0200u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B1           (0x0100u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B0           (0x0080u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A2           (0x0040u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A1           (0x0020u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A0           (0x0010u)      /* TSM start trigger ACLK divider */
#define ESIDIV21            (0x0008u)      /* ACLK divider */
#define ESIDIV20            (0x0004u)      /* ACLK divider */
#define ESIDIV11            (0x0002u)      /* TSM SMCLK divider */
#define ESIDIV10            (0x0001u)      /* TSM SMCLK divider */

/* ESITSM Control Bits */
#define ESIDIV3B0_L         (0x0080u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A2_L         (0x0040u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A1_L         (0x0020u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A0_L         (0x0010u)      /* TSM start trigger ACLK divider */
#define ESIDIV21_L          (0x0008u)      /* ACLK divider */
#define ESIDIV20_L          (0x0004u)      /* ACLK divider */
#define ESIDIV11_L          (0x0002u)      /* TSM SMCLK divider */
#define ESIDIV10_L          (0x0001u)      /* TSM SMCLK divider */

/* ESITSM Control Bits */
#define ESICLKAZSEL_H       (0x0040u)      /* Functionality selection of ESITSMx bit5 */
#define ESITSMTRG1_H        (0x0020u)      /* TSM start trigger selection */
#define ESITSMTRG0_H        (0x0010u)      /* TSM start trigger selection */
#define ESISTART_H          (0x0008u)      /* TSM software start trigger */
#define ESITSMRP_H          (0x0004u)      /* TSM repeat modee */
#define ESIDIV3B2_H         (0x0002u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B1_H         (0x0001u)      /* TSM start trigger ACLK divider */

#define ESITSMTRG_0         (0x0000u)      /* Halt mode */
#define ESITSMTRG_1         (0x1000u)      /* TSM start trigger ACLK divider */
#define ESITSMTRG_2         (0x2000u)      /* Software trigger for TSM */
#define ESITSMTRG_3         (0x3000u)      /* Either the ACLK divider or the ESISTART biT */
#define ESIDIV3B_0          (0x0000u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B_1          (0x0080u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B_2          (0x0100u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B_3          (0x0180u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B_4          (0x0200u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B_5          (0x0280u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B_6          (0x0300u)      /* TSM start trigger ACLK divider */
#define ESIDIV3B_7          (0x0380u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A_0          (0x0000u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A_1          (0x0010u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A_2          (0x0020u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A_3          (0x0030u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A_4          (0x0040u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A_5          (0x0050u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A_6          (0x0060u)      /* TSM start trigger ACLK divider */
#define ESIDIV3A_7          (0x0070u)      /* TSM start trigger ACLK divider */
#define ESIDIV2_0           (0x0000u)      /* ACLK divider mode: 0 */
#define ESIDIV2_1           (0x0004u)      /* ACLK divider mode: 1 */
#define ESIDIV2_2           (0x0008u)      /* ACLK divider mode: 2 */
#define ESIDIV2_3           (0x000Cu)      /* ACLK divider mode: 3 */
#define ESIDIV2__1          (0x0000u)      /* ACLK divider = /1 */
#define ESIDIV2__2          (0x0004u)      /* ACLK divider = /2 */
#define ESIDIV2__4          (0x0008u)      /* ACLK divider = /4 */
#define ESIDIV2__8          (0x000Cu)      /* ACLK divider = /8 */
#define ESIDIV1_0           (0x0000u)      /* TSM SMCLK/ESIOSC divider mode: 0 */
#define ESIDIV1_1           (0x0001u)      /* TSM SMCLK/ESIOSC divider mode: 1 */
#define ESIDIV1_2           (0x0002u)      /* TSM SMCLK/ESIOSC divider mode: 2 */
#define ESIDIV1_3           (0x0003u)      /* TSM SMCLK/ESIOSC divider mode: 3 */
#define ESIDIV1__1          (0x0000u)      /* TSM SMCLK/ESIOSC divider = /1 */
#define ESIDIV1__2          (0x0001u)      /* TSM SMCLK/ESIOSC divider = /2 */
#define ESIDIV1__4          (0x0002u)      /* TSM SMCLK/ESIOSC divider = /4 */
#define ESIDIV1__8          (0x0003u)      /* TSM SMCLK/ESIOSC divider = /8 */

/* ESIPSM Control Bits */
#define ESICNT2RST          (0x8000u)      /* ESI Counter 2 reset */
#define ESICNT1RST          (0x4000u)      /* ESI Counter 1 reset */
#define ESICNT0RST          (0x2000u)      /* ESI Counter 0 reset */
#define ESITEST4SEL1        (0x0200u)      /* Output signal selection for SIFTEST4 pin */
#define ESITEST4SEL0        (0x0100u)      /* Output signal selection for SIFTEST4 pin */
#define ESIV2SEL            (0x0080u)      /* Source Selection for V2 bit*/
#define ESICNT2EN           (0x0020u)      /* ESICNT2 enable (down counter) */
#define ESICNT1EN           (0x0010u)      /* ESICNT1 enable (up/down counter) */
#define ESICNT0EN           (0x0008u)      /* ESICNT0 enable (up counter) */
#define ESIQ7TRG            (0x0004u)      /* Enabling to use Q7 as trigger for a TSM sequence */
#define ESIQ6EN             (0x0001u)      /* Q6 enable */

/* ESIPSM Control Bits */
#define ESIV2SEL_L          (0x0080u)      /* Source Selection for V2 bit*/
#define ESICNT2EN_L         (0x0020u)      /* ESICNT2 enable (down counter) */
#define ESICNT1EN_L         (0x0010u)      /* ESICNT1 enable (up/down counter) */
#define ESICNT0EN_L         (0x0008u)      /* ESICNT0 enable (up counter) */
#define ESIQ7TRG_L          (0x0004u)      /* Enabling to use Q7 as trigger for a TSM sequence */
#define ESIQ6EN_L           (0x0001u)      /* Q6 enable */

/* ESIPSM Control Bits */
#define ESICNT2RST_H        (0x0080u)      /* ESI Counter 2 reset */
#define ESICNT1RST_H        (0x0040u)      /* ESI Counter 1 reset */
#define ESICNT0RST_H        (0x0020u)      /* ESI Counter 0 reset */
#define ESITEST4SEL1_H      (0x0002u)      /* Output signal selection for SIFTEST4 pin */
#define ESITEST4SEL0_H      (0x0001u)      /* Output signal selection for SIFTEST4 pin */

#define ESITEST4SEL_0       (0x0000u)      /* Q1 signal from PSM table */
#define ESITEST4SEL_1       (0x0100u)      /* Q2 signal from PSM table */
#define ESITEST4SEL_2       (0x0200u)      /* TSM clock signal from Timing State Machine */
#define ESITEST4SEL_3       (0x0300u)      /* AFE1's comparator output signal Comp1Out */

/* ESIOSC Control Bits */
#define ESICLKFQ5           (0x2000u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ4           (0x1000u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ3           (0x0800u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ2           (0x0400u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ1           (0x0200u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ0           (0x0100u)      /* Internal oscillator frequency adjust */
#define ESICLKGON           (0x0002u)      /* Internal oscillator control */
#define ESIHFSEL            (0x0001u)      /* Internal oscillator enable */

/* ESIOSC Control Bits */
#define ESICLKGON_L         (0x0002u)      /* Internal oscillator control */
#define ESIHFSEL_L          (0x0001u)      /* Internal oscillator enable */

/* ESIOSC Control Bits */
#define ESICLKFQ5_H         (0x0020u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ4_H         (0x0010u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ3_H         (0x0008u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ2_H         (0x0004u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ1_H         (0x0002u)      /* Internal oscillator frequency adjust */
#define ESICLKFQ0_H         (0x0001u)      /* Internal oscillator frequency adjust */

/* ESICTL Control Bits */
#define ESIS3SEL2           (0x8000u)      /* PPUS3 source select */
#define ESIS3SEL1           (0x4000u)      /* PPUS3 source select */
#define ESIS3SEL0           (0x2000u)      /* PPUS3 source select */
#define ESIS2SEL2           (0x1000u)      /* PPUS2 source select */
#define ESIS2SEL1           (0x0800u)      /* PPUS2 source select */
#define ESIS2SEL0           (0x0400u)      /* PPUS2 source select */
#define ESIS1SEL2           (0x0200u)      /* PPUS1 source select */
#define ESIS1SEL1           (0x0100u)      /* PPUS1 source select */
#define ESIS1SEL0           (0x0080u)      /* PPUS1 source select */
#define ESITCH11            (0x0040u)      /* select the comparator input for test channel 1 */
#define ESITCH10            (0x0020u)      /* select the comparator input for test channel 1 */
#define ESITCH01            (0x0010u)      /* select the comparator input for test channel 0 */
#define ESITCH00            (0x0008u)      /* select the comparator input for test channel 0 */
#define ESICS               (0x0004u)      /* Comparator output/Timer_A input selection */
#define ESITESTD            (0x0002u)      /* Test cycle insertion */
#define ESIEN               (0x0001u)      /* Extended Scan interface enable */

/* ESICTL Control Bits */
#define ESIS1SEL0_L         (0x0080u)      /* PPUS1 source select */
#define ESITCH11_L          (0x0040u)      /* select the comparator input for test channel 1 */
#define ESITCH10_L          (0x0020u)      /* select the comparator input for test channel 1 */
#define ESITCH01_L          (0x0010u)      /* select the comparator input for test channel 0 */
#define ESITCH00_L          (0x0008u)      /* select the comparator input for test channel 0 */
#define ESICS_L             (0x0004u)      /* Comparator output/Timer_A input selection */
#define ESITESTD_L          (0x0002u)      /* Test cycle insertion */
#define ESIEN_L             (0x0001u)      /* Extended Scan interface enable */

/* ESICTL Control Bits */
#define ESIS3SEL2_H         (0x0080u)      /* PPUS3 source select */
#define ESIS3SEL1_H         (0x0040u)      /* PPUS3 source select */
#define ESIS3SEL0_H         (0x0020u)      /* PPUS3 source select */
#define ESIS2SEL2_H         (0x0010u)      /* PPUS2 source select */
#define ESIS2SEL1_H         (0x0008u)      /* PPUS2 source select */
#define ESIS2SEL0_H         (0x0004u)      /* PPUS2 source select */
#define ESIS1SEL2_H         (0x0002u)      /* PPUS1 source select */
#define ESIS1SEL1_H         (0x0001u)      /* PPUS1 source select */

#define ESIS3SEL_0          (0x0000u)      /* ESIOUT0 is the PPUS3 source */
#define ESIS3SEL_1          (0x2000u)      /* ESIOUT1 is the PPUS3 source */
#define ESIS3SEL_2          (0x4000u)      /* ESIOUT2 is the PPUS3 source */
#define ESIS3SEL_3          (0x6000u)      /* ESIOUT3 is the PPUS3 source */
#define ESIS3SEL_4          (0x8000u)      /* ESIOUT4 is the PPUS3 source */
#define ESIS3SEL_5          (0xA000u)      /* ESIOUT5 is the PPUS3 source */
#define ESIS3SEL_6          (0xC000u)      /* ESIOUT6 is the PPUS3 source */
#define ESIS3SEL_7          (0xE000u)      /* ESIOUT7 is the PPUS3 source */
#define ESIS2SEL_0          (0x0000u)      /* ESIOUT0 is the PPUS2 source */
#define ESIS2SEL_1          (0x0400u)      /* ESIOUT1 is the PPUS2 source */
#define ESIS2SEL_2          (0x0800u)      /* ESIOUT2 is the PPUS2 source */
#define ESIS2SEL_3          (0x0C00u)      /* ESIOUT3 is the PPUS2 source */
#define ESIS2SEL_4          (0x1000u)      /* ESIOUT4 is the PPUS2 source */
#define ESIS2SEL_5          (0x1400u)      /* ESIOUT5 is the PPUS2 source */
#define ESIS2SEL_6          (0x1800u)      /* ESIOUT6 is the PPUS2 source */
#define ESIS2SEL_7          (0x1C00u)      /* ESIOUT7 is the PPUS2 source */
#define ESIS1SEL_0          (0x0000u)      /* ESIOUT0 is the PPUS1 source */
#define ESIS1SEL_1          (0x0080u)      /* ESIOUT1 is the PPUS1 source */
#define ESIS1SEL_2          (0x0100u)      /* ESIOUT2 is the PPUS1 source */
#define ESIS1SEL_3          (0x0180u)      /* ESIOUT3 is the PPUS1 source */
#define ESIS1SEL_4          (0x0200u)      /* ESIOUT4 is the PPUS1 source */
#define ESIS1SEL_5          (0x0280u)      /* ESIOUT5 is the PPUS1 source */
#define ESIS1SEL_6          (0x0300u)      /* ESIOUT6 is the PPUS1 source */
#define ESIS1SEL_7          (0x0380u)      /* ESIOUT7 is the PPUS1 source */
#define ESITCH1_0           (0x0000u)      /* Comparator input is ESICH0 when ESICAX = 0; Comparator input is ESICI0 when ESICAX = 1 */
#define ESITCH1_1           (0x0400u)      /* Comparator input is ESICH1 when ESICAX = 0; Comparator input is ESICI1 when ESICAX = 1 */
#define ESITCH1_2           (0x0800u)      /* Comparator input is ESICH2 when ESICAX = 0; Comparator input is ESICI2 when ESICAX = 1 */
#define ESITCH1_3           (0x0C00u)      /* Comparator input is ESICH3 when ESICAX = 0; Comparator input is ESICI3 when ESICAX = 1 */
#define ESITCH0_0           (0x0000u)      /* Comparator input is ESICH0 when ESICAX = 0; Comparator input is ESICI0 when ESICAX = 1 */
#define ESITCH0_1           (0x0008u)      /* Comparator input is ESICH1 when ESICAX = 0; Comparator input is ESICI1 when ESICAX = 1 */
#define ESITCH0_2           (0x0010u)      /* Comparator input is ESICH2 when ESICAX = 0; Comparator input is ESICI2 when ESICAX = 1 */
#define ESITCH0_3           (0x0018u)      /* Comparator input is ESICH3 when ESICAX = 0; Comparator input is ESICI3 when ESICAX = 1 */

/* Timing State Machine Control Bits */
#define ESIREPEAT4          (0x8000u)      /* These bits together with the ESICLK bit configure the duration of this state */
#define ESIREPEAT3          (0x4000u)      /* ESIREPEATx selects the number of clock cycles for this state. The number of clock cycles = ESIREPEATx + 1 */
#define ESIREPEAT2          (0x2000u)      /* */
#define ESIREPEAT1          (0x1000u)      /* */
#define ESIREPEAT0          (0x0800u)      /* */
#define ESICLK              (0x0400u)      /* This bit selects the clock source for the TSM */
#define ESISTOP             (0x0200u)      /* This bit indicates the end of the TSM sequence */
#define ESIDAC              (0x0100u)      /* TSM DAC on */
#define ESITESTS1           (0x0080u)      /* TSM test cycle control */
#define ESIRSON             (0x0040u)      /* Internal output latches enabled */
#define ESICLKON            (0x0020u)      /* High-frequency clock on */
#define ESICA               (0x0010u)      /* TSM comparator on */
#define ESIEX               (0x0008u)      /* Excitation and sample-and-hold */
#define ESILCEN             (0x0004u)      /* LC enable */
#define ESICH1              (0x0002u)      /* Input channel select */
#define ESICH0              (0x0001u)      /* Input channel select */

/* Timing State Machine Control Bits */
#define ESITESTS1_L         (0x0080u)      /* TSM test cycle control */
#define ESIRSON_L           (0x0040u)      /* Internal output latches enabled */
#define ESICLKON_L          (0x0020u)      /* High-frequency clock on */
#define ESICA_L             (0x0010u)      /* TSM comparator on */
#define ESIEX_L             (0x0008u)      /* Excitation and sample-and-hold */
#define ESILCEN_L           (0x0004u)      /* LC enable */
#define ESICH1_L            (0x0002u)      /* Input channel select */
#define ESICH0_L            (0x0001u)      /* Input channel select */

/* Timing State Machine Control Bits */
#define ESIREPEAT4_H        (0x0080u)      /* These bits together with the ESICLK bit configure the duration of this state */
#define ESIREPEAT3_H        (0x0040u)      /* ESIREPEATx selects the number of clock cycles for this state. The number of clock cycles = ESIREPEATx + 1 */
#define ESIREPEAT2_H        (0x0020u)      /* */
#define ESIREPEAT1_H        (0x0010u)      /* */
#define ESIREPEAT0_H        (0x0008u)      /* */
#define ESICLK_H            (0x0004u)      /* This bit selects the clock source for the TSM */
#define ESISTOP_H           (0x0002u)      /* This bit indicates the end of the TSM sequence */
#define ESIDAC_H            (0x0001u)      /* TSM DAC on */

#define ESICAAZ             (0x0020u)      /* Comparator Offset calibration annulation */

#define ESIREPEAT_0         (0x0000u)      /* These bits configure the duration of this state */
#define ESIREPEAT_1         (0x0800u)      /* ESIREPEATx selects the number of clock cycles for this state. The number of clock cycles = ESIREPEATx + 1 */
#define ESIREPEAT_2         (0x1000u)
#define ESIREPEAT_3         (0x1800u)
#define ESIREPEAT_4         (0x2000u)
#define ESIREPEAT_5         (0x2800u)
#define ESIREPEAT_6         (0x3000u)
#define ESIREPEAT_7         (0x3800u)
#define ESIREPEAT_8         (0x4000u)
#define ESIREPEAT_9         (0x4800u)
#define ESIREPEAT_10        (0x5000u)
#define ESIREPEAT_11        (0x5800u)
#define ESIREPEAT_12        (0x6000u)
#define ESIREPEAT_13        (0x6800u)
#define ESIREPEAT_14        (0x7000u)
#define ESIREPEAT_15        (0x7800u)
#define ESIREPEAT_16        (0x8000u)
#define ESIREPEAT_17        (0x8800u)
#define ESIREPEAT_18        (0x9000u)
#define ESIREPEAT_19        (0x9800u)
#define ESIREPEAT_20        (0xA000u)
#define ESIREPEAT_21        (0xA800u)
#define ESIREPEAT_22        (0xB000u)
#define ESIREPEAT_23        (0xB800u)
#define ESIREPEAT_24        (0xC000u)
#define ESIREPEAT_25        (0xC800u)
#define ESIREPEAT_26        (0xD000u)
#define ESIREPEAT_27        (0xD800u)
#define ESIREPEAT_28        (0xE000u)
#define ESIREPEAT_29        (0xE800u)
#define ESIREPEAT_30        (0xF000u)
#define ESIREPEAT_31        (0xF800u)
#define ESICH_0             (0x0000u)      /* Input channel select: ESICH0 */
#define ESICH_1             (0x0001u)      /* Input channel select: ESICH1 */
#define ESICH_2             (0x0002u)      /* Input channel select: ESICH2 */
#define ESICH_3             (0x0003u)      /* Input channel select: ESICH3 */
/************************************************************
* EXTENDED SCAN INTERFACE RAM
************************************************************/
#define __MSP430_HAS_ESI_RAM__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_ESI_RAM__ 0x0E00
#define ESI_RAM_BASE __MSP430_BASEADDRESS_ESI_RAM__

#define ESIRAM0_                    (0x0E00u)      /* ESI RAM 0 */
DEFC(   ESIRAM0                   , ESIRAM0_)
#define ESIRAM1_                    (0x0E01u)      /* ESI RAM 1 */
DEFC(   ESIRAM1                   , ESIRAM1_)
#define ESIRAM2_                    (0x0E02u)      /* ESI RAM 2 */
DEFC(   ESIRAM2                   , ESIRAM2_)
#define ESIRAM3_                    (0x0E03u)      /* ESI RAM 3 */
DEFC(   ESIRAM3                   , ESIRAM3_)
#define ESIRAM4_                    (0x0E04u)      /* ESI RAM 4 */
DEFC(   ESIRAM4                   , ESIRAM4_)
#define ESIRAM5_                    (0x0E05u)      /* ESI RAM 5 */
DEFC(   ESIRAM5                   , ESIRAM5_)
#define ESIRAM6_                    (0x0E06u)      /* ESI RAM 6 */
DEFC(   ESIRAM6                   , ESIRAM6_)
#define ESIRAM7_                    (0x0E07u)      /* ESI RAM 7 */
DEFC(   ESIRAM7                   , ESIRAM7_)
#define ESIRAM8_                    (0x0E08u)      /* ESI RAM 8 */
DEFC(   ESIRAM8                   , ESIRAM8_)
#define ESIRAM9_                    (0x0E09u)      /* ESI RAM 9 */
DEFC(   ESIRAM9                   , ESIRAM9_)
#define ESIRAM10_                    (0x0E0Au)      /* ESI RAM 10 */
DEFC(   ESIRAM10                   , ESIRAM10_)
#define ESIRAM11_                    (0x0E0Bu)      /* ESI RAM 11 */
DEFC(   ESIRAM11                   , ESIRAM11_)
#define ESIRAM12_                    (0x0E0Cu)      /* ESI RAM 12 */
DEFC(   ESIRAM12                   , ESIRAM12_)
#define ESIRAM13_                    (0x0E0Du)      /* ESI RAM 13 */
DEFC(   ESIRAM13                   , ESIRAM13_)
#define ESIRAM14_                    (0x0E0Eu)      /* ESI RAM 14 */
DEFC(   ESIRAM14                   , ESIRAM14_)
#define ESIRAM15_                    (0x0E0Fu)      /* ESI RAM 15 */
DEFC(   ESIRAM15                   , ESIRAM15_)
#define ESIRAM16_                    (0x0E10u)      /* ESI RAM 16 */
DEFC(   ESIRAM16                   , ESIRAM16_)
#define ESIRAM17_                    (0x0E11u)      /* ESI RAM 17 */
DEFC(   ESIRAM17                   , ESIRAM17_)
#define ESIRAM18_                    (0x0E12u)      /* ESI RAM 18 */
DEFC(   ESIRAM18                   , ESIRAM18_)
#define ESIRAM19_                    (0x0E13u)      /* ESI RAM 19 */
DEFC(   ESIRAM19                   , ESIRAM19_)
#define ESIRAM20_                    (0x0E14u)      /* ESI RAM 20 */
DEFC(   ESIRAM20                   , ESIRAM20_)
#define ESIRAM21_                    (0x0E15u)      /* ESI RAM 21 */
DEFC(   ESIRAM21                   , ESIRAM21_)
#define ESIRAM22_                    (0x0E16u)      /* ESI RAM 22 */
DEFC(   ESIRAM22                   , ESIRAM22_)
#define ESIRAM23_                    (0x0E17u)      /* ESI RAM 23 */
DEFC(   ESIRAM23                   , ESIRAM23_)
#define ESIRAM24_                    (0x0E18u)      /* ESI RAM 24 */
DEFC(   ESIRAM24                   , ESIRAM24_)
#define ESIRAM25_                    (0x0E19u)      /* ESI RAM 25 */
DEFC(   ESIRAM25                   , ESIRAM25_)
#define ESIRAM26_                    (0x0E1Au)      /* ESI RAM 26 */
DEFC(   ESIRAM26                   , ESIRAM26_)
#define ESIRAM27_                    (0x0E1Bu)      /* ESI RAM 27 */
DEFC(   ESIRAM27                   , ESIRAM27_)
#define ESIRAM28_                    (0x0E1Cu)      /* ESI RAM 28 */
DEFC(   ESIRAM28                   , ESIRAM28_)
#define ESIRAM29_                    (0x0E1Du)      /* ESI RAM 29 */
DEFC(   ESIRAM29                   , ESIRAM29_)
#define ESIRAM30_                    (0x0E1Eu)      /* ESI RAM 30 */
DEFC(   ESIRAM30                   , ESIRAM30_)
#define ESIRAM31_                    (0x0E1Fu)      /* ESI RAM 31 */
DEFC(   ESIRAM31                   , ESIRAM31_)
#define ESIRAM32_                    (0x0E20u)      /* ESI RAM 32 */
DEFC(   ESIRAM32                   , ESIRAM32_)
#define ESIRAM33_                    (0x0E21u)      /* ESI RAM 33 */
DEFC(   ESIRAM33                   , ESIRAM33_)
#define ESIRAM34_                    (0x0E22u)      /* ESI RAM 34 */
DEFC(   ESIRAM34                   , ESIRAM34_)
#define ESIRAM35_                    (0x0E23u)      /* ESI RAM 35 */
DEFC(   ESIRAM35                   , ESIRAM35_)
#define ESIRAM36_                    (0x0E24u)      /* ESI RAM 36 */
DEFC(   ESIRAM36                   , ESIRAM36_)
#define ESIRAM37_                    (0x0E25u)      /* ESI RAM 37 */
DEFC(   ESIRAM37                   , ESIRAM37_)
#define ESIRAM38_                    (0x0E26u)      /* ESI RAM 38 */
DEFC(   ESIRAM38                   , ESIRAM38_)
#define ESIRAM39_                    (0x0E27u)      /* ESI RAM 39 */
DEFC(   ESIRAM39                   , ESIRAM39_)
#define ESIRAM40_                    (0x0E28u)      /* ESI RAM 40 */
DEFC(   ESIRAM40                   , ESIRAM40_)
#define ESIRAM41_                    (0x0E29u)      /* ESI RAM 41 */
DEFC(   ESIRAM41                   , ESIRAM41_)
#define ESIRAM42_                    (0x0E2Au)      /* ESI RAM 42 */
DEFC(   ESIRAM42                   , ESIRAM42_)
#define ESIRAM43_                    (0x0E2Bu)      /* ESI RAM 43 */
DEFC(   ESIRAM43                   , ESIRAM43_)
#define ESIRAM44_                    (0x0E2Cu)      /* ESI RAM 44 */
DEFC(   ESIRAM44                   , ESIRAM44_)
#define ESIRAM45_                    (0x0E2Du)      /* ESI RAM 45 */
DEFC(   ESIRAM45                   , ESIRAM45_)
#define ESIRAM46_                    (0x0E2Eu)      /* ESI RAM 46 */
DEFC(   ESIRAM46                   , ESIRAM46_)
#define ESIRAM47_                    (0x0E2Fu)      /* ESI RAM 47 */
DEFC(   ESIRAM47                   , ESIRAM47_)
#define ESIRAM48_                    (0x0E30u)      /* ESI RAM 48 */
DEFC(   ESIRAM48                   , ESIRAM48_)
#define ESIRAM49_                    (0x0E31u)      /* ESI RAM 49 */
DEFC(   ESIRAM49                   , ESIRAM49_)
#define ESIRAM50_                    (0x0E32u)      /* ESI RAM 50 */
DEFC(   ESIRAM50                   , ESIRAM50_)
#define ESIRAM51_                    (0x0E33u)      /* ESI RAM 51 */
DEFC(   ESIRAM51                   , ESIRAM51_)
#define ESIRAM52_                    (0x0E34u)      /* ESI RAM 52 */
DEFC(   ESIRAM52                   , ESIRAM52_)
#define ESIRAM53_                    (0x0E35u)      /* ESI RAM 53 */
DEFC(   ESIRAM53                   , ESIRAM53_)
#define ESIRAM54_                    (0x0E36u)      /* ESI RAM 54 */
DEFC(   ESIRAM54                   , ESIRAM54_)
#define ESIRAM55_                    (0x0E37u)      /* ESI RAM 55 */
DEFC(   ESIRAM55                   , ESIRAM55_)
#define ESIRAM56_                    (0x0E38u)      /* ESI RAM 56 */
DEFC(   ESIRAM56                   , ESIRAM56_)
#define ESIRAM57_                    (0x0E39u)      /* ESI RAM 57 */
DEFC(   ESIRAM57                   , ESIRAM57_)
#define ESIRAM58_                    (0x0E3Au)      /* ESI RAM 58 */
DEFC(   ESIRAM58                   , ESIRAM58_)
#define ESIRAM59_                    (0x0E3Bu)      /* ESI RAM 59 */
DEFC(   ESIRAM59                   , ESIRAM59_)
#define ESIRAM60_                    (0x0E3Cu)      /* ESI RAM 60 */
DEFC(   ESIRAM60                   , ESIRAM60_)
#define ESIRAM61_                    (0x0E3Du)      /* ESI RAM 61 */
DEFC(   ESIRAM61                   , ESIRAM61_)
#define ESIRAM62_                    (0x0E3Eu)      /* ESI RAM 62 */
DEFC(   ESIRAM62                   , ESIRAM62_)
#define ESIRAM63_                    (0x0E3Fu)      /* ESI RAM 63 */
DEFC(   ESIRAM63                   , ESIRAM63_)
#define ESIRAM64_                    (0x0E40u)      /* ESI RAM 64 */
DEFC(   ESIRAM64                   , ESIRAM64_)
#define ESIRAM65_                    (0x0E41u)      /* ESI RAM 65 */
DEFC(   ESIRAM65                   , ESIRAM65_)
#define ESIRAM66_                    (0x0E42u)      /* ESI RAM 66 */
DEFC(   ESIRAM66                   , ESIRAM66_)
#define ESIRAM67_                    (0x0E43u)      /* ESI RAM 67 */
DEFC(   ESIRAM67                   , ESIRAM67_)
#define ESIRAM68_                    (0x0E44u)      /* ESI RAM 68 */
DEFC(   ESIRAM68                   , ESIRAM68_)
#define ESIRAM69_                    (0x0E45u)      /* ESI RAM 69 */
DEFC(   ESIRAM69                   , ESIRAM69_)
#define ESIRAM70_                    (0x0E46u)      /* ESI RAM 70 */
DEFC(   ESIRAM70                   , ESIRAM70_)
#define ESIRAM71_                    (0x0E47u)      /* ESI RAM 71 */
DEFC(   ESIRAM71                   , ESIRAM71_)
#define ESIRAM72_                    (0x0E48u)      /* ESI RAM 72 */
DEFC(   ESIRAM72                   , ESIRAM72_)
#define ESIRAM73_                    (0x0E49u)      /* ESI RAM 73 */
DEFC(   ESIRAM73                   , ESIRAM73_)
#define ESIRAM74_                    (0x0E4Au)      /* ESI RAM 74 */
DEFC(   ESIRAM74                   , ESIRAM74_)
#define ESIRAM75_                    (0x0E4Bu)      /* ESI RAM 75 */
DEFC(   ESIRAM75                   , ESIRAM75_)
#define ESIRAM76_                    (0x0E4Cu)      /* ESI RAM 76 */
DEFC(   ESIRAM76                   , ESIRAM76_)
#define ESIRAM77_                    (0x0E4Du)      /* ESI RAM 77 */
DEFC(   ESIRAM77                   , ESIRAM77_)
#define ESIRAM78_                    (0x0E4Eu)      /* ESI RAM 78 */
DEFC(   ESIRAM78                   , ESIRAM78_)
#define ESIRAM79_                    (0x0E4Fu)      /* ESI RAM 79 */
DEFC(   ESIRAM79                   , ESIRAM79_)
#define ESIRAM80_                    (0x0E50u)      /* ESI RAM 80 */
DEFC(   ESIRAM80                   , ESIRAM80_)
#define ESIRAM81_                    (0x0E51u)      /* ESI RAM 81 */
DEFC(   ESIRAM81                   , ESIRAM81_)
#define ESIRAM82_                    (0x0E52u)      /* ESI RAM 82 */
DEFC(   ESIRAM82                   , ESIRAM82_)
#define ESIRAM83_                    (0x0E53u)      /* ESI RAM 83 */
DEFC(   ESIRAM83                   , ESIRAM83_)
#define ESIRAM84_                    (0x0E54u)      /* ESI RAM 84 */
DEFC(   ESIRAM84                   , ESIRAM84_)
#define ESIRAM85_                    (0x0E55u)      /* ESI RAM 85 */
DEFC(   ESIRAM85                   , ESIRAM85_)
#define ESIRAM86_                    (0x0E56u)      /* ESI RAM 86 */
DEFC(   ESIRAM86                   , ESIRAM86_)
#define ESIRAM87_                    (0x0E57u)      /* ESI RAM 87 */
DEFC(   ESIRAM87                   , ESIRAM87_)
#define ESIRAM88_                    (0x0E58u)      /* ESI RAM 88 */
DEFC(   ESIRAM88                   , ESIRAM88_)
#define ESIRAM89_                    (0x0E59u)      /* ESI RAM 89 */
DEFC(   ESIRAM89                   , ESIRAM89_)
#define ESIRAM90_                    (0x0E5Au)      /* ESI RAM 90 */
DEFC(   ESIRAM90                   , ESIRAM90_)
#define ESIRAM91_                    (0x0E5Bu)      /* ESI RAM 91 */
DEFC(   ESIRAM91                   , ESIRAM91_)
#define ESIRAM92_                    (0x0E5Cu)      /* ESI RAM 92 */
DEFC(   ESIRAM92                   , ESIRAM92_)
#define ESIRAM93_                    (0x0E5Du)      /* ESI RAM 93 */
DEFC(   ESIRAM93                   , ESIRAM93_)
#define ESIRAM94_                    (0x0E5Eu)      /* ESI RAM 94 */
DEFC(   ESIRAM94                   , ESIRAM94_)
#define ESIRAM95_                    (0x0E5Fu)      /* ESI RAM 95 */
DEFC(   ESIRAM95                   , ESIRAM95_)
#define ESIRAM96_                    (0x0E60u)      /* ESI RAM 96 */
DEFC(   ESIRAM96                   , ESIRAM96_)
#define ESIRAM97_                    (0x0E61u)      /* ESI RAM 97 */
DEFC(   ESIRAM97                   , ESIRAM97_)
#define ESIRAM98_                    (0x0E62u)      /* ESI RAM 98 */
DEFC(   ESIRAM98                   , ESIRAM98_)
#define ESIRAM99_                    (0x0E63u)      /* ESI RAM 99 */
DEFC(   ESIRAM99                   , ESIRAM99_)
#define ESIRAM100_                    (0x0E64u)      /* ESI RAM 100 */
DEFC(   ESIRAM100                   , ESIRAM100_)
#define ESIRAM101_                    (0x0E65u)      /* ESI RAM 101 */
DEFC(   ESIRAM101                   , ESIRAM101_)
#define ESIRAM102_                    (0x0E66u)      /* ESI RAM 102 */
DEFC(   ESIRAM102                   , ESIRAM102_)
#define ESIRAM103_                    (0x0E67u)      /* ESI RAM 103 */
DEFC(   ESIRAM103                   , ESIRAM103_)
#define ESIRAM104_                    (0x0E68u)      /* ESI RAM 104 */
DEFC(   ESIRAM104                   , ESIRAM104_)
#define ESIRAM105_                    (0x0E69u)      /* ESI RAM 105 */
DEFC(   ESIRAM105                   , ESIRAM105_)
#define ESIRAM106_                    (0x0E6Au)      /* ESI RAM 106 */
DEFC(   ESIRAM106                   , ESIRAM106_)
#define ESIRAM107_                    (0x0E6Bu)      /* ESI RAM 107 */
DEFC(   ESIRAM107                   , ESIRAM107_)
#define ESIRAM108_                    (0x0E6Cu)      /* ESI RAM 108 */
DEFC(   ESIRAM108                   , ESIRAM108_)
#define ESIRAM109_                    (0x0E6Du)      /* ESI RAM 109 */
DEFC(   ESIRAM109                   , ESIRAM109_)
#define ESIRAM110_                    (0x0E6Eu)      /* ESI RAM 110 */
DEFC(   ESIRAM110                   , ESIRAM110_)
#define ESIRAM111_                    (0x0E6Fu)      /* ESI RAM 111 */
DEFC(   ESIRAM111                   , ESIRAM111_)
#define ESIRAM112_                    (0x0E70u)      /* ESI RAM 112 */
DEFC(   ESIRAM112                   , ESIRAM112_)
#define ESIRAM113_                    (0x0E71u)      /* ESI RAM 113 */
DEFC(   ESIRAM113                   , ESIRAM113_)
#define ESIRAM114_                    (0x0E72u)      /* ESI RAM 114 */
DEFC(   ESIRAM114                   , ESIRAM114_)
#define ESIRAM115_                    (0x0E73u)      /* ESI RAM 115 */
DEFC(   ESIRAM115                   , ESIRAM115_)
#define ESIRAM116_                    (0x0E74u)      /* ESI RAM 116 */
DEFC(   ESIRAM116                   , ESIRAM116_)
#define ESIRAM117_                    (0x0E75u)      /* ESI RAM 117 */
DEFC(   ESIRAM117                   , ESIRAM117_)
#define ESIRAM118_                    (0x0E76u)      /* ESI RAM 118 */
DEFC(   ESIRAM118                   , ESIRAM118_)
#define ESIRAM119_                    (0x0E77u)      /* ESI RAM 119 */
DEFC(   ESIRAM119                   , ESIRAM119_)
#define ESIRAM120_                    (0x0E78u)      /* ESI RAM 120 */
DEFC(   ESIRAM120                   , ESIRAM120_)
#define ESIRAM121_                    (0x0E79u)      /* ESI RAM 121 */
DEFC(   ESIRAM121                   , ESIRAM121_)
#define ESIRAM122_                    (0x0E7Au)      /* ESI RAM 122 */
DEFC(   ESIRAM122                   , ESIRAM122_)
#define ESIRAM123_                    (0x0E7Bu)      /* ESI RAM 123 */
DEFC(   ESIRAM123                   , ESIRAM123_)
#define ESIRAM124_                    (0x0E7Cu)      /* ESI RAM 124 */
DEFC(   ESIRAM124                   , ESIRAM124_)
#define ESIRAM125_                    (0x0E7Du)      /* ESI RAM 125 */
DEFC(   ESIRAM125                   , ESIRAM125_)
#define ESIRAM126_                    (0x0E7Eu)      /* ESI RAM 126 */
DEFC(   ESIRAM126                   , ESIRAM126_)
#define ESIRAM127_                    (0x0E7Fu)      /* ESI RAM 127 */
DEFC(   ESIRAM127                   , ESIRAM127_)
/*************************************************************
* FRAM Memory
*************************************************************/
#define __MSP430_HAS_FRAM__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_FRAM__ 0x0140
#define FRAM_BASE __MSP430_BASEADDRESS_FRAM__
#define __MSP430_HAS_GC__            /* Definition to show that Module is available */

#define  FRCTL0_               (0x0140u)  /* FRAM Controller Control 0 */
DEFCW(   FRCTL0              , FRCTL0_)
#define  GCCTL0_               (0x0144u)  /* General Control 0 */
DEFCW(   GCCTL0              , GCCTL0_)
#define  GCCTL1_               (0x0146u)  /* General Control 1 */
DEFCW(   GCCTL1              , GCCTL1_)

#define FRCTLPW             (0xA500u)  /* FRAM password for write */
#define FRPW                (0x9600u)  /* FRAM password returned by read */
#define FWPW                (0xA500u)  /* FRAM password for write */
#define FXPW                (0x3300u)  /* for use with XOR instruction */

/* FRCTL0 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
//#define RESERVED            (0x0002u)  /* RESERVED */
//#define RESERVED            (0x0004u)  /* RESERVED */
#define NWAITS0             (0x0010u)  /* FRAM Wait state control Bit: 0 */
#define NWAITS1             (0x0020u)  /* FRAM Wait state control Bit: 1 */
#define NWAITS2             (0x0040u)  /* FRAM Wait state control Bit: 2 */
//#define RESERVED            (0x0080u)  /* RESERVED */

/* FRCTL0 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
//#define RESERVED            (0x0002u)  /* RESERVED */
//#define RESERVED            (0x0004u)  /* RESERVED */
#define NWAITS0_L           (0x0010u)  /* FRAM Wait state control Bit: 0 */
#define NWAITS1_L           (0x0020u)  /* FRAM Wait state control Bit: 1 */
#define NWAITS2_L           (0x0040u)  /* FRAM Wait state control Bit: 2 */
//#define RESERVED            (0x0080u)  /* RESERVED */

#define NWAITS_0            (0x0000u)  /* FRAM Wait state control: 0 */
#define NWAITS_1            (0x0010u)  /* FRAM Wait state control: 1 */
#define NWAITS_2            (0x0020u)  /* FRAM Wait state control: 2 */
#define NWAITS_3            (0x0030u)  /* FRAM Wait state control: 3 */
#define NWAITS_4            (0x0040u)  /* FRAM Wait state control: 4 */
#define NWAITS_5            (0x0050u)  /* FRAM Wait state control: 5 */
#define NWAITS_6            (0x0060u)  /* FRAM Wait state control: 6 */
#define NWAITS_7            (0x0070u)  /* FRAM Wait state control: 7 */

/* GCCTL0 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
#define FRLPMPWR            (0x0002u)  /* FRAM Enable FRAM auto power up after LPM */
#define FRPWR               (0x0004u)  /* FRAM Power Control */
#define ACCTEIE             (0x0008u)  /* Enable NMI event if Access time error occurs */
//#define RESERVED            (0x0010u)  /* RESERVED */
#define CBDIE               (0x0020u)  /* Enable NMI event if correctable bit error detected */
#define UBDIE               (0x0040u)  /* Enable NMI event if uncorrectable bit error detected */
#define UBDRSTEN            (0x0080u)  /* Enable Power Up Clear (PUC) reset if FRAM uncorrectable bit error detected */

/* GCCTL0 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
#define FRLPMPWR_L          (0x0002u)  /* FRAM Enable FRAM auto power up after LPM */
#define FRPWR_L             (0x0004u)  /* FRAM Power Control */
#define ACCTEIE_L           (0x0008u)  /* Enable NMI event if Access time error occurs */
//#define RESERVED            (0x0010u)  /* RESERVED */
#define CBDIE_L             (0x0020u)  /* Enable NMI event if correctable bit error detected */
#define UBDIE_L             (0x0040u)  /* Enable NMI event if uncorrectable bit error detected */
#define UBDRSTEN_L          (0x0080u)  /* Enable Power Up Clear (PUC) reset if FRAM uncorrectable bit error detected */

/* GCCTL1 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
#define CBDIFG              (0x0002u)  /* FRAM correctable bit error flag */
#define UBDIFG              (0x0004u)  /* FRAM uncorrectable bit error flag */
#define ACCTEIFG            (0x0008u)  /* Access time error flag */

/* GCCTL1 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
#define CBDIFG_L            (0x0002u)  /* FRAM correctable bit error flag */
#define UBDIFG_L            (0x0004u)  /* FRAM uncorrectable bit error flag */
#define ACCTEIFG_L          (0x0008u)  /* Access time error flag */

/************************************************************
* Memory Protection Unit
************************************************************/
#define __MSP430_HAS_MPU__             /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_MPU__ 0x05A0
#define MPU_BASE __MSP430_BASEADDRESS_MPU__

#define  MPUCTL0_               (0x05A0u)  /* MPU Control Register 0 */
DEFCW(   MPUCTL0              , MPUCTL0_)
#define  MPUCTL1_               (0x05A2u)  /* MPU Control Register 1 */
DEFCW(   MPUCTL1              , MPUCTL1_)
#define  MPUSEGB2_              (0x05A4u)  /* MPU Segmentation Border 2 Register */
DEFCW(   MPUSEGB2             , MPUSEGB2_)
#define  MPUSEGB1_              (0x05A6u)  /* MPU Segmentation Border 1 Register */
DEFCW(   MPUSEGB1             , MPUSEGB1_)
#define  MPUSAM_                (0x05A8u)  /* MPU Access Management Register */
DEFCW(   MPUSAM               , MPUSAM_)
#define  MPUIPC0_               (0x05AAu)  /* MPU IP Control 0 Register */
DEFCW(   MPUIPC0              , MPUIPC0_)
#define  MPUIPSEGB2_            (0x05ACu)  /* MPU IP Segment Border 2 Register */
DEFCW(   MPUIPSEGB2           , MPUIPSEGB2_)
#define  MPUIPSEGB1_            (0x05AEu)  /* MPU IP Segment Border 1 Register */
DEFCW(   MPUIPSEGB1           , MPUIPSEGB1_)

/* MPUCTL0 Control Bits */
#define MPUENA               (0x0001u)  /* MPU Enable */
#define MPULOCK              (0x0002u)  /* MPU Lock */
#define MPUSEGIE             (0x0010u)  /* MPU Enable NMI on Segment violation */

/* MPUCTL0 Control Bits */
#define MPUENA_L            (0x0001u)  /* MPU Enable */
#define MPULOCK_L           (0x0002u)  /* MPU Lock */
#define MPUSEGIE_L          (0x0010u)  /* MPU Enable NMI on Segment violation */

#define MPUPW                (0xA500u)  /* MPU Access Password */
#define MPUPW_H              (0xA5)    /* MPU Access Password */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG           (0x0001u)  /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG           (0x0002u)  /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG           (0x0004u)  /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG           (0x0008u)  /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG          (0x0010u)  /* MPU IP Memory Segment violation interupt flag */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG_L        (0x0001u)  /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG_L        (0x0002u)  /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG_L        (0x0004u)  /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG_L        (0x0008u)  /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG_L       (0x0010u)  /* MPU IP Memory Segment violation interupt flag */

/* MPUSEGB2 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSAM Control Bits */
#define MPUSEG1RE            (0x0001u)  /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE            (0x0002u)  /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE            (0x0004u)  /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS            (0x0008u)  /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE            (0x0010u)  /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE            (0x0020u)  /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE            (0x0040u)  /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS            (0x0080u)  /* MPU Main memory Segment 2 Violation select */
#define MPUSEG3RE            (0x0100u)  /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE            (0x0200u)  /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE            (0x0400u)  /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS            (0x0800u)  /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE            (0x1000u)  /* MPU Info memory Segment Read enable */
#define MPUSEGIWE            (0x2000u)  /* MPU Info memory Segment Write enable */
#define MPUSEGIXE            (0x4000u)  /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS            (0x8000u)  /* MPU Info memory Segment Violation select */

/* MPUSAM Control Bits */
#define MPUSEG1RE_L         (0x0001u)  /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE_L         (0x0002u)  /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE_L         (0x0004u)  /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS_L         (0x0008u)  /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE_L         (0x0010u)  /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE_L         (0x0020u)  /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE_L         (0x0040u)  /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS_L         (0x0080u)  /* MPU Main memory Segment 2 Violation select */

/* MPUSAM Control Bits */
#define MPUSEG3RE_H         (0x0001u)  /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE_H         (0x0002u)  /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE_H         (0x0004u)  /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS_H         (0x0008u)  /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE_H         (0x0010u)  /* MPU Info memory Segment Read enable */
#define MPUSEGIWE_H         (0x0020u)  /* MPU Info memory Segment Write enable */
#define MPUSEGIXE_H         (0x0040u)  /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS_H         (0x0080u)  /* MPU Info memory Segment Violation select */

/* MPUIPC0 Control Bits */
#define MPUIPVS              (0x0020u)  /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA             (0x0040u)  /* MPU MPU IP Protection Enable */
#define MPUIPLOCK            (0x0080u)  /* MPU IP Protection Lock */

/* MPUIPC0 Control Bits */
#define MPUIPVS_L           (0x0020u)  /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA_L          (0x0040u)  /* MPU MPU IP Protection Enable */
#define MPUIPLOCK_L         (0x0080u)  /* MPU IP Protection Lock */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB1 Control Bits */

/* MPUIPSEGB1 Control Bits */

/* MPUIPSEGB1 Control Bits */

/************************************************************
* HARDWARE MULTIPLIER 32Bit
************************************************************/
#define __MSP430_HAS_MPY32__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_MPY32__ 0x04C0
#define MPY32_BASE __MSP430_BASEADDRESS_MPY32__

#define  MPY_                   (0x04C0u)  /* Multiply Unsigned/Operand 1 */
DEFCW(   MPY                  , MPY_)
#define  MPYS_                  (0x04C2u)  /* Multiply Signed/Operand 1 */
DEFCW(   MPYS                 , MPYS_)
#define  MAC_                   (0x04C4u)  /* Multiply Unsigned and Accumulate/Operand 1 */
DEFCW(   MAC                  , MAC_)
#define  MACS_                  (0x04C6u)  /* Multiply Signed and Accumulate/Operand 1 */
DEFCW(   MACS                 , MACS_)
#define  OP2_                   (0x04C8u)  /* Operand 2 */
DEFCW(   OP2                  , OP2_)
#define  RESLO_                 (0x04CAu)  /* Result Low Word */
DEFCW(   RESLO                , RESLO_)
#define  RESHI_                 (0x04CCu)  /* Result High Word */
DEFCW(   RESHI                , RESHI_)
#define SUMEXT_                (0x04CEu)  /* Sum Extend */
READ_ONLY_DEFCW( SUMEXT            , SUMEXT_)

#define  MPY32L_                (0x04D0u)  /* 32-bit operand 1 - multiply - low word */
DEFCW(   MPY32L               , MPY32L_)
#define  MPY32H_                (0x04D2u)  /* 32-bit operand 1 - multiply - high word */
DEFCW(   MPY32H               , MPY32H_)
#define  MPYS32L_               (0x04D4u)  /* 32-bit operand 1 - signed multiply - low word */
DEFCW(   MPYS32L              , MPYS32L_)
#define  MPYS32H_               (0x04D6u)  /* 32-bit operand 1 - signed multiply - high word */
DEFCW(   MPYS32H              , MPYS32H_)
#define  MAC32L_                (0x04D8u)  /* 32-bit operand 1 - multiply accumulate - low word */
DEFCW(   MAC32L               , MAC32L_)
#define  MAC32H_                (0x04DAu)  /* 32-bit operand 1 - multiply accumulate - high word */
DEFCW(   MAC32H               , MAC32H_)
#define  MACS32L_               (0x04DCu)  /* 32-bit operand 1 - signed multiply accumulate - low word */
DEFCW(   MACS32L              , MACS32L_)
#define  MACS32H_               (0x04DEu)  /* 32-bit operand 1 - signed multiply accumulate - high word */
DEFCW(   MACS32H              , MACS32H_)
#define  OP2L_                  (0x04E0u)  /* 32-bit operand 2 - low word */
DEFCW(   OP2L                 , OP2L_)
#define  OP2H_                  (0x04E2u)  /* 32-bit operand 2 - high word */
DEFCW(   OP2H                 , OP2H_)
#define  RES0_                  (0x04E4u)  /* 32x32-bit result 0 - least significant word */
DEFCW(   RES0                 , RES0_)
#define  RES1_                  (0x04E6u)  /* 32x32-bit result 1 */
DEFCW(   RES1                 , RES1_)
#define  RES2_                  (0x04E8u)  /* 32x32-bit result 2 */
DEFCW(   RES2                 , RES2_)
#define  RES3_                  (0x04EAu)  /* 32x32-bit result 3 - most significant word */
DEFCW(   RES3                 , RES3_)
#define  MPY32CTL0_             (0x04ECu)  /* MPY32 Control Register 0 */
DEFCW(   MPY32CTL0            , MPY32CTL0_)

#define MPY_B               MPY_L      /* Multiply Unsigned/Operand 1 (Byte Access) */
#define MPYS_B              MPYS_L     /* Multiply Signed/Operand 1 (Byte Access) */
#define MAC_B               MAC_L      /* Multiply Unsigned and Accumulate/Operand 1 (Byte Access) */
#define MACS_B              MACS_L     /* Multiply Signed and Accumulate/Operand 1 (Byte Access) */
#define OP2_B               OP2_L      /* Operand 2 (Byte Access) */
#define MPY32L_B            MPY32L_L   /* 32-bit operand 1 - multiply - low word (Byte Access) */
#define MPY32H_B            MPY32H_L   /* 32-bit operand 1 - multiply - high word (Byte Access) */
#define MPYS32L_B           MPYS32L_L  /* 32-bit operand 1 - signed multiply - low word (Byte Access) */
#define MPYS32H_B           MPYS32H_L  /* 32-bit operand 1 - signed multiply - high word (Byte Access) */
#define MAC32L_B            MAC32L_L   /* 32-bit operand 1 - multiply accumulate - low word (Byte Access) */
#define MAC32H_B            MAC32H_L   /* 32-bit operand 1 - multiply accumulate - high word (Byte Access) */
#define MACS32L_B           MACS32L_L  /* 32-bit operand 1 - signed multiply accumulate - low word (Byte Access) */
#define MACS32H_B           MACS32H_L  /* 32-bit operand 1 - signed multiply accumulate - high word (Byte Access) */
#define OP2L_B              OP2L_L     /* 32-bit operand 2 - low word (Byte Access) */
#define OP2H_B              OP2H_L     /* 32-bit operand 2 - high word (Byte Access) */

/* MPY32CTL0 Control Bits */
#define MPYC                (0x0001u)  /* Carry of the multiplier */
//#define RESERVED            (0x0002u)  /* Reserved */
#define MPYFRAC             (0x0004u)  /* Fractional mode */
#define MPYSAT              (0x0008u)  /* Saturation mode */
#define MPYM0               (0x0010u)  /* Multiplier mode Bit:0 */
#define MPYM1               (0x0020u)  /* Multiplier mode Bit:1 */
#define OP1_32              (0x0040u)  /* Bit-width of operand 1 0:16Bit / 1:32Bit */
#define OP2_32              (0x0080u)  /* Bit-width of operand 2 0:16Bit / 1:32Bit */
#define MPYDLYWRTEN         (0x0100u)  /* Delayed write enable */
#define MPYDLY32            (0x0200u)  /* Delayed write mode */

/* MPY32CTL0 Control Bits */
#define MPYC_L              (0x0001u)  /* Carry of the multiplier */
//#define RESERVED            (0x0002u)  /* Reserved */
#define MPYFRAC_L           (0x0004u)  /* Fractional mode */
#define MPYSAT_L            (0x0008u)  /* Saturation mode */
#define MPYM0_L             (0x0010u)  /* Multiplier mode Bit:0 */
#define MPYM1_L             (0x0020u)  /* Multiplier mode Bit:1 */
#define OP1_32_L            (0x0040u)  /* Bit-width of operand 1 0:16Bit / 1:32Bit */
#define OP2_32_L            (0x0080u)  /* Bit-width of operand 2 0:16Bit / 1:32Bit */

/* MPY32CTL0 Control Bits */
//#define RESERVED            (0x0002u)  /* Reserved */
#define MPYDLYWRTEN_H       (0x0001u)  /* Delayed write enable */
#define MPYDLY32_H          (0x0002u)  /* Delayed write mode */

#define MPYM_0              (0x0000u)  /* Multiplier mode: MPY */
#define MPYM_1              (0x0010u)  /* Multiplier mode: MPYS */
#define MPYM_2              (0x0020u)  /* Multiplier mode: MAC */
#define MPYM_3              (0x0030u)  /* Multiplier mode: MACS */
#define MPYM__MPY           (0x0000u)  /* Multiplier mode: MPY */
#define MPYM__MPYS          (0x0010u)  /* Multiplier mode: MPYS */
#define MPYM__MAC           (0x0020u)  /* Multiplier mode: MAC */
#define MPYM__MACS          (0x0030u)  /* Multiplier mode: MACS */

/************************************************************
* PMM - Power Management System for FRAM
************************************************************/
#define __MSP430_HAS_PMM_FRAM__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PMM_FRAM__ 0x0120
#define PMM_BASE __MSP430_BASEADDRESS_PMM_FRAM__

#define  PMMCTL0_              (0x0120u)  /* PMM Control 0 */
DEFCW(   PMMCTL0             , PMMCTL0_)
#define  PMMIFG_               (0x012Au)  /* PMM Interrupt Flag */
DEFCW(   PMMIFG              , PMMIFG_)
#define  PM5CTL0_              (0x0130u)  /* PMM Power Mode 5 Control Register 0 */
DEFCW(   PM5CTL0             , PM5CTL0_)

#define PMMPW               (0xA500u)  /* PMM Register Write Password */
#define PMMPW_H             (0xA5)    /* PMM Register Write Password for high word access */

/* PMMCTL0 Control Bits */
#define PMMSWBOR            (0x0004u)  /* PMM Software BOR */
#define PMMSWPOR            (0x0008u)  /* PMM Software POR */
#define PMMREGOFF           (0x0010u)  /* PMM Turn Regulator off */
#define SVSHE               (0x0040u)  /* SVS high side enable */
#define PMMLPRST            (0x0080u)  /* PMM Low-Power Reset Enable */

/* PMMCTL0 Control Bits */
#define PMMSWBOR_L          (0x0004u)  /* PMM Software BOR */
#define PMMSWPOR_L          (0x0008u)  /* PMM Software POR */
#define PMMREGOFF_L         (0x0010u)  /* PMM Turn Regulator off */
#define SVSHE_L             (0x0040u)  /* SVS high side enable */
#define PMMLPRST_L          (0x0080u)  /* PMM Low-Power Reset Enable */

/* PMMIFG Control Bits */
#define PMMBORIFG           (0x0100u)  /* PMM Software BOR interrupt flag */
#define PMMRSTIFG           (0x0200u)  /* PMM RESET pin interrupt flag */
#define PMMPORIFG           (0x0400u)  /* PMM Software POR interrupt flag */
#define SVSHIFG             (0x2000u)  /* SVS low side interrupt flag */
#define PMMLPM5IFG          (0x8000u)  /* LPM5 indication Flag */

/* PMMIFG Control Bits */
#define PMMBORIFG_H         (0x0001u)  /* PMM Software BOR interrupt flag */
#define PMMRSTIFG_H         (0x0002u)  /* PMM RESET pin interrupt flag */
#define PMMPORIFG_H         (0x0004u)  /* PMM Software POR interrupt flag */
#define SVSHIFG_H           (0x0020u)  /* SVS low side interrupt flag */
#define PMMLPM5IFG_H        (0x0080u)  /* LPM5 indication Flag */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5            (0x0001u)  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5_L          (0x0001u)  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */


/************************************************************
* DIGITAL I/O Port1/2 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT1_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT1_R__ 0x0200
#define P1_BASE __MSP430_BASEADDRESS_PORT1_R__
#define __MSP430_HAS_PORT2_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT2_R__ 0x0200
#define P2_BASE __MSP430_BASEADDRESS_PORT2_R__
#define __MSP430_HAS_PORTA_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTA_R__ 0x0200
#define PA_BASE __MSP430_BASEADDRESS_PORTA_R__
#define __MSP430_HAS_P1SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P2SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PASEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P1SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_P2SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_PASEL1__         /* Define for DriverLib */

#define PAIN_                  (0x0200u)  /* Port A Input */
READ_ONLY_DEFCW( PAIN              , PAIN_)
#define  PAOUT_                 (0x0202u)  /* Port A Output */
DEFCW(   PAOUT                , PAOUT_)
#define  PADIR_                 (0x0204u)  /* Port A Direction */
DEFCW(   PADIR                , PADIR_)
#define  PAREN_                 (0x0206u)  /* Port A Resistor Enable */
DEFCW(   PAREN                , PAREN_)
#define  PASEL0_                (0x020Au)  /* Port A Selection 0 */
DEFCW(   PASEL0               , PASEL0_)
#define  PASEL1_                (0x020Cu)  /* Port A Selection 1 */
DEFCW(   PASEL1               , PASEL1_)
#define  PASELC_                (0x0216u)  /* Port A Complement Selection */
DEFCW(   PASELC               , PASELC_)
#define  PAIES_                 (0x0218u)  /* Port A Interrupt Edge Select */
DEFCW(   PAIES                , PAIES_)
#define  PAIE_                  (0x021Au)  /* Port A Interrupt Enable */
DEFCW(   PAIE                 , PAIE_)
#define  PAIFG_                 (0x021Cu)  /* Port A Interrupt Flag */
DEFCW(   PAIFG                , PAIFG_)


#define P1IV_                  (0x020Eu)  /* Port 1 Interrupt Vector Word */
DEFW(   P1IV                 , P1IV_)
#define P2IV_                  (0x021Eu)  /* Port 2 Interrupt Vector Word */
DEFW(   P2IV                 , P2IV_)
#define P1IN                (PAIN_L)  /* Port 1 Input */
#define P1OUT               (PAOUT_L) /* Port 1 Output */
#define P1DIR               (PADIR_L) /* Port 1 Direction */
#define P1REN               (PAREN_L) /* Port 1 Resistor Enable */
#define P1SEL0              (PASEL0_L) /* Port 1 Selection 0 */
#define P1SEL1              (PASEL1_L) /* Port 1 Selection 1 */
#define P1SELC              (PASELC_L) /* Port 1 Complement Selection */
#define P1IES               (PAIES_L) /* Port 1 Interrupt Edge Select */
#define P1IE                (PAIE_L)  /* Port 1 Interrupt Enable */
#define P1IFG               (PAIFG_L) /* Port 1 Interrupt Flag */

//Definitions for P1IV
#define P1IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P1IV_P1IFG0          (0x0002u)    /* P1IV P1IFG.0 */
#define P1IV_P1IFG1          (0x0004u)    /* P1IV P1IFG.1 */
#define P1IV_P1IFG2          (0x0006u)    /* P1IV P1IFG.2 */
#define P1IV_P1IFG3          (0x0008u)    /* P1IV P1IFG.3 */
#define P1IV_P1IFG4          (0x000Au)    /* P1IV P1IFG.4 */
#define P1IV_P1IFG5          (0x000Cu)    /* P1IV P1IFG.5 */
#define P1IV_P1IFG6          (0x000Eu)    /* P1IV P1IFG.6 */
#define P1IV_P1IFG7          (0x0010u)    /* P1IV P1IFG.7 */

#define P2IN                (PAIN_H)  /* Port 2 Input */
#define P2OUT               (PAOUT_H) /* Port 2 Output */
#define P2DIR               (PADIR_H) /* Port 2 Direction */
#define P2REN               (PAREN_H) /* Port 2 Resistor Enable */
#define P2SEL0              (PASEL0_H) /* Port 2 Selection 0 */
#define P2SEL1              (PASEL1_H) /* Port 2 Selection 1 */
#define P2SELC              (PASELC_H) /* Port 2 Complement Selection */
#define P2IES               (PAIES_H) /* Port 2 Interrupt Edge Select */
#define P2IE                (PAIE_H)  /* Port 2 Interrupt Enable */
#define P2IFG               (PAIFG_H) /* Port 2 Interrupt Flag */

//Definitions for P2IV
#define P2IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P2IV_P2IFG0          (0x0002u)    /* P2IV P2IFG.0 */
#define P2IV_P2IFG1          (0x0004u)    /* P2IV P2IFG.1 */
#define P2IV_P2IFG2          (0x0006u)    /* P2IV P2IFG.2 */
#define P2IV_P2IFG3          (0x0008u)    /* P2IV P2IFG.3 */
#define P2IV_P2IFG4          (0x000Au)    /* P2IV P2IFG.4 */
#define P2IV_P2IFG5          (0x000Cu)    /* P2IV P2IFG.5 */
#define P2IV_P2IFG6          (0x000Eu)    /* P2IV P2IFG.6 */
#define P2IV_P2IFG7          (0x0010u)    /* P2IV P2IFG.7 */


/************************************************************
* DIGITAL I/O Port3/4 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT3_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT3_R__ 0x0220
#define P3_BASE __MSP430_BASEADDRESS_PORT3_R__
#define __MSP430_HAS_PORT4_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT4_R__ 0x0220
#define P4_BASE __MSP430_BASEADDRESS_PORT4_R__
#define __MSP430_HAS_PORTB_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTB_R__ 0x0220
#define PB_BASE __MSP430_BASEADDRESS_PORTB_R__
#define __MSP430_HAS_P3SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P4SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PBSEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P3SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_P4SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_PBSEL1__         /* Define for DriverLib */

#define PBIN_                  (0x0220u)  /* Port B Input */
READ_ONLY_DEFCW( PBIN              , PBIN_)
#define  PBOUT_                 (0x0222u)  /* Port B Output */
DEFCW(   PBOUT                , PBOUT_)
#define  PBDIR_                 (0x0224u)  /* Port B Direction */
DEFCW(   PBDIR                , PBDIR_)
#define  PBREN_                 (0x0226u)  /* Port B Resistor Enable */
DEFCW(   PBREN                , PBREN_)
#define  PBSEL0_                (0x022Au)  /* Port B Selection 0 */
DEFCW(   PBSEL0               , PBSEL0_)
#define  PBSEL1_                (0x022Cu)  /* Port B Selection 1 */
DEFCW(   PBSEL1               , PBSEL1_)
#define  PBSELC_                (0x0236u)  /* Port B Complement Selection */
DEFCW(   PBSELC               , PBSELC_)
#define  PBIES_                 (0x0238u)  /* Port B Interrupt Edge Select */
DEFCW(   PBIES                , PBIES_)
#define  PBIE_                  (0x023Au)  /* Port B Interrupt Enable */
DEFCW(   PBIE                 , PBIE_)
#define  PBIFG_                 (0x023Cu)  /* Port B Interrupt Flag */
DEFCW(   PBIFG                , PBIFG_)


#define P3IV_                  (0x022Eu)  /* Port 3 Interrupt Vector Word */
DEFW(   P3IV                 , P3IV_)
#define P4IV_                  (0x023Eu)  /* Port 4 Interrupt Vector Word */
DEFW(   P4IV                 , P4IV_)
#define P3IN                (PBIN_L)  /* Port 3 Input */
#define P3OUT               (PBOUT_L) /* Port 3 Output */
#define P3DIR               (PBDIR_L) /* Port 3 Direction */
#define P3REN               (PBREN_L) /* Port 3 Resistor Enable */
#define P3SEL0              (PBSEL0_L) /* Port 3 Selection 0 */
#define P3SEL1              (PBSEL1_L) /* Port 3 Selection 1 */
#define P3SELC              (PBSELC_L) /* Port 3 Complement Selection */
#define P3IES               (PBIES_L) /* Port 3 Interrupt Edge Select */
#define P3IE                (PBIE_L)  /* Port 3 Interrupt Enable */
#define P3IFG               (PBIFG_L) /* Port 3 Interrupt Flag */

//Definitions for P3IV
#define P3IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P3IV_P3IFG0          (0x0002u)    /* P3IV P3IFG.0 */
#define P3IV_P3IFG1          (0x0004u)    /* P3IV P3IFG.1 */
#define P3IV_P3IFG2          (0x0006u)    /* P3IV P3IFG.2 */
#define P3IV_P3IFG3          (0x0008u)    /* P3IV P3IFG.3 */
#define P3IV_P3IFG4          (0x000Au)    /* P3IV P3IFG.4 */
#define P3IV_P3IFG5          (0x000Cu)    /* P3IV P3IFG.5 */
#define P3IV_P3IFG6          (0x000Eu)    /* P3IV P3IFG.6 */
#define P3IV_P3IFG7          (0x0010u)    /* P3IV P3IFG.7 */

#define P4IN                (PBIN_H)  /* Port 4 Input */
#define P4OUT               (PBOUT_H) /* Port 4 Output */
#define P4DIR               (PBDIR_H) /* Port 4 Direction */
#define P4REN               (PBREN_H) /* Port 4 Resistor Enable */
#define P4SEL0              (PBSEL0_H) /* Port 4 Selection 0 */
#define P4SEL1              (PBSEL1_H) /* Port 4 Selection 1 */
#define P4SELC              (PBSELC_H) /* Port 4 Complement Selection */
#define P4IES               (PBIES_H) /* Port 4 Interrupt Edge Select */
#define P4IE                (PBIE_H)  /* Port 4 Interrupt Enable */
#define P4IFG               (PBIFG_H) /* Port 4 Interrupt Flag */

//Definitions for P4IV
#define P4IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P4IV_P4IFG0          (0x0002u)    /* P4IV P4IFG.0 */
#define P4IV_P4IFG1          (0x0004u)    /* P4IV P4IFG.1 */
#define P4IV_P4IFG2          (0x0006u)    /* P4IV P4IFG.2 */
#define P4IV_P4IFG3          (0x0008u)    /* P4IV P4IFG.3 */
#define P4IV_P4IFG4          (0x000Au)    /* P4IV P4IFG.4 */
#define P4IV_P4IFG5          (0x000Cu)    /* P4IV P4IFG.5 */
#define P4IV_P4IFG6          (0x000Eu)    /* P4IV P4IFG.6 */
#define P4IV_P4IFG7          (0x0010u)    /* P4IV P4IFG.7 */


/************************************************************
* DIGITAL I/O Port5/6 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT5_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT5_R__ 0x0240
#define P5_BASE __MSP430_BASEADDRESS_PORT5_R__
#define __MSP430_HAS_PORT6_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT6_R__ 0x0240
#define P6_BASE __MSP430_BASEADDRESS_PORT6_R__
#define __MSP430_HAS_PORTC_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTC_R__ 0x0240
#define PC_BASE __MSP430_BASEADDRESS_PORTC_R__
#define __MSP430_HAS_P5SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P6SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PCSEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P5SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_P6SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_PCSEL1__         /* Define for DriverLib */

#define PCIN_                  (0x0240u)  /* Port C Input */
READ_ONLY_DEFCW( PCIN              , PCIN_)
#define  PCOUT_                 (0x0242u)  /* Port C Output */
DEFCW(   PCOUT                , PCOUT_)
#define  PCDIR_                 (0x0244u)  /* Port C Direction */
DEFCW(   PCDIR                , PCDIR_)
#define  PCREN_                 (0x0246u)  /* Port C Resistor Enable */
DEFCW(   PCREN                , PCREN_)
#define  PCSEL0_                (0x024Au)  /* Port C Selection 0 */
DEFCW(   PCSEL0               , PCSEL0_)
#define  PCSEL1_                (0x024Cu)  /* Port C Selection 1 */
DEFCW(   PCSEL1               , PCSEL1_)
#define  PCSELC_                (0x0256u)  /* Port C Complement Selection */
DEFCW(   PCSELC               , PCSELC_)


#define P5IN                (PCIN_L)  /* Port 5 Input */
#define P5OUT               (PCOUT_L) /* Port 5 Output */
#define P5DIR               (PCDIR_L) /* Port 5 Direction */
#define P5REN               (PCREN_L) /* Port 5 Resistor Enable */
#define P5SEL0              (PCSEL0_L) /* Port 5 Selection 0 */
#define P5SEL1              (PCSEL1_L) /* Port 5 Selection 1 */
#define P5SELC              (PCSELC_L) /* Port 5 Complement Selection */

#define P6IN                (PCIN_H)  /* Port 6 Input */
#define P6OUT               (PCOUT_H) /* Port 6 Output */
#define P6DIR               (PCDIR_H) /* Port 6 Direction */
#define P6REN               (PCREN_H) /* Port 6 Resistor Enable */
#define P6SEL0              (PCSEL0_H) /* Port 6 Selection 0 */
#define P6SEL1              (PCSEL1_H) /* Port 6 Selection 1 */
#define P6SELC              (PCSELC_H) /* Port 6 Complement Selection */


/************************************************************
* DIGITAL I/O Port7/8 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT7_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT7_R__ 0x0260
#define P7_BASE __MSP430_BASEADDRESS_PORT7_R__
#define __MSP430_HAS_PORT8_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT8_R__ 0x0260
#define P8_BASE __MSP430_BASEADDRESS_PORT8_R__
#define __MSP430_HAS_PORTD_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTD_R__ 0x0260
#define PD_BASE __MSP430_BASEADDRESS_PORTD_R__
#define __MSP430_HAS_P7SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P8SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PDSEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P7SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_P8SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_PDSEL1__         /* Define for DriverLib */

#define PDIN_                  (0x0260u)  /* Port D Input */
READ_ONLY_DEFCW( PDIN              , PDIN_)
#define  PDOUT_                 (0x0262u)  /* Port D Output */
DEFCW(   PDOUT                , PDOUT_)
#define  PDDIR_                 (0x0264u)  /* Port D Direction */
DEFCW(   PDDIR                , PDDIR_)
#define  PDREN_                 (0x0266u)  /* Port D Resistor Enable */
DEFCW(   PDREN                , PDREN_)
#define  PDSEL0_                (0x026Au)  /* Port D Selection 0 */
DEFCW(   PDSEL0               , PDSEL0_)
#define  PDSEL1_                (0x026Cu)  /* Port D Selection 1 */
DEFCW(   PDSEL1               , PDSEL1_)
#define  PDSELC_                (0x0276u)  /* Port D Complement Selection */
DEFCW(   PDSELC               , PDSELC_)


#define P7IN                (PDIN_L)  /* Port 7 Input */
#define P7OUT               (PDOUT_L) /* Port 7 Output */
#define P7DIR               (PDDIR_L) /* Port 7 Direction */
#define P7REN               (PDREN_L) /* Port 7 Resistor Enable */
#define P7SEL0              (PDSEL0_L) /* Port 7 Selection 0 */
#define P7SEL1              (PDSEL1_L) /* Port 7 Selection 1 */
#define P7SELC              (PDSELC_L) /* Port 7 Complement Selection */

#define P8IN                (PDIN_H)  /* Port 8 Input */
#define P8OUT               (PDOUT_H) /* Port 8 Output */
#define P8DIR               (PDDIR_H) /* Port 8 Direction */
#define P8REN               (PDREN_H) /* Port 8 Resistor Enable */
#define P8SEL0              (PDSEL0_H) /* Port 8 Selection 0 */
#define P8SEL1              (PDSEL1_H) /* Port 8 Selection 1 */
#define P8SELC              (PDSELC_H) /* Port 8 Complement Selection */


/************************************************************
* DIGITAL I/O Port9/10 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT9_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT9_R__ 0x0280
#define P9_BASE __MSP430_BASEADDRESS_PORT9_R__
#define __MSP430_HAS_PORT10_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT10_R__ 0x0280
#define P10_BASE __MSP430_BASEADDRESS_PORT10_R__
#define __MSP430_HAS_PORTE_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTE_R__ 0x0280
#define PE_BASE __MSP430_BASEADDRESS_PORTE_R__
#define __MSP430_HAS_P9SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P10SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PESEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P9SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_P10SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_PESEL1__         /* Define for DriverLib */

#define PEIN_                  (0x0280u)  /* Port E Input */
READ_ONLY_DEFCW( PEIN              , PEIN_)
#define  PEOUT_                 (0x0282u)  /* Port E Output */
DEFCW(   PEOUT                , PEOUT_)
#define  PEDIR_                 (0x0284u)  /* Port E Direction */
DEFCW(   PEDIR                , PEDIR_)
#define  PEREN_                 (0x0286u)  /* Port E Resistor Enable */
DEFCW(   PEREN                , PEREN_)
#define  PESEL0_                (0x028Au)  /* Port E Selection 0 */
DEFCW(   PESEL0               , PESEL0_)
#define  PESEL1_                (0x028Cu)  /* Port E Selection 1 */
DEFCW(   PESEL1               , PESEL1_)
#define  PESELC_                (0x0296u)  /* Port E Complement Selection */
DEFCW(   PESELC               , PESELC_)


#define P9IN                (PEIN_L)  /* Port 9 Input */
#define P9OUT               (PEOUT_L) /* Port 9 Output */
#define P9DIR               (PEDIR_L) /* Port 9 Direction */
#define P9REN               (PEREN_L) /* Port 9 Resistor Enable */
#define P9SEL0              (PESEL0_L) /* Port 9 Selection 0 */
#define P9SEL1              (PESEL1_L) /* Port 9 Selection 1 */
#define P9SELC              (PESELC_L) /* Port 9 Complement Selection */

#define P10IN                (PEIN_H)  /* Port 10 Input */
#define P10OUT               (PEOUT_H) /* Port 10 Output */
#define P10DIR               (PEDIR_H) /* Port 10 Direction */
#define P10REN               (PEREN_H) /* Port 10 Resistor Enable */
#define P10SEL0              (PESEL0_H) /* Port 10 Selection 0 */
#define P10SEL1              (PESEL1_H) /* Port 10 Selection 1 */
#define P10SELC              (PESELC_H) /* Port 10 Complement Selection */


/************************************************************
* DIGITAL I/O PortJ Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORTJ_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTJ_R__ 0x0320
#define PJ_BASE __MSP430_BASEADDRESS_PORTJ_R__
#define __MSP430_HAS_PJSEL0__          /* Define for DriverLib */
#define __MSP430_HAS_PJSEL1__          /* Define for DriverLib */

#define PJIN_                  (0x0320u)  /* Port J Input */
READ_ONLY_DEFCW( PJIN              , PJIN_)
#define  PJOUT_                 (0x0322u)  /* Port J Output */
DEFCW(   PJOUT                , PJOUT_)
#define  PJDIR_                 (0x0324u)  /* Port J Direction */
DEFCW(   PJDIR                , PJDIR_)
#define  PJREN_                 (0x0326u)  /* Port J Resistor Enable */
DEFCW(   PJREN                , PJREN_)
#define  PJSEL0_                (0x032Au)  /* Port J Selection 0 */
DEFCW(   PJSEL0               , PJSEL0_)
#define  PJSEL1_                (0x032Cu)  /* Port J Selection 1 */
DEFCW(   PJSEL1               , PJSEL1_)
#define  PJSELC_                (0x0336u)  /* Port J Complement Selection */
DEFCW(   PJSELC               , PJSELC_)

/*************************************************************
* RAM Control Module for FRAM
*************************************************************/
#define __MSP430_HAS_RC_FRAM__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_RC_FRAM__ 0x0158
#define RAM_BASE __MSP430_BASEADDRESS_RC_FRAM__

#define  RCCTL0_               (0x0158u)  /* Ram Controller Control Register */
DEFCW(   RCCTL0              , RCCTL0_)

/* RCCTL0 Control Bits */
#define RCRS0OFF0           (0x0001u)  /* RAM Controller RAM Sector 0 Off Bit: 0 */
#define RCRS0OFF1           (0x0002u)  /* RAM Controller RAM Sector 0 Off Bit: 1 */
#define RCRS1OFF0           (0x0004u)  /* RAM Controller RAM Sector 1 Off Bit: 0 */
#define RCRS1OFF1           (0x0008u)  /* RAM Controller RAM Sector 1 Off Bit: 1 */
#define RCRS2OFF0           (0x0010u)  /* RAM Controller RAM Sector 2 Off Bit: 0 */
#define RCRS2OFF1           (0x0020u)  /* RAM Controller RAM Sector 2 Off Bit: 1 */
#define RCRS3OFF0           (0x0040u)  /* RAM Controller RAM Sector 3 Off Bit: 0 */
#define RCRS3OFF1           (0x0080u)  /* RAM Controller RAM Sector 3 Off Bit: 1 */

/* RCCTL0 Control Bits */
#define RCRS0OFF0_L         (0x0001u)  /* RAM Controller RAM Sector 0 Off Bit: 0 */
#define RCRS0OFF1_L         (0x0002u)  /* RAM Controller RAM Sector 0 Off Bit: 1 */
#define RCRS1OFF0_L         (0x0004u)  /* RAM Controller RAM Sector 1 Off Bit: 0 */
#define RCRS1OFF1_L         (0x0008u)  /* RAM Controller RAM Sector 1 Off Bit: 1 */
#define RCRS2OFF0_L         (0x0010u)  /* RAM Controller RAM Sector 2 Off Bit: 0 */
#define RCRS2OFF1_L         (0x0020u)  /* RAM Controller RAM Sector 2 Off Bit: 1 */
#define RCRS3OFF0_L         (0x0040u)  /* RAM Controller RAM Sector 3 Off Bit: 0 */
#define RCRS3OFF1_L         (0x0080u)  /* RAM Controller RAM Sector 3 Off Bit: 1 */

#define RCKEY               (0x5A00u)

#define RCRS0OFF_0          (0x0000u)  /* RAM Controller RAM Sector 0 Off : 0 */
#define RCRS0OFF_1          (0x0001u)  /* RAM Controller RAM Sector 0 Off : 1 */
#define RCRS0OFF_2          (0x0002u)  /* RAM Controller RAM Sector 0 Off : 2 */
#define RCRS0OFF_3          (0x0003u)  /* RAM Controller RAM Sector 0 Off : 3 */
#define RCRS1OFF_0          (0x0000u)  /* RAM Controller RAM Sector 1 Off : 0 */
#define RCRS1OFF_1          (0x0004u)  /* RAM Controller RAM Sector 1 Off : 1 */
#define RCRS1OFF_2          (0x0008u)  /* RAM Controller RAM Sector 1 Off : 2 */
#define RCRS1OFF_3          (0x000Cu)  /* RAM Controller RAM Sector 1 Off : 3 */
#define RCRS2OFF_0          (0x0000u)  /* RAM Controller RAM Sector 2 Off : 0 */
#define RCRS2OFF_1          (0x0010u)  /* RAM Controller RAM Sector 2 Off : 1 */
#define RCRS2OFF_2          (0x0020u)  /* RAM Controller RAM Sector 2 Off : 2 */
#define RCRS2OFF_3          (0x0030u)  /* RAM Controller RAM Sector 2 Off : 3 */
#define RCRS3OFF_0          (0x0000u)  /* RAM Controller RAM Sector 3 Off : 0 */
#define RCRS3OFF_1          (0x0040u)  /* RAM Controller RAM Sector 3 Off : 1 */
#define RCRS3OFF_2          (0x0080u)  /* RAM Controller RAM Sector 3 Off : 2 */
#define RCRS3OFF_3          (0x00C0u)  /* RAM Controller RAM Sector 3 Off : 3 */

/************************************************************
* Shared Reference
************************************************************/
#define __MSP430_HAS_REF_A__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_REF_A__ 0x01B0
#define REF_A_BASE __MSP430_BASEADDRESS_REF_A__

#define  REFCTL0_               (0x01B0u)  /* REF Shared Reference control register 0 */
DEFCW(   REFCTL0              , REFCTL0_)

/* REFCTL0 Control Bits */
#define REFON               (0x0001u)  /* REF Reference On */
#define REFOUT              (0x0002u)  /* REF Reference output Buffer On */
//#define RESERVED            (0x0004u)  /* Reserved */
#define REFTCOFF            (0x0008u)  /* REF Temp.Sensor off */
#define REFVSEL0            (0x0010u)  /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1            (0x0020u)  /* REF Reference Voltage Level Select Bit:1 */
#define REFGENOT            (0x0040u)  /* REF Reference generator one-time trigger */
#define REFBGOT             (0x0080u)  /* REF Bandgap and bandgap buffer one-time trigger */
#define REFGENACT           (0x0100u)  /* REF Reference generator active */
#define REFBGACT            (0x0200u)  /* REF Reference bandgap active */
#define REFGENBUSY          (0x0400u)  /* REF Reference generator busy */
#define BGMODE              (0x0800u)  /* REF Bandgap mode */
#define REFGENRDY           (0x1000u)  /* REF Reference generator ready */
#define REFBGRDY            (0x2000u)  /* REF Reference bandgap ready */
//#define RESERVED            (0x4000u)  /* Reserved */
//#define RESERVED            (0x8000u)  /* Reserved */

/* REFCTL0 Control Bits */
#define REFON_L             (0x0001u)  /* REF Reference On */
#define REFOUT_L            (0x0002u)  /* REF Reference output Buffer On */
//#define RESERVED            (0x0004u)  /* Reserved */
#define REFTCOFF_L          (0x0008u)  /* REF Temp.Sensor off */
#define REFVSEL0_L          (0x0010u)  /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1_L          (0x0020u)  /* REF Reference Voltage Level Select Bit:1 */
#define REFGENOT_L          (0x0040u)  /* REF Reference generator one-time trigger */
#define REFBGOT_L           (0x0080u)  /* REF Bandgap and bandgap buffer one-time trigger */
//#define RESERVED            (0x4000u)  /* Reserved */
//#define RESERVED            (0x8000u)  /* Reserved */

/* REFCTL0 Control Bits */
//#define RESERVED            (0x0004u)  /* Reserved */
#define REFGENACT_H         (0x0001u)  /* REF Reference generator active */
#define REFBGACT_H          (0x0002u)  /* REF Reference bandgap active */
#define REFGENBUSY_H        (0x0004u)  /* REF Reference generator busy */
#define BGMODE_H            (0x0008u)  /* REF Bandgap mode */
#define REFGENRDY_H         (0x0010u)  /* REF Reference generator ready */
#define REFBGRDY_H          (0x0020u)  /* REF Reference bandgap ready */
//#define RESERVED            (0x4000u)  /* Reserved */
//#define RESERVED            (0x8000u)  /* Reserved */

#define REFVSEL_0           (0x0000u)  /* REF Reference Voltage Level Select 1.2V */
#define REFVSEL_1           (0x0010u)  /* REF Reference Voltage Level Select 2.0V */
#define REFVSEL_2           (0x0020u)  /* REF Reference Voltage Level Select 2.5V */
#define REFVSEL_3           (0x0030u)  /* REF Reference Voltage Level Select 2.5V */

/************************************************************
* Real Time Clock
************************************************************/
#define __MSP430_HAS_RTC_C__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_RTC_C__ 0x04A0
#define RTC_C_BASE __MSP430_BASEADDRESS_RTC_C__

#define RTCCTL0_               (0x04A0u)  /* Real Timer Clock Control 0/Key */
DEFCW(  RTCCTL0              , RTCCTL0_)
#define RTCPWD              RTCCTL0_H
#define RTCCTL13_              (0x04A2u)  /* Real Timer Clock Control 1/3 */
DEFCW(  RTCCTL13             , RTCCTL13_)
#define RTCCTL1             RTCCTL13_L
#define RTCCTL3             RTCCTL13_H
#define RTCOCAL_               (0x04A4u)  /* Real Timer Clock Offset Calibartion */
DEFCW(  RTCOCAL              , RTCOCAL_)
#define RTCTCMP_               (0x04A6u)  /* Real Timer Temperature Compensation */
DEFCW(  RTCTCMP              , RTCTCMP_)
#define RTCPS0CTL_             (0x04A8u)  /* Real Timer Prescale Timer 0 Control */
DEFCW(  RTCPS0CTL            , RTCPS0CTL_)
#define RTCPS1CTL_             (0x04AAu)  /* Real Timer Prescale Timer 1 Control */
DEFCW(  RTCPS1CTL            , RTCPS1CTL_)
#define RTCPS_                 (0x04ACu)  /* Real Timer Prescale Timer Control */
DEFCW(  RTCPS                , RTCPS_)
#define RTCIV_                 (0x04AEu)  /* Real Time Clock Interrupt Vector */
DEFW(   RTCIV                , RTCIV_)
#define RTCTIM0_               (0x04B0u)  /* Real Time Clock Time 0 */
DEFCW(  RTCTIM0              , RTCTIM0_)
#define RTCTIM1_               (0x04B2u)  /* Real Time Clock Time 1 */
DEFCW(  RTCTIM1              , RTCTIM1_)
#define RTCDATE_               (0x04B4u)  /* Real Time Clock Date */
DEFCW(  RTCDATE              , RTCDATE_)
#define RTCYEAR_               (0x04B6u)  /* Real Time Clock Year */
DEFCW(  RTCYEAR              , RTCYEAR_)
#define RTCAMINHR_             (0x04B8u)  /* Real Time Clock Alarm Min/Hour */
DEFCW(  RTCAMINHR            , RTCAMINHR_)
#define RTCADOWDAY_            (0x04BAu)  /* Real Time Clock Alarm day of week/day */
DEFCW(  RTCADOWDAY           , RTCADOWDAY_)
#define BIN2BCD_               (0x04BCu)  /* Real Time Binary-to-BCD conversion register */
DEFW(   BIN2BCD              , BIN2BCD_)
#define BCD2BIN_               (0x04BEu)  /* Real Time BCD-to-binary conversion register */
DEFW(   BCD2BIN              , BCD2BIN_)
#define RTCSEC              RTCTIM0_L
#define RTCMIN              RTCTIM0_H
#define RTCHOUR             RTCTIM1_L
#define RTCDOW              RTCTIM1_H
#define RTCDAY              RTCDATE_L
#define RTCMON              RTCDATE_H
#define RTCYEARL            RTCYEAR_L
#define RT0PS               RTCPS_L
#define RT1PS               RTCPS_H
#define RTCAMIN             RTCAMINHR_L    /* Real Time Clock Alarm Min */
#define RTCAHOUR            RTCAMINHR_H    /* Real Time Clock Alarm Hour */
#define RTCADOW             RTCADOWDAY_L   /* Real Time Clock Alarm day of week */
#define RTCADAY             RTCADOWDAY_H   /* Real Time Clock Alarm day */

/* RTCCTL0 Control Bits */
#define RTCOFIE             (0x0080u)   /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE            (0x0040u)   /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE              (0x0020u)   /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE            (0x0010u)   /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG            (0x0008u)   /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG           (0x0004u)   /* RTC Time Event Interrupt Flag */
#define RTCAIFG             (0x0002u)   /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG           (0x0001u)   /* RTC Ready Interrupt Flag */

/* RTCCTL0 Control Bits */
#define RTCOFIE_L           (0x0080u)   /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE_L          (0x0040u)   /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE_L            (0x0020u)   /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE_L          (0x0010u)   /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG_L          (0x0008u)   /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG_L         (0x0004u)   /* RTC Time Event Interrupt Flag */
#define RTCAIFG_L           (0x0002u)   /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG_L         (0x0001u)   /* RTC Ready Interrupt Flag */

#define RTCKEY              (0xA500u)   /* RTC Key for RTC write access */
#define RTCKEY_H            (0xA5)     /* RTC Key for RTC write access (high word) */

/* RTCCTL13 Control Bits */
#define RTCCALF1            (0x0200u)   /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0            (0x0100u)   /* RTC Calibration Frequency Bit 0 */
#define RTCBCD              (0x0080u)   /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD             (0x0040u)   /* RTC Hold */
#define RTCMODE             (0x0020u)   /* RTC Mode 0:Counter / 1: Calendar */
#define RTCRDY              (0x0010u)   /* RTC Ready */
#define RTCSSEL1            (0x0008u)   /* RTC Source Select 1 */
#define RTCSSEL0            (0x0004u)   /* RTC Source Select 0 */
#define RTCTEV1             (0x0002u)   /* RTC Time Event 1 */
#define RTCTEV0             (0x0001u)   /* RTC Time Event 0 */

/* RTCCTL13 Control Bits */
#define RTCBCD_L            (0x0080u)   /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD_L           (0x0040u)   /* RTC Hold */
#define RTCMODE_L           (0x0020u)   /* RTC Mode 0:Counter / 1: Calendar */
#define RTCRDY_L            (0x0010u)   /* RTC Ready */
#define RTCSSEL1_L          (0x0008u)   /* RTC Source Select 1 */
#define RTCSSEL0_L          (0x0004u)   /* RTC Source Select 0 */
#define RTCTEV1_L           (0x0002u)   /* RTC Time Event 1 */
#define RTCTEV0_L           (0x0001u)   /* RTC Time Event 0 */

/* RTCCTL13 Control Bits */
#define RTCCALF1_H          (0x0002u)   /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0_H          (0x0001u)   /* RTC Calibration Frequency Bit 0 */

#define RTCSSEL_0           (0x0000u)   /* RTC Source Select ACLK */
#define RTCSSEL_1           (0x0004u)   /* RTC Source Select SMCLK */
#define RTCSSEL_2           (0x0008u)   /* RTC Source Select RT1PS */
#define RTCSSEL_3           (0x000Cu)   /* RTC Source Select RT1PS */
#define RTCSSEL__ACLK       (0x0000u)   /* RTC Source Select ACLK */
#define RTCSSEL__SMCLK      (0x0004u)   /* RTC Source Select SMCLK */
#define RTCSSEL__RT1PS      (0x0008u)   /* RTC Source Select RT1PS */

#define RTCTEV_0            (0x0000u)   /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV_1            (0x0001u)   /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV_2            (0x0002u)   /* RTC Time Event: 2 (12:00 changed) */
#define RTCTEV_3            (0x0003u)   /* RTC Time Event: 3 (00:00 changed) */
#define RTCTEV__MIN         (0x0000u)   /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV__HOUR        (0x0001u)   /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV__0000        (0x0002u)   /* RTC Time Event: 2 (00:00 changed) */
#define RTCTEV__1200        (0x0003u)   /* RTC Time Event: 3 (12:00 changed) */

#define RTCCALF_0           (0x0000u)   /* RTC Calibration Frequency: No Output */
#define RTCCALF_1           (0x0100u)   /* RTC Calibration Frequency: 512 Hz */
#define RTCCALF_2           (0x0200u)   /* RTC Calibration Frequency: 256 Hz */
#define RTCCALF_3           (0x0300u)   /* RTC Calibration Frequency: 1 Hz */

/* RTCOCAL Control Bits */
#define RTCOCALS            (0x8000u)   /* RTC Offset Calibration Sign */
#define RTCOCAL7            (0x0080u)   /* RTC Offset Calibration Bit 7 */
#define RTCOCAL6            (0x0040u)   /* RTC Offset Calibration Bit 6 */
#define RTCOCAL5            (0x0020u)   /* RTC Offset Calibration Bit 5 */
#define RTCOCAL4            (0x0010u)   /* RTC Offset Calibration Bit 4 */
#define RTCOCAL3            (0x0008u)   /* RTC Offset Calibration Bit 3 */
#define RTCOCAL2            (0x0004u)   /* RTC Offset Calibration Bit 2 */
#define RTCOCAL1            (0x0002u)   /* RTC Offset Calibration Bit 1 */
#define RTCOCAL0            (0x0001u)   /* RTC Offset Calibration Bit 0 */

/* RTCOCAL Control Bits */
#define RTCOCAL7_L          (0x0080u)   /* RTC Offset Calibration Bit 7 */
#define RTCOCAL6_L          (0x0040u)   /* RTC Offset Calibration Bit 6 */
#define RTCOCAL5_L          (0x0020u)   /* RTC Offset Calibration Bit 5 */
#define RTCOCAL4_L          (0x0010u)   /* RTC Offset Calibration Bit 4 */
#define RTCOCAL3_L          (0x0008u)   /* RTC Offset Calibration Bit 3 */
#define RTCOCAL2_L          (0x0004u)   /* RTC Offset Calibration Bit 2 */
#define RTCOCAL1_L          (0x0002u)   /* RTC Offset Calibration Bit 1 */
#define RTCOCAL0_L          (0x0001u)   /* RTC Offset Calibration Bit 0 */

/* RTCOCAL Control Bits */
#define RTCOCALS_H          (0x0080u)   /* RTC Offset Calibration Sign */

/* RTCTCMP Control Bits */
#define RTCTCMPS            (0x8000u)   /* RTC Temperature Compensation Sign */
#define RTCTCRDY            (0x4000u)   /* RTC Temperature compensation ready */
#define RTCTCOK             (0x2000u)   /* RTC Temperature compensation write OK */
#define RTCTCMP7            (0x0080u)   /* RTC Temperature Compensation Bit 7 */
#define RTCTCMP6            (0x0040u)   /* RTC Temperature Compensation Bit 6 */
#define RTCTCMP5            (0x0020u)   /* RTC Temperature Compensation Bit 5 */
#define RTCTCMP4            (0x0010u)   /* RTC Temperature Compensation Bit 4 */
#define RTCTCMP3            (0x0008u)   /* RTC Temperature Compensation Bit 3 */
#define RTCTCMP2            (0x0004u)   /* RTC Temperature Compensation Bit 2 */
#define RTCTCMP1            (0x0002u)   /* RTC Temperature Compensation Bit 1 */
#define RTCTCMP0            (0x0001u)   /* RTC Temperature Compensation Bit 0 */

/* RTCTCMP Control Bits */
#define RTCTCMP7_L          (0x0080u)   /* RTC Temperature Compensation Bit 7 */
#define RTCTCMP6_L          (0x0040u)   /* RTC Temperature Compensation Bit 6 */
#define RTCTCMP5_L          (0x0020u)   /* RTC Temperature Compensation Bit 5 */
#define RTCTCMP4_L          (0x0010u)   /* RTC Temperature Compensation Bit 4 */
#define RTCTCMP3_L          (0x0008u)   /* RTC Temperature Compensation Bit 3 */
#define RTCTCMP2_L          (0x0004u)   /* RTC Temperature Compensation Bit 2 */
#define RTCTCMP1_L          (0x0002u)   /* RTC Temperature Compensation Bit 1 */
#define RTCTCMP0_L          (0x0001u)   /* RTC Temperature Compensation Bit 0 */

/* RTCTCMP Control Bits */
#define RTCTCMPS_H          (0x0080u)   /* RTC Temperature Compensation Sign */
#define RTCTCRDY_H          (0x0040u)   /* RTC Temperature compensation ready */
#define RTCTCOK_H           (0x0020u)   /* RTC Temperature compensation write OK */

#define RTCAE               (0x80)     /* Real Time Clock Alarm enable */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x8000u)
//#define Reserved          (0x4000u)
#define RT0PSDIV2           (0x2000u)   /* RTC Prescale Timer 0 Clock Divide Bit: 2 */
#define RT0PSDIV1           (0x1000u)   /* RTC Prescale Timer 0 Clock Divide Bit: 1 */
#define RT0PSDIV0           (0x0800u)   /* RTC Prescale Timer 0 Clock Divide Bit: 0 */
//#define Reserved          (0x0400u)
//#define Reserved          (0x0200u)
#define RT0PSHOLD           (0x0100u)   /* RTC Prescale Timer 0 Hold */
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)
#define RT0IP2              (0x0010u)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1              (0x0008u)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0              (0x0004u)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE             (0x0002u)   /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG            (0x0001u)   /* RTC Prescale Timer 0 Interrupt Flag */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x8000u)
//#define Reserved          (0x4000u)
//#define Reserved          (0x0400u)
//#define Reserved          (0x0200u)
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)
#define RT0IP2_L            (0x0010u)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1_L            (0x0008u)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0_L            (0x0004u)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE_L           (0x0002u)   /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG_L          (0x0001u)   /* RTC Prescale Timer 0 Interrupt Flag */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x8000u)
//#define Reserved          (0x4000u)
#define RT0PSDIV2_H         (0x0020u)   /* RTC Prescale Timer 0 Clock Divide Bit: 2 */
#define RT0PSDIV1_H         (0x0010u)   /* RTC Prescale Timer 0 Clock Divide Bit: 1 */
#define RT0PSDIV0_H         (0x0008u)   /* RTC Prescale Timer 0 Clock Divide Bit: 0 */
//#define Reserved          (0x0400u)
//#define Reserved          (0x0200u)
#define RT0PSHOLD_H         (0x0001u)   /* RTC Prescale Timer 0 Hold */
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)

#define RT0IP_0             (0x0000u)   /* RTC Prescale Timer 0 Interrupt Interval /2 */
#define RT0IP_1             (0x0004u)   /* RTC Prescale Timer 0 Interrupt Interval /4 */
#define RT0IP_2             (0x0008u)   /* RTC Prescale Timer 0 Interrupt Interval /8 */
#define RT0IP_3             (0x000Cu)   /* RTC Prescale Timer 0 Interrupt Interval /16 */
#define RT0IP_4             (0x0010u)   /* RTC Prescale Timer 0 Interrupt Interval /32 */
#define RT0IP_5             (0x0014u)   /* RTC Prescale Timer 0 Interrupt Interval /64 */
#define RT0IP_6             (0x0018u)   /* RTC Prescale Timer 0 Interrupt Interval /128 */
#define RT0IP_7             (0x001Cu)   /* RTC Prescale Timer 0 Interrupt Interval /256 */

#define RT0PSDIV_0          (0x0000u)   /* RTC Prescale Timer 0 Clock Divide: /2 */
#define RT0PSDIV_1          (0x0800u)   /* RTC Prescale Timer 0 Clock Divide: /4 */
#define RT0PSDIV_2          (0x1000u)   /* RTC Prescale Timer 0 Clock Divide: /8 */
#define RT0PSDIV_3          (0x1800u)   /* RTC Prescale Timer 0 Clock Divide: /16 */
#define RT0PSDIV_4          (0x2000u)   /* RTC Prescale Timer 0 Clock Divide: /32 */
#define RT0PSDIV_5          (0x2800u)   /* RTC Prescale Timer 0 Clock Divide: /64 */
#define RT0PSDIV_6          (0x3000u)   /* RTC Prescale Timer 0 Clock Divide: /128 */
#define RT0PSDIV_7          (0x3800u)   /* RTC Prescale Timer 0 Clock Divide: /256 */

/* RTCPS1CTL Control Bits */
#define RT1SSEL1            (0x8000u)   /* RTC Prescale Timer 1 Source Select Bit: 1 */
#define RT1SSEL0            (0x4000u)   /* RTC Prescale Timer 1 Source Select Bit: 0 */
#define RT1PSDIV2           (0x2000u)   /* RTC Prescale Timer 1 Clock Divide Bit: 2 */
#define RT1PSDIV1           (0x1000u)   /* RTC Prescale Timer 1 Clock Divide Bit: 1 */
#define RT1PSDIV0           (0x0800u)   /* RTC Prescale Timer 1 Clock Divide Bit: 0 */
//#define Reserved          (0x0400u)
//#define Reserved          (0x0200u)
#define RT1PSHOLD           (0x0100u)   /* RTC Prescale Timer 1 Hold */
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)
#define RT1IP2              (0x0010u)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1              (0x0008u)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0              (0x0004u)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE             (0x0002u)   /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG            (0x0001u)   /* RTC Prescale Timer 1 Interrupt Flag */

/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0400u)
//#define Reserved          (0x0200u)
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)
#define RT1IP2_L            (0x0010u)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1_L            (0x0008u)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0_L            (0x0004u)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE_L           (0x0002u)   /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG_L          (0x0001u)   /* RTC Prescale Timer 1 Interrupt Flag */

/* RTCPS1CTL Control Bits */
#define RT1SSEL1_H          (0x0080u)   /* RTC Prescale Timer 1 Source Select Bit: 1 */
#define RT1SSEL0_H          (0x0040u)   /* RTC Prescale Timer 1 Source Select Bit: 0 */
#define RT1PSDIV2_H         (0x0020u)   /* RTC Prescale Timer 1 Clock Divide Bit: 2 */
#define RT1PSDIV1_H         (0x0010u)   /* RTC Prescale Timer 1 Clock Divide Bit: 1 */
#define RT1PSDIV0_H         (0x0008u)   /* RTC Prescale Timer 1 Clock Divide Bit: 0 */
//#define Reserved          (0x0400u)
//#define Reserved          (0x0200u)
#define RT1PSHOLD_H         (0x0001u)   /* RTC Prescale Timer 1 Hold */
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)

#define RT1IP_0             (0x0000u)   /* RTC Prescale Timer 1 Interrupt Interval /2 */
#define RT1IP_1             (0x0004u)   /* RTC Prescale Timer 1 Interrupt Interval /4 */
#define RT1IP_2             (0x0008u)   /* RTC Prescale Timer 1 Interrupt Interval /8 */
#define RT1IP_3             (0x000Cu)   /* RTC Prescale Timer 1 Interrupt Interval /16 */
#define RT1IP_4             (0x0010u)   /* RTC Prescale Timer 1 Interrupt Interval /32 */
#define RT1IP_5             (0x0014u)   /* RTC Prescale Timer 1 Interrupt Interval /64 */
#define RT1IP_6             (0x0018u)   /* RTC Prescale Timer 1 Interrupt Interval /128 */
#define RT1IP_7             (0x001Cu)   /* RTC Prescale Timer 1 Interrupt Interval /256 */

#define RT1PSDIV_0          (0x0000u)   /* RTC Prescale Timer 1 Clock Divide: /2 */
#define RT1PSDIV_1          (0x0800u)   /* RTC Prescale Timer 1 Clock Divide: /4 */
#define RT1PSDIV_2          (0x1000u)   /* RTC Prescale Timer 1 Clock Divide: /8 */
#define RT1PSDIV_3          (0x1800u)   /* RTC Prescale Timer 1 Clock Divide: /16 */
#define RT1PSDIV_4          (0x2000u)   /* RTC Prescale Timer 1 Clock Divide: /32 */
#define RT1PSDIV_5          (0x2800u)   /* RTC Prescale Timer 1 Clock Divide: /64 */
#define RT1PSDIV_6          (0x3000u)   /* RTC Prescale Timer 1 Clock Divide: /128 */
#define RT1PSDIV_7          (0x3800u)   /* RTC Prescale Timer 1 Clock Divide: /256 */

#define RT1SSEL_0           (0x0000u)   /* RTC Prescale Timer 1 Source Select: 0 */
#define RT1SSEL_1           (0x4000u)   /* RTC Prescale Timer 1 Source Select: 1 */
#define RT1SSEL_2           (0x8000u)   /* RTC Prescale Timer 1 Source Select: 2 */
#define RT1SSEL_3           (0xC000u)   /* RTC Prescale Timer 1 Source Select: 3 */

/* RTC Definitions */
#define RTCIV_NONE         (0x0000u)    /* No Interrupt pending */
#define RTCIV_RTCOFIFG     (0x0002u)    /* RTC Osc fault: RTCOFIFG */
#define RTCIV_RTCRDYIFG    (0x0004u)    /* RTC ready: RTCRDYIFG */
#define RTCIV_RTCTEVIFG    (0x0006u)    /* RTC interval timer: RTCTEVIFG */
#define RTCIV_RTCAIFG      (0x0008u)    /* RTC user alarm: RTCAIFG */
#define RTCIV_RT0PSIFG     (0x000Au)    /* RTC prescaler 0: RT0PSIFG */
#define RTCIV_RT1PSIFG     (0x000Cu)    /* RTC prescaler 1: RT1PSIFG */

/* Legacy Definitions */
#define RTC_NONE           (0x0000u)    /* No Interrupt pending */
#define RTC_RTCOFIFG       (0x0002u)    /* RTC Osc fault: RTCOFIFG */
#define RTC_RTCRDYIFG      (0x0004u)    /* RTC ready: RTCRDYIFG */
#define RTC_RTCTEVIFG      (0x0006u)    /* RTC interval timer: RTCTEVIFG */
#define RTC_RTCAIFG        (0x0008u)    /* RTC user alarm: RTCAIFG */
#define RTC_RT0PSIFG       (0x000Au)    /* RTC prescaler 0: RT0PSIFG */
#define RTC_RT1PSIFG       (0x000Cu)    /* RTC prescaler 1: RT1PSIFG */

/************************************************************
* SFR - Special Function Register Module
************************************************************/
#define __MSP430_HAS_SFR__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SFR__ 0x0100
#define SFR_BASE __MSP430_BASEADDRESS_SFR__

#define  SFRIE1_                (0x0100u)  /* Interrupt Enable 1 */
DEFCW(   SFRIE1               , SFRIE1_)

/* SFRIE1 Control Bits */
#define WDTIE               (0x0001u)  /* WDT Interrupt Enable */
#define OFIE                (0x0002u)  /* Osc Fault Enable */
//#define Reserved          (0x0004u)
#define VMAIE               (0x0008u)  /* Vacant Memory Interrupt Enable */
#define NMIIE               (0x0010u)  /* NMI Interrupt Enable */
#define JMBINIE             (0x0040u)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE            (0x0080u)  /* JTAG Mail Box output Interrupt Enable */

#define WDTIE_L             (0x0001u)  /* WDT Interrupt Enable */
#define OFIE_L              (0x0002u)  /* Osc Fault Enable */
//#define Reserved          (0x0004u)
#define VMAIE_L             (0x0008u)  /* Vacant Memory Interrupt Enable */
#define NMIIE_L             (0x0010u)  /* NMI Interrupt Enable */
#define JMBINIE_L           (0x0040u)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE_L          (0x0080u)  /* JTAG Mail Box output Interrupt Enable */

#define  SFRIFG1_               (0x0102u)  /* Interrupt Flag 1 */
DEFCW(   SFRIFG1              , SFRIFG1_)
/* SFRIFG1 Control Bits */
#define WDTIFG              (0x0001u)  /* WDT Interrupt Flag */
#define OFIFG               (0x0002u)  /* Osc Fault Flag */
//#define Reserved          (0x0004u)
#define VMAIFG              (0x0008u)  /* Vacant Memory Interrupt Flag */
#define NMIIFG              (0x0010u)  /* NMI Interrupt Flag */
//#define Reserved          (0x0020u)
#define JMBINIFG            (0x0040u)  /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG           (0x0080u)  /* JTAG Mail Box output Interrupt Flag */

#define WDTIFG_L            (0x0001u)  /* WDT Interrupt Flag */
#define OFIFG_L             (0x0002u)  /* Osc Fault Flag */
//#define Reserved          (0x0004u)
#define VMAIFG_L            (0x0008u)  /* Vacant Memory Interrupt Flag */
#define NMIIFG_L            (0x0010u)  /* NMI Interrupt Flag */
//#define Reserved          (0x0020u)
#define JMBINIFG_L          (0x0040u)  /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG_L         (0x0080u)  /* JTAG Mail Box output Interrupt Flag */

#define  SFRRPCR_               (0x0104u)  /* RESET Pin Control Register */
DEFCW(   SFRRPCR              , SFRRPCR_)
/* SFRRPCR Control Bits */
#define SYSNMI              (0x0001u)  /* NMI select */
#define SYSNMIIES           (0x0002u)  /* NMI edge select */
#define SYSRSTUP            (0x0004u)  /* RESET Pin pull down/up select */
#define SYSRSTRE            (0x0008u)  /* RESET Pin Resistor enable */

#define SYSNMI_L            (0x0001u)  /* NMI select */
#define SYSNMIIES_L         (0x0002u)  /* NMI edge select */
#define SYSRSTUP_L          (0x0004u)  /* RESET Pin pull down/up select */
#define SYSRSTRE_L          (0x0008u)  /* RESET Pin Resistor enable */

/************************************************************
* SYS - System Module
************************************************************/
#define __MSP430_HAS_SYS__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SYS__ 0x0180
#define SYS_BASE __MSP430_BASEADDRESS_SYS__

#define  SYSCTL_                (0x0180u)  /* System control */
DEFCW(   SYSCTL               , SYSCTL_)
#define  SYSJMBC_               (0x0186u)  /* JTAG mailbox control */
DEFCW(   SYSJMBC              , SYSJMBC_)
#define  SYSJMBI0_              (0x0188u)  /* JTAG mailbox input 0 */
DEFCW(   SYSJMBI0             , SYSJMBI0_)
#define  SYSJMBI1_              (0x018Au)  /* JTAG mailbox input 1 */
DEFCW(   SYSJMBI1             , SYSJMBI1_)
#define  SYSJMBO0_              (0x018Cu)  /* JTAG mailbox output 0 */
DEFCW(   SYSJMBO0             , SYSJMBO0_)
#define  SYSJMBO1_              (0x018Eu)  /* JTAG mailbox output 1 */
DEFCW(   SYSJMBO1             , SYSJMBO1_)

#define  SYSUNIV_               (0x019Au)  /* User NMI vector generator */
DEFCW(   SYSUNIV              , SYSUNIV_)
#define  SYSSNIV_               (0x019Cu)  /* System NMI vector generator */
DEFCW(   SYSSNIV              , SYSSNIV_)
#define  SYSRSTIV_              (0x019Eu)  /* Reset vector generator */
DEFCW(   SYSRSTIV             , SYSRSTIV_)

/* SYSCTL Control Bits */
#define SYSRIVECT           (0x0001u)  /* SYS - RAM based interrupt vectors */
//#define RESERVED            (0x0002u)  /* SYS - Reserved */
#define SYSPMMPE            (0x0004u)  /* SYS - PMM access protect */
//#define RESERVED            (0x0008u)  /* SYS - Reserved */
#define SYSBSLIND           (0x0010u)  /* SYS - TCK/RST indication detected */
#define SYSJTAGPIN          (0x0020u)  /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED            (0x0040u)  /* SYS - Reserved */
//#define RESERVED            (0x0080u)  /* SYS - Reserved */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
//#define RESERVED            (0x4000u)  /* SYS - Reserved */
//#define RESERVED            (0x8000u)  /* SYS - Reserved */

/* SYSCTL Control Bits */
#define SYSRIVECT_L         (0x0001u)  /* SYS - RAM based interrupt vectors */
//#define RESERVED            (0x0002u)  /* SYS - Reserved */
#define SYSPMMPE_L          (0x0004u)  /* SYS - PMM access protect */
//#define RESERVED            (0x0008u)  /* SYS - Reserved */
#define SYSBSLIND_L         (0x0010u)  /* SYS - TCK/RST indication detected */
#define SYSJTAGPIN_L        (0x0020u)  /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED            (0x0040u)  /* SYS - Reserved */
//#define RESERVED            (0x0080u)  /* SYS - Reserved */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
//#define RESERVED            (0x4000u)  /* SYS - Reserved */
//#define RESERVED            (0x8000u)  /* SYS - Reserved */

/* SYSJMBC Control Bits */
#define JMBIN0FG            (0x0001u)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG            (0x0002u)  /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG           (0x0004u)  /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG           (0x0008u)  /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE             (0x0010u)  /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED            (0x0020u)  /* SYS - Reserved */
#define JMBCLR0OFF          (0x0040u)  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF          (0x0080u)  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
//#define RESERVED            (0x4000u)  /* SYS - Reserved */
//#define RESERVED            (0x8000u)  /* SYS - Reserved */

/* SYSJMBC Control Bits */
#define JMBIN0FG_L          (0x0001u)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG_L          (0x0002u)  /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG_L         (0x0004u)  /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG_L         (0x0008u)  /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE_L           (0x0010u)  /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED            (0x0020u)  /* SYS - Reserved */
#define JMBCLR0OFF_L        (0x0040u)  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF_L        (0x0080u)  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
//#define RESERVED            (0x4000u)  /* SYS - Reserved */
//#define RESERVED            (0x8000u)  /* SYS - Reserved */


/* SYSUNIV Definitions */
#define SYSUNIV_NONE       (0x0000u)    /* No Interrupt pending */
#define SYSUNIV_NMIIFG     (0x0002u)    /* SYSUNIV : NMIIFG */
#define SYSUNIV_OFIFG      (0x0004u)    /* SYSUNIV : Osc. Fail - OFIFG */

/* SYSSNIV Definitions */
#define SYSSNIV_NONE       (0x0000u)    /* No Interrupt pending */
#define SYSSNIV_SVS        (0x0002u)    /* SYSSNIV : SVS low_power reset */
#define SYSSNIV_UBDIFG     (0x0004u)    /* SYSSNIV : FRAM Uncorrectable bit Error */
#define SYSSNIV_ACCTEIFG   (0x0006u)    /* SYSSNIV : Access time error */
#define SYSSNIV_MPUSEGPIFG (0x0008u)    /* SYSSNIV : MPUSEGPIFG violation */
#define SYSSNIV_MPUSEGIIFG (0x000Au)    /* SYSSNIV : MPUSEGIIFG violation */
#define SYSSNIV_MPUSEG1IFG (0x000Cu)    /* SYSSNIV : MPUSEG1IFG violation */
#define SYSSNIV_MPUSEG2IFG (0x000Eu)    /* SYSSNIV : MPUSEG2IFG violation */
#define SYSSNIV_MPUSEG3IFG (0x0010u)    /* SYSSNIV : MPUSEG3IFG violation */
#define SYSSNIV_VMAIFG     (0x0012u)    /* SYSSNIV : VMAIFG */
#define SYSSNIV_JMBINIFG   (0x0014u)    /* SYSSNIV : JMBINIFG */
#define SYSSNIV_JMBOUTIFG  (0x0016u)    /* SYSSNIV : JMBOUTIFG */
#define SYSSNIV_CBDIFG     (0x0018u)    /* SYSSNIV : FRAM Correctable Bit error */

/* SYSRSTIV Definitions */
#define SYSRSTIV_NONE      (0x0000u)    /* No Interrupt pending */
#define SYSRSTIV_BOR       (0x0002u)    /* SYSRSTIV : BOR */
#define SYSRSTIV_RSTNMI    (0x0004u)    /* SYSRSTIV : RST/NMI */
#define SYSRSTIV_DOBOR     (0x0006u)    /* SYSRSTIV : Do BOR */
#define SYSRSTIV_LPM5WU    (0x0008u)    /* SYSRSTIV : Port LPM5 Wake Up */
#define SYSRSTIV_SECYV     (0x000Au)    /* SYSRSTIV : Security violation */
#define SYSRSTIV_RES0C     (0x000Cu)    /* SYSRSTIV : Reserved */
#define SYSRSTIV_SVSHIFG   (0x000Eu)    /* SYSRSTIV : SVSHIFG */
#define SYSRSTIV_RES10     (0x0010u)    /* SYSRSTIV : Reserved */
#define SYSRSTIV_RES12     (0x0012u)    /* SYSRSTIV : Reserved */
#define SYSRSTIV_DOPOR     (0x0014u)    /* SYSRSTIV : Do POR */
#define SYSRSTIV_WDTTO     (0x0016u)    /* SYSRSTIV : WDT Time out */
#define SYSRSTIV_WDTKEY    (0x0018u)    /* SYSRSTIV : WDTKEY violation */
#define SYSRSTIV_FRCTLPW   (0x001Au)    /* SYSRSTIV : FRAM Key violation */
#define SYSRSTIV_UBDIFG    (0x001Cu)    /* SYSRSTIV : FRAM Uncorrectable bit Error */
#define SYSRSTIV_PERF      (0x001Eu)    /* SYSRSTIV : peripheral/config area fetch */
#define SYSRSTIV_PMMPW     (0x0020u)    /* SYSRSTIV : PMM Password violation */
#define SYSRSTIV_MPUPW     (0x0022u)    /* SYSRSTIV : MPU Password violation */
#define SYSRSTIV_CSPW      (0x0024u)    /* SYSRSTIV : CS Password violation */
#define SYSRSTIV_MPUSEGPIFG (0x0026u)   /* SYSRSTIV : MPUSEGPIFG violation */
#define SYSRSTIV_MPUSEGIIFG (0x0028u)   /* SYSRSTIV : MPUSEGIIFG violation */
#define SYSRSTIV_MPUSEG1IFG (0x002Au)   /* SYSRSTIV : MPUSEG1IFG violation */
#define SYSRSTIV_MPUSEG2IFG (0x002Cu)   /* SYSRSTIV : MPUSEG2IFG violation */
#define SYSRSTIV_MPUSEG3IFG (0x002Eu)   /* SYSRSTIV : MPUSEG3IFG violation */
#define SYSRSTIV_ACCTEIFG  (0x0030u)    /* SYSRSTIV : ACCTEIFG access time error */

/************************************************************
* Timer0_A3
************************************************************/
#define __MSP430_HAS_T0A3__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T0A3__ 0x0340
#define TIMER_A0_BASE __MSP430_BASEADDRESS_T0A3__

#define  TA0CTL_                (0x0340u)  /* Timer0_A3 Control */
DEFW(    TA0CTL               , TA0CTL_)
#define  TA0CCTL0_              (0x0342u)  /* Timer0_A3 Capture/Compare Control 0 */
DEFW(    TA0CCTL0             , TA0CCTL0_)
#define  TA0CCTL1_              (0x0344u)  /* Timer0_A3 Capture/Compare Control 1 */
DEFW(    TA0CCTL1             , TA0CCTL1_)
#define  TA0CCTL2_              (0x0346u)  /* Timer0_A3 Capture/Compare Control 2 */
DEFW(    TA0CCTL2             , TA0CCTL2_)
#define  TA0R_                  (0x0350u)  /* Timer0_A3 */
DEFW(    TA0R                 , TA0R_)
#define  TA0CCR0_               (0x0352u)  /* Timer0_A3 Capture/Compare 0 */
DEFW(    TA0CCR0              , TA0CCR0_)
#define  TA0CCR1_               (0x0354u)  /* Timer0_A3 Capture/Compare 1 */
DEFW(    TA0CCR1              , TA0CCR1_)
#define  TA0CCR2_               (0x0356u)  /* Timer0_A3 Capture/Compare 2 */
DEFW(    TA0CCR2              , TA0CCR2_)
#define  TA0IV_                 (0x036Eu)  /* Timer0_A3 Interrupt Vector Word */
DEFW(    TA0IV                , TA0IV_)
#define  TA0EX0_                (0x0360u)  /* Timer0_A3 Expansion Register 0 */
DEFW(    TA0EX0               , TA0EX0_)

/* TAxCTL Control Bits */
#define TASSEL1             (0x0200u)  /* Timer A clock source select 1 */
#define TASSEL0             (0x0100u)  /* Timer A clock source select 0 */
#define ID1                 (0x0080u)  /* Timer A clock input divider 1 */
#define ID0                 (0x0040u)  /* Timer A clock input divider 0 */
#define MC1                 (0x0020u)  /* Timer A mode control 1 */
#define MC0                 (0x0010u)  /* Timer A mode control 0 */
#define TACLR               (0x0004u)  /* Timer A counter clear */
#define TAIE                (0x0002u)  /* Timer A counter interrupt enable */
#define TAIFG               (0x0001u)  /* Timer A counter interrupt flag */

#define MC_0                (0*0x10u)  /* Timer A mode control: 0 - Stop */
#define MC_1                (1*0x10u)  /* Timer A mode control: 1 - Up to CCR0 */
#define MC_2                (2*0x10u)  /* Timer A mode control: 2 - Continuous up */
#define MC_3                (3*0x10u)  /* Timer A mode control: 3 - Up/Down */
#define ID_0                (0*0x40u)  /* Timer A input divider: 0 - /1 */
#define ID_1                (1*0x40u)  /* Timer A input divider: 1 - /2 */
#define ID_2                (2*0x40u)  /* Timer A input divider: 2 - /4 */
#define ID_3                (3*0x40u)  /* Timer A input divider: 3 - /8 */
#define TASSEL_0            (0*0x100u) /* Timer A clock source select: 0 - TACLK */
#define TASSEL_1            (1*0x100u) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL_2            (2*0x100u) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL_3            (3*0x100u) /* Timer A clock source select: 3 - INCLK */
#define MC__STOP            (0*0x10u)  /* Timer A mode control: 0 - Stop */
#define MC__UP              (1*0x10u)  /* Timer A mode control: 1 - Up to CCR0 */
#define MC__CONTINUOUS      (2*0x10u)  /* Timer A mode control: 2 - Continuous up */
#define MC__CONTINOUS       (2*0x10u)  /* Legacy define */
#define MC__UPDOWN          (3*0x10u)  /* Timer A mode control: 3 - Up/Down */
#define ID__1               (0*0x40u)  /* Timer A input divider: 0 - /1 */
#define ID__2               (1*0x40u)  /* Timer A input divider: 1 - /2 */
#define ID__4               (2*0x40u)  /* Timer A input divider: 2 - /4 */
#define ID__8               (3*0x40u)  /* Timer A input divider: 3 - /8 */
#define TASSEL__TACLK       (0*0x100u) /* Timer A clock source select: 0 - TACLK */
#define TASSEL__ACLK        (1*0x100u) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL__SMCLK       (2*0x100u) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL__INCLK       (3*0x100u) /* Timer A clock source select: 3 - INCLK */

/* TAxCCTLx Control Bits */
#define CM1                 (0x8000u)  /* Capture mode 1 */
#define CM0                 (0x4000u)  /* Capture mode 0 */
#define CCIS1               (0x2000u)  /* Capture input select 1 */
#define CCIS0               (0x1000u)  /* Capture input select 0 */
#define SCS                 (0x0800u)  /* Capture sychronize */
#define SCCI                (0x0400u)  /* Latched capture signal (read) */
#define CAP                 (0x0100u)  /* Capture mode: 1 /Compare mode : 0 */
#define OUTMOD2             (0x0080u)  /* Output mode 2 */
#define OUTMOD1             (0x0040u)  /* Output mode 1 */
#define OUTMOD0             (0x0020u)  /* Output mode 0 */
#define CCIE                (0x0010u)  /* Capture/compare interrupt enable */
#define CCI                 (0x0008u)  /* Capture input signal (read) */
#define OUT                 (0x0004u)  /* PWM Output signal if output mode 0 */
#define COV                 (0x0002u)  /* Capture/compare overflow flag */
#define CCIFG               (0x0001u)  /* Capture/compare interrupt flag */

#define OUTMOD_0            (0*0x20u)  /* PWM output mode: 0 - output only */
#define OUTMOD_1            (1*0x20u)  /* PWM output mode: 1 - set */
#define OUTMOD_2            (2*0x20u)  /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3            (3*0x20u)  /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4            (4*0x20u)  /* PWM output mode: 4 - toggle */
#define OUTMOD_5            (5*0x20u)  /* PWM output mode: 5 - Reset */
#define OUTMOD_6            (6*0x20u)  /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7            (7*0x20u)  /* PWM output mode: 7 - PWM reset/set */
#define CCIS_0              (0*0x1000u) /* Capture input select: 0 - CCIxA */
#define CCIS_1              (1*0x1000u) /* Capture input select: 1 - CCIxB */
#define CCIS_2              (2*0x1000u) /* Capture input select: 2 - GND */
#define CCIS_3              (3*0x1000u) /* Capture input select: 3 - Vcc */
#define CM_0                (0*0x4000u) /* Capture mode: 0 - disabled */
#define CM_1                (1*0x4000u) /* Capture mode: 1 - pos. edge */
#define CM_2                (2*0x4000u) /* Capture mode: 1 - neg. edge */
#define CM_3                (3*0x4000u) /* Capture mode: 1 - both edges */

/* TAxEX0 Control Bits */
#define TAIDEX0             (0x0001u)  /* Timer A Input divider expansion Bit: 0 */
#define TAIDEX1             (0x0002u)  /* Timer A Input divider expansion Bit: 1 */
#define TAIDEX2             (0x0004u)  /* Timer A Input divider expansion Bit: 2 */

#define TAIDEX_0            (0*0x0001u) /* Timer A Input divider expansion : /1 */
#define TAIDEX_1            (1*0x0001u) /* Timer A Input divider expansion : /2 */
#define TAIDEX_2            (2*0x0001u) /* Timer A Input divider expansion : /3 */
#define TAIDEX_3            (3*0x0001u) /* Timer A Input divider expansion : /4 */
#define TAIDEX_4            (4*0x0001u) /* Timer A Input divider expansion : /5 */
#define TAIDEX_5            (5*0x0001u) /* Timer A Input divider expansion : /6 */
#define TAIDEX_6            (6*0x0001u) /* Timer A Input divider expansion : /7 */
#define TAIDEX_7            (7*0x0001u) /* Timer A Input divider expansion : /8 */

/* T0A3IV Definitions */
#define TA0IV_NONE          (0x0000u)    /* No Interrupt pending */
#define TA0IV_TACCR1        (0x0002u)    /* TA0CCR1_CCIFG */
#define TA0IV_TACCR2        (0x0004u)    /* TA0CCR2_CCIFG */
#define TA0IV_3             (0x0006u)    /* Reserved */
#define TA0IV_4             (0x0008u)    /* Reserved */
#define TA0IV_5             (0x000Au)    /* Reserved */
#define TA0IV_6             (0x000Cu)    /* Reserved */
#define TA0IV_TAIFG         (0x000Eu)    /* TA0IFG */

/* Legacy Defines */
#define TA0IV_TA0CCR1       (0x0002u)    /* TA0CCR1_CCIFG */
#define TA0IV_TA0CCR2       (0x0004u)    /* TA0CCR2_CCIFG */
#define TA0IV_TA0IFG        (0x000Eu)    /* TA0IFG */

/************************************************************
* Timer1_A3
************************************************************/
#define __MSP430_HAS_T1A3__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T1A3__ 0x0380
#define TIMER_A1_BASE __MSP430_BASEADDRESS_T1A3__

#define  TA1CTL_                (0x0380u)  /* Timer1_A3 Control */
DEFW(    TA1CTL               , TA1CTL_)
#define  TA1CCTL0_              (0x0382u)  /* Timer1_A3 Capture/Compare Control 0 */
DEFW(    TA1CCTL0             , TA1CCTL0_)
#define  TA1CCTL1_              (0x0384u)  /* Timer1_A3 Capture/Compare Control 1 */
DEFW(    TA1CCTL1             , TA1CCTL1_)
#define  TA1CCTL2_              (0x0386u)  /* Timer1_A3 Capture/Compare Control 2 */
DEFW(    TA1CCTL2             , TA1CCTL2_)
#define  TA1R_                  (0x0390u)  /* Timer1_A3 */
DEFW(    TA1R                 , TA1R_)
#define  TA1CCR0_               (0x0392u)  /* Timer1_A3 Capture/Compare 0 */
DEFW(    TA1CCR0              , TA1CCR0_)
#define  TA1CCR1_               (0x0394u)  /* Timer1_A3 Capture/Compare 1 */
DEFW(    TA1CCR1              , TA1CCR1_)
#define  TA1CCR2_               (0x0396u)  /* Timer1_A3 Capture/Compare 2 */
DEFW(    TA1CCR2              , TA1CCR2_)
#define  TA1IV_                 (0x03AEu)  /* Timer1_A3 Interrupt Vector Word */
DEFW(    TA1IV                , TA1IV_)
#define  TA1EX0_                (0x03A0u)  /* Timer1_A3 Expansion Register 0 */
DEFW(    TA1EX0               , TA1EX0_)

/* Bits are already defined within the Timer0_Ax */

/* TA1IV Definitions */
#define TA1IV_NONE          (0x0000u)    /* No Interrupt pending */
#define TA1IV_TACCR1        (0x0002u)    /* TA1CCR1_CCIFG */
#define TA1IV_TACCR2        (0x0004u)    /* TA1CCR2_CCIFG */
#define TA1IV_3             (0x0006u)    /* Reserved */
#define TA1IV_4             (0x0008u)    /* Reserved */
#define TA1IV_5             (0x000Au)    /* Reserved */
#define TA1IV_6             (0x000Cu)    /* Reserved */
#define TA1IV_TAIFG         (0x000Eu)    /* TA1IFG */

/* Legacy Defines */
#define TA1IV_TA1CCR1      (0x0002u)    /* TA1CCR1_CCIFG */
#define TA1IV_TA1CCR2      (0x0004u)    /* TA1CCR2_CCIFG */
#define TA1IV_TA1IFG       (0x000Eu)    /* TA1IFG */

/************************************************************
* Timer2_A2
************************************************************/
#define __MSP430_HAS_T2A2__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T2A2__ 0x0400
#define TIMER_A2_BASE __MSP430_BASEADDRESS_T2A2__

#define  TA2CTL_                (0x0400u)  /* Timer2_A2 Control */
DEFW(    TA2CTL               , TA2CTL_)
#define  TA2CCTL0_              (0x0402u)  /* Timer2_A2 Capture/Compare Control 0 */
DEFW(    TA2CCTL0             , TA2CCTL0_)
#define  TA2CCTL1_              (0x0404u)  /* Timer2_A2 Capture/Compare Control 1 */
DEFW(    TA2CCTL1             , TA2CCTL1_)
#define  TA2R_                  (0x0410u)  /* Timer2_A2 */
DEFW(    TA2R                 , TA2R_)
#define  TA2CCR0_               (0x0412u)  /* Timer2_A2 Capture/Compare 0 */
DEFW(    TA2CCR0              , TA2CCR0_)
#define  TA2CCR1_               (0x0414u)  /* Timer2_A2 Capture/Compare 1 */
DEFW(    TA2CCR1              , TA2CCR1_)
#define  TA2IV_                 (0x042Eu)  /* Timer2_A2 Interrupt Vector Word */
DEFW(    TA2IV                , TA2IV_)
#define  TA2EX0_                (0x0420u)  /* Timer2_A2 Expansion Register 0 */
DEFW(    TA2EX0               , TA2EX0_)

/* Bits are already defined within the Timer0_Ax */

/* TA2IV Definitions */
#define TA2IV_NONE          (0x0000u)    /* No Interrupt pending */
#define TA2IV_TACCR1        (0x0002u)    /* TA2CCR1_CCIFG */
#define TA2IV_3             (0x0006u)    /* Reserved */
#define TA2IV_4             (0x0008u)    /* Reserved */
#define TA2IV_5             (0x000Au)    /* Reserved */
#define TA2IV_6             (0x000Cu)    /* Reserved */
#define TA2IV_TAIFG         (0x000Eu)    /* TA2IFG */

/* Legacy Defines */
#define TA2IV_TA2CCR1      (0x0002u)    /* TA2CCR1_CCIFG */
#define TA2IV_TA2IFG       (0x000Eu)    /* TA2IFG */

/************************************************************
* Timer3_A5
************************************************************/
#define __MSP430_HAS_T3A5__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T3A5__ 0x0440
#define TIMER_A3_BASE __MSP430_BASEADDRESS_T3A5__

#define  TA3CTL_                (0x0440u)  /* Timer3_A5 Control */
DEFW(    TA3CTL               , TA3CTL_)
#define  TA3CCTL0_              (0x0442u)  /* Timer3_A5 Capture/Compare Control 0 */
DEFW(    TA3CCTL0             , TA3CCTL0_)
#define  TA3CCTL1_              (0x0444u)  /* Timer3_A5 Capture/Compare Control 1 */
DEFW(    TA3CCTL1             , TA3CCTL1_)
#define  TA3CCTL2_              (0x0446u)  /* Timer3_A5 Capture/Compare Control 2 */
DEFW(    TA3CCTL2             , TA3CCTL2_)
#define  TA3CCTL3_              (0x0448u)  /* Timer3_A5 Capture/Compare Control 3 */
DEFW(    TA3CCTL3             , TA3CCTL3_)
#define  TA3CCTL4_              (0x044Au)  /* Timer3_A5 Capture/Compare Control 4 */
DEFW(    TA3CCTL4             , TA3CCTL4_)
#define  TA3R_                  (0x0450u)  /* Timer3_A5 */
DEFW(    TA3R                 , TA3R_)
#define  TA3CCR0_               (0x0452u)  /* Timer3_A5 Capture/Compare 0 */
DEFW(    TA3CCR0              , TA3CCR0_)
#define  TA3CCR1_               (0x0454u)  /* Timer3_A5 Capture/Compare 1 */
DEFW(    TA3CCR1              , TA3CCR1_)
#define  TA3CCR2_               (0x0456u)  /* Timer3_A5 Capture/Compare 2 */
DEFW(    TA3CCR2              , TA3CCR2_)
#define  TA3CCR3_               (0x0458u)  /* Timer3_A5 Capture/Compare 3 */
DEFW(    TA3CCR3              , TA3CCR3_)
#define  TA3CCR4_               (0x045Au)  /* Timer3_A5 Capture/Compare 4 */
DEFW(    TA3CCR4              , TA3CCR4_)
#define  TA3IV_                 (0x046Eu)  /* Timer3_A5 Interrupt Vector Word */
DEFW(    TA3IV                , TA3IV_)
#define  TA3EX0_                (0x0460u)  /* Timer3_A5 Expansion Register 0 */
DEFW(    TA3EX0               , TA3EX0_)

/* Bits are already defined within the Timer0_Ax */

/* TA3IV Definitions */
#define TA3IV_NONE          (0x0000u)    /* No Interrupt pending */
#define TA3IV_TACCR1        (0x0002u)    /* TA3CCR1_CCIFG */
#define TA3IV_TACCR2        (0x0004u)    /* TA3CCR2_CCIFG */
#define TA3IV_TACCR3        (0x0006u)    /* TA3CCR3_CCIFG */
#define TA3IV_TACCR4        (0x0008u)    /* TA3CCR4_CCIFG */
#define TA3IV_TAIFG         (0x000Eu)    /* TA3IFG */

/* Legacy Defines */
#define TA3IV_TA3CCR1      (0x0002u)    /* TA3CCR1_CCIFG */
#define TA3IV_TA3CCR2      (0x0004u)    /* TA3CCR2_CCIFG */
#define TA3IV_TA3CCR3      (0x0006u)    /* TA3CCR3_CCIFG */
#define TA3IV_TA3CCR4      (0x0008u)    /* TA3CCR4_CCIFG */
#define TA3IV_TA3IFG       (0x000Eu)    /* TA3IFG */

/************************************************************
* Timer0_B7
************************************************************/
#define __MSP430_HAS_T0B7__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T0B7__ 0x03C0
#define TIMER_B0_BASE __MSP430_BASEADDRESS_T0B7__

#define  TB0CTL_                (0x03C0u)  /* Timer0_B7 Control */
DEFW(    TB0CTL               , TB0CTL_)
#define  TB0CCTL0_              (0x03C2u)  /* Timer0_B7 Capture/Compare Control 0 */
DEFW(    TB0CCTL0             , TB0CCTL0_)
#define  TB0CCTL1_              (0x03C4u)  /* Timer0_B7 Capture/Compare Control 1 */
DEFW(    TB0CCTL1             , TB0CCTL1_)
#define  TB0CCTL2_              (0x03C6u)  /* Timer0_B7 Capture/Compare Control 2 */
DEFW(    TB0CCTL2             , TB0CCTL2_)
#define  TB0CCTL3_              (0x03C8u)  /* Timer0_B7 Capture/Compare Control 3 */
DEFW(    TB0CCTL3             , TB0CCTL3_)
#define  TB0CCTL4_              (0x03CAu)  /* Timer0_B7 Capture/Compare Control 4 */
DEFW(    TB0CCTL4             , TB0CCTL4_)
#define  TB0CCTL5_              (0x03CCu)  /* Timer0_B7 Capture/Compare Control 5 */
DEFW(    TB0CCTL5             , TB0CCTL5_)
#define  TB0CCTL6_              (0x03CEu)  /* Timer0_B7 Capture/Compare Control 6 */
DEFW(    TB0CCTL6             , TB0CCTL6_)
#define  TB0R_                  (0x03D0u)  /* Timer0_B7 */
DEFW(    TB0R                 , TB0R_)
#define  TB0CCR0_               (0x03D2u)  /* Timer0_B7 Capture/Compare 0 */
DEFW(    TB0CCR0              , TB0CCR0_)
#define  TB0CCR1_               (0x03D4u)  /* Timer0_B7 Capture/Compare 1 */
DEFW(    TB0CCR1              , TB0CCR1_)
#define  TB0CCR2_               (0x03D6u)  /* Timer0_B7 Capture/Compare 2 */
DEFW(    TB0CCR2              , TB0CCR2_)
#define  TB0CCR3_               (0x03D8u)  /* Timer0_B7 Capture/Compare 3 */
DEFW(    TB0CCR3              , TB0CCR3_)
#define  TB0CCR4_               (0x03DAu)  /* Timer0_B7 Capture/Compare 4 */
DEFW(    TB0CCR4              , TB0CCR4_)
#define  TB0CCR5_               (0x03DCu)  /* Timer0_B7 Capture/Compare 5 */
DEFW(    TB0CCR5              , TB0CCR5_)
#define  TB0CCR6_               (0x03DEu)  /* Timer0_B7 Capture/Compare 6 */
DEFW(    TB0CCR6              , TB0CCR6_)
#define  TB0EX0_                (0x03E0u)  /* Timer0_B7 Expansion Register 0 */
DEFW(    TB0EX0               , TB0EX0_)
#define  TB0IV_                 (0x03EEu)  /* Timer0_B7 Interrupt Vector Word */
DEFW(    TB0IV                , TB0IV_)

/* Legacy Type Definitions for TimerB */
#define TBCTL                TB0CTL    /* Timer0_B7 Control */
#define TBCCTL0              TB0CCTL0  /* Timer0_B7 Capture/Compare Control 0 */
#define TBCCTL1              TB0CCTL1  /* Timer0_B7 Capture/Compare Control 1 */
#define TBCCTL2              TB0CCTL2  /* Timer0_B7 Capture/Compare Control 2 */
#define TBCCTL3              TB0CCTL3  /* Timer0_B7 Capture/Compare Control 3 */
#define TBCCTL4              TB0CCTL4  /* Timer0_B7 Capture/Compare Control 4 */
#define TBCCTL5              TB0CCTL5  /* Timer0_B7 Capture/Compare Control 5 */
#define TBCCTL6              TB0CCTL6  /* Timer0_B7 Capture/Compare Control 6 */
#define TBR                  TB0R      /* Timer0_B7 */
#define TBCCR0               TB0CCR0   /* Timer0_B7 Capture/Compare 0 */
#define TBCCR1               TB0CCR1   /* Timer0_B7 Capture/Compare 1 */
#define TBCCR2               TB0CCR2   /* Timer0_B7 Capture/Compare 2 */
#define TBCCR3               TB0CCR3   /* Timer0_B7 Capture/Compare 3 */
#define TBCCR4               TB0CCR4   /* Timer0_B7 Capture/Compare 4 */
#define TBCCR5               TB0CCR5   /* Timer0_B7 Capture/Compare 5 */
#define TBCCR6               TB0CCR6   /* Timer0_B7 Capture/Compare 6 */
#define TBEX0                TB0EX0    /* Timer0_B7 Expansion Register 0 */
#define TBIV                 TB0IV     /* Timer0_B7 Interrupt Vector Word */
#define TIMERB1_VECTOR       TIMER0_B1_VECTOR /* Timer0_B7 CC1-6, TB */
#define TIMERB0_VECTOR       TIMER0_B0_VECTOR /* Timer0_B7 CC0 */

/* TBxCTL Control Bits */
#define TBCLGRP1            (0x4000u)  /* Timer0_B7 Compare latch load group 1 */
#define TBCLGRP0            (0x2000u)  /* Timer0_B7 Compare latch load group 0 */
#define CNTL1               (0x1000u)  /* Counter lenght 1 */
#define CNTL0               (0x0800u)  /* Counter lenght 0 */
#define TBSSEL1             (0x0200u)  /* Clock source 1 */
#define TBSSEL0             (0x0100u)  /* Clock source 0 */
#define TBCLR               (0x0004u)  /* Timer0_B7 counter clear */
#define TBIE                (0x0002u)  /* Timer0_B7 interrupt enable */
#define TBIFG               (0x0001u)  /* Timer0_B7 interrupt flag */

#define SHR1                (0x4000u)  /* Timer0_B7 Compare latch load group 1 */
#define SHR0                (0x2000u)  /* Timer0_B7 Compare latch load group 0 */

#define TBSSEL_0            (0*0x0100u)  /* Clock Source: TBCLK */
#define TBSSEL_1            (1*0x0100u)  /* Clock Source: ACLK  */
#define TBSSEL_2            (2*0x0100u)  /* Clock Source: SMCLK */
#define TBSSEL_3            (3*0x0100u)  /* Clock Source: INCLK */
#define CNTL_0              (0*0x0800u)  /* Counter lenght: 16 bit */
#define CNTL_1              (1*0x0800u)  /* Counter lenght: 12 bit */
#define CNTL_2              (2*0x0800u)  /* Counter lenght: 10 bit */
#define CNTL_3              (3*0x0800u)  /* Counter lenght:  8 bit */
#define SHR_0               (0*0x2000u)  /* Timer0_B7 Group: 0 - individually */
#define SHR_1               (1*0x2000u)  /* Timer0_B7 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define SHR_2               (2*0x2000u)  /* Timer0_B7 Group: 2 - 2 groups (1-3, 4-6)*/
#define SHR_3               (3*0x2000u)  /* Timer0_B7 Group: 3 - 1 group (all) */
#define TBCLGRP_0           (0*0x2000u)  /* Timer0_B7 Group: 0 - individually */
#define TBCLGRP_1           (1*0x2000u)  /* Timer0_B7 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define TBCLGRP_2           (2*0x2000u)  /* Timer0_B7 Group: 2 - 2 groups (1-3, 4-6)*/
#define TBCLGRP_3           (3*0x2000u)  /* Timer0_B7 Group: 3 - 1 group (all) */
#define TBSSEL__TBCLK       (0*0x100u) /* Timer0_B7 clock source select: 0 - TBCLK */
#define TBSSEL__TACLK       (0*0x100u) /* Timer0_B7 clock source select: 0 - TBCLK (legacy) */
#define TBSSEL__ACLK        (1*0x100u) /* Timer0_B7 clock source select: 1 - ACLK  */
#define TBSSEL__SMCLK       (2*0x100u) /* Timer0_B7 clock source select: 2 - SMCLK */
#define TBSSEL__INCLK       (3*0x100u) /* Timer0_B7 clock source select: 3 - INCLK */
#define CNTL__16            (0*0x0800u)  /* Counter lenght: 16 bit */
#define CNTL__12            (1*0x0800u)  /* Counter lenght: 12 bit */
#define CNTL__10            (2*0x0800u)  /* Counter lenght: 10 bit */
#define CNTL__8             (3*0x0800u)  /* Counter lenght:  8 bit */

/* Additional Timer B Control Register bits are defined in Timer A */
/* TBxCCTLx Control Bits */
#define CLLD1               (0x0400u)  /* Compare latch load source 1 */
#define CLLD0               (0x0200u)  /* Compare latch load source 0 */

#define SLSHR1              (0x0400u)  /* Compare latch load source 1 */
#define SLSHR0              (0x0200u)  /* Compare latch load source 0 */

#define SLSHR_0             (0*0x0200u) /* Compare latch load sourec : 0 - immediate */
#define SLSHR_1             (1*0x0200u) /* Compare latch load sourec : 1 - TBR counts to 0 */
#define SLSHR_2             (2*0x0200u) /* Compare latch load sourec : 2 - up/down */
#define SLSHR_3             (3*0x0200u) /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

#define CLLD_0              (0*0x0200u) /* Compare latch load sourec : 0 - immediate */
#define CLLD_1              (1*0x0200u) /* Compare latch load sourec : 1 - TBR counts to 0 */
#define CLLD_2              (2*0x0200u) /* Compare latch load sourec : 2 - up/down */
#define CLLD_3              (3*0x0200u) /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

/* TBxEX0 Control Bits */
#define TBIDEX0             (0x0001u)   /* Timer0_B7 Input divider expansion Bit: 0 */
#define TBIDEX1             (0x0002u)   /* Timer0_B7 Input divider expansion Bit: 1 */
#define TBIDEX2             (0x0004u)   /* Timer0_B7 Input divider expansion Bit: 2 */

#define TBIDEX_0            (0*0x0001u) /* Timer0_B7 Input divider expansion : /1 */
#define TBIDEX_1            (1*0x0001u) /* Timer0_B7 Input divider expansion : /2 */
#define TBIDEX_2            (2*0x0001u) /* Timer0_B7 Input divider expansion : /3 */
#define TBIDEX_3            (3*0x0001u) /* Timer0_B7 Input divider expansion : /4 */
#define TBIDEX_4            (4*0x0001u) /* Timer0_B7 Input divider expansion : /5 */
#define TBIDEX_5            (5*0x0001u) /* Timer0_B7 Input divider expansion : /6 */
#define TBIDEX_6            (6*0x0001u) /* Timer0_B7 Input divider expansion : /7 */
#define TBIDEX_7            (7*0x0001u) /* Timer0_B7 Input divider expansion : /8 */
#define TBIDEX__1           (0*0x0001u) /* Timer0_B7 Input divider expansion : /1 */
#define TBIDEX__2           (1*0x0001u) /* Timer0_B7 Input divider expansion : /2 */
#define TBIDEX__3           (2*0x0001u) /* Timer0_B7 Input divider expansion : /3 */
#define TBIDEX__4           (3*0x0001u) /* Timer0_B7 Input divider expansion : /4 */
#define TBIDEX__5           (4*0x0001u) /* Timer0_B7 Input divider expansion : /5 */
#define TBIDEX__6           (5*0x0001u) /* Timer0_B7 Input divider expansion : /6 */
#define TBIDEX__7           (6*0x0001u) /* Timer0_B7 Input divider expansion : /7 */
#define TBIDEX__8           (7*0x0001u) /* Timer0_B7 Input divider expansion : /8 */

/* TB0IV Definitions */
#define TB0IV_NONE          (0x0000u)    /* No Interrupt pending */
#define TB0IV_TBCCR1        (0x0002u)    /* TB0CCR1_CCIFG */
#define TB0IV_TBCCR2        (0x0004u)    /* TB0CCR2_CCIFG */
#define TB0IV_TBCCR3        (0x0006u)    /* TB0CCR3_CCIFG */
#define TB0IV_TBCCR4        (0x0008u)    /* TB0CCR4_CCIFG */
#define TB0IV_TBCCR5        (0x000Au)    /* TB0CCR5_CCIFG */
#define TB0IV_TBCCR6        (0x000Cu)    /* TB0CCR6_CCIFG */
#define TB0IV_TBIFG         (0x000Eu)    /* TB0IFG */

/* Legacy Defines */
#define TB0IV_TB0CCR1       (0x0002u)    /* TB0CCR1_CCIFG */
#define TB0IV_TB0CCR2       (0x0004u)    /* TB0CCR2_CCIFG */
#define TB0IV_TB0CCR3       (0x0006u)    /* TB0CCR3_CCIFG */
#define TB0IV_TB0CCR4       (0x0008u)    /* TB0CCR4_CCIFG */
#define TB0IV_TB0CCR5       (0x000Au)    /* TB0CCR5_CCIFG */
#define TB0IV_TB0CCR6       (0x000Cu)    /* TB0CCR6_CCIFG */
#define TB0IV_TB0IFG        (0x000Eu)    /* TB0IFG */


/************************************************************
* USCI A0
************************************************************/
#define __MSP430_HAS_EUSCI_A0__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_A0__ 0x05C0
#define EUSCI_A0_BASE __MSP430_BASEADDRESS_EUSCI_A0__

#define UCA0CTLW0_             (0x05C0u)  /* USCI A0 Control Word Register 0 */
DEFCW(  UCA0CTLW0            , UCA0CTLW0_)
#define UCA0CTL1            UCA0CTLW0_L  /* USCI A0 Control Register 1 */
#define UCA0CTL0            UCA0CTLW0_H  /* USCI A0 Control Register 0 */
#define UCA0CTLW1_             (0x05C2u)  /* USCI A0 Control Word Register 1 */
DEFCW(  UCA0CTLW1            , UCA0CTLW1_)
#define UCA0BRW_               (0x05C6u)  /* USCI A0 Baud Word Rate 0 */
DEFCW(  UCA0BRW              , UCA0BRW_)
#define UCA0BR0             UCA0BRW_L /* USCI A0 Baud Rate 0 */
#define UCA0BR1             UCA0BRW_H /* USCI A0 Baud Rate 1 */
#define UCA0MCTLW_             (0x05C8u)  /* USCI A0 Modulation Control */
DEFCW(  UCA0MCTLW            , UCA0MCTLW_)
#define UCA0STATW_             (0x05CAu)  /* USCI A0 Status Register */
DEFC(   UCA0STATW            , UCA0STATW_)
#define UCA0RXBUF_            (0x05CCu)  /* USCI A0 Receive Buffer */
READ_ONLY_DEFCW( UCA0RXBUF        , UCA0RXBUF_)
#define  UCA0TXBUF_            (0x05CEu)  /* USCI A0 Transmit Buffer */
DEFCW(   UCA0TXBUF           , UCA0TXBUF_)
#define UCA0ABCTL_             (0x05D0u)  /* USCI A0 LIN Control */
DEFC(   UCA0ABCTL            , UCA0ABCTL_)
#define UCA0IRCTL_             (0x05D2u)  /* USCI A0 IrDA Transmit Control */
DEFCW(  UCA0IRCTL            , UCA0IRCTL_)
#define UCA0IRTCTL          UCA0IRCTL_L  /* USCI A0 IrDA Transmit Control */
#define UCA0IRRCTL          UCA0IRCTL_H  /* USCI A0 IrDA Receive Control */
#define UCA0IE_                (0x05DAu)  /* USCI A0 Interrupt Enable Register */
DEFCW(  UCA0IE               , UCA0IE_)
#define UCA0IFG_               (0x05DCu)  /* USCI A0 Interrupt Flags Register */
DEFCW(  UCA0IFG              , UCA0IFG_)
#define UCA0IV_                (0x05DEu)  /* USCI A0 Interrupt Vector Register */
DEFW(   UCA0IV               , UCA0IV_)


/************************************************************
* USCI A1
************************************************************/
#define __MSP430_HAS_EUSCI_A1__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_A1__ 0x05E0
#define EUSCI_A1_BASE __MSP430_BASEADDRESS_EUSCI_A1__

#define UCA1CTLW0_             (0x05E0u)  /* USCI A1 Control Word Register 0 */
DEFCW(  UCA1CTLW0            , UCA1CTLW0_)
#define UCA1CTL1            UCA1CTLW0_L  /* USCI A1 Control Register 1 */
#define UCA1CTL0            UCA1CTLW0_H  /* USCI A1 Control Register 0 */
#define UCA1CTLW1_             (0x05E2u)  /* USCI A1 Control Word Register 1 */
DEFCW(  UCA1CTLW1            , UCA1CTLW1_)
#define UCA1BRW_               (0x05E6u)  /* USCI A1 Baud Word Rate 0 */
DEFCW(  UCA1BRW              , UCA1BRW_)
#define UCA1BR0             UCA1BRW_L /* USCI A1 Baud Rate 0 */
#define UCA1BR1             UCA1BRW_H /* USCI A1 Baud Rate 1 */
#define UCA1MCTLW_             (0x05E8u)  /* USCI A1 Modulation Control */
DEFCW(  UCA1MCTLW            , UCA1MCTLW_)
#define UCA1STATW_             (0x05EAu)  /* USCI A1 Status Register */
DEFC(   UCA1STATW            , UCA1STATW_)
#define UCA1RXBUF_            (0x05ECu)  /* USCI A1 Receive Buffer */
READ_ONLY_DEFCW( UCA1RXBUF        , UCA1RXBUF_)
#define  UCA1TXBUF_            (0x05EEu)  /* USCI A1 Transmit Buffer */
DEFCW(   UCA1TXBUF           , UCA1TXBUF_)
#define UCA1ABCTL_             (0x05F0u)  /* USCI A1 LIN Control */
DEFC(   UCA1ABCTL            , UCA1ABCTL_)
#define UCA1IRCTL_             (0x05F2u)  /* USCI A1 IrDA Transmit Control */
DEFCW(  UCA1IRCTL            , UCA1IRCTL_)
#define UCA1IRTCTL          UCA1IRCTL_L  /* USCI A1 IrDA Transmit Control */
#define UCA1IRRCTL          UCA1IRCTL_H  /* USCI A1 IrDA Receive Control */
#define UCA1IE_                (0x05FAu)  /* USCI A1 Interrupt Enable Register */
DEFCW(  UCA1IE               , UCA1IE_)
#define UCA1IFG_               (0x05FCu)  /* USCI A1 Interrupt Flags Register */
DEFCW(  UCA1IFG              , UCA1IFG_)
#define UCA1IV_                (0x05FEu)  /* USCI A1 Interrupt Vector Register */
DEFW(   UCA1IV               , UCA1IV_)


/************************************************************
* USCI B0
************************************************************/
#define __MSP430_HAS_EUSCI_B0__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_B0__ 0x0640
#define EUSCI_B0_BASE __MSP430_BASEADDRESS_EUSCI_B0__


#define UCB0CTLW0_             (0x0640u)  /* USCI B0 Control Word Register 0 */
DEFCW(  UCB0CTLW0            , UCB0CTLW0_)
#define UCB0CTL1            UCB0CTLW0_L  /* USCI B0 Control Register 1 */
#define UCB0CTL0            UCB0CTLW0_H  /* USCI B0 Control Register 0 */
#define UCB0CTLW1_             (0x0642u)  /* USCI B0 Control Word Register 1 */
DEFCW(  UCB0CTLW1            , UCB0CTLW1_)
#define UCB0BRW_               (0x0646u)  /* USCI B0 Baud Word Rate 0 */
DEFCW(  UCB0BRW              , UCB0BRW_)
#define UCB0BR0             UCB0BRW_L /* USCI B0 Baud Rate 0 */
#define UCB0BR1             UCB0BRW_H /* USCI B0 Baud Rate 1 */
#define UCB0STATW_             (0x0648u)  /* USCI B0 Status Word Register */
DEFCW(  UCB0STATW            , UCB0STATW_)
#define UCB0STAT            UCB0STATW_L /* USCI B0 Status Register */
#define UCB0BCNT            UCB0STATW_H /* USCI B0 Byte Counter Register */
#define UCB0TBCNT_             (0x064Au)  /* USCI B0 Byte Counter Threshold Register */
DEFCW(  UCB0TBCNT            , UCB0TBCNT_)
#define UCB0RXBUF_            (0x064Cu)  /* USCI B0 Receive Buffer */
READ_ONLY_DEFCW( UCB0RXBUF        , UCB0RXBUF_)
#define  UCB0TXBUF_            (0x064Eu)  /* USCI B0 Transmit Buffer */
DEFCW(   UCB0TXBUF           , UCB0TXBUF_)
#define UCB0I2COA0_            (0x0654u)  /* USCI B0 I2C Own Address 0 */
DEFCW(  UCB0I2COA0           , UCB0I2COA0_)
#define UCB0I2COA1_            (0x0656u)  /* USCI B0 I2C Own Address 1 */
DEFCW(  UCB0I2COA1           , UCB0I2COA1_)
#define UCB0I2COA2_            (0x0658u)  /* USCI B0 I2C Own Address 2 */
DEFCW(  UCB0I2COA2           , UCB0I2COA2_)
#define UCB0I2COA3_            (0x065Au)  /* USCI B0 I2C Own Address 3 */
DEFCW(  UCB0I2COA3           , UCB0I2COA3_)
#define UCB0ADDRX_             (0x065Cu)  /* USCI B0 Received Address Register */
DEFCW(  UCB0ADDRX            , UCB0ADDRX_)
#define UCB0ADDMASK_           (0x065Eu)  /* USCI B0 Address Mask Register */
DEFCW(  UCB0ADDMASK          , UCB0ADDMASK_)
#define UCB0I2CSA_             (0x0660u)  /* USCI B0 I2C Slave Address */
DEFCW(  UCB0I2CSA            , UCB0I2CSA_)
#define UCB0IE_                (0x066Au)  /* USCI B0 Interrupt Enable Register */
DEFCW(  UCB0IE               , UCB0IE_)
#define UCB0IFG_               (0x066Cu)  /* USCI B0 Interrupt Flags Register */
DEFCW(  UCB0IFG              , UCB0IFG_)
#define UCB0IV_                (0x066Eu)  /* USCI B0 Interrupt Vector Register */
DEFW(   UCB0IV               , UCB0IV_)

/************************************************************
* USCI B1
************************************************************/
#define __MSP430_HAS_EUSCI_B1__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_B1__ 0x0680
#define EUSCI_B1_BASE __MSP430_BASEADDRESS_EUSCI_B1__


#define UCB1CTLW0_             (0x0680u)  /* USCI B1 Control Word Register 0 */
DEFCW(  UCB1CTLW0            , UCB1CTLW0_)
#define UCB1CTL1            UCB1CTLW0_L  /* USCI B1 Control Register 1 */
#define UCB1CTL0            UCB1CTLW0_H  /* USCI B1 Control Register 0 */
#define UCB1CTLW1_             (0x0682u)  /* USCI B1 Control Word Register 1 */
DEFCW(  UCB1CTLW1            , UCB1CTLW1_)
#define UCB1BRW_               (0x0686u)  /* USCI B1 Baud Word Rate 0 */
DEFCW(  UCB1BRW              , UCB1BRW_)
#define UCB1BR0             UCB1BRW_L /* USCI B1 Baud Rate 0 */
#define UCB1BR1             UCB1BRW_H /* USCI B1 Baud Rate 1 */
#define UCB1STATW_             (0x0688u)  /* USCI B1 Status Word Register */
DEFCW(  UCB1STATW            , UCB1STATW_)
#define UCB1STAT            UCB1STATW_L /* USCI B1 Status Register */
#define UCB1BCNT            UCB1STATW_H /* USCI B1 Byte Counter Register */
#define UCB1TBCNT_             (0x068Au)  /* USCI B1 Byte Counter Threshold Register */
DEFCW(  UCB1TBCNT            , UCB1TBCNT_)
#define UCB1RXBUF_            (0x068Cu)  /* USCI B1 Receive Buffer */
READ_ONLY_DEFCW( UCB1RXBUF        , UCB1RXBUF_)
#define  UCB1TXBUF_            (0x068Eu)  /* USCI B1 Transmit Buffer */
DEFCW(   UCB1TXBUF           , UCB1TXBUF_)
#define UCB1I2COA0_            (0x0694u)  /* USCI B1 I2C Own Address 0 */
DEFCW(  UCB1I2COA0           , UCB1I2COA0_)
#define UCB1I2COA1_            (0x0696u)  /* USCI B1 I2C Own Address 1 */
DEFCW(  UCB1I2COA1           , UCB1I2COA1_)
#define UCB1I2COA2_            (0x0698u)  /* USCI B1 I2C Own Address 2 */
DEFCW(  UCB1I2COA2           , UCB1I2COA2_)
#define UCB1I2COA3_            (0x069Au)  /* USCI B1 I2C Own Address 3 */
DEFCW(  UCB1I2COA3           , UCB1I2COA3_)
#define UCB1ADDRX_             (0x069Cu)  /* USCI B1 Received Address Register */
DEFCW(  UCB1ADDRX            , UCB1ADDRX_)
#define UCB1ADDMASK_           (0x069Eu)  /* USCI B1 Address Mask Register */
DEFCW(  UCB1ADDMASK          , UCB1ADDMASK_)
#define UCB1I2CSA_             (0x06A0u)  /* USCI B1 I2C Slave Address */
DEFCW(  UCB1I2CSA            , UCB1I2CSA_)
#define UCB1IE_                (0x06AAu)  /* USCI B1 Interrupt Enable Register */
DEFCW(  UCB1IE               , UCB1IE_)
#define UCB1IFG_               (0x06ACu)  /* USCI B1 Interrupt Flags Register */
DEFCW(  UCB1IFG              , UCB1IFG_)
#define UCB1IV_                (0x06AEu)  /* USCI B1 Interrupt Vector Register */
DEFW(   UCB1IV               , UCB1IV_)

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN               (0x8000u)    /* Async. Mode: Parity enable */
#define UCPAR               (0x4000u)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB               (0x2000u)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT              (0x1000u)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB               (0x0800u)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1             (0x0400u)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0             (0x0200u)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC              (0x0100u)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */
#define UCSSEL1             (0x0080u)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0             (0x0040u)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE             (0x0020u)    /* RX Error interrupt enable */
#define UCBRKIE             (0x0010u)    /* Break interrupt enable */
#define UCDORM              (0x0008u)    /* Dormant (Sleep) Mode */
#define UCTXADDR            (0x0004u)    /* Send next Data as Address */
#define UCTXBRK             (0x0002u)    /* Send next Data as Break */
#define UCSWRST             (0x0001u)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCSSEL1_L           (0x0080u)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0_L           (0x0040u)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE_L           (0x0020u)    /* RX Error interrupt enable */
#define UCBRKIE_L           (0x0010u)    /* Break interrupt enable */
#define UCDORM_L            (0x0008u)    /* Dormant (Sleep) Mode */
#define UCTXADDR_L          (0x0004u)    /* Send next Data as Address */
#define UCTXBRK_L           (0x0002u)    /* Send next Data as Break */
#define UCSWRST_L           (0x0001u)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN_H             (0x0080u)    /* Async. Mode: Parity enable */
#define UCPAR_H             (0x0040u)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB_H             (0x0020u)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT_H            (0x0010u)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB_H             (0x0008u)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1_H           (0x0004u)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0_H           (0x0002u)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC_H            (0x0001u)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */

// UCxxCTLW0 SPI-Mode Control Bits
#define UCCKPH              (0x8000u)    /* Sync. Mode: Clock Phase */
#define UCCKPL              (0x4000u)    /* Sync. Mode: Clock Polarity */
#define UCMST               (0x0800u)    /* Sync. Mode: Master Select */
//#define res               (0x0020u)    /* reserved */
//#define res               (0x0010u)    /* reserved */
//#define res               (0x0008u)    /* reserved */
//#define res               (0x0004u)    /* reserved */
#define UCSTEM             (0x0002u)    /* USCI STE Mode */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10               (0x8000u)    /* 10-bit Address Mode */
#define UCSLA10             (0x4000u)    /* 10-bit Slave Address Mode */
#define UCMM                (0x2000u)    /* Multi-Master Environment */
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */
#define UCTXACK             (0x0020u)    /* Transmit ACK */
#define UCTR                (0x0010u)    /* Transmit/Receive Select/Flag */
#define UCTXNACK            (0x0008u)    /* Transmit NACK */
#define UCTXSTP             (0x0004u)    /* Transmit STOP */
#define UCTXSTT             (0x0002u)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */
#define UCTXACK_L           (0x0020u)    /* Transmit ACK */
#define UCTR_L              (0x0010u)    /* Transmit/Receive Select/Flag */
#define UCTXNACK_L          (0x0008u)    /* Transmit NACK */
#define UCTXSTP_L           (0x0004u)    /* Transmit STOP */
#define UCTXSTT_L           (0x0002u)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10_H             (0x0080u)    /* 10-bit Address Mode */
#define UCSLA10_H           (0x0040u)    /* 10-bit Slave Address Mode */
#define UCMM_H              (0x0020u)    /* Multi-Master Environment */
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */

#define UCMODE_0            (0x0000u)    /* Sync. Mode: USCI Mode: 0 */
#define UCMODE_1            (0x0200u)    /* Sync. Mode: USCI Mode: 1 */
#define UCMODE_2            (0x0400u)    /* Sync. Mode: USCI Mode: 2 */
#define UCMODE_3            (0x0600u)    /* Sync. Mode: USCI Mode: 3 */

#define UCSSEL_0            (0x0000u)    /* USCI 0 Clock Source: 0 */
#define UCSSEL_1            (0x0040u)    /* USCI 0 Clock Source: 1 */
#define UCSSEL_2            (0x0080u)    /* USCI 0 Clock Source: 2 */
#define UCSSEL_3            (0x00C0u)    /* USCI 0 Clock Source: 3 */
#define UCSSEL__UCLK        (0x0000u)    /* USCI 0 Clock Source: UCLK */
#define UCSSEL__ACLK        (0x0040u)    /* USCI 0 Clock Source: ACLK */
#define UCSSEL__SMCLK       (0x0080u)    /* USCI 0 Clock Source: SMCLK */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1             (0x0002u)    /* USCI Deglitch Time Bit 1 */
#define UCGLIT0             (0x0001u)    /* USCI Deglitch Time Bit 0 */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1_L           (0x0002u)    /* USCI Deglitch Time Bit 1 */
#define UCGLIT0_L           (0x0001u)    /* USCI Deglitch Time Bit 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT            (0x0100u)    /* USCI Early UCTXIFG0 */
#define UCCLTO1             (0x0080u)    /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0             (0x0040u)    /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK           (0x0020u)    /* USCI Acknowledge Stop last byte */
#define UCSWACK             (0x0010u)    /* USCI Software controlled ACK */
#define UCASTP1             (0x0008u)    /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0             (0x0004u)    /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1             (0x0002u)    /* USCI Deglitch time Bit: 1 */
#define UCGLIT0             (0x0001u)    /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCCLTO1_L           (0x0080u)    /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0_L           (0x0040u)    /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK_L         (0x0020u)    /* USCI Acknowledge Stop last byte */
#define UCSWACK_L           (0x0010u)    /* USCI Software controlled ACK */
#define UCASTP1_L           (0x0008u)    /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0_L           (0x0004u)    /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1_L           (0x0002u)    /* USCI Deglitch time Bit: 1 */
#define UCGLIT0_L           (0x0001u)    /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT_H          (0x0001u)    /* USCI Early UCTXIFG0 */

#define UCGLIT_0            (0x0000u)    /* USCI Deglitch time: 0 */
#define UCGLIT_1            (0x0001u)    /* USCI Deglitch time: 1 */
#define UCGLIT_2            (0x0002u)    /* USCI Deglitch time: 2 */
#define UCGLIT_3            (0x0003u)    /* USCI Deglitch time: 3 */

#define UCASTP_0            (0x0000u)    /* USCI Automatic Stop condition generation: 0 */
#define UCASTP_1            (0x0004u)    /* USCI Automatic Stop condition generation: 1 */
#define UCASTP_2            (0x0008u)    /* USCI Automatic Stop condition generation: 2 */
#define UCASTP_3            (0x000Cu)    /* USCI Automatic Stop condition generation: 3 */

#define UCCLTO_0            (0x0000u)    /* USCI Clock low timeout: 0 */
#define UCCLTO_1            (0x0040u)    /* USCI Clock low timeout: 1 */
#define UCCLTO_2            (0x0080u)    /* USCI Clock low timeout: 2 */
#define UCCLTO_3            (0x00C0u)    /* USCI Clock low timeout: 3 */

/* UCAxMCTLW Control Bits */
#define UCBRS7              (0x8000u)    /* USCI Second Stage Modulation Select 7 */
#define UCBRS6              (0x4000u)    /* USCI Second Stage Modulation Select 6 */
#define UCBRS5              (0x2000u)    /* USCI Second Stage Modulation Select 5 */
#define UCBRS4              (0x1000u)    /* USCI Second Stage Modulation Select 4 */
#define UCBRS3              (0x0800u)    /* USCI Second Stage Modulation Select 3 */
#define UCBRS2              (0x0400u)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1              (0x0200u)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0              (0x0100u)    /* USCI Second Stage Modulation Select 0 */
#define UCBRF3              (0x0080u)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2              (0x0040u)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1              (0x0020u)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0              (0x0010u)    /* USCI First Stage Modulation Select 0 */
#define UCOS16              (0x0001u)    /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRF3_L            (0x0080u)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2_L            (0x0040u)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1_L            (0x0020u)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0_L            (0x0010u)    /* USCI First Stage Modulation Select 0 */
#define UCOS16_L            (0x0001u)    /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRS7_H            (0x0080u)    /* USCI Second Stage Modulation Select 7 */
#define UCBRS6_H            (0x0040u)    /* USCI Second Stage Modulation Select 6 */
#define UCBRS5_H            (0x0020u)    /* USCI Second Stage Modulation Select 5 */
#define UCBRS4_H            (0x0010u)    /* USCI Second Stage Modulation Select 4 */
#define UCBRS3_H            (0x0008u)    /* USCI Second Stage Modulation Select 3 */
#define UCBRS2_H            (0x0004u)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1_H            (0x0002u)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0_H            (0x0001u)    /* USCI Second Stage Modulation Select 0 */

#define UCBRF_0             (0x00)    /* USCI First Stage Modulation: 0 */
#define UCBRF_1             (0x10)    /* USCI First Stage Modulation: 1 */
#define UCBRF_2             (0x20)    /* USCI First Stage Modulation: 2 */
#define UCBRF_3             (0x30)    /* USCI First Stage Modulation: 3 */
#define UCBRF_4             (0x40)    /* USCI First Stage Modulation: 4 */
#define UCBRF_5             (0x50)    /* USCI First Stage Modulation: 5 */
#define UCBRF_6             (0x60)    /* USCI First Stage Modulation: 6 */
#define UCBRF_7             (0x70)    /* USCI First Stage Modulation: 7 */
#define UCBRF_8             (0x80)    /* USCI First Stage Modulation: 8 */
#define UCBRF_9             (0x90)    /* USCI First Stage Modulation: 9 */
#define UCBRF_10            (0xA0)    /* USCI First Stage Modulation: A */
#define UCBRF_11            (0xB0)    /* USCI First Stage Modulation: B */
#define UCBRF_12            (0xC0)    /* USCI First Stage Modulation: C */
#define UCBRF_13            (0xD0)    /* USCI First Stage Modulation: D */
#define UCBRF_14            (0xE0)    /* USCI First Stage Modulation: E */
#define UCBRF_15            (0xF0)    /* USCI First Stage Modulation: F */

/* UCAxSTATW Control Bits */
#define UCLISTEN            (0x0080u)  /* USCI Listen mode */
#define UCFE                (0x0040u)  /* USCI Frame Error Flag */
#define UCOE                (0x0020u)  /* USCI Overrun Error Flag */
#define UCPE                (0x0010u)  /* USCI Parity Error Flag */
#define UCBRK               (0x0008u)  /* USCI Break received */
#define UCRXERR             (0x0004u)  /* USCI RX Error Flag */
#define UCADDR              (0x0002u)  /* USCI Address received Flag */
#define UCBUSY              (0x0001u)  /* USCI Busy Flag */
#define UCIDLE              (0x0002u)  /* USCI Idle line detected Flag */

/* UCBxSTATW I2C Control Bits */
#define UCBCNT7             (0x8000u)  /* USCI Byte Counter Bit 7 */
#define UCBCNT6             (0x4000u)  /* USCI Byte Counter Bit 6 */
#define UCBCNT5             (0x2000u)  /* USCI Byte Counter Bit 5 */
#define UCBCNT4             (0x1000u)  /* USCI Byte Counter Bit 4 */
#define UCBCNT3             (0x0800u)  /* USCI Byte Counter Bit 3 */
#define UCBCNT2             (0x0400u)  /* USCI Byte Counter Bit 2 */
#define UCBCNT1             (0x0200u)  /* USCI Byte Counter Bit 1 */
#define UCBCNT0             (0x0100u)  /* USCI Byte Counter Bit 0 */
#define UCSCLLOW            (0x0040u)  /* SCL low */
#define UCGC                (0x0020u)  /* General Call address received Flag */
#define UCBBUSY             (0x0010u)  /* Bus Busy Flag */

/* UCBxTBCNT I2C Control Bits */
#define UCTBCNT7            (0x0080u)  /* USCI Byte Counter Bit 7 */
#define UCTBCNT6            (0x0040u)  /* USCI Byte Counter Bit 6 */
#define UCTBCNT5            (0x0020u)  /* USCI Byte Counter Bit 5 */
#define UCTBCNT4            (0x0010u)  /* USCI Byte Counter Bit 4 */
#define UCTBCNT3            (0x0008u)  /* USCI Byte Counter Bit 3 */
#define UCTBCNT2            (0x0004u)  /* USCI Byte Counter Bit 2 */
#define UCTBCNT1            (0x0002u)  /* USCI Byte Counter Bit 1 */
#define UCTBCNT0            (0x0001u)  /* USCI Byte Counter Bit 0 */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5           (0x8000u)  /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4           (0x4000u)  /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3           (0x2000u)  /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2           (0x1000u)  /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1           (0x0800u)  /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0           (0x0400u)  /* IRDA Receive Filter Length 0 */
#define UCIRRXPL            (0x0200u)  /* IRDA Receive Input Polarity */
#define UCIRRXFE            (0x0100u)  /* IRDA Receive Filter enable */
#define UCIRTXPL5           (0x0080u)  /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4           (0x0040u)  /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3           (0x0020u)  /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2           (0x0010u)  /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1           (0x0008u)  /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0           (0x0004u)  /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK           (0x0002u)  /* IRDA Transmit Pulse Clock Select */
#define UCIREN              (0x0001u)  /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRTXPL5_L         (0x0080u)  /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4_L         (0x0040u)  /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3_L         (0x0020u)  /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2_L         (0x0010u)  /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1_L         (0x0008u)  /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0_L         (0x0004u)  /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK_L         (0x0002u)  /* IRDA Transmit Pulse Clock Select */
#define UCIREN_L            (0x0001u)  /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5_H         (0x0080u)  /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4_H         (0x0040u)  /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3_H         (0x0020u)  /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2_H         (0x0010u)  /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1_H         (0x0008u)  /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0_H         (0x0004u)  /* IRDA Receive Filter Length 0 */
#define UCIRRXPL_H          (0x0002u)  /* IRDA Receive Input Polarity */
#define UCIRRXFE_H          (0x0001u)  /* IRDA Receive Filter enable */

/* UCAxABCTL Control Bits */
//#define res               (0x80)    /* reserved */
//#define res               (0x40)    /* reserved */
#define UCDELIM1            (0x20)    /* Break Sync Delimiter 1 */
#define UCDELIM0            (0x10)    /* Break Sync Delimiter 0 */
#define UCSTOE              (0x08)    /* Sync-Field Timeout error */
#define UCBTOE              (0x04)    /* Break Timeout error */
//#define res               (0x02)    /* reserved */
#define UCABDEN             (0x01)    /* Auto Baud Rate detect enable */

/* UCBxI2COA0 Control Bits */
#define UCGCEN              (0x8000u)  /* I2C General Call enable */
#define UCOAEN              (0x0400u)  /* I2C Own Address enable */
#define UCOA9               (0x0200u)  /* I2C Own Address Bit 9 */
#define UCOA8               (0x0100u)  /* I2C Own Address Bit 8 */
#define UCOA7               (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6               (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5               (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4               (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3               (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2               (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1               (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0               (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCOA7_L             (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6_L             (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5_L             (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4_L             (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3_L             (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2_L             (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1_L             (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0_L             (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCGCEN_H            (0x0080u)  /* I2C General Call enable */
#define UCOAEN_H            (0x0004u)  /* I2C Own Address enable */
#define UCOA9_H             (0x0002u)  /* I2C Own Address Bit 9 */
#define UCOA8_H             (0x0001u)  /* I2C Own Address Bit 8 */

/* UCBxI2COAx Control Bits */
#define UCOAEN              (0x0400u)  /* I2C Own Address enable */
#define UCOA9               (0x0200u)  /* I2C Own Address Bit 9 */
#define UCOA8               (0x0100u)  /* I2C Own Address Bit 8 */
#define UCOA7               (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6               (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5               (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4               (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3               (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2               (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1               (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0               (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOA7_L             (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6_L             (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5_L             (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4_L             (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3_L             (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2_L             (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1_L             (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0_L             (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOAEN_H            (0x0004u)  /* I2C Own Address enable */
#define UCOA9_H             (0x0002u)  /* I2C Own Address Bit 9 */
#define UCOA8_H             (0x0001u)  /* I2C Own Address Bit 8 */

/* UCBxADDRX Control Bits */
#define UCADDRX9            (0x0200u)  /* I2C Receive Address Bit 9 */
#define UCADDRX8            (0x0100u)  /* I2C Receive Address Bit 8 */
#define UCADDRX7            (0x0080u)  /* I2C Receive Address Bit 7 */
#define UCADDRX6            (0x0040u)  /* I2C Receive Address Bit 6 */
#define UCADDRX5            (0x0020u)  /* I2C Receive Address Bit 5 */
#define UCADDRX4            (0x0010u)  /* I2C Receive Address Bit 4 */
#define UCADDRX3            (0x0008u)  /* I2C Receive Address Bit 3 */
#define UCADDRX2            (0x0004u)  /* I2C Receive Address Bit 2 */
#define UCADDRX1            (0x0002u)  /* I2C Receive Address Bit 1 */
#define UCADDRX0            (0x0001u)  /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX7_L          (0x0080u)  /* I2C Receive Address Bit 7 */
#define UCADDRX6_L          (0x0040u)  /* I2C Receive Address Bit 6 */
#define UCADDRX5_L          (0x0020u)  /* I2C Receive Address Bit 5 */
#define UCADDRX4_L          (0x0010u)  /* I2C Receive Address Bit 4 */
#define UCADDRX3_L          (0x0008u)  /* I2C Receive Address Bit 3 */
#define UCADDRX2_L          (0x0004u)  /* I2C Receive Address Bit 2 */
#define UCADDRX1_L          (0x0002u)  /* I2C Receive Address Bit 1 */
#define UCADDRX0_L          (0x0001u)  /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX9_H          (0x0002u)  /* I2C Receive Address Bit 9 */
#define UCADDRX8_H          (0x0001u)  /* I2C Receive Address Bit 8 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9            (0x0200u)  /* I2C Address Mask Bit 9 */
#define UCADDMASK8            (0x0100u)  /* I2C Address Mask Bit 8 */
#define UCADDMASK7            (0x0080u)  /* I2C Address Mask Bit 7 */
#define UCADDMASK6            (0x0040u)  /* I2C Address Mask Bit 6 */
#define UCADDMASK5            (0x0020u)  /* I2C Address Mask Bit 5 */
#define UCADDMASK4            (0x0010u)  /* I2C Address Mask Bit 4 */
#define UCADDMASK3            (0x0008u)  /* I2C Address Mask Bit 3 */
#define UCADDMASK2            (0x0004u)  /* I2C Address Mask Bit 2 */
#define UCADDMASK1            (0x0002u)  /* I2C Address Mask Bit 1 */
#define UCADDMASK0            (0x0001u)  /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK7_L        (0x0080u)  /* I2C Address Mask Bit 7 */
#define UCADDMASK6_L        (0x0040u)  /* I2C Address Mask Bit 6 */
#define UCADDMASK5_L        (0x0020u)  /* I2C Address Mask Bit 5 */
#define UCADDMASK4_L        (0x0010u)  /* I2C Address Mask Bit 4 */
#define UCADDMASK3_L        (0x0008u)  /* I2C Address Mask Bit 3 */
#define UCADDMASK2_L        (0x0004u)  /* I2C Address Mask Bit 2 */
#define UCADDMASK1_L        (0x0002u)  /* I2C Address Mask Bit 1 */
#define UCADDMASK0_L        (0x0001u)  /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9_H        (0x0002u)  /* I2C Address Mask Bit 9 */
#define UCADDMASK8_H        (0x0001u)  /* I2C Address Mask Bit 8 */

/* UCBxI2CSA Control Bits */
#define UCSA9               (0x0200u)  /* I2C Slave Address Bit 9 */
#define UCSA8               (0x0100u)  /* I2C Slave Address Bit 8 */
#define UCSA7               (0x0080u)  /* I2C Slave Address Bit 7 */
#define UCSA6               (0x0040u)  /* I2C Slave Address Bit 6 */
#define UCSA5               (0x0020u)  /* I2C Slave Address Bit 5 */
#define UCSA4               (0x0010u)  /* I2C Slave Address Bit 4 */
#define UCSA3               (0x0008u)  /* I2C Slave Address Bit 3 */
#define UCSA2               (0x0004u)  /* I2C Slave Address Bit 2 */
#define UCSA1               (0x0002u)  /* I2C Slave Address Bit 1 */
#define UCSA0               (0x0001u)  /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA7_L             (0x0080u)  /* I2C Slave Address Bit 7 */
#define UCSA6_L             (0x0040u)  /* I2C Slave Address Bit 6 */
#define UCSA5_L             (0x0020u)  /* I2C Slave Address Bit 5 */
#define UCSA4_L             (0x0010u)  /* I2C Slave Address Bit 4 */
#define UCSA3_L             (0x0008u)  /* I2C Slave Address Bit 3 */
#define UCSA2_L             (0x0004u)  /* I2C Slave Address Bit 2 */
#define UCSA1_L             (0x0002u)  /* I2C Slave Address Bit 1 */
#define UCSA0_L             (0x0001u)  /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA9_H             (0x0002u)  /* I2C Slave Address Bit 9 */
#define UCSA8_H             (0x0001u)  /* I2C Slave Address Bit 8 */

/* UCAxIE UART Control Bits */
#define UCTXCPTIE           (0x0008u)  /* UART Transmit Complete Interrupt Enable */
#define UCSTTIE             (0x0004u)  /* UART Start Bit Interrupt Enalble */
#define UCTXIE              (0x0002u)  /* UART Transmit Interrupt Enable */
#define UCRXIE              (0x0001u)  /* UART Receive Interrupt Enable */

/* UCAxIE/UCBxIE SPI Control Bits */

/* UCBxIE I2C Control Bits */
#define UCBIT9IE            (0x4000u)  /* I2C Bit 9 Position Interrupt Enable 3 */
#define UCTXIE3             (0x2000u)  /* I2C Transmit Interrupt Enable 3 */
#define UCRXIE3             (0x1000u)  /* I2C Receive Interrupt Enable 3 */
#define UCTXIE2             (0x0800u)  /* I2C Transmit Interrupt Enable 2 */
#define UCRXIE2             (0x0400u)  /* I2C Receive Interrupt Enable 2 */
#define UCTXIE1             (0x0200u)  /* I2C Transmit Interrupt Enable 1 */
#define UCRXIE1             (0x0100u)  /* I2C Receive Interrupt Enable 1 */
#define UCCLTOIE            (0x0080u)  /* I2C Clock Low Timeout interrupt enable */
#define UCBCNTIE            (0x0040u)  /* I2C Automatic stop assertion interrupt enable */
#define UCNACKIE            (0x0020u)  /* I2C NACK Condition interrupt enable */
#define UCALIE              (0x0010u)  /* I2C Arbitration Lost interrupt enable */
#define UCSTPIE             (0x0008u)  /* I2C STOP Condition interrupt enable */
#define UCSTTIE             (0x0004u)  /* I2C START Condition interrupt enable */
#define UCTXIE0             (0x0002u)  /* I2C Transmit Interrupt Enable 0 */
#define UCRXIE0             (0x0001u)  /* I2C Receive Interrupt Enable 0 */

/* UCAxIFG UART Control Bits */
#define UCTXCPTIFG          (0x0008u)  /* UART Transmit Complete Interrupt Flag */
#define UCSTTIFG            (0x0004u)  /* UART Start Bit Interrupt Flag */
#define UCTXIFG             (0x0002u)  /* UART Transmit Interrupt Flag */
#define UCRXIFG             (0x0001u)  /* UART Receive Interrupt Flag */

/* UCAxIFG/UCBxIFG SPI Control Bits */
#define UCTXIFG             (0x0002u)  /* SPI Transmit Interrupt Flag */
#define UCRXIFG             (0x0001u)  /* SPI Receive Interrupt Flag */

/* UCBxIFG Control Bits */
#define UCBIT9IFG           (0x4000u)  /* I2C Bit 9 Possition Interrupt Flag 3 */
#define UCTXIFG3            (0x2000u)  /* I2C Transmit Interrupt Flag 3 */
#define UCRXIFG3            (0x1000u)  /* I2C Receive Interrupt Flag 3 */
#define UCTXIFG2            (0x0800u)  /* I2C Transmit Interrupt Flag 2 */
#define UCRXIFG2            (0x0400u)  /* I2C Receive Interrupt Flag 2 */
#define UCTXIFG1            (0x0200u)  /* I2C Transmit Interrupt Flag 1 */
#define UCRXIFG1            (0x0100u)  /* I2C Receive Interrupt Flag 1 */
#define UCCLTOIFG           (0x0080u)  /* I2C Clock low Timeout interrupt Flag */
#define UCBCNTIFG           (0x0040u)  /* I2C Byte counter interrupt flag */
#define UCNACKIFG           (0x0020u)  /* I2C NACK Condition interrupt Flag */
#define UCALIFG             (0x0010u)  /* I2C Arbitration Lost interrupt Flag */
#define UCSTPIFG            (0x0008u)  /* I2C STOP Condition interrupt Flag */
#define UCSTTIFG            (0x0004u)  /* I2C START Condition interrupt Flag */
#define UCTXIFG0            (0x0002u)  /* I2C Transmit Interrupt Flag 0 */
#define UCRXIFG0            (0x0001u)  /* I2C Receive Interrupt Flag 0 */

/* USCI UART Definitions */
#define USCI_NONE            (0x0000u)   /* No Interrupt pending */
#define USCI_UART_UCRXIFG    (0x0002u)   /* USCI UCRXIFG */
#define USCI_UART_UCTXIFG    (0x0004u)   /* USCI UCTXIFG */
#define USCI_UART_UCSTTIFG   (0x0006u)   /* USCI UCSTTIFG */
#define USCI_UART_UCTXCPTIFG (0x0008u)   /* USCI UCTXCPTIFG */

/* USCI SPI Definitions */
#define USCI_SPI_UCRXIFG    (0x0002u)    /* USCI UCRXIFG */
#define USCI_SPI_UCTXIFG    (0x0004u)    /* USCI UCTXIFG */

/* USCI I2C Definitions */
#define USCI_I2C_UCALIFG    (0x0002u)    /* USCI I2C Mode: UCALIFG */
#define USCI_I2C_UCNACKIFG  (0x0004u)    /* USCI I2C Mode: UCNACKIFG */
#define USCI_I2C_UCSTTIFG   (0x0006u)    /* USCI I2C Mode: UCSTTIFG*/
#define USCI_I2C_UCSTPIFG   (0x0008u)    /* USCI I2C Mode: UCSTPIFG*/
#define USCI_I2C_UCRXIFG3   (0x000Au)    /* USCI I2C Mode: UCRXIFG3 */
#define USCI_I2C_UCTXIFG3   (0x000Cu)    /* USCI I2C Mode: UCTXIFG3 */
#define USCI_I2C_UCRXIFG2   (0x000Eu)    /* USCI I2C Mode: UCRXIFG2 */
#define USCI_I2C_UCTXIFG2   (0x0010u)    /* USCI I2C Mode: UCTXIFG2 */
#define USCI_I2C_UCRXIFG1   (0x0012u)    /* USCI I2C Mode: UCRXIFG1 */
#define USCI_I2C_UCTXIFG1   (0x0014u)    /* USCI I2C Mode: UCTXIFG1 */
#define USCI_I2C_UCRXIFG0   (0x0016u)    /* USCI I2C Mode: UCRXIFG0 */
#define USCI_I2C_UCTXIFG0   (0x0018u)    /* USCI I2C Mode: UCTXIFG0 */
#define USCI_I2C_UCBCNTIFG  (0x001Au)    /* USCI I2C Mode: UCBCNTIFG */
#define USCI_I2C_UCCLTOIFG  (0x001Cu)    /* USCI I2C Mode: UCCLTOIFG */
#define USCI_I2C_UCBIT9IFG  (0x001Eu)    /* USCI I2C Mode: UCBIT9IFG */

/************************************************************
* WATCHDOG TIMER A
************************************************************/
#define __MSP430_HAS_WDT_A__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_WDT_A__ 0x0150
#define WDT_A_BASE __MSP430_BASEADDRESS_WDT_A__

#define  WDTCTL_                (0x015Cu)  /* Watchdog Timer Control */
DEFCW(   WDTCTL               , WDTCTL_)
/* The bit names have been prefixed with "WDT" */
/* WDTCTL Control Bits */
#define WDTIS0              (0x0001u)  /* WDT - Timer Interval Select 0 */
#define WDTIS1              (0x0002u)  /* WDT - Timer Interval Select 1 */
#define WDTIS2              (0x0004u)  /* WDT - Timer Interval Select 2 */
#define WDTCNTCL            (0x0008u)  /* WDT - Timer Clear */
#define WDTTMSEL            (0x0010u)  /* WDT - Timer Mode Select */
#define WDTSSEL0            (0x0020u)  /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1            (0x0040u)  /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD             (0x0080u)  /* WDT - Timer hold */

/* WDTCTL Control Bits */
#define WDTIS0_L            (0x0001u)  /* WDT - Timer Interval Select 0 */
#define WDTIS1_L            (0x0002u)  /* WDT - Timer Interval Select 1 */
#define WDTIS2_L            (0x0004u)  /* WDT - Timer Interval Select 2 */
#define WDTCNTCL_L          (0x0008u)  /* WDT - Timer Clear */
#define WDTTMSEL_L          (0x0010u)  /* WDT - Timer Mode Select */
#define WDTSSEL0_L          (0x0020u)  /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1_L          (0x0040u)  /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD_L           (0x0080u)  /* WDT - Timer hold */

#define WDTPW               (0x5A00u)

#define WDTIS_0           (0*0x0001u)  /* WDT - Timer Interval Select: /2G */
#define WDTIS_1           (1*0x0001u)  /* WDT - Timer Interval Select: /128M */
#define WDTIS_2           (2*0x0001u)  /* WDT - Timer Interval Select: /8192k */
#define WDTIS_3           (3*0x0001u)  /* WDT - Timer Interval Select: /512k */
#define WDTIS_4           (4*0x0001u)  /* WDT - Timer Interval Select: /32k */
#define WDTIS_5           (5*0x0001u)  /* WDT - Timer Interval Select: /8192 */
#define WDTIS_6           (6*0x0001u)  /* WDT - Timer Interval Select: /512 */
#define WDTIS_7           (7*0x0001u)  /* WDT - Timer Interval Select: /64 */
#define WDTIS__2G         (0*0x0001u)  /* WDT - Timer Interval Select: /2G */
#define WDTIS__128M       (1*0x0001u)  /* WDT - Timer Interval Select: /128M */
#define WDTIS__8192K      (2*0x0001u)  /* WDT - Timer Interval Select: /8192k */
#define WDTIS__512K       (3*0x0001u)  /* WDT - Timer Interval Select: /512k */
#define WDTIS__32K        (4*0x0001u)  /* WDT - Timer Interval Select: /32k */
#define WDTIS__8192       (5*0x0001u)  /* WDT - Timer Interval Select: /8192 */
#define WDTIS__512        (6*0x0001u)  /* WDT - Timer Interval Select: /512 */
#define WDTIS__64         (7*0x0001u)  /* WDT - Timer Interval Select: /64 */

#define WDTSSEL_0         (0*0x0020u)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL_1         (1*0x0020u)  /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL_2         (2*0x0020u)  /* WDT - Timer Clock Source Select: VLO_CLK */
#define WDTSSEL_3         (3*0x0020u)  /* WDT - Timer Clock Source Select: reserved */
#define WDTSSEL__SMCLK    (0*0x0020u)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL__ACLK     (1*0x0020u)  /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL__VLO      (2*0x0020u)  /* WDT - Timer Clock Source Select: VLO_CLK */

/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2)                         /* 32ms interval (default) */
#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS0)                  /* 8ms     " */
#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS1)                  /* 0.5ms   " */
#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS1+WDTIS0)           /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0)                /* 1000ms  " */
#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS0)         /* 250ms   " */
#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS1)         /* 16ms    " */
#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS1+WDTIS0)  /* 1.9ms   " */
/* Watchdog mode -> reset after expired time */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MRST_32         (WDTPW+WDTCNTCL+WDTIS2)                                  /* 32ms interval (default) */
#define WDT_MRST_8          (WDTPW+WDTCNTCL+WDTIS2+WDTIS0)                           /* 8ms     " */
#define WDT_MRST_0_5        (WDTPW+WDTCNTCL+WDTIS2+WDTIS1)                           /* 0.5ms   " */
#define WDT_MRST_0_064      (WDTPW+WDTCNTCL+WDTIS2+WDTIS1+WDTIS0)                    /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ARST_1000       (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2)                         /* 1000ms  " */
#define WDT_ARST_250        (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS0)                  /* 250ms   " */
#define WDT_ARST_16         (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS1)                  /* 16ms    " */
#define WDT_ARST_1_9        (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS1+WDTIS0)           /* 1.9ms   " */


/************************************************************
* TLV Descriptors
************************************************************/
#define __MSP430_HAS_TLV__              /* Definition to show that Module is available */
#define TLV_BASE __MSP430_BASEADDRESS_TLV__

#define TLV_START             (0x1A08u)    /* Start Address of the TLV structure */
#define TLV_END               (0x1AFFu)    /* End Address of the TLV structure */

#define TLV_LDTAG             (0x01)      /*  Legacy descriptor (1xx, 2xx, 4xx families) */
#define TLV_PDTAG             (0x02)      /*  Peripheral discovery descriptor */
#define TLV_Reserved3         (0x03)      /*  Future usage */
#define TLV_Reserved4         (0x04)      /*  Future usage */
#define TLV_BLANK             (0x05)      /*  Blank descriptor */
#define TLV_Reserved6         (0x06)      /*  Future usage */
#define TLV_Reserved7         (0x07)      /*  Serial Number */
#define TLV_DIERECORD         (0x08)      /*  Die Record  */
#define TLV_ADCCAL            (0x11)      /*  ADC12 calibration */
#define TLV_ADC12CAL          (0x11)      /*  ADC12 calibration */
#define TLV_ADC10CAL          (0x13)      /*  ADC10 calibration */
#define TLV_REFCAL            (0x12)      /*  REF calibration */
#define TLV_TAGEXT            (0xFE)      /*  Tag extender */
#define TLV_TAGEND            (0xFF)      //  Tag End of Table

/************************************************************
* Interrupt Vectors (offset from 0xFF80 + 0x10 for Password)
************************************************************/


#define AES256_VECTOR       (27 * 2u) /* 0xFFC6 AES256 */
#define RTC_VECTOR          (28 * 2u) /* 0xFFC8 RTC */
#define PORT4_VECTOR        (30 * 2u) /* 0xFFCC Port 4 */
#define PORT3_VECTOR        (31 * 2u) /* 0xFFCE Port 3 */
#define TIMER3_A1_VECTOR    (32 * 2u) /* 0xFFD0 Timer3_A2 CC1, TA */
#define TIMER3_A0_VECTOR    (33 * 2u) /* 0xFFD2 Timer3_A2 CC0 */
#define PORT2_VECTOR        (34 * 2u) /* 0xFFD4 Port 2 */
#define TIMER2_A1_VECTOR    (35 * 2u) /* 0xFFD6 Timer2_A3 CC1, TA */
#define TIMER2_A0_VECTOR    (36 * 2u) /* 0xFFD8 Timer2_A3 CC0 */
#define PORT1_VECTOR        (37 * 2u) /* 0xFFDA Port 1 */
#define TIMER1_A1_VECTOR    (38 * 2u) /* 0xFFDC Timer1_A3 CC1-2, TA1 */
#define TIMER1_A0_VECTOR    (39 * 2u) /* 0xFFDE Timer1_A3 CC0 */
#define DMA_VECTOR          (40 * 2u) /* 0xFFE0 DMA */
#define USCI_B1_VECTOR      (41 * 2u) /* 0xFFE2 USCI B1 Receive/Transmit */
#define USCI_A1_VECTOR      (42 * 2u) /* 0xFFE4 USCI A1 Receive/Transmit */
#define TIMER0_A1_VECTOR    (43 * 2u) /* 0xFFE6 Timer0_A5 CC1-4, TA */
#define TIMER0_A0_VECTOR    (44 * 2u) /* 0xFFE8 Timer0_A5 CC0 */
#define ADC12_VECTOR        (45 * 2u) /* 0xFFEA ADC */
#define USCI_B0_VECTOR      (46 * 2u) /* 0xFFEC USCI B0 Receive/Transmit */
#define USCI_A0_VECTOR      (47 * 2u) /* 0xFFEE USCI A0 Receive/Transmit */
#define ESCAN_IF_VECTOR     (48 * 2u) /* 0xFFF0 Extended Scan IF */
#define WDT_VECTOR          (49 * 2u) /* 0xFFF2 Watchdog Timer */
#define TIMER0_B1_VECTOR    (50 * 2u) /* 0xFFF4 Timer0_B3 CC1-2, TB */
#define TIMER0_B0_VECTOR    (51 * 2u) /* 0xFFF6 Timer0_B3 CC0 */
#define COMP_E_VECTOR       (52 * 2u) /* 0xFFF8 Comparator E */
#define UNMI_VECTOR         (53 * 2u) /* 0xFFFA User Non-maskable */
#define SYSNMI_VECTOR       (54 * 2u) /* 0xFFFC System Non-maskable */
#define RESET_VECTOR        (55 * 2u) /* 0xFFFE Reset [Highest Priority] */

/************************************************************
* End of Modules
************************************************************/
#pragma language=restore

#endif /* #ifndef __MSP430FR5989 */

