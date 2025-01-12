
#include "F2.h"

int sl_time = 65;
int sl_time2 = 65;
int sp_lev[5] = { 25,35,45,65,90 };
/*
int count_j = 30;
int count_zaw_j = 120;
int xl = 10;a
int right = 60;
int down = 25;
int food_sum_up = 3;//食物上限
int gamemenu_xy[2][2] = { {0,0},{60,25} };
*/
wchar_t music[6][100] = 
{
	L"open ./music_a.mp3 alias Star" ,
	L"open ./music_b.mp3 alias Star" ,
	L"open ./music_c.mp3 alias Star" ,
	L"open ./music_d.mp3 alias Star" ,
	L"open ./music_e.mp3 alias Star" ,
	L"open ./music_f.mp3 alias Star" 
};

char music2[6][100] =
{
	"a.音乐1\n" ,
	"b.音乐2\n" ,
	"c.音乐3\n" ,
	"d.音乐4\n" ,
	"e.音乐5\n" ,
	"f.音乐6\n"
};

struct movexy {
	int x;
	int y;
	struct movexy* next;
};

struct user {
	int ser_num;
	char name[20];
	char other[100];
	struct user* next;
};

void HideCursor()
{
	HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cinfo;
	cinfo.bVisible = 0;
	cinfo.dwSize = 1;
	SetConsoleCursorInfo(fd, &cinfo);
}

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

struct xz_sel {
	int count_j;//食物更新计数
	int count_zaw_j;//障碍物更新计数+
	int xl;//血量上限
	int food_sum_up;//食物上限
	int music_cur_num;
	int gamemenu_xy[2][2];
};

