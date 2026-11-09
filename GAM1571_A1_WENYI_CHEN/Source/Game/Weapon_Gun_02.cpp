#include "Weapon_Gun_02.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"

const float Weapon_Gun_02::c_weaponFreezeTimer=0.3f;
const int Weapon_Gun_02::c_initialWeaponDamage = 1;
const int Weapon_Gun_02::c_weaponDamageIncreasePerLevel = 2;
const float Weapon_Gun_02::c_weaponRadiusIncreasePerLevel = 20.0f;

const int Weapon_Gun_02::c_initialWeaponRadius=40;
const int Weapon_Gun_02::c_initialWeaponAmount=1;

Weapon_Gun_02::Weapon_Gun_02(Game* pGame)
	: Weapon(pGame)
	/*, m_tweenSize(m_visibleScale)*/
{
	m_pSprite = new Sprite(pGame->getTexture("Weapon_Gun_02"));
	m_weaponDamage = c_initialWeaponDamage;
	m_weaponRadius = c_initialWeaponRadius;
	//m_currentWeaponAmount = c_initialWeaponAmount;
}

Weapon_Gun_02::~Weapon_Gun_02()
{
	delete m_pSprite;
}

void Weapon_Gun_02::reset()
{
	m_weaponDamage = c_initialWeaponDamage;
	m_weaponRadius = c_initialWeaponRadius;
	m_currentWeaponFreezeTimer = 0.0f;
}

void Weapon_Gun_02::update(float deltaTime)
{
	//if (m_fadingOut == true)
	//{
	//	//m_isActive = false;
	//	setActive(deltaTime);
	//	return;
	//}

	if (m_pGame->isGameOver() == false&&m_isActive==true)
	{
		m_angle += 5.0f * deltaTime;
		m_weaponPos = m_pGame->getPlayer()->getPlayerPosition() + vec2(
			cosf(m_angle) * m_weaponRadius, sinf(m_angle) * m_weaponRadius
		);
	}
	
	
}

void Weapon_Gun_02::draw(bool drawDebugData)
{
	m_pSprite->draw(m_weaponPos, 0.0f, vec2(0.2f, 0.2f));
}

void Weapon_Gun_02::handleCollisions(EnemyList& enemyList)
{
	if (m_isActive)
	{
		for (size_t e = 0; e < enemyList.size(); e++)
		{
			if (enemyList[e]->isActive() && enemyList[e]->getHealth() > 0)
			{
				vec2 ePos = enemyList[e]->getPosition();
				if ((m_weaponPos - ePos).length() < m_weaponRadius + enemyList[e]->getRadius())
				{
					enemyList[e]->applyDamage(c_initialWeaponDamage);
					m_isActive = false;
					freezeTimer();
					if (enemyList[e]->getHealth()<= 0)
					{
						m_pGame->onEnemyKilled(m_weaponPos);
					}
					//startDeathAnim();
					//m_isActive = false;
					
					break;
				}
			}
		}
	}

}


void Weapon_Gun_02::levelUp()
{
	m_weaponDamage += c_weaponDamageIncreasePerLevel;
	m_weaponRadius += c_weaponRadiusIncreasePerLevel;
}

//void Weapon_Gun_02::startDeathAnim()
//{
//	m_fadingOut = true;
//	m_tweenSize.start(0.0f, 0.5f, 0.0f, easingLinear);
//}

//void Weapon_Gun_02::setActive(float deltaTime)
//{
//	if (m_fadingOut == true)
//	{
//		m_currentWeaponSpawnTime += deltaTime;
//		if (m_currentWeaponSpawnTime >= c_weaponFreezeTimer)
//		{
//			m_fadingOut = false;
//			m_currentWeaponSpawnTime = 0;
//			m_isActive = true;
//		}
//	}
//}

void Weapon_Gun_02::freezeTimer()
{
	m_currentWeaponFreezeTimer = c_weaponFreezeTimer;
	m_currentWeaponFreezeTimer--;
	if (m_currentWeaponFreezeTimer <= 0)
	{
		m_currentWeaponFreezeTimer = 0;
		m_isActive = true;
	}
}