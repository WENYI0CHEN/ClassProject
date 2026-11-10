#include "Framework.h"
#include "Weapon_Gun.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"

const int Weapon_Gun::c_numBullets = 5;
const int Weapon_Gun::c_bulletDamage = 1000;
const float Weapon_Gun::c_initialBulletSpawnTime = 0.8f;
const float Weapon_Gun::c_bulletSpawnTimeDecreasePerLevel = 0.1f;

Weapon_Gun::Weapon_Gun(Game* pGame)
    : Weapon( pGame )
{
    // Fill a vector with bullets.
    for( int i=0; i<c_numBullets; i++ )
    {
        Bullet* pBullet = new Bullet( pGame, pGame->getMesh("circle"), pGame->getShader() );
        m_bullets.push_back( pBullet );
    }
}

Weapon_Gun::~Weapon_Gun()
{
    for( Bullet* pBullet : m_bullets )
    {
        delete pBullet;
    }
}

void Weapon_Gun::reset()
{
    for( Bullet* pBullet : m_bullets )
    {
        pBullet->reset();
    }

    m_currentBulletSpawnTime = c_initialBulletSpawnTime;
    m_bulletSpawnTimer = 0;
}

void Weapon_Gun::update(float deltaTime)
{
    for( Bullet* pBullet : m_bullets )
    {
        if( pBullet->isActive() )
        {
            pBullet->update( deltaTime );
        }
    }

    if( m_pGame->isGameOver() == false )
    {
        m_bulletSpawnTimer -= deltaTime;
        if( m_bulletSpawnTimer < 0 )
        {
            m_bulletSpawnTimer = m_currentBulletSpawnTime;
            spawnBullet( m_pGame->getPlayer()->getPosition() );
        }
    }
}

void Weapon_Gun::draw(fw::Camera* pCamera)
{
    for( Bullet* pBullet : m_bullets )
    {
        if( pBullet->isActive() )
        {
            pBullet->draw( pCamera );
        }
    }
}

void Weapon_Gun::handleCollisions(EnemyList& enemyList)
{
    // Bullets hitting enemies.
    for( int b=0; b<c_numBullets; b++ )
    {
        if( m_bullets[b]->isActive() )
        {
            vec2 bulletPos = m_bullets[b]->getPosition();
            float bulletRadius = m_bullets[b]->getRadius();

            // Check if they collide with any enemies.
            for( size_t e=0; e<enemyList.size(); e++ )
            {
                if( enemyList[e]->isActive() && enemyList[e]->getHealth() > 0 )
                {
                    vec2 enemyPos = enemyList[e]->getPosition();
                    float enemyRadius = enemyList[e]->getRadius();

                    if( (enemyPos - bulletPos).length() < enemyRadius + bulletRadius )
                    {
                        enemyList[e]->applyDamage( c_bulletDamage );
                        m_bullets[b]->setActive( false );
                        m_pGame->onEnemyKilled( enemyPos );
                        break;
                    }
                }
            }
        }
    }
}

void Weapon_Gun::spawnBullet(vec2 pos)
{
    Bullet* pBullet = nullptr;

    // Find the first inactive Bullet.
    for( int i=0; i<c_numBullets; i++ )
    {
        if( m_bullets[i]->isActive() == false )
        {
            pBullet = m_bullets[i];
            break;
        }
    }

    // Active the Bullet and send it toward the closest enemy.
    if( pBullet )
    {
        vec2 playerPos = m_pGame->getPlayer()->getPosition();
        Enemy* nearestEnemy = m_pGame->getClosestEnemy( playerPos );
        if( nearestEnemy )
        {
            pBullet->setActive( true );
            pBullet->setPosition( pos );

            vec2 enemyPos = nearestEnemy->getPosition();
            vec2 dir = (enemyPos - playerPos).getNormalized();
            pBullet->setDirection( dir );
        }
    }
}

void Weapon_Gun::levelUp()
{
    m_currentBulletSpawnTime -= c_bulletSpawnTimeDecreasePerLevel;
}
