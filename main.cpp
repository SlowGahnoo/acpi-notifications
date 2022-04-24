#include <iostream>
#include <giomm-2.4/giomm.h>
#include <sys/socket.h>

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


int main (int argc, char *argv[])
{
	app = Gio::Application::create("", Gio::APPLICATION_FLAGS_NONE);
	app->register_application();
	return 0;
}
