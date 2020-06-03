#include "ApplicationWindow.h"
#include "ui_ApplicationWindow.h"

ApplicationWindow::ApplicationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ApplicationWindow)
{
    ui->setupUi(this);
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    QPoint center = rect.center();
    center.setX(center.x() - (this->width()/2));
    center.setY(center.y() - (this->height()/2) - 15);
    move(center);
    startApp();
}

ApplicationWindow::~ApplicationWindow()
{
    delete ui;
}

void ApplicationWindow::startApp()
{
    MainMenu *mainMenu = new MainMenu();
    connect(mainMenu, &MainMenu::toDockyard, this, &ApplicationWindow::goToDockyard);
    connect(mainMenu, &MainMenu::exit, this, &ApplicationWindow::exitApp);
    setCentralWidget(mainMenu);
}

void ApplicationWindow::goToDockyard()
{
    Dockyard *dockyard = new Dockyard();
    connect(dockyard, &Dockyard::toMainMenu, this, &ApplicationWindow::startApp);
    setCentralWidget(dockyard);
}

void ApplicationWindow::exitApp()
{
    this->close();
}
