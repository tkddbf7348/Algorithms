#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#pragma comment(lib, "winmm.lib") //음악, 효과음 추가 : google 참조 https://m.blog.naver.com/PostView.nhn?blogId=enter_maintanance&logNo=220950709844&proxyReferer=https:%2F%2Fwww.google.com%2F
#include <mmsystem.h>
#include "model.hpp"
#include <Windows.h>
#include <random>

using namespace std;
using namespace cv;

#define BGM_CAROL "./sound/All I Want For Christmas Is You.wav"
#define BGM_FEAR "./sound/Fear.wav"
#define _WINDOW_WIDTH 500
#define _WINDOW_HEIGHT 500

int sec = 0; //시간초 재기
int mode = 0; //카메라 모드

//카메라 변수
GLfloat camx = 0, camy = 5, camz = 0.1;
GLfloat cam2x = 0, cam2y = 0, cam2z = 0;
GLfloat cam_upx = 0, cam_upy = 1, cam_upz = 0;

//이벤트 발생 구간
GLfloat room1_range_x[2] = { -1.2, 1.3 };
GLfloat room1_range_z[2] = { -0.7, 1 };
GLfloat tree_range_x = 0.9;//트리 이벤트 발생구간
GLfloat tree_range_z = -0.25;
GLfloat radio_range_x[2] = { -0.6, -0.4 };//라디오 이벤트 발생구간
GLfloat radio_range_z[2] = { -0.3, -0.2 };
GLfloat paper1_range_x[2] = { 0.2, 0.45 };//왼쪽 종이(종이 정면으로 봤을때 기준)
GLfloat paper2_range_x[2] = { -0.2, 0 };//오른쪽 종이(종이 정면으로 봤을때 기준)
GLfloat paper_range_z[2] = { 0.45, 0.55 };//종이 z축
GLfloat fireplace_range_x[2] = { -1.25, -0.7 };//벽난로 이벤트 발생 구간
GLfloat fireplace_range_z[2] = { 0.6, 0.7 };
GLfloat hammer_range_x[2] = { 0.1, 0.35 };//망치 이벤트 발생 구간
GLfloat hammer_range_z[2] = { -0.3, -0.15 };
GLfloat teapot_range_x[2] = { -0.2, 0 };//주전자 이벤트 발생 구간
GLfloat teapot_range_z[2] = { -0.3, -0.15 };
GLfloat keybox_range_x[2] = { 0.75, 1.25 };
GLfloat keybox_range_z[2] = { 0.6, 0.75 };
GLfloat door_range_x[2] = { -1.25, -0.95 };
GLfloat door_range_z[2] = { -0.7, -0.5 };


int radio_mode = 0; //라디오 온오프 변수// 0 : FEAR 1 : CAROL
int fire_mode = 0; //벽난로 불 온오프 변수// 0 : off 1 : on
int fireplace_key_display = 0; //불을 키고 다시 끄면 열쇠 생기는 변수
int is_keybox_open = 0; //열쇠상자 열렸는지 온오프 변수
int is_door_open = 0; //문 열렸는지 확인
int is_game_dome = 0; //게임이 끝나면 1

//캐릭터 좌표
GLfloat char_x = 0, char_y = 0, char_z = 0;

//캐릭터 방향전환 각도
int char_state_angle = 0;

//트리 전구 색상 변수
float ball_RGB[3] = { 0, 0, 0 };

//트리 불 on/off
int tree_light = 0;

//오브젝트 구현 변수("model.hpp" 참조)
CModel tree, room1, hammer, desk, keybox, keyboxopen, snowman, radio, key, fireplace, door_locked, door_unlocked;

//눈사람 크기 조정 변수
GLfloat snowman_HP = 60; //초기

//눈사람이 다 녹으면 1이되고 프로그램 종료
int snowman_dead = 0;

//망치, 주전자 가졌는지 검사하는 변수
int item[4] = { 0, 0, 0, 0}; //{망치, 주전자, 불에서꺼낸열쇠, 상자에서꺼낸열쇠}
//0 : 아이템 없음
//1 : 아이템 있음
//2 : 사용하고 사라짐

//이미지 객체
Mat image[17];
// 0 : tree
// 1 : wood
// 2 : concrete
// 3 : star
// 4 : black
// 5 : carrot
// 6 : snow
// 7 : metal
// 8 : block
// 9 : ^w^
// 10 : -m-
// 11 : o_o
// 12 : OoO
// 13 : owo
// 14 : ㅋㅅㅋ
// 15 : paper
// 16 : fire

int face_mode = 2;

//벽에걸린 표정 좌표
GLfloat v[4][3] = { { -1, -1, 1 } ,
					{ -1, 1, 1 },
					 { 1, 1, 1 },
					{ 1, -1, 1 }};

//종이 좌표
GLfloat paper[4][3] = { { -1, 1, 1 } ,
					{ -1, 1, -1 },
					 { 1, 1, -1 },
					{ 1, 1, 1 } };

//texture 수 객체
GLuint tex_ids[6] = { 1, 2, 3, 4, 5, 6 };
int nTex = 6;
GLfloat backlight_color[3] = { 1, 1, 1 };// 처음 : 붉은색


// A 클릭 시 CMD메세지 변수

