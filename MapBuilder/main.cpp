#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <iostream>
//#include <afxdlgs.h>
#include "MapBuilder.h"
#include "Constants.h"

using namespace std;

//�ص�����;
void RenderScene();
void SetupRc();
void ChangeSize(GLsizei w, GLsizei h);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void KeyboardFunc(unsigned char key, int x, int y);
void CreateMenus();
void ProcessMenuEvents(int option);


void main(int argc, char* argv[])
{
	//������ͼ;
	MapBuilder::Instance()->BulidMap(10, 10);

	//����gl���Ʋ��֣���ʾ��ͼ;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(windowWith, windowHeight);
	glutCreateWindow("Keanu");
	glutReshapeFunc( ChangeSize );
	glutDisplayFunc( RenderScene );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutKeyboardFunc( KeyboardFunc );

	SetupRc();

	//�����˵�;
	CreateMenus();

	glutMainLoop();

	return;
}

//-----------------------------------------ChangeSize-----------------------------------------
//�ص�������������Ⱦ;
//--------------------------------------------------------------------------------------------
void RenderScene()
{
	//�õ�ǰ��ɫ�������;
	glClear(GL_COLOR_BUFFER_BIT);

	//�任opengl��������ԭ��;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, windowWith, windowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//����ʼ����;
	MapBuilder::Instance()->Render();

	//���»���;
	glutPostRedisplay();

	//ˢ��;
	glutSwapBuffers();
}


//-----------------------------------------ChangeSize-----------------------------------------
//�ص�������������Ⱦǰ��׼������;
//--------------------------------------------------------------------------------------------
void SetupRc()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


//-----------------------------------------ChangeSize-----------------------------------------
//�ص������������ڴ�С�ı�ʱ����;
//--------------------------------------------------------------------------------------------
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;

	//��ֹ����;
	if (h == 0)
	{
		h = 1;
	}

	//�����ӿ�;
	glViewport(0, 0, w, h);

	//����ͶӰ�����ջ;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
	{
		//��ͶӰ����;
		glOrtho(-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	}
	else
	{
		glOrtho(-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
	}

	glutReshapeWindow(windowWith, windowHeight);

}


//-----------------------------------------MouseButton-----------------------------------------
//���ݸ�glutMouseFunc���������ʱ����;
//--------------------------------------------------------------------------------------------
void MouseButton(int button, int state, int x, int y)
{
	switch(button)
	{
	//������;
	case GLUT_LEFT_BUTTON:   
		switch(state)
		{
			//��갴��;
		case GLUT_DOWN:
			MapBuilder::Instance()->SetCellBrush(x, y);
			printf("����������: %d,%d\n", x, y);
			break;
			//����ͷ�;
		case GLUT_UP:
			printf("�������ͷ�\n");
			break;
		default:
			break;
		}
	default:
		break;
	}
}


//-----------------------------------------MouseMotion-----------------------------------------
//���ݸ�glutMousMotionFunc�������ק����Ҳ���ǵ���갴�²��ƶ���ʱ����;
//--------------------------------------------------------------------------------------------
void MouseMotion(int x, int y)
{
	MapBuilder::Instance()->SetCellBrush(x, y);
	printf ("mouse drag position: %d, %d. \n", x, y);
}


//-----------------------------------------MouseMotion-----------------------------------------
//���ݸ�glutKeyboardFunc�������������¼�ʱ����;
//--------------------------------------------------------------------------------------------
void KeyboardFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:   //����ESC��;
		//�˳�ʱ�ȱ����ͼ��Ϣ;
		MapBuilder::Instance()->SaveMap("map.xml");  
		exit(1);
	case 13:  //�س���;
		MapBuilder::Instance()->ClearMap();
		break;
	case '1':
		MapBuilder::Instance()->SetBrush(bNORMAL);
		break;
	case '2':
		MapBuilder::Instance()->SetBrush(bWALL);
		break;
	case '3':
		MapBuilder::Instance()->SetBrush(bWATER);
		break;
	case 'l':  //����ָ����ͼ;
	case 'L':
		MapBuilder::Instance()->LoadMap("map.xml");
		break;
	case 's':
	case 'S':
		MapBuilder::Instance()->SetBrush(bSOURCE);
		break;
	case 'd':
	case 'D':
		MapBuilder::Instance()->SetBrush(bDEST);
		break;
	case 'p':
	case 'P':
		MapBuilder::Instance()->CreatePath();
		break;
	case 'o':
	case 'O':
		MapBuilder::Instance()->TestOut();
		break;
	default:
		break;
	}
}


//-----------------------------------------CreateMenus-----------------------------------------
//����ϵͳ�Ĳ˵������ڴ������ͼ�����ص�ͼ��;
//--------------------------------------------------------------------------------------------
void CreateMenus()
{
	int menu;

	//�����˵���֪ͨGLUT��processMenuEvent����˵��¼�;
	menu = glutCreateMenu(ProcessMenuEvents);

	//���˵������Ŀ;
	glutAddMenuEntry("New", NEW);

	glutAddMenuEntry("Load", LOAD);

	glutAddMenuEntry("Save", SAVE);

	glutAddMenuEntry("Quit", QUIT);

	//�Ѳ˵�����������������;
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


//-----------------------------------------ProcessMenuEvents-----------------------------------------
//����˵���Ӧ�¼�;
//--------------------------------------------------------------------------------------------
void ProcessMenuEvents(int option)
{
	//option��ʾ�˵���ݹ�����ֵ;
	switch(option)
	{
	case NEW:
		MapBuilder::Instance()->ClearMap();
		break;
	case LOAD:  //�������;
		//CFileDialog loadDia(TRUE, ".xml", NULL, 
			//OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			//"(*.xml)|*.xml|");
		MapBuilder::Instance()->LoadMap("map.xml");
		break;
	case SAVE:  //���ؽ���;
		MapBuilder::Instance()->SaveMap("map.xml");
		break;
	case QUIT:
		exit(1);
		break;
	default:
		break;
	}
}