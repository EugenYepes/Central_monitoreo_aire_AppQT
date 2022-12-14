#include "airdatadao.h"

AirDataDAO::AirDataDAO()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    ///<@todo get path of the application or only use the local path
    db.setDatabaseName("C:/INFO2/AppQt/Central_monitoreo_aire_AppQT/airdata.db");
    //C:\INFO2\AppQt\Central_monitoreo_aire_AppQT
}

ErrorCode AirDataDAO::insertDB(AirData data)
{
    LOG_MSG("db data to insert %.3f %.3f %.3f %.3f ", data.getSulfDioxide(), data.getCarbonMonoxide(), data.getLowerExplosiveLimit(), data.getTemperature());
    QSqlQuery q;
    char cQuery[200];

    sprintf(cQuery, SQL_QUERY_FORMAT_INSERT,
            SQL_INSERT, SQL_TABLE_NAME, SQL_FIELD_OXY, SQL_FIELD_CO, SQL_FIELD_LEL, SQL_FIELD_TMP, SQL_FIELD_DATE, SQL_VALUES,
            data.getSulfDioxide(), data.getCarbonMonoxide(), data.getLowerExplosiveLimit(), data.getTemperature(),
            getDate());
    LOG_MSG("SQL query: %s", cQuery);
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        LOG_MSG("ERROR fail to open DB %d", db.lastError().text().data());
        return ERROR_OPEN;
    }
    // execute query
    if (q.exec(query) == false) {
        LOG_MSG("ERROR fail to exec QUERY %s", db.lastError().text().data());
        return ERROR_SQL_QUERY;
    }

    q.clear();
    db.close();
    return SUCCESS;
}

ErrorCode AirDataDAO::selectDB(AirData *data, int whereID)
{
    QSqlQuery q;
    float sulfDioxide, carbonMonoxide, lowerExplosiveLimit, temperature;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_SELECT,
            SQL_SELECT, SQL_FROM, SQL_TABLE_NAME, SQL_WHERE, SQL_TABLE_ID, whereID);
    LOG_MSG("SQL query: %s", cQuery);
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        LOG_MSG("ERROR fail to open DB %d", db.lastError().type());
        return ERROR_OPEN;
    }

    // execute query
    if (q.exec(query) == false) {
        LOG_MSG("ERROR fail to exec QUERY %d", db.lastError().type());
        return ERROR_SQL_QUERY;
    }
    // get data
    q.first();
    if (false == q.isValid()) {
        LOG_MSG("ERROR the regist doesn't exist");
        return ERROR_UNKNOWN;
    }
    sulfDioxide = q.value(1).toFloat();
    carbonMonoxide = q.value(2).toFloat();
    lowerExplosiveLimit = q.value(3).toFloat();
    temperature = q.value(4).toFloat();

    data->loadDataFromDB(sulfDioxide, carbonMonoxide, lowerExplosiveLimit, temperature);
    LOG_MSG("db data selected %.3f %.3f %.3f %.3f ", data->getSulfDioxide(), data->getCarbonMonoxide(), data->getLowerExplosiveLimit(), data->getTemperature());
    q.clear();
    db.close();
    return SUCCESS;
}

ErrorCode AirDataDAO::selectBetweenIntervalDB(AirData *data, int *numbElements, int maxId, int lowID)
{
    QSqlQuery q;
    float sulfDioxide, carbonMonoxide, lowerExplosiveLimit, temperature;
    int i = 0;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_SELECT_BETWEEN_ID,
            SQL_SELECT, SQL_FROM, SQL_TABLE_NAME, SQL_WHERE, SQL_TABLE_ID, lowID, SQL_AND, SQL_TABLE_ID, maxId);
    LOG_MSG("SQL query: %s", cQuery);
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        LOG_MSG("ERROR fail to open DB %d", db.lastError().type());
        return ERROR_OPEN;
    }

    // execute query
    if (q.exec(query) == false) {
        LOG_MSG("ERROR fail to exec QUERY %d", db.lastError().type());
        return ERROR_SQL_QUERY;
    }
    // get data
    q.first();
    ///<@todo select from the ent to the first element (SELECT * FROM tableName ORDER BY col1 DESC LIMIT 1)   AS last
    while (q.next() && (i < *numbElements)){
        sulfDioxide = q.value(1).toFloat();
        carbonMonoxide = q.value(2).toFloat();
        lowerExplosiveLimit = q.value(3).toFloat();
        temperature = q.value(4).toFloat();
        data[i].loadDataFromDB(sulfDioxide, carbonMonoxide, lowerExplosiveLimit, temperature);
        i++;
    }
    *numbElements = i;
    q.clear();
    db.close();
    return SUCCESS;
}

