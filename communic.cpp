#include "communic.h"

QString Communic::portName;
QString Communic::baudRate;
bool Communic::isConnected;
AirData Communic::airData(2, 2, 2, 2);

Communic::Communic(QString baudRate, QString portName)
{
    this->baudRate = baudRate;
    this->portName = portName;
}

int Communic::parseAirDataTLV(unsigned char *buffer, int lengthData, AirData *airData)
{
    float sulfDioxide = -1;
    float carbonMonoxide = -1;
    float lowerExplosiveLimit = -1;
    float temperature = -100;
    int i, j, pos, lengthDataTag;
    unsigned char aux[50];
    unsigned char auxTag[10];
    unsigned char checkByte = 0;
    //check byte
    for (i = 0; i < (lengthData - 1); i++) {
        checkByte ^= buffer[i];
    }
    if (buffer[i] != checkByte) {
        std::cout << "ERROR calculated checkByte dont match with the received byte" << std::endl;
        return -1;
    }
    for (i = 0, pos = 0; i < lengthData;) {
        memset(aux, 0x00, sizeof(aux));
        memset(auxTag, 0x00, sizeof(auxTag));
        lengthDataTag = 0;
        pos = 0;
        // tag
        if ((buffer[i] & 0x1F) == 0x1F) {
            aux[pos] = buffer[i];
        }
        do {// has more tag bytes 1xxx xxxx
            pos++;
            i++;
            aux[pos] = buffer[i];
        } while ((buffer[i] & 0x80) == 0x80);
        pos++;
        i++;
        memcpy(auxTag, aux, sizeof(auxTag));
        // length of tag
        if ((buffer[i] & 0x80) != 0x80) {
            if (buffer[i] != 0x80) {// is not a definite length
                lengthDataTag += buffer[i] & 0x7F;
            } else {
                printf("indefinite length not developed\n");
            }
        }
        i++;
        pos++;
        // get data
        memset(aux, 0x00, sizeof(aux));
        if (memcmp(auxTag, TAG_SULFDIOXIDE, SIZEOF_TAG(TAG_SULFDIOXIDE)) == 0) {
            for (j = 0; j < lengthDataTag; i++, j++) {
                aux[j] =  buffer [i];
            }
            aux[j] = '\0';
            sulfDioxide = strtof((char*)aux, NULL);
        }
        if (memcmp(auxTag, TAG_CARBON_MONOXIDE, SIZEOF_TAG(TAG_CARBON_MONOXIDE)) == 0) {
            for (j = 0; j < lengthDataTag; i++, j++) {
                aux[j] =  buffer [i];
            }
            aux[j] = '\0';
            carbonMonoxide = strtof((char*)aux, NULL);
        }
        if (memcmp(auxTag, TAG_LEL, SIZEOF_TAG(TAG_LEL)) == 0) {
            for (j = 0; j < lengthDataTag; i++, j++) {
                aux[j] =  buffer [i];
            }
            aux[j] = '\0';
            lowerExplosiveLimit = strtof((char*)aux, NULL);
        }
        if (memcmp(auxTag, TAG_TEMPERATURE, SIZEOF_TAG(TAG_TEMPERATURE)) == 0) {
            for (j = 0; j < lengthDataTag; i++, j++) {
                aux[j] =  buffer [i];
            }
            aux[j] = '\0';
            temperature = strtof((char*)aux, NULL);
        }

    }
    printf("in function %s %f %f %f %f\n", __func__, sulfDioxide, carbonMonoxide, lowerExplosiveLimit, temperature);
    if (sulfDioxide == -1 || carbonMonoxide == -1 || lowerExplosiveLimit == -1 || temperature == -100){
        printf("the data isn't complete, INVALID\n");
        return -1;
    }
    AirData airDataAux(sulfDioxide, carbonMonoxide, lowerExplosiveLimit, temperature);
    *airData = airDataAux;// overload = operator why i have a warning
    return 0;
}

