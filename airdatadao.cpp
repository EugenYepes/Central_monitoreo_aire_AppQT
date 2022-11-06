#include "airdatadao.h"

AirDataDAO::AirDataDAO()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    ///<@todo get path of the application or only use the local path
    db.setDatabaseName("E:/UTN/EnCurso/Info_2_linux/TPO/AppQt/CentralMonitoreo/airdata.db");
}

int AirDataDAO::insertDB(AirData data)
{
    std::cout << "Entering at function " << __func__ << std::endl;
    std::cout << "db data " << data.getOxygen() << " " << data.getCarbonMonoxide() << " " << data.getLowerExplosiveLimit() << " " << data.getTemperature() << std::endl;
    QSqlQuery q;
    char cQuery[200];
    sprintf(cQuery, SQL_QUERY_FORMAT_INSERT,
            SQL_INSERT, SQL_TABLE_NAME, SQL_FIELD_OXY, SQL_FIELD_CO, SQL_FIELD_LEL, SQL_FIELD_TMP, SQL_FIELD_DATE, SQL_VALUES,
            data.getOxygen(), data.getCarbonMonoxide(), data.getLowerExplosiveLimit(), data.getTemperature(),
            "2022-10-30 00:13:41");
    std::cout << "SQL query: " << cQuery << std::endl;
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        printf("ERROR fail to open DB %d\n", db.lastError().type());
        return 1;
    }
    // execute query
    if (q.exec(query) == false) {
        printf("ERROR fail to exec QUERY %d\n", db.lastError().type());
        return 2;
    }

    q.clear();
    db.close();
    return 0;
}

int AirDataDAO::selectDB(AirData *data, int whereID)
{
    QSqlQuery q;
    float oxygen, carbonMonoxide, lowerExplosiveLimit, temperature;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_SELECT,
            SQL_SELECT, SQL_FROM, SQL_TABLE_NAME, SQL_WHERE, SQL_TABLE_ID, whereID);
    std::cout << "SQL query: " << cQuery << std::endl;
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        printf("ERROR fail to open DB %d\n", db.lastError().type());
        return 1;
    }
    // execute query
    std::cout << "is open" << std::endl;
    if (q.exec(query) == false) {
        printf("ERROR fail to exec QUERY\n");
        return 2;
    }
    // get data
    q.first();
    if (false == q.isValid()) {
        printf("ERROR the regist doesn't exist");
        return 3;
    }
    oxygen = q.value(1).toFloat();
    carbonMonoxide = q.value(2).toFloat();
    lowerExplosiveLimit = q.value(3).toFloat();
    temperature = q.value(4).toFloat();

    data->loadDataFromDB(oxygen, carbonMonoxide, lowerExplosiveLimit, temperature);
    std::cout << "db data " << data->getOxygen() << " " << data->getCarbonMonoxide() << " " << data->getLowerExplosiveLimit() << " " << data->getTemperature() << std::endl;
    q.clear();
    db.close();
    return 0;
}

int AirDataDAO::selectAllDB(AirData *data, int *numbElements)
{
    QSqlQuery q;
    float oxygen, carbonMonoxide, lowerExplosiveLimit, temperature;
    int i = 0;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_SELECT,
            SQL_SELECT, SQL_FROM, SQL_TABLE_NAME);
    std::cout << "SQL query: " << cQuery << std::endl;
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        printf("ERROR fail to open DB %d\n", db.lastError().type());
        return 1;
    }
    // execute query
    std::cout << "is open" << std::endl;
    if (q.exec(query) == false) {
        printf("ERROR fail to exec QUERY\n");
        return 2;
    }
    // get data
    q.first();
    while (q.next() && (i < *numbElements)){
        oxygen = q.value(1).toFloat();
        carbonMonoxide = q.value(2).toFloat();
        lowerExplosiveLimit = q.value(3).toFloat();
        temperature = q.value(4).toFloat();
        data[i].loadDataFromDB(oxygen, carbonMonoxide, lowerExplosiveLimit, temperature);
        i++;
    }
    *numbElements = i;
    q.clear();
    db.close();
    return 0;
}

int AirDataDAO::deleteDB(int whereID)
{
    QSqlQuery q;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_DELETE,
           SQL_DELETE, SQL_FROM, SQL_TABLE_NAME, SQL_WHERE, SQL_TABLE_ID, whereID);
    std::cout << "SQL query: " << cQuery << std::endl;
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        printf("ERROR fail to open DB %d\n", db.lastError().type());
        return 1;
    }
    // execute query
    q.exec(query);

    q.clear();
    db.close();
    return 0;
}

int AirDataDAO::deleteAllData()
{
    QSqlQuery q;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_DELETE_ALL,
           SQL_DELETE, SQL_FROM, SQL_TABLE_NAME);
    std::cout << "SQL query: " << cQuery << std::endl;
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        printf("ERROR fail to open DB %d\n", db.lastError().type());
        return 1;
    }
    // execute query
    q.exec(query);

    q.clear();
    db.close();
    return 0;
    return 0;
}

int AirDataDAO::getLastID(void)
{
    QSqlQuery q;
    int id;
    QString query("SELECT MAX(ID) FROM datatable;");

    // open DB
    if (db.open() == false) {
        printf("ERROR fail to open DB %d\n", db.lastError().type());
        return 1;
    }
    // execute query
    std::cout << "is open" << std::endl;
    if (q.exec(query) == false) {
        printf("ERROR fail to exec QUERY %d\n", db.lastError().type());
        return 2;
    }

    // get data
    q.exec();
    q.first();
    id = q.value(0).toInt();

    q.clear();
    db.close();
    return id;
}
