#include <QtGui/QApplication>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>

#include "mainwindow.h"
//#include <arpa/inet.h>
#include<Winsock2.h>
#include "libpq-fe.h"

//#pragma   comment(lib,"ws2_32")


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

static void exit_nicely(PGconn *conn)
{
    PQfinish(conn);
}

/*
 * This function prints a query result that is a binary-format fetch from
 * a table defined as in the comment above.  We split it out because the
 * main() function uses it twice.
 */
static void
show_binary_results(PGresult *res)
{
    int         i,
            j;
    int         i_fnum,
            t_fnum,
            b_fnum;

    /* Use PQfnumber to avoid assumptions about field order in result */
    i_fnum = PQfnumber(res, "i");
    t_fnum = PQfnumber(res, "t");
    b_fnum = PQfnumber(res, "b");

    for (i = 0; i < PQntuples(res); i++)
    {
        char       *iptr;
        char       *tptr;
        char       *bptr;
        int         blen;
        int         ival;

        /* Get the field values (we ignore possibility they are null!) */
        iptr = PQgetvalue(res, i, i_fnum);
        tptr = PQgetvalue(res, i, t_fnum);
        bptr = PQgetvalue(res, i, b_fnum);

        /*
         * The binary representation of INT4 is in network byte order, which
         * we'd better coerce to the local byte order.
         */
        ival = ntohl(*((uint32_t *) iptr));

        /*
         * The binary representation of TEXT is, well, text, and since libpq
         * was nice enough to append a zero byte to it, it'll work just fine
         * as a C string.
         *
         * The binary representation of BYTEA is a bunch of bytes, which could
         * include embedded nulls so we have to pay attention to field length.
         */
        blen = PQgetlength(res, i, b_fnum);

        printf("tuple %d: got\n", i);
        printf(" i = (%d bytes) %d\n",
               PQgetlength(res, i, i_fnum), ival);
        printf(" t = (%d bytes) '%s'\n",
               PQgetlength(res, i, t_fnum), tptr);
        printf(" b = (%d bytes) ", blen);
        for (j = 0; j < blen; j++)
            printf("\\%03o", bptr[j]);
        printf("\n\n");
    }
}

#if 1
int main(int argc, char *argv[])
{
    PGconn       *conn;
    PGresult   *res;
    const char *paramValues[1];
    int         paramLengths[1];
    int         paramFormats[1];
    uint32_t    binaryIntVal;


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
    // server_version , server_encoding， client_encoding， session_authorization， DateStyle， TimeZone， integer_datetimes 和 standard_conforming_strings
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



    /*
        * The point of this program is to illustrate use of PQexecParams() with
        * out-of-line parameters, as well as binary transmission of data.
        *
        * This first example transmits the parameters as text, but receives the
        * results in binary format.  By using out-of-line parameters we can
        * avoid a lot of tedious mucking about with quoting and escaping, even
        * though the data is text.  Notice how we don't have to do anything
        * special with the quote mark in the parameter value.
        */

    /* Here is our out-of-line parameter value */
    paramValues[0] = "joe's place";

    //创建表

    res = PQexecParams(conn,
                       "SELECT * FROM test1 WHERE t = $1",
                       1,       /* one param */
                       NULL,    /* let the backend deduce param type */
                       paramValues,
                       NULL,    /* don't need param lengths since text */
                       NULL,    /* default to all text params */
                       1);      /* ask for binary results */

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    show_binary_results(res);

    PQclear(res);
















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
