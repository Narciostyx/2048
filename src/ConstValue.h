#pragma once

#include <stdio.h>
#include <graphics.h>

//常量声明

//初始化界面边长以及部分游戏常量
#define Line 400
#define blk 4
#define MAX_Number 2048

//结构体声明

//储存各块的数据信息
struct block {
    int num = 0;    //储存数字
    boolean mix = 0;    //储存是否为合成块的布尔值
};
//定义生成各块中心
extern const int ctr[4];

//伪随机数
extern const int rdnum[2];

//对应图片的数字数组
extern const int num[12];

//游戏内各块数
extern block grp[blk][blk];

//各方块颜色
extern IMAGE pic[11];