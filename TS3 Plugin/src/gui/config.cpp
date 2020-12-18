#include "config.h"
#include "ui_config.h"

config::config(QWidget *parent, void(*valSetCB)(std::string port, unsigned long baud), std::string currPort, quint64 currBaud)
    : QDialog(parent), ui(new Ui::config), valSetCB(valSetCB)
{
    static quint64 baudRates[12] { 9600, 19200, 28800, 38400, 57600, 76800, 115200, 230400, 460800, 576000, 921600, 1000000 };

    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    for (int i = 0; i < 12; i++) {
        ui->baudSelectBox->addItem(std::to_string(baudRates[i]).c_str(), QVariant(baudRates[i]));
        if(baudRates[i] == currBaud) {
            ui->baudSelectBox->setCurrentIndex(i);
        }
    }

    ui->portTextBox->setText(currPort.c_str());
}

config::~config()
{
    delete ui;
}

void config::on_portTextBox_textEdited(const QString &arg1)
{
#if defined (WIN32) || defined (WIN64)
    bool textValid = arg1.size() > 3 && arg1.startsWith("COM");
#else
    bool textValid = arg1.size() > 8 && arg1.startsWith("/dev/tty");
#endif
    ui->okButton->setEnabled(textValid);
}

void config::on_okButton_clicked()
{
    std::string portName = ui->portTextBox->text().toStdString();
    unsigned long baudRate = ui->baudSelectBox->currentData().toULongLong();
    close();
    valSetCB(portName, baudRate);
}

void config::on_cancelButton_clicked()
{
    close();
    valSetCB("", 0);
}
