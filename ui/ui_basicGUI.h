/********************************************************************************
** Form generated from reading UI file 'basicGUI.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BASICGUIRXKEBL_H
#define BASICGUIRXKEBL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *original_file_gbox;
    QPushButton *original_file_browse_btn;
    QLabel *original_file_label;
    QGroupBox *texture_folder_gbox;
    QPushButton *texture_folder_browse_btn;
    QPushButton *dump_textures_btn;
    QLabel *texture_folder_label;
    QLabel *label_3;
    QGroupBox *change_file_gbox;
    QPushButton *change_file_browse_btn;
    QLabel *edit_file_label;
    QGroupBox *ouput_folder_gbox;
    QPushButton *output_folder_browse_btn;
    QPushButton *apply_edits_btn;
    QLabel *output_folder_label;
    QLabel *notice_label;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(550, 410);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(550, 400));
        MainWindow->setMaximumSize(QSize(550, 410));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        original_file_gbox = new QGroupBox(centralwidget);
        original_file_gbox->setObjectName(QString::fromUtf8("original_file_gbox"));
        original_file_gbox->setGeometry(QRect(10, 10, 531, 61));
        original_file_browse_btn = new QPushButton(original_file_gbox);
        original_file_browse_btn->setObjectName(QString::fromUtf8("original_file_browse_btn"));
        original_file_browse_btn->setGeometry(QRect(10, 20, 101, 31));
        original_file_label = new QLabel(original_file_gbox);
        original_file_label->setObjectName(QString::fromUtf8("original_file_label"));
        original_file_label->setGeometry(QRect(120, 20, 401, 31));
        texture_folder_gbox = new QGroupBox(centralwidget);
        texture_folder_gbox->setObjectName(QString::fromUtf8("texture_folder_gbox"));
        texture_folder_gbox->setGeometry(QRect(10, 80, 531, 91));
        texture_folder_browse_btn = new QPushButton(texture_folder_gbox);
        texture_folder_browse_btn->setObjectName(QString::fromUtf8("texture_folder_browse_btn"));
        texture_folder_browse_btn->setGeometry(QRect(10, 20, 101, 31));
        dump_textures_btn = new QPushButton(texture_folder_gbox);
        dump_textures_btn->setObjectName(QString::fromUtf8("dump_textures_btn"));
        dump_textures_btn->setGeometry(QRect(10, 50, 101, 31));
        texture_folder_label = new QLabel(texture_folder_gbox);
        texture_folder_label->setObjectName(QString::fromUtf8("texture_folder_label"));
        texture_folder_label->setGeometry(QRect(120, 20, 401, 31));
        label_3 = new QLabel(texture_folder_gbox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(240, 90, 401, 31));
        change_file_gbox = new QGroupBox(centralwidget);
        change_file_gbox->setObjectName(QString::fromUtf8("change_file_gbox"));
        change_file_gbox->setGeometry(QRect(10, 180, 531, 61));
        change_file_browse_btn = new QPushButton(change_file_gbox);
        change_file_browse_btn->setObjectName(QString::fromUtf8("change_file_browse_btn"));
        change_file_browse_btn->setGeometry(QRect(10, 20, 101, 31));
        edit_file_label = new QLabel(change_file_gbox);
        edit_file_label->setObjectName(QString::fromUtf8("edit_file_label"));
        edit_file_label->setGeometry(QRect(120, 20, 401, 31));
        ouput_folder_gbox = new QGroupBox(centralwidget);
        ouput_folder_gbox->setObjectName(QString::fromUtf8("ouput_folder_gbox"));
        ouput_folder_gbox->setGeometry(QRect(10, 250, 531, 91));
        output_folder_browse_btn = new QPushButton(ouput_folder_gbox);
        output_folder_browse_btn->setObjectName(QString::fromUtf8("output_folder_browse_btn"));
        output_folder_browse_btn->setGeometry(QRect(10, 20, 101, 31));
        apply_edits_btn = new QPushButton(ouput_folder_gbox);
        apply_edits_btn->setObjectName(QString::fromUtf8("apply_edits_btn"));
        apply_edits_btn->setGeometry(QRect(10, 50, 101, 31));
        output_folder_label = new QLabel(ouput_folder_gbox);
        output_folder_label->setObjectName(QString::fromUtf8("output_folder_label"));
        output_folder_label->setGeometry(QRect(120, 20, 401, 31));
        notice_label = new QLabel(centralwidget);
        notice_label->setObjectName(QString::fromUtf8("notice_label"));
        notice_label->setGeometry(QRect(20, 350, 511, 41));
        notice_label->setWordWrap(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 550, 22));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "JPAC2 Editor", nullptr));
        original_file_gbox->setTitle(QCoreApplication::translate("MainWindow", "Original Particle File (.jpc)", nullptr));
        original_file_browse_btn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        original_file_label->setText(QCoreApplication::translate("MainWindow", "Choose the original particle file", nullptr));
        texture_folder_gbox->setTitle(QCoreApplication::translate("MainWindow", "Texture Folder", nullptr));
        texture_folder_browse_btn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        dump_textures_btn->setText(QCoreApplication::translate("MainWindow", "Dump Textures", nullptr));
        texture_folder_label->setText(QCoreApplication::translate("MainWindow", "Choose the texture folder for replacements/dump destination", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Choose the texture folder for replacements/dump destination", nullptr));
        change_file_gbox->setTitle(QCoreApplication::translate("MainWindow", "Change File (.json)", nullptr));
        change_file_browse_btn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        edit_file_label->setText(QCoreApplication::translate("MainWindow", "Choose the json file for edits to the particle file", nullptr));
        ouput_folder_gbox->setTitle(QCoreApplication::translate("MainWindow", "Output Folder", nullptr));
        output_folder_browse_btn->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        apply_edits_btn->setText(QCoreApplication::translate("MainWindow", "Apply Edits", nullptr));
        output_folder_label->setText(QCoreApplication::translate("MainWindow", "Output folder. File will be named the same as the input file.", nullptr));
        notice_label->setText(QCoreApplication::translate("MainWindow", "This tool only support JPAC2-10 and JPAC2-11 Files. See template doc for details and readme for directions.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BASICGUIRXKEBL_H
