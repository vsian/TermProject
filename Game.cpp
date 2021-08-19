#include "Game.h"

INT Game::Paint()
{
	//RenderTarget的特性是双缓冲 向Target绘画的过程在EndDraw()之后全部完成，不用手写双缓冲.
	pMap->mapShow(pRenderTarget);
	player->draw();
	TracerList->Draw();
	AttackerList->Draw();
	EliteList->Draw();
	EnemyBulletList->Draw();
	PlayerBulletList->Draw();
	SuperBulletList->Draw();
	EliteBulletList->Draw();
	swprintf_s(line, 100, L"SCORE:%d\nENERGY:%d", player->getScore(), player->getEnergy());
	pRenderTarget->DrawText(line, 30, pTextFormat, energyAndScoreRect, pBlackBrush);
	return 0;
}

INT Game::SetVelocity()
{
	EliteList->setVelocity();
	AttackerList->setVelocity();
	TracerList->setVelocity(player);

	SuperBulletList->setVelocity();
	EliteBulletList->setVelocity();
	PlayerBulletList->setVelocity();
	EnemyBulletList->setVelocity();
	return 0;
}

INT Game::CrushJudge()
{
	AttackerList->Crush(player);
	TracerList->Crush(player);
	EliteList->Crush(player);
	EnemyBulletList->Crush(player);
	EliteBulletList->Crush(player);
	PlayerBulletList->Crush(AttackerList->_list);
	PlayerBulletList->Crush(TracerList->_list);
	PlayerBulletList->Crush(EliteList->_list);
	SuperBulletList->Crush(AttackerList->_list);
	SuperBulletList->Crush(TracerList->_list);
	SuperBulletList->Crush(EliteList->_list);

	SuperBulletList->clearTrash();
	EnemyBulletList->clearTrash();
	PlayerBulletList->clearTrash();
	TracerList->clearTrash(player);
	AttackerList->clearTrash(player);
	EliteList->clearTrash(player);
	return 0;
}

DWORD __stdcall Game::Generate(LPVOID LpParameter)
{
	MSG exitFlag;
	Game* pGame = (Game*)LpParameter; //将传入参数转化为Game类的指针
	while (1) {
		Sleep(rand() % 1001);	//随机间隔 0-1s
		if (PeekMessage(&exitFlag, NULL, WM_QUIT, WM_QUIT, PM_NOREMOVE)) {	//若传入退出程序的消息，则循环退出，线程结束
			break;
		}
		if (pGame->GameState == 1) {			//若在游戏中
			pGame->AttackerList->generate();	//随机生成敌人
			pGame->TracerList->generate();
			pGame->EliteList->generate();
		}
		Sleep(rand() % 1001);
	}
	pGame->TracerList->resetToEmpty();
	pGame->AttackerList->resetToEmpty();
	pGame->EliteList->resetToEmpty();
	return 0;
}

DWORD __stdcall Game::Shoot(LPVOID LpParameter)
{
	MSG exitFlag;
	Game* pGame = (Game*)LpParameter;
	while (1) {
		if (PeekMessage(&exitFlag, NULL, WM_QUIT, WM_QUIT, PM_NOREMOVE)) {
			break;
		}
		if (pGame->GameState == 1) {
			pGame->PlayerBulletList->shoot(pGame->player);
			pGame->EnemyBulletList->listShoot(pGame->AttackerList->_list);
			pGame->EliteBulletList->listShoot(pGame->EliteList->_list);
		}
		Sleep(500);		//子弹发射间隔500ms
	}
	return 0;
}

