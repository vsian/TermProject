#pragma once
#include"framework.h"
#include"TermProject.h"
class Game {
public:
	int GameState = 0;
	//������Ա���
	HRESULT hr;

	//d2dȫ�ֱ���
	ID2D1Factory* pD2DFactory = NULL;
	ID2D1HwndRenderTarget* pRenderTarget = NULL;
	D2D1_RECT_F D2dRcF = D2D1::RectF(0.f, 0.f, WINDOWWIDTH, WINDOWHEIGHT);
	IDWriteFactory* pIDWriteFactory = NULL;
	IDWriteTextFormat* pTextFormat = NULL;
	ID2D1SolidColorBrush* pBlackBrush = NULL;
	D2D1_RECT_F energyAndScoreRect;
	//�������ȫ�ֱ���
	const wchar_t AppName[5] = L"MAIN";
	HWND MainHwnd;
	MSG MainMsg;
	WNDCLASS MainWin = {};
	RECT rc;

	//WIC���ȫ�ֱ���
	IWICImagingFactory* pIwicImgFactory = NULL;

	//��Ϸ�ж��󴴽�
	
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

	//��Ϸ��Ⱦ����������ݹ��̺���
	INT Paint();
	INT SetVelocity();
	INT CrushJudge();

	//���̺߳���
	HANDLE Thread_Generate, Thread_Shoot;
	DWORD id_Generate, id_Shoot;
	static DWORD WINAPI Generate(LPVOID LpParameter);
	static DWORD WINAPI Shoot(LPVOID LpParameter);

	//����֡��
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