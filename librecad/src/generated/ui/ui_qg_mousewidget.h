/********************************************************************************
** Form generated from reading UI file 'qg_mousewidget.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_MOUSEWIDGET_H
#define UI_QG_MOUSEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_MouseWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *lRightButton;
    QLabel *lLeftButton;
    QToolButton *toolButton;

    void setupUi(QWidget *QG_MouseWidget)
    {
        if (QG_MouseWidget->objectName().isEmpty())
            QG_MouseWidget->setObjectName(QString::fromUtf8("QG_MouseWidget"));
        QG_MouseWidget->resize(318, 28);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_MouseWidget->sizePolicy().hasHeightForWidth());
        QG_MouseWidget->setSizePolicy(sizePolicy);
        QG_MouseWidget->setMinimumSize(QSize(300, 28));
        QG_MouseWidget->setMaximumSize(QSize(500, 50));
        gridLayout = new QGridLayout(QG_MouseWidget);
        gridLayout->setSpacing(5);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lRightButton = new QLabel(QG_MouseWidget);
        lRightButton->setObjectName(QString::fromUtf8("lRightButton"));
        sizePolicy.setHeightForWidth(lRightButton->sizePolicy().hasHeightForWidth());
        lRightButton->setSizePolicy(sizePolicy);
        lRightButton->setMinimumSize(QSize(0, 28));
        lRightButton->setMaximumSize(QSize(32767, 28));
        QFont font;
        font.setFamily(QString::fromUtf8("Helvetica"));
        lRightButton->setFont(font);
        lRightButton->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lRightButton->setWordWrap(false);

        gridLayout->addWidget(lRightButton, 0, 2, 1, 1);

        lLeftButton = new QLabel(QG_MouseWidget);
        lLeftButton->setObjectName(QString::fromUtf8("lLeftButton"));
        sizePolicy.setHeightForWidth(lLeftButton->sizePolicy().hasHeightForWidth());
        lLeftButton->setSizePolicy(sizePolicy);
        lLeftButton->setMinimumSize(QSize(0, 27));
        lLeftButton->setMaximumSize(QSize(32767, 28));
        lLeftButton->setFont(font);
        lLeftButton->setFrameShape(QFrame::NoFrame);
        lLeftButton->setFrameShadow(QFrame::Plain);
        lLeftButton->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        lLeftButton->setWordWrap(false);

        gridLayout->addWidget(lLeftButton, 0, 0, 1, 1);

        toolButton = new QToolButton(QG_MouseWidget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ui/mouse.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);

        gridLayout->addWidget(toolButton, 0, 1, 1, 1);


        retranslateUi(QG_MouseWidget);

        QMetaObject::connectSlotsByName(QG_MouseWidget);
    } // setupUi

    void retranslateUi(QWidget *QG_MouseWidget)
    {
        QG_MouseWidget->setWindowTitle(QApplication::translate("QG_MouseWidget", "Mouse", 0, QApplication::UnicodeUTF8));
        lRightButton->setText(QApplication::translate("QG_MouseWidget", "Right", 0, QApplication::UnicodeUTF8));
        lLeftButton->setText(QApplication::translate("QG_MouseWidget", "Left", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("QG_MouseWidget", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_MouseWidget: public Ui_QG_MouseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_MOUSEWIDGET_H
