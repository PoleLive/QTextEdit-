#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
namespace Ui {
class MainWindow;
}
class QTextEdit;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void InitStyle();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTextEdit *m_pInput;
    std::map<int, QList<QString>> m_OptionsMap;
    int m_iOriginalTextEndIndex;
    int m_iOriginalTextStartIndex;
    int m_iBlockOffset;
};

#endif // MAINWINDOW_H
