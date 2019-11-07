#include "Road.h"
#include "Region.h"
#include "Mesh.h"

void Road::draw(const BasicCamera3D& camera) const
{
	auto p0 = m_fr->getCenter();
	auto p1 = m_to->getCenter();
	auto c = (p0 + p1) / 2.0;
	auto d = (p1 - p0).cross(p1).normalized() * 1.5;
	Mesh(Array<Vec3>({ p0 + d, p0 - d * 0.8, p1 - d, p1 + d * 0.8 }), ColorF(Palette::Gray, 1.0), 4.0).drawFill(camera.getMat4x4());
	Mesh(Array<Vec3>({ p0 + d * 1.0, p0 + d * 0.8, p1 + d * 0.8, p1 + d * 1.0 }), ColorF(Palette::White, 1.0), 4.0).drawFill(camera.getMat4x4());
	Mesh(Array<Vec3>({ p0 - d * 0.8, p0 - d * 1.0, p1 - d * 1.0, p1 - d * 0.8 }), ColorF(Palette::White, 1.0), 4.0).drawFill(camera.getMat4x4());
}

double Road::getLength() const
{
	return m_fr->getCenter().distanceFrom(m_to->getCenter());
}
