/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
** Copyright (C) 2016 ravas (github.com/r-a-v-a-s)
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/

#include "qg_lsp_commandedit.h"

#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QKeyEvent>
#include <QRegularExpression>
#include <QTextStream>

#include "rs_commands.h"
#include "rs_dialogfactory.h"
#include "rs_math.h"
#include "rs_settings.h"

namespace {
// Limits for command file reading
// limit for the number of lines read together
constexpr unsigned g_maxLinesToRead = 10240;
// the maximum line length allowed
constexpr unsigned g_maxLineLength = 4096;

}

/**
 * Default Constructor. You must call init manually if you choose
 * to use this constructor.
 */
QG_Lsp_CommandEdit::QG_Lsp_CommandEdit(QWidget* parent)
    : QLineEdit(parent)
    , keycode_mode(false)
    , relative_ray("none")
    , calculator_mode(false)

{
    setStyleSheet("selection-color: white; selection-background-color: green;");
    setFrame(false);
    setFocusPolicy(Qt::StrongFocus);
    prombt();
    //connect(this, SIGNAL(selectionChanged()), this, SLOT(positionChanged()));

    QObject::connect(
        this,
        &QLineEdit::cursorPositionChanged,
        this,
        [this](){
            if (cursorPosition() < prombtSize()) {
                prombt();
                setCursorPosition(prombtSize());
            }
        });
}

/**
 * Bypass for key press events from keys...
 */
bool QG_Lsp_CommandEdit::event(QEvent* e) {
    return QLineEdit::event(e);
}

/**
 * History (arrow key up/down) support
 */
void QG_Lsp_CommandEdit::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
    case Qt::Key_Up:
        if (!historyList.isEmpty() && it > historyList.begin())
        {
            it--;
            setText(*it);
        }
        break;

    case Qt::Key_Down:
        if (!historyList.isEmpty() && it < historyList.end() )
        {
            it++;
            if (it<historyList.end()) {
                setText(*it);
            }
            else {
                setText("");
                //prombt();
            }
        }
        break;

    case Qt::Key_Enter:
    case Qt::Key_Return:
        processInput(text());
        break;
    case Qt::Key_Escape:
        if (text().isEmpty()) {
            emit escape();
        }
        else {
            setText("");
        }
        break;
    default:
        QLineEdit::keyPressEvent(e);
        break;
    }

    if (keycode_mode)
    {
        qDebug() << __func__ << "keycode_mode";
        auto input = text();
        if (input.size() == 2)
        {
            emit keycode(input);
        }
    }

}

void QG_Lsp_CommandEdit::focusInEvent(QFocusEvent *e) {
    qDebug() << __func__;
    emit focusIn();
    QLineEdit::focusInEvent(e);
}

void QG_Lsp_CommandEdit::focusOutEvent(QFocusEvent *e) {
    emit focusOut();
    QLineEdit::focusOutEvent(e);
}

void QG_Lsp_CommandEdit::processInput(QString input)
{
    if (input != "")
    {
        historyList.append(input);
        it = historyList.end();
        emit message(input);
        prombt();
    }
    else
    {
        historyList.append("\n");
        it = historyList.end();
        emit message("\n");
        prombt();
    }
    //return cmd;
}

void QG_Lsp_CommandEdit::readCommandFile(const QString& path)
{
    // author: ravas
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // keep the pos of the read part
    size_t pos = 0;
    bool ended = false;
    while (!ended) {
        if (!file.isOpen())
        {
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                break;
            file.skip(pos);
        }

        // read lines to buffer and close the file immediately
        QTextStream txt_stream(&file);
        QStringList lines;
        for(unsigned i=0; i < g_maxLinesToRead; ++i) {
            if (txt_stream.atEnd())
                break;
            lines << txt_stream.readLine(g_maxLineLength);
        }
        ended = txt_stream.atEnd();
        pos = txt_stream.pos();

        // Issue #1803: close the file to avoid blocking command loading
        file.close();

        // Process the commands while the file is closed
        for (QString line: lines) {
            line.remove(" ");
            if (!line.startsWith("#"))
                processInput(line);
        }
    }
}

void QG_Lsp_CommandEdit::modifiedPaste()
{
    auto txt = qApp->clipboard()->text();
    txt.replace("\n", ";");
    setText(txt);
}
