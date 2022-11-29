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
#define TAG_REQUEST "\x5F\x04"
#define TAG_DATE_TIME "\x5F\x05"

#define SIZEOF_TAG(data) (sizeof(data)/sizeof(*data)) - 1
#define NUM_DECIMALS_FORMAT "%.3f"

class Communic
{
    static QString baudRate;
    static QString portName;
    pthread_t thread;
    static AirData airData;
    static bool readValue;
    static unsigned char dataToSend[255];
    static int sizeMsgToSend;
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
    static ErrorCode parseAirDataTLV(unsigned char *buffer, int lengthData, AirData *airData);

    /**
     * @brief makeTLV
     * make a buffer with the AirData data, using TLV format
     * @param[in] airData
     * @param[out] buffer
     * @return Error Code
     * @todo
     */
    static ErrorCode makeTLV(AirData airData, unsigned char **buffer, int *lengthBuffer);

    /**
     * @brief makeTLVdate
     * make a tlv containing the date
     * @param buffer
     * @param lengthBuffer
     * @return Error Code
     */
    static ErrorCode makeTLVdate(unsigned char **buffer, int *lengthBuffer);

    /**
     * @brief readMessageSerial
     * read message from the serial port and storage in the buffer
     * @return
     */
    static void* readMessageSerial(void*);

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

    /**
     * @brief analyzeRequest
     * analyse a request message and get action
     * @param request
     * @return Error Code
     */
    static ErrorCode analyzeRequest(int request);


    //getters and setters
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
     * @return ErrorCode
     */
    ErrorCode hexToAscii(unsigned char *buffInHex, int tamIn, unsigned char **buffOutChar, int *tamOut);

    /**
     * @brief asciiToHex convert a char array(string) to a hexadecimal array
     *
     * @param [in] buffInChar
     * @param [in] tamIn
     * @param [out] buffOutHex
     * @param [out] tamOut
     * @return 0ErrorCode
     */

    ErrorCode asciiToHex(unsigned char *buffInChar, int tamIn, unsigned char **buffOutHex, int *tamOut);
};

#endif // COMMUNIC_H
