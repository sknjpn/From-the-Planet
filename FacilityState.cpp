#include "FacilityState.h"
#include "FacilityAsset.h"
#include "PlanetManager.h"
#include "Region.h"
#include "Road.h"
#include "TruckState.h"
#include "TerrainAsset.h"

void FacilityState::draw(const BasicCamera3D& camera)
{
	float scale = 3.0;

	auto p = m_region.lock()->getPosition() / 100.0;
	auto t = float(-atan2(p.z, p.x));
	auto f = float(Math::HalfPi - asin(p.y));

	auto k = EaseOutQuart(m_constructionProgress);
	auto s = (9.0 + Sin(m_progress * Math::TwoPi)) / 10.0;
	Mat4x4 mat = Mat4x4::Scaling(float(scale / s), float(scale * k * s * s * (1.0 - g_planetManagerPtr->m_destroy)), float(scale / s))
		* Mat4x4::Translation(0, float(m_region.lock()->getPosition().length()), 0)
		* Mat4x4::RotationZ(-f) * Mat4x4::RotationY(t)
		* camera.getMat4x4();

	for (const auto& m : m_facilityAsset->getMeshes()) m.drawBlack(mat);
	for (const auto& m : m_facilityAsset->getMeshes()) m.draw(mat, ColorF(m.m_color, (1.0 - g_planetManagerPtr->m_destroy) * (0.5 + m_constructionProgress / 2.0)));

	m_volume = Min(1.0, 10000.0 / camera.getEyePosition().distanceFromSq(getPosition()));
}

void FacilityState::update()
{
	if (!m_isInit) { m_isInit = true; onInit(); }

	if (!m_isActive && m_facilityAsset->m_material <= m_storage)
	{
		if ((m_constructionProgress += 0.05) >= 1.0)
		{
			m_isActive = true;
			m_storage.clear();
			m_constructionProgress = 1.0;

			onConstructed();
		}
	}

	if (m_isActive) exportItem();
}

Vec3 FacilityState::getPosition() const
{
	return m_region.lock()->getPosition();
}

void FacilityState::exportItem()
{
	// 送付
	for (const auto& fs : m_connected)
	{
		for (auto i : fs->m_need.getItemList())
		{
			if (m_export.numItem(i.first) > 0)
			{
				// Itemのやり取り
				fs->m_inTransit.addItem(i.first);
				fs->m_need.pullItem(i.first);
				m_export.pullItem(i.first);
				
				// インスタンス
				auto& ts = g_planetManagerPtr->m_truckStates.emplace_back(MakeShared<TruckState>());
				ts->m_itemAsset = i.first;
				ts->m_progress = 0.0;

				// Cost Map
				const auto func = [](const shared_ptr<Road>& r) { return
					(r->getTo()->getTerrainAsset()->m_isSea && r->getFr()->hasHarbor()) ||
					(r->getTo()->hasHarbor() && r->getTo()->getFacilityState()->m_isActive && r->getFr()->getTerrainAsset()->m_isSea) ||
					(!r->getTo()->getTerrainAsset()->m_isSea && !r->getTo()->hasHarbor() && !r->getFr()->getTerrainAsset()->m_isSea) ||
					(!r->getTo()->getTerrainAsset()->m_isSea && r->getTo()->hasHarbor() && r->getTo()->getFacilityState()->m_isActive && !r->getFr()->getTerrainAsset()->m_isSea) ||
					(r->getTo()->getTerrainAsset()->m_isSea && r->getFr()->getTerrainAsset()->m_isSea);
				};
				g_planetManagerPtr->bakeCostMap(fs->m_region.lock(), func);
				ts->m_route = g_planetManagerPtr->getRoute(m_region.lock(), fs->m_region.lock());

				return;
			}
		}
	}
}

void FacilityState::onInit()
{
	// 必要なものの登録
	m_need = m_facilityAsset->m_material;

	g_planetManagerPtr->addDamage(m_facilityAsset->m_constructinDamage);
}

void FacilityState::updateConnected()
{
	m_connected.clear();

	const auto func = [](const shared_ptr<Road>& r) { return
		(r->getFr()->getTerrainAsset()->m_isSea && r->getTo()->hasHarbor()) ||
		(r->getFr()->hasHarbor() && r->getFr()->getFacilityState()->m_isActive && r->getTo()->getTerrainAsset()->m_isSea) ||
		(!r->getFr()->getTerrainAsset()->m_isSea && !r->getFr()->hasHarbor() && !r->getTo()->getTerrainAsset()->m_isSea) ||
		(!r->getFr()->getTerrainAsset()->m_isSea && r->getFr()->hasHarbor() && r->getFr()->getFacilityState()->m_isActive && !r->getTo()->getTerrainAsset()->m_isSea) ||
		(r->getFr()->getTerrainAsset()->m_isSea && r->getTo()->getTerrainAsset()->m_isSea);
	};

	g_planetManagerPtr->bakeCostMap(m_region.lock(), func);

	for (const auto& fs : g_planetManagerPtr->m_facilityStates)
		if (fs->m_region.lock()->getFrom()) m_connected.emplace_back(fs);

	m_connected.sort_by([this](const auto& fs1, const auto& fs2) {return fs1->m_region.lock()->getCost() < fs2->m_region.lock()->getCost(); });
}
