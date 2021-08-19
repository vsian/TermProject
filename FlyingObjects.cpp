#include "FlyingObjects.h"
void FlyingObjects::drawImage(FlyingObject& tgt) {
	if (tgt.explode()) {
		tgt.setLive(_explode->show(tgt.getX(), tgt.getY(), tgt._explodeFrames)); //如果爆炸，绘制动画图片，尝试设置存活状态
		return;
	}
	tgt.setX(tgt.getX() + tgt.getVX()); //根据速度和坐标更新位置信息
	tgt.setY(tgt.getY() + tgt.getVY());
	_img.Draw(tgt.getX(), tgt.getY()); //画出图像
}
INT FlyingObjects::getW() {
	return _size._x;
}
INT FlyingObjects::getH() {
	return _size._y;
}
INT FlyingObjects::getBW() {
	return _bounding_box._x;
}
INT FlyingObjects::getBH() {
	return _bounding_box._y;
}

INT FlyingObject::getX()
{
	return _pos._x;
}

INT FlyingObject::getY()
{
	return _pos._y;
}

INT FlyingObject::getVX()
{
	return _velocity._x;
}

INT FlyingObject::getVY()
{
	return _velocity._y;
}

INT FlyingObject::getLeftupX()
{
	return _bounding_box_leftup._x;
}

INT FlyingObject::getLeftupY()
{
	return _bounding_box_leftup._y;
}

INT FlyingObject::getRightbottomX()
{
	return _bounding_box_rightbottom._x;
}

INT FlyingObject::getRightbottomY()
{
	return _bounding_box_rightbottom._y;
}

void FlyingObject::setVX(INT x)
{
	_velocity._x = x;
}

void FlyingObject::setVY(INT y)
{
	_velocity._y = y;
}

void FlyingObject::setX(INT x)
{
	_bounding_box_leftup._x += x - getX();
	_bounding_box_rightbottom._x += x - getX();
	_pos._x = x;
}

void FlyingObject::setY(INT y)
{
	_bounding_box_leftup._y += y - getY();
	_bounding_box_rightbottom._y += y - getY();
	_pos._y = y;
}

bool FlyingObject::isAlive()
{
	return _live;
}

bool FlyingObject::explode()
{
	return _explode;
}

void FlyingObject::setLive(bool f)
{
	_live = f;
}

void FlyingObject::setExplode(bool f)
{
	if (f) {
		if (--_life)
			return;
		_explode = true;
	}
	else {
		_explode = false;
	}
}

void FlyingObject::setLife(int x)
{
	_life = x;
}

void CrushJudge(FlyingObject* lhs, FlyingObject* rhs)
{
	if (!(lhs->explode() || rhs->explode())) {		//若其中一个已经爆炸，则不算碰撞，跳过即可
		INT minX = max(lhs->getLeftupX(), rhs->getLeftupX());
		INT minY = max(lhs->getLeftupY(), rhs->getLeftupY());
		INT maxX = min(lhs->getRightbottomX(), rhs->getRightbottomX());
		INT maxY = min(lhs->getRightbottomY(), rhs->getRightbottomY());		//尝试构建相交矩形
		if (!(minX > maxX || minY > maxY)) {		//相交矩形存在
			lhs->setExplode(true);		//尝试将双方设置为爆炸状态
			rhs->setExplode(true);
		}
	}
}

//判断是否在屏幕之外
bool outOfScreen(FlyingObjects* pobj, FlyingObject* tgt)
{
	if (tgt) {
		if (tgt->getX() < -(pobj ->getW()) || tgt ->getX() > WINDOWWIDTH
			|| tgt ->getY() < -(pobj ->getH()) || tgt ->getY() > WINDOWHEIGHT)
			return true;
	}
	return false;
}