const char* dial_start[] = { "오늘은 12월 24일,, 크리스마스 이브",
								"어제 분명 공터에서 만들어졌는데.. 왜 이런 방에 갇혀있지? 기억이 나지 않는다..",
								"어딘지는 모르겠지만.. 내일이 크리스마스인데 여기 갇혀있을 순 없지.. 탈출해야겠어!!",
								"///////////////////////////////////////////////////////////////////////////////////",
								"  움직이기 : 방향키    행동 : a    시점전환(관찰모드) : c      아이템 확인 : x      아이템 사용 : z     조작키 확인 : h" ,
								"///////////////////////////////////////////////////////////////////////////////////",
								"궁금한 곳 앞에서 행동을 해보자! 단서를 찾을수도 있다." };


const char* dial_tree[] = { "트리가 놓여있다. 전원 스위치를 누르니 전구가 켜진다. 이쁘다."};

const char* dial_radio[] = { "라디오가 놓여있다. 다이얼을 돌리면 노래가 나올까..?" };

const char* dial_empty[] = { "여긴 아무것도 없다.. 누가 날 비웃는 듯한 느낌이 든다." };

const char* dial_paper1[] = { "종이에 뭔가 쓰여져 있다.",
							"///////////////////////////눈사람 손질법///////////////////////////",
							"                1. 머리, 가슴, 배를 분리한다.",
							"                2. 모자를 벗기고 코, 눈을 뗀다.",
							"                3. 소스를 뿌리고 각종 향신료를 뿌려 눈비린내를 제거한다.",
							"                4. 냉동실에 1시간동안 재운다. ",
							"                5. 1시간 뒤 꺼내서 맛있게 먹는다.",
							"                P.S. 회를 떠서 생으로 먹을 수 있는데 이 경우 복통을 주의한다.",
							"////////////////////////////////////////////////////////////////////",
							"",
							"......빨리 탈출해야겠어."
};

const char* dial_paper2[] = { "종이에 뭔가 쓰여져 있다.",
							"///////////////////////////오늘 할 일///////////////////////////",
							"                1. 벽난로 끄기",
							"                2. 눈사람 머리, 가슴 손질하기",
							"                3. 재워놓을 동안 배 부분 회로 먹어야지 ㅎ",
							"////////////////////////////////////////////////////////////////////",
							"",
							"......빨리 탈출해야겠어."
};

const char* dial_fireplace_on[] = { "벽난로가 있다.",
							"...",
							"...",
							"...",
							"앗! 갑자기 불이 어디서 나온거지...? 몸이 녹아버리겠어..!",
							"어떡하지..? 불을 꺼야하는데..",
							"///////벽난로를 끌 방법을 서둘러 찾자. 몸이 다 녹아버린다면 GAME OVER."
};

const char* dial_fireplace_off[] = { "주전자에 있는 물을 부었더니 불이 꺼졌다.",
							"살았다.. 그대로 녹아버릴 뻔 했네....",
							"불을 껐더니 안에 열쇠가 들어있다. 아깐 분명 없었는데..?"
};


const char* dial_hammer[] = { "망치를 주웠다. 오래된 것 같다."
};

const char* dial_teapot[] = { "주전자를 주웠다. 물이 들어있다."
};

const char* dial_keybox_hammer[] = { "망치로 자물쇠를 내쳐렸다.",
									"...콰직!",
									"망치가 부서졌다.",
									"이 망치.. 쓸모가 없군"
};

const char* dial_exit[] = { "눈사람이 다 녹아버렸다. 탈출 실패........",
};

//키보드치듯이 출력
void print_event(const char* msg[], int length) {
	cout << endl;
	for (int i = 0; i < length; i++) { //메세지 개수
		for (int j = 0; j < strlen(msg[i]); j++) { //메세지 별 글자 수
			cout << msg[i][j];
			Sleep(10);
		}
		cout << endl << endl;
	}
}

//바로 출력
void print_direct(const char* msg[], int length) {
	for (int i = 0; i < length; i++) {
		cout << msg[i] << endl;
	}
}

//빛 초기화
void init_light() {
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

//빛 설정
void set_light() {
	//빛의 색상 & 위치 정의
	GLfloat light_global_ambient[] = { 0.3, 0.3, 0.3, 1.0 };//전역광
	GLfloat light_0_pos[] = { 0, 1, 0.0, 1.0 };//빛 위치 : cam위치 고려 
	GLfloat light_0_ambient[] = { 1.0, 1.0, 1.0, 1.0 }; //주변광
	GLfloat light_0_diffuse[] = { 1, 1, 1.0, 1.0 }; //일반반사(색)
	GLfloat light_0_specular[] = { 1.0, 1.0, 1.0, 1.0 };//경면반사

	//빛 활성화, 빛 색상 & 위치 적용
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);
}

//캐릭터 색 설정
void set_character_color() {
	//물체 색상 정의 & 지정
	glDisable(GL_COLOR_MATERIAL);
	GLfloat matrial_character_ambient[] = { 0.3, 0.3, 0.3, 1.0 }; //주변광
	GLfloat matrial_character_diffuse[] = { 1, 1, 1, 1.0 }; //일반반사(색) //0.68, 0.93, 0.93
	GLfloat matrial_character_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //경면반사(거울)
	GLfloat matrial_character_shininess[] = { 0.0 };//반짝임 정도

	glMaterialfv(GL_FRONT, GL_AMBIENT, matrial_character_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matrial_character_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matrial_character_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matrial_character_shininess);
}

//주전자 색 설정
void set_teapot_color() {
	//물체 색상 정의 & 지정
	glDisable(GL_COLOR_MATERIAL);
	GLfloat matrial_teapot_ambient[] = { 0.3, 0.3, 0.3, 1.0 }; //주변광
	GLfloat matrial_teapot_diffuse[] = { 0.78, 0.62, 0.13, 1.0 }; //일반반사(색) //금색
	GLfloat matrial_teapot_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //경면반사(거울)
	GLfloat matrial_teapot_shininess[] = { 50.0 };//반짝임 정도

	glMaterialfv(GL_FRONT, GL_AMBIENT, matrial_teapot_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matrial_teapot_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matrial_teapot_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matrial_teapot_shininess);
}

