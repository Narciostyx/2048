#include <stdio.h>
#include <time.h>
#include <graphics.h>
#include <mmsystem.h>
#include <Windows.h>

#include "MainLogic.h"  //导入主体逻辑库
#include "Draw_and_Load.h"  //导入主体绘制库

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )   //避免生成程序跳出cmd命令行
#pragma comment (lib,"winmm.lib") //导入MCI库，导入并使用音乐


//主函数
int main() {
    mciSendString(L"open rsc\\msi\\BlockMix.mp3 alias music", NULL, 0, NULL);
    mciSendString(L"open rsc\\msi\\Background.mp3 alias BGM", NULL, 0, NULL);
    wchar_t recordCHAR[10]; //储存record的临时数组
    wchar_t record_MaxCHAR[10]; //存储record_max的临时数组
    int record_cur = 0; //记录当前得分
    int record_max = 0; //记录最高得分
    IMAGE img[4];   //存储开始界面背景,开始界面按钮与游戏主背景
    ExMessage ctr;  //获取键盘输入控制的消息类型
    boolean run = 1;    //游戏是否继续执行的判定条件
    BYTE mes = 0;   //读取键盘等BYTE数据类型输入
    FILE* fp;   //创建文件指针，便于存放记录

	record_max = RecordCheck();
	MemoryCheck();

    loadimage(&img[0], L"rsc\\pic\\start.png"); //载入开始界面背景
    loadimage(&img[1],L"rsc\\pic\\background.png");   //载入游戏背景
    mciSendString(L"play BGM repeat from 0", NULL, 0, NULL); 

    Start(&ctr,&img[0],&run,record_max);

    initgraph(Line + 100, Line, EX_NOCLOSE + EX_NOMINIMIZE);    //初始化界面

    LoadPic();  //加载数字数组对应的图片
    cleardevice();  //清除开始界面画面

    while (run) {
        BeginBatchDraw();   //开始执行游戏绘图
        cleardevice();  //清除当前内容，避免偶然因素导致的问题

        setbkcolor(RGB(68, 138, 202));  //设置背景颜色
        putimage(0,0,&img[1]);  //载入背景图片
        DrawNum();  //绘制图案

        if (record_cur > record_max)    //判断当前分数是否大于最高分
            record_max = record_cur;
        _itow_s(record_cur,recordCHAR,10, 10);  //使用itow函数将int类型数据转化成能够被outtextxy函数输出的wchar类型数据
        _itow_s(record_max, record_MaxCHAR, 10, 10);

        settextstyle(18,10,_T("方正舒体"));
        outtextxy(Line + 10, Line / 10 , L"得分：");
        outtextxy(Line + 40, Line / 10 + 22, recordCHAR);
        outtextxy(Line + 10, Line / 4, L"最高得分：");
        outtextxy(Line + 40, Line / 4 + 22, record_MaxCHAR);
        FlushBatchDraw();
        
        CheckRun(&run); //检测游戏是否能够继续进行
        if (run == FALSE)
            break;
        while (TRUE) {  //通过死循环不断读取输入
            if (peekmessage(&ctr, EX_KEY) && ctr.message == WM_KEYUP) { //判断是否输入按键，且通过按键弹起限定输入，避免按下按键输入两次
                mes = ctr.vkcode; //将输入键盘的虚拟键码传入mes
                break;
            }
            else
                Sleep(15);  //优化CPU资源占用
            continue;
        }
        MoveLogic(mes,&record_cur,&run);    //将键盘输入，当前记录值以及run布尔值传入主逻辑函数

        Sleep(15);     //优化CPU资源占用
        continue;
    }

    if (!fopen_s(&fp, "rsc\\TXT\\Records.txt", "r+")) {
        fscanf_s(fp, "Record_Max = %d", &record_max);
		fseek(fp, 0, SEEK_SET);
        if (record_cur > record_max)
            fprintf_s(fp, "Record_Max = %d", record_cur);
    }
    fclose(fp);

    mciSendString(L"close music", NULL, 0, NULL);
    mciSendString(L"close BGM", NULL, 0, NULL);
    EndBatchDraw();
    closegraph();
    return 0;
}