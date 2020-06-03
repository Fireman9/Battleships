#include "MainMenu.h"
#include "ui_MainMenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_playButton_clicked()
{
    this->close();
    emit toDockyard();
}

void MainMenu::on_exitButton_clicked()
{
    this->close();
    emit exit();
}
