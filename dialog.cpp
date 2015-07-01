#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QUdpSocket>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);


    qDebug() <<  QDir::homePath();


    m_selectedDir = QDir::currentPath();

    // create a QUDP socket for clent
    m_socket = new QUdpSocket(this);
    m_socket->bind(QHostAddress::LocalHost, 3334);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyUdpRead()));
    connect(this, SIGNAL(setRange(int,int)), ui->progressBar, SLOT(setRange(int,int)));
    connect(this, SIGNAL(setValue(int)), ui->progressBar, SLOT(setValue(int)));
    //sendDatagram();

    qDebug() << "Udp Client Started";

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_DirSelect_clicked()
{
    m_selectedDir = QFileDialog::getExistingDirectory(
                this, "Select Dir", m_selectedDir);
    ui->ScanPath->setText(m_selectedDir);
}

void Dialog::on_FileSelect_clicked()
{
    m_malwareFile = QFileDialog::getOpenFileName(
                this, "Select Signanures File", QDir::homePath());
    ui->FilePath->setText(m_malwareFile);

}

void Dialog::on_StartScan_clicked()
{
    if(m_selectedDir.isEmpty()) return;
    if(m_malwareFile.isEmpty()) return;
    ui->reportList->clear();
    //m_listInfectedFiles.clear();
    //ui->listView->reset();

    sendDatagram();

}


void Dialog::readyUdpRead()
{
    // when data comes in

    int min, max, value;

    QByteArray buffer;
    buffer.resize(m_socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort, op;

    m_socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

    QDataStream stream( &buffer, QIODevice::ReadWrite );

    op = 0;
    stream >> op;

    switch(op)
    {
    case 1:
        stream >> m_infectedFile >> m_guid;
        ui->reportList->addItem(m_infectedFile + ":  " + m_guid);
        break;
    case 2:
        stream >> min >> max >> value;
        emit setRange(min,max);
        emit setValue(value);
        break;
    }
}

void Dialog::sendDatagram()
{
    qint16 op = 0;
    QByteArray buffer;
    QDataStream stream( &buffer, QIODevice::ReadWrite );

    stream << op << m_selectedDir << m_malwareFile;

    m_socket->writeDatagram(buffer, QHostAddress::LocalHost, 3333);
}

