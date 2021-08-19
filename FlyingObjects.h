#pragma once
#include "Vec2.h"
#include "Animation.h"
#include "Data.h"

class FlyingObject;
class FlyingObjects;
class AutoFlight;
template<typename T, typename bullet = AutoFlight> struct FlyingObjectsList;
//飞行物体的位置，存活信息
class FlyingObject {
protected:
	INT _life = 1;
	Vec2 _pos, _velocity, _bounding_box_leftup, _bounding_box_rightbottom;
	bool _live = true, _explode = false;
public:
	INT _explodeFrames = 0;
	FlyingObject(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = 1) :
		_pos(x, y), _velocity(0, 0),
		_bounding_box_leftup(x + (W - BW) / 2, y + (H - BH) / 2),
		_bounding_box_rightbottom(_bounding_box_leftup._x + BW, _bounding_box_leftup._y + BH),
		_life(life){}
	INT getX(), getY(), getVX(), getVY(), getLeftupX(), getLeftupY(), getRightbottomX(), getRightbottomY();
	void setVX(INT x), setVY(INT y), setX(INT x), setY(INT y);
	bool isAlive(), explode();
	void setLive(bool f), setExplode(bool f), setLife(int x);
};

//飞行物体的图片，宽高，碰撞盒宽高
class FlyingObjects {
protected:
	Animation* _explode;
	Vec2 _size, _bounding_box;
	ImageR _img;
public:
	FlyingObjects(PCWSTR filename, Animation* pAnimation,
		INT width, INT height,
		INT bwidth, INT bheight,
		ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory) : 
		_size(width, height), _bounding_box(bwidth, bheight), 
		_img(pRenderTarget, pIWICFactory, filename, _size._x, _size._y), _explode(pAnimation){
	
	}
	void drawImage(FlyingObject& tgt);
	INT getW(), getH(), getBW(), getBH();
	~FlyingObjects() {
	}
};

class Enemy : public FlyingObjects {
protected:
	INT _energy, _score;
public:
	Enemy(PCWSTR filename, Animation* pAnimation,
		INT width, INT height,
		INT bwidth, INT bheight,
		ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, INT energy, INT score) : 
		FlyingObjects(filename, pAnimation, width, height, bwidth, bheight, pRenderTarget, pIWICFactory),
		_energy(energy), _score(score){

	}
	INT getEnergy(), getScore();
};

//由FlyingObject和FlyingObjects组成的玩家飞机类，并且加入能量，分数和三个技能
class Player : public FlyingObject, public FlyingObjects {
private:
	INT _energy = 0, _score = 0;
public:
	Player(
		PCWSTR filename, Animation* pAnimation,
		INT width, INT height,
		INT bwidth, INT bheight,
		ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory,
		INT x, INT y
	) : FlyingObject(x, y, width, height, bwidth, bheight),
		FlyingObjects(filename, pAnimation, width, height, bwidth, bheight, pRenderTarget, pIWICFactory) {
		
	}
	void draw(), blink(), resetScore(), resetEnergy(), addScore(INT x), addEnergy(INT x), slowMove();
	INT getScore(), getEnergy();
};

//各种函数
Vec2 randPoint(INT Width, INT Height, INT windowWidth, INT windowHeight);
void CrushJudge(FlyingObject* lhs, FlyingObject* rhs);
bool outOfScreen(FlyingObjects* pobj, FlyingObject* tgt);

//由飞行物体信息类派生出的自动飞行类（纯抽象类，可以在list中通过抽象基指针调用不同形式的setVelocity）
class AutoFlight : public FlyingObject {
public:
	AutoFlight(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = 1)
		: FlyingObject(x, y, W, H, BW, BH, life) {}
	virtual void setVelocity(FlyingObject* goal = NULL) = 0;
};

class Tracer : public AutoFlight {
public:
	Tracer(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = 1)
		: AutoFlight(x, y, W, H, BW, BH, life) {}
	virtual void setVelocity(FlyingObject* goal = NULL);
};

class Attacker : public AutoFlight {
public:
	Attacker(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = 1)
		: AutoFlight(x, y, W, H, BW, BH, life){}
	virtual void setVelocity(FlyingObject* goal = NULL);
};

class Elite : public AutoFlight {
public:
	Elite(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = 5)
		: AutoFlight(x, y, W, H, BW, BH, life) {}
	virtual void setVelocity(FlyingObject* goal = NULL);
};

class EnemyBullet : public AutoFlight {
public:
	EnemyBullet(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = 1)
		: AutoFlight(x, y, W, H, BW, BH, life) {}
	virtual void setVelocity(FlyingObject* goal = NULL);
};

class PlayerBullet : public AutoFlight {
public:
	PlayerBullet(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = 1)
		: AutoFlight(x, y, W, H, BW, BH, life) {}
	virtual void setVelocity(FlyingObject* goal = NULL);
};

class EliteBullet : public AutoFlight {
public:
	EliteBullet(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = 1)
		: AutoFlight(x, y, W, H, BW, BH, life) {}
	virtual void setVelocity(FlyingObject* goal = NULL);
};

