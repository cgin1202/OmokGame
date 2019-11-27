#include <mysql.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "Types.h"

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "tmdwls7127"
#define DB_NAME "op"
#define CHOP(x) x[strlen(x) - 1] = ' '

_User user;
_User user2;
int timeover;

int login2(MYSQL *conn); //두번째 고객 로그인
int Calculate_rate(int a, int b); //승률 계산 함수
void Print_Information(); //개인 정보 출력
void end_game_screen(); //게임 종료 스크린
void second_menu(); //두번째 메뉴 스크린
void input_data(_User *User); //회원가입
int insert(MYSQL *, _User User);
int login(MYSQL *); //로그인함수
int print(MYSQL *); //전체 고객 출력 
int delete(MYSQL *); //회원탈퇴
void gotoxy(int x, int y); //커서 이동
void first_screen(); //첫번째 스크린
void second_screen(); //두번째 스크린
void program_screen(); //프로그램 설명 스크린
void game_screen(); //게임 설명 스크린
void logout_fail_screen(); //로그인 실패 스크린
void logout_screen();//로그아웃 스크린
void zero_screen(); //제로 스크린

Board* makeEmptyBoard();
Board* makeBoard(ll A, ll B, ll C, ll D);
Board* board_mask(Board* board);
Board* board_shift_right(Board* board, int shift);
Board* board_shift_left(Board* board, int shift);
Board* board_and(Board* baseBoard, Board* layerBoard);
Board* board_or(Board* baseBoard, Board* layerBoard);
Board* board_xor(Board* baseBoard, Board* layerBoard);
Board* board_not(Board* board);
Board* board_copy(Board* board);
Board* board_set(Board* board, int i, int j);
bool board_isSet(Board* board, int i, int j);
bool board_isEmpty(Board* board);
bool board_isEqual(Board* boardA, Board* boardB);
void board_find_diff_one(Board* bigger, Board* smaller, int *x, int *y);
bool check_board_complete(Board* board);
bool check_board_mask(Board* board, Board* mask);

NodeArray* narray_init();
void narray_insert(NodeArray *a, Node element);
NodeArray* narray_copy(NodeArray *target);
void narray_free(NodeArray *a);

void MCTS(Node *root);
int simulate(Node *node);
Node* selectHeuristic(Node *root);

Node* node_init(Node* node);
Node* node_copy(Node* target, bool liveChild);
void setBound(Node* node, int x, int y, Turn turn);

void ready();
Node* findNext(Node* node);
Node* findChildSameBoard(Node* node, Board* black, Board* white);
Node *realRoot = NULL, *tmpRoot, *toFind;

void game_table(MYSQL *conn);
void game_table_com();
void draw_rectangle(int r, int c);
void print_board_com(Board* boardBlack, Board* boardWhite);
void print_board(Board* board_1, Board* board_2, int n);
int turn_two_players(int n);
void Game_Tutorial(Board* board);
void game_table_tutorial(Board *board);
void move_point();
void move_control_tuto(Board* board);
void draw_omok();
void move_control(Board* board_A, Board* board_B);
////////////
int x_c, y_c;
int count_time;

DWORD WINAPI ThreadFunc(void *data);
HANDLE thread;

char password1[20];
char nickname1[20];

char password2[20];
char nickname2[20];

