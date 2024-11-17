#include "rs_pythongui.h"
#include <QMessageBox>

RS_PythonGui::RS_PythonGui()
{
}

RS_PythonGui::~RS_PythonGui()
{
}

void RS_PythonGui::MessageBox(const char *msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("LibreCAD");
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void RS_PythonGui::HalloWelt()
{
    qDebug() << "RS_PythonGui::HalloWelt: Hallo, Welt";
}
