#include "AppInfo.hpp"
#include "ui/window/MainWindow.hpp"

#include "stapik/app/StapikApplication.hpp"
#include "stapik/app/version.hpp"
#include "stapik/ui/dialog/DialogUtils.hpp"

#include <string>

int main(const int argc, char *argv[])
{
    const stapik::app::AppInfo appInfo{
        .applicationId = APPLICATION_ID,
        .internalName = std::string{stapik::app::build::NAME},
        .displayName = std::string{stapik::app::build::DISPLAY_NAME},
        .version = std::string{stapik::app::build::VERSION},
        .author = APP_AUTHOR,
        .repositoryUrl = APP_REPOSITORY_URL
    };

    stapik::app::StapikApplicationOptions options;
    options.checkForUpdates = true;

    const auto app = stapik::app::StapikApplication::create(appInfo, options);

    app->signal_activate().connect([&app]
    {
        if (const auto windows = app->get_windows(); !windows.empty())
        {
            windows.front()->present();
            return;
        }

        auto* window = new MainWindow();
        deleteOnHide(window);
        app->add_window(*window);
        window->present();
    });

    return app->run(argc, argv);
}
