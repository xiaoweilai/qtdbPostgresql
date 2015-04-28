#include <QtGui/QApplication>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>

#include "mainwindow.h"
#include "libpq-fe.h"


/* �������ݿ⣬failed */
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
        // QSqlTableModel ��Ϊ����Դ
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

/* ��ѯ��������ǰ�Ĳ������� */
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

static void exit_nicely(PGconn *conn)
{
    PQfinish(conn);
}



#if 1
int main(int argc, char *argv[])
{
        PGconn       *conn;

        /*
         * PGconn *PQconnectdb(const char *conninfo);
        */

        //const char *conninfo = "hostaddr=127.0.0.1 port=5444 dbname=mydb user=enterprisedb password=edb";
        // conn = PQconnectdb(conninfo);
//        conn = PQconnectdb("hostaddr=127.0.0.1 port=5444 dbname=mydb user=enterprisedb password=edb");

        const char* coninfo = "user=postgres password=00000000 dbname=alldb hostaddr=127.0.0.1 port=5432";
        conn = PQconnectdb(coninfo);
        /*
         *PGconn *PQsetdbLogin(const char *pghost,
             const char *pgport,
             const char *pgoptions,
             const char *pgtty,
             const char *dbName,
             const char *login,
             const char *pwd);
        */

        /* Make a connection to the database */
        //conn = PQsetdbLogin("localhost","5444",NULL,NULL,"mydb","enterprisedb","edb");

        /* Check to see that the backend connection was successfully made */
        if (PQstatus(conn) != CONNECTION_OK)
        {
            qDebug() << "connect failed. PQstatus : " << PQstatus(conn);
            // char *PQerrorMessage(const PGconn *conn);
            qDebug() << PQerrorMessage(conn) ;
            exit_nicely(conn);

            return 0;
        }

        // PQdb
        qDebug() << "database name:" << PQdb(conn);

        // PQuser
        qDebug() << "user:" << PQuser(conn);

        // PQpass
        qDebug() << "password:" << PQpass(conn);

        switch(PQstatus(conn))
        {
        case CONNECTION_STARTED:
            qDebug() << "connecting..." ;
            break;

        case CONNECTION_MADE:
            qDebug() << "to server connected..." ;
            break;

        case CONNECTION_AUTH_OK:
            qDebug() << "auth is ok." ;
            break;

        case CONNECTION_OK:
            qDebug() << "connected." ;
            break;

        default:
            qDebug() << "connecting..." ;
        }

        qDebug() << "---------- connect status func ---------- " ;
        //
        // PQparameterStatus
        // server_version , server_encoding�� client_encoding�� session_authorization�� DateStyle�� TimeZone�� integer_datetimes �� standard_conforming_strings
        qDebug() << "server_version : " << PQparameterStatus(conn,"server_version") ;
        qDebug() << "server_encoding : " << PQparameterStatus(conn,"server_encoding") ;
        qDebug() << "client_encoding : " << PQparameterStatus(conn,"client_encoding") ;
        qDebug() << "session_authorization : " << PQparameterStatus(conn,"session_authorization") ;
        qDebug() << "DateStyle : " << PQparameterStatus(conn,"DateStyle") ;
        qDebug() << "TimeZone : " << PQparameterStatus(conn,"TimeZone") ;
        qDebug() << "integer_datetimes : " << PQparameterStatus(conn,"integer_datetimes") ;
        qDebug() << "standard_conforming_strings : " << PQparameterStatus(conn,"standard_conforming_strings") ;

        // PQprotocolVersion
        qDebug() << "PQprotocolVersion : " << PQprotocolVersion(conn) ;

        // PQbackendPID
        qDebug() << "PQbackendPID : " << PQbackendPID(conn) ;

        // PQgetssl
        qDebug() << "PQgetssl : " << PQgetssl(conn) ;

        PQfinish(conn);

        return 0;
}

#elif 0
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
