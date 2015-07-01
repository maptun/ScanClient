#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QAbstractSocket>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QModelIndex>
#include <QDir>

class QUdpSocket;



namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void setRange(int,int);
    void setValue(int);

public slots:
    void readyUdpRead();

private slots:
    void on_DirSelect_clicked();
    void on_FileSelect_clicked();
    void on_StartScan_clicked();

private:
    Ui::Dialog *ui;

    QUdpSocket *m_socket;

    void sendDatagram();

    QString m_selectedDir, m_malwareFile, m_infectedFile, m_guid;
    QStringList m_listInfectedFiles;
};

#endif // DIALOG_H
