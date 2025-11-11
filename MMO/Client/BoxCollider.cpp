#include "pch.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Mesh.h"

BoxCollider::BoxCollider(Vector3 minBox, Vector3 maxBox)
    : _minBox(minBox)
    , _maxBox(maxBox)
{
    _type = Box;
    CreateMesh();
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
    if (ray.direction.Length() == 0.0f)
        return false;

    Contact temp;
    temp.distance = FLT_MAX;

    UINT face[] = 
    {
        0, 1, 2, 3,
        4, 5, 6, 7,
        0, 1, 5, 4,
        1, 5, 6, 2,
        2, 3, 7, 6,
        0, 3, 7, 4,
    };

    for (UINT i = 0; i < 6; i++)
    {
        Vector3 p[4];

        p[0] = _vertices[face[i * 4 + 0]].position;
        p[1] = _vertices[face[i * 4 + 1]].position;
        p[2] = _vertices[face[i * 4 + 2]].position;
        p[3] = _vertices[face[i * 4 + 3]].position;

        p[0] = Vector3::Transform(p[0], _world);
        p[1] = Vector3::Transform(p[1], _world);
        p[2] = Vector3::Transform(p[2], _world);
        p[3] = Vector3::Transform(p[3], _world);

        float dist{};
        if (DirectX::TriangleTests::Intersects(ray.position, ray.direction, p[0], p[1], p[2], dist))
        {
            if (dist < temp.distance)
            {
                temp.distance = dist;
                temp.hitPoint = ray.position + ray.direction * dist;
            }
        }

        if (DirectX::TriangleTests::Intersects(ray.position, ray.direction, p[0], p[3], p[2], dist))
        {
            if (dist < temp.distance)
            {
                temp.distance = dist;
                temp.hitPoint = ray.position + ray.direction * dist;
            }
        }
    }

    if (temp.distance == FLT_MAX)
        return false;

    if (contact != nullptr)
    {
        contact->distance = temp.distance;
        contact->hitPoint = temp.hitPoint;
    }

    return true;
}

bool BoxCollider::BoxCollision(BoxCollider* collider)
{
    Obb box1 = GetObb();
    Obb box2 = collider->GetObb();

    Vector3 D = box2.position - box1.position;

    for(UINT i = 0; i < 3; i++)
    {
        if (SeperateAxis(D, box1.axis[i], box1, box2)) return false;
        if (SeperateAxis(D, box2.axis[i], box1, box2)) return false;
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if (box1.axis[i] == box2.axis[j]) return true;
        }
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            Vector3 cross = box1.axis[i].Cross(box2.axis[j]);
            if (SeperateAxis(D, cross, box1, box2)) return false;
        }
    }

    return true;
}

bool BoxCollider::SphereCollision(SphereCollider* collider)
{
    Matrix T = Matrix::CreateTranslation(GlobalPos());
    Matrix R = Matrix::CreateRotationX(GlobalRot().x) * Matrix::CreateRotationY(GlobalRot().y) * Matrix::CreateRotationZ(GlobalRot().z);

    Matrix invWorld = XMMatrixInverse(nullptr, R * T);

    Vector3 spherePos = XMVector3TransformCoord(collider->GlobalPos(), invWorld);

    Vector3 tempMin = _minBox * GlobalScale();
    Vector3 tempMax = _maxBox * GlobalScale();

    Vector3 temp;
    temp.x = max(tempMin.x, min(spherePos.x, tempMax.x));
    temp.y = max(tempMin.y, min(spherePos.y, tempMax.y));
    temp.z = max(tempMin.z, min(spherePos.z, tempMax.z));

    temp -= spherePos;

    return temp.Length() <= collider->Radius();
}

bool BoxCollider::CapsuleCollision(CapsuleCollider* collider)
{
    return collider->BoxCollision(this);
}

bool BoxCollider::SphereCollision(Vector3 center, float radius)
{
    Matrix T = Matrix::CreateTranslation(GlobalPos());
    Matrix R = Matrix::CreateRotationX(GlobalRot().x) * Matrix::CreateRotationY(GlobalRot().y) * Matrix::CreateRotationZ(GlobalRot().z);

    Matrix invWorld = XMMatrixInverse(nullptr, R * T);

    Vector3 spherePos = XMVector3TransformCoord(center, invWorld);

    Vector3 tempMin = _minBox * GlobalScale();
    Vector3 tempMax = _maxBox * GlobalScale();

    Vector3 temp;
    temp.x = max(tempMin.x, min(spherePos.x, tempMax.x));
    temp.y = max(tempMin.y, min(spherePos.y, tempMax.y));
    temp.z = max(tempMin.z, min(spherePos.z, tempMax.z));

    temp -= spherePos;

    return temp.Length() <= radius;
}

Vector3 BoxCollider::MinBox()
{
    return XMVector3TransformCoord(_minBox, _world);
}

Vector3 BoxCollider::MaxBox()
{
    return XMVector3TransformCoord(_maxBox, _world);
}

Obb BoxCollider::GetObb()
{
    _obb.position = GlobalPos();

    _obb.axis[0] = Right();
    _obb.axis[1] = Up();
    _obb.axis[2] = Forward();

    _obb.halfSize = (_minBox - _maxBox) * 0.5f * GlobalScale();

    return _obb;
}

void BoxCollider::CreateMesh()
{
    _vertices.emplace_back(_minBox.x, _minBox.y, _minBox.z);
    _vertices.emplace_back(_minBox.x, _maxBox.y, _minBox.z);
    _vertices.emplace_back(_maxBox.x, _maxBox.y, _minBox.z);
    _vertices.emplace_back(_maxBox.x, _minBox.y, _minBox.z);

    _vertices.emplace_back(_minBox.x, _minBox.y, _maxBox.z);
    _vertices.emplace_back(_minBox.x, _maxBox.y, _maxBox.z);
    _vertices.emplace_back(_maxBox.x, _maxBox.y, _maxBox.z);
    _vertices.emplace_back(_maxBox.x, _minBox.y, _maxBox.z);

    _indices = 
    {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    _mesh = new Mesh(_vertices.data(), sizeof(Vertex), (UINT)_vertices.size(), _indices.data(), (UINT)_indices.size());
}

bool BoxCollider::SeperateAxis(Vector3 D, Vector3 axis, Obb box1, Obb box2)
{
    float distance = abs(D.Dot(axis));

    float a = 0.0f;
    float b = 0.0f;

    float box1HalfSize[3];
    float box2HalfSize[3];
    memcpy(box1HalfSize, &box1.halfSize, sizeof(Vector3));
    memcpy(box2HalfSize, &box2.halfSize, sizeof(Vector3));

    for (UINT i = 0; i < 3; i++)
    {
        a += abs((box1.axis[i] * box1HalfSize[i]).Dot(axis));
        b += abs((box2.axis[i] * box2HalfSize[i]).Dot(axis));
    }

    return distance > a + b;
}
