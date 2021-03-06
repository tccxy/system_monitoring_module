/**
 * @file smm.h
 * @author zhao.wei (hw)
 * @brief smm头文件
 * @version 0.1
 * @date 2020-07-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _SMM_H_
#define _SMM_H_

#define smm_deal(cat, dealmode, format, args...)                                        \
    zlog(cat, __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, \
         dealmode, format, ##args)

#define DISPLAY_MODE 0 //使用打印模式
#define LOG_MODE 1     //使用zlog的info模式
#define ACTIVE 1

#define MAX_PID_NUM 128
#define PID_NAME_MAX_LEN 32   //进程名字最大长度
#define DEFAULT_INTERVAL 1000 //默认的监控时间间隔
#define MIN_INTERVAL 1000     //最小的监控时间间隔
#define SAMPLE_MS 300         //两次采样的时间间隔
/**
 * @brief smm进程名字
 * 
 */
struct smm_pid_name
{
    u8 name[PID_NAME_MAX_LEN];
};

/**
 * @brief smm pid信息
 * 
 */
struct smm_pid_msg
{
    struct smm_pid_name parse_pid_name[MAX_PID_NUM];
    u32 parse_pid_num;
    u8 flag;
};

/**
 * @brief 系统级监控数据
 * 
 */
struct smm_result
{
    u32 weight_times; //加权次数
    double r_cpu_ratio;
    double r_cpu_usr_ratio;
    double r_cpu_kernel_ratio;
    double r_cpu_si_ratio;
    double r_mem_ratio;
    double r_mem_cache;
    double r_mem_buffer;
};

/**
 * @brief 进程级监控数据
 * 
 */
struct smm_pid_result
{
    double r_pid_cpu_ratio;
    double r_pid_cpu_usr_ratio;
    double r_pid_cpu_sys_ratio;
    double r_pid_mem_ratio;
};
/**
 * @brief smm的控制数据结构
 * 
 */
struct smm_contrl
{
    u32 interval; //监控的时间间隔 单位：mm
    u8 dealmode;  // 实时显示 / 记录日志
    u8 pad1;
    u16 pad2; //保留
    struct smm_result result;

    u32 smm_pid[MAX_PID_NUM];      //进程PID
    u8 smm_pid_valid[MAX_PID_NUM]; //监控的进程是否有效
    struct smm_pid_name pid_name[MAX_PID_NUM];
    struct smm_pid_result pid_result[MAX_PID_NUM];
};

typedef void (*dealfun)(unsigned int, struct smm_contrl *, int, void *); //钩子

typedef enum e_dealtype
{
    CPU_RATIO = 0,
    CPU_USR_RATIO,
    CPU_KERNEL_RATIO,
    CPU_SI_RATIO,
    MEM_RATIO,
    MEM_CAHE,
    MEM_BUFFER,
    SMM_M_END,

    PID_CPU_RATIO,
    PID_CPU_USR_RATIO,
    PID_CPU_KERNEL_RATIO,
    PID_MEM_RATIO,
    SMM_PID_M_END
} dealtype;

struct smm_dealentity
{
    int deal_type;
    dealfun p_dealfun;
};

#endif