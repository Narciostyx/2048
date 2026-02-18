#pragma once

#include "ConstValue.h"

//开始界面
void Start(ExMessage* msg, IMAGE* pic,boolean* run,int record_max);

//绘制游戏数字
void DrawNum();

//载入对应方块图片
void LoadPic();

//设置开始界面四个按键功能集合
void StartBt(int num,boolean* start,WCHAR* n);

//暂存游戏当前数组值
void MemoryStore();

//获取保存在文件的暂存值
void MemoryGet();

//判断当前文件是否存在，若否，则创建
void MemoryCheck();

//判断分数储存文件是否存在
int RecordCheck();