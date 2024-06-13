#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ChatProtocol.h"

#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QHostAddress ip = QHostAddress::LocalHost, ushort port = 4500, QObject *parent = nullptr);
    explicit ClientManager(QTcpSocket *client, QObject *parent = nullptr);

    void connectToServer();
    void disconnectFromHost();

    void sendMessage(QString message);
    void sendName(QString name);
    QString name() const;
    void sendIsTyping();




signals:
    void connected();
    void disconnected();
    void textMessageReceived(const QString message, QString receiver);
    void isTyping();
    void nameChanged(QString prevName, QString name);
    void statusChanged(ChatProtocol::Status status);
private slots:

private: //fields
    QTcpSocket *_socket;
    QHostAddress _ip;
    ushort _port;
    ChatProtocol _protocol;

private: //methods
     void setupClient();

};

#endif // CLIENTMANAGER_H