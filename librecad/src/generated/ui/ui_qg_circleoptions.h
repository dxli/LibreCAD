/********************************************************************************
** Form generated from reading UI file 'qg_circleoptions.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CIRCLEOPTIONS_H
#define UI_QG_CIRCLEOPTIONS_H

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

class Ui_QG_CircleOptions
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *lRadius;
    QLineEdit *leRadius;
    QFrame *sep1;

    void setupUi(QWidget *QG_CircleOptions)
    {
        if (QG_CircleOptions->objectName().isEmpty())
            QG_CircleOptions->setObjectName(QString::fromUtf8("QG_CircleOptions"));
        QG_CircleOptions->resize(150, 22);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CircleOptions->sizePolicy().hasHeightForWidth());
        QG_CircleOptions->setSizePolicy(sizePolicy);
        QG_CircleOptions->setMinimumSize(QSize(111, 22));
        QG_CircleOptions->setMaximumSize(QSize(150, 22));
        hboxLayout = new QHBoxLayout(QG_CircleOptions);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lRadius = new QLabel(QG_CircleOptions);
        lRadius->setObjectName(QString::fromUtf8("lRadius"));
        lRadius->setWordWrap(false);

        hboxLayout->addWidget(lRadius);

        leRadius = new QLineEdit(QG_CircleOptions);
        leRadius->setObjectName(QString::fromUtf8("leRadius"));

        hboxLayout->addWidget(leRadius);

        sep1 = new QFrame(QG_CircleOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(1));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy1);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1);


        retranslateUi(QG_CircleOptions);
        QObject::connect(leRadius, SIGNAL(textChanged(QString)), QG_CircleOptions, SLOT(updateRadius(QString)));

        QMetaObject::connectSlotsByName(QG_CircleOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_CircleOptions)
    {
        QG_CircleOptions->setWindowTitle(QApplication::translate("QG_CircleOptions", "Circle Options", 0, QApplication::UnicodeUTF8));
        lRadius->setText(QApplication::translate("QG_CircleOptions", "Radius:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_CircleOptions: public Ui_QG_CircleOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CIRCLEOPTIONS_H
