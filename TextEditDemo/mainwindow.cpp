#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextEdit>
#include <QTextCursor>
#include <QTextBlock>
#include <QMenu>
#include <QtDebug>
#include <QList>
#include <map>
#include "KTextEdit.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    KTextEdit *pEdit = new KTextEdit();
    setCentralWidget(pEdit);
    InitStyle();
    QString strText = "[我|你|他]有一个[梦想|想法|麻烦]。\n[我|你|他]有一个[梦想|想法|麻烦]。\n[我|你|他]有一个[梦想|想法|麻烦]。\n[我|你|他]有一个[梦想|想法|麻烦]。\n[我|你|他]有一个[梦想|想法|麻烦]。";

    pEdit->SetText(strText);
}

void MainWindow::InitStyle()
{
    setStyleSheet("* {font-size: 12pt}");
    resize(600, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

