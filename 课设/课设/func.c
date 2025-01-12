
#include "F2.h"
int f_time = 100;
int f_time2 = 50;
int f_time3 = 50;

char MT[me_num+1][40] = {
	"[A] 开始游戏     ",
	"[B] 开始双人游戏",
	"[C] 功能1：排行榜 ",
	"[D] 功能2：查找   ",
	"[E] 功能3：保存   ",
	"[F] 功能4：说明   ",
	"[G] 功能5：删除   ",
	"[H] 功能6：游戏设置",
	"[I] Exit 退出程序"
};
char GSZ[8][40] = {
	"<1> 障碍物更新步伐\n",
	"<2> 食物上限\n",
	"<3> 食物更新步伐\n",
	"<4> 背景相对颜色\n",	
	"<5> 背景音乐\n",
	"<6> 地图扩大\n",
	"<7> 血量上限\n",
	"<8> 基础速度等级\n"
};

struct xz_sel {
	int count_j;//食物更新计数
	int count_zaw_j;//障碍物更新计数+
	int xl;//血量上限
	int food_sum_up;//食物上限
	int music_cur_num;
	int gamemenu_xy[2][2];
};

struct user {
	int ser_num;
	char name[20];
	char other[100];
	struct user* next;
};
struct movexy {
	int x;
	int y;
	struct movexy* next;
};
struct foodxy {
	int x;
	int y;
	int mark;
	int spe;
	struct foodxy* next;
};

struct zaw {
	int x;
	int y;
	struct zaw* next;
};

void cs_menu(int hhi) {//菜单显示函数
	int i;

	COORD pos = { 43,6 };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("========欢迎来到XEL程序========");
	for (i = 0; i < me_num+1; i++) {
		pos.Y += 3;
		SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
		if (i == hhi)printf("     %s   <<==", MT[i]);
		else { printf("     %s", MT[i]); }
	}
	pos.Y += 3;
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("===============================");
	pos.Y += 2;
}

struct user* input(int mark,int cd)//游戏结束时加入排行版
{
	char str_t[100];
	struct user* new = (struct user*)malloc(sizeof(struct user));
	printf("\n\n请输入\n\n名字： ");
	fgets(new->name, 20, stdin);
	if ('\n' == new->name[0])
	{
		return NULL;
	}
	new->name[strcspn(new->name, "\n")] = '\0';
	new->ser_num = mark;
	printf("\n\n请输入获奖感言Ψ(￣∀￣)Ψ\n\n");
	fgets(str_t, 200, stdin);
	str_t[strcspn(str_t, "\n")] = '\0';
	sprintf(new->other,"本次长度为%d!!!!获奖感言:%s",cd,str_t);
	printf("\n");
	new->next = NULL;

	return new;
}

struct user* lb_add(struct user* head, struct user* cur_new) {//插入函数
	struct user* cur_1 = NULL;
	struct user* cur = head;
	while (cur != NULL && (cur->ser_num) > (cur_new->ser_num))
	{
		cur_1 = cur;
		cur = cur->next;
	}
	if (cur == NULL) {
		if (cur_1 == NULL) {
			head = cur_new;
		}
		else {
			cur_1->next = cur_new;
		}
	}
	else {
		if (cur_1 == NULL)
		{
			cur_new->next = head;
			head = cur_new;
		}
		else {
			cur_1->next = cur_new;
			cur_new->next = cur;
		}
	}
	return head;
}

void lb_print(struct user* head) {
	struct user* cur = head;
	if (head == NULL) { return; }
	int count = 0;
	while (cur != NULL)
	{
		count++;
		printf("\n》》第%d名《《\n\n--账号   %s\n\n--得分   %d\n\n--other     %s\n\n\n",count, cur->name, cur->ser_num, cur->other);
		cur = cur->next;
	}
}

