/********************************************************************************
** Form generated from reading UI file 'qg_lineoptions.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_LINEOPTIONS_H
#define UI_QG_LINEOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_LineOptions
{
public:
    QHBoxLayout *horizontalLayout;
    QToolButton *bClose;
    QToolButton *bUndo;
    QToolButton *bRedo;
    QFrame *sep1;

    void setupUi(QWidget *QG_LineOptions)
    {
        if (QG_LineOptions->objectName().isEmpty())
            QG_LineOptions->setObjectName(QString::fromUtf8("QG_LineOptions"));
        QG_LineOptions->resize(124, 22);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_LineOptions->sizePolicy().hasHeightForWidth());
        QG_LineOptions->setSizePolicy(sizePolicy);
        QG_LineOptions->setMinimumSize(QSize(94, 22));
        QG_LineOptions->setMaximumSize(QSize(280, 22));
        horizontalLayout = new QHBoxLayout(QG_LineOptions);
        horizontalLayout->setSpacing(1);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 0, 0, 0);
        bClose = new QToolButton(QG_LineOptions);
        bClose->setObjectName(QString::fromUtf8("bClose"));
        bClose->setIconSize(QSize(24, 22));
        bClose->setCheckable(false);

        horizontalLayout->addWidget(bClose);

        bUndo = new QToolButton(QG_LineOptions);
        bUndo->setObjectName(QString::fromUtf8("bUndo"));

        horizontalLayout->addWidget(bUndo);

        bRedo = new QToolButton(QG_LineOptions);
        bRedo->setObjectName(QString::fromUtf8("bRedo"));

        horizontalLayout->addWidget(bRedo);

        sep1 = new QFrame(QG_LineOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy1);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(sep1);


        retranslateUi(QG_LineOptions);
        QObject::connect(bClose, SIGNAL(clicked()), QG_LineOptions, SLOT(close()));
        QObject::connect(bUndo, SIGNAL(clicked()), QG_LineOptions, SLOT(undo()));
        QObject::connect(bRedo, SIGNAL(clicked()), QG_LineOptions, SLOT(redo()));

        QMetaObject::connectSlotsByName(QG_LineOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_LineOptions)
    {
        QG_LineOptions->setWindowTitle(QApplication::translate("QG_LineOptions", "Line Options", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bClose->setToolTip(QApplication::translate("QG_LineOptions", "Form a closed contour from lines drawn", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bClose->setText(QApplication::translate("QG_LineOptions", "Close", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bUndo->setToolTip(QApplication::translate("QG_LineOptions", "Undo the last line drawing", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bUndo->setText(QApplication::translate("QG_LineOptions", "Undo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bRedo->setToolTip(QApplication::translate("QG_LineOptions", "Redo the last line drawing", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bRedo->setText(QApplication::translate("QG_LineOptions", "Redo", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_LineOptions: public Ui_QG_LineOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_LINEOPTIONS_H
