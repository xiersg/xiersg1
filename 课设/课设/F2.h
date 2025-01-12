#ifndef F2_H
#define F2_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<conio.h>
#include <time.h>
#include <Windows.h> 
#pragma comment(lib, "winmm.lib") 
#define me_num 8
#pragma warning (disable:4996)
#define name_dz "name.txt"
#define num_dz "num.txt"
#define other_dz "other.txt"
#define bb1 2500,200
#define bb2 3000,200
#define bb3 2250,200
#define music_nums 6
#define xz_dz "xz.txt"

//定义一个宏，用于保护程序
struct user;
struct movexy;

void cs_menu(int hhi);//菜单函数
struct user* input(int mark,int cd);//输入函数
struct user* lb_add(struct user* head, struct user* cur_new);//插入函数
void lb_print(struct user* head);//全部输出函数
struct user* lb_index(struct user* head, int num, char* name);//查找函数
struct user* lb_r(struct user* head, char* dz1, char* dz2, char* dz3);//读取函数
void lb_w2(struct user* head, char* dz1, char* dz2, char* dz3);//添加函数
void lb_w(struct user* head, char* dz1, char* dz2, char* dz3);//覆盖写入函数
void lb_pop(struct user* p, struct user* p0);
void game_menu(int l0, int l1, int w0, int w1, int colars[3]);//游戏菜单
void move_print(struct movexy* head, int colars[3]);//输出移动
struct movexy* move_kz(struct movexy* head,int colars[3]);//控制移动
struct movexy* move_kz2(struct movexy* head, int colars[3]);
void eat_TF(struct foodxy* temp_food,struct movexy* xy_head,struct zaw* zaw_head, int* food_sum_cur, int* mark, int* bc, int sl_time, int* sl_time2, int xl, int* xl_cur, int* lhh, int colars[3], int gamemenu_xy[2][2]);
void move_tail(struct movexy* tail, int colars[3]);
struct foodxy* food_spe(int x, int y, int spe, struct foodxy* head_f,int colars[3]);
struct foodxy* food_eat(int x, int y, struct foodxy* head_f);
int num_sj(int l, int r);
int zq(int x, int y,struct movexy*body, int fanwei[2][2]);
void gameover(int mark, int cd);
void sz_menu(int num_sel);
struct zaw* obs(int x, int y, struct zaw* head_zaw);
void HP_print(int x, int y, int HP_ceil, int HP_cur);
void instructions();
void  music_menu(int sel, char music[6][100]);//打印音乐选择菜单
int music_sel(char music2[6][100], wchar_t music[6][100], int music_cur);
int num_sel(int up, int down, int cur, int y,int jg);
void xz_w(struct xz_sel* head, char* dz);
struct xz_sel* xz_sel_r(char* dz);
void win(int x, int y);
void gameover2(int mark1, int cd1, int mark2, int cd2);

#endif