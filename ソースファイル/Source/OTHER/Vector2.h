#pragma once

#include	"iextreme.h"


struct Vector2
{
public:
	float x, y;
	//	コンストラクタ
	Vector2( ) {};
	inline Vector2( float x, float y ) { this->x = x, this->y = y; }
	inline Vector2( CONST Vector2& v ) { this->x = v.x, this->y = v.y; }
	inline Vector2( Vector3 v ) { x = v.x; y = v.y; }
	//	距離計算
	inline float Length( ) { return sqrtf( x*x + y*y ); }
	inline float LengthSq( ) { return x*x + y*y; }

	//	正規化
	void Normalize( )
	{
		float l = Length( );
		if ( l != .0f ) { x /= l; y /= l; }
	}

	//	オペレーター
	inline Vector2& operator = ( CONST Vector2& v ) { x = v.x; y = v.y; return *this; }
	inline Vector2& operator += ( CONST Vector2& v ) { x += v.x; y += v.y; return *this; }
	inline Vector2& operator -= ( CONST Vector2& v ) { x -= v.x; y -= v.y; return *this; }
	inline Vector2& operator *= ( FLOAT v ) { x *= v; y *= v; return *this; }
	inline Vector2& operator /= ( FLOAT v ) { x /= v; y /= v; return *this; }

	inline Vector2 operator + ( ) const { Vector2 ret( x, y ); return ret; }
	inline Vector2 operator - ( ) const { Vector2 ret( -x, -y ); return ret; }

	inline Vector2 operator + ( CONST Vector2& v ) const { return Vector2( x + v.x, y + v.y ); }
	inline Vector2 operator - ( CONST Vector2& v ) const { return Vector2( x - v.x, y - v.y ); }
	inline Vector2 operator * ( FLOAT v ) const { Vector2 ret( x*v, y*v ); return ret; }
	inline Vector2 operator / ( FLOAT v ) const { Vector2 ret( x / v, y / v ); return ret; }

	BOOL operator == ( CONST Vector2& v ) const { return (x == v.x) && (y == v.y); }
	BOOL operator != ( CONST Vector2& v ) const { return (x != v.x) || (y != v.y); }


	float Dot( const Vector2& v ) { return x*v.x + y*v.y; }
};

