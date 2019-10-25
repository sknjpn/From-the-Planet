﻿#include "Road.h"
#include "Region.h"
#include "Mesh.h"

void Road::draw(const BasicCamera3D& camera) const
{
	auto p0 = m_fr->getCenter();
	auto p1 = m_to->getCenter();
	auto c = (p0 + p1) / 2.0;
	auto d = (p1 - p0).cross(p1).normalized() * 1.5;
	Mesh mesh(Array<Vec3>({ p0 + d, p0 - d, p1 - d, p1 + d }), Palette::Lightgrey, 4.0);

	mesh.drawFill(camera.getMat4x4());
}
