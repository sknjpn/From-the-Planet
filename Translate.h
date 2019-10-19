#pragma once

Vec2	TranslateWorldToScreen(const Mat4x4& mat, const Vec3& position);
Array<Vec2>	TranslateWorldToScreen(const Mat4x4& mat, const Array<Vec3>& positions);