INT Game::Init(HINSTANCE hInstance, int nCmdShow, WNDPROC windowProc)
{
	//windows类的初始化
	MainWin.style = CS_HREDRAW | CS_VREDRAW;
	MainWin.lpfnWndProc = windowProc;
	MainWin.lpszClassName = AppName;
	MainWin.hInstance = hInstance;

	RegisterClass(&MainWin);
	MainHwnd = CreateWindowEx(
		0,
		AppName,
		L"FLYING COMBAT",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, //固定窗口的大小 禁止其变化
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOWWIDTH, WINDOWHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (MainHwnd == NULL) return -1;

	//d2d的初始化
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr)) {
		MessageBox(MainHwnd, L"Create D2D factory failed!", L"Error", MB_OK);
		return -1;
	}
	GetClientRect(MainHwnd, &rc);
	energyAndScoreRect = D2D1::RectF(
		static_cast<FLOAT>(rc.left),
		static_cast<FLOAT>(rc.top),
		static_cast<FLOAT>((rc.right - rc.left)),
		static_cast<FLOAT>((rc.bottom - rc.top) / 4)
	);
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(MainHwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&pRenderTarget
	);
	if (FAILED(hr)) {
		MessageBox(MainHwnd, L"Create render target failed!", L"Error", 0);
		return -1;
	}

	hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&pBlackBrush
	);
	if (FAILED(hr))
	{
		MessageBox(MainHwnd, L"Create brush failed!", L"Error", 0);
		return -1;
	}

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pIDWriteFactory)
	);
	
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create DirectWrite factory failed!", L"Error", 0);
		return -1;
	}

	hr = pIDWriteFactory->CreateTextFormat(
		L"7pxbus",                     // Font family name
		NULL,                          // Font collection(NULL sets it to the system font collection)
		DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		DWRITE_FONT_STYLE_NORMAL,      // Style
		DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		20.0f,                         // Size    
		L"en-us",                      // Local
		&pTextFormat                   // Pointer to recieve the created object
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create IDWriteTextFormat failed!", L"Error", 0);
		return -1;
	}
	//WIC初始化
	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		MessageBox(MainHwnd, L"IWIC initialize failed!", L"Error", 0);
		return -1;
	}
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pIwicImgFactory)
	);
	if (FAILED(hr)) {
		MessageBox(MainHwnd, L"Create IWICImageFactory failed!", L"Error", 0);
		return -1;
	}

	//游戏对象创建
	EnemyBulletList = new BulletManage<EnemyBullet>;
	PlayerBulletList = new BulletManage<PlayerBullet>;
	SuperBulletList = new BulletManage<PlayerSuperBullet>;
	EliteBulletList = new BulletManage<EliteBullet>;
	TracerList = new EnemyManage<Tracer>(4);
	AttackerList = new EnemyManage<Attacker>(6);
	EliteList = new EnemyManage<Elite>(1);
	explodeAnime = new Animation(pRenderTarget);

	explodeAnime->append(pIwicImgFactory, L"resource\\EXPLOSION_1.png", 30, 30);
	explodeAnime->append(pIwicImgFactory, L"resource\\EXPLOSION_2.png", 30, 30);
	explodeAnime->append(pIwicImgFactory, L"resource\\EXPLOSION_3.png", 30, 30);
	explodeAnime->append(pIwicImgFactory, L"resource\\EXPLOSION_4.png", 30, 30);
	explodeAnime->append(pIwicImgFactory, L"resource\\EXPLOSION_5.png", 30, 30);
	explodeAnime->append(pIwicImgFactory, L"resource\\EXPLOSION_6.png", 30, 30);
	explodeAnime->append(pIwicImgFactory, L"resource\\EXPLOSION_7.png", 30, 30);
	explodeAnime->append(pIwicImgFactory, L"resource\\EXPLOSION_8.png", 30, 30);

	player = new Player(
		L"resource\\PLAYER.png",
		explodeAnime,
		30, 40, 20, 30,
		pRenderTarget, pIwicImgFactory,
		250, 600
		);

	//player->setLife(-1);
	//player->addEnergy(500);

	pMap = new Map(L"resource\\BACKGROUND.png", SCROLLSPEED, pRenderTarget, pIwicImgFactory);

	SuperBulletList ->_pObjects = new FlyingObjects(
		L"resource\\SUPER_BULLET.png",
		explodeAnime,
		92, 32, 90, 30, pRenderTarget, pIwicImgFactory
	);
	EliteBulletList->_pObjects = new FlyingObjects(
		L"resource\\ELITE_BULLET.png",
		explodeAnime,
		18, 38, 16, 36, pRenderTarget, pIwicImgFactory
	);
	EnemyBulletList->_pObjects = new FlyingObjects(
		L"resource\\BULLET.png",
		explodeAnime,
		10, 10, 8, 8, pRenderTarget, pIwicImgFactory
	);
	PlayerBulletList->_pObjects = new FlyingObjects(
		L"resource\\PLAYER_BULLET.png",
		explodeAnime,
		12, 22, 8, 18, pRenderTarget, pIwicImgFactory
	);
	TracerList->_pObjects = new Enemy(
		L"resource\\TRACER.png",
		explodeAnime,
		26, 30, 14, 22,
		pRenderTarget, pIwicImgFactory,
		2, 10
	);
	AttackerList->_pObjects = new Enemy(
		L"resource\\ATTACKER.png",
		explodeAnime,
		42, 50, 20, 30, pRenderTarget, pIwicImgFactory,
		1, 20
	);
	EliteList->_pObjects = new Enemy(
		L"resource\\ELITE.png",
		explodeAnime,
		66, 40, 66, 40, pRenderTarget, pIwicImgFactory,
		5, 50
	);

	pStartImg = new ImageR(
		pRenderTarget, pIwicImgFactory,
		L"resource\\START.png",
		WINDOWWIDTH, WINDOWHEIGHT
	);

	pHelpImg = new ImageR(
		pRenderTarget, pIwicImgFactory,
		L"resource\\HELP.png",
		WINDOWWIDTH, WINDOWHEIGHT
	);

	ShowWindow(MainHwnd, nCmdShow);
	SetFocus(MainHwnd);

	Thread_Generate = CreateThread(NULL, 0, Generate, (LPVOID)this, 0, &id_Generate);
	Thread_Shoot = CreateThread(NULL, 0, Shoot, (LPVOID)this, 0, &id_Shoot);
}

