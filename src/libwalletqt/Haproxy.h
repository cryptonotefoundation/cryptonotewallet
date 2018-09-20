#ifndef HAPROXY
#define HAPROXY

#include <QObject>
#include <QDebug>
namespace Monero {
    class Haproxy; // forward declaration
}
class Haproxy : public QObject
{
    Q_OBJECT
public:
    //static Haproxy * haproxy();
    Q_INVOKABLE void haproxy(const QString &host, const QString &ip, const QString &port, const QString &endpoint, const QString &endpointport, const QString &fixedHost, const QString &auth, const QString &provider, const QString &plan, const QString &serviceName);
    Q_INVOKABLE void haproxyCert(const QString &host, const QString &certificate);
    Q_INVOKABLE void killHAproxy();

    // returns true if proxy is online and accepting connections, false otherwise
    Q_INVOKABLE bool verifyHaproxy(const QString &host, const QString &ip, const QString &provider);


private:
};

#endif // HAPROXY

