#pragma once
#include"framework.h"
#include"TermProject.h"
class Game {
public:
	int GameState = 0;
	//程序调试变量
	HRESULT hr;

	//d2d全局变量
	ID2D1Factory* pD2DFactory = NULL;
	ID2D1HwndRenderTarget* pRenderTarget = NULL;
	D2D1_RECT_F D2dRcF = D2D1::RectF(0.f, 0.f, WINDOWWIDTH, WINDOWHEIGHT);
	IDWriteFactory* pIDWriteFactory = NULL;
	IDWriteTextFormat* pTextFormat = NULL;
	ID2D1SolidColorBrush* pBlackBrush = NULL;
	D2D1_RECT_F energyAndScoreRect;
	//窗口相关全局变量
	const wchar_t AppName[5] = L"MAIN";
	HWND MainHwnd;
	MSG MainMsg;
	WNDCLASS MainWin = {};
	RECT rc;

	//WIC相关全局变量
	IWICImagingFactory* pIwicImgFactory = NULL;

	//游戏中对象创建
	
	wchar_t line[100];
	Animation* explodeAnime = NULL;
	ImageR* pStartImg = NULL;
	ImageR* pHelpImg = NULL;
	Map* pMap = NULL;
	Player* player = NULL;
	EnemyManage<Tracer>* TracerList = NULL;
	EnemyManage<Attacker>* AttackerList = NULL;
	EnemyManage<Elite>* EliteList = NULL;
	BulletManage<PlayerBullet>* PlayerBulletList = NULL;
	BulletManage<PlayerSuperBullet>* SuperBulletList = NULL;
	BulletManage<EnemyBullet>* EnemyBulletList = NULL;
	BulletManage<EliteBullet>* EliteBulletList = NULL;

	//游戏渲染画面更新数据过程函数
	INT Paint();
	INT SetVelocity();
	INT CrushJudge();

	//多线程函数
	HANDLE Thread_Generate, Thread_Shoot;
	DWORD id_Generate, id_Shoot;
	static DWORD WINAPI Generate(LPVOID LpParameter);
	static DWORD WINAPI Shoot(LPVOID LpParameter);

	//限制帧数
	const float constFps = 80.00f;
	DWORD timeInPerFrame = 1000.0f / constFps;
	DWORD timeBegin, timePhase;

	INT Init(HINSTANCE hInstance, int nCmdShow, WNDPROC windowProc);
	void Start(), startPage(), inGame(), endGame(), allExplode(), reset();
	~Game() {
		SAFE_RELEASE(pD2DFactory);
		SAFE_RELEASE(pRenderTarget);
		SAFE_RELEASE(pIwicImgFactory);
		
		delete explodeAnime;
		delete pStartImg;
		delete pHelpImg;
		delete pMap;
		delete player;
		delete TracerList;
		delete AttackerList;
		delete EliteList;
		delete PlayerBulletList;
		delete EnemyBulletList;
		delete EliteBulletList;
		delete SuperBulletList;
	}
};