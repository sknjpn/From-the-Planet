#include "FacilityState.h"
#include "FacilityAsset.h"
#include "Region.h"

void FacilityState::draw(const BasicCamera3D& camera)
{
	float scale = 4.0;

	auto p = m_region.lock()->getPosition() / 100.0;
	auto t = float(-atan2(p.z, p.x));
	auto f = float(Math::HalfPi - asin(p.y));

	Mat4x4 mat = Mat4x4::Scaling(scale, scale, scale)
		* Mat4x4::Translation(0, float(m_region.lock()->getCenter().length()), 0)
		* Mat4x4::RotationZ(-f) * Mat4x4::RotationY(t)
		* camera.getMat4x4();

	for (const auto& m : m_facilityAsset->getMeshes()) m.draw(mat);
}
