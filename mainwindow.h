#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "core/style_loader.h"
#include "core/settings_loader.h"
#include "core/data/data_holder.h"

#include "core/update/update_manager.h"

namespace Ui {
class MainWindow;
}

class QSplashScreen;
class AbstractExtension;
class ExtensionBar;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QSplashScreen* splashScreen);
    ~MainWindow();

    void initialize();
    void reloadStyles();
    void loadSettings();
    StyleLoader *styleLoader();
    data::DataHolder* dataHolder();
    bool applicationUpdated() const;
    
protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
private slots:
    void on_actionAbout_Qt_triggered();
    
    void on_actionExit_triggered();
    void onExtensionChanged(AbstractExtension* extension);
    void on_action_Settings_triggered();
    
    void on_actionFull_Screen_triggered(bool checked);
    void restart();
    
    void on_actionAbout_triggered();
    
    void on_actionExtension_Bar_triggered(bool checked);
    
    void on_actionAbout_Extensions_triggered();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    
private:
    Ui::MainWindow *ui;
    QSplashScreen* m_splashScreen;
    QWidget* m_container;
    ExtensionBar* m_extensionBar;
    StyleLoader m_styleLoader;
    Qt::WindowStates m_previousWindowsState;
    data::DataHolder m_dataHolder;
    UpdateManager m_updateManager;
    SettingsDialog* m_settingsDialog;
    QSystemTrayIcon* m_trayIcon;
    
    void initializeDefaultExtension();
};

#endif // MAINWINDOW_H
