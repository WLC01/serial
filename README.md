# serial

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
