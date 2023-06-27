#include "mytcpserver-ex.h"
#include <QDebug>
#include <QCoreApplication>

void MyTcpServer::swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void MyTcpServer::sift(QList <int> A, int i, int m) 
{
  int j = i, k = i*2+1;	 
  while (k <= m)
  {
    if (k<m && A[k]<A[k+1]) k++; 
    if (A[j] < A[k]) {
    swap(&A[j], &A[k]);
    j = k; k = k*2+1; }
    else break;
  }
}

void MyTcpServer::heap_sort_with_step(QList <int> A, int n, int step) 
{                                                                      
  int i, m;                                                             
  int k = 1;                                                            

  for (i = n/2; i >= 0; i--){
    sift(A, i, n-1);
    k++;
    if (k == step);{ 
        return ; 
    }
  }

  for (m = n-1; m >= 1; m--)
  {
    swap(&A[0], &A[m]); k++;
    if (k == step);{ 
        return ; 
    }
    sift(A, 0, m-1); k++;
    if (k == step);{ 
        return ; 
    }
  }
}

QString MyTcpServer::Exam(QList <int> A, int n, int step){
    QString out_DATA = "";
    heap_sort_with_step(A, n, step);
    for (int i=0;i<A.size();i++){ 
        out_DATA += QString::number(A[i]);
        out_DATA += " ";
    }
    return out_DATA;
}


MyTcpServer::~MyTcpServer()
{
    //mTcpSocket->close();
    mTcpServer->close();
    server_status=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    if(server_status==1){
        mTcpSocket = mTcpServer->nextPendingConnection();
        mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
        connect(mTcpSocket, &QTcpSocket::readyRead,
                this,&MyTcpServer::slotServerRead);
        connect(mTcpSocket,&QTcpSocket::disconnected,
                this,&MyTcpServer::slotClientDisconnected);
    }
}

void MyTcpServer::slotServerRead(){
    QTcpSocket *curr_mTcpSocket = (QTcpSocket*)sender(); 
    QString out_DATA = ""; 
    QString in_DATA;
    int step;
    QList <int> values;
    QStringList temp;
    while(curr_mTcpSocket->bytesAvailable()>0){ 
        in_DATA += curr_mTcpSocket->readAll();
    }
    in_DATA = in_DATA.left(in_DATA.length()-2);  
    qDebug() << in_DATA.toUtf8();  
    QStringList str = in_DATA.split('&'); 
    if (str[0] == "sort"){ 
        step = str[1].toInt(); 
        temp = str[2].split(' '); 
        for (int i=0;i<temp.size();i++){ 
            values.push_back(temp[i].toInt());
        }
        out_DATA = Exam(values,values.size(),step); 
    }
    out_DATA += "\n\r"; 
    qDebug()<<out_DATA; 
    curr_mTcpSocket->write(out_DATA.toUtf8());
    in_DATA.clear(); 
    out_DATA.clear();
}

void MyTcpServer::slotClientDisconnected(){
    mTcpSocket->close();
}
