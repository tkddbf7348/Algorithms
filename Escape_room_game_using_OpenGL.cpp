#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#pragma comment(lib, "winmm.lib") //����, ȿ���� �߰� : google ���� https://m.blog.naver.com/PostView.nhn?blogId=enter_maintanance&logNo=220950709844&proxyReferer=https:%2F%2Fwww.google.com%2F
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

int sec = 0; //�ð��� ���
int mode = 0; //ī�޶� ���

//ī�޶� ����
GLfloat camx = 0, camy = 5, camz = 0.1;
GLfloat cam2x = 0, cam2y = 0, cam2z = 0;
GLfloat cam_upx = 0, cam_upy = 1, cam_upz = 0;

//�̺�Ʈ �߻� ����
GLfloat room1_range_x[2] = { -1.2, 1.3 };
GLfloat room1_range_z[2] = { -0.7, 1 };
GLfloat tree_range_x = 0.9;//Ʈ�� �̺�Ʈ �߻�����
GLfloat tree_range_z = -0.25;
GLfloat radio_range_x[2] = { -0.6, -0.4 };//���� �̺�Ʈ �߻�����
GLfloat radio_range_z[2] = { -0.3, -0.2 };
GLfloat paper1_range_x[2] = { 0.2, 0.45 };//���� ����(���� �������� ������ ����)
GLfloat paper2_range_x[2] = { -0.2, 0 };//������ ����(���� �������� ������ ����)
GLfloat paper_range_z[2] = { 0.45, 0.55 };//���� z��
GLfloat fireplace_range_x[2] = { -1.25, -0.7 };//������ �̺�Ʈ �߻� ����
GLfloat fireplace_range_z[2] = { 0.6, 0.7 };
GLfloat hammer_range_x[2] = { 0.1, 0.35 };//��ġ �̺�Ʈ �߻� ����
GLfloat hammer_range_z[2] = { -0.3, -0.15 };
GLfloat teapot_range_x[2] = { -0.2, 0 };//������ �̺�Ʈ �߻� ����
GLfloat teapot_range_z[2] = { -0.3, -0.15 };
GLfloat keybox_range_x[2] = { 0.75, 1.25 };
GLfloat keybox_range_z[2] = { 0.6, 0.75 };
GLfloat door_range_x[2] = { -1.25, -0.95 };
GLfloat door_range_z[2] = { -0.7, -0.5 };


int radio_mode = 0; //���� �¿��� ����// 0 : FEAR 1 : CAROL
int fire_mode = 0; //������ �� �¿��� ����// 0 : off 1 : on
int fireplace_key_display = 0; //���� Ű�� �ٽ� ���� ���� ����� ����
int is_keybox_open = 0; //������� ���ȴ��� �¿��� ����
int is_door_open = 0; //�� ���ȴ��� Ȯ��
int is_game_dome = 0; //������ ������ 1

//ĳ���� ��ǥ
GLfloat char_x = 0, char_y = 0, char_z = 0;

//ĳ���� ������ȯ ����
int char_state_angle = 0;

//Ʈ�� ���� ���� ����
float ball_RGB[3] = { 0, 0, 0 };

//Ʈ�� �� on/off
int tree_light = 0;

//������Ʈ ���� ����("model.hpp" ����)
CModel tree, room1, hammer, desk, keybox, keyboxopen, snowman, radio, key, fireplace, door_locked, door_unlocked;

//����� ũ�� ���� ����
GLfloat snowman_HP = 60; //�ʱ�

//������� �� ������ 1�̵ǰ� ���α׷� ����
int snowman_dead = 0;

//��ġ, ������ �������� �˻��ϴ� ����
int item[4] = { 0, 0, 0, 0}; //{��ġ, ������, �ҿ�����������, ���ڿ�����������}
//0 : ������ ����
//1 : ������ ����
//2 : ����ϰ� �����

//�̹��� ��ü
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
// 14 : ������
// 15 : paper
// 16 : fire

int face_mode = 2;

//�����ɸ� ǥ�� ��ǥ
GLfloat v[4][3] = { { -1, -1, 1 } ,
					{ -1, 1, 1 },
					 { 1, 1, 1 },
					{ 1, -1, 1 }};

