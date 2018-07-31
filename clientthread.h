#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QAbstractSocket>
#include <QTcpSocket>

class clientThread : public QThread
{
    Q_OBJECT

public:
    explicit clientThread(QString host, quint16 port);
    ~clientThread();

    void setPause(bool pause);
    void setStop();

    int sendBytes;
    qint64 blockNumber;
    qint64 maxBytes;

    void sendFile(const QString& file, const QHostAddress& dest);
    void sendFile(const QString& file);
    void sendEOF();

    QTcpSocket* getSocket()
    {
        return m_socket;
    }

protected:
    void run();

signals:
    void fileSize(qint64 size);
    void message(const QString& str);
    void onError(qint32);
    void SendFinish(const QString& file);

public slots:

    void displaySocketError(QAbstractSocket::SocketError e);
    void on_socket_readySend();

private:
    QString filePath;
    QTcpSocket  *m_socket;
    QString host;//ip地址
    quint16 port;//端口

    volatile bool m_Run;
    volatile bool m_Pause;

};

#endif // CLIENTTHREAD_H
