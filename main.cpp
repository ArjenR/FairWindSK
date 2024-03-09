#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

#include "FairWindSK.hpp"
#include "ui/MainWindow.hpp"



using namespace Qt::StringLiterals;

int main(int argc, char *argv[]) {

    // Set the organization name
    QCoreApplication::setOrganizationName("uniparthenope.it");

    // Enable OpenGL shared contexts
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts );

    // The QT application
    QApplication app(argc, argv);

    // Set the window icon
    QApplication::setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/resources/images/icons/fairwind_icon.png"))));

    // Get the splash screen logo
    QPixmap pixmap(":/resources/images/other/splash_logo.png");

    // Create a splash screen containing the logo
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);

    // Show the logo
    splash.show();

    // Show message
    splash.showMessage("Welcome to FairWindSK a GUI for the Signal K server!", 500, Qt::white);

    // Get the FairWind singleton
    auto fairWindSK = fairwindsk::FairWindSK::getInstance();

    // Load the configuration inside the FairWind singleton itself
    fairWindSK->loadConfig();

    // Start the connection
    fairWindSK->startSignalK();

    // Load the apps inside the FairWind singleton itself
    fairWindSK->loadApps();
    
    // Check if the virtual keyboard have to be activated
    if (fairWindSK->useVirtualKeyboard()) {

        // Activate the virtual keyboard
        qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    }

    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);

    // Create a new MainWindow object
    fairwindsk::ui::MainWindow w;

    // Close the splash screen presenting the MainWindow UI
    splash.finish((QWidget *)&w);

    return QApplication::exec();
}
