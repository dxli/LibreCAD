/********************************************************************************
** Form generated from reading UI file 'qg_dimlinearoptions.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DIMLINEAROPTIONS_H
#define UI_QG_DIMLINEAROPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_DimLinearOptions
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *lAngle;
    QLineEdit *leAngle;
    QToolButton *bHor;
    QToolButton *bVer;
    QFrame *sep1;

    void setupUi(QWidget *QG_DimLinearOptions)
    {
        if (QG_DimLinearOptions->objectName().isEmpty())
            QG_DimLinearOptions->setObjectName(QString::fromUtf8("QG_DimLinearOptions"));
        QG_DimLinearOptions->resize(200, 22);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DimLinearOptions->sizePolicy().hasHeightForWidth());
        QG_DimLinearOptions->setSizePolicy(sizePolicy);
        QG_DimLinearOptions->setMinimumSize(QSize(180, 22));
        QG_DimLinearOptions->setMaximumSize(QSize(200, 22));
        hboxLayout = new QHBoxLayout(QG_DimLinearOptions);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lAngle = new QLabel(QG_DimLinearOptions);
        lAngle->setObjectName(QString::fromUtf8("lAngle"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lAngle->sizePolicy().hasHeightForWidth());
        lAngle->setSizePolicy(sizePolicy1);
        lAngle->setMinimumSize(QSize(0, 19));
        lAngle->setFrameShape(QFrame::NoFrame);
        lAngle->setFrameShadow(QFrame::Plain);
        lAngle->setWordWrap(false);

        hboxLayout->addWidget(lAngle);

        leAngle = new QLineEdit(QG_DimLinearOptions);
        leAngle->setObjectName(QString::fromUtf8("leAngle"));
        sizePolicy.setHeightForWidth(leAngle->sizePolicy().hasHeightForWidth());
        leAngle->setSizePolicy(sizePolicy);
        leAngle->setMinimumSize(QSize(0, 19));

        hboxLayout->addWidget(leAngle);

        bHor = new QToolButton(QG_DimLinearOptions);
        bHor->setObjectName(QString::fromUtf8("bHor"));
        sizePolicy1.setHeightForWidth(bHor->sizePolicy().hasHeightForWidth());
        bHor->setSizePolicy(sizePolicy1);
        bHor->setMinimumSize(QSize(0, 19));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/dimhor.png"), QSize(), QIcon::Normal, QIcon::Off);
        bHor->setIcon(icon);

        hboxLayout->addWidget(bHor);

        bVer = new QToolButton(QG_DimLinearOptions);
        bVer->setObjectName(QString::fromUtf8("bVer"));
        sizePolicy1.setHeightForWidth(bVer->sizePolicy().hasHeightForWidth());
        bVer->setSizePolicy(sizePolicy1);
        bVer->setMinimumSize(QSize(0, 19));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/dimver.png"), QSize(), QIcon::Normal, QIcon::Off);
        bVer->setIcon(icon1);

        hboxLayout->addWidget(bVer);

        sep1 = new QFrame(QG_DimLinearOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy2);
        sep1->setMinimumSize(QSize(0, 19));
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1);


        retranslateUi(QG_DimLinearOptions);
        QObject::connect(leAngle, SIGNAL(textChanged(QString)), QG_DimLinearOptions, SLOT(updateAngle(QString)));
        QObject::connect(bHor, SIGNAL(clicked()), QG_DimLinearOptions, SLOT(setHor()));
        QObject::connect(bVer, SIGNAL(clicked()), QG_DimLinearOptions, SLOT(setVer()));

        QMetaObject::connectSlotsByName(QG_DimLinearOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_DimLinearOptions)
    {
        QG_DimLinearOptions->setWindowTitle(QApplication::translate("QG_DimLinearOptions", "Linear Dimension Options", 0, QApplication::UnicodeUTF8));
        lAngle->setText(QApplication::translate("QG_DimLinearOptions", "Angle:", 0, QApplication::UnicodeUTF8));
        leAngle->setText(QString());
        bHor->setText(QString());
        bVer->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_DimLinearOptions: public Ui_QG_DimLinearOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DIMLINEAROPTIONS_H
