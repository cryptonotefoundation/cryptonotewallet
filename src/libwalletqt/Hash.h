#ifndef HASH
#define HASH

#include <QObject>
#include <QDebug>
namespace Monero {
    class Hash; // forward declaration
}
class Hash : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE void hash(const QString &data, const QString &password);
private:
};

#endif // HASH

