/**
 * 用1500行代码从0开始实现TCP/IP协议栈+WEB服务器
 *
 * 本源码旨在用最简单、最易懂的方式帮助你快速地了解TCP/IP以及HTTP工作原理的主要核心知识点。
 * 所有代码经过精心简化设计，避免使用任何复杂的数据结构和算法，避免实现其它无关紧要的细节。
 *
 * 作者：李述铜
 * 微信公众号：01课堂
 * 网址：https://www.yuque.com/lishutong-docs
 *
 * 版权声明：源码仅供学习参考，请勿用于商业产品，不保证可靠性。二次开发或其它商用前请联系作者。
 *
 * 注意：本课程提供的tcp/ip实现很简单，只能够用于演示基本的协议运行机制。我还开发了另一套更加完整的课程，
 * 展示了一个更加完成的TCP/IP协议栈的实现。功能包括：
 * 1. IP层的分片与重组
 * 2. Ping功能的实现
 * 3. TCP的流量控制等
 * 4. 基于UDP的TFTP服务器实现
 * 5. DNS域名接触
 * 6. HTTP服务器
 * 7. 提供socket接口供应用程序使用
 * 8、代码可移植，可移植到arm和x86平台上
 * ..... 更多功能开发中...........
 * 如果你有兴趣的话，请扫仓库中的二维码，或者点击以上面的链接可找到该课程。
 */
#include "tserver_datetime.h"
#include <string.h>
#include <time.h>

#define TIME_STR_SIZE       128         // 时间字符串存储长度

static xnet_err_t datetime_handler (xudp_t * udp, xipaddr_t * src_ip, uint16_t src_port, xnet_packet_t * packet) {
    xnet_packet_t * tx_packet;
    time_t rawtime;
    const struct tm * timeinfo;

    tx_packet = xnet_alloc_for_send(TIME_STR_SIZE);
    if (tx_packet == (xnet_packet_t *)0) {
        return XNET_ERR_MEM;
    }

    // 参见：http://www.cplusplus.com/reference/ctime/localtime/
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    // strftime参见：http://www.cplusplus.com/reference/ctime/strftime/
    // Weekday, Month Day, Year Time-Zone
    strftime((char *)tx_packet->data, TIME_STR_SIZE, "%A, %B %d, %Y %T-%z", timeinfo);
    truncate_packet(tx_packet, (uint16_t)strlen((char *)tx_packet->data));

    return xudp_out(udp, src_ip, src_port, tx_packet);
}

xnet_err_t xserver_datetime_create(uint16_t port) {
    xnet_err_t err;

    xudp_t* udp = xudp_open(datetime_handler);
    if (udp == (xudp_t*)0) {
        return -1;
    }

    err = xudp_bind(udp, port);
    if (err < 0) {
        xudp_close(udp);
        return err;
    }
    return 0;
}
