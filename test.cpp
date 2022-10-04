#include "test.h"

Test::Test()
{

}

int Test::testCommunic(void)
{
    int retVal = -1;
    //test serial parse tlv
    unsigned char buffer[] = "\0x5F\0x00\0x03\0x11\0xFF\0x1A\0x5F\0x01\0x03\0x11\0xFF\0x1A\0x5F\0x02\0x03\0x11\0xFF\0x1A\0x5F\0x03\0x03\0x11\0xFF\0x1A";
    AirData airDataTest;
    retVal = Communic::parseAirDataTLV(buffer, sizeof(buffer),&airDataTest);
    std::cout << airDataTest.getOxygen() << " " << airDataTest.getCarbonMonoxide() << " " << airDataTest.getLowerExplosiveLimit() << " " << airDataTest.getTemperature() << std::endl;
    std::cout << "retVal: " << retVal << std::endl;
    return retVal;
}