//트리 전구 색 설정
void set_ball_color() {
	//  
	glDisable(GL_COLOR_MATERIAL);
	GLfloat matrial_teapot_ambient[] = { 0.3, 0.3, 0.3, 1.0 }; 
	GLfloat matrial_teapot_diffuse[] = { ball_RGB[0], ball_RGB[1], ball_RGB[2], 1.0 }; 
	GLfloat matrial_teapot_specular[] = { 1.0, 1.0, 1.0, 1.0 }; 
	GLfloat matrial_teapot_shininess[] = { 50.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, matrial_teapot_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matrial_teapot_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matrial_teapot_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matrial_teapot_shininess);
}

//벽에걸린 표정 텍스쳐 설정
void init_texture_face(uchar* pImageArray, int width, int height) {
	GLuint tex_id_face = 1;
	glGenTextures(1, &tex_id_face);
	glBindTexture(GL_TEXTURE_2D, 0); //texture 초기화
	glBindTexture(GL_TEXTURE_2D, tex_id_face);
	//image를 texture로 사용
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageArray);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
}

//나머지 텍스쳐(질감) 설정
void init_texture() { //보통 6개까지 texture 지정해도 무리 없음
	glGenTextures(nTex, tex_ids); //texture를 배열로 넣어줌
	glEnable(GL_TEXTURE_2D);
}

//오브젝트 구현
void display_objs(CModel m, const char* model_name) {
	GLfloat x, y, z, nx, ny, nz, tx, ty, tz;
	int v_id, vt_id, vn_id;
	int idx;

	// 0 : tree
	// 1 : wood
	// 2 : concrete
	// 3 : star
	// 4 : black
	// 5 : carrot
	// 6 : snow
	// 7 : metal
	// 8 : block
	// 9 : ^w^
	// 10 : -m-
	// 11 : o_o
	// 12 : OoO
	// 13 : owo
	// 14 : ㅋㅅㅋ
	// 15 : paper
	// 16 : fire

	glBindTexture(GL_TEXTURE_2D, 0); //texture 초기화
	//obj 파일 parsing
	for (int o = 0; o < m.objs.size(); o++) {

		//obj 순서에 따라 idx 할당
		if (model_name == "tree") {
			if (o == 0) {
				idx = 0; //트리
			}
			else if (o == 1) {
				idx = 1; //트리박스
			}
			else {
				idx = 3; //알록달록 별
			}
		}
		else if (model_name == "room1") {
			idx = 2; //콘크리트
		}
		else if (model_name == "hammer") {
			if (o == 0) 
				idx = 2; //해머부분(콘크리트)
			else 
				idx = 1; //손잡이 부분(나무)
		}
		else if (model_name == "desk") {
			idx = 1; //테이블(나무)
		}
		else if (model_name == "keybox") {
			if (o == 1 || o == 9)
				idx = 7; //자물쇠 : 금속
			else
				idx = 1; //상자 : 나무
		}
		else if (model_name == "keyboxopen") {
			if (o == 1 || o == 2)
				idx = 7; //연결부분 : 금속
			else
				idx = 1; //상자 : 나무
		}
		else if (model_name == "snowman") {
			if (o == 0 || o == 1 || o == 2)//몸뚱아리
				idx = 6;//눈색
			else if (o == 3 || o == 4 || o == 6 || o == 7) //모자
				idx = 4;//검은색
			else if (o == 5) //코
				idx = 5;//당근색
		}
		else if (model_name == "key") {
			idx = 7; //금속
		}
		else if (model_name == "door_locked") {
			if (o == 0 || o == 1 || o == 2)
				idx = 1; //문, 문 손잡이 : 나무
			else
				idx = 7;// 잠금장치 : 금속
		}
		else if (model_name == "door_unlocked") {
			if (o == 3)
				idx = 7; //문 고리 : 금속
			else if (o == 4)
				idx = 4;// 나가는 길 : 검은색
			else
				idx = 1; //나머지 : 나무
		}
		else if (model_name == "radio") {
			if (o == 0)
				idx = 1; //몸체 : 나무
			else if (o == 1 || o == 2)
				idx = 4; //스피커, LCD : 검은색
			else
				idx = 7; //안테나, 다이얼 : 금속
		}
		else if (model_name == "fireplace") {
			if (o <= 4)
				idx = 8; //난로 몸체 : 벽돌
			else
				idx = 1; //모닥불용 장작 : 나무
		}

		//texture 적용
		glBindTexture(GL_TEXTURE_2D, tex_ids[idx]); //texture를 특정 id에 적용
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[idx].cols, image[idx].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[idx].data); //image를 texture로 사용
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //안티에일리어싱
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //안티에일리어싱

		int nFaces = m.objs[o].f.size(); //면의 개수
		for (int k = 0; k < nFaces; k++) {
			int nPoints = m.objs[o].f[k].v_pairs.size();
			glBegin(GL_POLYGON);
			for (int i = 0; i < nPoints; i++) {
				v_id = m.objs[o].f[k].v_pairs[i].d[0];
				vt_id = m.objs[o].f[k].v_pairs[i].d[1];
				vn_id = m.objs[o].f[k].v_pairs[i].d[2];
				x = m.objs[o].v[v_id - 1].d[0];
				y = m.objs[o].v[v_id - 1].d[1];
				z = m.objs[o].v[v_id - 1].d[2];

				nx = m.objs[o].vn[vn_id - 1].d[0];
				ny = m.objs[o].vn[vn_id - 1].d[1];
				nz = m.objs[o].vn[vn_id - 1].d[2];

				//vt = (x, y)
				tx = m.objs[o].vt[vt_id - 1].d[0];
				ty = m.objs[o].vt[vt_id - 1].d[1];

				glNormal3f(nx, ny, nz); //법선
				glTexCoord2f(tx, ty); //texture 좌표 지정
				glVertex3f(x, y, z);
			}
			glEnd();
		}
	}
}

