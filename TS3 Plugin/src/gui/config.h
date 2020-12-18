#ifndef CONFIG_H
#define CONFIG_H

#include <qdialog.h>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class config; }
QT_END_NAMESPACE

class config : public QDialog
{
    Q_OBJECT

public:
    config(QWidget *parent = nullptr, void (*valSetCB)(std::string, unsigned long) = nullptr, std::string currPort = "COM1", quint64 currBaud = 115200);
    ~config();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_portTextBox_textEdited(const QString &arg1);

private:
    Ui::config *ui;
    void (*valSetCB)(std::string, unsigned long);
};
#endif // CONFIG_H
