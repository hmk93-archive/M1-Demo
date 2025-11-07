#include "pch.h"
#include "Factory.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "Utility.h"
using namespace Utility;

Enemy* Factory::CreateEnemy(Enemy::EnemyType type, const vector<Vector3>& positions)
{
    Enemy* enemy = nullptr;
    switch (type)
    {
    case Enemy::EnemyType::Warrok:
        enemy = new Enemy("Warrok");
        for (UINT i = 0; i < positions.size(); i++)
        {
            Transform* transform = enemy->AddTransform();
            transform->position = positions[i];
            transform->rotation.y = Random(0.0f, XM_2PI);
            transform->scale = Vector3(0.1f);
            enemy->UpdateTransforms();
            enemy->AddHPBar(i);
            enemy->SetAnimation(i, Enemy::EnemyAnimState::Idle);

            enemy->SetEndEvents(i, (UINT)Enemy::EnemyAnimState::Punch, bind(&Enemy::SetIdle, enemy, i));
            enemy->SetEndEvents(i, (UINT)Enemy::EnemyAnimState::Hit, bind(&Enemy::SetIdle, enemy, i));
            enemy->SetEndEvents(i, (UINT)Enemy::EnemyAnimState::Dead, bind(&Enemy::DeathEnd, enemy, i));

            enemy->SetParams(i, (UINT)Enemy::EnemyAnimState::Punch, i);
            enemy->SetParams(i, (UINT)Enemy::EnemyAnimState::Hit, i);
            enemy->SetParams(i, (UINT)Enemy::EnemyAnimState::Dead, i);

            enemy->SetFrameEvents(i, (UINT)Enemy::EnemyAnimState::Punch, bind(&Enemy::OnAttack, enemy), {15});

            const float radius = (enemy->worldMinBox - enemy->worldMaxBox).Length() * 0.5f;
            Vector3 offset = Vector3(0.0f, 10.0f, 0.0f);
            enemy->mainCollider[i] = new SphereCollider(radius * 0.5f);
            enemy->mainCollider[i]->SetTarget(transform, offset);
        }
        break;
    }

    return enemy;
}