//���� ��ǥ
GLfloat paper[4][3] = { { -1, 1, 1 } ,
					{ -1, 1, -1 },
					 { 1, 1, -1 },
					{ 1, 1, 1 } };

//texture �� ��ü
GLuint tex_ids[6] = { 1, 2, 3, 4, 5, 6 };
int nTex = 6;
GLfloat backlight_color[3] = { 1, 1, 1 };// ó�� : ������


// A Ŭ�� �� CMD�޼��� ����

const char* dial_start[] = { "������ 12�� 24��,, ũ�������� �̺�",
								"���� �и� ���Ϳ��� ��������µ�.. �� �̷� �濡 ��������? ����� ���� �ʴ´�..",
								"������� �𸣰�����.. ������ ũ���������ε� ���� �������� �� ����.. Ż���ؾ߰ھ�!!",
								"///////////////////////////////////////////////////////////////////////////////////",
								"  �����̱� : ����Ű    �ൿ : a    ������ȯ(�������) : c      ������ Ȯ�� : x      ������ ��� : z     ����Ű Ȯ�� : h" ,
								"///////////////////////////////////////////////////////////////////////////////////",
								"�ñ��� �� �տ��� �ൿ�� �غ���! �ܼ��� ã������ �ִ�." };


const char* dial_tree[] = { "Ʈ���� �����ִ�. ���� ����ġ�� ������ ������ ������. �̻ڴ�."};

const char* dial_radio[] = { "������ �����ִ�. ���̾��� ������ �뷡�� ���ñ�..?" };

const char* dial_empty[] = { "���� �ƹ��͵� ����.. ���� �� ����� ���� ������ ���." };

const char* dial_paper1[] = { "���̿� ���� ������ �ִ�.",
							"///////////////////////////����� ������///////////////////////////",
							"                1. �Ӹ�, ����, �踦 �и��Ѵ�.",
							"                2. ���ڸ� ����� ��, ���� ����.",
							"                3. �ҽ��� �Ѹ��� ���� ��ŷḦ �ѷ� ���񸰳��� �����Ѵ�.",
							"                4. �õ��ǿ� 1�ð����� ����. ",
							"                5. 1�ð� �� ������ ���ְ� �Դ´�.",
							"                P.S. ȸ�� ���� ������ ���� �� �ִµ� �� ��� ������ �����Ѵ�.",
							"////////////////////////////////////////////////////////////////////",
							"",
							"......���� Ż���ؾ߰ھ�."
};

const char* dial_paper2[] = { "���̿� ���� ������ �ִ�.",
							"///////////////////////////���� �� ��///////////////////////////",
							"                1. ������ ����",
							"                2. ����� �Ӹ�, ���� �����ϱ�",
							"                3. ������� ���� �� �κ� ȸ�� �Ծ���� ��",
							"////////////////////////////////////////////////////////////////////",
							"",
							"......���� Ż���ؾ߰ھ�."
};

const char* dial_fireplace_on[] = { "�����ΰ� �ִ�.",
							"...",
							"...",
							"...",
							"��! ���ڱ� ���� ��� ���°���...? ���� ��ƹ����ھ�..!",
							"�����..? ���� �����ϴµ�..",
							"///////�����θ� �� ����� ���ѷ� ã��. ���� �� ��ƹ����ٸ� GAME OVER."
};

const char* dial_fireplace_off[] = { "�����ڿ� �ִ� ���� �ξ����� ���� ������.",
							"��Ҵ�.. �״�� ��ƹ��� �� �߳�....",
							"���� ������ �ȿ� ���谡 ����ִ�. �Ʊ� �и� �����µ�..?"
};


const char* dial_hammer[] = { "��ġ�� �ֿ���. ������ �� ����."
};

const char* dial_teapot[] = { "�����ڸ� �ֿ���. ���� ����ִ�."
};

const char* dial_keybox_hammer[] = { "��ġ�� �ڹ��踦 ���ķȴ�.",
									"...����!",
									"��ġ�� �μ�����.",
									"�� ��ġ.. ���� ����"
};

const char* dial_exit[] = { "������� �� ��ƹ��ȴ�. Ż�� ����........",
};

