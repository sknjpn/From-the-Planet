#include "TruckState.h"
#include "ItemAsset.h"
#include "Road.h"
#include "Region.h"
#include "Translate.h"
#include "FacilityState.h"

void TruckState::update()
{
	const auto& cr = getCurrentRoad();
	const auto& fr = cr->getFr();
	const auto& to = cr->getTo();

	if ((m_progress += 0.2 / fr->getCenter().distanceFrom(to->getCenter())) > 1.0)
	{
		m_progress = 0.0;
		m_route.pop_front();

		if (m_route.isEmpty())
		{
			to->getFacilityState()->m_storage.addItem(m_itemAsset);
			to->getFacilityState()->m_inTransit.pullItem(m_itemAsset);
		}
	}
}

void TruckState::draw(const BasicCamera3D& camera) const
{
	float scale = 2.5;

	auto p = getPosition() / 100.0;
	auto t = float(-atan2(p.z, p.x));
	auto f = float(Math::HalfPi - asin(p.y));

	Mat4x4 mat = Mat4x4::Scaling(scale, scale, scale)
		* Mat4x4::Translation(0, float(getPosition().length()), 0)
		* Mat4x4::RotationZ(-f) * Mat4x4::RotationY(t)
		* camera.getMat4x4();
	Array<Vec3> positions({ {-1.0, 0.0, 1.0}, {-1.0, 0.0, -1.0}, {1.0, 0.0, -1.0}, {1.0, 0.0, 1.0} });

	auto vertices = TranslateWorldToScreen(mat, positions);
	Array<Vertex2D> vertex2Ds;
	for (int i = 0; i < 4; ++i)
	{
		auto& v = vertices[i];
		auto& v2D = vertex2Ds.emplace_back();
		v2D.pos = v;
		v2D.color = Float4(1.0, 1.0 ,1.0 ,1.0);
		switch (i)
		{
		case 0: v2D.tex = Float2(0, 0); break;
		case 1: v2D.tex = Float2(1, 0); break;
		case 2: v2D.tex = Float2(1, 1); break;
		case 3: v2D.tex = Float2(0, 1); break;
		};
	}
	Sprite sprite(vertex2Ds, Array<uint16>({ 0, 1, 2, 0, 2, 3 }));
	sprite.draw(m_itemAsset->getTexture());
}

Vec3 TruckState::getPosition() const
{
	const auto& cr = getCurrentRoad();
	const auto& fr = cr->getFr();
	const auto& to = cr->getTo();
	return fr->getCenter().lerp(to->getCenter(), m_progress);
}
