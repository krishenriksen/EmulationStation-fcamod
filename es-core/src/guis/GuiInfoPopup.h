#pragma once
#ifndef ES_APP_GUIS_GUI_INFO_POPUP_H
#define ES_APP_GUIS_GUI_INFO_POPUP_H

#include "GuiComponent.h"
#include "Window.h"

class ComponentGrid;
class NinePatchComponent;

class GuiInfoPopup : public GuiComponent, public Window::InfoPopup
{
public:
	GuiInfoPopup(Window* window, std::string message, int duration);
	~GuiInfoPopup();
	void render(const Transform4x4f& parentTrans) override;
	inline void stop() { running = false; };
private:
	std::string mMessage;
	int mDuration;
	int alpha;
	bool updateState();
	int mStartTime;
	ComponentGrid* mGrid;
	NinePatchComponent* mFrame;
	bool running;
	unsigned int mBackColor;	
};

#endif // ES_APP_GUIS_GUI_INFO_POPUP_H
