#include <gtkmm.h>

#include "stapik/locale/LocaleManager.hpp"
#include "stapik/storage/AppPaths.hpp"
#include "stapik/theme/ThemeManager.hpp"
#include "stapik/ui/style/AppStyleProvider.hpp"
#include "ui/window/MainWindow.hpp"

namespace
{
    constexpr auto APP_NAME = "stapikbudgeting";
}

int main(const int argc, char *argv[])
{
    const auto app = Gtk::Application::create("pl.stapik.budgeting");

    AppStyleProvider styleProvider(AppPaths::resourcesDir());

    app->signal_activate().connect([&]
    {
        LocaleManager::instance(APP_NAME);

        styleProvider.apply(ThemeManager::instance(APP_NAME).getTheme());
        ThemeManager::instance().signalThemeChanged().connect(
            [&styleProvider] { styleProvider.apply(ThemeManager::instance().getTheme()); });

        auto* window = new MainWindow();
        app->add_window(*window);
        window->show();
    });
    return app->run(argc, argv);
}
