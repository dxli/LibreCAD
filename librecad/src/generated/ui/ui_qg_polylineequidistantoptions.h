/********************************************************************************
** Form generated from reading UI file 'qg_polylineequidistantoptions.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_POLYLINEEQUIDISTANTOPTIONS_H
#define UI_QG_POLYLINEEQUIDISTANTOPTIONS_H

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

class Ui_QG_PolylineEquidistantOptions
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *lLength;
    QLineEdit *leDist;
    QLabel *lNumber;
    QLineEdit *leNumber;
    QFrame *sep1;

    void setupUi(QWidget *QG_PolylineEquidistantOptions)
    {
        if (QG_PolylineEquidistantOptions->objectName().isEmpty())
            QG_PolylineEquidistantOptions->setObjectName(QString::fromUtf8("QG_PolylineEquidistantOptions"));
        QG_PolylineEquidistantOptions->resize(180, 24);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_PolylineEquidistantOptions->sizePolicy().hasHeightForWidth());
        QG_PolylineEquidistantOptions->setSizePolicy(sizePolicy);
        QG_PolylineEquidistantOptions->setMinimumSize(QSize(120, 22));
        QG_PolylineEquidistantOptions->setMaximumSize(QSize(180, 32767));
        horizontalLayout = new QHBoxLayout(QG_PolylineEquidistantOptions);
        horizontalLayout->setSpacing(1);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 0, 0, 0);
        lLength = new QLabel(QG_PolylineEquidistantOptions);
        lLength->setObjectName(QString::fromUtf8("lLength"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lLength->sizePolicy().hasHeightForWidth());
        lLength->setSizePolicy(sizePolicy1);
        lLength->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lLength->setWordWrap(false);

        horizontalLayout->addWidget(lLength);

        leDist = new QLineEdit(QG_PolylineEquidistantOptions);
        leDist->setObjectName(QString::fromUtf8("leDist"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(leDist->sizePolicy().hasHeightForWidth());
        leDist->setSizePolicy(sizePolicy2);
        leDist->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(leDist);

        lNumber = new QLabel(QG_PolylineEquidistantOptions);
        lNumber->setObjectName(QString::fromUtf8("lNumber"));
        sizePolicy1.setHeightForWidth(lNumber->sizePolicy().hasHeightForWidth());
        lNumber->setSizePolicy(sizePolicy1);
        lNumber->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lNumber->setWordWrap(false);

        horizontalLayout->addWidget(lNumber);

        leNumber = new QLineEdit(QG_PolylineEquidistantOptions);
        leNumber->setObjectName(QString::fromUtf8("leNumber"));
        sizePolicy2.setHeightForWidth(leNumber->sizePolicy().hasHeightForWidth());
        leNumber->setSizePolicy(sizePolicy2);
        leNumber->setMaximumSize(QSize(24, 16777215));

        horizontalLayout->addWidget(leNumber);

        sep1 = new QFrame(QG_PolylineEquidistantOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy3);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(sep1);


        retranslateUi(QG_PolylineEquidistantOptions);
        QObject::connect(leDist, SIGNAL(textChanged(QString)), QG_PolylineEquidistantOptions, SLOT(updateDist(QString)));
        QObject::connect(leNumber, SIGNAL(textChanged(QString)), QG_PolylineEquidistantOptions, SLOT(updateNumber(QString)));

        QMetaObject::connectSlotsByName(QG_PolylineEquidistantOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_PolylineEquidistantOptions)
    {
        QG_PolylineEquidistantOptions->setWindowTitle(QApplication::translate("QG_PolylineEquidistantOptions", "Line Relative Angle Options", 0, QApplication::UnicodeUTF8));
        lLength->setText(QApplication::translate("QG_PolylineEquidistantOptions", "Spacing:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        leDist->setToolTip(QApplication::translate("QG_PolylineEquidistantOptions", "Distance from original polyline", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lNumber->setText(QApplication::translate("QG_PolylineEquidistantOptions", "Copies:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        leNumber->setToolTip(QApplication::translate("QG_PolylineEquidistantOptions", "Distance from original polyline", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_PolylineEquidistantOptions: public Ui_QG_PolylineEquidistantOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_POLYLINEEQUIDISTANTOPTIONS_H
