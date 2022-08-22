#pragma once
#include <concepts>
#include <array>
#include "Utils.h"

template <typename T> requires (std::same_as<T, float> || std::same_as<T, double>)
class Vector3
{
public:
	union { T x, r; };
	union { T y, g; };
	union { T z, b; };

	Vector3() = default;

	Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z)
	{
	}

	Vector3(const Vector3& arg) : x(arg.x), y(arg.y), z(arg.z)
	{
	}

	Vector3& operator=(const Vector3& arg)
	{
		x = arg.x; 
		y = arg.y;
		z = arg.z;
		return *this;
	}

	operator Vector3<float>() 
	{
		return vector3<float>(float(x), float(y), float(z));
	}

	operator Vector3<double>()
	{
		return vector3<double>(double(x), double(y), double(z));
	}

	Vector3& operator/=(const float& a)
	{
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}

	Vector3& operator*=(const float& a)
	{
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}

	Vector3 operator*(const float& a) const
	{
		return Vector3(x * a, y * a, z * a);
	}

	Vector3 operator/(const float& a) const
	{
		return Vector3(x / a, y / a, z / a);
	}

	Vector3& operator+=(const Vector3& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	Vector3& operator-=(const Vector3& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	Vector3 operator+(const Vector3& a) const
	{
		return Vector3(x + a.x, y + a.y, z + a.z);
	}

	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 operator-(const Vector3& a) const
	{
		return Vector3(x - a.x, y - a.y, z - a.z);
	}

	static float Dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	float GetSquaredLength() const
	{
		return x * x + y * y + z * z;
	}

	float GetLength() const
	{
		return std::sqrt(GetSquaredLength());
	}

	Vector3 GetNormalized() const
	{
		return Vector3(x / GetLength(), y / GetLength(), z / GetLength());
	}

	static Vector3 Random()
	{
		return Vector3(random_float(), random_float(), random_float());
	}

	static Vector3 Random(float min, float max)
	{
		return Vector3(random_float(min, max), random_float(min, max), random_float(min, max));
	}

	static Vector3 RandomInUnitSphere()
	{
		while (true)
		{
			auto p = Vector3::Random(-1.0f, 1.0f);
			if (p.GetSquaredLength() >= 1)
			{
				continue;
			}
			return p;
		}
	}

	static Vector3 RandomUnitVector()
	{
		auto a = random_float(0, 2 * pi);
		auto z = random_float(-1, 1);
		auto r = sqrt(1 - z * z);

		return Vector3{ r * std::cos(a), r * std::sin(a), z };
	}

	static Vector3 RandomInHemiSphere(const Vector3& normal)
	{
		const Vector3 in_unit_Sphere = RandomInUnitSphere();

		if (Dot(in_unit_Sphere, normal) > 0.0)
		{
			return in_unit_Sphere;
		}
		else
		{
			return -in_unit_Sphere;
		}
	}

	static Vector3 RandomInUnitDisk()
	{
		while (true)
		{
			auto p = Vector3{ random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), 0 };
			if (p.GetSquaredLength() >= 1)
			{
				continue;
			}
			return p;
		}
	}

	static Vector3 Reflect(const Vector3& v, const Vector3& n)
	{
		return v - 2 * Dot(v, n) * n;
	}

	static Vector3 Refract(const Vector3& uv, const Vector3& n, float etai_over_etat)
	{
		float cos_theta = Dot(-uv, n);
		Vector3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
		Vector3 r_out_perp = -sqrt(1.0 - r_out_parallel.GetSquaredLength()) * n;

		return r_out_parallel + r_out_perp;
	}

	static void NormalizeAndOutput(T r, T g, T b, std::ostream& out, int samples_per_pixel)
	{
		const auto scale = 1.0 / samples_per_pixel;
		const auto R = sqrt(scale * r);
		const auto G = sqrt(scale * g);
		const auto B = sqrt(scale * b);

		out << static_cast<int>(256 * std::clamp(R, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * std::clamp(G, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * std::clamp(B, 0.0, 0.999)) << '\n';
	}

	void OutputValues(std::ostream& out, int samples_per_pixel) const
	{
		const auto scale = 1.0 / samples_per_pixel;
		const auto R = sqrt(scale * r);
		const auto G = sqrt(scale * g);
		const auto B = sqrt(scale * b);

		out << static_cast<int>(256 * std::clamp(R, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * std::clamp(G, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * std::clamp(B, 0.0, 0.999)) << '\n';
	}
};

template <typename T>
Vector3<T> operator*(const float& f, const Vector3<T>& v)
{
	return Vector3(f * v.x, f * v.y, f * v.z);
}

template <typename T>
Vector3<T> operator*(const Vector3<T>& f, const Vector3<T>& v)
{
	return Vector3(f.x * v.x, f.y * v.y, f.z * v.z);
}

#define Point3 Vector3
#define ColorRGB Vector3

#ifdef FLOAT_VECTOR	
	#define Vector3 Vector3<float>
#elif defined DOUBLE_VECTOR
	#define Vector3 Vector3<double>
#endif
