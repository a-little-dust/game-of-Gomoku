#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <graphics.h>    //easyxͼ�ο��ͷ�ļ�
#include <vector>
#include <fstream>//�ļ���
#include<mmsystem.h>//���ǰ�����ý���豸�ӿ�ͷ�ļ�
#pragma comment(lib,"winmm.lib")//���ؾ�̬��
using namespace std;

#define bkcolor RGB(255,245,247)//ͳһ���ý����ɫ
#define txtcolor RGB(69,137,148)//ͳһ����������ɫ
#define txtcolor2 RGB(22,133,169)//��һ��������ɫ
#define lncolor RGB(114,83,52)//�����ߵ���ɫ
#define LINEDIST  64  //������֮��ľ���
#define SCREENWIDTH 580//��Ļ��
#define SCREENHIGH   620//��Ļ��
#define R 30   //���ӵİ뾶��С
#define start_num 7   //��Ϸ��ʼʱ�������� 7
#define array_size 9 //���̴�С
#define max_num 81//���������ɵ����������
#define edge 40//��ԵҪԤ���Ŀ��

int colorr[9][9] = { 0 };   //��¼�������������ӵ���ɫ��ֵΪ0����û������
int score=0;//�÷�
int countt=0;//�����ϵ��������������ж���Ϸ�Ƿ����
bool if_music = true;
class bead {
public:
	void setcolor();//������������ö�Ӧ�������ɫ
	bead(int row, int col);//��ĳ��ĳ���������һ����ɫ������
	void print();//������
	void erase();//��������
	int getcolor();//����color��ţ�1��6��
	bead(int row, int col, int color_);//��ĳ��ĳ������ָ����ɫ������
private:
	int color = rand() % 6 + 1;        //���ӵ���ɫ
	int x;            //����Բ���ڽ����ϵĺ�����
	int y;            //����Բ���ڽ����ϵ�������
};
//����
bead* pbead[9][9];
struct grid {
	int row;
	int col;
};
//��������ɽṹ��
vector <grid> path ;//�����·��
int best = 100;//�����·����
bead::bead(int row, int col)
{
	y =2+ LINEDIST / 2 + row * LINEDIST;//����2�ǰ���߿�
	x =2+ LINEDIST / 2 + col * LINEDIST;
}
bead::bead(int row, int col, int color_)
{
	y = 2 + LINEDIST / 2 + row * LINEDIST;//����2�ǰ���߿�
	x = 2 + LINEDIST / 2 + col * LINEDIST;
	color = color_;
}
void bead::setcolor()
{
	switch (color) {
	case 1:
		setfillcolor(RGB(195,33,54));        //��
		break;
	case 2:
		setfillcolor(RGB(69,137,148));              //����
		break;
	case 3:
		setfillcolor(RGB(255,182,193));           //��
		break;
	case 4:
		setfillcolor(RGB(167,220,224));                  //ǳ��
		break;
	case 5:
		setfillcolor(RGB(117,36,35));         //��
		break;
	case 6:
		setfillcolor(RGB(185, 121, 177));        //��
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
	//�������ȡһ��λ�ã��ж�������û�б�ռ�ã�Ȼ����������
	while (true)//��������ѱ�ռ�ã���Ҫ������������
	{
		int bead_col = rand() % 9;
		int bead_row = rand() % 9;//�ֱ���������С���
		if (colorr[bead_row][bead_col] == 0)
		{//�������ԭ��û������
			pbead[bead_row][bead_col] = new(nothrow) bead(bead_row, bead_col);//��nothrow��ʽ���ɶ����������ʧ�ܻ�õ���ָ��
			if (pbead[bead_row][bead_col] == NULL) {
				cout << "����ռ�ʧ��" << endl;
				exit(-1);
			}			
			pbead[bead_row][bead_col]->print();//������
			colorr[bead_row][bead_col] = pbead[bead_row][bead_col]->getcolor();//��colorr���鸳ֵ
			break;
		}
	}
}
//�������һ������
struct gameScore
{
	int fen = 0;//��ʼ��Ϊ0�������ʱû����ҵĻ����а�ͻ���ʾ������0����λ�Դ���
	string name = "��λ�Դ�";
};
//�ѷ�������ɽṹ��
bool PointInRect(int x, int y, RECT& r)         
{
	return (x >= r.left && x <= r.right && y <= r.bottom && y >= r.top);
}
//�����ж�����Ƿ���ָ������
void playmusic(int alt = 0)
{
	if (if_music == false)//�����Ч�رգ��򲻲�������
		return;
	TCHAR szBuffer[100] = { 0 };
	TCHAR Buffer[100] = { 0 };
	switch (alt)
	{
	case 0://����ѡ�е�����
		wsprintf(szBuffer, _T("xuanzhong.mp3"));
		break;
	case 1://��������������
		wsprintf(szBuffer, _T("xiaochu.mp3"));
		break;
	case 2://�����޷�ѡ�е�����
		wsprintf(szBuffer, _T("fail.mp3"));
		break;
	case 3://������Ϸ����������
		wsprintf(szBuffer, _T("gameover.mp3"));
		break;
	}
	wsprintf(Buffer, _T("close \"%s\""), szBuffer);//���ԭ���Ѿ��ڲ��ţ�Ҫֹͣǰһ������
	mciSendString(Buffer, 0, 0, 0);
	wsprintf(Buffer, _T("open \"%s\""), szBuffer);
	mciSendString(Buffer, NULL, 0, NULL);//���豸
	wsprintf(Buffer, _T("play \"%s\""), szBuffer);
	mciSendString(Buffer, NULL, 0, NULL);//����
}
//��������
int begin()
{	
	//����Щ���ܾ�д��Щ��
	LPCTSTR tplay = _T("��ʼ��Ϸ");
	LPCTSTR texit = _T("�˳���Ϸ");
	LPCTSTR tother = _T("����");
	BeginBatchDraw();//�Ȼ�������ͳһչʾ����ֹ��ʾ���ȶ�
	setbkcolor(bkcolor);
	cleardevice();
	settextstyle(40, 0, _T("����"));//����Ϊ���߶ȣ���ȣ�����
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
	
	outtextxy(rplay.left, rplay.top, tplay);//������x��y���ַ�����
	outtextxy(rexit.left, rexit.top, texit);
	outtextxy(rother.left, rother.top, tother);

	//�������
	IMAGE title;
	loadimage(&title, _T("title.png"));
	putimage(SCREENWIDTH / 2 - title.getwidth() / 2, SCREENHIGH / 5-title.getheight()/2, &title);

	EndBatchDraw();
	playmusic();
	while (true) {//�����û���Ϣ������������ѡ���ת����Ӧ����
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton) //�������
		{
			if (PointInRect(mess.x, mess.y, rplay))//��ʼ��Ϸ
			{
				return 1;
			}
			else if (PointInRect(mess.x, mess.y, rexit))//�˳���Ϸ
			{
				cout << "\n��Ϸ�ѽ�����\n";
				exit(0);//�˳����򲢷���0
			}
			else if (PointInRect(mess.x, mess.y, rother))//������
			{				
				return 2;
			}
		}
	}
}
//��ʼ����
void printline()            
{
	int i;
	setlinestyle(PS_SOLID, 4);//ʵ�ߣ��߿�Ϊ4
	setlinecolor(lncolor);
	for (i = 0; i <= 9; i++) {//һ��Ҫ��ʮ������
		line(2+i*LINEDIST, 0, 2 + i * LINEDIST, SCREENWIDTH-2);
	}
	//
	for (i = 0; i <= 9; i++) {//ʮ������
		line(0,2 + i * LINEDIST,SCREENWIDTH,2 + i * LINEDIST);
	}
}
//�����̵�����
void lightgrid(int row,int col,int c=0)
{
	setlinestyle(PS_SOLID, 4);//ʵ�ߣ��߿�Ϊ4
	switch (c)
	{
	case 0://��ԭ
		setlinecolor(lncolor);
		rectangle(2 + col * LINEDIST, 2 + row * LINEDIST, 2 + (col + 1) * LINEDIST, 2 + (row + 1) * LINEDIST);//��һ���б߿�ľ���
		break;
	case 1://ѡ��
		playmusic();
		setlinecolor(RGB(244, 208, 0));
		rectangle(2 + col * LINEDIST, 2 + row * LINEDIST, 2 + (col + 1) * LINEDIST, 2 + (row + 1) * LINEDIST);
		break;
	case 2://����
		playmusic(1);
		setlinecolor(RGB(227, 249, 253));//�°�ɫ
		rectangle(2 + col * LINEDIST, 2 + row * LINEDIST, 2 + (col + 1) * LINEDIST, 2 + (row + 1) * LINEDIST);
		break;
	case 3://���ܵ���
		playmusic(2);
		setlinecolor(RGB(204,202,169));//��ɫ
		rectangle(2 + col * LINEDIST, 2 + row * LINEDIST, 2 + (col + 1) * LINEDIST, 2 + (row + 1) * LINEDIST);
		break;
	}
}
//�޸�������ɫ
void outputScore()
{
	settextstyle(20, 0, _T("����"));
	settextcolor(txtcolor);
	//�����ַ���
	TCHAR scr[15];//������÷�
	_stprintf(scr, _T("�÷� ��%d"), score);//������תΪ�ַ������ٴ���ַ�����
	//���
	int top = SCREENWIDTH + (SCREENHIGH - SCREENWIDTH) / 2 - textheight(scr) / 2;
	outtextxy(SCREENWIDTH - edge - textwidth(scr),top, scr);
}
//ʵʱ��ʾ����
void updateNum()//��û�и�ʵʱ��ʾ��������һ����Ϊ�����������˵÷�δ���иı�
{
	settextstyle(20, 0, _T("����"));
	settextcolor(txtcolor);
	//�����ַ���
	TCHAR num[15];//����������
	_stprintf(num, _T("������ ��%d"), countt);
	//���
	int top = SCREENWIDTH + (SCREENHIGH - SCREENWIDTH) / 2 - textheight(num) / 2;
	outtextxy(SCREENWIDTH / 2 - textwidth(num) / 2, top, num);
}
//ʵʱ��ʾ������
bool ifInArray(int i, int j)
{
	if (i >= 0 && i < array_size)
		if (j >= 0 && j < array_size)
			return true;
	return false;
}
//�ж��Ƿ�����Խ��
void noteRepeat(bool flag[][9])
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (colorr[i][j] == 0)
				continue;//�������û�����ӣ����Բ�����
			int a1 = i;
			int b1 = j;//��abȥ��̽��a1b1��¼Ҫ�����ĵ�
			int ccount = 0;//��¼���ٸ���������һ������
			//��������
			for (int a = i, b = j; ifInArray(a + 1, b) && colorr[a][b] == colorr[a + 1][b]; a++)
			{
				ccount++;
			}
			if (ccount >= 4)//�����4����ͬ��˵����5������һ��
			{
				for (a1, b1; a1 <= i + ccount; a1++)
				{
					flag[a1][b1] = 1;
					lightgrid(a1, b1, 2);//�����λ�ø���
				}
			}
			//��������
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
			//����б������
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
			//����б������
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
//��¼��Щ�ط���Ҫ�޸�
int delBead()
{
	bool flag[9][9];//��¼Ҫ����������
	noteRepeat(flag);
	int nnum = 0;//��¼��ο�����ȥ���ٸ����ӣ�Ȼ��תΪ����
	for(int i=0;i<9;i++)
		for (int j = 0; j < 9; j++)
		{
			if (flag[i][j] == 1)
			{
				if (nnum == 0)//���ֿ�������ʱ
				{
					playmusic(1);
					Sleep(1500);//�ö���ͣ��1.5����				
				}
				nnum++;
				pbead[i][j]->erase();//��ȥ����
				lightgrid(i, j);//�����߸�ԭ
				delete pbead[i][j];//���ٶ���
				colorr[i][j] = 0;
			}
		}	
	if (nnum > 0)
	{
		updateNum();
		score += nnum * 2;//�޸ĵ÷�
		outputScore();
		return 1;
	}
	else
		return 0;//��ʾû����������Ҫ�����������
}
//����������1��0�����Ƿ��з�������
void find_in_way(int row, int col, bool way[][array_size])
{
	way[row][col] = true;//���ظ�����ͬһ����
	int dirs[4][2] = { {-1,0},{1,0},{0,1},{0,-1} };
	for (int d = 0; d < 4; d++)
	{
		int next_row = row + dirs[d][0];
		int next_col = col + dirs[d][1];
		if (ifInArray(next_row, next_col))//��ֵ�Ϸ�
			if (colorr[next_row][next_col] == 0 && way[next_row][next_col] == false)//������
			{
				find_in_way(next_row, next_col, way);
			}
	}
}
//��ÿ�����Ծ����ĵ㣬����������
bool findway(int row, int col, int dstrow, int dstcol)
{
	bool way[array_size][array_size]={0};
	find_in_way(row, col, way);
	return way[dstrow][dstcol];
}
//�ж��ܷ��ҵ�һ��·�������ַ����ܱ�֤ÿ���ط�ֻ����һ�Σ�Ч�ʸ�
void findbest(int row, int col, int dstrow, int dstcol,int cost=-1)
{
	cost++;
	if (cost >= best)//�����ǰcost�Ѿ����˵����һ���������·��
	{
		return;
	}	
	if (row == dstrow && col == dstcol)//��ʾ�����յ�
	{
		best = cost;//�޸����·������
		return;
	}	
	int recolor = colorr[row][col];
	colorr[row][col] = -1;//��ֹ�߻�ͷ·
	int dirs[4][2] = { {-1,0},{1,0},{0,1},{0,-1} };
	for (int d = 0; d < 4; d++)
	{
		int next_row = row + dirs[d][0];
		int next_col = col + dirs[d][1];
		if(ifInArray(next_row,next_col))//��ֵ�Ϸ�
			if (colorr[next_row][next_col] == 0)//������
			{
				findbest(next_row, next_col, dstrow, dstcol, cost);
			}
	}
	colorr[row][col] =recolor;
}
//Ѱ�����·����
int findPath(int row, int col, int dstrow, int dstcol, int cost=-1)
{
	cost++;
	grid curr{ row,col };
	if (row == dstrow && col == dstcol&&cost==best)//��ʾ�����յ����������·��
	{
		path.push_back( curr);//�������
		best=best-100;//Ϊ��ֹ�ҵ�������С·�����ҵ�һ���Ժ�best�޸ģ������ٻ�����
		return 0;
	}
	if (cost >= best)//�����ǰû���յ���cost�Ѿ����˵����һ���������·��
	{
		return 1;
	}	
	int recolor = colorr[row][col];
	colorr[row][col] = -1;//��ֹ�߻�ͷ·
	int dirs[4][2] = { {-1,0},{1,0},{0,1},{0,-1} };
	int test = 1;
	for (int d = 0; d < 4; d++)
	{
		int next_row = row + dirs[d][0];
		int next_col = col + dirs[d][1];
		if (ifInArray(next_row, next_col))//��ֵ�Ϸ�
			if (colorr[next_row][next_col] == 0)//������
			{//ֻҪ��һ�������·�����ͻ᷵��0��test�ͻ���0
				test*= findPath(next_row, next_col, dstrow, dstcol, cost);
			}
	}
	colorr[row][col] = recolor;
	if (test == 0)//˵������������ҵ������·��
	{
		path.push_back(curr);//�������
	}
	return test;
}
//�ҵ���̵�·�����������������
int getRank()
{
	int rank = 1;//�ӵ�һ����ʼ������
	ifstream in;
	in.open("game_record.txt", ios::in);//��ȡ�ļ�
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
//��ȡ�ļ�����õ�ǰ����
void gameOver()
{	
	settextstyle(40, 0, _T("����"));//����Ϊ���߶ȣ���ȣ�����
	settextcolor(txtcolor2);
	//����Щ���ܾ�д��Щ��
	LPCTSTR tscore = _T("�������ĵ÷�Ϊ��");
	TCHAR scr[5];//������÷�
	_stprintf(scr, _T("%d"), score);
	LPCTSTR trank = _T("�����г����У����γɼ�����Ϊ��");
	int rank = getRank();
	TCHAR rnk[5];
	_stprintf(rnk, _T("%d"), rank);
	LPCTSTR choose = _T("�Ƿ񱣴汾�μ�¼��");
	LPCTSTR remind = _T("���ڿ���̨�����������û���");
	LPCTSTR tyes = _T("  ��  ");
	LPCTSTR tno = _T("  ��  ");
    BeginBatchDraw();//�Ȼ�������ͳһչʾ����ֹ��ʾ���ȶ�
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

	//�������
	IMAGE ending;
	loadimage(&ending, _T("ending.png"));
	putimage(SCREENWIDTH / 2 - ending.getwidth() / 2, SCREENHIGH / 5 - ending.getheight() / 2, &ending);

	settextstyle(30, 0, _T("����"));//����Ϊ���߶ȣ���ȣ�����
	outtextxy(SCREENWIDTH / 2 - textwidth(tscore)/2-textwidth(scr)/2, SCREENHIGH / 10 * 3.6, tscore);
	outtextxy(SCREENWIDTH / 2 + textwidth(tscore) / 2 - textwidth(scr) / 2, SCREENHIGH / 10 * 3.6, scr);
	outtextxy(SCREENWIDTH / 2 - textwidth(trank) / 2 - textwidth(rnk) / 2, SCREENHIGH / 10 * 5.2, trank);
	outtextxy(SCREENWIDTH / 2 + textwidth(trank) / 2 - textwidth(rnk) / 2, SCREENHIGH / 10 * 5.2, rnk);
	settextstyle(40, 0, _T("����"));
	outtextxy(SCREENWIDTH / 2 - textwidth(choose) / 2, SCREENHIGH / 10 * 6.8, choose);

	EndBatchDraw();
	playmusic(3);
	while (true) {//�����û���Ϣ������������ѡ���ת����Ӧ����
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton) //�������
		{
			if (PointInRect(mess.x, mess.y, ryes))//����û�Ҫ�����¼
			{
				playmusic();
				setfillcolor(bkcolor);
				solidrectangle(ryes.left-2,ryes.top-2, ryes.right+2, ryes.bottom+2);
				solidrectangle(rno.left-2,rno.top-2, rno. right+2, rno.bottom+2);
				outtextxy(SCREENWIDTH / 2 - textwidth(remind) / 2, SCREENHIGH / 10 * 8.4, remind);
				string s;
				cout << "����������û�����������10���ַ�����";
				cin >> s;
				ofstream out;//������ļ���
				out.open("game_record.txt", ios::app);
				out  << score << " " << s<<endl;//���û���Ϣ��ӵ��ļ�����
				out.close();
				return;
			}
			else if (PointInRect(mess.x, mess.y, rno))//����û��������¼
			{
				return;
			}
		}
	}
}
//��Ϸ����
void newGame() {	
	playmusic();
	//��ʼ��������
	score = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			colorr[i][j] = 0;//�տ�ʼʱû�����ӣ�����û����ɫ
	countt = 0;
	//������
	setbkcolor(bkcolor);
	cleardevice();
	printline();
	settextstyle(20, 0, _T("����"));
	settextcolor(txtcolor);
	//�����˳�����
	LPCTSTR texit = _T("������Ϸ");
	RECT rexit;
	rexit.left = edge;
	rexit.right = rexit.left + textwidth(texit);
	rexit.top = SCREENWIDTH + (SCREENHIGH - SCREENWIDTH) / 2 - textheight(texit) / 2;
	rexit.bottom = rexit.top + textheight(texit);
	outtextxy(rexit.left, rexit.top, texit);
	//ʵʱ��ʾ����
	outputScore();
	//�������ϵ����ӡ���ʼʱ�̣�������Ϊstart_num
	for (int i = 0; i < start_num; i++)
		newBead();
	updateNum();
	//�����������Ӻ��ж��ܷ�������Ȼ���������
	delBead();
	//��������ʽ��ʼ��Ϸ
	int oldrow = -1, oldcol = -1;//������һ��ѡ�е�����
	int midrow = -1, midcol = -1;//������һ��ѡ�еĿո���
	while (1)//while��������ػ�ȡ��Ϣ
	{	
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);      //��ȡ�����Ϣ
		if (mess.lbutton)
		{               //�������������
			MOUSEMSG m = GetMouseMsg();
			if (PointInRect(m.x, m.y, rexit))
			{
				break;
			}     //�ж�����Ƿ�����Ϸ�����е���ˡ��˳���Ϸ����ť
			int row, col;
			row = (m.y - 2) / LINEDIST;
			col = (m.x - 2) / LINEDIST;
			if (ifInArray(row, col))//���ѡ�е������������
			{//����Ҫ�ж�ԭ����״̬�����ԭ����ѡ�����ӣ�����ѡ������ʱҪȡ��ԭ���ĸ���
				cout << "row=" << row << "col=" << col << endl;
				if (colorr[row][col] != 0)//����������
				{					
					if (oldrow == -1)//ԭ��ûѡ������
					{						
						lightgrid(row, col, 1);//�����Ӹ���	
						oldrow = row, oldcol = col;
					}
					else //ԭ����ѡ������
					{			
						if (midrow != -1)//ԭ����һ�����ѣ���ʱ��Ҫ��������
						{
							lightgrid(midrow, midcol);
							midrow = midcol = -1;
						}
						lightgrid(oldrow, oldcol);//ȡ��ԭ���ĸ���
						lightgrid(row, col, 1);//�Ѹ��Ӹ���	
						oldrow = row, oldcol = col;
					}
				}
				else//����һ���ո���
					if (oldrow == -1)//ԭ��ûѡ������
						continue;
					else
					{//ԭ����ѡ�У����ʱҪ�ж��ܷ񵽴�						
						//������ܵ��Ҫ���ֻ�ɫ�������ʾ
						if (findway(oldrow,oldcol,row,col)==false)
						{
							if(midrow!=-1)//ԭ���Ѿ�ѡ��һ������ѡ�еĵ�
								lightgrid(midrow, midcol);
							lightgrid(row, col, 3);//��ǰ���ӱ�ɻ�ɫ
							midrow = row, midcol = col;
						}
						//����ܵ�����ֻ�ɫ��Ȼ���ƶ�
						else
						{
							if (midrow != -1)//ԭ����һ�����ѣ���ʱ��Ҫ��������
							{
								lightgrid(midrow, midcol);
								midrow = midcol = -1;
							}
							lightgrid(row, col, 1);//�Ѹ��Ӹ���
							best = 100;
							findbest(oldrow, oldcol, row, col);							
							findPath(oldrow, oldcol, row, col);
							best+=100;//��Ϊ����һ�������иĶ���best������Ҫ��ԭ
							for(int i= path.size()-1;i>=0;i--)
								lightgrid(path[i].row, path[i].col, 1);//��·�������и��Ӹ���
							Sleep(800);//ͣ��0.8��
							pbead[oldrow][oldcol]->erase();//ԭ��λ�õ����ӱ�����
							pbead[row][col] = new(nothrow) bead(row, col, pbead[oldrow][oldcol]->getcolor());//��ָ��λ������ָ����ɫ��������
							if(pbead[row][col]==NULL)
							{
								cout << "����ռ�ʧ��" << endl;
								exit(-1);
							}
							pbead[row][col]->print();//��������
							Sleep(600);//ͣ��0.6��
							for (int i = 0; i < path.size(); i++)
								lightgrid(path[i].row, path[i].col);//ȡ��·���ĸ���
							path.clear();//���·��							
							colorr[row][col] = colorr[oldrow][oldcol];
							colorr[oldrow][oldcol] = 0;
							delete pbead[oldrow][oldcol];//����ԭ��������
							oldrow = oldcol = -1;
							//�ƶ��Ժ��ж��ܷ��������
							if (delBead() == 0)//���û��������Ҫ�����������
							{
								for (int i = 0; countt < max_num && i < 3; i++)
								{
									newBead();									
								}
								updateNum();
								if (countt == max_num)//����ﵽ��81�����ӣ�����Ϸ����
								{
									Sleep(800);//ͣ��0.8��
									break;
								}
							}
						}
					}
			}
		}

	}
	//����û��˳���Ϸ������롰gameover������
	gameOver();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (colorr[i][j] != 0)//������ж�����Ϊֻ����new�½��Ķ���ſ�����delete�ͷ�
			{
				delete pbead[i][j];
			}
}
//��Ϸ����
gameScore bestScore[5];//��¼ǰ�����ĳɼ�
void showRank()
{
	playmusic();
	for (int j = 0; j < 5; j++)//ÿ�����½������а�Ҫ���и���
	{
		bestScore[j].name = "��λ�Դ�";
		bestScore[j].fen = 0;
	}
	ifstream in;
	in.open("game_record.txt", ios::app);//�Ը��ӷ�ʽ��ȡ
	streampos pos = in.tellg();
	//������ѡ�����򣬶��ѭ��
	int fenn;//���ڽ����ļ��е���Ϣ
	string namee;
	int dst_fen;//���ڱ�����һ��Ҫ���յ�����
	string dst_name;
	bool noted;//�ж���������Ƿ��б���¼��
	int i;
	for (i = 0; i < 5; i++)
	{
		dst_fen = -1;	
		while (!in.eof())//��ȡ���м�¼���ҵ����ֵ
		{
			in >> fenn >> namee;
			if (fenn > dst_fen)
			{
				noted = false;
				for (int j = 0; j < i; j++)
					if (bestScore[j].name == namee)//��������б���¼��
					{
						noted = true;
						break;
					}
				if (noted == false)//���û����¼����������
				{
					dst_fen = fenn;
					dst_name = namee;
				}
			}
		}
		if (dst_fen != -1)//˵�����ҵ�
		{
			bestScore[i].fen = dst_fen;
			bestScore[i].name = dst_name;
			in.clear();//��ָ���ض�λ����ͷ
			in.seekg(pos);
		}
		else//û���ҵ�
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
	settextstyle(30, 0, _T("����"));
	LPCTSTR treturn = _T("�ص���ҳ");
	RECT rreturn;
	rreturn.left= SCREENWIDTH / 2 - textwidth(treturn) / 2;
	rreturn.right = rreturn.left + textwidth(treturn);
	rreturn.bottom = SCREENHIGH - edge;
	rreturn.top = rreturn.bottom - textheight(treturn);
	outtextxy(rreturn.left, rreturn.top, treturn);
	//setlinecolor(txtcolor);
	//rectangle(rreturn.left-5, rreturn.top - 5, rreturn.right + 5, rreturn.bottom + 5);
	//�������
	IMAGE trank;
	loadimage(&trank, _T("trank.png"));
	putimage(SCREENWIDTH / 2 - trank.getwidth() / 2, SCREENHIGH / 10*2.1 - trank.getheight() , &trank);
	settextcolor(lncolor);
	settextstyle(40, 0, _T("����"));
	TCHAR str[5][25];	
	int rank ;
	for (int j = 0; j < i&&j<5; j++)
	{
		string namee = bestScore[j].name;//���ַ���תΪchar���飬��תΪwchar_t
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
		_stprintf(str[j], _T("��%dλ%16d��"), rank, bestScore[j].fen);
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
//չʾ���а�
void gameSetting()
{
	playmusic();
	BeginBatchDraw();
	cleardevice();
	setbkcolor(bkcolor);
	settextcolor(txtcolor2);
	settextstyle(50, 0, _T("����"));
	LPCTSTR tplay = _T("��/�ر���Ϸ��Ч");
	LPCTSTR trank = _T("�鿴���а�");
	
	//�������
	IMAGE setting;
	loadimage(&setting, _T("setting.png"));
	putimage(SCREENWIDTH / 2 - setting.getwidth() / 2, SCREENHIGH / 5 - setting.getheight() / 2, &setting);
	//����ѡ��
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

	settextstyle(30, 0, _T("����"));
	LPCTSTR treturn = _T("����");
	LPCTSTR yes = _T("��Ч���� ");
	LPCTSTR no = _T("��Ч���� ");
	RECT rreturn;
	rreturn.left = edge;
	rreturn.right = rreturn.left + textwidth(treturn);
	rreturn.bottom = SCREENHIGH - edge;
	rreturn.top = rreturn.bottom - textheight(treturn);
	outtextxy(rreturn.left, rreturn.top, treturn);
	if (if_music == true)//�����ǰ��Ч�ǿ��ŵ�
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
				if_music = !if_music;//ȡ��
				playmusic();
				if (if_music == true)//�����ǰ��Ч�ǿ��ŵ�
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
//��Ϸ���ý���
int main()
{
	srand((unsigned int)time(NULL));//�������������
	initgraph(SCREENWIDTH, SCREENHIGH, SHOWCONSOLE | NOCLOSE);//�������棬��ʾ����̨�����ý���Ĺرռ�
	while (1)
	{
		int i = begin();
		if (i == 1)//��ʼ����Ϸ
		{
			newGame();
			continue;
		}
		else//������Ϸ����
		{
			gameSetting();
		}
	}
	return 0;
}