#ifndef LOGGING_H
#define LOGGING_H

#include <easylogging++.h>
#include "core/constants.h"

static void configureLoggers() {

    el::Logger* dataLogger = el::Loggers::getLogger("data");
    el::Loggers::getLogger("quran");
    el::Loggers::getLogger("locationTrace");
    /*
     // Having issue with path
     QString logConfigFilename;
#if _ELPP_OS_UNIX
    logConfigFilename = QApplication::applicationDirPath() + QDir::separator() + "configs/logging.linux.config";
#elif _ELPP_OS_WINDOWS
    logConfigFilename = ":/logging_config/win";
#endif

    el::Loggers::configureFromGlobal(logConfigFilename.toStdString().c_str());
    */

    el::Configurations* configurations = el::Loggers::getLogger("default")->configurations();
    configurations->set(el::Level::Global, el::ConfigurationType::Filename, 
        std::string(kHomeDir) + "logs" + QString(QDir::separator()).toStdString() + "project-islam.log");
    el::Loggers::setDefaultConfigurations(*configurations, true);
    
    dataLogger->configurations()->set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
    dataLogger->reconfigure();
    
    el::Logger* defaultLogger = el::Loggers::getLogger("default");
    defaultLogger->configurations()->set(el::Level::Trace, el::ConfigurationType::Format, "%datetime %level %func");
    defaultLogger->configurations()->set(el::Level::Debug, el::ConfigurationType::Format, "%datetime %level [%func] %msg");
    defaultLogger->reconfigure();
}

#define TRACE_LOCATION CLOG(TRACE, "locationTrace")

#define DATA_LOG(LEVEL) CLOG(LEVEL, "data")
#define DDATA_LOG(LEVEL) DCLOG(LEVEL, "data")

#define QURAN_LOG(LEVEL) CLOG(LEVEL, "quran")

#define _TRACE LOG(TRACE)

#endif // LOGGING_H