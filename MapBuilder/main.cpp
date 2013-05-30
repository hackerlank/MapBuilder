#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <iostream>
//#include <afxdlgs.h>
#include "MapBuilder.h"
#include "Constants.h"

using namespace std;

//回调函数;
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
	//构建地图;
	MapBuilder::Instance()->BulidMap(10, 10);

	//构建gl绘制部分，显示地图;
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

	//创建菜单;
	CreateMenus();

	glutMainLoop();

	return;
}

//-----------------------------------------ChangeSize-----------------------------------------
//回调函数，用于渲染;
//--------------------------------------------------------------------------------------------
void RenderScene()
{
	//用当前颜色清除窗口;
	glClear(GL_COLOR_BUFFER_BIT);

	//变换opengl的坐标轴原点;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, windowWith, windowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//并开始绘制;
	MapBuilder::Instance()->Render();

	//重新绘制;
	glutPostRedisplay();

	//刷新;
	glutSwapBuffers();
}


//-----------------------------------------ChangeSize-----------------------------------------
//回调函数，设置渲染前的准备工作;
//--------------------------------------------------------------------------------------------
void SetupRc()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


//-----------------------------------------ChangeSize-----------------------------------------
//回调函数，当窗口大小改变时调用;
//--------------------------------------------------------------------------------------------
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;

	//防止除零;
	if (h == 0)
	{
		h = 1;
	}

	//设置视口;
	glViewport(0, 0, w, h);

	//重置投影矩阵堆栈;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
	{
		//正投影函数;
		glOrtho(-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	}
	else
	{
		glOrtho(-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
	}

	glutReshapeWindow(windowWith, windowHeight);

}


//-----------------------------------------MouseButton-----------------------------------------
//传递给glutMouseFunc，当鼠标点击时调用;
//--------------------------------------------------------------------------------------------
void MouseButton(int button, int state, int x, int y)
{
	switch(button)
	{
	//鼠标左键;
	case GLUT_LEFT_BUTTON:   
		switch(state)
		{
			//鼠标按下;
		case GLUT_DOWN:
			MapBuilder::Instance()->SetCellBrush(x, y);
			printf("鼠标左键按下: %d,%d\n", x, y);
			break;
			//鼠标释放;
		case GLUT_UP:
			printf("鼠标左键释放\n");
			break;
		default:
			break;
		}
	default:
		break;
	}
}


//-----------------------------------------MouseMotion-----------------------------------------
//传递给glutMousMotionFunc，当鼠标拽动（也就是当鼠标按下并移动）时调用;
//--------------------------------------------------------------------------------------------
void MouseMotion(int x, int y)
{
	MapBuilder::Instance()->SetCellBrush(x, y);
	printf ("mouse drag position: %d, %d. \n", x, y);
}


//-----------------------------------------MouseMotion-----------------------------------------
//传递给glutKeyboardFunc，当发生键盘事件时调用;
//--------------------------------------------------------------------------------------------
void KeyboardFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:   //代表ESC键;
		//退出时先保存地图信息;
		MapBuilder::Instance()->SaveMap("map.xml");  
		exit(1);
	case 13:  //回车键;
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
	case 'l':  //加载指定地图;
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
//创建系统的菜单，用于处理保存地图，加载地图等;
//--------------------------------------------------------------------------------------------
void CreateMenus()
{
	int menu;

	//创建菜单并通知GLUT，processMenuEvent处理菜单事件;
	menu = glutCreateMenu(ProcessMenuEvents);

	//给菜单添加条目;
	glutAddMenuEntry("New", NEW);

	glutAddMenuEntry("Load", LOAD);

	glutAddMenuEntry("Save", SAVE);

	glutAddMenuEntry("Quit", QUIT);

	//把菜单和鼠标左键关联起来;
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


//-----------------------------------------ProcessMenuEvents-----------------------------------------
//处理菜单响应事件;
//--------------------------------------------------------------------------------------------
void ProcessMenuEvents(int option)
{
	//option表示菜单项传递过来的值;
	switch(option)
	{
	case NEW:
		MapBuilder::Instance()->ClearMap();
		break;
	case LOAD:  //保存界面;
		//CFileDialog loadDia(TRUE, ".xml", NULL, 
			//OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			//"(*.xml)|*.xml|");
		MapBuilder::Instance()->LoadMap("map.xml");
		break;
	case SAVE:  //加载界面;
		MapBuilder::Instance()->SaveMap("map.xml");
		break;
	case QUIT:
		exit(1);
		break;
	default:
		break;
	}
}