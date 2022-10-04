#include "communic.h"

Communic::Communic()
{

}

int Communic::parseAirDataTLV(unsigned char *data, int lengthData, AirData *airData)
{
    float oxygen = -1;
    float carbonMonoxide = -1;
    float lowerExplosiveLimit = -1;
    float temperature = -100;
    int i, j, lengthTag;
    unsigned char aux[50];
    for (i = 0; i < lengthData; i++) {
        memset(aux, 0x00, sizeof(aux));
        lengthTag = 0;

        // tag
        if ((data[i] & 0x1F) == 0x1F) {
            aux[lengthTag] = data[i];
            lengthTag++;
            i++;
        }
        while ((data[i] & 0x80) == 0x80) {// has more tag bytes 1xxx xxxx
            aux[lengthTag] = data[i];
            lengthTag++;
            i++;
        }

        // length of tag
        if ((data[i] & 0x80) == 0x80) {
            if (data[i] != 0x80) {// is not a definite length
                lengthTag += data[i] & 0x7F;
            }
        }
        lengthTag++;

        // get data
        if (memcmp(aux, TAG_OXYGEN, sizeof(TAG_OXYGEN)) == 0) {
            for (j = 0; j < lengthTag; i++, j++) {
                aux[j] =  data [i];
            }
            aux[j] = '\0';
            oxygen = strtof((char*)aux, NULL);
        }
        if (memcmp(aux, TAG_CARBON_MONOXIDE, sizeof(TAG_CARBON_MONOXIDE)) == 0) {
            for (j = 0; j < lengthTag; i++, j++) {
                aux[j] =  data [i];
            }
            aux[j] = '\0';
            carbonMonoxide = strtof((char*)aux, NULL);
        }
        if (memcmp(aux, TAG_LEL, sizeof(TAG_LEL)) == 0) {
            for (j = 0; j < lengthTag; i++, j++) {
                aux[j] =  data [i];
            }
            aux[j] = '\0';
            lowerExplosiveLimit = strtof((char*)aux, NULL);
        }
        if (memcmp(aux, TAG_TEMPERATURE, sizeof(TAG_TEMPERATURE)) == 0) {
            for (j = 0; j < lengthTag; i++, j++) {
                aux[j] =  data [i];
            }
            aux[j] = '\0';
            temperature = strtof((char*)aux, NULL);
        }

    }
    std::cout << "in function" << oxygen << " " << carbonMonoxide << " " << lowerExplosiveLimit << " " << temperature << std::endl;
    if (oxygen == -1 || carbonMonoxide == -1 || lowerExplosiveLimit == -1 || temperature == -100)
        return -1;
    AirData airDataAux(oxygen, carbonMonoxide, lowerExplosiveLimit, temperature);
    *airData = airDataAux;// overload = operator
    return 0;
}

int Communic::makeTLV(AirData airData, unsigned char **buffer)
{
    // oxygen
    memcpy(*buffer, TAG_OXYGEN, sizeof(TAG_OXYGEN));
    *buffer += sizeof(TAG_OXYGEN);
    *buffer += sprintf((char*)*buffer, "%.3f", airData.getOxygen());

    // carbonMonoxide
    memcpy(*buffer, TAG_CARBON_MONOXIDE, sizeof(TAG_CARBON_MONOXIDE));
    *buffer += sizeof(TAG_CARBON_MONOXIDE);
    *buffer += sprintf((char*)*buffer, "%.3f", airData.getCarbonMonoxide());

    // lowerExplosiveLimit
    memcpy(*buffer, TAG_LEL, sizeof(TAG_LEL));
    *buffer += sizeof(TAG_LEL);
    *buffer += sprintf((char*)*buffer, "%.3f", airData.getLowerExplosiveLimit());

    // temperature
    memcpy(*buffer, TAG_TEMPERATURE, sizeof(TAG_TEMPERATURE));
    *buffer += sizeof(TAG_TEMPERATURE);
    *buffer += sprintf((char*)*buffer, "%.3f", airData.getTemperature());
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

/*
  int getValueOfTLVTag(unsigned char *data, unsigned char **value, unsigned char *tag)
{

        //obtener tag y ver si es el pedido
        //ver longitud
        //guardar datos

    int i = 0;
    int lengthTag = 0;
    int lengthDataTag = 0;
    for(i = 0; memcmp(tag, data, tamTag), i++){
        // has next tag byte xxx1 1111
        if((data[i] & 0x1F) == 0x1F){
            tamTag++;
            i++;
        }
        // has more tag bytes 1xxx xxxx
        while((data[i] & 0x80) == 0x80){
            tamTag++;
            i++;
        }
        //get length of data of the tag
        if
    }
    // copy de data of the tag
}
*/
