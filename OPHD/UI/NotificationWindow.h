#pragma once

#include "Core/Window.h"
#include "Core/Button.h"
#include "Core/TextArea.h"

#include "NotificationArea.h"

#include <NAS2D/Signal/Signal.h>


struct MapCoordinate;


class NotificationWindow : public Window
{
public:
	using TakeMeThereSignal = NAS2D::Signal<const MapCoordinate&>;

public:
	NotificationWindow();

	void notification(const NotificationArea::Notification&);

	TakeMeThereSignal::Source& takeMeThere() { return mTakeMeThereClicked; }

	void update() override;

private:
	void btnOkayClicked();
	void btnTakeMeThereClicked();

	const NAS2D::Image& mIcons;

	NotificationArea::Notification mNotification;
	Button btnOkay{"Okay", {this, &NotificationWindow::btnOkayClicked}};
	Button btnTakeMeThere{"Take Me There", {this, &NotificationWindow::btnTakeMeThereClicked}};
	TextArea mMessageArea;
	bool mTakeMeThereVisible{false};

	TakeMeThereSignal mTakeMeThereClicked;
};
