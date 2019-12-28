#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "Settings.h"
#include "TranslatorManager.h"

using namespace WalletGui;

TranslatorManager* TranslatorManager::m_Instance = 0;

TranslatorManager::TranslatorManager()
{
    QString lang = Settings::instance().getLanguage();
    if(lang.isEmpty()) {
        lang = QLocale::system().name();
        lang.truncate(lang.lastIndexOf('_'));
    }

#if defined(_MSC_VER)
  m_langPath = QApplication::applicationDirPath();
  m_langPath.append("/languages");
#elif defined(Q_OS_MAC)
  m_langPath = QApplication::applicationDirPath();
  m_langPath = m_langPath + "/../Resources/languages/";
#elif defined(__FreeBSD__)
  m_langPath = "/usr/local/share/karbo/karbowallet/languages";
#else
  m_langPath = "/opt/karbo/languages";
#endif

    QDir dir(m_langPath);
    QStringList resources = dir.entryList(QStringList("??.qm"));
    for (int j = 0; j < resources.size(); j++)
    {
        QString locale = resources[j];
        locale.truncate(locale.lastIndexOf('.'));
        if (locale == lang)
        {
            QTranslator* pTranslator = new QTranslator;
            if (pTranslator->load(resources[j], m_langPath))
            {
                qApp->installTranslator(pTranslator);
                m_keyLang = locale;
                m_translators.insert(locale, pTranslator);
                break;
            }
        }
    }
    QString langPathSys;
#if !defined(_MSC_VER) && !defined(Q_OS_MAC)
#if defined(__FreeBSD__)
    langPathSys = "/usr/local/share/qt5/translations";
#else
    langPathSys = "/usr/share/qt5/translations";
#endif
    QDir dirSys(langPathSys);
    QStringList Listfilter;
    Listfilter << "qt_??.qm" << "qtbase_??.qm" << "qtscript_??.qm" << "qtscript_??.qm";
    Listfilter << "qtquick1_??.qm" << "qtmultimedia_??.qm" << "qtxmlpatterns_??.qm";
    QStringList resourcesQt = dirSys.entryList(Listfilter);
#else
    langPathSys = m_langPath;
    QDir dirSys(langPathSys);
    QStringList resourcesQt = dirSys.entryList(QStringList("qt_??.qm"));
#endif
    for (int j = 0; j < resourcesQt.size(); j++)
    {
        const int langLen = 2;
        QString locale = resourcesQt[j];
        locale.truncate(locale.lastIndexOf('.'));
        QString l = locale;
        l.remove(0, l.length() - langLen);
        if (l == lang)
        {
            QTranslator* qTranslator = new QTranslator;
            if (qTranslator->load(resourcesQt[j], langPathSys))
            {
                qApp->installTranslator(qTranslator);
                m_keyLang = locale;
                m_translators.insert(locale, qTranslator);
            }
        }
    }
}

TranslatorManager::~TranslatorManager()
{
    TranslatorMap::const_iterator i = m_translators.begin();
    while (i != m_translators.end())
    {
        QTranslator* pTranslator = i.value();
        delete pTranslator;
        ++i;
    }

    m_translators.clear();
}

TranslatorManager* TranslatorManager::instance()
{
    static QMutex mutex;
    if (!m_Instance)
    {
        mutex.lock();

        if (!m_Instance)
            m_Instance = new TranslatorManager;

        mutex.unlock();
    }

    return m_Instance;
}

void TranslatorManager::switchTranslator(QTranslator& translator, const QString& filename)
{
  // remove the old translator
  qApp->removeTranslator(&translator);

  // load the new translator
  if(translator.load(filename))
   qApp->installTranslator(&translator);
}
