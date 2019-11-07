﻿#pragma once

class Viewer
	: public enable_shared_from_this<Viewer>
{
	friend class ViewerManager;

	bool	m_isRoot = false;
	bool	m_isDestroyed = false;
	Vec2	m_drawPos = Vec2::Zero();
	RectF	m_viewerRect = RectF(Scene::Size());
	Color	m_backgroundColor = Color(0, 0);
	unique_ptr<Transformer2D>	m_transformer;
	shared_ptr<Viewer>			m_parentViewer;
	Array<shared_ptr<Viewer>>	m_childViewers;
	static shared_ptr<Viewer>	g_mouseoveredViewer;

	static void UpdateAllViewers();

public:
	virtual ~Viewer() = default;

	static void Run();
	static const shared_ptr<Viewer>& GetRootViewer();

	const shared_ptr<Viewer>& getParentViewer() const { return m_parentViewer; }

	template <typename T>
	shared_ptr<T>	getParentViewer() const
	{
		if (dynamic_pointer_cast<T>(m_parentViewer) != nullptr) return dynamic_pointer_cast<T>(m_parentViewer);

		return nullptr;
	}

	template <typename T>
	shared_ptr<T>	getChildViewer() const
	{
		for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) return dynamic_pointer_cast<T>(*it);

		return nullptr;
	}

	template <typename T>
	bool	hasChildViewer() const
	{
		for (auto it = m_childViewers.begin(); it != m_childViewers.end(); ++it)
			if (dynamic_pointer_cast<T>(*it) != nullptr) return true;

		return false;
	}

	template <typename T, typename... Args>
	shared_ptr<T>	addChildViewer(Args&&... args)
	{
		auto cv = make_shared<T>(args...);

		m_childViewers.emplace_back(cv);
		cv->m_parentViewer = shared_from_this();
		cv->init();

		return cv;
	}

	void	destroy();

	const Array<shared_ptr<Viewer>>& getChildViewers() const { return m_childViewers; }

	// 再帰的
	Array<shared_ptr<Viewer>>	getAllChildViewers() const;

	// Set
	void	setBackgroundColor(const Color& color) { m_backgroundColor = color; }
	void	setDrawPos(const Vec2& pos) { m_drawPos = pos; m_transformer.reset(); m_transformer = make_unique<Transformer2D>(Mat3x2::Translate(m_drawPos), true); }
	void	setDrawPos(double x, double y) { setDrawPos(Vec2(x, y)); }
	void	setViewerSize(const Vec2& size) { m_viewerRect.size = size; }
	void	setViewerSize(double x, double y) { setViewerSize(Vec2(x, y)); }
	void	setViewerPos(const Vec2& pos) { m_viewerRect.pos = pos; }
	void	setViewerPos(double x, double y) { setViewerPos(Vec2(x, y)); }
	void	setViewerRect(const Vec2& pos, const Vec2& size) { m_viewerRect = RectF(pos, size); }
	void	setViewerRect(const Rect& rect) { m_viewerRect = rect; }
	void	setViewerRect(const RectF& rect) { m_viewerRect = rect; }
	void	setViewerRect(const Vec2& size) { m_viewerRect = RectF(size); }
	void	setViewerRect(double x, double y, double w, double h) { m_viewerRect = RectF(x, y, w, h); }
	void	setViewerRect(double w, double h) { m_viewerRect = RectF(w, h); }
	void	moveDrawPos(double dx, double dy) { setDrawPos(m_drawPos.movedBy(dx, dy)); }

	// Get
	bool	isMouseover() const { return shared_from_this() == g_mouseoveredViewer; }
	const RectF& getViewerRect() const { return m_viewerRect; }
	const Vec2& getViewerSize() const { return m_viewerRect.size; }
	const Vec2& getDrawPos() const { return m_drawPos; }
	const Vec2	getDrawCenter() const { return m_viewerRect.center(); }

	virtual void	init() {}
	virtual void	update() {}
};