class PlayerSuperBullet : public AutoFlight {
public:
	PlayerSuperBullet(INT x, INT y, INT W, INT H, INT BW, INT BH, INT life = -1)
		: AutoFlight(x, y, W, H, BW, BH, life) {}
	virtual void setVelocity(FlyingObject* goal = NULL);
};
class Manage {
public:
	FlyingObjects* _pObjects;
	std::vector<AutoFlight*> _list;
	void Draw(), setVelocity(FlyingObject* tgt = NULL), resetToEmpty(), allSetExplode(), Crush(FlyingObject* tgt), Crush(std::vector<AutoFlight*>& list);
	~Manage() {
		delete _pObjects;
		for (auto p : _list) {
			delete p;
		}
	}
};
template<typename EnemyType>
class EnemyManage : public Manage {
private:
	int _cnt, _max;
public:
	EnemyManage(int x) : _max(x), _cnt(0) {}
	void clearTrash(Player* pPlayer) {
		for (auto it = _list.begin(); it != _list.end();) {
			if (!(*it)->isAlive()) {
				pPlayer->addEnergy(((Enemy*)_pObjects)->getEnergy());
				pPlayer->addScore(((Enemy*)_pObjects)->getScore());
				it = _list.erase(it);
			}
			else if (outOfScreen(_pObjects, *it)) {
				it = _list.erase(it);
			}
			else {
				it++;
			}
		}
		_cnt = _list.size();
	}
	void generate() {
		if (_cnt < _max) {
			int cnt = rand() % (_max - _cnt) + 1;
			for (int i = 0; i < cnt; i++) {
				Vec2 pnt = randPoint(_pObjects->getW(), _pObjects->getH(), WINDOWWIDTH, WINDOWHEIGHT / 4);
				AutoFlight* newP = new EnemyType(
					pnt._x, pnt._y, 
					_pObjects->getW(), _pObjects->getH(),
					_pObjects->getBW(), _pObjects->getBH()
				);
				_list.push_back(newP);
			}
			_cnt = _list.size();
		}
	}
};

template<typename BulletType>
class BulletManage : public Manage {
public:
	void shoot(FlyingObject* tgt) {
		INT x = (tgt ->getX()) + (_pObjects->getW() / 2);
		AutoFlight* newB = new BulletType(x, tgt->getY(), _pObjects->getW(), _pObjects->getH(), _pObjects->getBW(), _pObjects->getBH());
		_list.push_back(newB);
	}
	void listShoot(std::vector<AutoFlight*>& list) {
		if (!list.empty()) {
			for (auto it : list) {
				shoot(it);
			}
		}
	}
	void clearTrash() {
		for (auto it = _list.begin(); it != _list.end();) {
			if (!(*it) ->isAlive() || outOfScreen(_pObjects, *it)) {
				delete *it;
				it = _list.erase(it);
			}
			else {
				it++;
			}
		}
	}
};
//自动飞行物的列表
template<typename T, typename bullet = AutoFlight>
struct FlyingObjectsList {
	//此飞行物的存在上限，为零时表示没有上限
	int _max = 0, _cnt = 0;
	std::vector<AutoFlight*> list;
	FlyingObjects* pobj = NULL;
	FlyingObjectsList(int x) : _max(x){}

	//按指定的坐标向指定的子弹列表添加对象
	void append(FlyingObjectsList<bullet>& tgt, int x, int y) {
		AutoFlight* p = new bullet(
			x, y,
			tgt.pobj->getW(), tgt.pobj->getH(),
			tgt.pobj->getBW(), tgt.pobj->getBH()
		);
		tgt.list.push_back(p);
	}
	//列表中所有飞行物射击(bullet指定子弹类型)
	void Shoot(FlyingObjectsList<bullet>& tgt) {
		for (auto p : list) {
			append(tgt, p->getX() + (pobj ->getW() / 2), p->getY());
		}
	}
	//重绘列表中所有对象
	void Draw() {
		if (!list.empty()) {
			for (auto p : list) {
				if (p->isAlive()) {
					pobj->drawImage(*p);
				}
			}
		}
	}
	//设置列表中所有对象的速度
	void setVelocity(FlyingObject* goal = NULL) {
		if (!list.empty()) {
			for (auto p : list) {
				p->setVelocity(goal);
			}
		}
	}
	//只将list中的所有对象删除
	void resetToEmpty() {
		for (auto pointer : list) {
			delete pointer;
		}
		list.clear();
	}
	//整个对象全部重置
	void resetToNULL() {
		for (auto pointer : list) {
			delete pointer;
		}
		list.clear();
		delete pobj;
	}

	//将已爆炸和飞出屏幕的飞行物删除
	void clearTrash() {
		for (auto it = list.begin(); it != list.end();) {
			if (!(*it)->isAlive() || outOfScreen(pobj, *it)) {
				it = list.erase(it);
			}
			else {
				it++;
			}
		}
		_cnt = list.size();
	}

	//对一个列表里的所有对象做碰撞检测
	void Crush(std::vector<AutoFlight*>& t) {
		for (auto p1 : t) {
			for (auto p2 : list) {
				CrushJudge(p1, p2);
			}
		}
	}

	//对一个对象做碰撞检测
	void Crush(FlyingObject* obj) {
		if (obj) {
			for (auto pointer : list) {
				CrushJudge(pointer, obj);
			}
		}
	}

	//随机生成（屏幕上方1/4处)
	void generate() {
		if (_max == 0 || _cnt < _max) {
			int cnt = rand() % (_max - _cnt) + 1;
			for (int i = 0; i < cnt; i++) {
				Vec2 pnt = randPoint(pobj->getW(), pobj->getH(), WINDOWWIDTH, WINDOWHEIGHT / 4);
				AutoFlight* p = new T(
					pnt._x, pnt._y,
					pobj->getW(), pobj->getH(),
					pobj->getBW(), pobj->getBH()
				);
				list.push_back(p);
				_cnt += cnt;
			}
		}
	}
};