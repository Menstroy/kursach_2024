#ifndef CHATPROTOCOL_H
#define CHATPROTOCOL_H

#include <QByteArray>
#include <QString>



class ChatProtocol
{
public:
    enum MessageType{
        Text,
        IsTyping,
        SetName,
        SetStatus,
        InitSendingFile,
        AcceptSendingFile,
        RejectSendingFile,
        SendFile,
        ClientName,
        ConnectionACK,
        NewClient,
        ClientDisconnected
    };

    enum Status{
        None,
        Available,
        Away,
        Busy
    };

    ChatProtocol();

    QByteArray textMessage(QString message, QString receiver);
    QByteArray isTypingMessage();
    QByteArray setNameMessage(QString name);
    QByteArray setStatusMessage(Status status);
    QByteArray setInitSendingFileMessage(QString fileName);
    QByteArray setAcceptFileMessage();
    QByteArray setRejectFileMessage();
    QByteArray setFileMessage(QString fileName);

    QByteArray setClientNameMessage(QString prevName, QString name);
    QByteArray setConnectionACKMessage(QString clientName, QStringList otherClients);
    QByteArray setNewClientMessage(QString clientName);
    QByteArray setClinetDisconnectedMessage(QString clientName);

    void loadData(QByteArray data);

    const QString &message() const;

    const QString &name() const;

    MessageType type() const;

    const QString &receiver() const;

private:
    QByteArray getData(MessageType type, QString data);

    MessageType _type;
    QString _message;
    QString _name;
    QString _receiver;

};

#endif // CHATPROTOCOL_H
