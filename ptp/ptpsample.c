#include <rtthread.h>
#include <rtdef.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <netdev_ipaddr.h>
#include <netdev.h>
typedef struct
{
        int32_t seconds;
        int32_t nanoseconds;
} TimeInternal;

void ptpd_init();
void ptpd_task();
void updateTimer(void);
void getTime(TimeInternal *time);
void setTime(const TimeInternal *time);
#define DBG_COLOR
#define DBG_TAG "ptp"
#define DBG_LVL DBG_LOG


#include <rtdbg.h>

rt_thread_t ptp_thread;
/* 线程1入口函数 */
static void ptp_entry(void *p)
{
         ptpd_init();
    while (1)
    {
           ptpd_task();
           updateTimer();
           rt_thread_delay(1);
    }

}

static int ptp_int(void)
{
    ptp_thread = rt_thread_create("ptp_thread", ptp_entry, RT_NULL, 2048, 14, 5); //闪灯线程
    if (ptp_thread != RT_NULL)
        rt_thread_startup(ptp_thread);
    else
        rt_kprintf("ptp_thread");
    return 0;


}
INIT_DEVICE_EXPORT(ptp_int);





rt_thread_t ptp_time_print_thread;
/* 线程1入口函数 */
static void ptp_print_entry(void *p)
{
       TimeInternal  currentTime;

    while (1)
    {
         getTime(&currentTime);//mac层读取的时间
         rt_kprintf("s:%d,%d\n",currentTime.seconds,currentTime.nanoseconds);
           rt_thread_delay(1);
    }

}

static int ptp_time_print(void)
{
    ptp_time_print_thread = rt_thread_create("ptp_time_print_thread", ptp_print_entry, RT_NULL, 2048, 14, 5); //闪灯线程
    if (ptp_time_print_thread != RT_NULL)
        rt_thread_startup(ptp_time_print_thread);
    else
        rt_kprintf("ptp_time_print_thread");
    return 0;


}
MSH_CMD_EXPORT(ptp_time_print, ptp time print);


static void setTime_test(void)
{
    TimeInternal  rtc_set_time;
    rtc_set_time.seconds=100;
    rtc_set_time.nanoseconds=0;
     setTime(&rtc_set_time);
}
MSH_CMD_EXPORT(setTime_test, setTime);

