#pragma once

#include <qobject.h>
#include <qscreen.h>
#include <qtmetamacros.h>
#include <qwayland-wlr-foreign-toplevel-management-unstable-v1.h>
#include <qwindow.h>

#include "wayland-wlr-foreign-toplevel-management-unstable-v1-client-protocol.h"

namespace qs::wayland::toplevel_management::impl {

class ToplevelHandle
    : public QObject
    , public QtWayland::zwlr_foreign_toplevel_handle_v1 {
	Q_OBJECT;

public:
	[[nodiscard]] QString appId() const;
	[[nodiscard]] QString title() const;
	[[nodiscard]] QVector<QScreen*> visibleScreens() const;
	[[nodiscard]] ToplevelHandle* parent() const;
	[[nodiscard]] bool activated() const;
	[[nodiscard]] bool maximized() const;
	[[nodiscard]] bool minimized() const;
	[[nodiscard]] bool fullscreen() const;

	void activate();
	void setMaximized(bool maximized);
	void setMinimized(bool minimized);
	void setFullscreen(bool fullscreen);
	void fullscreenOn(QScreen* screen);
	void setRectangle(QWindow* window, QRect rect);

signals:
	// sent after the first done event.
	void ready();
	// sent right before delete this.
	void closed();

	void appIdChanged();
	void titleChanged();
	void visibleScreenAdded(QScreen* screen);
	void visibleScreenRemoved(QScreen* screen);
	void parentChanged();
	void activatedChanged();
	void maximizedChanged();
	void minimizedChanged();
	void fullscreenChanged();

private slots:
	void onParentClosed();
	void onRectWindowDestroyed();
	void onScreenAdded(QScreen* screen);

private:
	void zwlr_foreign_toplevel_handle_v1_done() override;
	void zwlr_foreign_toplevel_handle_v1_closed() override;
	void zwlr_foreign_toplevel_handle_v1_app_id(const QString& appId) override;
	void zwlr_foreign_toplevel_handle_v1_title(const QString& title) override;
	void zwlr_foreign_toplevel_handle_v1_state(wl_array* stateArray) override;
	void zwlr_foreign_toplevel_handle_v1_output_enter(wl_output* output) override;
	void zwlr_foreign_toplevel_handle_v1_output_leave(wl_output* output) override;
	void zwlr_foreign_toplevel_handle_v1_parent(::zwlr_foreign_toplevel_handle_v1* parent) override;

	bool isReady = false;
	QString mAppId;
	QString mTitle;
	QVector<QScreen*> mVisibleScreens;
	QVector<wl_output*> mPendingVisibleScreens;
	ToplevelHandle* mParent = nullptr;
	bool mActivated = false;
	bool mMaximized = false;
	bool mMinimized = false;
	bool mFullscreen = false;
	QWindow* rectWindow = nullptr;
};

} // namespace qs::wayland::toplevel_management::impl