//큐브 구현
void drawCuboid(GLfloat sx, GLfloat sy, GLfloat sz) {
	glPushMatrix();
		glScalef(sx, sy, sz); //
		glutSolidCube(0.25);
	glPopMatrix();
}

//구 구현
void drawSphere(GLfloat r, GLfloat sx, GLfloat sy, GLfloat sz) {
	glPushMatrix();
		glScalef(sx, sy, sz); //
		glutSolidSphere(r, 20, 20);
	glPopMatrix();
}

//주전자 구현
void makeTeapot(GLfloat x, GLfloat y, GLfloat z) {
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, 0); //texture 초기화
		set_teapot_color();
		glTranslatef(x, y, z);
		glRotatef(-90, 0, 1, 0); //y축 기준 90도회전
		glutSolidTeapot(0.05);
	glPopMatrix();

}

//트리 전구 구현
void make_tree_ball() {
	glPushMatrix();
	set_ball_color();
	glBindTexture(GL_TEXTURE_2D, 0); //texture 

	//중간
	glTranslatef(1.2, 0.25, -0.45);
	drawSphere(0.03, 1, 1, 1);

	//왼쪽위
	glPushMatrix();
	glTranslatef(-0.07, 0.1, -0.1);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	//오른쪽위
	glPushMatrix();
	glTranslatef(0.09, 0.05, -0.15);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	//왼쪽아래
	glPushMatrix();
	glTranslatef(-0.15, -0.1, -0.03);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	//오른쪽아래
	glPushMatrix();
	glTranslatef(0.15, -0.1, -0.01);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	//중간아래
	glPushMatrix();
	glTranslatef(-0.05, -0.15, 0.04);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	glPopMatrix();
}

//오브젝트 구현(display_objs) 호출
void make_objs() {
	//트리 생성
	glPushMatrix();
		glScalef(0.2, 0.2, 0.2);
		glTranslatef(6, 0, -3);     
		glRotatef(90, 0, 1, 0);
		display_objs(tree, "tree");
	glPopMatrix();

	//1번째 방 벽 생성
	glPushMatrix();
		glTranslatef(-1.5, 0.2, 0);
		glScalef(0.18, 0.1, 0.17);
		glRotatef(90, 0, 1, 0); //y축 기준 90도회전
		display_objs(room1, "room1");
	glPopMatrix();

	if (item[0] == 0) {
		//망치 생성
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(2, 1.5, -4);
			glRotatef(90, 0, 0, 1); //z축 기준 90도회전
			display_objs(hammer, "hammer");
		glPopMatrix();
	}

	//책상1 생성
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(0.5, 1, -5);
		glRotatef(90, 0, 1, 0); //y축 기준 90도회전
		display_objs(desk, "desk");
	glPopMatrix();

	//책상2 생성
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(-6, 1, -5);
		glRotatef(90, 0, 1, 0); //y축 기준 90도회전
		display_objs(desk, "desk");
	glPopMatrix();

	//책상3 생성
	glPushMatrix();
		glScalef(0.2, 0.1, 0.1);
		glTranslatef(0, 1.5, 8);
		glRotatef(90, 0, 1, 0); //y축 기준 90도회전
		display_objs(desk, "desk"); //나무
	glPopMatrix();

	if (is_keybox_open) { // 박스가 열려있는 상태일 떄
			//열쇠상자 생성(열린 것)
			glPushMatrix();
				glScalef(0.1, 0.1, 0.1);
				glTranslatef(10, 0, 9);
				glRotatef(90, 0, 1, 0); // y축 기준 90도회전
				display_objs(keyboxopen, "keyboxopen"); // 열쇠상자는 책상과 같은 나무재질로
			glPopMatrix();

		if (item[3] == 0) { // 열쇠를 안가져갔을 때
			// 열쇠 생성(열쇠상자에 있는 것)
			glPushMatrix();
				glScalef(0.1, 0.1, 0.1);
				glTranslatef(10, 0, 9);
				glRotatef(90, 0, 1, 0); //y축 기준 90도회전
				display_objs(key, "key"); //열쇠상자는 책상과 같은 나무재질로
			glPopMatrix();
		}
	}
	else { // 박스가 닫혀있을 때
		// 열쇠상자 생성(닫힌 것)
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(10, 0, 9);
			glRotatef(90, 0, 1, 0); // y축 기준 90도회전
			display_objs(keybox, "keybox"); // 열쇠상자는 책상과 같은 나무재질로
		glPopMatrix();
	}

	//캐릭터(눈사람) 생성
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(char_x * 10, char_y * 10, char_z * 10); //움직임
		glRotatef(char_state_angle, 0, 1, 0); //y축 기준 반시계 회전
		display_objs(snowman, "snowman"); //임시 텍스쳐
	glPopMatrix();

	//라디오
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(-5, 1.5, -4);
		glRotatef(-90, 0, 1, 0); //y축 기준 -90도회전
		display_objs(radio, "radio");
	glPopMatrix();

	if (!is_door_open) { //문이 닫혀있음
		//잠긴 문
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(-3, 0.8, -6.8);
			glRotatef(180, 0, 1, 0); //y축 기준 90도회전
			display_objs(door_locked, "door_locked"); // 문은 나무, 잠금장치는 금속
		glPopMatrix();
	}
	else { //문이 열림
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(-11.3, 0, -7);
			glRotatef(-90, 0, 1, 0); //y축 기준 90도회전
			display_objs(door_unlocked, "door_unlocked"); // 문은 나무, 잠금장치는 금속
		glPopMatrix();
		is_game_dome = 1; //게임 끝
	}

	//벽난로 생성
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(-10, 0, 9);
		glRotatef(90, 0, 1, 0); //y축 기준 90도회전
		display_objs(fireplace, "fireplace"); //난로 생성(벽돌재질, 장작 : 나무)
	glPopMatrix();

	if (fireplace_key_display && !(item[2] == 1)) { //불을 끄고 열쇠를 줍지는 않았을 때
		//열쇠 생성(벽난로 안에 있는 것)
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(-10, 0, 9);
			glRotatef(90, 0, 1, 0); //y축 기준 90도회전
			display_objs(key, "key"); //열쇠상자는 책상과 같은 나무재질로
		glPopMatrix();
	}

	if (item[1] == 0) {
		makeTeapot(-0.1, 0.15, -0.5);
	}
	make_tree_ball();
}

