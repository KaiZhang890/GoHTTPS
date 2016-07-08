//
//  main.c
//  RSATheory
//
//  Created by zhangkai on 5/19/16.
//  Copyright © 2016 Kai Zhang. All rights reserved.
//

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
    uint64_t msg = 200;
    uint64_t encodeMsg = power(msg, n, e);
    uint64_t decodeMsg = power(encodeMsg, n, d);
    printf("加密前：%lld 加密后：%lld 解密后：%lld\n", msg, encodeMsg, decodeMsg);
}
