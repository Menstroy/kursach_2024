#include "ClientChatWidget.h"
#include "ui_ClientChatWidget.h"

#include <QMessageBox>
#include <QDesktopServices>

ClientChatWidget::ClientChatWidget(QTcpSocket *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientChatWidget)
{
    ui->setupUi(this);
    _client = new ClientManager(client, this);
    connect(_client, &ClientManager::disconnected, this, &ClientChatWidget::clientDisconnected);
    connect(_client, &ClientManager::textMessageReceived, this, &ClientChatWidget::textMessageReceived);
    connect(_client, &ClientManager::isTyping, this, &ClientChatWidget::onTyping);
    connect(_client, &ClientManager::nameChanged, this, &ClientChatWidget::onClientNameChanged);
    connect(ui->lnMessage, &QLineEdit::textChanged, _client, &ClientManager::sendIsTyping);
}

void ClientChatWidget::disconnect()
{
    _client->disconnectFromHost();
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}

void ClientChatWidget::clientDisconnected()
{
    ui->wdgSendMessage->setEnabled(false);
}

void ClientChatWidget::logging(QString message)
{
    QFile fileOut("C:/Users/reime/Desktop/log.txt"); // Связываем объект с файлом fileout.txt
    if(fileOut.open(QIODevice::Append | QIODevice::Text))
    { // Если файл успешно открыт для записи в текстовом режиме
        QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream
        // и передаем ему адрес объекта fileOut
        QTime currentTime = QTime::currentTime();
        QString currentTimeString = currentTime.toString(" (hh:mm:ss)");
        writeStream << "\n server:" << message << currentTimeString  ;
        fileOut.close(); // Закрываем файл

    }
}

void ClientChatWidget::on_btnSend_clicked()
{
    auto message = ui->lnMessage->text().trimmed();
    _client->sendMessage(message);
    logging(message);
    ui->lnMessage->setText("");
    ui->lstMessages->addItem(message);
}

void ClientChatWidget::textMessageReceived(QString message, QString receiver)
{
    if (receiver == "Server" || receiver == "All") {
        ui->lstMessages->addItem(message);
    }
    if(receiver != "Server"){
        emit textForOtherClients(message, receiver, _client->name());
    }
}


void ClientChatWidget::on_lblOpenFolder_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_client->name()));
}

void ClientChatWidget::onClientNameChanged(QString prevName, QString name)
{
    emit clientNameChanged(prevName, name);
}

