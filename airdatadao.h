#ifndef AIRDATADAO_H
#define AIRDATADAO_H

#include "airdata.h"
#include "logs.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QDateTime>
#include <iostream>


#define DB_DRIVER "QSQLITE"

//SQL
#define SQL_SELECT      " SELECT "
#define SQL_INSERT      " INSERT INTO "
#define SQL_DELETE      " DELETE "
#define SQL_FROM        " FROM "
#define SQL_WHERE       " WHERE "
#define SQL_VALUES      " VALUES "
#define SQL_AND         " AND "
#define SQL_NONE        ""
#define SQL_FIELD_OXY   " sulfDioxide "
#define SQL_FIELD_CO    " carbonMonoxide "
#define SQL_FIELD_LEL   " lowerExplosiveLimit "
#define SQL_FIELD_TMP   " temperature "
#define SQL_FIELD_DATE  " date "

#define SQL_TABLE_NAME  "datatable"
#define SQL_TABLE_ID    "ID"
#define SQL_QUERY_FORMAT_SELECT "%s*%s%s%s%s=%d;"
#define SQL_QUERY_FORMAT_INSERT "%s%s(%s,%s,%s,%s,%s)%s(%.3f,%.3f,%.3f,%.3f,\"%s\");"
#define SQL_QUERY_FORMAT_DELETE "%s%s%s%s%s=%d;"
#define SQL_QUERY_FORMAT_SELECT_ALL "%s*%s%s;"
#define SQL_QUERY_FORMAT_DELETE_ALL "%s%s%s;"
#define SQL_QUERY_FORMAT_SELECT_BETWEEN_ID "%s*%s%s%s%s>=%d%s%s<=%d;"

class AirDataDAO
{
    QSqlDatabase db;

public:
    AirDataDAO();
    /**
     * @brief insertDB
     * QUERY: "INSERT INTO nombreTabla (campo1, campo2) VALUES (valor, valor);"
     * @param[in] data
     * @return Error Code
     */
    ErrorCode insertDB(AirData data);

    /**
     * @brief selectDB
     * QUERY: "SELECT campo1, campo2 FROM nombreTabla WHERE campoN = valor;"
     * @param[out] data
     * @param[in] where id
     * @return Error Code
     */
    ErrorCode selectDB(AirData *data, int whereID);

    /**
     * @brief selectBetweenIntervalDB
     * select all elements beetwen maxId and lowId
     * dont overflow numbElements, numbElements contains the max elemntes to read
     * numbElements also return the number of readed rows
     * QUERY: "SELECT campo1, campo2 FROM nombreTabla WHERE campoN = valor AND campoM = valor;"
     * @param[out] data
     * @param[in uot] numbElements
     * @param[in] maxId
     * @param[in] lowID
     * @return Error Code
     */
    ErrorCode selectBetweenIntervalDB(AirData *data, int *numbElements, int maxId, int lowID);

    /**
     * @brief selectDB
     * QUERY: "SELECT * FROM nombreTabla;"
     * select all data from data table an only save the number of elements specified in #numbElements.
     * if the table doesn't have enough rows in #numbElements is set the number of readed elements
     * @param[out] data
     * @param[in out] numbElements      number of elements of data, reciev the size of data and returns the number of loaded
     * @return Error code
     */
    ErrorCode selectAllDB(AirData *data, int *numbElements);

    /**
     * @brief deleteDB
     * delete the data when match with the ID
     * QUERY: "DELETE FROM nombreTabla WHERE campoN = valor AND campoM = valor;"
     * after delete reseed de auto increment ID with this QUERY: "DBCC CHECKIDENT (mytable, RESEED, 0)"
     * @param[in] whereID
     * @return Error Code
     */
    ErrorCode deleteDB(int whereID);

    /**
     * @brief getLastID
     * get the last ID of the data base
     * @return error code
     */
    ErrorCode getLastID(int *id);

    /**
     * @brief deleteAllData
     * Delete all data form data base
     * QUERY: "DELETE FROM nombreTabla;"
     * @return error code
     */
    ErrorCode deleteAllData(void);
};

#endif // AIRDATADAO_H
