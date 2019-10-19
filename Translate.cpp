#include "stdafx.h"
#include "Translate.h"

Vec2 TranslateWorldToScreen(const Mat4x4& mat, const Vec3& position)
{
	Float3 out;
	Float3 input = position;

	SIMD::Vector3TransformCoordStream(&out, &input, 1, mat);

	const Float2 resolution = Graphics2D::GetRenderTargetSize();

	out.x += 1.0f;
	out.y += 1.0f;
	out.x *= 0.5f * resolution.x;
	out.y *= 0.5f;
	out.y = 1.0f - out.y;
	out.y *= resolution.y;

	return out.xy();
}

Array<Vec2> TranslateWorldToScreen(const Mat4x4& mat, const Array<Vec3>& positions)
{
	const size_t size = positions.size();

	Array<Float3> out(size);
	Array<Float3> input(size);

	for (int i = 0; i < size; ++i)
		input[i] = positions[i];

	SIMD::Vector3TransformCoordStream(&out.front(), &input.front(), size, mat);

	const Float2 resolution = Graphics2D::GetRenderTargetSize();

	Array<Vec2> result(size);
	for (int i = 0; i < size; ++i)
	{
		auto& v = out[i];
		v.x += 1.0f;
		v.y += 1.0f;
		v.x *= 0.5f * resolution.x;
		v.y *= 0.5f;
		v.y = 1.0f - v.y;
		v.y *= resolution.y;

		result[i] = v.xy();
	}

	return result;
}
