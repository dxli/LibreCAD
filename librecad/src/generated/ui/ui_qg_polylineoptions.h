/********************************************************************************
** Form generated from reading UI file 'qg_polylineoptions.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_POLYLINEOPTIONS_H
#define UI_QG_POLYLINEOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_PolylineOptions
{
public:
    QToolButton *bClose;
    QToolButton *bUndo;
    QComboBox *cbMode;
    QLabel *lRadius;
    QLineEdit *leRadius;
    QLabel *lAngle;
    QLineEdit *leAngle;
    QGroupBox *buttonGroup1;
    QFrame *sep1;
    QRadioButton *rbPos;
    QRadioButton *rbNeg;

    void setupUi(QWidget *QG_PolylineOptions)
    {
        if (QG_PolylineOptions->objectName().isEmpty())
            QG_PolylineOptions->setObjectName(QString::fromUtf8("QG_PolylineOptions"));
        QG_PolylineOptions->resize(380, 24);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_PolylineOptions->sizePolicy().hasHeightForWidth());
        QG_PolylineOptions->setSizePolicy(sizePolicy);
        QG_PolylineOptions->setMinimumSize(QSize(380, 24));
        QG_PolylineOptions->setMaximumSize(QSize(650, 22));
        bClose = new QToolButton(QG_PolylineOptions);
        bClose->setObjectName(QString::fromUtf8("bClose"));
        bClose->setGeometry(QRect(0, 0, 40, 21));
        bUndo = new QToolButton(QG_PolylineOptions);
        bUndo->setObjectName(QString::fromUtf8("bUndo"));
        bUndo->setGeometry(QRect(41, 0, 40, 21));
        cbMode = new QComboBox(QG_PolylineOptions);
        cbMode->setObjectName(QString::fromUtf8("cbMode"));
        cbMode->setGeometry(QRect(82, 0, 90, 22));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cbMode->sizePolicy().hasHeightForWidth());
        cbMode->setSizePolicy(sizePolicy1);
        cbMode->setMinimumSize(QSize(90, 0));
        lRadius = new QLabel(QG_PolylineOptions);
        lRadius->setObjectName(QString::fromUtf8("lRadius"));
        lRadius->setGeometry(QRect(173, 0, 46, 22));
        lRadius->setAlignment(Qt::AlignVCenter);
        lRadius->setWordWrap(false);
        leRadius = new QLineEdit(QG_PolylineOptions);
        leRadius->setObjectName(QString::fromUtf8("leRadius"));
        leRadius->setGeometry(QRect(220, 0, 44, 22));
        leRadius->setMaximumSize(QSize(44, 22));
        lAngle = new QLabel(QG_PolylineOptions);
        lAngle->setObjectName(QString::fromUtf8("lAngle"));
        lAngle->setGeometry(QRect(265, 0, 40, 22));
        lAngle->setAlignment(Qt::AlignVCenter);
        lAngle->setWordWrap(false);
        leAngle = new QLineEdit(QG_PolylineOptions);
        leAngle->setObjectName(QString::fromUtf8("leAngle"));
        leAngle->setGeometry(QRect(306, 0, 44, 22));
        leAngle->setMaximumSize(QSize(44, 22));
        buttonGroup1 = new QGroupBox(QG_PolylineOptions);
        buttonGroup1->setObjectName(QString::fromUtf8("buttonGroup1"));
        buttonGroup1->setGeometry(QRect(521, 0, 16, 22));
        buttonGroup1->setFlat(true);
        sep1 = new QFrame(QG_PolylineOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        sep1->setGeometry(QRect(378, 0, 16, 22));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy2);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);
        rbPos = new QRadioButton(QG_PolylineOptions);
        rbPos->setObjectName(QString::fromUtf8("rbPos"));
        rbPos->setGeometry(QRect(349, 1, 14, 22));
        rbPos->setChecked(true);
        rbNeg = new QRadioButton(QG_PolylineOptions);
        rbNeg->setObjectName(QString::fromUtf8("rbNeg"));
        rbNeg->setGeometry(QRect(364, 0, 16, 22));
        rbNeg->setMinimumSize(QSize(16, 0));

        retranslateUi(QG_PolylineOptions);
        QObject::connect(bClose, SIGNAL(clicked()), QG_PolylineOptions, SLOT(close()));
        QObject::connect(bUndo, SIGNAL(clicked()), QG_PolylineOptions, SLOT(undo()));
        QObject::connect(leRadius, SIGNAL(textChanged(QString)), QG_PolylineOptions, SLOT(updateRadius(QString)));
        QObject::connect(leAngle, SIGNAL(textChanged(QString)), QG_PolylineOptions, SLOT(updateAngle(QString)));
        QObject::connect(cbMode, SIGNAL(activated(int)), QG_PolylineOptions, SLOT(updateMode(int)));
        QObject::connect(rbNeg, SIGNAL(toggled(bool)), QG_PolylineOptions, SLOT(updateDirection(bool)));
        QObject::connect(rbPos, SIGNAL(toggled(bool)), QG_PolylineOptions, SLOT(updateDirection(bool)));

        QMetaObject::connectSlotsByName(QG_PolylineOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_PolylineOptions)
    {
        QG_PolylineOptions->setWindowTitle(QApplication::translate("QG_PolylineOptions", "Polyline Options", 0, QApplication::UnicodeUTF8));
        bClose->setText(QApplication::translate("QG_PolylineOptions", "Close", 0, QApplication::UnicodeUTF8));
        bUndo->setText(QApplication::translate("QG_PolylineOptions", "Undo", 0, QApplication::UnicodeUTF8));
        cbMode->clear();
        cbMode->insertItems(0, QStringList()
         << QApplication::translate("QG_PolylineOptions", "Line", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_PolylineOptions", "Tangential", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_PolylineOptions", "Tan Radius", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_PolylineOptions", "Angle", 0, QApplication::UnicodeUTF8)
        );
        lRadius->setText(QApplication::translate("QG_PolylineOptions", "Radius:", 0, QApplication::UnicodeUTF8));
        lAngle->setText(QApplication::translate("QG_PolylineOptions", "Angle:", 0, QApplication::UnicodeUTF8));
        buttonGroup1->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        rbPos->setToolTip(QApplication::translate("QG_PolylineOptions", "Counter Clockwise", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbPos->setText(QString());
#ifndef QT_NO_TOOLTIP
        rbNeg->setToolTip(QApplication::translate("QG_PolylineOptions", "Clockwise", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbNeg->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_PolylineOptions: public Ui_QG_PolylineOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_POLYLINEOPTIONS_H
