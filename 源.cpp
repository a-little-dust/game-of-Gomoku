#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <graphics.h>    //easyx图形库的头文件
#include <vector>
#include <fstream>//文件流
#include<mmsystem.h>//这是包含多媒体设备接口头文件
#pragma comment(lib,"winmm.lib")//加载静态库
using namespace std;

#define bkcolor RGB(255,245,247)//统一设置界面底色
#define txtcolor RGB(69,137,148)//统一设置字体颜色
#define txtcolor2 RGB(22,133,169)//另一种字体颜色
#define lncolor RGB(114,83,52)//网格线的颜色
#define LINEDIST  64  //网格线之间的距离
#define SCREENWIDTH 580//屏幕宽
#define SCREENHIGH   620//屏幕高
#define R 30   //珠子的半径大小
#define start_num 7   //游戏开始时的珠子数 7
#define array_size 9 //棋盘大小
#define max_num 81//棋盘能容纳的最多珠子数
#define edge 40//边缘要预留的宽度

int colorr[9][9] = { 0 };   //记录整个棋盘上珠子的颜色，值为0代表还没有珠子
int score=0;//得分
int countt=0;//棋盘上的珠子数，用来判断游戏是否结束
bool if_music = true;
class bead {
public:
	void setcolor();//根据随机数设置对应的填充颜色
	bead(int row, int col);//在某行某列生成随机一种颜色的珠子
	void print();//画珠子
	void erase();//擦除珠子
	int getcolor();//返回color编号（1到6）
	bead(int row, int col, int color_);//在某行某列生成指定颜色的珠子
private:
	int color = rand() % 6 + 1;        //珠子的颜色
	int x;            //珠子圆心在界面上的横坐标
	int y;            //珠子圆心在界面上的纵坐标
};
//珠子
bead* pbead[9][9];
struct grid {
	int row;
	int col;
};
//把网格定义成结构体
vector <grid> path ;//存最短路径
int best = 100;//存最短路径长
bead::bead(int row, int col)
{
	y =2+ LINEDIST / 2 + row * LINEDIST;//这里2是半个线宽
	x =2+ LINEDIST / 2 + col * LINEDIST;
}
bead::bead(int row, int col, int color_)
{
	y = 2 + LINEDIST / 2 + row * LINEDIST;//这里2是半个线宽
	x = 2 + LINEDIST / 2 + col * LINEDIST;
	color = color_;
}
void bead::setcolor()
{
	switch (color) {
	case 1:
		setfillcolor(RGB(195,33,54));        //红
		break;
	case 2:
		setfillcolor(RGB(69,137,148));              //青绿
		break;
	case 3:
		setfillcolor(RGB(255,182,193));           //粉
		break;
	case 4:
		setfillcolor(RGB(167,220,224));                  //浅蓝
		break;
	case 5:
		setfillcolor(RGB(117,36,35));         //棕
		break;
	case 6:
		setfillcolor(RGB(185, 121, 177));        //紫
		break;
	default:
		break;
	}
}
void bead::print()
{
	countt++;
	setcolor();
	solidcircle(x, y, R);
}
void bead::erase()
{
	setfillcolor(bkcolor);
	solidcircle(x, y, R);
	countt--;
}
int bead::getcolor()
{
	return color;
}
void newBead()
{
	//用随机数取一个位置，判断这里有没有被占用，然后生成珠子
	while (true)//如果这里已被占用，就要重新生成珠子
	{
		int bead_col = rand() % 9;
		int bead_row = rand() % 9;//分别随机生成行、列
		if (colorr[bead_row][bead_col] == 0)
		{//如果这里原本没有珠子
			pbead[bead_row][bead_col] = new(nothrow) bead(bead_row, bead_col);//用nothrow方式生成对象，如果分配失败会得到空指针
			if (pbead[bead_row][bead_col] == NULL) {
				cout << "申请空间失败" << endl;
				exit(-1);
			}			
			pbead[bead_row][bead_col]->print();//画出来
			colorr[bead_row][bead_col] = pbead[bead_row][bead_col]->getcolor();//给colorr数组赋值
			break;
		}
	}
}
//随机生成一个珠子
struct gameScore
{
	int fen = 0;//初始化为0，如果此时没有玩家的话排行榜就会显示“分数0，虚位以待”
	string name = "虚位以待";
};
//把分数定义成结构体
bool PointInRect(int x, int y, RECT& r)         
{
	return (x >= r.left && x <= r.right && y <= r.bottom && y >= r.top);
}
//用于判断鼠标是否在指定区域
void playmusic(int alt = 0)
{
	if (if_music == false)//如果音效关闭，则不播放音乐
		return;
	TCHAR szBuffer[100] = { 0 };
	TCHAR Buffer[100] = { 0 };
	switch (alt)
	{
	case 0://播放选中的音乐
		wsprintf(szBuffer, _T("xuanzhong.mp3"));
		break;
	case 1://播放消除的音乐
		wsprintf(szBuffer, _T("xiaochu.mp3"));
		break;
	case 2://播放无法选中的音乐
		wsprintf(szBuffer, _T("fail.mp3"));
		break;
	case 3://播放游戏结束的音乐
		wsprintf(szBuffer, _T("gameover.mp3"));
		break;
	}
	wsprintf(Buffer, _T("close \"%s\""), szBuffer);//如果原本已经在播放，要停止前一个音乐
	mciSendString(Buffer, 0, 0, 0);
	wsprintf(Buffer, _T("open \"%s\""), szBuffer);
	mciSendString(Buffer, NULL, 0, NULL);//打开设备
	wsprintf(Buffer, _T("play \"%s\""), szBuffer);
	mciSendString(Buffer, NULL, 0, NULL);//播放
}
//播放音乐
int begin()
{	
	//有哪些功能就写哪些字
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");
	LPCTSTR tother = _T("其他");
	BeginBatchDraw();//先画界面再统一展示，防止显示不稳定
	setbkcolor(bkcolor);
	cleardevice();
	settextstyle(40, 0, _T("黑体"));//参数为：高度，宽度，字体
	settextcolor(txtcolor);

	RECT rplay, rexit,rother;
	rplay.left = SCREENWIDTH / 2 - textwidth(tplay) / 2;
	rplay.right = rplay.left + textwidth(tplay);
	rplay.top = SCREENHIGH / 5 * 2.5;
	rplay.bottom = rplay.top + textheight(tplay);
	rexit.left = SCREENWIDTH / 2 - textwidth(texit) / 2;
	rexit.right = rexit.left + textwidth(texit);
	rexit.top = SCREENHIGH / 5 * 3.3;
	rexit.bottom = rexit.top + textheight(texit);
	rother.left= SCREENWIDTH / 2 - textwidth(tother) / 2;
	rother.right = rother.left + textwidth(tother);
	rother.top = SCREENHIGH / 5 * 4.1;
	rother.bottom = rother.top + textheight(tother);
	
	outtextxy(rplay.left, rplay.top, tplay);//参数是x，y，字符串。
	outtextxy(rexit.left, rexit.top, texit);
	outtextxy(rother.left, rother.top, tother);

	//加入标题
	IMAGE title;
	loadimage(&title, _T("title.png"));
	putimage(SCREENWIDTH / 2 - title.getwidth() / 2, SCREENHIGH / 5-title.getheight()/2, &title);

	EndBatchDraw();
	playmusic();
	while (true) {//接收用户信息，如果用鼠标点进选项就转到相应界面
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton) //按下左键
		{
			if (PointInRect(mess.x, mess.y, rplay))//开始游戏
			{
				return 1;
			}
			else if (PointInRect(mess.x, mess.y, rexit))//退出游戏
			{
				cout << "\n游戏已结束。\n";
				exit(0);//退出程序并返回0
			}
			else if (PointInRect(mess.x, mess.y, rother))//“其他
			{				
				return 2;
			}
		}
	}
}
//开始界面
void printline()            
{
	int i;
	setlinestyle(PS_SOLID, 4);//实线，线宽为4
	setlinecolor(lncolor);
	for (i = 0; i <= 9; i++) {//一共要画十条竖线
		line(2+i*LINEDIST, 0, 2 + i * LINEDIST, SCREENWIDTH-2);
	}
	//
	for (i = 0; i <= 9; i++) {//十条横线
		line(0,2 + i * LINEDIST,SCREENWIDTH,2 + i * LINEDIST);
	}
}
//画棋盘的网格
void lightgrid(int row,int col,int c=0)
{
	setlinestyle(PS_SOLID, 4);//实线，线宽为4
	switch (c)
	{
	case 0://复原
		setlinecolor(lncolor);
		rectangle(2 + col * LINEDIST, 2 + row * LINEDIST, 2 + (col + 1) * LINEDIST, 2 + (row + 1) * LINEDIST);//画一个有边框的矩形
		break;
	case 1://选中
		playmusic();
		setlinecolor(RGB(244, 208, 0));
		rectangle(2 + col * LINEDIST, 2 + row * LINEDIST, 2 + (col + 1) * LINEDIST, 2 + (row + 1) * LINEDIST);
		break;
	case 2://消除
		playmusic(1);
		setlinecolor(RGB(227, 249, 253));//月白色
		rectangle(2 + col * LINEDIST, 2 + row * LINEDIST, 2 + (col + 1) * LINEDIST, 2 + (row + 1) * LINEDIST);
		break;
	case 3://不能到达
		playmusic(2);
		setlinecolor(RGB(204,202,169));//灰色
		rectangle(2 + col * LINEDIST, 2 + row * LINEDIST, 2 + (col + 1) * LINEDIST, 2 + (row + 1) * LINEDIST);
		break;
	}
}
//修改网格颜色
void outputScore()
{
	settextstyle(20, 0, _T("楷体"));
	settextcolor(txtcolor);
	//设置字符串
	TCHAR scr[15];//用来存得分
	_stprintf(scr, _T("得分 ：%d"), score);//把整型转为字符串，再存进字符数组
	//输出
	int top = SCREENWIDTH + (SCREENHIGH - SCREENWIDTH) / 2 - textheight(scr) / 2;
	outtextxy(SCREENWIDTH - edge - textwidth(scr),top, scr);
}
//实时显示分数
void updateNum()//它没有跟实时显示分数放在一起，因为珠子数增多了得分未必有改变
{
	settextstyle(20, 0, _T("楷体"));
	settextcolor(txtcolor);
	//设置字符串
	TCHAR num[15];//用来存珠子
	_stprintf(num, _T("珠子数 ：%d"), countt);
	//输出
	int top = SCREENWIDTH + (SCREENHIGH - SCREENWIDTH) / 2 - textheight(num) / 2;
	outtextxy(SCREENWIDTH / 2 - textwidth(num) / 2, top, num);
}
//实时显示珠子数
bool ifInArray(int i, int j)
{
	if (i >= 0 && i < array_size)
		if (j >= 0 && j < array_size)
			return true;
	return false;
}
//判断是否数组越界
void noteRepeat(bool flag[][9])
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (colorr[i][j] == 0)
				continue;//如果这里没有珠子，可以不用找
			int a1 = i;
			int b1 = j;//用ab去试探，a1b1记录要消除的点
			int ccount = 0;//记录多少个珠子连在一条线上
			//消除纵向
			for (int a = i, b = j; ifInArray(a + 1, b) && colorr[a][b] == colorr[a + 1][b]; a++)
			{
				ccount++;
			}
			if (ccount >= 4)//如果有4次相同，说明有5个连在一起
			{
				for (a1, b1; a1 <= i + ccount; a1++)
				{
					flag[a1][b1] = 1;
					lightgrid(a1, b1, 2);//把这个位置高亮
				}
			}
			//消除横向
			a1 = i, b1 = j;
			ccount = 0;
			for (int a = i, b = j; ifInArray(a, b + 1) && colorr[a][b] == colorr[a][b + 1]; b++)
			{
				ccount++;
			}
			if (ccount >= 4)
			{
				for (a1, b1; b1 <= j + ccount; b1++)
				{
					flag[a1][b1] = 1;
					lightgrid(a1, b1, 2);
				}
			}
			//消除斜向右下
			a1 = i, b1 = j;
			ccount = 0;
			for (int a = i, b = j; ifInArray(a + 1, b + 1) && colorr[a][b] == colorr[a + 1][b + 1]; a++, b++)
			{
				ccount++;
			}
			if (ccount >= 4)
			{
				for (a1, b1; a1 <= i + ccount && b1 <= j + ccount; a1++, b1++)
				{
					flag[a1][b1] = 1;
					lightgrid(a1, b1, 2);
				}
			}
			//消除斜向左下
			a1 = i, b1 = j;
			ccount = 0;
			if (i > 0) {
				for (int a = i, b = j; ifInArray(a - 1, b + 1) && colorr[a][b] == colorr[a - 1][b + 1] ; a--, b++)
				{
					ccount++;
				}
			}
			if (ccount >= 4)
			{
				for (a1, b1; b1 <= j + ccount; a1--, b1++)
				{
					flag[a1][b1] = 1;
					lightgrid(a1, b1, 2);
				}
			}
		}
}
//记录哪些地方需要修改
int delBead()
{
	bool flag[9][9];//记录要消除的内容
	noteRepeat(flag);
	int nnum = 0;//记录这次可以消去多少个珠子，然后转为分数
	for(int i=0;i<9;i++)
		for (int j = 0; j < 9; j++)
		{
			if (flag[i][j] == 1)
			{
				if (nnum == 0)//发现可以消除时
				{
					playmusic(1);
					Sleep(1500);//让动画停留1.5秒钟				
				}
				nnum++;
				pbead[i][j]->erase();//擦去珠子
				lightgrid(i, j);//网格线复原
				delete pbead[i][j];//销毁对象
				colorr[i][j] = 0;
			}
		}	
	if (nnum > 0)
	{
		updateNum();
		score += nnum * 2;//修改得分
		outputScore();
		return 1;
	}
	else
		return 0;//表示没有消除，则要继续添加珠子
}
//消除，返回1或0代表是否有发生消除
void find_in_way(int row, int col, bool way[][array_size])
{
	way[row][col] = true;//不重复经过同一个点
	int dirs[4][2] = { {-1,0},{1,0},{0,1},{0,-1} };
	for (int d = 0; d < 4; d++)
	{
		int next_row = row + dirs[d][0];
		int next_col = col + dirs[d][1];
		if (ifInArray(next_row, next_col))//数值合法
			if (colorr[next_row][next_col] == 0 && way[next_row][next_col] == false)//可以走
			{
				find_in_way(next_row, next_col, way);
			}
	}
}
//对每个可以经过的点，向四面搜索
bool findway(int row, int col, int dstrow, int dstcol)
{
	bool way[array_size][array_size]={0};
	find_in_way(row, col, way);
	return way[dstrow][dstcol];
}
//判断能否找到一条路径，这种方法能保证每个地方只搜索一次，效率高
void findbest(int row, int col, int dstrow, int dstcol,int cost=-1)
{
	cost++;
	if (cost >= best)//如果当前cost已经超额，说明它一定不是最短路径
	{
		return;
	}	
	if (row == dstrow && col == dstcol)//表示到达终点
	{
		best = cost;//修改最短路径长度
		return;
	}	
	int recolor = colorr[row][col];
	colorr[row][col] = -1;//防止走回头路
	int dirs[4][2] = { {-1,0},{1,0},{0,1},{0,-1} };
	for (int d = 0; d < 4; d++)
	{
		int next_row = row + dirs[d][0];
		int next_col = col + dirs[d][1];
		if(ifInArray(next_row,next_col))//数值合法
			if (colorr[next_row][next_col] == 0)//可以走
			{
				findbest(next_row, next_col, dstrow, dstcol, cost);
			}
	}
	colorr[row][col] =recolor;
}
//寻找最短路径长
int findPath(int row, int col, int dstrow, int dstcol, int cost=-1)
{
	cost++;
	grid curr{ row,col };
	if (row == dstrow && col == dstcol&&cost==best)//表示到达终点且这是最短路径
	{
		path.push_back( curr);//存进数组
		best=best-100;//为防止找到多条最小路径，找到一条以后将best修改，找完再换回来
		return 0;
	}
	if (cost >= best)//如果当前没到终点且cost已经超额，说明它一定不是最短路径
	{
		return 1;
	}	
	int recolor = colorr[row][col];
	colorr[row][col] = -1;//防止走回头路
	int dirs[4][2] = { {-1,0},{1,0},{0,1},{0,-1} };
	int test = 1;
	for (int d = 0; d < 4; d++)
	{
		int next_row = row + dirs[d][0];
		int next_col = col + dirs[d][1];
		if (ifInArray(next_row, next_col))//数值合法
			if (colorr[next_row][next_col] == 0)//可以走
			{//只要有一条是最短路径，就会返回0，test就会变成0
				test*= findPath(next_row, next_col, dstrow, dstcol, cost);
			}
	}
	colorr[row][col] = recolor;
	if (test == 0)//说明经过这个点找到了最短路径
	{
		path.push_back(curr);//存进数组
	}
	return test;
}
//找到最短的路径，并将它存进数组
int getRank()
{
	int rank = 1;//从第一名开始往后排
	ifstream in;
	in.open("game_record.txt", ios::in);//读取文件
	int old_fen;
	string old_name;
	while (!in.eof())
	{
		in >> old_fen >> old_name;
		if (old_fen > score)
			rank++;
	}
	in.close();
	return rank;
}
//读取文件，获得当前排名
void gameOver()
{	
	settextstyle(40, 0, _T("楷体"));//参数为：高度，宽度，字体
	settextcolor(txtcolor2);
	//有哪些功能就写哪些字
	LPCTSTR tscore = _T("本次您的得分为：");
	TCHAR scr[5];//用来存得分
	_stprintf(scr, _T("%d"), score);
	LPCTSTR trank = _T("在所有场次中，本次成绩排名为：");
	int rank = getRank();
	TCHAR rnk[5];
	_stprintf(rnk, _T("%d"), rank);
	LPCTSTR choose = _T("是否保存本次记录？");
	LPCTSTR remind = _T("请在控制台上输入您的用户名");
	LPCTSTR tyes = _T("  是  ");
	LPCTSTR tno = _T("  否  ");
    BeginBatchDraw();//先画界面再统一展示，防止显示不稳定
	setbkcolor(bkcolor);
	cleardevice();

	RECT ryes,rno;
	ryes.left = edge;
	ryes.right = ryes.left + textwidth(tyes);
	ryes.bottom = SCREENHIGH - edge;
	ryes.top = ryes.bottom - textheight(tyes);
	rno.right = SCREENWIDTH - edge;
	rno.left = rno.right - textwidth(tno);
	rno.top = ryes.top;
	rno.bottom = ryes.bottom;
	rectangle(ryes.left, ryes.top, ryes.right, ryes.bottom);
	rectangle(rno.left, rno.top, rno.right, rno.bottom);
	outtextxy(ryes.left, ryes.top, tyes);
	outtextxy(rno.left, rno.top, tno);

	//加入标题
	IMAGE ending;
	loadimage(&ending, _T("ending.png"));
	putimage(SCREENWIDTH / 2 - ending.getwidth() / 2, SCREENHIGH / 5 - ending.getheight() / 2, &ending);

	settextstyle(30, 0, _T("楷体"));//参数为：高度，宽度，字体
	outtextxy(SCREENWIDTH / 2 - textwidth(tscore)/2-textwidth(scr)/2, SCREENHIGH / 10 * 3.6, tscore);
	outtextxy(SCREENWIDTH / 2 + textwidth(tscore) / 2 - textwidth(scr) / 2, SCREENHIGH / 10 * 3.6, scr);
	outtextxy(SCREENWIDTH / 2 - textwidth(trank) / 2 - textwidth(rnk) / 2, SCREENHIGH / 10 * 5.2, trank);
	outtextxy(SCREENWIDTH / 2 + textwidth(trank) / 2 - textwidth(rnk) / 2, SCREENHIGH / 10 * 5.2, rnk);
	settextstyle(40, 0, _T("楷体"));
	outtextxy(SCREENWIDTH / 2 - textwidth(choose) / 2, SCREENHIGH / 10 * 6.8, choose);

	EndBatchDraw();
	playmusic(3);
	while (true) {//接收用户信息，如果用鼠标点进选项就转到相应界面
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton) //按下左键
		{
			if (PointInRect(mess.x, mess.y, ryes))//如果用户要保存记录
			{
				playmusic();
				setfillcolor(bkcolor);
				solidrectangle(ryes.left-2,ryes.top-2, ryes.right+2, ryes.bottom+2);
				solidrectangle(rno.left-2,rno.top-2, rno. right+2, rno.bottom+2);
				outtextxy(SCREENWIDTH / 2 - textwidth(remind) / 2, SCREENHIGH / 10 * 8.4, remind);
				string s;
				cout << "请输入你的用户名（不超过10个字符）：";
				cin >> s;
				ofstream out;//输出的文件流
				out.open("game_record.txt", ios::app);
				out  << score << " " << s<<endl;//将用户信息添加到文件后面
				out.close();
				return;
			}
			else if (PointInRect(mess.x, mess.y, rno))//如果用户不保存记录
			{
				return;
			}
		}
	}
}
//游戏结束
void newGame() {	
	playmusic();
	//初始化各变量
	score = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			colorr[i][j] = 0;//刚开始时没有珠子，所以没有颜色
	countt = 0;
	//画界面
	setbkcolor(bkcolor);
	cleardevice();
	printline();
	settextstyle(20, 0, _T("楷体"));
	settextcolor(txtcolor);
	//设置退出功能
	LPCTSTR texit = _T("结束游戏");
	RECT rexit;
	rexit.left = edge;
	rexit.right = rexit.left + textwidth(texit);
	rexit.top = SCREENWIDTH + (SCREENHIGH - SCREENWIDTH) / 2 - textheight(texit) / 2;
	rexit.bottom = rexit.top + textheight(texit);
	outtextxy(rexit.left, rexit.top, texit);
	//实时显示分数
	outputScore();
	//画界面上的珠子。初始时刻，珠子数为start_num
	for (int i = 0; i < start_num; i++)
		newBead();
	updateNum();
	//生成所有珠子后，判断能否消除，然后产生动画
	delBead();
	//接下来正式开始游戏
	int oldrow = -1, oldcol = -1;//代表上一个选中的珠子
	int midrow = -1, midcol = -1;//代表上一个选中的空格子
	while (1)//while代表持续地获取信息
	{	
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);      //获取鼠标信息
		if (mess.lbutton)
		{               //如果鼠标左键按下
			MOUSEMSG m = GetMouseMsg();
			if (PointInRect(m.x, m.y, rexit))
			{
				break;
			}     //判断玩家是否在游戏过程中点击了“退出游戏”按钮
			int row, col;
			row = (m.y - 2) / LINEDIST;
			col = (m.x - 2) / LINEDIST;
			if (ifInArray(row, col))//如果选中的这个在棋盘中
			{//首先要判断原来的状态，如果原来是选中珠子，则再选中珠子时要取消原来的高亮
				cout << "row=" << row << "col=" << col << endl;
				if (colorr[row][col] != 0)//这里有珠子
				{					
					if (oldrow == -1)//原来没选中珠子
					{						
						lightgrid(row, col, 1);//把珠子高亮	
						oldrow = row, oldcol = col;
					}
					else //原本有选中珠子
					{			
						if (midrow != -1)//原本有一个提醒，此时需要消除提醒
						{
							lightgrid(midrow, midcol);
							midrow = midcol = -1;
						}
						lightgrid(oldrow, oldcol);//取消原来的高亮
						lightgrid(row, col, 1);//把格子高亮	
						oldrow = row, oldcol = col;
					}
				}
				else//这是一个空格子
					if (oldrow == -1)//原来没选中珠子
						continue;
					else
					{//原来有选中，则此时要判断能否到达						
						//如果不能到达，要出现灰色框并输出提示
						if (findway(oldrow,oldcol,row,col)==false)
						{
							if(midrow!=-1)//原本已经选了一个不能选中的点
								lightgrid(midrow, midcol);
							lightgrid(row, col, 3);//当前格子变成灰色
							midrow = row, midcol = col;
						}
						//如果能到达，出现黄色框，然后移动
						else
						{
							if (midrow != -1)//原本有一个提醒，此时需要消除提醒
							{
								lightgrid(midrow, midcol);
								midrow = midcol = -1;
							}
							lightgrid(row, col, 1);//把格子高亮
							best = 100;
							findbest(oldrow, oldcol, row, col);							
							findPath(oldrow, oldcol, row, col);
							best+=100;//因为在上一个函数中改动了best，所以要复原
							for(int i= path.size()-1;i>=0;i--)
								lightgrid(path[i].row, path[i].col, 1);//把路径上所有格子高亮
							Sleep(800);//停留0.8秒
							pbead[oldrow][oldcol]->erase();//原来位置的珠子被擦除
							pbead[row][col] = new(nothrow) bead(row, col, pbead[oldrow][oldcol]->getcolor());//在指定位置生成指定颜色的新珠子
							if(pbead[row][col]==NULL)
							{
								cout << "申请空间失败" << endl;
								exit(-1);
							}
							pbead[row][col]->print();//画出珠子
							Sleep(600);//停留0.6秒
							for (int i = 0; i < path.size(); i++)
								lightgrid(path[i].row, path[i].col);//取消路径的高亮
							path.clear();//清空路径							
							colorr[row][col] = colorr[oldrow][oldcol];
							colorr[oldrow][oldcol] = 0;
							delete pbead[oldrow][oldcol];//销毁原来的珠子
							oldrow = oldcol = -1;
							//移动以后，判断能否进行消除
							if (delBead() == 0)//如果没有消除，要继续添加珠子
							{
								for (int i = 0; countt < max_num && i < 3; i++)
								{
									newBead();									
								}
								updateNum();
								if (countt == max_num)//如果达到了81个珠子，则游戏结束
								{
									Sleep(800);//停留0.8秒
									break;
								}
							}
						}
					}
			}
		}

	}
	//如果用户退出游戏，则进入“gameover”界面
	gameOver();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (colorr[i][j] != 0)//加这个判断是因为只有用new新建的对象才可以用delete释放
			{
				delete pbead[i][j];
			}
}
//游戏界面
gameScore bestScore[5];//记录前五名的成绩
void showRank()
{
	playmusic();
	for (int j = 0; j < 5; j++)//每次重新进入排行榜都要进行更新
	{
		bestScore[j].name = "虚位以待";
		bestScore[j].fen = 0;
	}
	ifstream in;
	in.open("game_record.txt", ios::app);//以附加方式读取
	streampos pos = in.tellg();
	//方法：选择排序，多次循环
	int fenn;//用于接收文件中的信息
	string namee;
	int dst_fen;//用于保存这一次要接收的内容
	string dst_name;
	bool noted;//判断这个名字是否有被记录过
	int i;
	for (i = 0; i < 5; i++)
	{
		dst_fen = -1;	
		while (!in.eof())//读取所有记录，找到最大值
		{
			in >> fenn >> namee;
			if (fenn > dst_fen)
			{
				noted = false;
				for (int j = 0; j < i; j++)
					if (bestScore[j].name == namee)//这个名字有被记录过
					{
						noted = true;
						break;
					}
				if (noted == false)//如果没被记录过，则存进来
				{
					dst_fen = fenn;
					dst_name = namee;
				}
			}
		}
		if (dst_fen != -1)//说明有找到
		{
			bestScore[i].fen = dst_fen;
			bestScore[i].name = dst_name;
			in.clear();//把指针重定位到开头
			in.seekg(pos);
		}
		else//没有找到
		{
			i++;
			break;
		}
	}
	in.close();
	
	BeginBatchDraw();
	setbkcolor(bkcolor);
	cleardevice();
	settextcolor(txtcolor);
	settextstyle(30, 0, _T("黑体"));
	LPCTSTR treturn = _T("回到首页");
	RECT rreturn;
	rreturn.left= SCREENWIDTH / 2 - textwidth(treturn) / 2;
	rreturn.right = rreturn.left + textwidth(treturn);
	rreturn.bottom = SCREENHIGH - edge;
	rreturn.top = rreturn.bottom - textheight(treturn);
	outtextxy(rreturn.left, rreturn.top, treturn);
	//setlinecolor(txtcolor);
	//rectangle(rreturn.left-5, rreturn.top - 5, rreturn.right + 5, rreturn.bottom + 5);
	//加入标题
	IMAGE trank;
	loadimage(&trank, _T("trank.png"));
	putimage(SCREENWIDTH / 2 - trank.getwidth() / 2, SCREENHIGH / 10*2.1 - trank.getheight() , &trank);
	settextcolor(lncolor);
	settextstyle(40, 0, _T("楷体"));
	TCHAR str[5][25];	
	int rank ;
	for (int j = 0; j < i&&j<5; j++)
	{
		string namee = bestScore[j].name;//把字符串转为char数组，再转为wchar_t
		char s[15];
		int k;
		for ( k = 0; k < namee.length()&&k<9; k++)
			s[k] = namee[k];
		s[k] = '\0';
		int n = MultiByteToWideChar(0, 0, s, -1, NULL, 0);
		wchar_t* wide = new wchar_t[n];
		MultiByteToWideChar(0, 0, s, -1, wide, n);

		if (j == 0)
			rank = 1;
		else
			if (bestScore[j].fen < bestScore[j - 1].fen)
				rank++;
		_stprintf(str[j], _T("第%d位%16d分"), rank, bestScore[j].fen);
		outtextxy( edge, SCREENHIGH / 10 * (2.4 + j*1.3 ), str[j]);
		outtextxy(SCREENWIDTH / 2 - textwidth(wide) / 2, SCREENHIGH / 10 * (2.4 + j * 1.3), wide);
	}
	EndBatchDraw();
	while (true) {
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton) {
			if (PointInRect(mess.x, mess.y, rreturn))
			{
				return;
			}
		}
	}
}
//展示排行榜
void gameSetting()
{
	playmusic();
	BeginBatchDraw();
	cleardevice();
	setbkcolor(bkcolor);
	settextcolor(txtcolor2);
	settextstyle(50, 0, _T("楷体"));
	LPCTSTR tplay = _T("打开/关闭游戏音效");
	LPCTSTR trank = _T("查看排行榜");
	
	//加入标题
	IMAGE setting;
	loadimage(&setting, _T("setting.png"));
	putimage(SCREENWIDTH / 2 - setting.getwidth() / 2, SCREENHIGH / 5 - setting.getheight() / 2, &setting);
	//加入选项
	RECT rplay,rrank;
	rplay.left = SCREENWIDTH / 2 - textwidth(tplay) / 2;
	rplay.right = rplay.left + textwidth(tplay);
	rplay.top = SCREENHIGH / 5 * 2.3;
	rplay.bottom = rplay.top + textheight(tplay);
	rrank.left= SCREENWIDTH / 2 - textwidth(trank) / 2;
	rrank.right = rrank.left + textwidth(trank);
	rrank.top= SCREENHIGH / 5 * 3.3;
	rrank.bottom = rrank.top + textheight(trank);
	outtextxy(rplay.left, rplay.top, tplay);
	outtextxy(rrank.left, rrank.top, trank);

	settextstyle(30, 0, _T("黑体"));
	LPCTSTR treturn = _T("返回");
	LPCTSTR yes = _T("音效：开 ");
	LPCTSTR no = _T("音效：关 ");
	RECT rreturn;
	rreturn.left = edge;
	rreturn.right = rreturn.left + textwidth(treturn);
	rreturn.bottom = SCREENHIGH - edge;
	rreturn.top = rreturn.bottom - textheight(treturn);
	outtextxy(rreturn.left, rreturn.top, treturn);
	if (if_music == true)//如果当前音效是开着的
		outtextxy(SCREENWIDTH - edge - textwidth(yes), rreturn.top, yes);
	else
		outtextxy(SCREENWIDTH - edge - textwidth(yes), rreturn.top, no);

	EndBatchDraw();
	while (true) {
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton) {
			if (PointInRect(mess.x, mess.y, rplay))
			{				
				if_music = !if_music;//取反
				playmusic();
				if (if_music == true)//如果当前音效是开着的
					outtextxy(SCREENWIDTH - edge - textwidth(yes), rreturn.top, yes);
				else
					outtextxy(SCREENWIDTH - edge - textwidth(yes), rreturn.top, no);				
			}
			else if (PointInRect(mess.x, mess.y, rrank))
			{
				showRank();
				return;
			}
			else if (PointInRect(mess.x, mess.y, rreturn))
			{
				return;
			}
		}
	}
}
//游戏设置界面
int main()
{
	srand((unsigned int)time(NULL));//构建随机数种子
	initgraph(SCREENWIDTH, SCREENHIGH, SHOWCONSOLE | NOCLOSE);//创建界面，显示控制台，禁用界面的关闭键
	while (1)
	{
		int i = begin();
		if (i == 1)//开始新游戏
		{
			newGame();
			continue;
		}
		else//进行游戏设置
		{
			gameSetting();
		}
	}
	return 0;
}