#include "Game.h"
Game pGame;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	pGame.Init(hInstance, nCmdShow, WindowProc);
	pGame.Start();
	return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
		case WM_DESTROY:
		{
			PostThreadMessage(pGame.id_Shoot, WM_QUIT, 0, 0);
			PostThreadMessage(pGame.id_Generate, WM_QUIT, 0, 0);
			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case 0x41:
			{
				pGame.player->setVX(-5);
				return 0;
			}
			case 0x44:
			{
				pGame.player->setVX(5);
				return 0;
			}
			case 0x57:
			{
				pGame.player->setVY(-5);
				return 0;
			}
			case 0x53:
			{
				pGame.player->setVY(5);
				return 0;
			}
			case 0x46:
			{
				if (pGame.player->getEnergy() >= 30) {
					pGame.player->addEnergy(-30);
					pGame.allExplode();
				}
				return 0;
			}
			case 0x47:
			{
				if (pGame.player->getEnergy() >= 10) {
					pGame.player->addEnergy(-10);
					pGame.SuperBulletList->shoot(pGame.player);
				}
				return 0;
			}
			case VK_SPACE:
			{
				pGame.player->blink();
				return 0;
			}
			case VK_SHIFT:
			{
				pGame.player->slowMove();
				return 0;
			}
			default:
				return 0;
			}
		}
		case WM_KEYUP:
		{
			switch (wParam)
			{
			case 0x41:
			{
				if (pGame.player->getVX() < 0) {
					pGame.player->setVX(0);
				}
				return 0;
			}
			case 0x44:
			{
				if (pGame.player->getVX() > 0) {
					pGame.player->setVX(0);
				}
				return 0;
			}
			case 0x57:
			{
				if (pGame.player->getVY() < 0) {
					pGame.player->setVY(0);
				}
				return 0;
			}
			case 0x53:
			{
				if (pGame.player->getVY() > 0) {
					pGame.player->setVY(0);
				}
				return 0;
			}
			default:
				return 0;
			}
		}
		default:
			break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}