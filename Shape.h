#pragma once
class Shape {
public:
	enum class ShapeType
	{
		SHAPE_SPHERE,
	};

	virtual ShapeType GetType() const = 0;
	virtual Mat3 InertiaTensor() const = 0;
	virtual Vec3 GetCenterOfMass() const { return centerOfMass; }
protected:
	Vec3 centerOfMass;
};

class ShapeSphere : public Shape {
public:
	ShapeSphere(float radiusP) : radius(radiusP)
	{
		centerOfMass.Zero();
	}

	ShapeType GetType() const override { return ShapeType::SHAPE_SPHERE; }

	Mat3 InertiaTensor() const override
	{
		Mat3 tensor;
		tensor.Zero();
		tensor.rows[0][0] = 2.0f * radius * radius / 5.0f;
		tensor.rows[1][1] = 2.0f * radius * radius / 5.0f;
		tensor.rows[2][2] = 2.0f * radius * radius / 5.0f;
		return tensor;
	}

	float radius;
};

