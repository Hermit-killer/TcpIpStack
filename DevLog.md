# 开发日志记录

用于记录开发过程中的一些坑

## 开发环境搭建

### VScode

#### c_cpp_properties.json

其实这个c_cpp_properties.json的作用和cmake等构建类似，我这里用来指定了头文件的目录。

``` json
"includePath": [
                "${workspaceFolder}/**",
                "E:/WpdPack/Include/**"
            ]
```
由于要使用第三方库pcap的头文件，所以需要将其目录的路径添加到这里，否则用VScode读写代码时，会提示报错，说找不到文件、函数以及变量。。。

#### settings.json
其实settings.json是用于对工作区的编辑器行为进行配置的，因为针对不同项目，我们对VScode的要求也不一样，所以不能总是反复去修改用户设置，因为用户设置是全局的。

#### tasks.json
用于配置编译相关的任务

### CMake

`project(NAME)` 不要放在中间，要放在最前面。

### 插件使用
`Vim`, `vscode-icons`

### Git
远程仓库链接不要用http的那个，因为配置的是ssh的密钥，所以就用ssh那个地址。
然后写一个gitignore，免得每次都要吧build下面的东西提交出去。

## C语言

### 内存对齐

使用 `programa pack(n)` 来改变对齐方式。 `#program pack(0)` 表示默认的对齐方式。

## Vim用法

### 光标移动

#### 以单词方式移动

`w`: **下个** 单词 **首**

`e`: **下个** 单词 **尾**

`b`: **上个** 单词 **首**

> 注意如果光标处于一个单词的内部，那么 `b` 就是本单词的词首， `e`就是本单词的词尾。