struct user* lb_index(struct user* head, int num, char* name) {
	struct user* cur;
	cur = head;
	while (cur != NULL) {
		if ((cur->ser_num == num) || (!(strcmp(cur->name, name))))
		{
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

void lb_w(struct user* head, char* dz1, char* dz2, char* dz3)//写入
{
	if (head == NULL)return;
	FILE* a = fopen(dz1, "w+");
	FILE* b = fopen(dz2, "wb+");
	FILE* c = fopen(dz3, "w+");
	struct user* temp_w = head;

	if (a == NULL || b == NULL || c == NULL)
	{
		return;
	}

	do {

		temp_w->other[strlen(temp_w->other) + 1] = '\n';
		temp_w->name[strlen(temp_w->name) + 1] = '\n';

		fwrite(&(temp_w->name), 1, strlen(temp_w->name) + 2, a);
		fwrite(&(temp_w->ser_num), sizeof(int), 1, b);
		fwrite(&(temp_w->other), 1, strlen(temp_w->other) + 2, c);

		temp_w = temp_w->next;//指针指向下一个

	} while (temp_w != NULL);

	fclose(a);
	fclose(b);
	fclose(c);
}

void lb_w2(struct user* head, char* dz1, char* dz2, char* dz3) {
	if (head == NULL)return;
	FILE* a = fopen(dz1, "a");
	FILE* b = fopen(dz2, "ab");
	FILE* c = fopen(dz3, "a");
	struct user* temp_w2 = head;

	if (a == NULL || b == NULL || c == NULL) {
		// 处理文件打开失败情况
		return;
	}

	do {
		temp_w2->name[strlen(temp_w2->name) + 1] = '\n';
		temp_w2->other[strlen(temp_w2->other) + 1] = '\n';

		fwrite(&(temp_w2->name), 1, strlen(temp_w2->name) + 2, a);
		fwrite(&(temp_w2->ser_num), sizeof(int), 1, b);
		fwrite(&(temp_w2->other), 1, strlen(temp_w2->other) + 2, c);

		temp_w2 = temp_w2->next;
	} while (temp_w2 != NULL);

	fclose(a);
	fclose(b);
	fclose(c);
}

struct user* lb_r(struct user* head, char* dz1, char* dz2, char* dz3)//用来读取文件的排行榜
{
	FILE* name_r = fopen(dz1, "r");
	FILE* ser_num_r = fopen(dz2, "rb");
	FILE* other_r = fopen(dz3, "r");

	char name_r2[20];
	int num_r;
	char other_r2[100];

	while (fscanf(name_r, "%s", name_r2) == 1 && fread(&num_r, sizeof(int), 1, ser_num_r) == 1 && fscanf(other_r, "%s", other_r2) == 1)
	{
		struct user* temp = (struct user*)malloc(sizeof(struct user));

		temp->next = NULL;
		strcpy(temp->name, name_r2);
		temp->ser_num = num_r;
		strcpy(temp->other, other_r2);

		head = lb_add(head, temp);
	}

	fclose(name_r);
	fclose(ser_num_r);
	fclose(other_r);

	return head;
}

void lb_pop(struct user* p, struct user* p0)
{
	if (p == NULL)return;
	if (p->next != NULL)
	{
		lb_pop(p->next, p);
	}
	if (p != p0) {
		p0->next = NULL;
		free(p);
	}
	else { free(p0); }
}

void game_menu(int sx0,int sx1,int zy0,int zy1,int colars[3])//
{
	char temp_col_char[100];
	sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", (colars[0] + 100) % 256, (200 + colars[1]) % 256, (300 + colars[2]) % 256);
	printf("%s", temp_col_char);

	COORD pos = { zy0,sx0 };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	for (int i = sx0; i < sx1; i++) {
		for (int j = zy0; j < zy1; j++) {
			if (i == sx0 || i == sx1-1) {
				printf("▓");
			}
			else {
				if (j == zy0 || j == zy1-1)
				{
					printf("▓");
				}
				else {
					sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", colars[0], colars[1], colars[2]);
					printf("%s", temp_col_char);
					printf(" ");
					sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", (colars[0] + 100) % 256, (200 + colars[1]) % 256, (300 + colars[2]) % 256);
					printf("%s", temp_col_char);
				}
			}
		}
		pos.Y += 1;
		SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	}

	return;
}

void move_print(struct movexy*head,int colars[3]) {


	char temp_col_char[100];
	sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", colars[0], colars[1], colars[2]);
	printf("%s", temp_col_char);

	COORD pos = { head->x,head->y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("★");

	if (head->next != NULL) {
		COORD pos = { head->next->x,head->next->y };
		SetConsoleCursorPosition(hOut, pos);
		char c_s[100];
		sprintf(c_s, "\033[38;2;%d;%d;%dm█\033[0m", (colars[0]+106)%256, (colars[2]+180)%256, (colars[1] + 160)%256);//蛇身
		if (head->x - head->next->x == 0)
		{
			printf("%s",c_s);
		}
		else {
			printf("%s",c_s);
		}
	}
}

struct movexy* move_kz(struct movexy* head,int colars[3])
{
	int x = head->x;
	int y = head->y;
	struct movexy* temp_xy = (struct move_xy*)malloc(sizeof(struct movexy));
	temp_xy->next = head;
	if (kbhit())
	{
		if (head->next == NULL) {
			if (GetAsyncKeyState(VK_UP)) {//检查是上下左右还是不动
				y--;
			}
			else if (GetAsyncKeyState(VK_DOWN)) {
				y++;
			}
			else if (GetAsyncKeyState(VK_LEFT)) {
				x--;
			}
			else if (GetAsyncKeyState(VK_RIGHT)) {
				x++;
			}
		}
		else {
			if (GetAsyncKeyState(VK_UP)&& head->y-head->next->y != 1) {//检查是上下左右还是不动
				if (head->y - head->next->y == -1) {
					f_time2 = f_time2 - 50;
				}
				y--;
			}
			else if (GetAsyncKeyState(VK_DOWN) && head->y - head->next->y != -1) {
				if (head->y - head->next->y == 1) {
					f_time2 = f_time2 - 50;
				}
				y++;
			}
			else if (GetAsyncKeyState(VK_LEFT) && head->x - head->next->x != 1) {
				if (head->x - head->next->x == -1) {
					f_time2 = f_time2 - 50;
				}
				x--;
			}
			else if (GetAsyncKeyState(VK_RIGHT) && head->x - head->next->x != -1) {
				if (head->x - head->next->x == 1) {
					f_time2 = f_time2 - 50;
				}
				x++;
			}
			else {
				x = x + head->x - head->next->x;
				y = y + head->y - head->next->y;
			}
		}

		Sleep(f_time2);
		f_time2 = f_time3;
		temp_xy->x = x;
		temp_xy->y = y;
		//打印蛇身体
		move_print(temp_xy,colars);
		return temp_xy;
	}
	return head;
}

void move_tail(struct movexy *tail,int colars[3]) {


	char temp_col_char[100];
	sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", colars[0], colars[1], colars[2]);
	printf("%s", temp_col_char);

	while (tail->next->next != NULL)
	{
		tail = tail->next;
	}
	COORD pos = {tail->next->x,tail->next->y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf(" ");
	struct movexy* temp = tail->next;
	tail->next = NULL;
	free(temp);
}

//以下为打印食物函数，记得待会在主函数补充相关代码
struct foodxy* food_spe(int x, int y, int spe, struct foodxy* head_f,int colars[3])//注意，这里的spe最好是0~5的整数
{

	char temp_col_char[100];
	sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", colars[0], colars[1], colars[2]);
	printf("%s", temp_col_char);

	int nums[6] = { -5,-3,0,1,5,10 };
	int nums2[6] = { -2,0,-1,1,1,2 };
	int colour[6][3] = { {216,21,21},{101,21,126},{68,268,105},{92,207,234},{146,92,234}, { 232,232,64 } };//颜色，大红，大紫，绿，蓝，紫，金
	int mark_t = nums[spe % 6];
	struct foodxy* new_f = malloc(sizeof(struct foodxy));
	new_f->x = x;
	new_f->y = y;
	new_f->mark = mark_t;
	new_f->spe = nums2[spe];
	new_f->next = head_f;

	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	char c_s[40];
	sprintf(c_s, "\033[38;2;%d;%d;%dm●\033[0m", (colour[spe][0]+236)%256, (colour[spe][1]+236)%256, (236+colour[spe][2])%256);//根据不同颜色打印食物位置
	printf("%s",c_s);//打印
	return new_f;//返回事物链表节点，更新节点
} 

int num_sj(int l, int r)//生成单个随机数
{
	rand((unsigned int)time(NULL));
	int res = l + rand() % (r - l + 1);
	return res;
}

int zq(int x,int y,struct movexy*body,int fanwei[2][2])
{
	int x0, x1, y0, y1;
	x0 = fanwei[0][0];//上
	x1 = fanwei[1][0]-1;//下
	y0 = fanwei[0][1];//左
	y1 = fanwei[1][1]-1;//右
	struct movexy* temp = body;
	if (x <= x0 || x >= x1 || y <= y0 || y >= y1)
	{
		return -1;
	}
	while (temp != NULL)
	{
		if (body->next != NULL) {
			if (body->next->next != NULL && temp->x == x && temp->y == y) {
				return -1;
			}
		}
		temp = temp->next;
	}
	return 0;
}

void gameover(int mark,int cd) {
	char temp[500];
	sprintf(temp, "\033[38;2;92;207;234m                                                本次得分%d!!  本次长度%d!!             \n\n\033[0m",mark,cd);
	COORD pos = { 7,13 };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("\033[38;2;196;0;0m                     =======================游戏结束，都怪你啦=====================\n\n\033[0m");
	printf("%s",temp);

	return;
}

void sz_menu(int num_sel)
{
	printf("\n《------------设置界面------------》\n\n");
	for (int i = 0; i < 8; i++)
	{
		if (num_sel == i) {
			printf(">>>>%s\n", GSZ[i]);
		}
		else {
			printf("%s\n", GSZ[i]);
		}
	}
}

struct zaw* obs(int x, int y, struct zaw* head_zaw)//生成障碍物
{
	struct zaw* new_zaw = malloc(sizeof(struct zaw));
	new_zaw->x = x;
	new_zaw->y = y;
	new_zaw->next = head_zaw;

	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	char c_s[40];
	sprintf(c_s, "\033[38;2;%d;%d;%dm▓\033[0m", 56, 18,246);//墙位置
	printf("%s", c_s);//打印
	return new_zaw;//返回事物链表节点，更新节点
}

/*struct zaw* zaw_eat(int x, int y, struct zaw* head_zaw)//吃掉障碍物,注意，未处理head_zaw是NULL的情况
{
	struct zaw* zaw_temp;
	if (!(x == head_zaw->x && y == head_zaw->y)) {
		while (zaw_temp->next != NULL)
		{
			if (zaw_temp->next->x == x && zaw_temp->next->y == y)
			{
				free(zaw_temp->next);
				zaw_temp->next = zaw_temp->next->next;
			}
		}
	}
	else {
		zaw_temp = zaw_temp->next;
		free(head_zaw);
	}
	return zaw_temp;
}
*/

void HP_print(int x,int y,int HP_ceil,int HP_cur) {
	char* temp_hp = malloc(1000);
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("当前血量为:");
	if (HP_cur == HP_ceil) {
		pos.Y = y + 3;
		for (int i = 0; i < HP_ceil; i++) {
			if (i == 5) {
				pos.Y += 3;
				pos.X = x;
			}
			pos.Y -= 1;
			SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
			sprintf(temp_hp, "\033[38;2;%d;%d;%dm█▓\033[0m", 23, 241, 33);
			printf("%s",temp_hp);
			pos.Y += 1;
			SetConsoleCursorPosition(hOut, pos); 
			sprintf(temp_hp, "\033[38;2;%d;%d;%dm▓█\033[0m", 23, 241, 33);
			printf("%s", temp_hp);
			pos.X += 3;
		}
	}

	else {
		int xy_temp = HP_ceil - HP_cur-1;
		pos.X = x + 3 * (xy_temp%5);
		pos.Y = y + 2 + (3 * (xy_temp / 5));
		SetConsoleCursorPosition(hOut, pos);
		sprintf(temp_hp, "\033[38;2;%d;%d;%dm█▓\033[0m", 252, 52, 12);
		printf("%s", temp_hp);
		pos.Y += 1;
		SetConsoleCursorPosition(hOut, pos);
		sprintf(temp_hp, "\033[38;2;%d;%d;%dm▓█\033[0m", 252, 52, 12);
		printf("%s", temp_hp);
	}
	free(temp_hp);
}
/*
	int nums[6] = { -5,-3,0,1,5,10 };
	int nums2[6] = { -2,0,-1,1,1,2 };
*/
//{ {196,0,0},{101,8,126},{152,240,86},{92,207,234},{146,92,234}, { 225,213,234 } };颜色，大红，大紫，绿，蓝，紫，金
void instructions(){
	char* c_s = malloc(1000);
	sprintf(c_s, "\033[38;2;%d;%d;%dm规则说明：\033[0m", 132, 33, 231);//规则说明
	printf("%s\n\n", c_s);//打印

	sprintf(c_s, "\033[38;2;%d;%d;%dm●：\033[0m", 196, 0, 0);
	printf("\n%s红球：分数-5，长度-2\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm●：\033[0m", 101, 21, 126);
	printf("\n%s深紫：分数-3，长度+0，会消除一个障碍物,请注意，只有障碍物在3个以上才生效\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm●：\033[0m", 152, 240, 86);
	printf("\n%s绿球：分数+0，长度-1\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm●：\033[0m", 92, 207, 234);
	printf("\n%s蓝球：分数+1，长度+1,会回复一点血量，血量不会超过当前上限\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm●：\033[0m", 146, 92, 234);
	printf("\n%s紫球：分数+5，长度+1\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm●：\033[0m", 232, 232, 64);
	printf("\n%s金球：分数+10，长度+2\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm▓：\033[0m", 255, 255, 255);
	printf("\n%s墙体\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm▓：\033[0m", 56, 18, 246);
	printf("\n%s障碍物，碰到会扣血\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm█▓：\033[0m", 23, 241, 33);
	printf("\n%s血量，健康\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm█▓：\033[0m", 252, 52, 12);
	printf("\n%s血量，受损\n", c_s);//打印

	sprintf(c_s, "\033[38;2;%d;%d;%dm主菜单说明：\033[0m", 92, 207, 234);
	printf("\n\n\n%s\n", c_s);
	sprintf(c_s, "\033[38;2;%d;%d;%dm可以通过字母或者上下键对菜单进行操作：\033[0m", 152, 240, 86);
	printf("\n1.%s\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm可以通过排行榜查看历史记录：\033[0m", 152, 240, 86);
	printf("\n2.%s\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm删除功能可以删除当前排行榜的内容，为防止误删，想要彻底删除需要删除后进行保存：\033[0m", 152, 240, 86);
	printf("\n3.%s\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm设置功能可以对游戏进行多方面的调整，有更多样的玩法：\033[0m", 152, 240, 86);
	printf("\n4.%s\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm		设置处选择修改项后，使用上下键进行调整：\033[0m", 152, 240, 146);
	printf("\n%s\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm第一名的mvp结算会有特殊画面和音乐哦：\033[0m", 152, 240, 86);
	printf("\n5.%s\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm在设置处，将宽度调节为最大，可以对食物数量进行特殊操作哦：\033[0m", 152, 240, 86);
	printf("\n6.%s\n\n", c_s);//打印
	sprintf(c_s, "\033[38;2;%d;%d;%dm注意，在游戏中，障碍物可能偷偷的藏在食物里面，建议保持健康的血量哦：\033[0m", 152, 240, 86);
	printf("\n6.%s\n\n", c_s);//打印
}

void  music_menu(int sel,char music[6][100])//打印音乐选择菜单
{
	for (int i = 0; i < music_nums; i++)
	{
		if (i == sel) {
			printf(">>>>>%s\n", music[i]);
		}
		else {
			printf("%s\n", music[i]);
		}
	}
}

int music_sel(char music2[6][100],wchar_t music[6][100],int music_cur)
{
	int music_sum = 6;
	system("cls");
	mciSendString(L"close Star", NULL, 0, 0);
	mciSendString(music[0], NULL, 0, NULL); //初始化,mu.mp3 是播放的音乐文件 
	mciSendString(L"play Star repeat", NULL, 0, NULL); //启动播放 
	int sel = 0;
	music_menu(sel, music2);
	while (1) {
		if (kbhit()) {
			if (GetAsyncKeyState(VK_UP)) {
				Beep(bb1);
				music_cur = (music_cur + music_sum-1) % (music_sum);
				sel = (sel + music_sum-1) % (music_sum);//菜单索引
				mciSendString(L"close Star", NULL, 0, 0); //停止播放 
				mciSendString(music[sel], NULL, 0, NULL); //初始化播放的音乐文件 
				mciSendString(L"play Star repeat", NULL, 0, NULL); //启动播放 
				system("cls");
				music_menu(sel, music2);
			}
			else if (GetAsyncKeyState(VK_DOWN)) {//按下
				Beep(bb2);
				music_cur = (music_cur + 1) % (music_sum);
				sel = (sel + 1) % (music_sum);
				mciSendString(L"close Star", NULL, 0, 0); //停止播放 
				mciSendString(music[sel], NULL, 0, NULL); //初始化,mu.mp3 是你播放的音乐文件 
				mciSendString(L"play Star repeat", NULL, 0, NULL); //启动播放 
				system("cls");
				music_menu(sel, music2);
			}
			else {
				int temp = getch();
				if (temp == 27) {
					mciSendString(music[sel], NULL, 0, NULL); //初始化,mu.mp3 是你播放的音乐文件 
					mciSendString(L"play Star repeat", NULL, 0, NULL); //启动播放 
					break;
				}
			}
		}
	}
	return music_cur;
}
/*

				if (temp == '\r') {
					COORD pos = { 0,10 };
					HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
					SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
					printf("以选择音乐为:%ls",music[sel]);
					continue;
				}
*/
int food_ub() {

}

int num_sel(int up, int down, int cur, int y,int jg) {
	char print_num[100];
	COORD pos = { 35,(y+1)*2+1 };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
	sprintf(print_num, "\033[38;2;%d;%d;%dm%d   \033[0m", 196, 0, 0,cur);
	printf("当前%s   ", print_num);//打印
	while (1) {
		if (kbhit()) {
			if (GetAsyncKeyState(VK_DOWN)) {
				Beep(bb1);
				cur = (cur + up - 2*down-jg+1) % (up - down + 1)+down;
				SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
				sprintf(print_num, "\033[38;2;%d;%d;%dm %d   \033[0m", 196, 0, 0, cur);
				printf("当前%s   ", print_num);//打印
			}
			else if (GetAsyncKeyState(VK_UP)) {//按下
				Beep(bb2);
				cur = (cur-down + jg) % (up-down + 1)+down;
				SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
				sprintf(print_num, "\033[38;2;%d;%d;%dm %d   \033[0m", 196, 0, 0, cur);
				printf("当前%s   ", print_num);//打印
			}
			else {
				int sel_temp = getch();
				if (sel_temp == 27 || sel_temp == 13) {
					SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
					printf("已选择为%d    ", cur);
					return cur;  
				}
			}
		}
	}
}

void xz_w(struct xz_sel* head, char* dz)//写入
{
	if (head == NULL)return;
	FILE* b = fopen(dz, "wb");
	struct xz_sel* temp_w = head;

	if (b == NULL)
	{
		return;
	}
	int temp_w_array[9] = { temp_w->count_j,temp_w->count_zaw_j,temp_w->xl,temp_w->food_sum_up,temp_w->music_cur_num,temp_w->gamemenu_xy[0][0],temp_w->gamemenu_xy[0][1] ,temp_w->gamemenu_xy[1][0] ,temp_w->gamemenu_xy[1][1] };
	for (int i = 1; i <= 9; i++) {
		fwrite(&(temp_w_array[i-1]), sizeof(int), 1, b);
	}
	fclose(b);
}

/*
struct xz_sel {
	int count_j;//食物更新计数
	int count_zaw_j;//障碍物更新计数+
	int xl;//血量上限
	int food_sum_up;//食物上限
	int gamemenu_xy[2][2];
};
*/

struct xz_sel* xz_sel_r(char* dz) {
	FILE* xz_num = fopen(dz, "rb"); // 以二进制读取模式打开文件
	if (!xz_num) {
		perror("Failed to open file");
		return NULL;
	}

	int count = 0;
	int num_r = 0;
	struct xz_sel* temp = malloc(sizeof(struct xz_sel));
	if (!temp) {
		perror("Memory allocation failed");
		fclose(xz_num);
		return NULL;
	}

	while (count < 9) {
		switch (count) {
		case 0:
			if(fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->count_j = num_r;
			break;
		case 1:
			if (fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->count_zaw_j = num_r;
			break;
		case 2:
			if (fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->xl = num_r;
			break;
		case 3:
			if (fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->food_sum_up = num_r;
			break;
		case 4:
			if (fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->music_cur_num = num_r;
			break;
		case 5:
			if (fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->gamemenu_xy[0][0] = num_r;
			break;
		case 6:
			if (fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->gamemenu_xy[0][1] = num_r;
			break;
		case 7:
			if (fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->gamemenu_xy[1][0] = num_r;
			break;
		case 8:
			if (fread(&num_r, sizeof(int), 1, xz_num) == 1)
			temp->gamemenu_xy[1][1] = num_r;
			break;
		}
		count++;
	}

	fclose(xz_num);
	return temp; // 返回分配的结构体指针
}

void win(int x, int y) {
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	char z[40];
	sprintf(z, "\033[38;2;%d;%d;%dm█\033[0m", 255, 215, 0);
	printf("%s  %s  %s  %s   %s%s",z,z,z,z,z,z);
	pos.Y += 1;
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("%s %s %s %s     %s  %s", z, z, z, z, z, z);
	pos.Y += 1;
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("%s %s %s %s  %s  %s  %s",z,z,z,z,z,z,z);
	pos.Y += 1;
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("%s %s %s %s  %s  %s  %s",z,z,z,z,z,z,z);
	pos.Y += 1;
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf(" %s   %s   %s  %s  %s",z,z,z,z,z);
}

void gameover2(int mark1, int cd1,int mark2,int cd2) {
	char temp[500];
	sprintf(temp, "\033[38;2;92;207;234m                    本次玩家1得分%d!!  长度%d!!             \n\n\033[0m", mark1, cd1);
	COORD pos = { 7,13 };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
	printf("%s", temp);

	sprintf(temp, "\033[38;2;92;207;234m                                                本次玩家2得分%d!!  长度%d!!             \n\n\n\n\033[0m", mark2, cd2);
	pos.Y += 4;
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
	printf("%s", temp);
	printf("\033[38;2;196;0;0m                     =======================游戏结束，都怪你啦=====================\n\n\033[0m");

	return;
}

struct movexy* move_kz2(struct movexy* head, int colars[3])
{
	int x = head->x;
	int y = head->y;
	struct movexy* temp_xy = (struct move_xy*)malloc(sizeof(struct movexy));
	temp_xy->next = head;
	if (kbhit())
	{
		if (head->next == NULL) {
			if (GetAsyncKeyState('W')) {//检查是上下左右
				y--;
			}
			else if (GetAsyncKeyState('S')) {
				y++;
			}
			else if (GetAsyncKeyState('A')) {
				x--;
			}
			else if (GetAsyncKeyState('D')) {
				x++;
			}
		}
		else {
			if (GetAsyncKeyState('W') && head->y - head->next->y != 1) {//检查不动
				if (head->y - head->next->y == -1) {
					f_time2 = f_time2 - 50;
				}
				y--;
			}
			else if (GetAsyncKeyState('S') && head->y - head->next->y != -1) {
				if (head->y - head->next->y == 1) {
					f_time2 = f_time2 - 50;
				}
				y++;
			}
			else if (GetAsyncKeyState('A') && head->x - head->next->x != 1) {
				if (head->x - head->next->x == -1) {
					f_time2 = f_time2 - 50;
				}
				x--;
			}
			else if (GetAsyncKeyState('D') && head->x - head->next->x != -1) {
				if (head->x - head->next->x == 1) {
					f_time2 = f_time2 - 50;
				}
				x++;
			}
			else {
				x = x + head->x - head->next->x;
				y = y + head->y - head->next->y;
			}
		}

		Sleep(f_time2);
		f_time2 = f_time3;
		temp_xy->x = x;
		temp_xy->y = y;
		//打印蛇身体
		move_print(temp_xy, colars);
		return temp_xy;
	}
	return head;
}

void eat_TF(struct foodxy*temp_food,struct movexy*xy_head,struct zaw*zaw_head,int *food_sum_cur,int*mark,int*bc,int sl_time,int *sl_time2,int xl,int *xl_cur,int*lhh,int colars[3],int gamemenu_xy[2][2]) {
	if (temp_food != NULL && xy_head != NULL)
	{//判段是否吃到了
		struct foodxy* xy_cur = temp_food;
		struct foodxy* f0 = NULL;
		while (xy_cur != NULL)
		{
			if (xy_cur->x == xy_head->x && xy_cur->y == xy_head->y)//吃到了
			{
				*food_sum_cur--;
				*mark += xy_cur->mark;
				bc = xy_cur->spe;
				*sl_time2 = sl_time - 50;
				if (*sl_time2 < 0) { *sl_time2 = 1; }
				switch (*bc) {
				case(-2): {
					Beep(1000, 50);
					break;
				}
				case(-1): {
					Beep(1500, 50);
					break;
				}
				case(0): {
					Beep(2000, 50);
					if (zaw_head != NULL)
					{
						if (zaw_head->next != NULL)
						{
							if (zaw_head->next->next != NULL)
							{
								struct zaw* zaw_temp_m = zaw_head;

								while (zaw_temp_m->next->next != NULL)
								{
									zaw_temp_m = zaw_temp_m->next;
								}
								COORD pos_temp = { zaw_temp_m->next->x,zaw_temp_m->next->y };
								HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
								SetConsoleCursorPosition(hOut, pos_temp);//两个参数分别是指定哪个窗体，具体位置 
								char temp_col_char[100];
								sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", colars[0], colars[1], colars[2]);
								printf("%s", temp_col_char);
								printf(" ");
								printf("\033[0m");
								free(zaw_temp_m->next);
								zaw_temp_m->next = NULL;
							}
						}
					}
					break;
				}
				case(1): {
					if (*xl_cur < xl) {
						*xl_cur++;
					}
					Beep(1500, 50);
					break;
				}
				case(2): {
					Beep(3000, 50);
					break;
				}
				}

				if (f0 == NULL)//修改食物链表
				{
					temp_food = xy_cur->next;
					free(xy_cur);
				}
				else {
					f0->next = xy_cur->next;
					free(xy_cur);
				}
				COORD pos = { 0,gamemenu_xy[1][1] + 2 };
				HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
				SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
				printf("\033[0m");
				printf("当前分数:%d     ", *mark);
				pos.Y += 2;
				SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
				lhh += *bc;
				if (lhh < 2)lhh = 2;
				printf("当前长度 %d    ", lhh);
				break;
			}
			else {
				sl_time2 = sl_time;
			}
			f0 = xy_cur;
			xy_cur = xy_cur->next;
		}
	}
}