int Communic::makeTLV(AirData airData, unsigned char **buffer, int *lengthBuffer)
{
    unsigned char auxBuffer[500] = {0};
    unsigned char auxDataBuffer[10] = {0};
    unsigned char auxDataSize = 0;
    unsigned char checkByte = 0;
    int idx;

    *lengthBuffer = 0;
    printf("Air data in function %s %.3f\t%.3f\t%.3f\t%.3f\n", __func__, airData.getSulfDioxide(), airData.getCarbonMonoxide(), airData.getLowerExplosiveLimit(), airData.getTemperature());

    // Culfure dioxide
    memcpy(auxBuffer + *lengthBuffer, TAG_SULFDIOXIDE, SIZEOF_TAG(TAG_SULFDIOXIDE));
    *lengthBuffer += SIZEOF_TAG(TAG_SULFDIOXIDE);
    auxDataSize = sprintf((char*)auxDataBuffer, NUM_DECIMALS_FORMAT, airData.getSulfDioxide());
    *(auxBuffer + *lengthBuffer) = auxDataSize;
    (*lengthBuffer)++;
    *lengthBuffer += sprintf((char*)auxBuffer + *lengthBuffer, NUM_DECIMALS_FORMAT, airData.getSulfDioxide());

    memset(auxDataBuffer, 0x00, sizeof(auxDataBuffer));

    // Carbon Monoxide
    memcpy(auxBuffer + *lengthBuffer, TAG_CARBON_MONOXIDE, SIZEOF_TAG(TAG_CARBON_MONOXIDE));
    *lengthBuffer += SIZEOF_TAG(TAG_CARBON_MONOXIDE);
    auxDataSize = sprintf((char*)auxDataBuffer, NUM_DECIMALS_FORMAT, airData.getCarbonMonoxide());
    *(auxBuffer + *lengthBuffer) = auxDataSize;
    (*lengthBuffer)++;
    *lengthBuffer += sprintf((char*)auxBuffer + *lengthBuffer, NUM_DECIMALS_FORMAT, airData.getCarbonMonoxide());

    memset(auxDataBuffer, 0x00, sizeof(auxDataBuffer));

    // Lower Explosive Limit
    memcpy(auxBuffer + *lengthBuffer, TAG_LEL, SIZEOF_TAG(TAG_LEL));
    *lengthBuffer += SIZEOF_TAG(TAG_LEL);
    auxDataSize = sprintf((char*)auxDataBuffer, NUM_DECIMALS_FORMAT, airData.getLowerExplosiveLimit());
    *(auxBuffer + *lengthBuffer) = auxDataSize;
    (*lengthBuffer)++;
    *lengthBuffer += sprintf((char*)auxBuffer + *lengthBuffer, NUM_DECIMALS_FORMAT, airData.getLowerExplosiveLimit());

    memset(auxDataBuffer, 0x00, sizeof(auxDataBuffer));

    // Temperature
    memcpy(auxBuffer + *lengthBuffer, TAG_TEMPERATURE, SIZEOF_TAG(TAG_TEMPERATURE));
    *lengthBuffer += SIZEOF_TAG(TAG_TEMPERATURE);
    auxDataSize = sprintf((char*)auxDataBuffer, NUM_DECIMALS_FORMAT, airData.getTemperature());
    *(auxBuffer + *lengthBuffer) = auxDataSize;
    (*lengthBuffer)++;
    *lengthBuffer += sprintf((char*)auxBuffer + *lengthBuffer, "%.3f", airData.getTemperature());
    // set a check byte to send
    for (idx = 0; idx < *lengthBuffer; idx++) {
        checkByte ^= auxBuffer[idx];
    }
    *(auxBuffer + *lengthBuffer) = checkByte;
    (*lengthBuffer)++;
    printf("calculated check byte in %s %d\n", __func__, *(auxBuffer + *lengthBuffer));
    *buffer = (unsigned char*)malloc(*lengthBuffer);
    memcpy(*buffer, auxBuffer, *lengthBuffer);
    return 0;
}

