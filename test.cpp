#include "test.h"

Test::Test()
{

}

int Test::testCommunic(void)
{
    int retVal = -1;
    //test serial parse tlv
    unsigned char buffer[] = "\x5F\x00\x03\x30\x2E\x34\x5F\x01\x03\x31\x2E\x34\x5F\x02\x03\x37\x2E\x32\x5F\x03\x03\x35\x2E\x38\x09";
    unsigned char *outBuffer = NULL;
    AirData airDataTest;
    std::cout << "test parseAirDataTLV" << std::endl;
    retVal = Communic::parseAirDataTLV(buffer, sizeof(buffer), &airDataTest);
    std::cout << airDataTest.getSulfDioxide() << " " << airDataTest.getCarbonMonoxide() << " " << airDataTest.getLowerExplosiveLimit() << " " << airDataTest.getTemperature() << std::endl;
    std::cout << "retVal: " << retVal << std::endl;

    int lengthBuffer;
    std::cout << "test makeTLV" << std::endl;
    retVal = Communic::makeTLV(airDataTest, &outBuffer, &lengthBuffer);
    for (int i = 0; i < lengthBuffer; i++) {
        printf("%02X", outBuffer[i]);
    }
    printf("\n");
    std::cout << "retVal: " << retVal << std::endl;

    AirData airDataTest2;
    std::cout << "test recover data" << std::endl;
    retVal = Communic::parseAirDataTLV(outBuffer, lengthBuffer, &airDataTest2);
    std::cout << "after: " << airDataTest2.getSulfDioxide() << " " << airDataTest2.getCarbonMonoxide() << " " << airDataTest2.getLowerExplosiveLimit() << " " << airDataTest2.getTemperature() << std::endl;
    std::cout << "retVal: " << retVal << std::endl;
    return retVal;
}
