#include "parentwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Adding the startup image
    QPixmap pixmap(":/images/SplashScreen.png");
    QSplashScreen splash(pixmap);
    splash.show();

    a.processEvents();

    ParentWindow w;
    // Delaying the start of the app to show the splashscreen
    QTimer::singleShot(1000, &w, SLOT(show()));
    //w.show(); // if you want to make it run faster comment the previous line and uncomment this line
    splash.finish(&w);
    return a.exec();
}