//벽에 표정 걸기
void make_face(int mode) {
	init_texture_face(image[mode].data, image[mode].cols, image[mode].rows);

	glPushMatrix();
		glTranslatef(0.6, 0.2, -0.9);
		glScalef(0.15, 0.15, 0.15);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			//texture 1번째 점
			glTexCoord2f(0, 1);
			glVertex3f(v[0][0], v[0][1], v[0][2]);
			//texture 2번째 점
			glTexCoord2f(0, 0);
			glVertex3f(v[1][0], v[1][1], v[1][2]);
			//texture 3번째 점
			glTexCoord2f(1, 0);
			glVertex3f(v[2][0], v[2][1], v[2][2]);
			//texture 4번째 점
			glTexCoord2f(1, 1);
			glVertex3f(v[3][0], v[3][1], v[3][2]);
		glEnd();
	glPopMatrix();
}

//벽난로 키면 나오는 불
void make_fire() {
	init_texture_face(image[16].data, image[16].cols, image[16].rows);

	glPushMatrix();
		glTranslatef(-1, 0.15, 0.62);
		glScalef(0.3, 0.15, 0.15);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		//texture 1번째 점
		glTexCoord2f(0, 1);
		glVertex3f(v[0][0], v[0][1], v[0][2]);
		//texture 2번째 점
		glTexCoord2f(0, 0);
		glVertex3f(v[1][0], v[1][1], v[1][2]);
		//texture 3번째 점
		glTexCoord2f(1, 0);
		glVertex3f(v[2][0], v[2][1], v[2][2]);
		//texture 4번째 점
		glTexCoord2f(1, 1);
		glVertex3f(v[3][0], v[3][1], v[3][2]);
		glEnd();
	glPopMatrix();
}

//테이블에 있는 종이 구현
void make_paper(GLfloat x, GLfloat y, GLfloat z) {
	init_texture_face(image[15].data, image[15].cols, image[15].rows);

	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(0.07, 0.2, 0.1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		//texture 1번째 점
		glTexCoord2f(0, 1);
		glVertex3f(paper[0][0], paper[0][1], paper[0][2]);
		//texture 2번째 점
		glTexCoord2f(0, 0);
		glVertex3f(paper[1][0], paper[1][1], paper[1][2]);
		//texture 3번째 점
		glTexCoord2f(1, 0);
		glVertex3f(paper[2][0], paper[2][1], paper[2][2]);
		//texture 4번째 점
		glTexCoord2f(1, 1);
		glVertex3f(paper[3][0], paper[3][1], paper[3][2]);
		glEnd();
	glPopMatrix();
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camx, camy, camz, cam2x, cam2y, cam2z, cam_upx, cam_upy, cam_upz);

	make_objs();

	make_face(face_mode);//owo
	make_paper(0.3, 0, 0.8);
	make_paper(-0.1, 0, 0.8);
	if (fire_mode) {
		make_fire();
	}
	set_light();


	glEnable(GL_COLOR_MATERIAL);
	glutSwapBuffers();
}

void MyTimer(int value) {
	sec += 20;

	if (sec % 200 == 0) {
		if (tree_light) { //1초에 1번 트리에 붙은 공의 색을 바꿔줌
			for (int i = 0; i < 3; i++) {
				random_device rand;
				mt19937 gen(rand());
				uniform_int_distribution<int> dist(0, 99);
				ball_RGB[i] = dist(gen) / 100.0;
			}
		}

		if (fire_mode) {
			snowman_HP -= 1;
			cout << "눈사람의 생명력 : " << snowman_HP << endl;
		}

		if (snowman_HP == 0) {
			print_event(dial_exit, 1);
			exit(0);
		}

		if (is_game_dome) {
			system("PAUSE");
			exit(0);
		}
	}
	
	glutTimerFunc(20, MyTimer, 1);
	glutPostRedisplay();
}

