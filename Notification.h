#include <giomm-2.4/giomm.h>

static Glib::RefPtr<Gio::Application> app;

class Notification {
public:
	Notification() {
		n = Gio::Notification::create("");
		n->set_body("");
		n->set_icon(Gio::ThemedIcon::create("dialog-information"));
	}

	void send_notification(void) {
		n->set_title("Hello world");
		n->set_body("This is an excample notification");
		app->send_notification(n);
	}

private:
	Glib::RefPtr<Gio::Notification> n;
};
