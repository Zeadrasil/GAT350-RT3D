#pragma once
#include <cmath>
#include <utility>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/quaternion.hpp>

namespace nc
{
	constexpr float Pi = 3.14159265359f;
	constexpr float TwoPi = 6.28318530718f;
	constexpr float HalfPi = 1.57079632679f;

	constexpr float RadiansToDegrees(float radians) { return radians * (180.0f / Pi); }
	constexpr float DegreesToRadians(float degrees) { return degrees * (Pi / 180.0f); }
	// convert euler angles (degrees) to a quaternion
	//glm::vec3 QuaternionToEuler(const glm::quat& q);

	// convert quaternion to euler angles (degrees)
	//glm::quat EulerToQuaternion(const glm::vec3& euler);

	constexpr int Wrap(int value, int max)
	{
		return (value % max) + ((value < 0) ? max : 0);
	}

	inline float Wrap(float value, float max)
	{
		return std::fmod(value, max) + ((value < 0) ? max : 0);
	}

	template <typename T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template <typename T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template <typename T>
	constexpr T Clamp(T value, T min, T max)
	{
		if (min > max) std::swap(min, max);

		return (value < min) ? min : (value > max) ? max : value;
	}

	template<typename T> 
	constexpr T Lerp(const T& a, const T& b, float t) // t = 0 <-> 1
	{
		return (a * (1.0f - t)) + (b * t);
	}

    inline glm::vec3 QuaternionToEuler(const glm::quat& q)
    {
        glm::vec3 euler;

        // if the input quaternion is normalized, this is exactly one. Otherwise, this acts as a correction factor for the quaternion's not-normalizedness
        float unit = (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);

        // this will have a magnitude of 0.5 or greater if and only if this is a singularity case
        float test = q.x * q.w - q.y * q.z;

        if (test > 0.4995f * unit) // singularity at north pole
        {
            euler.x = Pi / 2;
            euler.y = 2.0f * std::atan2(q.y, q.x);
            euler.z = 0;
        }
        else if (test < -0.4995f * unit) // singularity at south pole
        {
            euler.x = -Pi / 2;
            euler.y = -2.0f * std::atan2(q.y, q.x);
            euler.z = 0;
        }
        else // no singularity - this is the majority of cases
        {
            euler.x = std::asin(2.0f * (q.w * q.x - q.y * q.z));
            euler.y = std::atan2(2.0f * q.w * q.y + 2.0f * q.z * q.x, 1 - 2.0f * (q.x * q.x + q.y * q.y));
            euler.z = std::atan2(2.0f * q.w * q.z + 2.0f * q.x * q.y, 1 - 2.0f * (q.z * q.z + q.x * q.x));
        }

        // all the math so far has been done in radians. Before returning, we convert to degrees...
        euler = glm::degrees(euler);

        //...and then ensure the degree values are between 0 and 360
        euler = glm::modf(glm::vec3{ 360.0f }, euler);

        return euler;
    }

    inline glm::quat EulerToQuaternion(const glm::vec3& euler)
    {
        float xOver2 = glm::radians(euler.x) * 0.5f;
        float yOver2 = glm::radians(euler.y) * 0.5f;
        float zOver2 = glm::radians(euler.z) * 0.5f;

        float sinXOver2 = std::sin(xOver2);
        float cosXOver2 = std::cos(xOver2);
        float sinYOver2 = std::sin(yOver2);
        float cosYOver2 = std::cos(yOver2);
        float sinZOver2 = std::sin(zOver2);
        float cosZOver2 = std::cos(zOver2);

        glm::quat result;
        result.x = cosYOver2 * sinXOver2 * cosZOver2 + sinYOver2 * cosXOver2 * sinZOver2;
        result.y = sinYOver2 * cosXOver2 * cosZOver2 - cosYOver2 * sinXOver2 * sinZOver2;
        result.z = cosYOver2 * cosXOver2 * sinZOver2 - sinYOver2 * sinXOver2 * cosZOver2;
        result.w = cosYOver2 * cosXOver2 * cosZOver2 + sinYOver2 * sinXOver2 * sinZOver2;

        return result;;
    }
}