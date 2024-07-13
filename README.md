# 自制小型TCP/IP协议栈

## 开发环境

`编辑器`: VScode

`操作系统`: Windows 11

`构建工具`: CMake 

`版本控制工具`: Git

`文档编写`： Markdown

## 编译
编译之前请手动更改编译器的位置，CMake的位置，以及第三方库的位置。并且手动创建一个build文件夹，因为需要进入build文件夹下进行编译。

在移植使用 `pcap_device.c` 第三方文件时，需要加载pcap的lib，需要根据库的位置来更改一下加载地址。