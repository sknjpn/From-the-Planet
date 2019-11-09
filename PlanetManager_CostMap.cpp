#include "PlanetManager.h"
#include "Region.h"
#include "Road.h"

void PlanetManager::resetCostMap()
{
	for (const auto& region : m_regions)
		region->m_from = nullptr;
}

void PlanetManager::bakeCostMap(const shared_ptr<Region>& from, function<bool(const shared_ptr<Road>&)> func)
{
	// CostMapのリセット
	resetCostMap();

	// Seedの登録
	Array<shared_ptr<Region>> list;
	list.emplace_back(from);

	// Baking
	for (int i = 0; i < list.size(); ++i)
	{
		auto l = list[i];
		for (const auto& road : l->m_roads)
		{
			auto t = road->getTo();
			if (t != from && (!t->m_from || t->m_cost > l->m_cost + road->getCost()) && func(road))
			{
				list.emplace_back(t);
				t->m_from = l;
				t->m_cost = l->m_cost + road->getCost();
			}
		}
	}
}

Array<shared_ptr<Road>> PlanetManager::getRoute(const shared_ptr<Region> from, const shared_ptr<Region> to) const
{
	if (!from->m_from) return Array<shared_ptr<Road>>();

	Array<shared_ptr<Road>> result;

	result.emplace_back(from->getRoad(from->m_from));

	for (;;)
	{
		if (!result.back()->getTo()->m_from) break;

		result.emplace_back(result.back()->getTo()->getRoad(result.back()->getTo()->m_from));
	}

	return result;
}

double PlanetManager::getCost(const shared_ptr<Region> from, const shared_ptr<Region> to) const
{
	const auto route = getRoute(from, to);

	double sum = 0.0;
	for (const auto& r : route)
		sum += r->getCost();
}