int Communic::hexToAscii(unsigned char *buffInHex, int tamIn, unsigned char **buffOutChar, int *tamOut)
{
    int i, j;
    // BIT_T bitNum;

    if (buffInHex == NULL){
        return -1;
    }
    *tamOut = (tamIn * 2) + 1;
    *buffOutChar = (unsigned char*)malloc((*tamOut) * sizeof(unsigned char));// malloc no funca para el micro, no se le puede pedir al SO
    if (*buffOutChar == NULL) {
        printf("ERROR at malloc memory at function %s", __func__);
        return -1;
    }

    for (i = 0, j = 0; i < tamIn; i++, j++) {
        // get first nible
        (*buffOutChar)[j] = (buffInHex[i] >> 4) + '0';//if de number is greater than 0x39 put a letter
        if ((*buffOutChar)[j] > '9') {
            (*buffOutChar)[j] += 'A' - '9' - 1;
        }
        j++;
        // get second nible
        (*buffOutChar)[j] = (buffInHex[i] & 0x0F) + '0';//if de number is greater than 0x39 put a letter
        if ((*buffOutChar)[j] > '9') {
            (*buffOutChar)[j] += 'A' - '9' - 1;
        }
    }
    (*buffOutChar)[j] = '\0';
    return 0;
}

int Communic::asciiToHex(unsigned char *buffInChar, int tamIn, unsigned char **buffOutHex, int *tamOut)
{
    int i, j;
    unsigned char aux1, aux2;

    if (buffInChar == NULL){
        return -1;
    }

    *tamOut = tamIn/2;
    *buffOutHex = (unsigned char*)malloc((*tamOut) * sizeof(unsigned char));
    if (*buffOutHex == NULL) {
        printf("ERROR at malloc memory at function %s", __func__);
        return -1;
    }

    for (i = 0, j = 0; j < *tamOut; i++, j++) {
        // first nible
        aux1 = buffInChar[i] - '0';
        if (aux1 > 0x09) {
            aux1 -= 'A' - '9' - 1;
        }
        // second nible
        i++;
        aux2 = buffInChar[i] - '0';//if de number is greater than 0x39 put a letter
        if (aux2 > 0x09) {
            aux2 -= 'A' - '9' - 1;
        }
        (*buffOutHex)[j] = (aux1 << 4) | aux2;
    }
    return 0;
}

void *Communic::readMessageSerial(void* arg)
{
    AirDataDAO *airDataDAO = new AirDataDAO();
    QSerialPort serial;
    serial.setPortName(portName);
    serial.setBaudRate(baudRate.toInt());
    serial.close();


    if (!serial.open(QIODevice::ReadWrite)) {
        std::cout << "serial port doesn't open at function " << __func__ << std::endl;
        return NULL;
    }
    sleep(1);
    while (true) {
        QByteArray data = serial.readAll();
        while (serial.waitForReadyRead(10))
            data += serial.readAll();
        if (!data.isEmpty()) {
            std::cout << "received data: " << data.toStdString() << std::endl;
            if (parseAirDataTLV((unsigned char*)data.data(), data.size(), &airData) == 0) {
                std::cout << "save data in DB" << std::endl;
                airDataDAO->insertDB(airData);
            }
        }
    }
}

void Communic::createCommunicSerialThread()
{
    if (pthread_create(&thread, NULL, Communic::readMessageSerial, NULL) != 0) {
        isConnected = true;
        std::cout << "create thread " << thread << std::endl;
    }
}

void Communic::closeCommunicSerialThread(void)
{
    if (pthread_kill(thread, 17) != 0) {
        isConnected = false;
        std::cout << "close thread " << thread << std::endl;
    }
}