void Keyboard(unsigned char Key, int X, int Y) {
	/*
GLfloat tree_range_x = 0.9;
GLfloat tree_range_z = -0.4;
GLfloat radio_range_x[2] = { -0.6, -0.4 };
GLfloat radio_range_z[2] = { -0.3, -0.2 };
GLfloat paper1_range_x[2] = {0.2, 0.45};
GLfloat paper2_range_x[2] = {-0.2, 0};
GLfloat paper_range_z[2] = { 0.45, 0.55 };
*/

	switch (Key) {
	case 'c':
		if (mode == 0) {
			//1인칭
			if (char_z >= 0) {
				camx = char_x, camy = 1, camz = char_z - 0.5;
				cam2x = char_x, cam2z = char_z + 0.5;
			}
			else {
				camx = char_x, camy = 1, camz = char_z + 0.5;
				cam2x = char_x, cam2z = char_z - 0.5;
			}
			mode = 1;
		}
		else if (mode == 1) {
			camx = 0, camy = 5, camz = 0.1;
			cam2x = 0, cam2y = 0, cam2z = 0;

			mode = 0;
		}
		break;
		
	case 'a' :
		////////////트리 이벤트 발생 구간////////////
		if (char_z <= tree_range_z && char_x >= tree_range_x) {
			face_mode = 9;//^w^     
			if (!tree_light) {
				tree_light = 1;
			}
			else {
				tree_light = 0;
			}
			print_event(dial_tree, 1);
			break;
		}

		////////////라디오 이벤트 발생 구간////////////
		else if (char_x >= radio_range_x[0] && char_x <= radio_range_x[1] && char_z >= radio_range_z[0] && char_z <= radio_range_z[1]) {
			if (radio_mode == 0) {
				face_mode = 9;//^w^         
				radio_mode = 1;// 켜짐
				PlaySound(NULL, 0, 0); //음악초기화
				PlaySound(TEXT(BGM_CAROL), 0, SND_ASYNC | SND_LOOP);
				print_event(dial_radio, 1);
			}
			else {
				face_mode = 10;//-m-
				radio_mode = 0; //꺼짐
				PlaySound(NULL, 0, 0); //음악초기화
				PlaySound(TEXT(BGM_FEAR), 0, SND_ASYNC | SND_LOOP);
				print_event(dial_radio, 1);
			}
		}

		/////////// 종이1 이벤트 발생 구간 ////////////////
		else if (char_x >= paper1_range_x[0] && char_x <= paper1_range_x[1] && char_z >= paper_range_z[0] && char_z <= paper_range_z[1]) {
			print_event(dial_paper1, 11);
		}
		/////////// 종이2 이벤트 발생 구간 ////////////////
		else if (char_x >= paper2_range_x[0] && char_x <= paper2_range_x[1] && char_z >= paper_range_z[0] && char_z <= paper_range_z[1]) {
			print_event(dial_paper2, 8);
		}
		
		/////////// 벽난로 이벤트 발생 구간 ///////////
		else if (char_x >= fireplace_range_x[0] && char_x <= fireplace_range_x[1] && char_z >= fireplace_range_z[0] && char_z <= fireplace_range_z[1]) {
			if (!fire_mode && !fireplace_key_display) {//불이 꺼져있고 열쇠를 못찾았을 때
				fire_mode = 1;
				face_mode = 12; //놀란 표정
				print_event(dial_fireplace_on, 7);
			}
			else if (!fire_mode && fireplace_key_display && item[2] == 0) { //불이 꺼져있고 열쇠를 찾았을 때(줍지는 않음)
				cout << endl << "열쇠를 주웠다." << endl;
				cout << endl <<  "이 열쇠는 어디다가 쓰지..?" << endl;
				item[2] = 1;
				face_mode = 12;
			}
			else if(fire_mode) { //불이 켜져있을 때
				cout << "어서 불을 꺼야 해." << endl;
			}
			else {
				cout << endl << "벽난로가 있다." << endl;
			}
		}

		/////////// 망치 이벤트 발생 구간 ////////////////
		else if (char_x >= hammer_range_x[0] && char_x <= hammer_range_x[1] && char_z >= hammer_range_z[0] && char_z <= hammer_range_z[1] && item[0] == 0) {
			item[0] = 1;
			cout << endl << "망치를 주웠다." << endl;
		}
		
		/////////// 주전자 이벤트 발생 구간 ////////////////
		else if (char_x >= teapot_range_x[0] && char_x <= teapot_range_x[1] && char_z >= teapot_range_z[0] && char_z <= teapot_range_z[1] && item[1] == 0) {
			item[1] = 1;
			cout << endl << "주전자를 주웠다." << endl;
		}

		///////////열쇠상자 이벤트 발생 구간 ////////////////
		else if (char_x >= keybox_range_x[0] && char_x <= keybox_range_x[1] && char_z >= keybox_range_z[0] && char_z <= keybox_range_z[1]) {
			if (is_keybox_open) {
				if (item[3] == 0) { //열쇠가 상자에 있을 떄
					cout << endl << "열쇠를 또 주웠다." << endl;
					cout << endl << "이 열쇠는 또 어디다가 쓰지..?" << endl;
					item[3] = 1;
					face_mode = 12;
				}
				else { //열쇠가 상자에 없을 때
					cout << endl << "이 상자에는 이제 쓸모가 없다." << endl;
				}
			}
			else {
				cout << endl << "자물쇠로 굳게 닫혀있는 상자가 있다. 안에는 뭐가 있을까?.." << endl;
			}
			
		}

		//문에서
		else if (char_x >= door_range_x[0] && char_x <= door_range_x[1] && char_z >= door_range_z[0] && char_z <= door_range_z[1]) {
			cout << "문은 굳게 닫혀있다." << endl;
		}
		////////////기타 구간////////////
		else {
			face_mode = 14;//ㅋㅅㅋ
			print_event(dial_empty, 1);
		}

		break;

	//아이템 확인
	case 'x' :
		cout << "//////////////////////////////아이템 창///////////////////////////////////" << endl;

		if (item[0] == 0 && item[1] == 0 && item[2] == 0 && item[3] == 0) { //없을 때
			cout << endl << "가지고 있는 아이템이 없다." << endl;
			break;
		}

		for (int i = 0; i < 4; i++) {
			switch (i) {
			case 0 : 
				if(item[i] == 1)
					cout << "망치 ";
				break;
			case 1 :
				if (item[i] == 1)
					cout << "주전자 ";
				break;
			case 2  :
				if (item[i] == 1)
					cout << "벽난로에서 찾은 열쇠 ";
				break;
			case 3 :
				if (item[i] == 1)
					cout << "상자에서 찾은 열쇠 ";
				break;
			}
		}
		cout << endl;
		break;
	
	//아이템 사용
	case 'z' :
		if (item[0] == 0 && item[1] == 0) {
			cout << endl << "아이템이 없다." << endl;
			break;
		}

		if (char_x >= keybox_range_x[0] && char_x <= keybox_range_x[1] && char_z >= keybox_range_z[0] && char_z <= keybox_range_z[1]) { //열쇠상자 주변에 있을 때
			if (item[0] == 1) { //망치가 있을 때
				print_event(dial_keybox_hammer, 4);
				item[0] = 2;//망치가 부서짐
				face_mode = 14;
			}
			else { //망치가 없을 때 
				if (item[2] == 1) { //열쇠 1이 있을 때
					is_keybox_open = 1; //열쇠상자 열림
					cout << endl << "상자가 열렸다. 안에 또 다른 열쇠가 들어있다." << endl;
					item[2] = 0;
					face_mode = 12;
				}
				else if(!is_keybox_open){ //열쇠 1이 없고 상자가 안열렸을 때
					cout << endl << "이 자물쇠는 어떻게 해야 열리지..?" << endl;
				}
			}
			break;
		}
		
		if (char_x >= fireplace_range_x[0] && char_x <= fireplace_range_x[1] && char_z >= fireplace_range_z[0] && char_z <= fireplace_range_z[1] && fire_mode == 1) { //벽난로 주변에 있고 불이 켜져 있을 떄
			if (item[1] == 1) { //주전자가 있을 때
				face_mode = 13; //만족한 표정
				print_event(dial_fireplace_off, 3);
				fire_mode = 0;
				fireplace_key_display = 1; //열쇠 보이기
				item[1] = 2;//주전자 사용 끝
			}
			else { //주전자가 없을 때
				cout << endl << "어서 빨리 불을 꺼야 한다." << endl;
			}
			break;
		}

		//문에서 
		if (char_x >= door_range_x[0] && char_x <= door_range_x[1] && char_z >= door_range_z[0] && char_z <= door_range_z[1]) {
			if (!is_door_open && item[3] == 1) {//문이 잠겨있고 열쇠가 있을 때
				is_door_open = 1;
				item[3] == 0;
				face_mode = 12;
				cout << endl << "문이 열렸다 !!!!! 당장 탈출하자!!!!!!" << endl;
		}
			break;
		}
		break;

	case 'h':  
		cout << endl << "///////////////////////////////////////////////////////////////////////////////////" << endl;
		cout << endl << "  움직이기 : 방향키    행동 : a    시점전환(관찰모드) : c      아이템 확인 : x      아이템 사용 : z     조작키 확인 : h" << endl;
		cout << endl << "///////////////////////////////////////////////////////////////////////////////////" << endl;
		break;

	default:
		break;
	}
}

