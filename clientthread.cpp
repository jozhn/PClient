#include "clientthread.h"
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QHostAddress>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <iostream>
#include <QTime>


clientThread::clientThread(QString host, quint16 port)
{

    m_Run = true;
    m_Pause = false;
    m_socket = new QTcpSocket;
    this->host=host;
    this->port=port;
}

clientThread::~clientThread()
{
    m_socket->disconnectFromHost();
    if (m_socket->state() == QAbstractSocket::UnconnectedState || m_socket->waitForDisconnected(1000))
    {
        qDebug("Disconnected!");
        delete m_socket;
    }
}


void clientThread::setPause(bool pause)
{
    m_Pause = pause;
}


void clientThread::setStop()
{
    m_Run = false;
}


// 函数重载
void clientThread::sendFile(const QString &file, const QHostAddress &dest)
{
    filePath = file;
   // m_socket->connectToHost(dest, 5234);
    qDebug() << "Send file " << file;
}

void clientThread::sendFile(const QString &file)
{
    filePath = file;
    //m_socket->connectToHost(dest, 5234);
    qDebug() << "Send file " << file;
}


void clientThread::run()
{
     m_socket->connectToHost(host, port);
    //qDebug() << "Connected";
    //QFile file("文件名或者带文件名的路径")：打开一个文件的方式有2种
    //一种是在构造函数中指定文件名；
    //第二种是使用setFileName（）函数设置文件名
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        //发送错误信号
        //emit onError(2);

        //断开与服务器的连接
        m_socket->disconnectFromHost();
        return;
    }
    else
        //发送文件大小  filesize（）
        emit fileSize(file.size());
    //发送信息，开始发送
    emit message(tr(" Start send!"));

    QString fileName = file.fileName();
    fileName = fileName.remove(0, fileName.lastIndexOf('/')+1);

    qint64 size;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //设置发送的数据流的qt版本
    out.setVersion(QDataStream::Qt_5_3);

    out << qint16(0x0001) << fileName.toUtf8();
    size = block.size();

    m_socket->write((char*)&size, sizeof(qint64));
    m_socket->write(block.data(), size);

    /*用Qt的TCP的write之后，接
     着调用waitForBytesWritten，
    因为Qt采用的是异步方式，write不会阻塞，是直接返回的，
    数据并没有真正立即从网口发出去，好像是写到了本地缓冲区了；
    所以要调用waitForBytesWritten促使数据发出去，然后产生相应的信号
    只有使用waitForBytesWritten（）后，QTcpSocket才真真的发送数据*/

    if(!m_socket->waitForBytesWritten(-1))
    {
        //qDebug() << "writen Bytes error " << m_socket->errorString();
        //抛出异常
        throw 2;
    }
    //如果你到了文件末尾，atEed()返回真。
    while(!file.atEnd() )
    {
        if  (!m_Pause)
        {
            //写入一个qint16大小（也就是2个字节）的文件名标志字和文件名。
                //然后使用out << qint16(0x0002) << file.read(0xFFF0)写入文件内容标志字
            //和一个QByteArray的文件内容，每次循环都先使用block.clear（）清空block里的内容，
            //然后再使用out.device()->seek(0)返回到block的开始
            block.clear();
            out.device()->seek(0);
            out << qint16(0x0002) << file.read(0xFFF0);
            size = block.size();
            //qDebug() << "Send Data " << size << " byte.";
            qDebug()<<"file send";
            m_socket->write((char*)&size, sizeof(qint64));
            m_socket->write(block.data(), size);
            if(!m_socket->waitForBytesWritten(-1))
            {
                //qDebug() << "writen Bytes error " << m_socket->errorString();
                throw 2;
            }
        }
        else
        {
            qDebug()<<"pause";
        }
    }

    sendEOF();

    qDebug() << "FileSend finished.";
    emit message(tr(" Send finished!"));
    emit  SendFinish(fileName);

    m_socket->disconnectFromHost();

        if(m_socket->state() == QAbstractSocket::UnconnectedState || m_socket->waitForDisconnected())
        {
            qDebug("Disconnected!");
        }

}

//功能：当socket连接错误后显示错误信息
void clientThread::displaySocketError(QAbstractSocket::SocketError e)
{
    //qDebug() << "Error: " << e << "\n\t" << m_socket->errorString();
    if(e == QAbstractSocket::HostNotFoundError)
        //emit onError(1);
    if(e == QAbstractSocket::ConnectionRefusedError)
    {
        //emit onError(3);
    }
}

/*1.EOF是end of file的缩写，表示"文字流"（stream）的结尾。
这里的"文字流"，可以是文件（file），也可以是标准输入（stdin）。
2.EOF不是特殊字符，而是一个定义在头文件stdio.h的常量，一般等于-1。
#define EOF (-1)
3.除了表示文件结尾，EOF还可以表示标准输入的结尾。
但是，标准输入与文件不一样，无法事先知道输入的长度，
必须手动输入一个字符，表示到达EOF。*/

//功能：到达文件末尾后发送结束标识符
void clientThread::sendEOF()
{
    qint64 size;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    QByteArray data = filePath.toUtf8();
    data.resize(0x8000);
    out << quint16(0x0003) << data << filePath.toUtf8();
    size = block.size();
    m_socket->write((char*)&size, sizeof(qint64));
    m_socket->write(block.data(), size);
    if(!m_socket->waitForBytesWritten(-1))
        qDebug() << "Send EOF Error: " << m_socket->errorString();
    else
        qDebug() << "Send EOF";

    //this->exit(0);

}

void clientThread::on_socket_readySend()
{

}

