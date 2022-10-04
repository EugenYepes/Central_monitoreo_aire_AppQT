#include "airdata.h"

AirData::AirData()
{
}

AirData::AirData(float oxygen, float carbonMonoxide, float lowerExplosiveLimit, float temperature)
{
    this->oxygen = oxygen;
    this->carbonMonoxide = carbonMonoxide;
    this->lowerExplosiveLimit = lowerExplosiveLimit;
    this->temperature = temperature;
}

AirData AirData::operator=(AirData airData)
{
    oxygen = airData.oxygen;
    carbonMonoxide = airData.carbonMonoxide;
    lowerExplosiveLimit = airData.lowerExplosiveLimit;
    temperature = airData.temperature;
    return airData;
}
