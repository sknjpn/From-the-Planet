#include "Road.h"
#include "Region.h"
#include "Mesh.h"

void Road::draw(const BasicCamera3D& camera) const
{
	auto p0 = m_fr.lock()->getCenter();
	auto p1 = m_to.lock()->getCenter();
	auto c = (p0 + p1) / 2.0;
	auto distance = camera.getEyePosition().distanceFrom(c);

	if (distance > Sqrt(camera.getEyePosition().lengthSq() - Square(100))) return;

	auto d = (p1 - p0).cross(p1).normalized() * 1.5;
	Mesh mesh(Array<Vec3>({ p0 + d, p0 - d, p1 - d, p1 + d }), Palette::Lightgrey, 4.0);

	mesh.drawFill(camera.getMat4x4());
}
