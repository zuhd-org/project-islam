
#include "core/extension/extension_bar.h"
#include <QVector>
#include "core/logging.h"
#include "core/constants.h"
#include "core/extension/extension.h"
#include "core/extension/extension_item.h"

ExtensionBar::ExtensionBar(QWidget *parent, QWidget* container) :
    QToolBar(parent),
    m_mainWindow(parent),
    m_container(container),
    m_currentExtension(nullptr)
{
    _TRACE;
    setMinimumWidth(kExtensionBarWidth);
    setMaximumWidth(kExtensionBarWidth);
    setFloatable(false);
    setOrientation(Qt::Vertical);
    setMovable(false);
}

ExtensionBar::~ExtensionBar()
{
    _TRACE;
    for (int i = m_extensionItems.size() - 1; i >= 0; --i) {
        delete m_extensionItems.at(i);
        m_extensionItems.remove(i);
    }
    for (int i = m_extensions.size() - 1; i >= 0; --i) {
        delete m_extensions.at(i);
        m_extensions.remove(i);
    }
    CHECK(m_extensionItems.empty()) << "Unable to retain memory from the extensions items. "
                                << "Memory leak from [" << m_extensionItems.size() << "] extensions items.";
    for (int i = m_extensions.size() - 1; i >= 0; --i) {
        delete m_extensions.at(i);
        m_extensions.remove(i);
    }
    CHECK(m_extensions.empty()) << "Unable to retain memory from the extensions. "
                                << "Memory leak from [" << m_extensions.size() << "] extensions.";
}

void ExtensionBar::addExtension(Extension* extension)
{
    if (extension->isDefault()) {
        // If we already have default, remove it
        for (ExtensionItem* extensionItem : m_extensionItems) {
            extensionItem->extension()->setIsDefault(false);
        }
    } else {
        // We use first extension as default
        if (m_extensionItems.isEmpty()) {
            extension->setIsDefault(true);
        }
    }
    m_extensions.push_back(extension);
    ExtensionItem* extensionItem = new ExtensionItem(this, extension, &m_extensionItems);
    extensionItem->setText(extension->name());
    extensionItem->show();
    m_extensionItems.push_back(extensionItem);
    addWidget(extensionItem);
}

ExtensionItem* ExtensionBar::defaultExtensionItem() const
{
    for (ExtensionItem* extensionItem : m_extensionItems) {
        if (extensionItem->extension()->isDefault()) {
            return extensionItem;
        }
    }
    return nullptr;
}

void ExtensionBar::setCurrentExtension(Extension *currentExtension)
{
    m_currentExtension = currentExtension;
    emit extensionChanged(currentExtension);
}

Extension *ExtensionBar::currentExtension() const
{
    return m_currentExtension;
}

QWidget *ExtensionBar::container() const
{
    return m_container;
}

const QVector<ExtensionItem*>* ExtensionBar::extensionItems() const
{
    return &m_extensionItems;
}

const QVector<Extension*>* ExtensionBar::extensions() const
{
    return &m_extensions;
}