//在屏幕前半段外面生成一个随机点
Vec2 randPoint(INT Width, INT Height, INT windowWidth, INT windowHeight)
{
	INT flag = rand() % 3;
	if (flag == 0) {
		INT pos = rand() % (windowWidth - Width);
		return Vec2(pos, -Height);
	}
	else {
		INT pos = rand() % (windowHeight - Height);
		if (flag == 1) {
			return Vec2(-Width, pos);
		}
		else {
			return Vec2(windowWidth, pos);
		}
	}
}

void Tracer::setVelocity(FlyingObject* goal)
{
	if (goal) {
		INT Sx = goal->getX() - getX(), Sy = goal->getY();
		setVX((INT)((double)(15 * Sx) / sqrt((double)Sx * (double)Sx + (double)Sy * (double)Sy)));
		if (getY() < goal->getY()) {
			setVY(5 * Sy / (INT)sqrt((double)Sx * (double)Sx + (double)Sy * (double)Sy));
		}
	}
}

void Attacker::setVelocity(FlyingObject* goal)
{
	int f = rand() % 3;
	if (getX() < 0) {
		setVX(f + 3);
	}
	else if (getX() > WINDOWWIDTH) {
		setVX(-f - 3);
	}
	setVY(f + 3);
}

void EnemyBullet::setVelocity(FlyingObject* goal)
{
	setVY(BULLETSPEED);
}

void PlayerBullet::setVelocity(FlyingObject* goal)
{
	setVY(-BULLETSPEED);
}

void Elite::setVelocity(FlyingObject* goal)
{
	if (getX() < 0) {
		setVX(1);
	}
	else if (getX() > WINDOWWIDTH) {
		setVX(-1);
	}
	setVY(1);
}

void Player::blink()
{
	if (_energy >= 5) {
		_energy -= 5;
		if (getVX() > 0) {
			setX(getX() + 150);
		}
		else if (getVX() < 0) {
			setX(getX() - 150);
		}
		if (getVY() > 0) {
			setY(getY() + 150);
		}
		else if (getVY() < 0) {
			setY(getY() - 150);
		}
	}
}

void Player::draw()
{
	drawImage(*this);
}

void Player::resetScore()
{
	_score = 0;
}

void Player::addScore(INT x)
{
	_score += x;
}

void Player::addEnergy(INT x)
{
	if (_energy <= 30 || x < 0) {
		_energy += x;
	}
}

void Player::slowMove()
{
	if (getVX() > 0) {
		setVX(2);
	}
	else if (getVX() < 0) {
		setVX(-2);
	}
	if (getVY() > 0) {
		setVY(2);
	}
	else if (getVY() < 0) {
		setVY(-2);
	}
}

void Player::resetEnergy()
{
	_energy = 0;
}

INT Player::getScore()
{
	return _score;
}

INT Player::getEnergy()
{
	return _energy;
}

void Manage::Draw()
{
	if (!_list.empty()) {
		for (auto p : _list) {
			_pObjects->drawImage(*p);
		}
	}
}

void Manage::setVelocity(FlyingObject* tgt)
{
	if (!_list.empty()) {
		for (auto p : _list) {
			p->setVelocity(tgt);
		}
	}
}

void Manage::resetToEmpty()
{
	for (auto pointer : _list) {
		delete pointer;
	}
	_list.clear();
}

void Manage::Crush(FlyingObject* tgt)
{
	if (tgt && !_list.empty()) {
		for (auto pointer : _list) {
			CrushJudge(pointer, tgt);
		}
	}
}

void Manage::Crush(std::vector<AutoFlight*>& list)
{
	if (!_list.empty() && !list.empty()) {
		for (auto p1 : list) {
			for (auto p2 : _list) {
				CrushJudge(p1, p2);
			}
		}
	}
}

void Manage::allSetExplode()
{
	for (auto it : _list) {
		it->setExplode(true);
	}
}

INT Enemy::getEnergy()
{
	return _energy;
}

INT Enemy::getScore()
{
	return _score;
}

void EliteBullet::setVelocity(FlyingObject* goal)
{
	setVY(getVY() + 1);
}

void PlayerSuperBullet::setVelocity(FlyingObject* goal)
{
	setVY(-8);
}
