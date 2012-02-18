/********************************************************************************
** Form generated from reading UI file 'qg_trimamountoptions.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_TRIMAMOUNTOPTIONS_H
#define UI_QG_TRIMAMOUNTOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_TrimAmountOptions
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *lDist;
    QLineEdit *leDist;
    QCheckBox *cbTotalLength;
    QFrame *sep1;

    void setupUi(QWidget *QG_TrimAmountOptions)
    {
        if (QG_TrimAmountOptions->objectName().isEmpty())
            QG_TrimAmountOptions->setObjectName(QString::fromUtf8("QG_TrimAmountOptions"));
        QG_TrimAmountOptions->resize(110, 24);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_TrimAmountOptions->sizePolicy().hasHeightForWidth());
        QG_TrimAmountOptions->setSizePolicy(sizePolicy);
        QG_TrimAmountOptions->setMinimumSize(QSize(110, 22));
        QG_TrimAmountOptions->setMaximumSize(QSize(150, 24));
        hboxLayout = new QHBoxLayout(QG_TrimAmountOptions);
        hboxLayout->setSpacing(1);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lDist = new QLabel(QG_TrimAmountOptions);
        lDist->setObjectName(QString::fromUtf8("lDist"));
        lDist->setMinimumSize(QSize(51, 0));
        lDist->setWordWrap(false);

        hboxLayout->addWidget(lDist);

        leDist = new QLineEdit(QG_TrimAmountOptions);
        leDist->setObjectName(QString::fromUtf8("leDist"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(40);
        sizePolicy1.setVerticalStretch(22);
        sizePolicy1.setHeightForWidth(leDist->sizePolicy().hasHeightForWidth());
        leDist->setSizePolicy(sizePolicy1);
        leDist->setMinimumSize(QSize(40, 22));
        leDist->setMaximumSize(QSize(40, 22));

        hboxLayout->addWidget(leDist);

        cbTotalLength = new QCheckBox(QG_TrimAmountOptions);
        cbTotalLength->setObjectName(QString::fromUtf8("cbTotalLength"));
        cbTotalLength->setMinimumSize(QSize(8, 0));
        cbTotalLength->setMaximumSize(QSize(16, 16777215));

        hboxLayout->addWidget(cbTotalLength);

        sep1 = new QFrame(QG_TrimAmountOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy2);
        sep1->setMinimumSize(QSize(2, 0));
        sep1->setMaximumSize(QSize(2, 16777215));
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1);


        retranslateUi(QG_TrimAmountOptions);
        QObject::connect(leDist, SIGNAL(textChanged(QString)), QG_TrimAmountOptions, SLOT(updateDist(QString)));

        QMetaObject::connectSlotsByName(QG_TrimAmountOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_TrimAmountOptions)
    {
        QG_TrimAmountOptions->setWindowTitle(QApplication::translate("QG_TrimAmountOptions", "Trim Amount Options", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        QG_TrimAmountOptions->setToolTip(QApplication::translate("QG_TrimAmountOptions", "Distance. Negative values for trimming, positive values for extending.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lDist->setText(QApplication::translate("QG_TrimAmountOptions", "Length:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        leDist->setToolTip(QApplication::translate("QG_TrimAmountOptions", "Distance. Negative values for trimming, positive values for extending. Negative sign is ignored when trimming to final total length.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        cbTotalLength->setToolTip(QApplication::translate("QG_TrimAmountOptions", "The input length is used as total length after trimming, instead of length increase", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cbTotalLength->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_TrimAmountOptions: public Ui_QG_TrimAmountOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_TRIMAMOUNTOPTIONS_H
