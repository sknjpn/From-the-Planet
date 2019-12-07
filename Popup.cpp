#include "Popup.h"

Popup::Popup(const Vec2& viewerSize)
{
	setViewerSize(viewerSize);

	setBackgroundColor(ColorF(0.2));
}

void Popup::init()
{
	// EasyViewer Rectの設定
	{
		RectF rect(Cursor::PosF(), getViewerSize());

		// Scene内になるように移動
		if (rect.pos.x < 0) rect.pos.x = 0;
		if (rect.pos.y < 0) rect.pos.y = 0;
		if (rect.br().x > Scene::Size().x) rect.moveBy(Scene::Size().x - rect.br().x, 0);
		if (rect.br().y > Scene::Size().y) rect.moveBy(0, Scene::Size().y - rect.br().y);

		setViewerRectInLocal(rect);
	}
}