int main()
{
	HideCursor();//除去光标
	struct xz_sel* xz_sel_main;
	int count_j = 30;
	int count_zaw_j = 120;
	int xl = 10;
	int food_sum_up = 3;//食物上限
	int dt_add = 0;
	int music_cur_num = 5;
	int gamemenu_xy[2][2] = { {32,2},{87,40} };
	int colars[3] = { 240,210,170 };
	int colars2[3] = { 140,110,70 };
 	int colarss[7][3] = { {0,0,0}, { 140,110,70 },{98,152,58},{154,56,56},{124,155,155 },{109,121,155},{117,155,109} };
	int col_sel = 1;
	int sp_lev_cur = 4;
	xz_sel_main = xz_sel_r(xz_dz);

	mciSendString(music[music_cur_num], NULL, 0, NULL); //初始化,mu.mp3 是你播放的音乐文件 
	mciSendString(L"play Star repeat", NULL, 0, NULL); //启动播放 
	srand((unsigned int)time(NULL));
	struct user* users = NULL;
	users = lb_r(users, name_dz, num_dz, other_dz);
	char c = ' ';
	int sel = 0;
	cs_menu(sel);
	for (;;) 
	{
		if (kbhit()) 
		{
			if (GetAsyncKeyState(VK_UP)) {
				Beep(bb1);
				sel = (sel + me_num) % ((me_num + 1));//菜单索引
				system("cls");
				cs_menu(sel);
			}
			if (GetAsyncKeyState(VK_DOWN)) {//按下
				Beep(bb2);
				sel = (sel + 1) % (me_num + 1);
				system("cls");
				cs_menu(sel);
			}
			c = getch();//读取按键值
			if (c <= 'z' && c >= 'a') {
				Beep(bb3);
				sel = c - 'a';
				system("cls");
				cs_menu(sel);
				printf(" 选择--功能：%c", c - 'a' + 'A');
			}
			else if (c == '\r') {
				switch (sel) {
				case(7): {
					system("cls");
					sz_menu(0);//初始化游戏设置界面
					int temp_num = 0;
					while (1) {
						if (kbhit()) {
							if (GetAsyncKeyState(VK_UP)) {
								Beep(bb1);
								temp_num = (temp_num + 7) % ((7 + 1));//菜单索引
								system("cls");
								sz_menu(temp_num);
							}
							else if (GetAsyncKeyState(VK_DOWN)) {//按下
								Beep(bb2);
								temp_num = (temp_num + 1) % (7 + 1);
								system("cls");
								sz_menu(temp_num);
							}
							else {
								int temp = getch();
								if (temp == 27) {
									break;
								}
								if (temp == 13) {
									switch (temp_num) {
									case(0): {
										count_zaw_j = num_sel(120, 30, count_zaw_j, 0,2);
										continue;
									}
									case(1): {
										int temp_up = 20;
										if (dt_add == 10) { temp_up = 30; }
										food_sum_up = num_sel(temp_up, 3, food_sum_up, 1, 1);
										continue;
									}
									case(2): {
										count_j = num_sel(100, 5, count_j, 2, 2);
										continue;
									}
									case(3): {
										col_sel = num_sel(6, 0, col_sel, 3, 1);
										colars[0] = colarss[col_sel][0];
										colars[1] = colarss[col_sel][1];
										colars[2] = colarss[col_sel][2];
										continue;
									}
									case(4): {
										music_cur_num = music_sel(music2,music,music_cur_num);//初始化音乐设置界面
										system("cls");
										sz_menu(temp_num);
										continue;
									}
									case(5): {
										gamemenu_xy[0][0] += dt_add;
										gamemenu_xy[1][0] -= dt_add;
										dt_add = num_sel(10, 0, dt_add, 5, 1);
										gamemenu_xy[0][0] -= dt_add;
										gamemenu_xy[1][0] += dt_add;

										if (dt_add != 10 && food_sum_up == 30) { food_sum_up = 20; }
										continue;
									}
									case(6): {
										xl = num_sel(10, 1, xl, 6, 1);
										continue;
									}
									case(7):{//选择基础速度
										sp_lev_cur = num_sel(5, 1, sp_lev_cur, 7, 1);
										sl_time = sp_lev[sp_lev_cur-1];
										sl_time2 = sl_time;
										continue;
									}
									}
									continue;
								}
							}
						}
					}
					system("cls");
					cs_menu(sel);
					break;
				}
				case(2): {
					system("cls");
					printf("！！！！排行榜！！！！\n\n\n");
					lb_print(users);
					printf("任意键退出");
					getch();
					system("cls");
					cs_menu(sel);
					break;
				}
				case(3): {
					system("cls");
					char* name_s = (char*)malloc(20);
					int* num_s = (int*)malloc(sizeof(int));
					struct user* temp_s = (struct user*)malloc(sizeof(struct user*));//一个暂时链表指针
					printf("\n\n请输入想要查询的目标(可通过名字或代号进行查询)，\n\n账号：");
					fgets(name_s, 20, stdin);
					name_s[strcspn(name_s, "\n")] = '\0';
					printf("长度：");
					scanf(" %d", num_s);
					getchar();

					temp_s = users;
					while (1) {
						temp_s = lb_index(temp_s, *num_s, name_s);
						if (temp_s == NULL)break;
						printf("---账号%s\n\n---长度%d\n\n---信息%s\n\n\n", temp_s->name, temp_s->ser_num, temp_s->other);
						if (temp_s != NULL) { temp_s = temp_s->next; }
					}

					free(name_s);
					free(num_s);
					free(temp_s);
					printf("按任意键返回\n");
					getch();
					system("cls");
					cs_menu(sel);
					break;
				}
				case(4): {
					printf("请选择保存方案1/2(覆盖，添加)");
					int i = getchar();
					getchar();
					struct user* temp_w;
					if (i == '1') {
						lb_w(users, name_dz, num_dz, other_dz);
					}
					else {
						if (i == '2') {
							lb_w2(users, name_dz, num_dz, other_dz);
						}
						else { printf("选择错误"); }
					}
					printf("按任意键返回\n");

					getch();
					system("cls");
					cs_menu(sel);
					break;
				}
				case(5):
				{
					system("cls");
					instructions();
					printf("按任意键返回\n");
					getch();
					system("cls");
					cs_menu(sel);
					break;
				}
				case(6):
				{
					system("cls");
					cs_menu(sel);
					printf("删除当前全部内容！");
					lb_pop(users, users);
					users = NULL;
					printf("按任意键返回\n");
					getch();
					system("cls");
					cs_menu(sel);
					break;
				}
				case(0): {
					system("cls");
					game_menu(gamemenu_xy[0][1],gamemenu_xy[1][1], gamemenu_xy[0][0], gamemenu_xy[1][0],colars);
					struct movexy* xy_head = (struct movexy*)malloc(sizeof(struct movexy));//作为蛇的开头
					struct movexy* xy_before = NULL;
					struct zaw* zaw_head = malloc(sizeof(struct zaw));//障碍物链表
					zaw_head = NULL;
					//初始化 xy_head
					xy_head->x = (gamemenu_xy[0][0] + gamemenu_xy[1][0]) / 2;
					xy_head->y = (gamemenu_xy[0][1] + gamemenu_xy[1][1]) / 2;
					xy_head->next = NULL;
					int mark = 0;//分数
					int bc = 0;//这个是变长参数
					int lhh = 2;
					int count = 0;//用来定期更换食物
					int count_zaw = 0;
					int xl_cur = xl;
					int food_sum_cur = 0;
					int zt_jud = 1;
					//int xl_bh = 0;//血量变化
					struct foodxy* temp_food = malloc(sizeof(struct foodxy));//食物链表
					temp_food = NULL;//定义一个空temp_food传入food_spe

					//游戏开始
					while (1) {
						if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {   //用来暂停
							COORD pos = { gamemenu_xy[1][0]+3,18};
							HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
							SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
							char temp_col[100];
							printf("\033[0m");
							sprintf(temp_col, "\033[38;2;%d;%d;%dm 回车结束战斗\033[0m", 196, 0, 0);
							printf("%s", temp_col);
							pos.Y += 2;
							SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
							sprintf(temp_col, "\033[38;2;%d;%d;%dm Tab返回战斗\033[0m", 196, 0, 0);
							printf("%s", temp_col);
							int temp_getch;
							while (1) {
								temp_getch = getch();
								if (temp_getch == '\r') {
									xl_cur = 0;
									zt_jud = 0;
									break;
								}
								else if (temp_getch == 9) {
									SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
									printf("              ");
									pos.Y -= 2;
									SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
									printf("              ");
									break;
								}
							}
						}
						printf("\033[0m");
						HP_print(gamemenu_xy[1][0]+3, 3, xl, xl_cur);//打印当前血量
						do {
							xy_head = move_kz(xy_head,colars);//移动
						} while (xy_head == xy_before && zt_jud == 1);
						xy_before = xy_head;
						if (xy_head->next != NULL) {
							if (xy_head->next->next != NULL)
							{
								if (bc <= 0) {
									if(bc ==0){
										if (xy_head->next->next->next != NULL) {
											move_tail(xy_head,colars);//除去尾巴
											count++;
											count_zaw++;
										}
									}
									else {
										int n = 0 - bc + 1;
										bc = 0;
										for (n; n > 0; n--) {
											if (xy_head->next != NULL) {
												if (xy_head->next->next != NULL)
												{
													move_tail(xy_head,colars);//除去尾巴
													count++;
													count_zaw++;
												}
											}
										}
									}
								}
								if (bc > 0) { bc -= 1; }
							}
						}

						if (count_zaw >= count_zaw_j) {//生成障碍物
							struct movexy* movexy_cur = xy_head;//用于遍历
							struct foodxy* foodxy_cur = temp_food;
							int x_t, y_t;
							int xy_temp[1000][2] = {0};
							int count_xy_temp = 0;
							x_t = num_sj(gamemenu_xy[0][0]+1, gamemenu_xy[1][0]-2);
							y_t = num_sj(gamemenu_xy[0][1]+1, gamemenu_xy[1][1]-2);
							while (movexy_cur != NULL)
							{
								xy_temp[count_xy_temp][0] = movexy_cur->x;
								xy_temp[count_xy_temp][1] = movexy_cur->y;
								count_xy_temp++;
								movexy_cur = movexy_cur->next;
							}
							while (foodxy_cur != NULL) {
								xy_temp[count_xy_temp][0] = foodxy_cur->x;
								xy_temp[count_xy_temp][1] = foodxy_cur->y;
								count_xy_temp++;
								foodxy_cur = foodxy_cur->next;
							}
							for (int i = 0; i <= count_xy_temp;i++) {
								if (x_t == xy_temp[i][0] && y_t == xy_temp[i][1]) {
									x_t = num_sj(gamemenu_xy[0][0]+1, gamemenu_xy[1][0]-2);
									y_t = num_sj(gamemenu_xy[0][1]+1, gamemenu_xy[1][1]-2);
									i = 0;
								}
							}
							zaw_head = obs(x_t, y_t, zaw_head);
							count_zaw = 0;
						}

						

						if (temp_food == NULL || temp_food->next ==NULL || temp_food->next->next == NULL || food_sum_cur<food_sum_up)//这里用来生成食物
						{
							struct movexy* xy_cur;
							xy_cur = xy_head;
							int x_t, y_t, spe_t;
							x_t = num_sj(gamemenu_xy[0][0] + 1, gamemenu_xy[1][0] - 2);
							y_t = num_sj(gamemenu_xy[0][1] + 1, gamemenu_xy[1][1] - 2);
							spe_t = num_sj(0, 5);
							while (xy_cur != NULL)
							{
								if (x_t == xy_cur->x && y_t == xy_cur->y)
								{
									x_t = num_sj(gamemenu_xy[0][0] + 1, gamemenu_xy[1][0] - 2);
									y_t = num_sj(gamemenu_xy[0][1] + 1, gamemenu_xy[1][1] - 2);
									xy_cur = xy_head;
								}
								else if (xy_cur->next != NULL) { xy_cur = xy_cur->next; }
								else {
									break;
								}
							}
							temp_food = food_spe(x_t, y_t, spe_t, temp_food,colars);
							food_sum_cur++;
						}
						else {
							if (count >= count_j) {//清除食物
								struct foodxy* p_t = temp_food;
								while (p_t->next->next != NULL)
								{
									p_t = p_t->next;
								}
								COORD pos = { p_t->next->x ,p_t->next->y };
								HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
								SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
								char temp_col_char[100];
								sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", colars[0], colars[1], colars[2]);
								printf("%s",temp_col_char);
								printf(" ");
								printf("\033[0m");
								free(p_t->next);
								p_t->next = NULL;
								count = 0;
								food_sum_cur--;
							}
						}
						if (zaw_head != NULL) {//判断是不是撞到障碍物了
							struct zaw* zaw_temp = zaw_head;
							if (!(xy_head->x == zaw_head->x && xy_head->y == zaw_head->y)) {//判断是不是第一个
								while (zaw_temp->next != NULL)
								{
									if (zaw_temp->next->x == xy_head->x && zaw_temp->next->y == xy_head->y)
									{
										struct zaw* temp_zaw_2 = zaw_temp->next;
										zaw_temp->next = zaw_temp->next->next;
										xl_cur--;
										Beep(500, 50);
										free(temp_zaw_2);
										break;
									}
									zaw_temp = zaw_temp->next;
								}
							}
							else {
								zaw_head = zaw_head->next;
								free(zaw_temp);
								xl_cur--;
								Beep(500, 50);
							}
						}
						if (temp_food != NULL&&xy_head!=NULL) 
						{//判段是否吃到了
							struct foodxy* xy_cur = temp_food;
							struct foodxy*f0 = NULL;
							while (xy_cur != NULL)
							{
								if (xy_cur->x == xy_head->x && xy_cur->y == xy_head->y)//吃到了
								{
									food_sum_cur--;
									mark += xy_cur->mark;
									bc = xy_cur->spe;
									sl_time2 = sl_time - 50;
									if (sl_time2 < 0) { sl_time2 = 1; }
									switch (bc) {
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
										if (xl_cur < xl) {
											xl_cur++;
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
										temp_food =xy_cur->next;
										free(xy_cur);
									}
									else {
										f0->next = xy_cur->next;
										free(xy_cur);
									}
									COORD pos = { 5,gamemenu_xy[0][1]+3};
									HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
									SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
									printf("\033[0m");
									printf("当前分数:%d     ", mark);
									pos.Y += 2;
									SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
									lhh += bc;
									if (lhh < 2)lhh = 2;
									printf("当前长度 %d    ",lhh);
									break;
								}
								else {
									sl_time2 = sl_time;
								}
								f0 = xy_cur;
								xy_cur = xy_cur->next;
							}
						}
						else {
							sl_time2 = sl_time;
						}
						if (xl_cur <= 0 || zq(xy_head->x, xy_head->y,xy_head->next,gamemenu_xy) == -1)//游戏结束判断条件
						{

							printf("\033[0m");
							system("cls");
							gameover(mark,lhh);
							HideCursor();
							HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
							FlushConsoleInputBuffer(hInput, NULL);
							if (users == NULL || mark >= users->ser_num) {
								mciSendString(L"close Star", NULL, 0, 0); //停止播放 

								win(52, 20);
								mciSendString(L"open ./winer.mp3 alias Star", NULL, 0, NULL); //初始化,mu.mp3 是你播放的音乐文件 
								mciSendString(L"play Star repeat", NULL, 0, NULL); //启动播放 
							}

							COORD pos = { 20,25 };
							HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
							SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
							printf("是否保存，回车保存，ESC不保存\n");
							char get;
							while (get = getch())
							{
								if (get == '\r')
								{
									users = lb_add(users, input(mark, lhh));
									lb_w(users, name_dz, num_dz, other_dz);
									printf("任意键退出\n");
									getch();
									break;
								}
								if (get == 27) {
									printf("退出");
									break;
								}
							}

							mciSendString(L"close Star", NULL, 0, 0); //停止播放 
							mciSendString(music[music_cur_num], NULL, 0, NULL); //初始化,mu.mp3 是你播放的音乐文件 
							mciSendString(L"play Star repeat", NULL, 0, NULL); //启动播放 
							break;
						}
						Sleep(sl_time2);
					}
					system("cls");
					cs_menu(sel);
					break;
				}
				case(1):
				{
					system("cls");//清屏
					game_menu(gamemenu_xy[0][1], gamemenu_xy[1][1], gamemenu_xy[0][0], gamemenu_xy[1][0], colars);//打印游戏页面
					struct movexy* xy_head1 = (struct movexy*)malloc(sizeof(struct movexy));//作为蛇的开头
					struct movexy* xy_head2 = (struct movexy*)malloc(sizeof(struct movexy));//第二条蛇的开头

					struct movexy* xy_before1 = NULL;
					struct movexy* xy_before2 = NULL;

					struct zaw* zaw_head = malloc(sizeof(struct zaw));//障碍物链表
					zaw_head = NULL;
					//初始化 xy_head
					xy_head1->x = (gamemenu_xy[0][0] + gamemenu_xy[1][0]) / 2 - 5;
					xy_head1->y = (gamemenu_xy[0][1] + gamemenu_xy[1][1]) / 2;
					xy_head1->next = NULL;
					xy_head2->x = (gamemenu_xy[0][0] + gamemenu_xy[1][0]) / 2 + 5;
					xy_head2->y = (gamemenu_xy[0][1] + gamemenu_xy[1][1]) / 2;
					xy_head2->next = NULL;

					int mark1 = 0;//分数
					int mark2 = 0;
					int bc1 = 0;//这个是变长参数
					int bc2 = 0;
					int lhh1 = 2;//当前长度
					int lhh2 = 2;
					int count = 0;//用来定期更换食物
					int count_zaw = 0;//用于定期更换障碍物
					int xl_cur1 = xl;//当前血量
					int xl_cur2 = xl;
					int food_sum_cur = 0;
					int zt_jud = 1;//用来停止退出
					//int xl_bh = 0;//血量变化
					struct foodxy* temp_food = malloc(sizeof(struct foodxy));//食物链表
					temp_food = NULL;//定义一个空temp_food传入food_spe

					//游戏开始
					while (1) 
					{
						if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) 
						{   //用来暂停
							COORD pos = { gamemenu_xy[1][0] + 3,18 };
							HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
							SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
							char temp_col[100];
							printf("\033[0m");
							sprintf(temp_col, "\033[38;2;%d;%d;%dm 回车结束战斗\033[0m", 196, 0, 0);
							printf("%s", temp_col);
							pos.Y += 2;
							SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
							sprintf(temp_col, "\033[38;2;%d;%d;%dm Tab返回战斗\033[0m", 196, 0, 0);
							printf("%s", temp_col);
							int temp_getch;
							while (1) 
							{
								temp_getch = getch();
								if (temp_getch == '\r') 
								{
									xl_cur1 = 0;
									xl_cur2 = 0;
									zt_jud = 0;
									break;
								}
								else if (temp_getch == 9) 
								{
									SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
									printf("              ");
									pos.Y -= 2;
									SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
									printf("              ");
									break;
								}
							}
						}
						printf("\033[0m");
						HP_print(gamemenu_xy[1][0] + 3, 3, xl, xl_cur1);//打印当前血量
						HP_print(0, 3, xl, xl_cur2);                    //打印当前血量
						do 
						{
							xy_head1 = move_kz2(xy_head1, colars);//移动
							xy_head2 = move_kz(xy_head2, colars2);//角色2移动
						} while (xy_head1 == xy_before1 && xy_head2 == xy_before2  && zt_jud == 1);//移动
						xy_before1 = xy_head1;
						xy_before2 = xy_head2;
						if (xy_head1->next != NULL) 
						{
							if (xy_head1->next->next != NULL)
							{
								if (bc1 <= 0) 
								{
									if (bc1 == 0) 
									{
										if (xy_head1->next->next->next != NULL) 
										{
											move_tail(xy_head1, colars);//除去尾巴
											count++;
											count_zaw++;
										}
									}
									else 
									{
										int n = 0 - bc1 + 1;
										bc1 = 0;
										for (n; n > 0; n--) 
										{
											if (xy_head1->next != NULL) {
												if (xy_head1->next->next != NULL)
												{
													move_tail(xy_head1, colars);//除去尾巴
													count++;
													count_zaw++;
												}
											}
										}
									}
								}
								if (bc1 > 0) { bc1 -= 1; }
							}
						}
						if (xy_head2->next != NULL) 
						{
							if (xy_head2->next->next != NULL)
							{
								if (bc2 <= 0) 
								{
									if (bc2 == 0) 
									{
										if (xy_head2->next->next->next != NULL) 
										{
											move_tail(xy_head2, colars);//除去尾巴
											count++;
											count_zaw++;
										}
									}
									else 
									{
										int n = 0 - bc2 + 1;
										bc2 = 0;
										for (n; n > 0; n--) 
										{
											if (xy_head2->next != NULL) 
											{
												if (xy_head2->next->next != NULL)
												{
													move_tail(xy_head2, colars);//除去尾巴
													count++;
													count_zaw++;
												}
											}
										}
									}
								}
								if (bc2 > 0) { bc2 -= 1; }
							}
						}
						if (count_zaw >= count_zaw_j) 
						{//生成障碍物
							struct movexy* movexy_cur1 = xy_head1;//用于遍历
							struct movexy* movexy_cur2 = xy_head2;
							struct foodxy* foodxy_cur = temp_food;
							int x_t, y_t;
							int xy_temp[1000][2] = { 0 };
							int count_xy_temp = 0;
							x_t = num_sj(gamemenu_xy[0][0] + 1, gamemenu_xy[1][0] - 2);
							y_t = num_sj(gamemenu_xy[0][1] + 1, gamemenu_xy[1][1] - 2);
							while (movexy_cur1 != NULL)//将内容不可生成位置保存在里面，可以对此进行优化
							{
								xy_temp[count_xy_temp][0] = movexy_cur1->x;
								xy_temp[count_xy_temp][1] = movexy_cur1->y;
								count_xy_temp++;
								movexy_cur1 = movexy_cur1->next;
							}
							while (movexy_cur2 != NULL)
							{
								xy_temp[count_xy_temp][0] = movexy_cur2->x;
								xy_temp[count_xy_temp][1] = movexy_cur2->y;
								count_xy_temp++;
								movexy_cur2 = movexy_cur2->next;
							}
							while (foodxy_cur != NULL) {
								xy_temp[count_xy_temp][0] = foodxy_cur->x;
								xy_temp[count_xy_temp][1] = foodxy_cur->y;
								count_xy_temp++;
								foodxy_cur = foodxy_cur->next;
							}
							for (int i = 0; i <= count_xy_temp; i++) {
								if (x_t == xy_temp[i][0] && y_t == xy_temp[i][1]) {
									x_t = num_sj(gamemenu_xy[0][0] + 1, gamemenu_xy[1][0] - 2);
									y_t = num_sj(gamemenu_xy[0][1] + 1, gamemenu_xy[1][1] - 2);
									i = 0;
								}
							}
							zaw_head = obs(x_t, y_t, zaw_head);
							count_zaw = 0;
						}
						if (temp_food == NULL || temp_food->next == NULL || temp_food->next->next == NULL || food_sum_cur < food_sum_up)//这里用来生成食物
						{
							int xy_temp[1000][2] = { 0 };
							int count_xy_temp = 0;
							struct movexy* xy_cur1;
							struct movexy* xy_cur2;
							xy_cur1 = xy_head1;
							xy_cur2 = xy_head2;
							int x_t, y_t, spe_t;
							x_t = num_sj(gamemenu_xy[0][0] + 1, gamemenu_xy[1][0] - 2);//生成随机数
							y_t = num_sj(gamemenu_xy[0][1] + 1, gamemenu_xy[1][1] - 2);//生成随机数
							spe_t = num_sj(0, 5);
							while (xy_cur1 != NULL)//将内容不可生成位置保存在里面，可以对此进行优化
							{
								xy_temp[count_xy_temp][0] = xy_cur1->x;
								xy_temp[count_xy_temp][1] = xy_cur1->y;
								count_xy_temp++;
								xy_cur1 = xy_cur1->next;
							}
							while (xy_cur2 != NULL)
							{
								xy_temp[count_xy_temp][0] = xy_cur2->x;
								xy_temp[count_xy_temp][1] = xy_cur2->y;
								count_xy_temp++;
								xy_cur2 = xy_cur2->next;
							}
							for (int i = 0; i <= count_xy_temp; i++) 
							{
								if (x_t == xy_temp[i][0] && y_t == xy_temp[i][1]) {
									x_t = num_sj(gamemenu_xy[0][0] + 1, gamemenu_xy[1][0] - 2);
									y_t = num_sj(gamemenu_xy[0][1] + 1, gamemenu_xy[1][1] - 2);
									i = 0;
								}
							}
							temp_food = food_spe(x_t, y_t, spe_t, temp_food, colars);
							food_sum_cur++;
						}
						else 
						{
							if (count >= count_j) {//清除食物
								struct foodxy* p_t = temp_food;
								while (p_t->next->next != NULL)
								{
									p_t = p_t->next;
								}
								COORD pos = { p_t->next->x ,p_t->next->y };
								HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
								SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
								char temp_col_char[100];
								sprintf(temp_col_char, "\033[48;2;%d;%d;%dm", colars[0], colars[1], colars[2]);
								printf("%s", temp_col_char);
								printf(" ");
								printf("\033[0m");
								free(p_t->next);
								p_t->next = NULL;
								count = 0;
								food_sum_cur--;
							}
						}
						if (zaw_head != NULL) {//判断是不是撞到障碍物了
							struct zaw* zaw_temp = zaw_head;
							if (!(xy_head1->x == zaw_head->x && xy_head1->y == zaw_head->y)) {//判断不是第一个
								while (zaw_temp->next != NULL)
								{
									if (zaw_temp->next->x == xy_head1->x && zaw_temp->next->y == xy_head1->y)
									{
										struct zaw* temp_zaw_21 = zaw_temp->next;
										zaw_temp->next = zaw_temp->next->next;
										xl_cur1--;
										Beep(500, 50);
										free(temp_zaw_21);
										break;
									}
									zaw_temp = zaw_temp->next;
								}
							}
							else {
								zaw_head = zaw_head->next;
								free(zaw_temp);
								zaw_temp = zaw_head;
								xl_cur1--;
								Beep(500, 50);
							}
							zaw_temp = zaw_head;
							if (zaw_head != NULL)
							{
								if (!(xy_head2->x == zaw_head->x && xy_head2->y == zaw_head->y)) {//判断是不是第一个
									while (zaw_temp->next != NULL)
									{
										if (zaw_temp->next->x == xy_head2->x && zaw_temp->next->y == xy_head2->y)
										{
											struct zaw* temp_zaw_22 = zaw_temp->next;
											zaw_temp->next = zaw_temp->next->next;
											xl_cur2--;
											Beep(500, 50);
											free(temp_zaw_22);
											break;
										}
										zaw_temp = zaw_temp->next;
									}
								}
								else {
									zaw_head = zaw_head->next;
									free(zaw_temp);
									xl_cur2--;
									Beep(500, 50);
								}
							}
						}

						if (temp_food != NULL && xy_head1 != NULL)
						{//判段是否吃到了
							struct foodxy* xy_cur = temp_food;
							struct foodxy* f0 = NULL;
							while (xy_cur != NULL)
							{
								if (xy_cur->x == xy_head1->x && xy_cur->y == xy_head1->y)//吃到了
								{
									food_sum_cur--;
									mark1 += xy_cur->mark;
									bc1 = xy_cur->spe;
									sl_time2 = sl_time - 50;
									if (sl_time2 < 0) { sl_time2 = 1; }
									switch (bc1) {
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
										if (xl_cur1 < xl) {
											xl_cur1++;
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
									printf("当前分数:%d     ", mark1);
									pos.Y += 2;
									SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
									lhh1 += bc1;
									if (lhh1 < 2)lhh1 = 2;
									printf("当前长度 %d    ", lhh1);
									break;
								}
								else {
									sl_time2 = sl_time;
								}
								f0 = xy_cur;
								xy_cur = xy_cur->next;
							}
						}
						if (temp_food != NULL && xy_head2 != NULL){//判段是否吃到了
				struct foodxy* xy_cur = temp_food;
				struct foodxy* f0 = NULL;
				while (xy_cur != NULL)
				{
					if (xy_cur->x == xy_head2->x && xy_cur->y == xy_head2->y)//吃到了
					{
						food_sum_cur--;
						mark2 += xy_cur->mark;
						bc2 = xy_cur->spe;
						sl_time2 = sl_time - 50;
						if (sl_time2 < 0) { sl_time2 = 1; }
						switch (bc2) {
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
							if (xl_cur2 < xl) {
								xl_cur2++;
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
						COORD pos = { gamemenu_xy[1][0],gamemenu_xy[1][1] + 2 };
						HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
						SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
						printf("\033[0m");
						printf("当前分数:%d     ", mark2);
						pos.Y += 2;
						SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
						lhh2 += bc2;
						if (lhh2 < 2)lhh2 = 2;
						printf("当前长度 %d    ", lhh2);
						break;
					}
					else {
						sl_time2 = sl_time;
					}
					f0 = xy_cur;
					xy_cur = xy_cur->next;
				}
}
						if (xl_cur1 <= 0|| xl_cur2<=0 || zq(xy_head1->x, xy_head1->y, xy_head1->next, gamemenu_xy) == -1 || zq(xy_head2->x, xy_head2->y, xy_head2->next, gamemenu_xy) == -1 || zq(xy_head1->x, xy_head1->y, xy_head2, gamemenu_xy) == -1 || zq(xy_head2->x, xy_head2->y, xy_head1, gamemenu_xy) == -1)//游戏结束判断条件
						{
							printf("\033[0m");
							system("cls");
							gameover2(mark1, lhh1,mark2,lhh2);
							HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
							FlushConsoleInputBuffer(hInput, NULL);
							COORD pos = { 20,25 };
							HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄 
							SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置 
							printf("是否保存，回车保存，ESC不保存\n");
							char get;
							while (get = getch())
							{
								if (get == '\r')
								{
									printf("\n\n玩家1\n\n");
									users = lb_add(users, input(mark1, lhh1));
									printf("\n\n玩家2\n\n");
									users = lb_add(users, input(mark2, lhh2));
									lb_w(users, name_dz, num_dz, other_dz);
									printf("任意键退出\n");
									getch();
									break;
								}
								if (get == 27) {
									printf("退出");
									break;
								}
							}
							break;
						}
						Sleep(sl_time2/2);//游戏时间处理

					}
					system("cls");
					cs_menu(sel);
					break;
				}
				case(8): {
					system("cls");
					printf("\n\n\n      =====程序已退出，欢迎再次使用喵======       \n\n\n");
					xz_w(xz_sel_main, "xz.txt");
					return 0;}
				}
			}
			Sleep(50);
		}
	}

	mciSendString(L"close Star", NULL, 0, 0); //停止播放 
	return 0;
}
