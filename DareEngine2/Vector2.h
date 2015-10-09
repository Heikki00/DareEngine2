#include <cmath>
#include "DebugUtility.h"

#define F (double)
#define PI 3.14159265359

template<typename T>
class Vector2{

public:
	Vector2(T x, T y){
		this->x = x;
		this->y = y;
	}

	inline float lenght(){
		return F sqrt(F x * F x + F y * F y);
	}

	inline float dot(Vector2 r){
		return F x * F r.getX() + F y * F r.getY();
	}

	inline Vector2 normalize(){
		x /= lenght();
		y /= lenght();

		return *this;
	}

	inline Vector2 rotate(double angle){
		double rad = angle * PI / 180.0;
		double cos = cos(rad);
		double sin = sin(rad);

		return Vector2((x * cos - y * sin), (x * sin + y * cos));
	}




#pragma region operators
	// +
	Vector2 operator +(T r){
		return Vector2(x + r, y + r);
	}

	Vector2<T> operator +(Vector2 r){
		return Vector2(x + r.getX(), y + r.getY());
	}

	// -
	Vector2 operator -(T r){
		return Vector2(x - r, y - r);
	}

	Vector2<T> operator -(Vector2 r){
		return Vector2(x - r.getX(), y - r.getY());
	}

	// *
	Vector2 operator *(T r){
		return Vector2(x * r, y * r);
	}

	Vector2<T> operator *(Vector2 r){
		return Vector2(x * r.getX(), y * r.getY());
	}

	// /
	Vector2 operator /(T r){
		if (r != 0)
			return Vector2(x / r, y / r);
			
		DebugUtility::gl_log_err("ERROR: Tried to divide Vector2 by scalar 0\n");
		return Vector2(x, y);
	}

	Vector2<T> operator /(Vector2 r){
		if (r.getX() != 0 && r.getY() != 0)
			return Vector2(x / r.getX(), y / r.getY());

		DebugUtility::gl_log_err("ERROR: Tried to divide Vector2 with another Vector2 witch had x or y = 0\n");
		return Vector2(x, y);

	}

#pragma endregion

	//Setters and getters

	inline T getX(){ return x; }
	inline T getY(){ return y; }
	inline void setX(T x){ this->x = x; }
	inline void setY(T y){ this->y = y; }

private:

	T x;
	T y;
};





















