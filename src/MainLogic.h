#pragma once

#include <stdio.h>
#include <time.h>
#include <graphics.h>

#include "ConstValue.h"

//输入判断逻辑
void MoveLogic(BYTE mes, int* record, boolean* run);

//生成随机数
void GenerateRandomNum();

//合成反馈逻辑
void MixBlock(char ch, int* record, boolean* mv, boolean* mixmv);

//判定游戏输赢
void CheckRun(boolean* run);

//辅助判定方块是否能继续移动
boolean CheckBlkAround(int i,int j);