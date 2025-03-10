*已经实现基本精准同步，主从时间差在30微秒*

修改主从方式从ptp的初始化函数修改
*主时钟时钟源设为240mhz，若需要修改，则对应修改下面两个宏定义
*#define ADJ_FREQ_BASE_ADDEND      0x35455A81
*#define ADJ_FREQ_BASE_INCREMENT   43


h7与f4寄存器更换

#include "stm32h7xx.h"       #include "stm32f4xx.h"
timestamp->tv_sec = ETH->MACSTSR;
timestamp->tv_sec = ETH->PTPTSHR;

timestamp->tv_nsec = ETH_PTPSubSecond2NanoSecond(ETH->MACSTNR);
timestamp->tv_nsec = ETH_PTPSubSecond2NanoSecond((ETH->PTPTSLR));

addend = ETH->MACTSAR;
ddend  = ETH->PTPTSAR;

ETH->MACTSCR |= ETH_MACTSCR_TSUPDT;
ETH->PTPTSCR |= ETH_PTPTSCR_TSSTU;

ETH->PTPTSCR &= (~(uint32_t)ETH_MACTSCR_TSCFUPDT);
ETH->PTPTSCR &= (~(uint32_t)ETH_PTPTSCR_TSFCU);

ETH->MACSTSUR = SecondValue;
ETH->PTPTSHUR = SecondValue;

ETH->MACSTNUR = Sign | SubSecondValue;
ETH->PTPTSLUR = Sign | SubSecondValue; 

ETH->MACSSIR = (SubSecondValue<<16);
ETH->PTPSSIR = SubSecondValue;

ETH->PTPTSCR |= ETH_MACTSCR_TSENA;
ETH->PTPTSCR |= ETH_MACTSCR_TSENA | ETH_MACTSCR_TSIPV4ENA | ETH_MACTSCR_TSIPV6ENA | ETH_MACTSCR_TSENALL;

ETH->PTPTSCR |= ETH_PTPTSCR_TSE;
ETH->PTPTSCR |= ETH_PTPTSCR_TSE | ETH_PTPTSSR_TSSIPV4FE | ETH_PTPTSSR_TSSIPV6FE | ETH_PTPTSSR_TSSARFE;

ETH->PTPTSCR &= (~(uint32_t)ETH_MACTSCR_TSENA);
ETH->PTPTSCR &= (~(uint32_t)ETH_PTPTSCR_TSE);













