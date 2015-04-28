#include <QtGui/QApplication>
#include "mainwindow.h"
#include "libpq-fe.h"

#if 1
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    printf("connect db begin...\n");
    PGconn *conn;
    const char* coninfo = "user=postgres password=00000000 dbname=alldb hostaddr=127.0.0.1 port=5432";
    conn = PQconnectdb(coninfo);
    if(PQstatus(conn)!= CONNECTION_OK)
    {
        printf("connect db failed...\n");
        PQfinish(conn);
       return a.exec();
    }
    printf("connect db successfull...\n");

    PQfinish(conn);

    MainWindow w;
    w.show();
    
    return a.exec();
}

#else
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    printf("now connect db...\n");

    PGconn *conn;
    const char* coninfo = "user=root password=letch dbname=hcviewmib hostaddr=127.0.0.1 port=9988";
    conn = PQconnectdb(coninfo);
    if(PQstatus(conn)!= CONNECTION_OK)
    {
        printf("connect db failed...\n");
        PQfinish(conn);
       return a.exec();
    }
    printf("connect db successfull...\n");

    PQfinish(conn);
    return a.exec();
}

#endif
