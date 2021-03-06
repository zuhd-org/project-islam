#ifndef EXTENSION_BASE_H
#define EXTENSION_BASE_H

#include <QtPlugin>
#include <QObject>
#include <QMenu>

#include "core/logging/logging.h"
#include "core/utils/memory.h"
#include "core/settings_loader.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension_info.h"
#include "core/utils/notify.h"
    
class ExtensionBase : public QObject
{
    // No Q_OBJECT here. Otherwise we get undefined AbstractExtension::staticMetaObject error
public:
    
    ExtensionBase() :
        m_extension(nullptr) {
        // Do not call initialize() here since we dont want to run into FATAL error
        // of "NOT" initializing extensionInfo at this point
    }
    
    virtual ~ExtensionBase() {
        // No need to delete m_extension as plugin loader
        // i.e, ExtensionLoader deletes this anyway in fact doing this
        // will cause segmentation fault
    }
    
    inline AbstractExtension *extension() {
        if (m_extension == nullptr) {
            m_extension = new AbstractExtension(0, &m_extensionInfo);
        }
        return m_extension;
    }
    
    inline QWidget* container() {
        return extension()->container();
    }
    
    inline data::DataHolder* data() {
        return extension()->dataHolder();
    }
    
    inline QWidget* settingsTabWidget() {
        return m_extension->newOrExistingSettingsTabWidget();
    }
    
    void notifyInfo(const QString& title, const QString& message, int durationMs = 3000) {
        ::notifyInfo(extension()->trayIcon(), title, message, durationMs);
    }
    
    void notifyWarn(const QString& title, const QString& message, int durationMs = 3000) {
        ::notifyWarn(extension()->trayIcon(), title, message, durationMs);
    }
    
    void notifyError(const QString& title, const QString& message, int durationMs = 3000) {
        ::notifyError(extension()->trayIcon(), title, message, durationMs);
    }
    
    void setLoggingRepository(el::base::type::StoragePointer repo) {
        m_storage = repo;
        el::Helpers::setStorage(m_storage);
    }
    
    /// @brief Need to call this in extension and only proceed if this returns true
    /// Returns true if successfully initialized
    virtual bool initialize(int argc, const char** argv) {
        el::Helpers::setStorage(m_storage);
        el::Helpers::setArgs(argc, argv);
#ifdef _LOGGER
        el::Loggers::getLogger(_LOGGER);
#endif // _LOGGER
#ifdef _PERFORMANCE_LOGGER
        el::Logger* extensionPerformanceLogger = el::Loggers::getLogger(_PERFORMANCE_LOGGER);
        LoggingConfigurer::reconfigurePerformanceLogger(extensionPerformanceLogger);
#endif // _PERFORMANCE_LOGGER
        setExtensionInfo(ExtensionInfo(getMajorVersion(), getMinorVersion(), getPatchVersion(),
                                   QString(getAuthor()), QString(getName()), 
                                   QString(getTitle()), QString(getDescription())));
        CHECK(m_extensionInfo.isInitialized()) << "Please initialize ExtensionInfo (using constructor) from constructor of your extension.";
        QObject::connect(extension(), SIGNAL(containerGeometryChanged(int, int)), this, SLOT(onContainerGeometryChanged(int, int)));
        QObject::connect(extension(), SIGNAL(activated()), this, SLOT(onActivated()));
        QObject::connect(extension(), SIGNAL(deactivated()), this, SLOT(onDeactivated()));
        return true;
    }
    
    inline void setExtensionInfo(const ExtensionInfo& extensionInfo) {
        m_extensionInfo = extensionInfo;
    }
    
    inline const ExtensionInfo& extensionInfo() const {
        return m_extensionInfo;
    }
    
    inline QMenu* extensionMenu() {
        return extension()->menu();
    }
    
    inline QString settingsKeyPrefix() {
        return "extension_setting__" + m_extensionInfo.name().replace(" ", "") 
                + "__";
    }
    
    inline QString settingKey(const QString& key) {
        return settingsKeyPrefix() + key;
    }
    
    /// @brief Returns value of extension-specific setting
    inline QVariant setting(const QString& key, const QVariant& defaultValue = QVariant()) {
        return nativeSetting(settingsKeyPrefix() + key, defaultValue);
    }
    
    /// @brief Returns value of native setting
    inline QVariant nativeSetting(const QString& key, const QVariant& defaultValue = QVariant()) {
        return extension()->settingsLoader()->get(key, defaultValue);
    }
    
    inline void saveSetting(const QString &key, const QVariant &value) {
        extension()->settingsLoader()->saveSettings(settingKey(key), value);
    }
    
    virtual inline unsigned int getMajorVersion() = 0;
    virtual inline unsigned int getMinorVersion() = 0;
    virtual inline unsigned int getPatchVersion() = 0;
    virtual inline const char* getAuthor() = 0;
    virtual inline const char* getTitle() = 0;
    virtual inline const char* getName() = 0;
    virtual inline const char* getDescription() = 0;
    
    
public:
    /// @brief Slot that is connected to signal that is emitted when container
    /// geometry changes. This is pretty much similar to QWidget::resizeEvent().
    virtual void onContainerGeometryChanged(int width, int height) = 0;
    
    /// @brief Slot that is connected to signal emmited when extension is activated
    virtual void onActivated() = 0;
    
    /// @brief Slot that is connected to signal emmited when extension is deactivated
    virtual void onDeactivated() = 0;
private:
    AbstractExtension* m_extension;
    ExtensionInfo m_extensionInfo;
    // Saving it locally is required for some compilers
    el::base::type::StoragePointer m_storage;
};

Q_DECLARE_INTERFACE(ExtensionBase, "ProjectIslam.Api.ExtensionBase.v1.0")
#define INITIALIZE_SHARED_LOGGING INITIALIZE_NULL_EASYLOGGINGPP

#endif // EXTENSION_BASE_H
