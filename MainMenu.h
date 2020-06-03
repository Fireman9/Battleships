#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

namespace Ui {
    class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_playButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::MainMenu *ui;

signals:
    void toDockyard();
    void exit();
};

#endif // MAINMENU_H
