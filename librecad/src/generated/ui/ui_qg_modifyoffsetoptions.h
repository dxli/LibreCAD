/********************************************************************************
** Form generated from reading UI file 'qg_modifyoffsetoptions.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_MODIFYOFFSETOPTIONS_H
#define UI_QG_MODIFYOFFSETOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_ModifyOffsetOptions
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *lDist;
    QLineEdit *leDist;
    QFrame *sep1;

    void setupUi(QWidget *QG_ModifyOffsetOptions)
    {
        if (QG_ModifyOffsetOptions->objectName().isEmpty())
            QG_ModifyOffsetOptions->setObjectName(QString::fromUtf8("QG_ModifyOffsetOptions"));
        QG_ModifyOffsetOptions->resize(112, 22);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_ModifyOffsetOptions->sizePolicy().hasHeightForWidth());
        QG_ModifyOffsetOptions->setSizePolicy(sizePolicy);
        QG_ModifyOffsetOptions->setMinimumSize(QSize(100, 22));
        QG_ModifyOffsetOptions->setMaximumSize(QSize(140, 26));
        hboxLayout = new QHBoxLayout(QG_ModifyOffsetOptions);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lDist = new QLabel(QG_ModifyOffsetOptions);
        lDist->setObjectName(QString::fromUtf8("lDist"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lDist->sizePolicy().hasHeightForWidth());
        lDist->setSizePolicy(sizePolicy1);
        lDist->setWordWrap(false);

        hboxLayout->addWidget(lDist);

        leDist = new QLineEdit(QG_ModifyOffsetOptions);
        leDist->setObjectName(QString::fromUtf8("leDist"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(leDist->sizePolicy().hasHeightForWidth());
        leDist->setSizePolicy(sizePolicy2);

        hboxLayout->addWidget(leDist);

        sep1 = new QFrame(QG_ModifyOffsetOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy3);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1);


        retranslateUi(QG_ModifyOffsetOptions);
        QObject::connect(leDist, SIGNAL(textChanged(QString)), QG_ModifyOffsetOptions, SLOT(updateDist(QString)));

        QMetaObject::connectSlotsByName(QG_ModifyOffsetOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_ModifyOffsetOptions)
    {
        QG_ModifyOffsetOptions->setWindowTitle(QApplication::translate("QG_ModifyOffsetOptions", "Modify Offset Options", 0, QApplication::UnicodeUTF8));
        lDist->setText(QApplication::translate("QG_ModifyOffsetOptions", "Distance:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        leDist->setToolTip(QApplication::translate("QG_ModifyOffsetOptions", "Offset from original entity", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_ModifyOffsetOptions: public Ui_QG_ModifyOffsetOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_MODIFYOFFSETOPTIONS_H