//Ű����ġ���� ���
void print_event(const char* msg[], int length) {
	cout << endl;
	for (int i = 0; i < length; i++) { //�޼��� ����
		for (int j = 0; j < strlen(msg[i]); j++) { //�޼��� �� ���� ��
			cout << msg[i][j];
			Sleep(10);
		}
		cout << endl << endl;
	}
}

//�ٷ� ���
void print_direct(const char* msg[], int length) {
	for (int i = 0; i < length; i++) {
		cout << msg[i] << endl;
	}
}

//�� �ʱ�ȭ
void init_light() {
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

//�� ����
void set_light() {
	//���� ���� & ��ġ ����
	GLfloat light_global_ambient[] = { 0.3, 0.3, 0.3, 1.0 };//������
	GLfloat light_0_pos[] = { 0, 1, 0.0, 1.0 };//�� ��ġ : cam��ġ ��� 
	GLfloat light_0_ambient[] = { 1.0, 1.0, 1.0, 1.0 }; //�ֺ���
	GLfloat light_0_diffuse[] = { 1, 1, 1.0, 1.0 }; //�Ϲݹݻ�(��)
	GLfloat light_0_specular[] = { 1.0, 1.0, 1.0, 1.0 };//���ݻ�

	//�� Ȱ��ȭ, �� ���� & ��ġ ����
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_0_pos);
}

//ĳ���� �� ����
void set_character_color() {
	//��ü ���� ���� & ����
	glDisable(GL_COLOR_MATERIAL);
	GLfloat matrial_character_ambient[] = { 0.3, 0.3, 0.3, 1.0 }; //�ֺ���
	GLfloat matrial_character_diffuse[] = { 1, 1, 1, 1.0 }; //�Ϲݹݻ�(��) //0.68, 0.93, 0.93
	GLfloat matrial_character_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //���ݻ�(�ſ�)
	GLfloat matrial_character_shininess[] = { 0.0 };//��¦�� ����

	glMaterialfv(GL_FRONT, GL_AMBIENT, matrial_character_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matrial_character_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matrial_character_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matrial_character_shininess);
}

//������ �� ����
void set_teapot_color() {
	//��ü ���� ���� & ����
	glDisable(GL_COLOR_MATERIAL);
	GLfloat matrial_teapot_ambient[] = { 0.3, 0.3, 0.3, 1.0 }; //�ֺ���
	GLfloat matrial_teapot_diffuse[] = { 0.78, 0.62, 0.13, 1.0 }; //�Ϲݹݻ�(��) //�ݻ�
	GLfloat matrial_teapot_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //���ݻ�(�ſ�)
	GLfloat matrial_teapot_shininess[] = { 50.0 };//��¦�� ����

	glMaterialfv(GL_FRONT, GL_AMBIENT, matrial_teapot_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matrial_teapot_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matrial_teapot_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matrial_teapot_shininess);
}

//Ʈ�� ���� �� ����
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

//�����ɸ� ǥ�� �ؽ��� ����
void init_texture_face(uchar* pImageArray, int width, int height) {
	GLuint tex_id_face = 1;
	glGenTextures(1, &tex_id_face);
	glBindTexture(GL_TEXTURE_2D, 0); //texture �ʱ�ȭ
	glBindTexture(GL_TEXTURE_2D, tex_id_face);
	//image�� texture�� ���
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageArray);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
}

//������ �ؽ���(����) ����
void init_texture() { //���� 6������ texture �����ص� ���� ����
	glGenTextures(nTex, tex_ids); //texture�� �迭�� �־���
	glEnable(GL_TEXTURE_2D);
}

