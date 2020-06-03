#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>

#include "MainMenu.h"
#include "Dockyard.h"

namespace Ui {
    class ApplicationWindow;
}

class ApplicationWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ApplicationWindow(QWidget *parent = nullptr);
    ~ApplicationWindow();

private:
    Ui::ApplicationWindow *ui;

public slots:
    void startApp();
    void goToDockyard();
    void exitApp();
};

#endif // APPLICATIONWINDOW_H