//방향키 설정
void specialKeyboard(int Key, int X, int Y) {
	switch (Key) {
	case GLUT_KEY_UP:
		if (char_z > room1_range_z[0])
			char_z -= 0.05; 


		char_state_angle = 90;
		
		break;

	case GLUT_KEY_DOWN:
		if (char_z < room1_range_z[1])
			char_z += 0.05;

		char_state_angle = 270;

		break;

	case GLUT_KEY_LEFT:
		if (char_x > room1_range_x[0])
			char_x -= 0.05;

		char_state_angle = 180;

		break;

	case GLUT_KEY_RIGHT:
		if (char_x < room1_range_x[1])
			char_x += 0.05;

		char_state_angle = 0;

		break;

	default:
		break;
	}
}

void reshape(int width, int height) {
	GLfloat left = 0, bottom = 0;
	glViewport(left, bottom, width - left, height - bottom);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat ratio = (float)width / height;
	gluPerspective(40, ratio, 0.1, 1000);


	/*glViewport(0, 0, width, height);
	GLfloat f_w = (GLfloat)width / (GLfloat)_WINDOW_WIDTH;
	GLfloat f_h = (GLfloat)height / (GLfloat)_WINDOW_HEIGHT;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * f_w, 1.0 * f_w, -1.0 * f_h, 1.0 * f_h, 10, -10);*/
}

