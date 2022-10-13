/*************************************************************************
  * File: remote serial.c
  * Date: 2022/9/10 13:52
  * Email: 352386674@qq.com
  * Author: Lei Wu 
  * Licence: GPL V2      
  * Description: 串口操作(请不要在多进程下使用)
  ************************************************************************/

#include "serial.h"
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static driver *serial = NULL;

/**
 * @inherit
 */
extern status serial_close();

/**
 * @inherit
 */
status serial_write(const char *data);

/**
 * @inherit
 */
void serial_read(struct _serial_data *serial_data);

/**
 * @inherit
 */
extern status return_err_string(const char *s, const char *describe);

/**
 * @inherit
 */
extern status serial_open(const char *serial_port, unsigned int baud_rate);

int main(int argc, char *argv[])
{
    serial_open("/dev/pts/2", 115200); // 打开串口

    struct _serial_data serial_data = {0};

    while(1)
    {
        serial_data.data_len = 1024; // 分配1024byte
        serial_data.data = (char *) malloc(1024);
        memset(serial_data.data, 0, serial_data.data_len);

        serial_read(&serial_data); // 读取串口数据

        if(serial_data.code == 0)
        {
            printf("%s", serial_data.data);
        }
        else
        {
            printf("err: %s", serial_data.data);
        }

        free(serial_data.data);

        usleep(200);
    }

    // serial_write("hello"); // 向串口发送数据

    // serial_close(); // 关闭串口

    return 0;
}

status serial_open(const char *serial_port, unsigned int baud_rate)
{
    int set_baud_status, serial_port_len, serial_model, fd, rv;

    serial != NULL && return_err_string("初始化串口失败", "重复打开串口设备");

    serial_port_len = (sizeof(char) * (strlen(serial_port) + 1));
    serial = (driver *) malloc( sizeof(driver) + serial_port_len);

    memset(serial->serial_port, '0', serial_port_len); // 初始化串口地址内存区域

    serial_model = O_RDWR | O_NOCTTY;

    fd = open(serial_port,  serial_model);
    fd < 0 && return_err_string("打开串口失败", strerror(errno));

    serial->terminal = (struct termios *) malloc(sizeof(struct termios));
    serial->terminal == NULL && return_err_string("serial->terminal分配内存失败", strerror(errno));

    memset(serial->terminal, '0', sizeof(struct termios));

    rv = tcgetattr(serial->fd, serial->terminal); // 将fd和串口数据结构绑定
    rv != 0 && return_err_string( "获取串口结构体失败", strerror(errno));

    set_baud_status = cfsetspeed(serial->terminal, baud_rate);
    set_baud_status != 0 && return_err_string("设置波特率失败", strerror(errno));

    serial->fd = fd;
    serial->serial_model = serial_model;
    memcpy(serial->serial_port, serial_port, serial_port_len);

    return SUCCESS;
}

status serial_write(const char *data)
{
    serial == NULL && return_err_string("向串口发送数据失败", "串口设备未打开");

    int status = write(serial->fd, data, strlen(data));

    status == -1 && return_err_string("向串口发送数据失败", strerror(errno));
}

status serial_close()
{
    serial == NULL && return_err_string("关闭串口设备失败", "串口设备未打开");

    int status = close(serial->fd);

    status < 0 && return_err_string("关闭串口设备失败", strerror(errno));

    free(serial->terminal);
    free(serial);
    serial = NULL;

    return SUCCESS;
}

void serial_read(struct _serial_data *serial_data)
{
    serial_data->code = ERROR;

    if(serial == NULL)
    {
        strcpy(serial_data->data, "读取串口设备失败: 串口设备未打开");

        exit(0);
    }

    int status = read(serial->fd, (void *)serial_data->data, serial_data->data_len);

    if(status == -1)
    {
        snprintf(serial_data->data, serial_data->data_len, "读取串口设备失败: %s", strerror(errno));

        exit(0);
    }

    serial_data->code = SUCCESS;
    serial_data->data_len = status;
}

status return_err_string(const char *s, const char *describe)
{
    printf("%s: %s\r\n", s, describe);

    exit(ERROR);
}