ErrorCode AirDataDAO::selectAllDB(AirData *data, int *numbElements)
{
    QSqlQuery q;
    float sulfDioxide, carbonMonoxide, lowerExplosiveLimit, temperature;
    int i = 0;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_SELECT_ALL,
            SQL_SELECT, SQL_FROM, SQL_TABLE_NAME, SQL_NONE);
    LOG_MSG("SQL query: %s", cQuery);
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        LOG_MSG("ERROR fail to open DB %d", db.lastError().type());
        return ERROR_OPEN;
    }
    
    // execute query
    if (q.exec(query) == false) {
        LOG_MSG("ERROR fail to exec QUERY %d", db.lastError().type());
        return ERROR_SQL_QUERY;
    }
    // get data
    q.first();
    while (q.next() && (i < *numbElements)){
        sulfDioxide = q.value(1).toFloat();
        carbonMonoxide = q.value(2).toFloat();
        lowerExplosiveLimit = q.value(3).toFloat();
        temperature = q.value(4).toFloat();
        data[i].loadDataFromDB(sulfDioxide, carbonMonoxide, lowerExplosiveLimit, temperature);
        i++;
    }
    *numbElements = i;
    q.clear();
    db.close();
    return SUCCESS;
}

ErrorCode AirDataDAO::deleteDB(int whereID)
{
    QSqlQuery q;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_DELETE,
           SQL_DELETE, SQL_FROM, SQL_TABLE_NAME, SQL_WHERE, SQL_TABLE_ID, whereID);
    LOG_MSG("SQL query: %s", cQuery);
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        LOG_MSG("ERROR fail to open DB %d", db.lastError().type());
        return ERROR_OPEN;
    }
    // execute query
    if (q.exec(query) == false) {
        LOG_MSG("ERROR fail to exec QUERY %d", db.lastError().type());
        return ERROR_SQL_QUERY;
    }

    q.clear();
    db.close();
    return SUCCESS;
}

ErrorCode AirDataDAO::deleteAllData()
{
    QSqlQuery q;
    char cQuery[100];
    sprintf(cQuery, SQL_QUERY_FORMAT_DELETE_ALL,
           SQL_DELETE, SQL_FROM, SQL_TABLE_NAME);
    LOG_MSG("SQL query: %s", cQuery);
    QString query(cQuery);

    // open DB
    if (db.open() == false) {
        LOG_MSG("ERROR fail to open DB %d", db.lastError().type());
        return ERROR_OPEN;
    }
    // execute query
    if (q.exec(query) == false) {
        LOG_MSG("ERROR fail to exec QUERY %d", db.lastError().type());
        return ERROR_SQL_QUERY;
    }

    q.clear();
    db.close();
    return SUCCESS;
}

ErrorCode AirDataDAO::getLastID(int *id)
{
    QSqlQuery q;
    QString query("SELECT MAX(ID) FROM datatable;");

    // open DB
    if (db.open() == false) {
        LOG_MSG("ERROR fail to open DB %d", db.lastError().type());
        return ERROR_OPEN;
    }

    // execute query
    if (q.exec(query) == false) {
        LOG_MSG("ERROR fail to exec QUERY %d", db.lastError().type());
        return ERROR_SQL_QUERY;
    }

    // get data
    q.exec();
    q.first();
    *id = q.value(0).toInt();

    q.clear();
    db.close();
    return SUCCESS;
}
