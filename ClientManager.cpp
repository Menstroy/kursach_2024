#include "ClientManager.h"

#include <QDir>

ClientManager::ClientManager(QHostAddress ip, ushort port, QObject *parent)
    : QObject{parent},
      _ip(ip),
      _port(port)
{
    _socket = new QTcpSocket(this);
    setupClient();
}

ClientManager::ClientManager(QTcpSocket *client, QObject *parent)
    : QObject{parent},
    _socket(client)
{
    setupClient();
}

void ClientManager::connectToServer()
{
    _socket->connectToHost(_ip, _port);
}

void ClientManager::disconnectFromHost()
{
    _socket->disconnectFromHost();
}

void ClientManager::sendMessage(QString message)
{
    _socket->write(_protocol.textMessage(message, name()));
}

void ClientManager::sendName(QString name)
{
    _socket->write(_protocol.setNameMessage(name));
}

void ClientManager::sendStatus(ChatProtocol::Status status)
{
    _socket->write(_protocol.setStatusMessage(status));
}

QString ClientManager::name() const
{
    auto id = _socket->property("id").toInt();
    auto name = _protocol.name().length() > 0 ? _protocol.name() : QString("Клиент (%1)").arg(id);

    return name;
}

void ClientManager::sendIsTyping()
{
    _socket->write(_protocol.isTypingMessage());
}


void ClientManager::setupClient()
{
    connect(_socket, &QTcpSocket::connected, this, &ClientManager::connected);
    connect(_socket, &QTcpSocket::disconnected, this, &ClientManager::disconnected);
}



