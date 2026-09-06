#include <gtkmm.h>

#include "ui/window/MainWindow.hpp"

int main(const int argc, char *argv[])
{
    const auto app = Gtk::Application::create("pl.stapik.budgeting");

    app->signal_activate().connect([&]
    {
        auto* window = new MainWindow();
        app->add_window(*window);
        window->show();
    });
    return app->run(argc, argv);
}