void Game::Start()
{
	while (MainMsg.message != WM_QUIT) {
		if (PeekMessage(&MainMsg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&MainMsg);
			DispatchMessage(&MainMsg);
		}
		else {
			pRenderTarget->BeginDraw();
			timeBegin = GetTickCount(); //得到开始渲染之前的系统时间
			switch (GameState)
			{
				case 0:
					startPage();
					break;
				case 1:
					inGame();
					break;
				case 2:
					endGame();
					break;
				default:
					return;
			}
			pRenderTarget->EndDraw();
			timePhase = GetTickCount() - timeBegin; //渲染完毕之后得到渲染一帧所用时间
			if (timePhase < timeInPerFrame) {
				Sleep(timeInPerFrame - timePhase); //若帧数过高，自动调用sleep降帧
			}
		}
	}
}

void Game::startPage()
{
	ImageR* pImg = pStartImg;
	if (GetAsyncKeyState(0x48)) {
		pImg = pHelpImg;
	}
	if (GetAsyncKeyState(VK_SPACE)) {
		GameState = 1;
	}
	pMap->mapShow(pRenderTarget);
	pImg->Draw(0, 0);
}

void Game::inGame()
{
	if (!player->isAlive()) {
		GameState = 2;
		return;
	}
	SetVelocity();
	CrushJudge();
	Paint();
}

void Game::endGame()
{
	swprintf_s(line, 100, L"Your final score is %d\nContinue?", player->getScore());
	if (MessageBox(MainHwnd, line, L"You Died...", MB_YESNO) == IDNO) {
		GameState = 3;
		return;
	}
	else {
		reset();
		GameState = 1;
	}
}

void Game::allExplode()
{
	TracerList->allSetExplode();
	AttackerList->allSetExplode();
	EliteList->allSetExplode();
	EnemyBulletList->allSetExplode();
	EliteBulletList->allSetExplode();
	SuperBulletList->allSetExplode();
}

void Game::reset()
{
	TracerList->resetToEmpty();
	AttackerList->resetToEmpty();
	EliteList->resetToEmpty();
	PlayerBulletList->resetToEmpty();
	EnemyBulletList->resetToEmpty();
	EliteBulletList->resetToEmpty();
	SuperBulletList->resetToEmpty();
	memset(line, 0, sizeof(line));
	player->resetScore();
	player->resetEnergy();
	player->setX(250);
	player->setY(600);
	player->setVX(0);
	player->setVY(0);
	player->setExplode(false);
	player->setLive(true);
	player->setLife(1);
}
