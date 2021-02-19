#include "math/Vector2f.h"

Vector2f& Vector2f::round()
{
	mX = (float)(int)(mX + 0.5f);
	mY = (float)(int)(mY + 0.5f);

	return *this;

} // round

Vector2f& Vector2f::lerp(const Vector2f& _start, const Vector2f& _end, const float _fraction)
{
	mX = Math::lerp(_start.x(), _end.x(), _fraction);
	mY = Math::lerp(_start.y(), _end.y(), _fraction);

	return *this;

} // lerp

const Vector2f Vector2f::parseString(const std::string& _input)
{
	Vector2f ret = Vector2f(0, 0);

	size_t divider = _input.find(' ');
	if (divider != std::string::npos)
	{
		std::string first = _input.substr(0, divider);
		std::string second = _input.substr(divider, std::string::npos);

		ret = Vector2f((float)atof(first.c_str()), (float)atof(second.c_str()));
	}

	return ret;
}


const std::string Vector2f::toString()
{
	return std::to_string(mX) + " " + std::to_string(mY);
}