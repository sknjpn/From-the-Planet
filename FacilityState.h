#pragma once

#include "Mesh.h"
#include "Storage.h"

class Region;
class FacilityAsset;

class FacilityState
	: public enable_shared_from_this<FacilityState>
{
public:
	double	m_constructionProgress = 0.0;
	double	m_progress = 0.0;
	Storage	m_export;
	Storage	m_storage;
	Storage	m_need;
	Storage	m_inTransit;
	bool	m_isActive = false;
	bool	m_isInit = false;
	Audio	m_audio;
	double	m_volume;
	Array<shared_ptr<FacilityState>>	m_connected;

public:
	weak_ptr<Region>	m_region;
	shared_ptr<FacilityAsset>	m_facilityAsset;

	void draw(const BasicCamera3D& camera);
	virtual void update();

	Vec3	getPosition() const;

	void exportItem();

	virtual void onInit();
	virtual void onConstructed() {}

	void	updateConnected();
};