int main(int argc, char** argv) {
	print_event(dial_start, 5);

	//텍스쳐 이미지 읽어오기
	// 0 : tree
	// 1 : wood
	// 2 : concrete
	// 3 : star
	// 4 : black
	// 5 : carrot
	// 6 : snow
	// 7 : metal
	// 8 : block
	// 9 : ^w^
	// 10 : -m-
	// 11 : o_o
	// 12 : OoO
	// 13 : owo
	// 14 : ㅋㅅㅋ
	// 15 : paper
	// 16 : fire

	image[0] = imread("./Tex/tree_texture.jpg", IMREAD_COLOR);
	cvtColor(image[0], image[0], COLOR_BGR2RGB);

	image[1] = imread("./Tex/wood_texture.jpg", IMREAD_COLOR);
	cvtColor(image[1], image[1], COLOR_BGR2RGB);

	image[2] = imread("./Tex/room_texture.jpg", IMREAD_COLOR);
	cvtColor(image[2], image[2], COLOR_BGR2RGB);

	image[3] = imread("./Tex/star_texture.jpg", IMREAD_COLOR);
	cvtColor(image[3], image[3], COLOR_BGR2RGB);

	image[4] = imread("./Tex/black_texture.jpg", IMREAD_COLOR);
	cvtColor(image[4], image[4], COLOR_BGR2RGB);

	image[5] = imread("./Tex/carrot_texture.jpg", IMREAD_COLOR);
	cvtColor(image[5], image[5], COLOR_BGR2RGB);

	image[6] = imread("./Tex/snow_texture.jpg", IMREAD_COLOR);
	cvtColor(image[6], image[6], COLOR_BGR2RGB);

	image[7] = imread("./Tex/metal_texture.jpg", IMREAD_COLOR);
	cvtColor(image[7], image[7], COLOR_BGR2RGB);

	image[8] = imread("./Tex/block_texture.jpg", IMREAD_COLOR);
	cvtColor(image[8], image[8], COLOR_BGR2RGB);

	//face 이미지 읽어오기
	image[9] = imread("./face/^w^.jpg", IMREAD_COLOR);
	cvtColor(image[9], image[9], COLOR_BGR2RGB);

	image[10] = imread("./face/-m-.jpg", IMREAD_COLOR);
	cvtColor(image[10], image[10], COLOR_BGR2RGB);

	image[11] = imread("./face/o_o.jpg", IMREAD_COLOR);
	cvtColor(image[11], image[11], COLOR_BGR2RGB);

	image[12] = imread("./face/OoO.jpg", IMREAD_COLOR);
	cvtColor(image[12], image[12], COLOR_BGR2RGB);

	image[13] = imread("./face/owo.jpg", IMREAD_COLOR);
	cvtColor(image[13], image[13], COLOR_BGR2RGB);

	image[14] = imread("./face/ㅋㅅㅋ.jpg", IMREAD_COLOR);
	cvtColor(image[14], image[14], COLOR_BGR2RGB);

	image[15] = imread("./Tex/paper_texture.jpg", IMREAD_COLOR);
	cvtColor(image[15], image[15], COLOR_BGR2RGB);

	image[16] = imread("./Tex/fire_texture.jpg", IMREAD_COLOR);
	cvtColor(image[16], image[16], COLOR_BGR2RGB);

	//모델 읽어오기
	string filepath_tree = "./Obj/tree.obj";
	ifstream fin_tree(filepath_tree);
	tree.loadObj(fin_tree);
	fin_tree.close();

	string filepath_room = "./Obj/room_fix.obj";
	ifstream fin_room(filepath_room);
	room1.loadObj(fin_room);
	fin_room.close();

	string filepath_hammer = "./Obj/hammer.obj";
	ifstream fin_hammer(filepath_hammer);
	hammer.loadObj(fin_hammer);
	fin_hammer.close();

	string filepath_desk = "./Obj/desk.obj";
	ifstream fin_desk(filepath_desk);
	desk.loadObj(fin_desk);
	fin_desk.close();

	string filepath_keybox = "./Obj/keybox.obj";
	ifstream fin_keybox(filepath_keybox);
	keybox.loadObj(fin_keybox);
	fin_keybox.close();

	string filepath_keyboxopen = "./Obj/keyboxopen.obj";
	ifstream fin_keyboxopen(filepath_keyboxopen);
	keyboxopen.loadObj(fin_keyboxopen);
	fin_keyboxopen.close();

	string filepath_snowman = "./Obj/snowman.obj";
	ifstream fin_snowman(filepath_snowman);
	snowman.loadObj(fin_snowman);
	fin_snowman.close();

	string filepath_radio = "./Obj/radio.obj";
	ifstream fin_radio(filepath_radio);
	radio.loadObj(fin_radio);
	fin_radio.close();

	string filepath_key = "./Obj/key.obj";
	ifstream fin_key(filepath_key);
	key.loadObj(fin_key);
	fin_key.close();

	string filepath_fireplace = "./Obj/fireplace.obj";
	ifstream fin_fireplace(filepath_fireplace);
	fireplace.loadObj(fin_fireplace);
	fin_fireplace.close();

	string filepath_door_locked = "./Obj/door_locked.obj";
	ifstream fin_door_locked(filepath_door_locked);
	door_locked.loadObj(fin_door_locked);
	fin_door_locked.close();

	string filepath_door_unlocked = "./Obj/door_unlocked.obj";
	ifstream fin_door_unlocked(filepath_door_unlocked);
	door_unlocked.loadObj(fin_door_unlocked);
	fin_door_unlocked.close();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
	glutCreateWindow("Escape room _ Christmas Edition");

	PlaySound(NULL, 0, 0); //음악초기화
	PlaySound(TEXT(BGM_FEAR), 0, SND_ASYNC | SND_LOOP); //루프 재생

	init_light(); 
	init_texture();

	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(specialKeyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(20, MyTimer, 1);

	glutMainLoop();
	return 0;
}