//������Ʈ ����
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
	// 14 : ������
	// 15 : paper
	// 16 : fire

	glBindTexture(GL_TEXTURE_2D, 0); //texture �ʱ�ȭ
	//obj ���� parsing
	for (int o = 0; o < m.objs.size(); o++) {

		//obj ������ ���� idx �Ҵ�
		if (model_name == "tree") {
			if (o == 0) {
				idx = 0; //Ʈ��
			}
			else if (o == 1) {
				idx = 1; //Ʈ���ڽ�
			}
			else {
				idx = 3; //�˷ϴ޷� ��
			}
		}
		else if (model_name == "room1") {
			idx = 2; //��ũ��Ʈ
		}
		else if (model_name == "hammer") {
			if (o == 0) 
				idx = 2; //�ظӺκ�(��ũ��Ʈ)
			else 
				idx = 1; //������ �κ�(����)
		}
		else if (model_name == "desk") {
			idx = 1; //���̺�(����)
		}
		else if (model_name == "keybox") {
			if (o == 1 || o == 9)
				idx = 7; //�ڹ��� : �ݼ�
			else
				idx = 1; //���� : ����
		}
		else if (model_name == "keyboxopen") {
			if (o == 1 || o == 2)
				idx = 7; //����κ� : �ݼ�
			else
				idx = 1; //���� : ����
		}
		else if (model_name == "snowman") {
			if (o == 0 || o == 1 || o == 2)//���׾Ƹ�
				idx = 6;//����
			else if (o == 3 || o == 4 || o == 6 || o == 7) //����
				idx = 4;//������
			else if (o == 5) //��
				idx = 5;//��ٻ�
		}
		else if (model_name == "key") {
			idx = 7; //�ݼ�
		}
		else if (model_name == "door_locked") {
			if (o == 0 || o == 1 || o == 2)
				idx = 1; //��, �� ������ : ����
			else
				idx = 7;// �����ġ : �ݼ�
		}
		else if (model_name == "door_unlocked") {
			if (o == 3)
				idx = 7; //�� �� : �ݼ�
			else if (o == 4)
				idx = 4;// ������ �� : ������
			else
				idx = 1; //������ : ����
		}
		else if (model_name == "radio") {
			if (o == 0)
				idx = 1; //��ü : ����
			else if (o == 1 || o == 2)
				idx = 4; //����Ŀ, LCD : ������
			else
				idx = 7; //���׳�, ���̾� : �ݼ�
		}
		else if (model_name == "fireplace") {
			if (o <= 4)
				idx = 8; //���� ��ü : ����
			else
				idx = 1; //��ںҿ� ���� : ����
		}

		//texture ����
		glBindTexture(GL_TEXTURE_2D, tex_ids[idx]); //texture�� Ư�� id�� ����
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[idx].cols, image[idx].rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image[idx].data); //image�� texture�� ���
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //��Ƽ���ϸ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //��Ƽ���ϸ����

		int nFaces = m.objs[o].f.size(); //���� ����
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

				glNormal3f(nx, ny, nz); //����
				glTexCoord2f(tx, ty); //texture ��ǥ ����
				glVertex3f(x, y, z);
			}
			glEnd();
		}
	}
}

//ť�� ����
void drawCuboid(GLfloat sx, GLfloat sy, GLfloat sz) {
	glPushMatrix();
		glScalef(sx, sy, sz); //
		glutSolidCube(0.25);
	glPopMatrix();
}

//�� ����
void drawSphere(GLfloat r, GLfloat sx, GLfloat sy, GLfloat sz) {
	glPushMatrix();
		glScalef(sx, sy, sz); //
		glutSolidSphere(r, 20, 20);
	glPopMatrix();
}

//������ ����
void makeTeapot(GLfloat x, GLfloat y, GLfloat z) {
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, 0); //texture �ʱ�ȭ
		set_teapot_color();
		glTranslatef(x, y, z);
		glRotatef(-90, 0, 1, 0); //y�� ���� 90��ȸ��
		glutSolidTeapot(0.05);
	glPopMatrix();

}

//Ʈ�� ���� ����
void make_tree_ball() {
	glPushMatrix();
	set_ball_color();
	glBindTexture(GL_TEXTURE_2D, 0); //texture 

	//�߰�
	glTranslatef(1.2, 0.25, -0.45);
	drawSphere(0.03, 1, 1, 1);

	//������
	glPushMatrix();
	glTranslatef(-0.07, 0.1, -0.1);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	//��������
	glPushMatrix();
	glTranslatef(0.09, 0.05, -0.15);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	//���ʾƷ�
	glPushMatrix();
	glTranslatef(-0.15, -0.1, -0.03);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	//�����ʾƷ�
	glPushMatrix();
	glTranslatef(0.15, -0.1, -0.01);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	//�߰��Ʒ�
	glPushMatrix();
	glTranslatef(-0.05, -0.15, 0.04);
	drawSphere(0.03, 1, 1, 1);
	glPopMatrix();

	glPopMatrix();
}

