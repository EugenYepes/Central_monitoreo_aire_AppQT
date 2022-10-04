#ifndef AIRDATADAO_H
#define AIRDATADAO_H

#include "airdata.h"

class AirDataDAO
{
    /*
     * how i can connect to de DB. Use a local network 127.0.0.1
     * i need ip, port url.
     */
    char *url = "";
    char *user = "root";
    char *pass = "root";
public:
    AirDataDAO();

    /*
        startConnection()
        user: root
        pass: root
        url: example jdbc:mysql://localhost:3306/ where i can find the url for the c++ connection
    */
    /**
     * @brief insertDB
     * QUERY: "INSERT INTO nombreBaseDatos.nombreTabla (campo1, campo2) VALUES (valor, valor);"
     * @param[in] data
     * @return Error Code
     */
    int insertDB(AirData data);

    /**
     * @brief selectDB
     * QUERY: "SELECT campo1, campo2 FROM nombreBaseDatos.nombreTabla WHERE campoN = valor AND campoM = valor;"
     * @param[out] data
     * @param[in] where id
     * @return Error Code
     */
    int selectDB(AirData *data, int where);

    /**
     * @brief deleteDB
     * QUERY: "DELETE FROM nombreBaseDatos.nombreTabla WHERE campoN = valor AND campoM = valor;"
     * @param[in] data
     * @param[in] where
     * @return Error Code
     */
    int deleteDB(AirData data, int where);
};

#endif // AIRDATADAO_H
