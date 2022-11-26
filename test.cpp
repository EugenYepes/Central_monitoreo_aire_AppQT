#include "test.h"

Test::Test()
{

}

int Test::testCommunic(void)
{
    int retVal = -1;
    //test serial parse tlv
    unsigned char buffer[] = "\x5F\x00\x03\x30\x2E\x34\x5F\x01\x03\x31\x2E\x34\x5F\x02\x03\x37\x2E\x32\x5F\x03\x03\x35\x2E\x38\x09";//the last byte is de xor checkbyte
    unsigned char *outBuffer = NULL;
    AirData airDataTest;

    LOG_MSG("test parseAirDataTLV");
    retVal = Communic::parseAirDataTLV(buffer, sizeof(buffer), &airDataTest);
    LOG_MSG("%.3f %.3f %.3f %.3f", airDataTest.getSulfDioxide(), airDataTest.getCarbonMonoxide(), airDataTest.getLowerExplosiveLimit(), airDataTest.getTemperature());
    LOG_MSG("retVal", retVal);

    int lengthBuffer;
    LOG_MSG("test makeTLV");
    retVal = Communic::makeTLV(airDataTest, &outBuffer, &lengthBuffer);
    for (int i = 0; i < lengthBuffer; i++) {
        printf("%02X", outBuffer[i]);
    }
    printf("\n");
    LOG_MSG("retVal", retVal);

    AirData airDataTest2;
    LOG_MSG("test recover data");
    retVal = Communic::parseAirDataTLV(outBuffer, lengthBuffer, &airDataTest2);
    LOG_MSG("after: %.3f %.3f %.3f %.3f", airDataTest2.getSulfDioxide(), airDataTest2.getCarbonMonoxide(), airDataTest2.getLowerExplosiveLimit(), airDataTest2.getTemperature());
    LOG_MSG("retVal", retVal);
    return retVal;
}
