#include <stdio.h>
#include <time.h>
#include <graphics.h>

#include "Draw_and_Load.h"
#include "MainLogic.h"

//定义生成各块中心
const int ctr[4] = { Line / 8 ,Line / 8 * 3 ,Line / 8 * 5 ,Line / 8 * 7 };

struct bt { //按键结构体
    int x_pos;
    int y_pos;
    IMAGE btDown;
}Startbt[4] = {
    {280,220,NULL},{280,260,NULL},{280,300,NULL},{280,340,NULL}
};

//伪随机数
const int rdnum[2] = { 2,4 };

//对应图片的数字数组
const int num[12] = { 0,2,4,8,16,32,64,128,256,512,1024,2048 };

//声明常量头文件内的共有数据
block grp[blk][blk];
IMAGE pic[11];

void Start(ExMessage* msg, IMAGE* pic,boolean* run,int record_max) {
    BYTE kyb;   //读取按键
    FILE* fp;   //设置文件指针，便于读取存档
    WCHAR num[10];  //将读取的历史最大值改成wcahr类型，用outtextxy函数输出
    IMAGE btBan;    //禁用按键图片
    int checkEsc;   //判定是否按下退出确定键
    boolean start = TRUE;   //开始主界面判定是否继续
    int i = 0;
    errno_t err = fopen_s(&fp, "rsc\\TXT\\Array.txt", "r"); //打开存档文件
    if (err == 0)
        fclose(fp); //关闭文件

    _itow_s(record_max, num, 10, 10);   //将历史最高纪录转换为wchar类型
    initgraph(Line, Line, EX_NOCLOSE + EX_NOMINIMIZE);  //初始化开始界面
    loadimage(&Startbt[0].btDown, L"rsc\\pic\\btDown1.png");    //导入按键图片
    loadimage(&Startbt[1].btDown, L"rsc\\pic\\btDown2.png");
    loadimage(&Startbt[2].btDown, L"rsc\\pic\\btDown3.png");
    loadimage(&Startbt[3].btDown, L"rsc\\pic\\btDown4.png");
    loadimage(&btBan, L"rsc\\pic\\btBan.png");

    while (start) {
        while (TRUE) {
            cleardevice();
            putimage(0, 0, pic);    //放置开始界面图片
            if (err != 0)
                putimage(Startbt[1].x_pos, Startbt[1].y_pos, &btBan);   //若打开存档文件失败，则禁用该按键
            putimage(Startbt[i].x_pos,Startbt[i].y_pos, &Startbt[i].btDown);
            if (peekmessage(msg, EX_KEY) && msg->message == WM_KEYUP) {
                kyb = msg->vkcode;  //读取键盘读取
                break;
            }
            else
                Sleep(15);  //缓解while循环CPU占用
            continue;
        }
        switch (kyb) {
        case VK_UP: //向上按键控制
        case 0x57:
            --i;
            if (i == 1 && err != 0) //不存在游戏存档，则跳过该按钮
                --i;
            if (i < 0)  //保证i始终在0-3之间
                i = 3;
            mciSendString(L"play music from 0", NULL, 0, NULL); //按键音效
            break;
        case VK_DOWN:   //向下按键控制
        case 0x53:
            ++i;
            if (i == 1 && err != 0)
                ++i;
            if (i > 3)
                i = 0;
            mciSendString(L"play music from 0", NULL, 0, NULL);
            break;
        case VK_ESCAPE: //退出按键控制
        case 0x51:
            checkEsc = MessageBox(NULL, L"是否退出?", L"操作提示", MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON1 | MB_SYSTEMMODAL);
            if (checkEsc == IDOK) {
                *run = 0;
                start = 0;
            }
            mciSendString(L"play music from 0", NULL, 0, NULL);
            break;
        case VK_RETURN: //确定按键控制
            if (i == 3) {
                checkEsc = MessageBox(NULL, L"是否退出?", L"操作提示", MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON1 | MB_SYSTEMMODAL);
                if (checkEsc == IDOK) {
                    *run = 0;
                    start = 0;
                }
            }
            else
                StartBt(i, &start, num);
        default:
            break;
        }
        continue;
    }
    closegraph();
}
void DrawNum() {
    int i = 0;
    int j = 0;
    int n = 0;

    for (i = 0; i < blk; i++)
        for (j = 0; j < blk; j++)
            if (grp[i][j].num != 0)  //判断当前数据是否为零，若非，则绘制图像
                for (n = 1; n < 12; n++)
                    if (grp[i][j].num == num[n])
                        putimage(ctr[j] - Line / 8, ctr[i] - Line / 8, &pic[n]);    //调用对应的数字图片
}
void LoadPic() {
    loadimage(&pic[0], L"rsc\\pic\\pic1.png");  //导入图片
    loadimage(&pic[1], L"rsc\\pic\\pic2.png");
    loadimage(&pic[2], L"rsc\\pic\\pic3.png");
    loadimage(&pic[3], L"rsc\\pic\\pic4.png");
    loadimage(&pic[4], L"rsc\\pic\\pic5.png");
    loadimage(&pic[5], L"rsc\\pic\\pic6.png");
    loadimage(&pic[6], L"rsc\\pic\\pic7.png");
    loadimage(&pic[7], L"rsc\\pic\\pic8.png");
    loadimage(&pic[8], L"rsc\\pic\\pic9.png");
    loadimage(&pic[9], L"rsc\\pic\\pic10.png");
    loadimage(&pic[10], L"rsc\\pic\\pic11.png");
}
void StartBt(int num,boolean* start,WCHAR* n) {
    switch (num) {
    case 0:
        for (int turn = 0; turn < 2; ++turn)    //提前绘制两个随机数
            GenerateRandomNum();
        MessageBox(NULL, L"使用\'W\',\'S\'.\'A\',\'D\'或方向键控制方块方向,Esc或\'Q\'键退出程序", L"游戏提示", MB_OK | MB_DEFBUTTON1 | MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
        *start = FALSE;
        break;
    case 1:
        MemoryGet();
        *start = FALSE;
        break;
    case 2:
        MessageBox(NULL, n, L"历史最高记录得分", MB_OK | MB_SYSTEMMODAL);
        break;
    }
}
void MemoryStore() {
    FILE* fp;
    int i = 0;
    int j = 0;

    errno_t err = fopen_s(&fp, "rsc\\TXT\\Array.txt", "w");
    if (err == 0) { //判定是否存在存档文件
        for (i = 0; i < blk; ++i)
            fprintf_s(fp, "%d %d %d %d\n", grp[i][0].num, grp[i][1].num, grp[i][2].num, grp[i][3].num);
        fclose(fp);
    }
    else //不存在文件，退出
        MessageBox(NULL, L"出于某些原因，储存失败！", L"游戏提示", MB_ICONSTOP);
}
void MemoryGet() {
    FILE* fp;
    int i, n, flag,j;

    errno_t err = fopen_s(&fp, "rsc\\TXT\\Array.txt", "r");
    if (err == 0) { //判定是否存在该文件
        for (i = 0; i < blk; ++i)
            fscanf_s(fp, "%d %d %d %d", &grp[i][0].num, &grp[i][1].num, &grp[i][2].num, &grp[i][3].num);
        MessageBox(NULL, L"游戏存档读取成功", L"游戏提示", MB_OK | MB_SYSTEMMODAL);
        fclose(fp);
        for (i = 0, j = 0; i < blk && j < blk; ++j) {
            flag = 0;
            int test = grp[i][j].num;
            for (n = 0; n < 12; ++n) {
                if (grp[i][j].num == num[n]) {
                    flag = 1;
                    break;
                }
            }
            if (j == 3) {
                ++i;
                j = 0;
            }
            if (flag)
                continue;
            else {
                MessageBox(NULL, L"游戏存档存在错误，请删除重建", L"游戏提示", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
                exit(-2);
            }
        }
        for (i = 0; i < blk; ++i)
            for (j = 0; j < blk; ++j)
                if (grp[i][j].num != 0)
					return;
		MessageBox(NULL, L"游戏存档为空,自动新建", L"游戏提示", MB_OK | MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
        for (int turn = 0; turn < 2; ++turn)    //提前绘制两个随机数
            GenerateRandomNum();
    }
    return;
}
void MemoryCheck() {
    FILE* fp;
    int i, j;

    errno_t err = fopen_s(&fp, "rsc\\TXT\\Array.txt", "r");
    if (err != 0) { //判定是否存在该文件
        int feedback = MessageBox(NULL, L"不存在对应的保存数组文件，是否创建该类文件？", L"游戏提示", MB_OKCANCEL | MB_SYSTEMMODAL | MB_ICONSTOP);    //弹出窗口，询问用户是否需要生成
        if (feedback == IDOK) { //若确认，尝试建立该文件
            err = fopen_s(&fp, "rsc\\TXT\\Array.txt", "w");
            if (err == 0) {
                for (i = 0; i < blk; ++i) {
                    for (j = 0; j < blk; ++j)
                        fprintf_s(fp, "%d ", grp[i][j].num);
                    fprintf_s(fp, "\n");
                }
                fclose(fp);
                return;
            }
            else //若建立失败，警告用户
                MessageBox(NULL, L"文件创建失败！", L"游戏提示", MB_SYSTEMMODAL | MB_ICONSTOP);
            return;
        }
        else //若用户取消或关闭，警告
            MessageBox(NULL, L"若不创建文件，则将无法保存历史记录！", L"游戏提示", MB_SYSTEMMODAL | MB_ICONEXCLAMATION);
        return;
    }
    else
		fclose(fp);
        return;
}

int RecordCheck() {
    FILE* fp;   //创建文件指针，便于存放记录
    int record_max = 0;
    errno_t err = fopen_s(&fp, "rsc\\TXT\\Records.txt", "r"); //fopen_s函数使用，判断是否存在记录文件
    if (err != 0) { //fopen_s函数打开文件返回0，不存在或打不开返回非零值
        int feedback = MessageBox(NULL, L"不存在对应的储存分数文件，是否创建文件？", L"游戏提示", MB_OKCANCEL | MB_SYSTEMMODAL | MB_ICONEXCLAMATION);   //询问用户是否需要创建该文件
        if (feedback == IDOK) { //用户点击“确定”按钮
            fopen_s(&fp, "rsc\\TXT\\Records.txt", "w");
            fprintf_s(fp, "Record_Max = %d", record_max);   //写入数据
            fclose(fp); //关闭文件
        }
        else
            MessageBox(NULL, L"未创建文件则将无法保存最高历史记录！", L"游戏提示", MB_SYSTEMMODAL | MB_ICONEXCLAMATION); //提示用户未创建文件的后果
    }
    else if (err == 0) {
        fscanf_s(fp, "Record_Max = %d", &record_max);   //若文件开始存在，则读取最大记录值
        fclose(fp);
    }
    return record_max;
}