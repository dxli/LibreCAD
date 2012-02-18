/********************************************************************************
** Form generated from reading UI file 'qg_arcoptions.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_ARCOPTIONS_H
#define UI_QG_ARCOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_ArcOptions
{
public:
    QGroupBox *buttonGroup1;
    QRadioButton *rbNeg;
    QRadioButton *rbPos;
    QFrame *sep1;

    void setupUi(QWidget *QG_ArcOptions)
    {
        if (QG_ArcOptions->objectName().isEmpty())
            QG_ArcOptions->setObjectName(QString::fromUtf8("QG_ArcOptions"));
        QG_ArcOptions->resize(96, 22);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_ArcOptions->sizePolicy().hasHeightForWidth());
        QG_ArcOptions->setSizePolicy(sizePolicy);
        QG_ArcOptions->setMinimumSize(QSize(96, 22));
        buttonGroup1 = new QGroupBox(QG_ArcOptions);
        buttonGroup1->setObjectName(QString::fromUtf8("buttonGroup1"));
        buttonGroup1->setGeometry(QRect(0, 0, 91, 23));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonGroup1->sizePolicy().hasHeightForWidth());
        buttonGroup1->setSizePolicy(sizePolicy1);
        buttonGroup1->setFlat(true);
        buttonGroup1->setProperty("lineWidth", QVariant(0));
        rbNeg = new QRadioButton(buttonGroup1);
        rbNeg->setObjectName(QString::fromUtf8("rbNeg"));
        rbNeg->setGeometry(QRect(47, 0, 47, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/dirneg.png"), QSize(), QIcon::Normal, QIcon::Off);
        rbNeg->setIcon(icon);
        rbNeg->setCheckable(true);
        rbPos = new QRadioButton(buttonGroup1);
        rbPos->setObjectName(QString::fromUtf8("rbPos"));
        rbPos->setGeometry(QRect(0, 0, 47, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/dirpos.png"), QSize(), QIcon::Normal, QIcon::Off);
        rbPos->setIcon(icon1);
        rbPos->setChecked(true);
        sep1 = new QFrame(buttonGroup1);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        sep1->setGeometry(QRect(40, 0, 16, 19));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy2);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        retranslateUi(QG_ArcOptions);
        QObject::connect(rbPos, SIGNAL(toggled(bool)), QG_ArcOptions, SLOT(updateDirection(bool)));
        QObject::connect(rbNeg, SIGNAL(toggled(bool)), QG_ArcOptions, SLOT(updateDirection(bool)));

        QMetaObject::connectSlotsByName(QG_ArcOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_ArcOptions)
    {
        QG_ArcOptions->setWindowTitle(QApplication::translate("QG_ArcOptions", "Arc Options", 0, QApplication::UnicodeUTF8));
        buttonGroup1->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        rbNeg->setToolTip(QApplication::translate("QG_ArcOptions", "Clockwise", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbNeg->setText(QString());
#ifndef QT_NO_TOOLTIP
        rbPos->setToolTip(QApplication::translate("QG_ArcOptions", "Counterclockwise", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbPos->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_ArcOptions: public Ui_QG_ArcOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_ARCOPTIONS_H
