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
    explicit Haproxy(QObject *parent = 0);
    //static Haproxy void haproxy();
    Q_INVOKABLE void haproxy();

    //void haproxy();
};

#endif // HAPROXY

