/********************************************************************************
** Form generated from reading UI file 'qg_coordinatewidget.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_COORDINATEWIDGET_H
#define UI_QG_COORDINATEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_CoordinateWidget
{
public:
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLabel *lCoord1;
    QLabel *lCoord1b;
    QFrame *line1;
    QVBoxLayout *vboxLayout1;
    QLabel *lCoord2;
    QLabel *lCoord2b;

    void setupUi(QWidget *QG_CoordinateWidget)
    {
        if (QG_CoordinateWidget->objectName().isEmpty())
            QG_CoordinateWidget->setObjectName(QString::fromUtf8("QG_CoordinateWidget"));
        QG_CoordinateWidget->resize(380, 27);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CoordinateWidget->sizePolicy().hasHeightForWidth());
        QG_CoordinateWidget->setSizePolicy(sizePolicy);
        QG_CoordinateWidget->setMinimumSize(QSize(380, 27));
        QG_CoordinateWidget->setMaximumSize(QSize(500, 50));
        hboxLayout = new QHBoxLayout(QG_CoordinateWidget);
        hboxLayout->setSpacing(0);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        lCoord1 = new QLabel(QG_CoordinateWidget);
        lCoord1->setObjectName(QString::fromUtf8("lCoord1"));
        QFont font;
        font.setFamily(QString::fromUtf8("Helvetica"));
        font.setPointSize(7);
        lCoord1->setFont(font);
        lCoord1->setFrameShape(QFrame::NoFrame);
        lCoord1->setFrameShadow(QFrame::Plain);
        lCoord1->setWordWrap(false);

        vboxLayout->addWidget(lCoord1);

        lCoord1b = new QLabel(QG_CoordinateWidget);
        lCoord1b->setObjectName(QString::fromUtf8("lCoord1b"));
        lCoord1b->setFont(font);
        lCoord1b->setFrameShape(QFrame::NoFrame);
        lCoord1b->setFrameShadow(QFrame::Plain);
        lCoord1b->setWordWrap(false);

        vboxLayout->addWidget(lCoord1b);


        hboxLayout->addLayout(vboxLayout);

        line1 = new QFrame(QG_CoordinateWidget);
        line1->setObjectName(QString::fromUtf8("line1"));
        line1->setFrameShape(QFrame::VLine);
        line1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(line1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(0);
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        lCoord2 = new QLabel(QG_CoordinateWidget);
        lCoord2->setObjectName(QString::fromUtf8("lCoord2"));
        lCoord2->setFont(font);
        lCoord2->setWordWrap(false);

        vboxLayout1->addWidget(lCoord2);

        lCoord2b = new QLabel(QG_CoordinateWidget);
        lCoord2b->setObjectName(QString::fromUtf8("lCoord2b"));
        lCoord2b->setFont(font);
        lCoord2b->setWordWrap(false);

        vboxLayout1->addWidget(lCoord2b);


        hboxLayout->addLayout(vboxLayout1);


        retranslateUi(QG_CoordinateWidget);

        QMetaObject::connectSlotsByName(QG_CoordinateWidget);
    } // setupUi

    void retranslateUi(QWidget *QG_CoordinateWidget)
    {
        QG_CoordinateWidget->setWindowTitle(QApplication::translate("QG_CoordinateWidget", "Coordinates", 0, QApplication::UnicodeUTF8));
        lCoord1->setText(QApplication::translate("QG_CoordinateWidget", "Coordinates", 0, QApplication::UnicodeUTF8));
        lCoord1b->setText(QApplication::translate("QG_CoordinateWidget", "Coordinates", 0, QApplication::UnicodeUTF8));
        lCoord2->setText(QApplication::translate("QG_CoordinateWidget", "Coordinates", 0, QApplication::UnicodeUTF8));
        lCoord2b->setText(QApplication::translate("QG_CoordinateWidget", "Coordinates", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_CoordinateWidget: public Ui_QG_CoordinateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_COORDINATEWIDGET_H
