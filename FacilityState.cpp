#include "FacilityState.h"
#include "Region.h"

FacilityState::FacilityState()
{
	JSONReader json(U"quarry.json");

	for (auto j1 : json[U"meshes"].arrayView())
	{
		auto& m = m_meshes.emplace_back();
		for (auto j2 : j1[U"vertices"].arrayView())
			m.m_vertices.emplace_back(j2.get<Vec3>());

		m.m_color = j1[U"color"].get<Color>();
		m.m_thickness = j1[U"thickness"].get<double>();
	}
}

void FacilityState::draw(const BasicCamera3D& camera)
{
	if (camera.getEyePosition().distanceFrom(m_region.lock()->getCenter()) > Sqrt(camera.getEyePosition().lengthSq() - Square(100))) return;

	float scale = 4.0;

	auto p = m_region.lock()->m_position / 100.0;
	auto t = float(-atan2(p.z, p.x));
	auto f = float(Math::HalfPi - asin(p.y));

	Mat4x4 mat = Mat4x4::Scaling(scale, scale, scale)
		* Mat4x4::Translation(0, m_region.lock()->getCenter().length(), 0.0)
		* Mat4x4::RotationZ(-f) * Mat4x4::RotationY(t)
		* camera.getMat4x4();

	for (const auto& m : m_meshes) m.draw(mat);

	m_region.lock()->m_isMouseover = true;
}
