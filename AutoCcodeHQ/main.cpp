#include <QtGui/QApplication>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>

#include "mainwindow.h"
#include "libpq-fe.h"


/* 创建数据库，failed */
void CreateDb()
{
    QSqlTableModel *sqlModel;
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL","MYPSQLDBCONN");
    db.setHostName("localhost");
    db.setDatabaseName("testdb");
    db.setUserName("postgres");
    db.setPassword("00000000");
    db.setPort(5432);

    if(db.open())
    {
        // QSqlTableModel 作为数据源
        sqlModel = new QSqlTableModel(NULL,db);
        sqlModel->setTable("testtable");
        sqlModel->setEditStrategy(QSqlTableModel::OnFieldChange);
        sqlModel->select();

//        // http://ode.cnblogs.com
//        // http://odevincent.blog.51cto.com
//        tableView->setModel(sqlModel);
//        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//        tableView->setAlternatingRowColors(true);
//        tableView->setGridStyle(Qt::DashLine);
//        tableView->show();
    }
    else
    {
        QMessageBox::critical(NULL,"error",db.lastError().text());
    }
}

/* 查询服务器当前的参数设置 */
void PrintPQparaInfo(PGconn *conn)
{
    qDebug() << "server_version : " << PQparameterStatus(conn,"server_version");
    qDebug() << "server_encoding : " << PQparameterStatus(conn,"server_encoding");
    qDebug() << "client_encoding : " << PQparameterStatus(conn,"client_encoding");
    qDebug() << "session_authorization : " << PQparameterStatus(conn,"session_authorization");
    qDebug() << "DateStyle : " << PQparameterStatus(conn,"DateStyle");
    qDebug() << "TimeZone : " << PQparameterStatus(conn,"TimeZone");
    qDebug() << "integer_datetimes : " << PQparameterStatus(conn,"integer_datetimes");
    qDebug() << "standard_conforming_strings : " << PQparameterStatus(conn,"standard_conforming_strings");
}


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

    PrintPQparaInfo(conn);


    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver, drivers)
        qDebug()<<driver;

//    CreateDb();

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