//������Ʈ ����(display_objs) ȣ��
void make_objs() {
	//Ʈ�� ����
	glPushMatrix();
		glScalef(0.2, 0.2, 0.2);
		glTranslatef(6, 0, -3);     
		glRotatef(90, 0, 1, 0);
		display_objs(tree, "tree");
	glPopMatrix();

	//1��° �� �� ����
	glPushMatrix();
		glTranslatef(-1.5, 0.2, 0);
		glScalef(0.18, 0.1, 0.17);
		glRotatef(90, 0, 1, 0); //y�� ���� 90��ȸ��
		display_objs(room1, "room1");
	glPopMatrix();

	if (item[0] == 0) {
		//��ġ ����
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(2, 1.5, -4);
			glRotatef(90, 0, 0, 1); //z�� ���� 90��ȸ��
			display_objs(hammer, "hammer");
		glPopMatrix();
	}

	//å��1 ����
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(0.5, 1, -5);
		glRotatef(90, 0, 1, 0); //y�� ���� 90��ȸ��
		display_objs(desk, "desk");
	glPopMatrix();

	//å��2 ����
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(-6, 1, -5);
		glRotatef(90, 0, 1, 0); //y�� ���� 90��ȸ��
		display_objs(desk, "desk");
	glPopMatrix();

	//å��3 ����
	glPushMatrix();
		glScalef(0.2, 0.1, 0.1);
		glTranslatef(0, 1.5, 8);
		glRotatef(90, 0, 1, 0); //y�� ���� 90��ȸ��
		display_objs(desk, "desk"); //����
	glPopMatrix();

	if (is_keybox_open) { // �ڽ��� �����ִ� ������ ��
			//������� ����(���� ��)
			glPushMatrix();
				glScalef(0.1, 0.1, 0.1);
				glTranslatef(10, 0, 9);
				glRotatef(90, 0, 1, 0); // y�� ���� 90��ȸ��
				display_objs(keyboxopen, "keyboxopen"); // ������ڴ� å��� ���� ����������
			glPopMatrix();

		if (item[3] == 0) { // ���踦 �Ȱ������� ��
			// ���� ����(������ڿ� �ִ� ��)
			glPushMatrix();
				glScalef(0.1, 0.1, 0.1);
				glTranslatef(10, 0, 9);
				glRotatef(90, 0, 1, 0); //y�� ���� 90��ȸ��
				display_objs(key, "key"); //������ڴ� å��� ���� ����������
			glPopMatrix();
		}
	}
	else { // �ڽ��� �������� ��
		// ������� ����(���� ��)
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(10, 0, 9);
			glRotatef(90, 0, 1, 0); // y�� ���� 90��ȸ��
			display_objs(keybox, "keybox"); // ������ڴ� å��� ���� ����������
		glPopMatrix();
	}

	//ĳ����(�����) ����
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(char_x * 10, char_y * 10, char_z * 10); //������
		glRotatef(char_state_angle, 0, 1, 0); //y�� ���� �ݽð� ȸ��
		display_objs(snowman, "snowman"); //�ӽ� �ؽ���
	glPopMatrix();

	//����
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(-5, 1.5, -4);
		glRotatef(-90, 0, 1, 0); //y�� ���� -90��ȸ��
		display_objs(radio, "radio");
	glPopMatrix();

	if (!is_door_open) { //���� ��������
		//��� ��
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(-3, 0.8, -6.8);
			glRotatef(180, 0, 1, 0); //y�� ���� 90��ȸ��
			display_objs(door_locked, "door_locked"); // ���� ����, �����ġ�� �ݼ�
		glPopMatrix();
	}
	else { //���� ����
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(-11.3, 0, -7);
			glRotatef(-90, 0, 1, 0); //y�� ���� 90��ȸ��
			display_objs(door_unlocked, "door_unlocked"); // ���� ����, �����ġ�� �ݼ�
		glPopMatrix();
		is_game_dome = 1; //���� ��
	}

	//������ ����
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glTranslatef(-10, 0, 9);
		glRotatef(90, 0, 1, 0); //y�� ���� 90��ȸ��
		display_objs(fireplace, "fireplace"); //���� ����(��������, ���� : ����)
	glPopMatrix();

	if (fireplace_key_display && !(item[2] == 1)) { //���� ���� ���踦 ������ �ʾ��� ��
		//���� ����(������ �ȿ� �ִ� ��)
		glPushMatrix();
			glScalef(0.1, 0.1, 0.1);
			glTranslatef(-10, 0, 9);
			glRotatef(90, 0, 1, 0); //y�� ���� 90��ȸ��
			display_objs(key, "key"); //������ڴ� å��� ���� ����������
		glPopMatrix();
	}

	if (item[1] == 0) {
		makeTeapot(-0.1, 0.15, -0.5);
	}
	make_tree_ball();
}

