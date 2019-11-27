#pragma once

#define MAX_LINE 15
#define MAX_BIT_CNT 64
#define NODE_ARRAY_INIT_SIZE 1
#define MAX_SEARCH_TIME_IN_SEC 5
#define ll unsigned long long

typedef enum { false, true } bool;
typedef enum { Black, White } Turn;
typedef struct Board Board;
typedef struct Node Node;
typedef struct NodeArray NodeArray;
typedef struct Bound Bound;

struct Board {
	ll OM1;
	ll OM2;
	ll OM3;
	ll OM4;
};

struct NodeArray {
	Node* array;
	size_t size;
	size_t allocated;
};

struct Bound {
	int max_x;
	int min_x;
	int max_y;
	int min_y;
};

struct Node {
	Board *boardWhite;
	Board *boardBlack;
	Bound boundWhite;
	Bound boundBlack;
	int win;
	int N;
	Turn turn;
	NodeArray *nodes;
};

typedef struct user {
	unsigned int id; //32  //사용자를 식별하기 위한 정보
	char name[20];         //사용자의 이름
	unsigned int age; //16 //사용자의 나이
	char e_mail[30];       //사용자의 이메일
	char nickname[20];     //사용자의 아이디
	char password[20];     //사용자의 패스워드
	char stone_shape1[10]; //사용자의 오목알백모양
	char stone_shape2[10]; //사용자의 오목알흑모양
	unsigned int win;      //사용자의 전적 승
	unsigned int lose;     //사용자의 전적 패
	unsigned int rate;     //사용자의 승률
}_User;