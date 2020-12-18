/********************************************************************************
** Form generated from reading UI file 'config.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_config
{
public:
    QFrame *frame;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *portTextBox;
    QComboBox *baudSelectBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *config)
    {
        if (config->objectName().isEmpty())
            config->setObjectName(QString::fromUtf8("config"));
        config->resize(240, 160);
        config->setModal(false);
        frame = new QFrame(config);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 241, 161));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayoutWidget = new QWidget(frame);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(-1, -1, 241, 161));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(20, -1, 20, -1);
        portTextBox = new QLineEdit(verticalLayoutWidget);
        portTextBox->setObjectName(QString::fromUtf8("portTextBox"));

        gridLayout->addWidget(portTextBox, 0, 0, 1, 1);

        baudSelectBox = new QComboBox(verticalLayoutWidget);
        baudSelectBox->setObjectName(QString::fromUtf8("baudSelectBox"));

        gridLayout->addWidget(baudSelectBox, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(20, -1, 20, -1);
        okButton = new QPushButton(verticalLayoutWidget);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout->addWidget(okButton);

        cancelButton = new QPushButton(verticalLayoutWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(config);

        QMetaObject::connectSlotsByName(config);
    } // setupUi

    void retranslateUi(QDialog *config)
    {
        config->setWindowTitle(QApplication::translate("config", "TS3SerialDisplay Config", nullptr));
        okButton->setText(QApplication::translate("config", "Ok", nullptr));
        cancelButton->setText(QApplication::translate("config", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class config: public Ui_config {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_H
