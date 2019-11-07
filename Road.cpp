#include "Road.h"
#include "Region.h"
#include "Mesh.h"
#include "RoadAsset.h"

Road::Road(const shared_ptr<Region> fr, const shared_ptr<Region> to)
	: m_fr(fr)
	, m_to(to)
{
	m_length = m_fr->getPosition().distanceFrom(m_to->getPosition());
}

void Road::draw(const BasicCamera3D& camera) const
{
	auto p0 = m_fr->getPosition();
	auto p1 = m_to->getPosition();
	auto c = (p0 + p1) / 2.0;
	auto d = (p1 - p0).cross(p1).normalized() * 1.5;
	Mesh(Array<Vec3>({ p0 + d, p0 - d * 0.8, p1 - d, p1 + d * 0.8 }), m_roadAsset->m_colorInside, 4.0).drawFill(camera.getMat4x4());
	Mesh(Array<Vec3>({ p0 + d * 1.0, p0 + d * 0.8, p1 + d * 0.8, p1 + d * 1.0 }), m_roadAsset->m_colorOutside, 4.0).drawFill(camera.getMat4x4());
	Mesh(Array<Vec3>({ p0 - d * 0.8, p0 - d * 1.0, p1 - d * 1.0, p1 - d * 0.8 }), m_roadAsset->m_colorOutside, 4.0).drawFill(camera.getMat4x4());
}

double Road::getLength() const
{
	return m_length;
}

double Road::getCost() const
{
	if (m_roadAsset) return m_length / m_roadAsset->getSpeed();
	else return m_length;
}
