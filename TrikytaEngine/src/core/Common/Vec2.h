#pragma once
#include <math.h>
#define FLT_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0

/// This function is used to ensure that a floating point number is not a NaN or infinity.
inline bool _IsValid(float x)
{
	return isfinite(x);
}

#define	Sqrt(x)	sqrtf(x)
#define	Atan2(y, x)	atan2f(y, x)

template<typename T>
struct Vec2
{
public:
	T x;
	T y;

	/*
	*Default constructor!
	*/
	Vec2(){};

	/*
	*/
	Vec2(T x, T y): x(static_cast<T>(x)), y(static_cast<T>(y)) {};

	/*
		Copy Constructor!
	*/
	Vec2(const Vec2& v) : x(v.x), y(v.y) {};

	//conversion 
	//operator b2Vec2() const { return b2Vec2(x, y); }

	/// Add a vector to this vector.
	void operator += (const Vec2& v)
	{
		x += v.x; y += v.y;
	}

	/// Subtract a vector from this vector.
	void operator -= (const Vec2& v)
	{
		x -= v.x; y -= v.y;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float a)
	{
		x *= a; y *= a;
	}

	/// Negate this vector.
	Vec2 operator -() const { Vec2 v; v.Set(-x, -y); return v; }

	/// Read from and indexed element.
	T operator () (int i) const
	{
		return (&x)[i];
	}

	/// Write to an indexed element.
	T& operator () (int i)
	{
		return (&x)[i];
	}

	Vec2& operator=(Vec2 other) 
	{ 
		this->x = other.x;
		this->y = other.y;
		return *this; 
	};

	Vec2 operator+(Vec2& v) {
		return Vec2(x + v.x, y + v.y);
	}

	friend Vec2<T> operator+(Vec2<T>& rhs, Vec2<T>& lhs) {
		return Vec2<T>(rhs.x + lhs.x, rhs.y + lhs.y);
	}

	friend Vec2 operator-(Vec2& rhs, Vec2& lhs) {
		return Vec2(rhs.x - rhs.x, rhs - lhs.y);
	}

	friend Vec2 operator/(Vec2& rhs, Vec2& lhs) {
		return Vec2(rhs.x / rhs.x, lhs / lhs.y);
	}

	friend Vec2 operator*(Vec2& rhs, Vec2& lhs) {
		return Vec2(rhs.x * rhs.x, lhs * lhs.y);
	}

	friend bool operator==(Vec2 rhs, Vec2 lhs) {
		return (rhs.x == lhs.x && rhs.y == lhs.y);
	}

	/*template<typename Typ>
	Vec2 operator+(Typ& s) {
		return Vec2(x + s, y + s);
	}

	template<typename Typ>
	Vec2 operator-(Typ& s) {
		return Vec2(x - s, y - s);
	}

	template<typename Typ>
	Vec2 operator*(Typ& s) {
		return Vec2(x * s, y * s);
	}

	template<typename Typ>
	Vec2 operator/(Typ& s) {
		return Vec2(x / s, y / s);
	}*/
	template<typename Typ>
	Vec2& operator/=(T& s) {
		x /= s;
		y /= s;
		return *this;
	}

	inline T getX() const { return this->x; };
	inline T getY() const { return this->y; };

	virtual ~Vec2() {};

private:

};

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
//typedef Vec2<float> b2Vec2;


struct b2Vec2 : Vec2f
{
	/// Default constructor does nothing (for performance).
	b2Vec2() {}

	/// Construct using coordinates.
	b2Vec2(float xIn, float yIn) : Vec2f(xIn, yIn) {}
	b2Vec2(Vec2f v) : Vec2f(v.x, v.y) {}

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; }

	/// Set this vector to some specified coordinates.
	void Set(float x_, float y_) { x = x_; y = y_; }

	/// Negate this vector.
	b2Vec2 operator -() const { b2Vec2 v; v.Set(-x, -y); return v; }

	/// Read from and indexed element.
	float operator () (int i) const
	{
		return (&x)[i];
	}

	/// Write to an indexed element.
	float& operator () (int i)
	{
		return (&x)[i];
	}

	/// Add a vector to this vector.
	void operator += (const b2Vec2& v)
	{
		x += v.x; y += v.y;
	}

	/// Subtract a vector from this vector.
	void operator -= (const b2Vec2& v)
	{
		x -= v.x; y -= v.y;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float a)
	{
		x *= a; y *= a;
	}

	/// Get the length of this vector (the norm).
	float Length() const
	{
		return Sqrt(x * x + y * y);
	}

	/// Get the length squared. For performance, use this instead of
	/// b2Vec2::Length (if possible).
	float LengthSquared() const
	{
		return x * x + y * y;
	}

	/// Convert this vector into a unit vector. Returns the length.
	float Normalize()
	{
		float length = Length();
		if (length < FLT_EPSILON)
		{
			return 0.0f;
		}
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;

		return length;
	}

	/// Does this vector contain finite coordinates?
	bool IsValid() const
	{
		return _IsValid(x) && _IsValid(y);
	}

	/// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
	b2Vec2 Skew() const
	{
		return b2Vec2(-y, x);
	}

};