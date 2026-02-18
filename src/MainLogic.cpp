#include <stdio.h>
#include <time.h>
#include <graphics.h>

#include "MainLogic.h"
#include "Draw_and_Load.h"

//***主体逻辑***//

void MoveLogic(BYTE mes, int* record, boolean* run) {
    int checkEsc;
    boolean mv = 0; //判断是否移动，决定是否生成随机数
    boolean mixmv = 0;

    switch (mes) {
    case VK_UP: //向上控制
    case 0x57:  //W对应的虚拟按键码
        MixBlock('w', record, &mv, &mixmv);
        break;
    case VK_DOWN:   //向下控制
    case 0x53:  //S对应的虚拟按键码
        MixBlock('s', record, &mv, &mixmv);
        break;
    case VK_LEFT:   //向左控制
    case 0x41:  //A对应的虚拟按键码
        MixBlock('a', record, &mv, &mixmv);
        break;
    case VK_RIGHT:  //向右控制
    case 0x44:  //D对应的虚拟按键码
        MixBlock('d', record, &mv, &mixmv);
        break;
    case VK_ESCAPE:
    case 0x51:
        checkEsc = MessageBox(NULL, L"是否退出?", L"操作提示", MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON1 | MB_SYSTEMMODAL);
        if (checkEsc == IDOK) {
            *run = 0;
            checkEsc = MessageBox(NULL, L"是否保存？", L"游戏提示", MB_OKCANCEL);
            if (checkEsc == IDOK)
                MemoryStore();
        }
        break;
    default:
        break;
    }

    if (mv == 1) {
        if (mixmv == 1)
            mciSendString(L"play music from 0", NULL, 0, NULL); //音效控制
        GenerateRandomNum();   //若移动，则生成随机数
        return;
    }
    else
        return;
}
void GenerateRandomNum() {
    int j_x;
    int j_y;
    int i = rand() % 2;
    srand(time(NULL));

    do {
        j_x = rand() % 4;
        j_y = rand() % 4;
    } while (grp[j_x][j_y].num != 0);   //生成随机数
    grp[j_x][j_y].num = rdnum[i];
}
void MixBlock(char ch, int* record, boolean* mv, boolean* mixmv) {
    int i = 0;
    int j = 0;
    int target;

    switch (ch) {
    case 'w':
        for (j = 0; j < blk; ++j) {
            for (i = 1; i < blk; ++i) { //由于i=0无法向上移动，故i初始值为1，同时对于数组进行从上往下历遍
                if (grp[i][j].num != 0) {   //判断当前grp[i][j]是否为零，避免每个数据都历遍，提高效率
                    target = i;
                    while (target > 0 && grp[target - 1][j].num == 0) { //判断当前数据前一个数据是否为零，同时防止数组下标溢出，再决定是否跳出循环或继续向上寻找非零元素
                        target--;   //若为零则将当前i值赋予target，便于后面非零数据跳出循环替换
                    }
                    if (target > 0 && grp[target - 1][j].num == grp[i][j].num && grp[target - 1][j].mix == 0 && grp[i][j].mix == 0) {   //若当前元素为相同元素且为非合成块，则合并
                        grp[target - 1][j].num *= 2;    //非零相同元素合成
                        *mv = TRUE;
                        *mixmv = TRUE;
                        *record += grp[target - 1][j].num;  //记录得分
                        grp[target - 1][j].mix = 1; //将合成块合并状态更改，避免块的多次合成
                        grp[i][j].num = 0;  //当前数据归档为零
                    }
                    else if (target != i) { //若不相等，且target为最后一个为零数据的下标，则替换当前数据至该数组位置，原数据归档为零
                        grp[target][j].num = grp[i][j].num;
                        *mv = TRUE;
                        grp[i][j].num = 0;
                    }
                }
            }
            for (i = 0; i < blk; ++i) {
                grp[i][j].mix = 0;  //执行完每列后，更改当前列的mix属性，比最后调用函数chmix（自定义函数）效率更高
            }
        }
        break;
    case 's':
        for (j = 0; j < blk; ++j) {
            for (i = blk - 2; i >= 0; --i) {    //由于数组下标为0-3，且第四行无法向下移动（即数组下标为3），故i初始化为2，从下往上历遍，其他与向上移动逻辑类似
                if (grp[i][j].num != 0) {
                    target = i;
                    while (target < blk - 1 && grp[target + 1][j].num == 0) {   //防止数组溢出，故target < 3
                        target++;
                    }
                    if (target < blk - 1 && grp[target + 1][j].num == grp[i][j].num && grp[target + 1][j].mix == 0 && grp[i][j].mix == 0) {
                        grp[target + 1][j].num *= 2;
                        *mv = TRUE;
                        *mixmv = TRUE;
                        *record += grp[target + 1][j].num;
                        grp[target + 1][j].mix = 1;
                        grp[i][j].num = 0;
                    }
                    else if (target != i) {
                        grp[target][j].num = grp[i][j].num;
                        *mv = TRUE;
                        grp[i][j].num = 0;
                    }
                }
            }
            for (i = 0; i < blk; ++i) {
                grp[i][j].mix = 0;
            }
        }
        break;
    case 'a':
        for (i = 0; i < blk; ++i) {
            for (j = 1; j < blk; ++j) {
                if (grp[i][j].num != 0) {
                    target = j;
                    while (target > 0 && grp[i][target - 1].num == 0) {
                        target--;
                    }
                    if (target > 0 && grp[i][target - 1].num == grp[i][j].num && grp[i][target - 1].mix == 0 && grp[i][j].mix == 0) {
                        grp[i][target - 1].num *= 2;
                        *mv = TRUE;
                        *mixmv = TRUE;
                        *record += grp[i][target - 1].num;
                        grp[i][target - 1].mix = 1;
                        grp[i][j].num = 0;
                    }
                    else if (target != j) {
                        grp[i][target].num = grp[i][j].num;
                        *mv = TRUE;
                        grp[i][j].num = 0;
                    }
                }
            }
            for (j = 0; j < blk; ++j) {
                grp[i][j].mix = 0;
            }
        }
        break;
    case 'd':
        for (i = 0; i < blk; ++i) {
            for (j = blk - 2; j >= 0; --j) {
                if (grp[i][j].num != 0) {
                    target = j;
                    while (target < blk - 1 && grp[i][target + 1].num == 0) {
                        target++;
                    }
                    if (target < blk - 1 && grp[i][target + 1].num == grp[i][j].num && grp[i][target + 1].mix == 0 && grp[i][j].mix == 0) {
                        grp[i][target + 1].num *= 2;
                        *mv = TRUE;
                        *mixmv = TRUE;
                        *record += grp[i][target + 1].num;
                        grp[i][target + 1].mix = 1;
                        grp[i][j].num = 0;
                    }
                    else if (target != j) {
                        grp[i][target].num = grp[i][j].num;
                        *mv = TRUE;
                        grp[i][j].num = 0;
                    }
                }
            }
            for (j = 0; j < blk; ++j) {
                grp[i][j].mix = 0;
            }
        }
        break;
    }
}
void CheckRun(boolean* run) {
    int flag = 0;

    for (int i = 0; i < blk; ++i) {
        for (int j = 0; j < blk; ++j) {
            if (grp[i][j].num == MAX_Number) {
                *run = FALSE;
                MessageBox(NULL, L"恭喜通关！", L"游戏提示", MB_OK | MB_SYSTEMMODAL);
            }
            else if (grp[i][j].num != 0 && !CheckBlkAround(i, j))   //若该块不等于零，且周围均合成不了，则flag增加1
                ++flag;
        }
    }
    if (flag == 16) {
        MessageBox(NULL, L"游戏结束！", L"游戏提示", MB_OK | MB_SYSTEMMODAL);
        *run = FALSE;
    }
}
boolean CheckBlkAround(int i,int j) {
    if ((grp[i][j].num == grp[i - 1][j].num && i - 1 >= 0) || (grp[i][j].num == grp[i + 1][j].num && i + 1 < blk) || (grp[i][j].num == grp[i][j - 1].num && j - 1 >= 0) || (grp[i][j].num == grp[i][j + 1].num && j + 1 < blk)) //若附件仍有可以合成的块，则返回TRUE
        return TRUE;
    else //该方块周围均为无法合成的块，则返回FALSE
        return FALSE;
}
//***主体逻辑***//