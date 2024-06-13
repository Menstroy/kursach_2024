#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    seupServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event); // Вызываем реализацию по умолчанию
    MainWindow::logging("Сервер отключен!");

}
void MainWindow::newClientConnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->lstClients->addItem(QString("Клиент подключился: %1").arg(id));
    MainWindow::logging(QString("Клиент подключился: %1").arg(id));
    auto chatWidget= new ClientChatWidget(client, ui->tbClientsChat);
    ui->tbClientsChat->addTab(chatWidget, QString("Client (%1)").arg(id));

    connect(chatWidget, &ClientChatWidget::clientNameChanged, this, &MainWindow::setClientName);
    connect(chatWidget, &ClientChatWidget::isTyping, [this](QString name){
        this->statusBar()->showMessage(name, 750);
    });

        connect(chatWidget, &ClientChatWidget::textForOtherClients, _server, &ServerManager::onTextForOtherClients);
}

void MainWindow::clientDisconnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->lstClients->addItem(QString("Клиент отключился: %1").arg(id));

    MainWindow::logging(QString("Клиент отключился: %1").arg(id));
}

void MainWindow::setClientName(QString prevName, QString name)
{
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tbClientsChat->indexOf(widget);
    ui->tbClientsChat->setTabText(index, name);

    _server->notifyOtherClients(prevName, name);


}

void MainWindow::logging(QString message)
{
    QFile fileOut("C:/Users/reime/Desktop/log.txt"); // Связываем объект с файлом fileout.txt
    if(fileOut.open(QIODevice::Append | QIODevice::Text))
    { // Если файл успешно открыт для записи в текстовом режиме
        QTime currentTime = QTime::currentTime();
        QString currentTimeString = currentTime.toString(" (hh:mm:ss)");
        QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream
        writeStream << "\n" << message << currentTimeString  ;
        fileOut.close(); // Закрываем файл
    }
}
void MainWindow::seupServer()
{
    _server = new ServerManager();
    connect(_server, &ServerManager::newClientConnected, this, &MainWindow::newClientConnected);
    connect(_server, &ServerManager::clientDisconnected, this, &MainWindow::clientDisconnected);

}


void MainWindow::on_tbClientsChat_tabCloseRequested(int index)
{
    auto chatWidget = qobject_cast<ClientChatWidget *>(ui->tbClientsChat->widget(index));
    chatWidget->disconnect();
    ui->tbClientsChat->removeTab(index);
}



