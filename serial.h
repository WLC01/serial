/*************************************************************************
  * File: remote serial.h
  * Date: 2022/9/9 16:29
  * Email: 352386674@qq.com
  * Author: Lei Wu 
  * Licence: GPL V2      
  * Description: 串口操作
  ************************************************************************/

#ifndef REMOTE_SERIAL_H
#define REMOTE_SERIAL_H

#include <stdio.h>
#include <termios.h>

typedef enum STATUS { SUCCESS, ERROR } status;

typedef struct _driver {
    int  fd; // 打开串口后返回的文件指针
    int  serial_model; // 打开串口的模式
    struct termios *terminal; // termios 结构
    char serial_port[0]; // 串口设备号
} driver;

struct _serial_data {
    char *data;
    status code;
    int data_len;
};

/**
 * 向终端打印一条信息
 *
 * @param const char *s 说明
 * @param const char *describe 详细描述
 *
 * @return status
 */
status return_err_string(const char *s, const char *describe);

/**
 * 打开串口设备
 *
 * @param const char *serial_port 串口设备号
 * @param unsigned int baud_rate 串口波特率
 *
 * @return status
 */
status serial_open(const char *serial_port, unsigned int baud_rate);

/**
 * 向串口写入数据
 *
 * @param const char *data 需要写入的串口数据
 *
 * @return status
 */
status serial_write(const char *data);

/**
 * 读取串口数据
 *
 * @param struct _serial_data *serial_data 串口数据将通过此结构体返回
 *
 * @return void
 */
void serial_read(struct _serial_data *serial_data);

/**
 * 关闭串口设备
 *
 * @return status
 */
status serial_close();

#endif //REMOTE_SERIAL_H
