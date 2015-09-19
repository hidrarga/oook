#include "configdialog.hpp"
#include "ui_config.h"

#include "config.hpp"
#include "utils.hpp"
#include "installer.hpp"

#include <QtGui/QApplication>
#include <QtGui/QWidget>

#include <boost/filesystem.hpp>

ConfigDialog::ConfigDialog(QString const& filename, QWidget *parent): 
    QDialog(parent),
    _ui(new Ui::ConfigDialog),
    _filename(filename),
    _parser()
{
    _ui->setupUi(this);
    
    show();
    
    readConfig();
}

ConfigDialog::~ConfigDialog()
{
    delete _ui;
}

void ConfigDialog::readConfig() {
    try {
        _parser.read(_filename.toStdString());
    } catch(std::exception const& e) { }
    
    if(Config::exists("calibre"))
        _ui->calibre->setText(Config::get("calibre").c_str());
    
    if(Config::exists("database"))
        _ui->database->setText(Config::get("database").c_str());
    
    if(Config::exists("catalog"))
        _ui->catalog->setText(Config::get("catalog").c_str());
    
    if(Config::exists("title"))
        _ui->title->setText(Config::get("title").c_str());
    else
        _ui->title->setText("Oook (Open Orang-outan Knowledge)");
    
    if(Config::exists("template"))
        _ui->templates->setText(Config::get("template").c_str());
    else
        _ui->templates->setText("../template/");
    
    if(Config::exists("items_per_page"))
        _ui->itemsPerPage->setValue(std::stoi(Config::get("items_per_page")));
    else
        _ui->itemsPerPage->setValue(20);
    
    if(Config::exists("thumbnail_size")) {
        std::vector<std::string> size = Utils::split('x', Config::get("thumbnail_size"));
        
        _ui->thumbnailL->setValue(std::stoi(size[0]));
        _ui->thumbnailH->setValue(std::stoi(size[1]));
    } else {
        _ui->thumbnailL->setValue(100);
        _ui->thumbnailH->setValue(144);
    }
    
    if(Config::exists("cover_size")) {
        std::vector<std::string> size = Utils::split('x', Config::get("cover_size"));
        
        _ui->coverL->setValue(std::stoi(size[0]));
        _ui->coverH->setValue(std::stoi(size[1]));
    } else {
        _ui->coverL->setValue(380);
        _ui->coverH->setValue(542);
    }
    
    if(Config::exists("copy")) {
        if(Config::get("copy") == "trivial")
            _ui->copyTrivial->setChecked(true);
        else if(Config::get("copy") == "normal")
            _ui->copyNormal->setChecked(true);
        else if(Config::get("copy") == "full")
            _ui->copyFull->setChecked(true);
    }
}

void ConfigDialog::accept() {
    Config::set("calibre", Utils::append(_ui->calibre->text().toStdString()));
    Config::set("database", _ui->database->text().toStdString());
    Config::set("catalog", Utils::append(_ui->catalog->text().toStdString()));
    Config::set("title", _ui->title->text().toStdString());
    Config::set("template", Utils::append(_ui->templates->text().toStdString()));
    Config::set("items_per_page", _ui->itemsPerPage->text().toStdString());
    Config::set("thumbnail_size", std::to_string(_ui->thumbnailL->value()) + "x" + std::to_string(_ui->thumbnailH->value()));
    Config::set("cover_size", std::to_string(_ui->coverL->value()) + "x" + std::to_string(_ui->coverH->value()));
    
    if(_ui->copyTrivial->isChecked())
        Config::set("copy", "trivial");
    else if(_ui->copyNormal->isChecked())
        Config::set("copy", "normal");
    else if(_ui->copyFull->isChecked())
        Config::set("copy", "full");
    
    try {
        validate();
        
        QString filename = QFileDialog::getSaveFileName(this, tr("Sauver la configuration"), QDir::currentPath() + "/" + _filename);
        if(!filename.isEmpty()) {
            _filename = filename;
            
            _parser.write(_filename.toStdString());
        
            Installer installer(this);
            
            QDialog::accept();
        }
    } catch(std::exception const& e) {
        QMessageBox().critical(this, tr("Erreur"), tr(e.what()));
    }
}

void ConfigDialog::validate() {
    if(_ui->calibre->text().isEmpty())
        throw std::runtime_error("Veuillez indiquer l'emplacement de la bibliothèque Calibre.");
    
    if(!boost::filesystem::is_directory(_ui->calibre->text().toStdString()))
        throw std::runtime_error("La bibliothèque Calibre n'est pas un dossier valide.");
    
    if(_ui->database->text().isEmpty())
        throw std::runtime_error("Veuillez indiquer l'emplacement de la base de données Calibre.");
    
    if(!boost::filesystem::is_regular_file(_ui->database->text().toStdString()))
        throw std::string("La base de données Calibre n'est pas un fichier valide.");
    
    if(_ui->catalog->text().isEmpty())
        throw std::runtime_error("Veuillez indiquer l'emplacement du catalogue de destination.");
    
    if(_ui->title->text().isEmpty())
        throw std::runtime_error("Veuillez spécifier un titre à votre bibliothèque.");
    
    if(_ui->templates->text().isEmpty())
        throw std::runtime_error("Veuillez indiquer l'emplacement des templates.");
    
    if(!boost::filesystem::is_directory(_ui->templates->text().toStdString()))
        throw std::runtime_error("Le dossier des templates n'existe pas.");
    
    if(_ui->itemsPerPage->value() <= 0)
        throw std::runtime_error("Le nombre d'éléments par page doit être strictement positif.");
}

void ConfigDialog::selectCalibre()
{
    QString currentDir = (_ui->calibre->text().isEmpty()) ? QDir::currentPath() : _ui->calibre->text();
    
    QString calibre = QFileDialog::getExistingDirectory(this, tr("Sélectionnez la bibliothèque Calibre"), currentDir);
    
    if(!calibre.isEmpty()) {
        _ui->calibre->setText(calibre);
        
        if(_ui->database->text().isEmpty()) {
            QFile database(calibre + "/metadata.db");
            
            if(database.exists())
                _ui->database->setText(database.fileName());
        }
    }
}

void ConfigDialog::selectDatabase()
{
    QString currentDir = (_ui->database->text().isEmpty()) ? QDir::currentPath() : _ui->database->text();
    
    QString database = QFileDialog::getOpenFileName(this, tr("Sélectionnez la base de données"), currentDir);
    
    if(!database.isEmpty())
        _ui->database->setText(database);
}

void ConfigDialog::selectCatalog()
{
    QString currentDir = (_ui->catalog->text().isEmpty()) ? QDir::currentPath() : _ui->catalog->text();
    
    QString catalog = QFileDialog::getExistingDirectory(this, tr("Sélectionnez le catalogue"), currentDir);
    
    if(!catalog.isEmpty())
        _ui->catalog->setText(catalog);
}

void ConfigDialog::selectTemplate()
{
    QString currentDir = (_ui->templates->text().isEmpty()) ? QDir::currentPath() : _ui->templates->text();
    
    QString templates = QFileDialog::getExistingDirectory(this, tr("Sélectionnez le dossier des templates"), currentDir);
    
    if(!templates.isEmpty())
        _ui->templates->setText(templates);
}


int main(int argc, char **argv) {
    QApplication app(argc, argv);
    
    QString filename = (argc == 1) ? "oook.conf" : argv[1];
    
    ConfigDialog dialog(filename);
        
    return app.exec();
}
