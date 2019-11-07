#pragma once

class ItemAsset;
class Road;

class TruckState
{
public:
	Array<shared_ptr<Road>>	m_route;
	shared_ptr<ItemAsset>	m_itemAsset;
	double	m_progress = 0.0;

	void	update();
	void	draw(const BasicCamera3D& camera) const;
	
	Vec3	getPosition() const;

	const shared_ptr<Road>& getCurrentRoad() const { return m_route.front(); }
};

