#ifndef __CONFIGDIALOG_H__
#define __CONFIGDIALOG_H__

#include <QtCore>
#include <QtWidgets>

#include <QtGui/QDialog>

#include "configparser.hpp"

class QWidget;

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog {
    Q_OBJECT
    
public:
    ConfigDialog(QString const& filename, QWidget* parent = 0);
    virtual ~ConfigDialog();
    
    void readConfig();
    void validate();
    
public slots:
    void accept() override;
    void selectCalibre();
    void selectDatabase();
    void selectCatalog();
    void selectTemplate();
    
private:
    Ui::ConfigDialog *_ui;
    QString _filename;
    ConfigParser _parser;
};

#endif