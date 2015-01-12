#pragma once

template <typename T>
struct Vector2D
{
public:
	Vector2D() {};
	Vector2D(T v) : x(v), y(v) {}
	Vector2D(T x, T y) : x(x), y(y) {}

	Vector2D operator+(Vector2D &vec) const{
		return Vector2D(x + vec.x, y + vec.y);
	}

	Vector2D operator-(Vector2D &vec) const{
		return operator+(vec * -1);
	}

	Vector2D operator*(T n) const{
		return Vector2D(x * n, y * n);
	}

	Vector2D operator/(T n) const{
		return operator*(1.0 / n);
	}

	template<typename U>
	Vector2D<U> convert(U (*func)(T)){
		return Vector2D<U>(func(x), func(y));
	}

	T x, y;
};

template <typename T>
inline Vector2D<T> operator*(T n, Vector2D<T> &vec){
	return vec*n;
}