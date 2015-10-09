#include <cmath>
#include "DebugUtility.h"

#define F (double)
#define PI 3.14159265359


template < typename T >
class Vector3{
public:

	Vector3(T x, T y, T z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline float lenght(){
		return F sqrt(F x * F x + F y * F y + F z * F z);
	}

	inline float dot(Vector3 r){
		return F x * F r.getX() + F y * F r.getY() + F z * F r.getZ();
	}

	inline Vector3 normalize(){
		x /= lenght();
		y /= lenght();
		z /= lenght();

		return *this;
	}

	inline Vector3 cross(Vector3 r){
		double x_ = y * r.getZ() - z * r.getY();
		double y_ = z * r.getX() - x * r.getZ();
		double z_ = x * r.getY - y * r.getX();

		return Vector3(x_, y_, x_)
	}

	



#pragma region operators
	// +
	Vector3 operator +(T r){
		return Vector3(x + r, y + r, z + r);
	}

	Vector3 operator +(Vector3 r){
		return Vector3(x + r.getX(), y + r.getY(), z + r.getZ());
	}

	// -
	Vector3 operator -(T r){
		return Vector3(x - r, y - r, z -r);
	}

	Vector3 operator -(Vector3 r){
		return Vector3(x - r.getX(), y - r.getY(), z - r.getZ);
	}

	// *
	Vector3 operator *(T r){
		return Vector3(x * r, y * r, z * r);
	}

	Vector3 operator *(Vector3 r){
		return Vector3(x * r.getX(), y * r.getY(), z * r.getZ());
	}

	// /
	Vector3 operator /(T r){
		if (r != 0)
			return Vector3(x / r, y / r, z / r);

		DebugUtility::gl_log_err("ERROR: Tried to divide Vector4 by scalar 0\n");
		return Vector2(x, y);
	}

	Vector3 operator /(Vector3 r){
		if (r.getX() != 0 && r.getY() != 0 && r.getZ() !=0)
			return Vector3(x / r.getX(), y / r.getY(), z / r.getZ());

		DebugUtility::gl_log_err("ERROR: Tried to divide Vector3 with another Vector3 witch had x or y or z = 0\n");
		return Vector3(x, y, z);

	}

#pragma endregion



#pragma region getSet
	inline T getX(){ return x; }
	inline T getY(){ return y; }
	inline T getZ(){ return z; }
	inline void setX(T x){ this->x = x; }
	inline void setY(T y){ this->y = y; }
	inline void setZ(T z){ this->z = z; }
#pragma endregion

private:
	T x, y, z;

};

















