//���� ǥ�� �ɱ�
void make_face(int mode) {
	init_texture_face(image[mode].data, image[mode].cols, image[mode].rows);

	glPushMatrix();
		glTranslatef(0.6, 0.2, -0.9);
		glScalef(0.15, 0.15, 0.15);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			//texture 1��° ��
			glTexCoord2f(0, 1);
			glVertex3f(v[0][0], v[0][1], v[0][2]);
			//texture 2��° ��
			glTexCoord2f(0, 0);
			glVertex3f(v[1][0], v[1][1], v[1][2]);
			//texture 3��° ��
			glTexCoord2f(1, 0);
			glVertex3f(v[2][0], v[2][1], v[2][2]);
			//texture 4��° ��
			glTexCoord2f(1, 1);
			glVertex3f(v[3][0], v[3][1], v[3][2]);
		glEnd();
	glPopMatrix();
}

//������ Ű�� ������ ��
void make_fire() {
	init_texture_face(image[16].data, image[16].cols, image[16].rows);

	glPushMatrix();
		glTranslatef(-1, 0.15, 0.62);
		glScalef(0.3, 0.15, 0.15);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		//texture 1��° ��
		glTexCoord2f(0, 1);
		glVertex3f(v[0][0], v[0][1], v[0][2]);
		//texture 2��° ��
		glTexCoord2f(0, 0);
		glVertex3f(v[1][0], v[1][1], v[1][2]);
		//texture 3��° ��
		glTexCoord2f(1, 0);
		glVertex3f(v[2][0], v[2][1], v[2][2]);
		//texture 4��° ��
		glTexCoord2f(1, 1);
		glVertex3f(v[3][0], v[3][1], v[3][2]);
		glEnd();
	glPopMatrix();
}

