#pragma once

class Light
{
public:
	enum Type
	{
		Directional,
		Point,
		Spot
	};

	Light()
	{
	}

	Vector4 color = Vector4(1.0f);
	Vector3 direction = Vector3(0.0f, -1.0f, 1.0f);
	Type type = Directional;
	Vector3 position = Vector3(0.0f);
	int active = 1;
};

