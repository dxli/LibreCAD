/********************************************************************************
** Form generated from reading UI file 'qg_linepolygonoptions.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_LINEPOLYGONOPTIONS_H
#define UI_QG_LINEPOLYGONOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_LinePolygonOptions
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *lNumber;
    QSpinBox *sbNumber;
    QFrame *sep1;

    void setupUi(QWidget *QG_LinePolygonOptions)
    {
        if (QG_LinePolygonOptions->objectName().isEmpty())
            QG_LinePolygonOptions->setObjectName(QString::fromUtf8("QG_LinePolygonOptions"));
        QG_LinePolygonOptions->resize(140, 22);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(4), static_cast<QSizePolicy::Policy>(5));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_LinePolygonOptions->sizePolicy().hasHeightForWidth());
        QG_LinePolygonOptions->setSizePolicy(sizePolicy);
        QG_LinePolygonOptions->setMinimumSize(QSize(100, 22));
        QG_LinePolygonOptions->setMaximumSize(QSize(140, 22));
        hboxLayout = new QHBoxLayout(QG_LinePolygonOptions);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lNumber = new QLabel(QG_LinePolygonOptions);
        lNumber->setObjectName(QString::fromUtf8("lNumber"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lNumber->sizePolicy().hasHeightForWidth());
        lNumber->setSizePolicy(sizePolicy1);
        lNumber->setWordWrap(false);

        hboxLayout->addWidget(lNumber);

        sbNumber = new QSpinBox(QG_LinePolygonOptions);
        sbNumber->setObjectName(QString::fromUtf8("sbNumber"));
        sbNumber->setMaximum(9999);
        sbNumber->setMinimum(3);

        hboxLayout->addWidget(sbNumber);

        sep1 = new QFrame(QG_LinePolygonOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(1));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy2);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1);


        retranslateUi(QG_LinePolygonOptions);
        QObject::connect(sbNumber, SIGNAL(valueChanged(int)), QG_LinePolygonOptions, SLOT(updateNumber(int)));

        QMetaObject::connectSlotsByName(QG_LinePolygonOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_LinePolygonOptions)
    {
        QG_LinePolygonOptions->setWindowTitle(QApplication::translate("QG_LinePolygonOptions", "Polygon Options", 0, QApplication::UnicodeUTF8));
        lNumber->setText(QApplication::translate("QG_LinePolygonOptions", "Number:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        sbNumber->setProperty("toolTip", QVariant(QApplication::translate("QG_LinePolygonOptions", "Number of edges", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_LinePolygonOptions: public Ui_QG_LinePolygonOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_LINEPOLYGONOPTIONS_H
