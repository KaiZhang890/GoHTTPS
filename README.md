# GoHTTPS
Something about HTTPS. 学习HTTPS总结。

####与HTTP的不同
1. HTTP的URL以http://开头，默认端口80；HTTPS的URL以https://开头，默认端口443。
2. HTTPS协议栈。

![HTTPS协议栈](https://raw.githubusercontent.com/KaiZhang890/GoHTTPS/master/HTTPS协议栈.jpg)
	
####HTTP的问题
HTTP协议传输的数据都是明文的。三大风险：
	
	窃听风险（eavesdropping）：第三方可以获知通信内容。
	篡改风险（tampering）：第三方可以修改通信内容。
	冒充风险（pretending）：第三方可以冒充他人身份参与通信。
	
为解决小三的问题，NetScape公司在1994年就设计了SSL协议(Secure Socket Layer)的1.0版，1999年由互联网标准化组织接力，发布了TLS 1.0版。

SSL/TLS为了解决这三大风险而设计的：

	所有信息都是加密传播，第三方无法窃听。
	具有校验机制，一旦被篡改，通信双方会立刻发现。
	配备身份证书，防止身份被冒充。

####HTTPS安全吗
SSL/TSL协议不仅仅是一套加密传输的协议，更是一件精心设计的艺术品。

使用了非对称加密、对称加密以及HASH算法。

* 非对称加密算法：RSA，DSA/DSS
* 对称加密算法：AES，RC4，3DES
* HASH算法：MD5，SHA1，SHA256

####HTTPS握手过程
*客户端*: 告诉服务端自己所支持的加密规则。

**服务端**: 选定的加密算法和HASA算法，并返回身份证书（地址、公钥、颁发机构等）。

*客户端*: 验证证书，生成随机数密码，生成握手消息，计算消息HASH值，密码加密消息，公钥加密密码。

**服务端**: 用私钥取出密码，解密消息，验证HASH值。

*客户端*，**服务端** 传输正式消息，使用随机数密码做对称加密运算

**非对称加密算法一般比对称加密算法慢2-3个数量级（100-1000倍）**

####RSA计算步骤
```c
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

uint64_t inv(uint64_t e, uint64_t r) {
    uint64_t d;
    for (d=2; d<r; d++) {
        uint64_t re = (e*d) % r;
        if (re == 1) {
            //printf("e=%lld d=%lld r=%lld (e*d) mod r=%lld\n", e, d, r, re);
            return d;
        }
    }
    assert(0);
}

uint64_t power(uint64_t msg, uint64_t baseNum, uint64_t key) {
    uint64_t p = 1;
    for (uint64_t i=1; i <= key; i++) {
        p = (p * msg) % baseNum;
    }
    return p;
}

int main() {
    // 随机挑选两个质数p，q
    uint64_t p =  2213, q = 2663;
    uint64_t n = p * q;
    // r表示与整数n互为质数的个数。
    uint64_t r = (p - 1) * (q - 1);
    // 挑选一个数e，满足1<e<r，并且与r互为质数
    uint64_t e = 4723;
    // 计算e关于n的模反元素，ed % r = 1
    // 如果两个正整数a和n互质，则一定可以找到整数b，使得ab-1被n整除。这时b叫做a的模反元素。
    uint64_t d = inv(e, r);
    printf("公钥对：(%lld, %lld) 私钥对：(%lld, %lld)\n", n, e, n, d);
    // (n,e)组成公钥对，(n,d)组成私钥对
    
    
    // p=3,q=11 (33, 3) (33, 7)
    // p=13,q=19 (247, 17) (247, 89)
    // p=2213,q=2663 (5893219, 4723) (5893219, 52363)
    
    // 要加密的msg需小于n
    uint64_t msg = 5893220;
    uint64_t encodeMsg = power(msg, n, e);
    uint64_t decodeMsg = power(encodeMsg, n, d);
    printf("加密前：%lld 加密后：%lld 解密后：%lld\n", msg, encodeMsg, decodeMsg);
}
```

####HTTPS使用成本
* verisign公司的证书一般一年几千到几万不等。
* 用户访问速度。

### License
[WTFPL (Do What The Fuck You Want To Public License)](http://www.wtfpl.net)

####参考
http://www.guokr.com/post/114121/

http://blog.jobbole.com/86660/

http://www.ruanyifeng.com/blog/2014/02/ssl_tls.html

http://www.codedata.com.tw/social-coding/rsa-c-implementation/