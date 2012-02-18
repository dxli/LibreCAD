/********************************************************************************
** Form generated from reading UI file 'qg_arctangentialoptions.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_ARCTANGENTIALOPTIONS_H
#define UI_QG_ARCTANGENTIALOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_ArcTangentialOptions
{
public:
    QFrame *sep1;
    QLineEdit *leRadius;
    QLineEdit *leAngle;
    QRadioButton *rbRadius;
    QRadioButton *rbAngle;

    void setupUi(QWidget *QG_ArcTangentialOptions)
    {
        if (QG_ArcTangentialOptions->objectName().isEmpty())
            QG_ArcTangentialOptions->setObjectName(QString::fromUtf8("QG_ArcTangentialOptions"));
        QG_ArcTangentialOptions->resize(258, 24);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_ArcTangentialOptions->sizePolicy().hasHeightForWidth());
        QG_ArcTangentialOptions->setSizePolicy(sizePolicy);
        QG_ArcTangentialOptions->setMinimumSize(QSize(248, 22));
        sep1 = new QFrame(QG_ArcTangentialOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        sep1->setGeometry(QRect(124, 0, 16, 24));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy1);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);
        leRadius = new QLineEdit(QG_ArcTangentialOptions);
        leRadius->setObjectName(QString::fromUtf8("leRadius"));
        leRadius->setGeometry(QRect(70, 0, 60, 22));
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(leRadius->sizePolicy().hasHeightForWidth());
        leRadius->setSizePolicy(sizePolicy2);
        leAngle = new QLineEdit(QG_ArcTangentialOptions);
        leAngle->setObjectName(QString::fromUtf8("leAngle"));
        leAngle->setGeometry(QRect(192, 0, 60, 22));
        sizePolicy2.setHeightForWidth(leAngle->sizePolicy().hasHeightForWidth());
        leAngle->setSizePolicy(sizePolicy2);
        rbRadius = new QRadioButton(QG_ArcTangentialOptions);
        rbRadius->setObjectName(QString::fromUtf8("rbRadius"));
        rbRadius->setGeometry(QRect(0, 0, 70, 22));
        rbRadius->setIconSize(QSize(12, 12));
        rbRadius->setChecked(true);
        rbAngle = new QRadioButton(QG_ArcTangentialOptions);
        rbAngle->setObjectName(QString::fromUtf8("rbAngle"));
        rbAngle->setGeometry(QRect(130, 0, 65, 22));
        rbAngle->setIconSize(QSize(12, 12));
        QWidget::setTabOrder(leRadius, leAngle);

        retranslateUi(QG_ArcTangentialOptions);
        QObject::connect(leRadius, SIGNAL(textChanged(QString)), QG_ArcTangentialOptions, SLOT(updateRadius(QString)));
        QObject::connect(leAngle, SIGNAL(textChanged(QString)), QG_ArcTangentialOptions, SLOT(updateAngle(QString)));

        QMetaObject::connectSlotsByName(QG_ArcTangentialOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_ArcTangentialOptions)
    {
        QG_ArcTangentialOptions->setWindowTitle(QApplication::translate("QG_ArcTangentialOptions", "Tangential Arc Options", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        leRadius->setToolTip(QApplication::translate("QG_ArcTangentialOptions", "Radius of the tangential arc", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        leAngle->setToolTip(QApplication::translate("QG_ArcTangentialOptions", "Total subtending angle of the tangential arc", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbRadius->setText(QApplication::translate("QG_ArcTangentialOptions", "Radius", 0, QApplication::UnicodeUTF8));
        rbAngle->setText(QApplication::translate("QG_ArcTangentialOptions", "Angle", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_ArcTangentialOptions: public Ui_QG_ArcTangentialOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_ARCTANGENTIALOPTIONS_H
