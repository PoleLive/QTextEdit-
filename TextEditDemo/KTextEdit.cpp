#include "KTextEdit.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QMenu>
#include <QtDebug>
#include <QList>
#include <map>
using namespace std;
KTextEdit::KTextEdit(QWidget *parent):
    QTextEdit(parent)
{

}

void KTextEdit::SetText(const QString &strTextOrgin)
{
    setText("");
    m_OptionsMap.clear();
    QList<QString> clickable_text_list;

    QRegExp rx("(\\[\.+(\\|\.+)+\\])");
    rx.setMinimal(true);
    int pos = 0;
    QString strText(strTextOrgin);
    while ((pos = rx.indexIn(strText, pos)) != -1) {
        QString content = rx.cap(1);
        qDebug() << content;
        clickable_text_list.append(content);
        content = content.left(content.length() - 1);
        content = content.right(content.length() - 1);
        QList<QString> datas = content.split("|");
        m_OptionsMap[clickable_text_list.size()-1] = datas;
        pos += rx.matchedLength();
    }

    QTextCursor tTextCursor = textCursor();
    QTextCharFormat normal_charformat = tTextCursor.charFormat();
    QTextCharFormat clickable_charformat = tTextCursor.charFormat();
    clickable_charformat.setAnchor(true);

    clickable_charformat.setForeground(QColor(80, 20, 250));
    for(int i = 0; i < clickable_text_list.size(); i++)
    {
        tTextCursor.setCharFormat(normal_charformat);
        setTextCursor(tTextCursor);
        int idx = strText.indexOf(clickable_text_list[i]);
        tTextCursor.insertText(strText.left(idx));
        strText = strText.right(strText.length() - idx - clickable_text_list[i].length());
        clickable_charformat.setAnchorName(QString::number(i));
        tTextCursor.setCharFormat(clickable_charformat);
        setTextCursor(tTextCursor);
        tTextCursor.insertText(m_OptionsMap[i][0]);
    }
    tTextCursor.setCharFormat(normal_charformat);
    setTextCursor(tTextCursor);
    tTextCursor.insertText(strText);
}

void KTextEdit::OnCursorMove()
{
    QTextCursor tTextCursor = textCursor();
    if(tTextCursor.hasSelection())
        return;
    QTextCharFormat clickable_charformat = tTextCursor.charFormat();
    if(clickable_charformat.isAnchor())
    {
        QTextBlock block = tTextCursor.block();
        m_iBlockOffset = block.position();
        QString text = block.text();
        m_iOriginalTextEndIndex = tTextCursor.positionInBlock() ;
        m_iOriginalTextStartIndex = m_iOriginalTextEndIndex - 1;
        if(tTextCursor.movePosition(QTextCursor::Left))
        {
            QTextCharFormat fmt1 = tTextCursor.charFormat();
            while(fmt1.anchorName() == clickable_charformat.anchorName() && fmt1.isAnchor())
            {

                if(!tTextCursor.movePosition(QTextCursor::Left))
                    break;
                m_iOriginalTextStartIndex = tTextCursor.positionInBlock();
                fmt1 = tTextCursor.charFormat();
            }
        }

        if(tTextCursor.movePosition(QTextCursor::Right))
        {
            QTextCharFormat fmt1 = tTextCursor.charFormat();
            while(fmt1.anchorName() == clickable_charformat.anchorName())
            {
                m_iOriginalTextEndIndex = tTextCursor.positionInBlock();
                if(!tTextCursor.movePosition(QTextCursor::Right))
                    break;
                fmt1 = tTextCursor.charFormat();
            }
        }
        QMenu menu;
        text = text.right(text.length() - m_iOriginalTextStartIndex);
        text = text.left(m_iOriginalTextEndIndex - m_iOriginalTextStartIndex );

        int id = clickable_charformat.anchorName().toInt();
        for(int i = 0; i < m_OptionsMap[id].size(); i++)
        {
            QAction *pAction  = menu.addAction(m_OptionsMap[id][i]);
            connect(pAction, &QAction::triggered, [=](){
                QTextCursor text_cursor = textCursor();
                int start = m_iBlockOffset + m_iOriginalTextStartIndex;
                if(start < 0)
                    start =0;
                for(int i = m_iBlockOffset + m_iOriginalTextEndIndex; i >= start; i--)
                {
                    text_cursor.setPosition(i);
                    QTextCharFormat ff = text_cursor.charFormat();
                    if(ff.anchorName() == clickable_charformat.anchorName())
                    {
                        text_cursor.deletePreviousChar();
                    }
                }
                text_cursor.setPosition(start);
                text_cursor.insertText(m_OptionsMap[id][i], clickable_charformat);
            });
        }
        menu.move(cursor().pos());
        menu.setStyleSheet("* {font-size: 12pt}");
        menu.exec();
    }
}

void KTextEdit::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        OnCursorMove();
    }
    QTextEdit::mousePressEvent(e);
}
