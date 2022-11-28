#ifndef COMMUNIC_H
#define COMMUNIC_H

#include "airdata.h"
#include "airdatadao.h"
#include "logs.h"

#include <QObject>
#include <QIODevice>
#include <QSerialPort>
#include <iostream>
#include <stdio.h>
#include <string.h>
//threads includes
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define TAG_SULFDIOXIDE "\x5F\x00"
#define TAG_CARBON_MONOXIDE "\x5F\x01"
#define TAG_LEL "\x5F\x02"
#define TAG_TEMPERATURE "\x5F\x03"

#define SIZEOF_TAG(data) (sizeof(data)/sizeof(*data))
#define NUM_DECIMALS_FORMAT "%.3f"

class Communic
{
    bool isFormatTLV;
    static QString baudRate;
    static QString portName;
    pthread_t thread;
    static AirData airData;
    static bool readValue;
public:
    static bool isConnected;
    /**
     *@brief communic constructor for serial comunication
     */
    Communic(QString baudRate, QString portName);

    /**
     * @brief parseTLV
     * reciev encoded data and parse to fill airData object
     * @param[in] data data to be parsed
     * @param[out] airData fill the air data object
     * @return Error code
     */
    static int parseAirDataTLV(unsigned char *buffer, int lengthData, AirData *airData);

    /**
     * @brief makeTLV
     * make a buffer with the AirData data, using TLV format
     * @param[in] airData
     * @param[out] buffer
     * @return Error Code
     * @todo
     */
    static int makeTLV(AirData airData, unsigned char **buffer, int *lengthBuffer);

    /**
     * @brief sendMessageSerial
     * send the buffer message through the serial port
     * @return
     */
    static int sendMessageSerial(void);

    /**
     * @brief readMessageSerial
     * read message from the serial port and storage in the buffer
     * @return
     */
    static void* readMessageSerial(void*);

    /**
     * @brief setMessageToSend
     * @param buffer
     */
    void setMessageToSend(unsigned char *buffer);

    /**
     * @brief createCommunicSerialThread
     * @param thread
     */
    void createCommunicSerialThread(void);

    /**
     * @brief closeCommunicSerialThread
     * @param thread
     */
    void closeCommunicSerialThread(void);

    AirData getAirData(){return airData;};
    static bool getReadValue() {return readValue;};
    static void setReadValue(bool readValue) {Communic::readValue = readValue;};
private:
    /**
     * @brief hexToAscii convert a hexadecimal array to a char array (string)
     *
     * @param [in] buffInHex
     * @param [in] tamIn
     * @param [out] buffOutChar
     * @param [out] tamOut
     * @return 0 for success -1 otherwise
     */
    int hexToAscii(unsigned char *buffInHex, int tamIn, unsigned char **buffOutChar, int *tamOut);

    /**
     * @brief asciiToHex convert a char array(string) to a hexadecimal array
     *
     * @param [in] buffInChar
     * @param [in] tamIn
     * @param [out] buffOutHex
     * @param [out] tamOut
     * @return 0 for success -1 otherwise
     */
    int asciiToHex(unsigned char *buffInChar, int tamIn, unsigned char **buffOutHex, int *tamOut);
};

#endif // COMMUNIC_H
