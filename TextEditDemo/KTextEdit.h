#pragma once

#include <QWidget>
#include <QTextEdit>

class KTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    KTextEdit(QWidget *parent = nullptr);
    void SetText(const QString &strText);
public slots:
    void OnCursorMove();
protected:
    void mouseReleaseEvent(QMouseEvent *e) override;
private:
    std::map<int, QList<QString>> m_OptionsMap;
    int m_iOriginalTextEndIndex;
    int m_iOriginalTextStartIndex;
};

