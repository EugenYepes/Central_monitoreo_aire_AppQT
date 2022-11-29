#ifndef AIRDATA_H
#define AIRDATA_H

/**
 * @brief The AirData class
 * this class contains the current data of the air measurements
 * also can be contain the data of the the Data Base
 */
class AirData
{
    float sulfDioxide;
    float carbonMonoxide;
    float lowerExplosiveLimit;
    float temperature;
public:
    AirData();
    AirData(float sulfDioxide, float carbonMonoxide, float lowerExplosiveLimit, float temperature);

    /**
     * @brief load Data From Data Base
     * call this method every time you need to update
     * @param sulfDioxide
     * @param carbonMonoxide
     * @param lowerExplosiveLimit
     * @param temperature
     */
    void loadDataFromDB(float sulfDioxide, float carbonMonoxide, float lowerExplosiveLimit, float temperature);


    float getSulfDioxide(void){return sulfDioxide;};
    float getCarbonMonoxide(void){return carbonMonoxide;};
    float getLowerExplosiveLimit(void){return lowerExplosiveLimit;};
    float getTemperature(void){return temperature;};
};

#endif // AIRDATA_H