//���̺� �ִ� ���� ����
void make_paper(GLfloat x, GLfloat y, GLfloat z) {
	init_texture_face(image[15].data, image[15].cols, image[15].rows);

	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(0.07, 0.2, 0.1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		//texture 1��° ��
		glTexCoord2f(0, 1);
		glVertex3f(paper[0][0], paper[0][1], paper[0][2]);
		//texture 2��° ��
		glTexCoord2f(0, 0);
		glVertex3f(paper[1][0], paper[1][1], paper[1][2]);
		//texture 3��° ��
		glTexCoord2f(1, 0);
		glVertex3f(paper[2][0], paper[2][1], paper[2][2]);
		//texture 4��° ��
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
		if (tree_light) { //1�ʿ� 1�� Ʈ���� ���� ���� ���� �ٲ���
			for (int i = 0; i < 3; i++) {
				random_device rand;
				mt19937 gen(rand());
				uniform_int_distribution<int> dist(0, 99);
				ball_RGB[i] = dist(gen) / 100.0;
			}
		}

		if (fire_mode) {
			snowman_HP -= 1;
			cout << "������� ����� : " << snowman_HP << endl;
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
			//1��Ī
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
		////////////Ʈ�� �̺�Ʈ �߻� ����////////////
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

		////////////���� �̺�Ʈ �߻� ����////////////
		else if (char_x >= radio_range_x[0] && char_x <= radio_range_x[1] && char_z >= radio_range_z[0] && char_z <= radio_range_z[1]) {
			if (radio_mode == 0) {
				face_mode = 9;//^w^         
				radio_mode = 1;// ����
				PlaySound(NULL, 0, 0); //�����ʱ�ȭ
				PlaySound(TEXT(BGM_CAROL), 0, SND_ASYNC | SND_LOOP);
				print_event(dial_radio, 1);
			}
			else {
				face_mode = 10;//-m-
				radio_mode = 0; //����
				PlaySound(NULL, 0, 0); //�����ʱ�ȭ
				PlaySound(TEXT(BGM_FEAR), 0, SND_ASYNC | SND_LOOP);
				print_event(dial_radio, 1);
			}
		}

		/////////// ����1 �̺�Ʈ �߻� ���� ////////////////
		else if (char_x >= paper1_range_x[0] && char_x <= paper1_range_x[1] && char_z >= paper_range_z[0] && char_z <= paper_range_z[1]) {
			print_event(dial_paper1, 11);
		}
		/////////// ����2 �̺�Ʈ �߻� ���� ////////////////
		else if (char_x >= paper2_range_x[0] && char_x <= paper2_range_x[1] && char_z >= paper_range_z[0] && char_z <= paper_range_z[1]) {
			print_event(dial_paper2, 8);
		}
		
		/////////// ������ �̺�Ʈ �߻� ���� ///////////
		else if (char_x >= fireplace_range_x[0] && char_x <= fireplace_range_x[1] && char_z >= fireplace_range_z[0] && char_z <= fireplace_range_z[1]) {
			if (!fire_mode && !fireplace_key_display) {//���� �����ְ� ���踦 ��ã���� ��
				fire_mode = 1;
				face_mode = 12; //��� ǥ��
				print_event(dial_fireplace_on, 7);
			}
			else if (!fire_mode && fireplace_key_display && item[2] == 0) { //���� �����ְ� ���踦 ã���� ��(������ ����)
				cout << endl << "���踦 �ֿ���." << endl;
				cout << endl <<  "�� ����� ���ٰ� ����..?" << endl;
				item[2] = 1;
				face_mode = 12;
			}
			else if(fire_mode) { //���� �������� ��
				cout << "� ���� ���� ��." << endl;
			}
			else {
				cout << endl << "�����ΰ� �ִ�." << endl;
			}
		}

		/////////// ��ġ �̺�Ʈ �߻� ���� ////////////////
		else if (char_x >= hammer_range_x[0] && char_x <= hammer_range_x[1] && char_z >= hammer_range_z[0] && char_z <= hammer_range_z[1] && item[0] == 0) {
			item[0] = 1;
			cout << endl << "��ġ�� �ֿ���." << endl;
		}
		
		/////////// ������ �̺�Ʈ �߻� ���� ////////////////
		else if (char_x >= teapot_range_x[0] && char_x <= teapot_range_x[1] && char_z >= teapot_range_z[0] && char_z <= teapot_range_z[1] && item[1] == 0) {
			item[1] = 1;
			cout << endl << "�����ڸ� �ֿ���." << endl;
		}

		///////////������� �̺�Ʈ �߻� ���� ////////////////
		else if (char_x >= keybox_range_x[0] && char_x <= keybox_range_x[1] && char_z >= keybox_range_z[0] && char_z <= keybox_range_z[1]) {
			if (is_keybox_open) {
				if (item[3] == 0) { //���谡 ���ڿ� ���� ��
					cout << endl << "���踦 �� �ֿ���." << endl;
					cout << endl << "�� ����� �� ���ٰ� ����..?" << endl;
					item[3] = 1;
					face_mode = 12;
				}
				else { //���谡 ���ڿ� ���� ��
					cout << endl << "�� ���ڿ��� ���� ���� ����." << endl;
				}
			}
			else {
				cout << endl << "�ڹ���� ���� �����ִ� ���ڰ� �ִ�. �ȿ��� ���� ������?.." << endl;
			}
			
		}

		//������
		else if (char_x >= door_range_x[0] && char_x <= door_range_x[1] && char_z >= door_range_z[0] && char_z <= door_range_z[1]) {
			cout << "���� ���� �����ִ�." << endl;
		}
		////////////��Ÿ ����////////////
		else {
			face_mode = 14;//������
			print_event(dial_empty, 1);
		}

		break;

	//������ Ȯ��
	case 'x' :
		cout << "//////////////////////////////������ â///////////////////////////////////" << endl;

		if (item[0] == 0 && item[1] == 0 && item[2] == 0 && item[3] == 0) { //���� ��
			cout << endl << "������ �ִ� �������� ����." << endl;
			break;
		}

		for (int i = 0; i < 4; i++) {
			switch (i) {
			case 0 : 
				if(item[i] == 1)
					cout << "��ġ ";
				break;
			case 1 :
				if (item[i] == 1)
					cout << "������ ";
				break;
			case 2  :
				if (item[i] == 1)
					cout << "�����ο��� ã�� ���� ";
				break;
			case 3 :
				if (item[i] == 1)
					cout << "���ڿ��� ã�� ���� ";
				break;
			}
		}
		cout << endl;
		break;
	
	//������ ���
	case 'z' :
		if (item[0] == 0 && item[1] == 0) {
			cout << endl << "�������� ����." << endl;
			break;
		}

		if (char_x >= keybox_range_x[0] && char_x <= keybox_range_x[1] && char_z >= keybox_range_z[0] && char_z <= keybox_range_z[1]) { //������� �ֺ��� ���� ��
			if (item[0] == 1) { //��ġ�� ���� ��
				print_event(dial_keybox_hammer, 4);
				item[0] = 2;//��ġ�� �μ���
				face_mode = 14;
			}
			else { //��ġ�� ���� �� 
				if (item[2] == 1) { //���� 1�� ���� ��
					is_keybox_open = 1; //������� ����
					cout << endl << "���ڰ� ���ȴ�. �ȿ� �� �ٸ� ���谡 ����ִ�." << endl;
					item[2] = 0;
					face_mode = 12;
				}
				else if(!is_keybox_open){ //���� 1�� ���� ���ڰ� �ȿ����� ��
					cout << endl << "�� �ڹ���� ��� �ؾ� ������..?" << endl;
				}
			}
			break;
		}
		
		if (char_x >= fireplace_range_x[0] && char_x <= fireplace_range_x[1] && char_z >= fireplace_range_z[0] && char_z <= fireplace_range_z[1] && fire_mode == 1) { //������ �ֺ��� �ְ� ���� ���� ���� ��
			if (item[1] == 1) { //�����ڰ� ���� ��
				face_mode = 13; //������ ǥ��
				print_event(dial_fireplace_off, 3);
				fire_mode = 0;
				fireplace_key_display = 1; //���� ���̱�
				item[1] = 2;//������ ��� ��
			}
			else { //�����ڰ� ���� ��
				cout << endl << "� ���� ���� ���� �Ѵ�." << endl;
			}
			break;
		}

		//������ 
		if (char_x >= door_range_x[0] && char_x <= door_range_x[1] && char_z >= door_range_z[0] && char_z <= door_range_z[1]) {
			if (!is_door_open && item[3] == 1) {//���� ����ְ� ���谡 ���� ��
				is_door_open = 1;
				item[3] == 0;
				face_mode = 12;
				cout << endl << "���� ���ȴ� !!!!! ���� Ż������!!!!!!" << endl;
		}
			break;
		}
		break;

	case 'h':  
		cout << endl << "///////////////////////////////////////////////////////////////////////////////////" << endl;
		cout << endl << "  �����̱� : ����Ű    �ൿ : a    ������ȯ(�������) : c      ������ Ȯ�� : x      ������ ��� : z     ����Ű Ȯ�� : h" << endl;
		cout << endl << "///////////////////////////////////////////////////////////////////////////////////" << endl;
		break;

	default:
		break;
	}
}

//����Ű ����
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

	//�ؽ��� �̹��� �о����
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
	// 14 : ������
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

	//face �̹��� �о����
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

	image[14] = imread("./face/������.jpg", IMREAD_COLOR);
	cvtColor(image[14], image[14], COLOR_BGR2RGB);

	image[15] = imread("./Tex/paper_texture.jpg", IMREAD_COLOR);
	cvtColor(image[15], image[15], COLOR_BGR2RGB);

	image[16] = imread("./Tex/fire_texture.jpg", IMREAD_COLOR);
	cvtColor(image[16], image[16], COLOR_BGR2RGB);

	//�� �о����
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

	PlaySound(NULL, 0, 0); //�����ʱ�ȭ
	PlaySound(TEXT(BGM_FEAR), 0, SND_ASYNC | SND_LOOP); //���� ���

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