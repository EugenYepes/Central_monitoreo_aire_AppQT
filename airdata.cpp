#include "airdata.h"

AirData::AirData()
{
}

AirData::AirData(float sulfDioxide, float carbonMonoxide, float lowerExplosiveLimit, float temperature)
{
    this->sulfDioxide = sulfDioxide;
    this->carbonMonoxide = carbonMonoxide;
    this->lowerExplosiveLimit = lowerExplosiveLimit;
    this->temperature = temperature;
}

void AirData::loadDataFromDB(float sulfDioxide, float carbonMonoxide, float lowerExplosiveLimit, float temperature)
{
    this->sulfDioxide = sulfDioxide;
    this->carbonMonoxide = carbonMonoxide;
    this->lowerExplosiveLimit = lowerExplosiveLimit;
    this->temperature = temperature;
}