int main(void)
{
	Board* board = makeBoard((ll)0, (ll)0, (ll)0, (ll)0);

	int logout_count = 0;
	int logout_key; //로그아웃 키
	int input_menu3; //네번째 메뉴 입력
	int input_menu2; //세번째 메뉴 입력
	int input_menu1; //두번째 메뉴 입력
	int first_menu; //첫번째 메뉴 입력
	int i, n;

	MYSQL       *connection = NULL, conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   row;
	int field;
	int query_stat;
	mysql_init(&conn);
	int query[255];

	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	if (connection == NULL) {
		printf("connect error!\n");
		exit(1);
	}
	zero_screen();
	first_screen(); //첫번째 화면 -> 게임시작
	while (1) {
		second_screen();//두번째 화면 -> 메뉴
		gotoxy(35, 21);
		printf("메뉴를 입력해주십시오 >>   ");
		scanf("%d", &first_menu);
		switch (first_menu) {
		case 1: //회원가입
			system("cls");
			input_data(&user);
			printf("%p\n", connection);
			insert(connection, user);
			system("cls");
			break;
		case 2: //로 그 인
			system("cls");
			if (login(connection) != 0) {
				system("cls");
				break;
			}
			system("cls");
			while (1) {
				system("cls");
				second_menu();
				gotoxy(35, 23);
				printf("메뉴를 입력해주십시오 >>   ");
				scanf("%d", &input_menu1);
				switch (input_menu1) {
				case 1: //프로그램 설명
					system("cls");
					program_screen();
					gotoxy(35, 21);
					printf("메뉴를 입력해주십시오 >>   ");
					scanf("%d", &input_menu2);
					switch (input_menu2) {
					case 1: //튜토리얼 시작
						system("cls");
						Game_Tutorial(board);
						//printf("튜토리얼 시작\n");
						getch();
						break;
					case 2: //뒤로 돌아가기
						break;
					default:
						break;
					} // end switch
					break;
				case 2: //개인 정보 출력
					system("cls");
					Print_Information();
					printf("\n\n");
					getch();
					system("cls");
					break;
				case 3: //1인용 / 2인용 게임플레이
					system("cls");
					game_screen();
					gotoxy(35, 21);
					printf("메뉴를 입력해주십시오 >>   ");
					scanf("%d", &input_menu2);
					switch (input_menu2) {
					case 1: //1인용 게임플레이
						system("cls");
						system("cls");
						game_table_com();
						//printf("1인용 게임플레이\n");
						getch();
						break;
					case 2: //2인용 게임플레이
						system("cls");
						if (login2(connection) != 0) {
							system("cls");
							break;
						}
						system("cls");
						game_table(connection);
						user.rate = Calculate_rate(user.win, user.lose);
						getch();
						break;
					case 3: //뒤로 돌아가기
						break;
					default:
						break;
					}
					break;

				case 4: //로그아웃창
					logout_count = 1;
					break;
				default:
					break;
				}
				if (logout_count == 1) {
					system("cls");
					logout_count = 0;
					gotoxy(35, 5);
					printf("오        목        프        로        그        램");
					gotoxy(100, 9);
					printf("by으라차차");
					gotoxy(35, 15);
					printf("로그아웃을 하시겠습니까?(로그아웃 : 1번) >> ");
					scanf("%d", &logout_key);
					if (logout_key == 1) {
						logout_screen();
						break;
					}
				}
			}
			system("cls");
			break;
		case 3: //종    료
			system("cls");
			end_game_screen();
			//system("cls");
			//zero_screen();
			return 1;
			break;
		default:
			system("cls");
			break;
		}
	}
	return 0;
}
void logout_screen() {//로그아웃 스크린
	system("cls");
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(100, 9);
	printf("by으라차차");
	gotoxy(39, 15);
	printf("로 그 아 웃 되 었 습 니 다");
	gotoxy(39, 18);
	printf("회원가입 로그인 창으로 이동하겠습니다.");
	printf("\n\n");
	getch();
}
void Print_Information() {
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(100, 9);
	printf("by으라차차");
	gotoxy(30, 15);
	printf("고객님의 이름 : %s\n", user.name);
	gotoxy(30, 17);
	printf("고객님의 나이 : %d\n", user.age);
	gotoxy(30, 19);
	printf("고객님의 아이디 : %s\n", user.nickname);
	gotoxy(30, 21);
	printf("고객님의 이메일 : %s\n", user.e_mail);
	gotoxy(30, 23);
	printf("고객님의 오목알 모양 : %s / %s\n", user.stone_shape1, user.stone_shape2);
	gotoxy(30, 25);
	printf("고객님의 승 : %d / 패 : %d / 승률 : %d\n", user.win, user.lose, user.rate);
}
void logout_fail_screen() {
	system("cls");
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(100, 9);
	printf("by으라차차");
	gotoxy(35, 15);
	printf("아이디 또는 비밀번호를 다시 확인하세요  \n");
	gotoxy(35, 18);
	printf("기존에 등록되지 않은 아이디이거나, 아이디 또는 비밀번호를 잘못 입력하셨습니다  \n");
	gotoxy(35, 21);
	printf("다시 입력해주십시오  ");
	printf("\n\n");
	getch();
	system("cls");
}
void end_game_screen() {
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(39, 15);
	printf("이 용 해 주 셔 서    감 사 합 니 다 ^^");
	gotoxy(90, 20);
	printf("팀 명 : 으  라  차  차");
	gotoxy(90, 22);
	printf("팀 원 : 최 승 진");
	gotoxy(90, 24);
	printf("        양 회 욱");
	gotoxy(90, 26);
	printf("        오 상 헌");
	printf("\n\n");
}
void game_screen() {
	int menu;
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(100, 9);
	printf("by으라차차");
	gotoxy(35, 15);
	printf("1 번 째 메 뉴       >>       1인용 게임플레이");
	gotoxy(35, 17);
	printf("2 번 째 메 뉴       >>       2인용 게임플레이");
}
void program_screen() {
	int menu;
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(100, 9);
	printf("by으라차차");
	gotoxy(35, 15);
	printf("1 번 째 메 뉴       >>       튜토리얼 시작");
	gotoxy(35, 17);
	printf("2 번 째 메 뉴       >>       뒤로 돌아가기");
}
void second_menu() {
	int menu;
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(100, 9);
	printf("by으라차차");
	gotoxy(35, 15);
	printf("1 번 째 메 뉴       >>       프로그램 설명");
	gotoxy(35, 17);
	printf("2 번 째 메 뉴       >>       개인 정보 출력");
	gotoxy(35, 19);
	printf("3 번 째 메 뉴       >>       1인용 / 2인용 게임플레이");
	gotoxy(35, 21);
	printf("4 번 째 메 뉴       >>       로 그 아 웃");
}
void first_screen() {
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(90, 20);
	printf("팀 명 : 으  라  차  차");
	gotoxy(90, 22);
	printf("팀 원 : 최 승 진");
	gotoxy(90, 24);
	printf("        양 회 욱");
	gotoxy(90, 26);
	printf("        오 상 헌");
	printf("\n\n");
	getch();
	system("cls");
}
void second_screen() {
	int menu;
	gotoxy(35, 5);
	printf("오        목        프        로        그        램");
	gotoxy(100, 9);
	printf("by으라차차");
	gotoxy(35, 15);
	printf("1 번 째 메 뉴       >>       회 원 가 입");
	gotoxy(35, 17);
	printf("2 번 째 메 뉴       >>       로   그  인");
	gotoxy(35, 19);
	printf("3 번 째 메 뉴       >>       종       료");
	printf("\n\n");
}
void gotoxy(int x, int y) {
	COORD Pos = { x - 1,y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void input_data(_User *user) // 회원가입
{
	int shape_number;
	gotoxy(35, 2);
	printf("사용자의 이름을 입력하시오 : ");
	scanf("%s", user->name);
	gotoxy(35, 4);
	printf("사용자의 나이을 입력하시오 : ");
	scanf("%d", &user->age);
	gotoxy(35, 6);
	printf("사용자의 이메일을 입력하시오 : ");
	scanf("%s", user->e_mail);
	gotoxy(35, 8);
	printf("사용자의 아이디을 입력하시오 : ");
	scanf("%s", &user->nickname);
	gotoxy(35, 10);
	printf("사용자의 패스워드을 입력하시오 : ");
	scanf("%s", &user->password);
	gotoxy(35, 12);
	printf("1.  ○  2.  ☆  3.  ◇  4.  ▽\n");
	gotoxy(35, 14);
	printf("사용자의 오목알모양에 해당되는 번호를 입력해주세요 : ");
	scanf("%d", &shape_number);
	switch (shape_number) {
	case 1:
		strcpy(user->stone_shape1, "○");
		strcpy(user->stone_shape2, "●");
		break;
	case 2:
		strcpy(user->stone_shape1, "☆");
		strcpy(user->stone_shape2, "★");
		break;
	case 3:
		strcpy(user->stone_shape1, "◇");
		strcpy(user->stone_shape2, "◆");
		break;
	case 4:
		strcpy(user->stone_shape1, "▽");
		strcpy(user->stone_shape2, "▼");
		break;
	default:
		break;

	}
}
int insert(MYSQL *conn, _User user)
{
	char buff[255];
	sprintf(buff, "insert into data (name, age, e_mail, nickname, password, stone_shape1,stone_shape2, win, lose, rate) values ('%s', '%d', '%s', '%s','%s','%s','%s', 0, 0, 0);", user.name, user.age, user.e_mail, user.nickname, user.password, user.stone_shape1, user.stone_shape2);
	printf("%s : %p: %d\n", buff, conn, mysql_query(conn, buff));
	fprintf(stderr, "Mysql query error : %s \n", mysql_error(&conn));
}
int login(MYSQL *conn)
{
	int offset = 0;
	int j;
	char buff2[255];
	int state;

	MYSQL_RES   *sql_result;
	MYSQL_ROW   row;
	int field;
	int query_stat;
	int id_number;
	gotoxy(35, 5);
	printf("아 이 디를   입력하시오 >>  ");
	scanf("%s", nickname1);
	gotoxy(35, 7);
	printf("비밀번호를   입력하시오 >>  ");
	scanf("%s", password1); //암호화실패

	sprintf(buff2, "SELECT * FROM data WHERE nickname='%s'AND password ='%s'", nickname1, password1);
	if (mysql_query(conn, "USE op")) {
		printf("%s\n", mysql_error(conn));
		return 1;
	}

	if (mysql_query(conn, buff2)) {
		printf("%s\n", mysql_error(conn));
		return 1;
	}
	sql_result = mysql_store_result(conn);

	if (mysql_affected_rows(conn) != 1) { // 로그인 처리
		logout_fail_screen();
		return 1;
	}
	field = mysql_num_fields(sql_result);

	while ((row = mysql_fetch_row(sql_result))) {
		for (j = 0; j < field; j++) {
			switch (j) {
			case 0:
				user.id = row[0];
				break;
			case 1:
				strcpy(user.name, row[1]);
				break;
			case 2:
				user.age = atoi(row[2]);
				break;
			case 3:
				strcpy(user.e_mail, row[3]);
				break;
			case 4:
				strcpy(user.nickname, row[4]);
				break;
			case 5:
				strcpy(user.password, row[5]);
				break;
			case 6:
				strcpy(user.stone_shape1, row[6]);
				break;
			case 7:
				strcpy(user.stone_shape2, row[7]);
				break;
			case 8:
				user.win = atoi(row[8]);
				break;
			case 9:
				user.lose = atoi(row[9]);
				break;
			case 10:
				user.rate = atoi(row[10]);
				break;
			default:
				break;

			}
		}
		user.rate = Calculate_rate(user.win, user.lose);
		//printf("%12s",row[j]);

		//printf("\n");
	}
	//mysql_free_result(sql_result);
	return 0;
}
int login2(MYSQL *conn)
{
	int offset = 0;
	int j;
	char buff2[255];
	int state;

	MYSQL_RES   *sql_result;
	MYSQL_ROW   row;
	int field;
	int query_stat;
	int id_number;
	gotoxy(35, 5);
	printf("아 이 디를   입력하시오 >>  ");
	scanf("%s", nickname2);
	gotoxy(35, 7);
	printf("비밀번호를   입력하시오 >>  ");
	scanf("%s", password2); //암호화실패

	sprintf(buff2, "SELECT * FROM data WHERE nickname='%s'AND password ='%s'", nickname2, password2);
	if (mysql_query(conn, "USE op")) {
		printf("%s\n", mysql_error(conn));
		return 1;
	}

	if (mysql_query(conn, buff2)) {
		printf("%s\n", mysql_error(conn));
		return 1;
	}
	sql_result = mysql_store_result(conn);

	if (mysql_affected_rows(conn) != 1) { // 로그인 처리
		logout_fail_screen();
		return 1;
	}
	field = mysql_num_fields(sql_result);

	while ((row = mysql_fetch_row(sql_result))) {
		for (j = 0; j < field; j++) {
			switch (j) {
			case 0:
				user2.id = row[0];
				break;
			case 1:
				strcpy(user2.name, row[1]);
				break;
			case 2:
				user2.age = atoi(row[2]);
				break;
			case 3:
				strcpy(user2.e_mail, row[3]);
				break;
			case 4:
				strcpy(user2.nickname, row[4]);
				break;
			case 5:
				strcpy(user2.password, row[5]);
				break;
			case 6:
				strcpy(user2.stone_shape1, row[6]);
				break;
			case 7:
				strcpy(user2.stone_shape2, row[7]);
				break;
			case 8:
				user2.win = atoi(row[8]);
				break;
			case 9:
				user2.lose = atoi(row[9]);
				break;
			case 10:
				user2.rate = Calculate_rate(user2.win, user2.lose);
				break;
			default:
				break;

			}
		}

		//printf("%12s",row[j]);

		//printf("\n");
	}
	//mysql_free_result(sql_result);
	return 0;
}
int Calculate_rate(int a, int b) {//a = 승, b= 패
	int result = 0;
	if (a == 0 && b == 0)
		return 0;
	else if (a == 0 && b>0)
		return 0;
	else if (a > 0 && b == 0)
		return 100;
	else {
		result = (a * 100) / (a + b);
		return result;
	}
}
int delete(MYSQL *conn) {
	char buff[255];
	char nickname[30];
	printf("삭제할 아이디를   입력하시오 >>  ");
	scanf("%s", nickname);
	sprintf(buff, "DELETE from data where nickname = '%s'", nickname);
	printf("%s : %p: %d\n", buff, conn, mysql_query(conn, buff));
	//printf("%s\n", mysql_error(conn));
}
int print(MYSQL *conn)
{
	int k;
	MYSQL_RES *sql_result;
	MYSQL_ROW row;
	int field;
	mysql_query(conn, "select * from data");
	sql_result = mysql_store_result(conn);
	field = mysql_num_fields(sql_result);
	printf("%12s%12s%12s%12s%12s%12s%12s%12s%12s%12s%12s\n", "id", "name", "age", "e_mail", "nickname", "password", "shape1", "shape2", "win", "lose", "rate");
	while ((row = mysql_fetch_row(sql_result))) {
		for (k = 0; k<field; k++)
			printf("%12s", row[k]);
		printf("\n");
	}
}
void zero_screen() {

	gotoxy(34, 5);
	printf("■■■■■■■"); printf("      ■■■■■■■   "); printf("   ■■■■■■■\n");
	gotoxy(34, 6);
	printf("■          ■"); printf("            ■         ");       printf("   ■          ■\n");
	gotoxy(34, 7);
	printf("■          ■"); printf("            ■         ");	   printf("   ■          ■\n");
	gotoxy(34, 8);
	printf("■          ■"); printf("            ■         ");       printf("   ■■■■■■■\n");
	gotoxy(34, 9);
	printf("■          ■"); printf("            ■         ");       printf("   ■\n");
	gotoxy(34, 10);
	printf("■          ■"); printf("            ■         ");       printf("   ■\n");
	gotoxy(34, 11);
	printf("■          ■"); printf("            ■         ");       printf("   ■\n");
	gotoxy(34, 12);
	printf("■■■■■■■"); printf("            ■         ");       printf("   ■\n");


	gotoxy(1, 25);
	printf("  ●●                                                                                                         ");   printf(" ●●\n");
	gotoxy(1, 26);
	printf("●●●●                                                                                                      "); printf("●    ●\n");
	gotoxy(1, 27);
	printf("●●●●                                                                                                      "); printf("●    ●\n");
	gotoxy(1, 28);
	printf("  ●●                                                                                                         ");   printf(" ●●\n");
	gotoxy(1, 29);
	getch();
	system("cls");
}


void draw_rectangle(int r, int c)
{
	int i, j;
	unsigned char a = 0xa6;
	unsigned char b[7];

	for (i = 1; i < 7; i++)
		b[i] = 0xa0 + i;

	printf("%c%c", a, b[3]);

	for (i = 0; i < c; i++)
		printf("%c%c", a, b[1]);

	printf("%c%c", a, b[4]);
	printf("\n");

	for (i = 0; i < r; i++)
	{
		printf("%c%c", a, b[2]);

		for (j = 0; j < c; j++)
			printf("  ");

		printf("%c%c", a, b[2]);
		printf("\n");
	}

	printf("%c%c", a, b[6]);

	for (i = 0; i<c; i++)
		printf("%c%c", a, b[1]);

	printf("%c%c", a, b[5]);
	printf("\n");
}
int turn_two_players(int n)
{
	if (n % 2 == 1) // 홀수
		return 1;

	else          // 짝수
		return 0;
}


void game_table_tutorial(Board *board)
{
	srand(time(NULL));
	int n;
	char stone[3];
	int i, j;

	draw_rectangle(25, 35);

	draw_omok();

	for (i = 0; i < MAX_LINE; i++)
	{
		for (j = 0; j < MAX_LINE; j++)
		{
			gotoxy(i * 2 + 22, j + 7);

			if (board_isSet(board, i, j))
				printf("●");
		}
	}

	gotoxy(80, 7);
	printf("player1 : 알파고 (●)\n");
	gotoxy(80, 8);
	printf("player2 : 이세돌 (○)\n");
	gotoxy(80, 9);
	printf("==============================\n");
	gotoxy(80, 10);
	printf("player1의 차례입니다. ");
	gotoxy(80, 11);
	printf("제한시간 : 30");
}

void Game_Tutorial(Board* board)
{
	int num, i, j;

	printf("안녕하세요 튜토리얼을 시작하겠습니다.\n");
	getch();
	system("cls");
	printf("먼저 게임기능에 대해 설명드리겠습니다.\n\n");

	game_table_tutorial(board);

	gotoxy(53, 13);     //gotoxy문을 사용해서 화살표로 오목판을 가리킨다
	printf("↖");

	gotoxy(53, 14);
	printf("이것은 오목판 입니다\n");
	gotoxy(53, 15);
	printf("여기에서 오목게임이 진행이 되어집니다.\n");

	getch();
	system("cls");

	printf("\n\n");

	game_table_tutorial(board);

	move_point();           //화살표를 이동해서 상황판으로 간다
	gotoxy(78, 14);
	printf("이것은 상황판 입니다.\n");
	gotoxy(78, 15);
	printf("게임플레이어가 누구인지, 누구의 차례인지\n");
	gotoxy(78, 16);
	printf("제한시간이 얼마나 남았는지를 알려줍니다.\n");
	getch();

	system("cls");

	game_table_tutorial(board);

	printf("\n");
	gotoxy(80, 14);
	printf("그럼 이제 오목알을 놓아보겠습니다.");
	gotoxy(80, 15);
	printf("화살표를 이용하여 이동하고.");
	gotoxy(80, 17);
	printf("스페이스바를 이용하여");
	gotoxy(80, 18);
	printf("오목알을 놓을 지점을 정합니다.\n");

	x_c = 36, y_c = 13;
	// 스페이스를 입력받으면 다음 화면으로 넘어간다
	gotoxy(x_c, y_c);
	move_control_tuto(board);

	//if(스페이스바를 눌렀을때 다음 명령문 출력)
	system("cls");
	game_table_tutorial(board);

	gotoxy(80, 14);
	printf("그럼 ");
	gotoxy(80, 15);
	printf("오목알을 가로 혹은 세로 혹은 대각선으로");
	gotoxy(80, 16);
	printf("다섯알을 놓아보겠습니다.\n");

	do
	{
		move_control_tuto(board);

		draw_omok();

		for (i = 0; i < MAX_LINE; i++)
		{
			for (j = 0; j < MAX_LINE; j++)
			{
				gotoxy(i * 2 + 22, j + 7);

				if (board_isSet(board, i, j))
					printf("●");

			}
		}
	} while (check_board_complete(board) != true);

	gotoxy(80, 18);
	printf("잘하셨습니다.\n");
	gotoxy(80, 19);
	printf("이렇게 다섯알을 놓으면");
	gotoxy(80, 20);
	printf("게임에서 승리하게 됩니다.\n");

	getch();
	system("cls");
	gotoxy(30, 8);
	printf("게임에 대한 이해가 조금 되셨나요?\n");
	gotoxy(30, 10);
	printf("수고하셨습니다^^\n");

	gotoxy(1, 20);

}

void game_table_com() {
	Turn turn = Black;
	Node *node = NULL;

	node = node_init(node);

	srand(time(NULL));

	x_c = 36, y_c = 13;

	draw_rectangle(25, 35);
	ready();

	while (1) {
		print_board_com(node->boardBlack, node->boardWhite);

		gotoxy(80, 7);
		printf("player1 : %s (%s)\n", user.name, user.stone_shape2);
		gotoxy(80, 8);
		printf("player2 : %s (%s)\n", "COM", "○");
		gotoxy(80, 9);
		printf("==============================\n");

		thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

		if (turn == Black) {
			// user

			if (timeover == 1)
			{
				print_board_com(node->boardBlack, node->boardWhite);
				gotoxy(80, 13);
				printf("player1이 승리하셨습니다");
				timeover = 0;
				break;
			}

			gotoxy(80, 10);
			printf("player1의 차례입니다.         ");
			gotoxy(80, 11);
			printf("제한시간 : ");
			move_control(node->boardBlack, node->boardWhite);
			turn = White;

		}
		else {
			// com

			if (timeover == 1)
			{
				print_board_com(node->boardBlack, node->boardWhite);
				gotoxy(80, 13);
				printf("player2이 승리하셨습니다.");
				timeover = 0;
				break;
			}
			gotoxy(80, 10);
			printf("COM의 차례입니다.          ");
			gotoxy(80, 11);
			printf("제한시간 :");

			node = findNext(node);
			turn = Black;
			TerminateThread(thread, 0);
		}

		if (check_board_complete(node->boardBlack) == true)
		{
			print_board_com(node->boardBlack, node->boardWhite);
			gotoxy(80, 13);
			printf("player1이 승리하셨습니다.");
			break;
		}

		if (check_board_complete(node->boardWhite) == true)
		{
			print_board_com(node->boardBlack, node->boardWhite);
			gotoxy(80, 13);
			printf("COM이 승리하셨습니다.");
			break;
		}
	}
}

void game_table(MYSQL *conn)
{

	Board* board_1 = makeBoard((ll)0, (ll)0, (ll)0, (ll)0);
	Board* board_2 = makeBoard((ll)0, (ll)0, (ll)0, (ll)0);

	char buff1[255];
	char buff2[255];
	char buff3[255];

	int n, first_n;
	int i, j;

	srand(time(NULL));
	n = rand() % 2;
	first_n = n;

	x_c = 36, y_c = 13;

	draw_rectangle(25, 35);

	if (n % 2 == 1)
	{
		while (1)
		{
			print_board(board_1, board_2, first_n);

			gotoxy(80, 7);
			printf("player1 : %s (%s)\n", user.name, user.stone_shape2);
			gotoxy(80, 8);
			printf("player2 : %s (%s)\n", user2.name, user2.stone_shape1);
			gotoxy(80, 9);
			printf("==============================\n");

			if (turn_two_players(n) == 1)
			{
				thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

				if (timeover == 1)
				{
					print_board(board_1, board_2, first_n);
					gotoxy(80, 13);
					printf("player1이 승리하셨습니다\n");
					timeover = 0;
					user.win++;
					user2.lose++;
					sprintf(buff1, "update data set win = win + 1 where name = '%s'", user.name);
					mysql_query(conn, buff1);
					sprintf(buff1, "update data set lose = lose + 1 where name='%s'", user2.name);
					mysql_query(conn, buff1);
					break;
				}

				gotoxy(80, 10);
				printf("player1의 차례입니다. ");
				gotoxy(80, 11);
				printf("제한시간 : ");
				move_control(board_1, board_2);
				n++;
			}

			else
			{
				thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

				if (timeover == 1)
				{
					print_board(board_1, board_2, first_n);
					gotoxy(80, 13);
					printf("player2이 승리하셨습니다\n");
					timeover = 0;
					user2.win++;
					user.lose++;
					sprintf(buff1, "update data set lose = lose + 1 where name = '%s'", user.name);
					mysql_query(conn, buff1);
					sprintf(buff1, "update data set win = win + 1 where name='%s'", user2.name);
					mysql_query(conn, buff1);
					break;
				}
				gotoxy(80, 10);
				printf("player2의 차례입니다. ");
				gotoxy(80, 11);
				printf("제한시간 :");
				move_control(board_2, board_1);
				n++;
			}

			if (check_board_complete(board_1) == true)
			{
				print_board(board_1, board_2, first_n);
				gotoxy(80, 13);
				printf("player1이 승리하셨습니다\n");
				user.win++;
				user2.lose++;
				sprintf(buff1, "update data set win = win + 1 where name = '%s'", user.name);
				mysql_query(conn, buff1);
				sprintf(buff1, "update data set lose = lose + 1 where name='%s'", user2.name);
				mysql_query(conn, buff1);
				break;
			}

			if (check_board_complete(board_2) == true)
			{
				print_board(board_1, board_2, first_n);
				gotoxy(80, 13);
				printf("player2이 승리하셨습니다\n");
				user2.win++;
				user.lose++;
				sprintf(buff1, "update data set lose = lose + 1 where name = '%s'", user.name);
				mysql_query(conn, buff1);
				sprintf(buff1, "update data set win = win + 1 where name='%s'", user2.name);
				mysql_query(conn, buff1);
				break;
			}

		}
		sprintf(buff1, "update data set rate = win*100/(win+lose) where name='%s'", user.name);
		mysql_query(conn, buff1);
		sprintf(buff1, "update data set rate = win*100/(win+lose) where name='%s'", user2.name);
		mysql_query(conn, buff1);
	}
	else
	{
		while (1)
		{
			print_board(board_1, board_2, first_n);

			gotoxy(80, 7);
			printf("player1 : %s (%s)\n", user.name, user.stone_shape1);
			gotoxy(80, 8);
			printf("player2 : %s (%s)\n", user2.name, user2.stone_shape2);
			gotoxy(80, 9);
			printf("==============================\n");

			if (turn_two_players(n) == 1)
			{
				thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

				if (timeover == 1)
				{
					print_board(board_1, board_2, first_n);
					gotoxy(80, 13);
					printf("player1이 승리하셨습니다\n");
					timeover = 0;
					user.win++;
					user2.lose++;
					sprintf(buff1, "update data set win = win + 1 where name = '%s'", user.name);
					mysql_query(conn, buff1);
					sprintf(buff1, "update data set lose = lose + 1 where name='%s'", user2.name);
					mysql_query(conn, buff1);
					break;
				}
				gotoxy(80, 10);
				printf("player1의 차례입니다. ");
				gotoxy(80, 11);
				printf("제한시간 : ");
				move_control(board_1, board_2);

				n++;

			}

			else
			{
				thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

				if (timeover == 1)
				{
					print_board(board_1, board_2, first_n);
					gotoxy(80, 13);
					printf("player2이 승리하셨습니다\n");
					timeover = 0;
					user2.win++;
					user.lose++;
					sprintf(buff1, "update data set lose = lose + 1 where name = '%s'", user.name);
					mysql_query(conn, buff1);
					sprintf(buff1, "update data set win = win + 1 where name='%s'", user2.name);
					mysql_query(conn, buff1);
					break;
				}

				gotoxy(80, 10);
				printf("player2의 차례입니다. ");
				gotoxy(80, 11);
				printf("제한시간 :");
				move_control(board_2, board_1);

				n++;
			}

			if (check_board_complete(board_1) == true)
			{
				print_board(board_1, board_2, first_n);
				gotoxy(80, 13);
				printf("player1이 승리하셨습니다\n");
				user.win++;
				user2.lose++;
				sprintf(buff1, "update data set win = win + 1 where name = '%s'", user.name);
				mysql_query(conn, buff1);
				sprintf(buff1, "update data set lose = lose + 1 where name='%s'", user2.name);
				mysql_query(conn, buff1);
				break;
			}

			if (check_board_complete(board_2) == true)
			{
				print_board(board_1, board_2, first_n);
				gotoxy(80, 13);
				printf("player2이 승리하셨습니다\n");
				user2.win++;
				user.lose++;
				sprintf(buff1, "update data set lose = lose + 1 where name = '%s'", user.name);
				mysql_query(conn, buff1);
				sprintf(buff1, "update data set win = win + 1 where name='%s'", user2.name);
				mysql_query(conn, buff1);
				break;
			}
		}
		sprintf(buff1, "update data set rate = win*100/(win+lose) where name='%s'", user.name);
		mysql_query(conn, buff1);
		sprintf(buff1, "update data set rate = win*100/(win+lose) where name='%s'", user2.name);
		mysql_query(conn, buff1);

	}
}
void move_point()
{
	int i;
	for (i = 53; i < 110; i++)
	{
		gotoxy(i, 13);
		printf(" ↖");

		Sleep(20);
	}
}

void move_control(Board* board_A, Board* board_B)
{
	char key;
	int put = 0;

	do
	{
		key = getch();

		if (count_time < 1)
		{
			break;
		}

		switch (key)
		{
		case 32:
			//printf("%d %d\n", x_c, y_c);
			if (board_isSet(board_A, (x_c - 22) / 2, y_c - 7) || board_isSet(board_B, (x_c - 22) / 2, y_c - 7))
				continue;
			*board_A = *(board_set(board_A, (x_c - 22) / 2, y_c - 7)); /*수를 놓았을때 표현*/
			put = 1;
			break;
		case 72:
			y_c--;
			if (y_c < 1)
				y_c = 1;
			break;
		case 75:
			x_c -= 2;
			if (x_c < 1)
				x_c = 1;
			break;
		case 77:
			x_c += 2;
			break;
		case 80:
			y_c++;
			break;
		}

		gotoxy(x_c, y_c);
	} while (put != 1);

	if (key == 32)
	{
		TerminateThread(thread, 0);
	}
}

void print_board_com(Board* boardBlack, Board* boardWhite) {
	int i, j;

	draw_omok();

	for (i = 0; i < MAX_LINE; i++)
	{
		for (j = 0; j < MAX_LINE; j++)
		{
			gotoxy(i * 2 + 22, j + 7);

			if (board_isSet(boardBlack, i, j)) {
				printf("%s", user.stone_shape2);
			}

			if (board_isSet(boardWhite, i, j)) {
				printf("○");
			}
		}
	}
}

void print_board(Board* board_1, Board* board_2, int n)
{
	draw_omok();

	int i, j;

	for (i = 0; i < MAX_LINE; i++)
	{
		for (j = 0; j < MAX_LINE; j++)
		{
			gotoxy(i * 2 + 22, j + 7);

			if (n % 2 == 1)
			{
				if (board_isSet(board_1, i, j))
					printf("%s", user.stone_shape2); // user오목알

				if (board_isSet(board_2, i, j))
					printf("%s", user2.stone_shape1); // user2오목알
			}

			else
			{
				if (board_isSet(board_1, i, j))
					printf("%s", user.stone_shape1); // user오목알

				if (board_isSet(board_2, i, j))
					printf("%s", user2.stone_shape2); // user2오목알
			}
		}
	}
}

void move_control_tuto(Board* board)
{
	char key;
	int put = 0;

	do
	{
		key = getch();

		switch (key)
		{
		case 32:
			if (board_isSet(board, (x_c - 22) / 2, y_c - 7))
				continue;
			//*board = *(board_set(board, (x_c - 22) / 2, y_c - 7)); /*수를 놓았을때 표현*/
			*board = *(board_set(board, (x_c - 22) / 2, y_c - 7));
			put = 1;
			break;
		case 72:
			y_c--;
			if (y_c < 1)
				y_c = 1;
			break;
		case 75:
			x_c -= 2;
			if (x_c < 1)
				x_c = 1;
			break;
		case 77:
			x_c += 2;
			break;
		case 80:
			y_c++;
			break;
		}

		gotoxy(x_c, y_c);
	} while (put != 1);
}

DWORD WINAPI ThreadFunc(void *data)
{
	count_time = 30;
	time_t startTime, endTime;

	startTime = time(NULL);
	while (1)
	{
		endTime = time(NULL);

		if (endTime - startTime >= 1) {
			startTime = endTime;
			count_time--;
			gotoxy(92, 11);

			printf("%d        ", count_time);
			gotoxy(x_c, y_c);

			if (count_time < 0) {
				gotoxy(92, 11);
				printf("시간초과!!!\n");

				timeover++;

				return 0;
			}
		}
	}

	return 0;
}

void draw_omok()
{
	int i, j;
	unsigned char a = 0xa6;
	unsigned char b[13];

	for (i = 1; i < 12; i++)
		b[i] = 0xa0 + i;

	gotoxy(22, 7);
	printf("%c%c", a, b[3]);

	for (i = 0; i < 13; i++)
	{
		gotoxy(i * 2 + 24, 7);
		printf("%c%c", a, b[8]);

	}
	printf("%c%c", a, b[4]);
	printf("\n");

	///

	for (i = 0; i < 13; i++)
	{
		gotoxy(22, 8 + i);
		printf("%c%c", a, b[7]);

		for (j = 0; j < 13; j++)
		{
			gotoxy(j * 2 + 24, 8 + i);
			printf("%c%c", a, b[11]);
		}

		gotoxy(50, 8 + i);
		printf("%c%c", a, b[9]);
		printf("\n");
	}

	////
	gotoxy(22, 21);
	printf("%c%c", a, b[6]);

	for (i = 0; i < 13; i++)
	{
		gotoxy(i * 2 + 24, 21);
		printf("%c%c", a, b[10]);
	}

	gotoxy(50, 21);
	printf("%c%c", a, b[5]);
	printf("\n");
}

void ready() {
	if (realRoot == NULL) {
		realRoot = node_init(realRoot);
		realRoot->boundWhite.max_x = 8;
		realRoot->boundWhite.min_x = 6;
		realRoot->boundWhite.max_y = 8;
		realRoot->boundWhite.min_y = 6;
		realRoot->boundBlack.max_x = 8;
		realRoot->boundBlack.min_x = 6;
		realRoot->boundBlack.max_y = 8;
		realRoot->boundBlack.min_y = 6;
	}

	MCTS(realRoot);

	tmpRoot = realRoot;
}

Node* findNext(Node* node) {
	int i;
	Node *child = NULL, *grandChild, *bestChild;
	double bestValue = 0.0f, tmpValue;


	child = findChildSameBoard(tmpRoot, node->boardBlack, node->boardWhite);
	if (child == NULL)
		toFind = node;

	MCTS(tmpRoot);

	child = findChildSameBoard(tmpRoot, node->boardBlack, node->boardWhite);


	for (i = 0; i < child->nodes->size; i++) {
		grandChild = child->nodes->array + i;
		tmpValue = (grandChild->N == 0) ? 0.0f : (double)grandChild->win / grandChild->N;

		if (tmpValue >= bestValue) {
			bestChild = grandChild;
			bestValue = tmpValue;
		}
	}

	tmpRoot = bestChild;
	return node_copy(tmpRoot, false);
}

Node* findChildSameBoard(Node* node, Board* black, Board* white) {
	int i;
	bool found = false;
	Node *child;

	for (i = 0; i < node->nodes->size; i++) {
		child = node->nodes->array + i;
		if ((board_isEqual(child->boardBlack, black) == true) && (board_isEqual(child->boardWhite, white) == true)) {
			found = true;
			break;
		}
	}

	if (found == true)
		return child;
	else
		return NULL;
}

void MCTS(Node *root) {

	ll startTime, endTime;

	srand(time(NULL));
	startTime = time(NULL);
	while (1) {

		// time out check to break while loop.
		endTime = time(NULL);
		if ((endTime - startTime) >= MAX_SEARCH_TIME_IN_SEC) break;

		simulate(root);
	}
}

int simulate(Node *node) {
	Node *randomMove = NULL;
	int result;

	if (check_board_complete(node->boardBlack)) {
		return 0;
	}
	else if (check_board_complete(node->boardWhite)) {
		return 1;
	}

	randomMove = selectHeuristic(node);
	if (randomMove == (Node*)NULL) {
		return 0;
	}
	else {
		result = simulate(randomMove);
		result = ((result == 0) ? 1 : 0);
		node->win = node->win + result;
		node->N = node->N + 1;
		return result;
	}
}

Node* selectHeuristic(Node *root) {
	int x, y;
	int i;
	Node *selectedNode = NULL;
	Node *childNode = NULL;
	Turn childTurn = (root->turn == Black) ? White : Black;
	Board *selectedBoard = NULL;
	int min_x, min_y, max_x, max_y;
	//time_t startTime, endTime;

	// limit boundary of oposite player's bound.
	if (childTurn == White) {
		min_x = root->boundBlack.min_x;
		min_y = root->boundBlack.min_y;
		max_x = root->boundBlack.max_x;
		max_y = root->boundBlack.max_y;
	}
	else {
		min_x = root->boundWhite.min_x;
		min_y = root->boundWhite.min_y;
		max_x = root->boundWhite.max_x;
		max_y = root->boundWhite.max_y;
	}

	while (1) {
		if (toFind != NULL) {
			if (childTurn == Black)
				board_find_diff_one(toFind->boardBlack, root->boardBlack, &x, &y);
			else
				board_find_diff_one(toFind->boardWhite, root->boardWhite, &x, &y);
		}
		else {
			x = (rand() % (max_x - min_x + 1)) + min_x;
			y = (rand() % (max_y - min_y + 1)) + min_y;
		}


		if (board_isSet(root->boardBlack, y, x) || board_isSet(root->boardWhite, y, x))
			continue;

		if (toFind != NULL) toFind = NULL;

		if (root->nodes != NULL) {
			for (i = 0; i < root->nodes->size; i++) {
				childNode = (root->nodes->array + i);
				selectedBoard = (childTurn == Black) ? childNode->boardBlack : childNode->boardWhite;

				// when there is matched child.
				if (board_isSet(selectedBoard, y, x))
					return childNode;
			}
		}

		// When there is no matchted child with proper x, y.
		selectedNode = node_copy(root, false);
		if (childTurn == Black) selectedNode->boardBlack = board_set(selectedNode->boardBlack, y, x);
		else selectedNode->boardWhite = board_set(selectedNode->boardWhite, y, x);
		setBound(selectedNode, x, y, childTurn);
		selectedNode->turn = childTurn;
		selectedNode->N = 0;
		selectedNode->win = 0;

		narray_insert(root->nodes, *selectedNode);
		return selectedNode;
	}
}

Node* node_init(Node* node) {
	node = (Node*)malloc(sizeof(Node));
	node->boardWhite = makeEmptyBoard();
	node->boardBlack = makeEmptyBoard();
	node->win = 0;
	node->N = 0;
	node->turn = White;
	node->nodes = narray_init();
	node->boundWhite.max_x = 0;
	node->boundWhite.max_y = 0;
	node->boundWhite.min_x = 0;
	node->boundWhite.min_y = 0;
	node->boundBlack.max_x = 0;
	node->boundBlack.max_y = 0;
	node->boundBlack.min_x = 0;
	node->boundBlack.min_y = 0;
	return node;
}

Node* node_copy(Node* target, bool liveChild) {
	Node *node = (Node*)malloc(sizeof(Node));
	node->boardWhite = board_copy(target->boardWhite);
	node->boardBlack = board_copy(target->boardBlack);
	node->win = target->win;
	node->N = target->N;
	node->turn = target->turn;
	if (liveChild == true)
		node->nodes = narray_copy(target->nodes);
	else
		node->nodes = narray_init();
	node->boundBlack.max_x = target->boundBlack.max_x;
	node->boundBlack.min_x = target->boundBlack.min_x;
	node->boundBlack.max_y = target->boundBlack.max_y;
	node->boundBlack.min_y = target->boundBlack.min_y;
	node->boundWhite.max_x = target->boundWhite.max_x;
	node->boundWhite.min_x = target->boundWhite.min_x;
	node->boundWhite.max_y = target->boundWhite.max_y;
	node->boundWhite.min_y = target->boundWhite.min_y;
	return node;
}

void setBound(Node* node, int x, int y, Turn turn) {
	if (turn == Black) {
		node->boundBlack.max_x = min(max(node->boundBlack.max_x, x + 1), 14);
		node->boundBlack.min_x = max(min(node->boundBlack.min_x, x - 1), 0);
		node->boundBlack.max_y = min(max(node->boundBlack.max_y, y + 1), 14);
		node->boundBlack.min_y = max(min(node->boundBlack.min_y, y - 1), 0);
	}
	else {
		node->boundWhite.max_x = min(max(node->boundWhite.max_x, x + 1), 14);
		node->boundWhite.min_x = max(min(node->boundWhite.min_x, x - 1), 0);
		node->boundWhite.max_y = min(max(node->boundWhite.max_y, y + 1), 14);
		node->boundWhite.min_y = max(min(node->boundWhite.min_y, y - 1), 0);
	}

}

bool check_board_complete(Board* board) {
	int i, j;
	bool isComplete = false;

	Board* boardHorizontal = makeBoard((ll)0, (ll)2181431069507584, (ll)0, (ll)0);  //가로 직선
	Board* boardReverseSlash = makeBoard((ll)0, (ll)281479271743488, (ll)65537, (ll)0);  //역슬래시
	Board* boardVertical = makeBoard((ll)0, (ll)9223653520421683200, (ll)16384, (ll)0);   //세로 직선
	Board* boardSlash = makeBoard((ll)0, (ll)4611967510585016320, (ll)4096, (ll)0);   //슬래시

	isComplete = check_board_mask(board, boardHorizontal);
	if (isComplete) return true;

	isComplete = check_board_mask(board, boardReverseSlash);
	if (isComplete) return true;

	isComplete = check_board_mask(board, boardVertical);
	if (isComplete) return true;

	isComplete = check_board_mask(board, boardSlash);
	if (isComplete) return true;

	return isComplete;
}

bool check_board_mask(Board* board, Board* mask) {
	int i;
	int max = MAX_LINE * MAX_LINE;
	int mid = max / 2;
	bool result;
	Board *checker;
	Board *target = board_copy(board);

	for (i = 0; i < max; i++) {
		if (i < mid) {
			checker = board_shift_left(mask, mid - i);
			result = board_isEqual(board_shift_right(board_and(target, checker), mid - i), mask);
		}
		else if (i == mid) {
			result = board_isEqual(board_and(target, mask), mask);
		}
		else {
			checker = board_shift_right(mask, i - mid);
			result = board_isEqual(board_shift_left(board_and(target, checker), i - mid), mask);

		}

		if (result == true) return true;
	}


	return false;
}

Board* makeBoard(ll A, ll B, ll C, ll D) {
	Board* board = (Board*)malloc(sizeof(Board));
	board->OM1 = A;
	board->OM2 = B;
	board->OM3 = C;
	board->OM4 = D;

	board_mask(board);

	return board;
}

/* It's Same As makeBoard(0, 0, 0, 0)*/
Board* makeEmptyBoard() {
	Board* board = (Board*)malloc(sizeof(Board));
	board->OM1 = 0ULL;
	board->OM2 = 0ULL;
	board->OM3 = 0ULL;
	board->OM4 = 0ULL;

	return board;
}

//operators
Board* board_mask(Board* board) {
	static const ll mask = (~0ULL) >> 31;

	board->OM4 = (board->OM4) & mask;

	return board;
}

Board* board_shift_left(Board* board, int shift) {
	int bigMove = shift / MAX_BIT_CNT;
	int smallMove = shift % MAX_BIT_CNT;
	ll mask = (ll)~0;
	ll tmp;
	Board* target = board_copy(board);

	while (bigMove-- > 0) {
		target->OM1 = target->OM2;
		target->OM2 = target->OM3;
		target->OM3 = target->OM4;
		target->OM4 = 0ULL;
	}

	if (smallMove > 0) {
		mask = mask >> (MAX_BIT_CNT - smallMove);
		target->OM1 = (target->OM1 >> smallMove) | ((mask & target->OM2) << (MAX_BIT_CNT - smallMove));
		target->OM2 = (target->OM2 >> smallMove) | ((mask & target->OM3) << (MAX_BIT_CNT - smallMove));
		target->OM3 = (target->OM3 >> smallMove) | ((mask & target->OM4) << (MAX_BIT_CNT - smallMove));
		target->OM4 = (target->OM4 >> smallMove);
	}

	board_mask(target);

	return target;
}

Board* board_shift_right(Board* board, int shift) {
	int bigMove = shift / MAX_BIT_CNT;
	int smallMove = shift % MAX_BIT_CNT;
	ll mask = (ll)~0;
	ll tmp;
	Board* target = board_copy(board);

	while (bigMove-- > 0) {
		target->OM4 = target->OM3;
		target->OM3 = target->OM2;
		target->OM2 = target->OM1;
		target->OM1 = 0ULL;
	}

	if (smallMove > 0) {
		mask = mask << MAX_BIT_CNT - smallMove;
		target->OM4 = (target->OM4 << smallMove) | ((mask & target->OM3) >> (MAX_BIT_CNT - smallMove));
		target->OM3 = (target->OM3 << smallMove) | ((mask & target->OM2) >> (MAX_BIT_CNT - smallMove));
		target->OM2 = (target->OM2 << smallMove) | ((mask & target->OM1) >> (MAX_BIT_CNT - smallMove));
		target->OM1 = (target->OM1 << smallMove);
	}

	board_mask(target);

	return target;
}

Board* board_and(Board* baseBoard, Board* layerBoard) {

	Board* target = board_copy(baseBoard);

	target->OM1 = target->OM1 & layerBoard->OM1;
	target->OM2 = target->OM2 & layerBoard->OM2;
	target->OM3 = target->OM3 & layerBoard->OM3;
	target->OM4 = target->OM4 & layerBoard->OM4;

	board_mask(target);
	return target;
}

Board* board_or(Board* baseBoard, Board* layerBoard) {

	Board* target = board_copy(baseBoard);

	target->OM1 = target->OM1 | layerBoard->OM1;
	target->OM2 = target->OM2 | layerBoard->OM2;
	target->OM3 = target->OM3 | layerBoard->OM3;
	target->OM4 = target->OM4 | layerBoard->OM4;

	board_mask(target);
	return target;
}

Board* board_xor(Board* baseBoard, Board* layerBoard) {

	Board* target = board_copy(baseBoard);

	target->OM1 = target->OM1 ^ layerBoard->OM1;
	target->OM2 = target->OM2 ^ layerBoard->OM2;
	target->OM3 = target->OM3 ^ layerBoard->OM3;
	target->OM4 = target->OM4 ^ layerBoard->OM4;

	board_mask(target);
	return target;
}

Board* board_not(Board* board) {
	Board* target = board_copy(board);

	target->OM1 = ~(target->OM1);
	target->OM2 = ~(target->OM2);
	target->OM3 = ~(target->OM3);
	target->OM4 = ~(target->OM4);

	board_mask(target);
	return target;
}

Board* board_copy(Board* board) {
	return makeBoard(board->OM1, board->OM2, board->OM3, board->OM4);
}

Board* board_set(Board* board, int i, int j) {
	ll mask = 1ULL;
	int which = 0;
	int shift;
	int toMove = i * MAX_LINE + j;
	Board* target = board_copy(board);

	if (0 <= j && j < MAX_LINE && 0 <= i && i < MAX_LINE) {
		which = toMove / MAX_BIT_CNT;
		shift = toMove % MAX_BIT_CNT;

		mask = mask << shift;

		switch (which) {
		case 0:
			target->OM1 = target->OM1 | mask;
			break;
		case 1:
			target->OM2 = target->OM2 | mask;
			break;
		case 2:
			target->OM3 = target->OM3 | mask;
			break;
		case 3:
			if (shift <= 32)
				target->OM4 = target->OM4 | mask;
			else
				printf("Error Occur In Board Set : Out Of Index\n");
			break;
		default:
			printf("Error Occur In Board Set : Out Of Index\n");
			break;
		}
	}
	else {
		printf("Error Occur In Board Set : Out Of Index\n");
	}
	return target;
}

bool board_isSet(Board* board, int i, int j) {
	ll mask = 1ULL;
	ll target;
	int which = 0;
	int shift;
	int toMove = i * MAX_LINE + j;

	if (0 <= j && j < MAX_LINE && 0 <= i && i < MAX_LINE) {
		which = toMove / MAX_BIT_CNT;
		shift = toMove % MAX_BIT_CNT;

		mask = mask << shift;

		switch (which) {
		case 0:
			target = board->OM1;
			break;
		case 1:
			target = board->OM2;
			break;
		case 2:
			target = board->OM3;
			break;
		case 3:
			target = board->OM4;
			break;
		default:
			printf("Error Occur In Board Set : Out Of Index\n");
			break;
		}
	}
	else {
		printf("Error Occur In Board Set : Out Of Index\n");
	}


	return (target & mask) != 0ULL;
}

bool board_isEmpty(Board* board) {
	return (board->OM1 == 0ULL) && (board->OM2 == 0ULL) && (board->OM3 == 0ULL) && (board->OM4 == 0ULL);
}

bool board_isEqual(Board* boardA, Board* boardB) {
	return (boardA->OM1 == boardB->OM1) && (boardA->OM2 == boardB->OM2) && (boardA->OM3 == boardB->OM3) && (boardA->OM4 == boardB->OM4);
}

void board_find_diff_one(Board* bigger, Board* smaller, int *x, int *y) {
	int i, j;
	for (i = 0; i < MAX_LINE; i++) {
		for (j = 0; j < MAX_LINE; j++) {
			if (board_isSet(bigger, i, j) == true && board_isSet(smaller, i, j) == false) {
				*x = j;
				*y = i;
				return;
			}
		}
	}
}

NodeArray* narray_init() {
	NodeArray *array = (NodeArray*)malloc(sizeof(NodeArray));
	array->array = (Node*)malloc(NODE_ARRAY_INIT_SIZE * sizeof(Node));
	array->size = 0;
	array->allocated = NODE_ARRAY_INIT_SIZE;
	return array;
}

void narray_insert(NodeArray *a, Node element) {
	if (a->size == a->allocated) {
		a->allocated *= 2;
		a->array = (Node*)realloc(a->array, a->allocated * sizeof(Node));
	}
	a->array[a->size++] = element;
}

NodeArray* narray_copy(NodeArray *target) {
	NodeArray* pointer = (NodeArray*)malloc(sizeof(NodeArray));
	int i;

	pointer->array = (Node*)malloc(target->allocated * sizeof(Node));
	pointer->size = target->size;
	pointer->allocated = target->allocated;

	for (i = 0; i < pointer->size; i++)
		(pointer->array)[i] = (target->array)[i];
	return pointer;
}

void narray_free(NodeArray *a) {
	int i;

	if (a->size > 0) {
		for (i = 0; i < a->size; i++)
			free(&(a->array)[i]);
	}

	free(a->array);
	a->array = NULL;
	a->allocated = a->size = 0;
}