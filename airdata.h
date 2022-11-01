#ifndef AIRDATA_H
#define AIRDATA_H

/**
 * @brief The AirData class
 * this class contains the current data of the air measurements
 * also can be contain the data of the the Data Base
 */
class AirData
{
    float oxygen;
    float carbonMonoxide;
    float lowerExplosiveLimit;
    float temperature;
public:
    AirData();
    AirData(float oxygen, float carbonMonoxide, float lowerExplosiveLimit, float temperature);

    /**
     * @brief load Data From Data Base
     * call this method every time you need to update
     * @param oxygen
     * @param carbonMonoxide
     * @param lowerExplosiveLimit
     * @param temperature
     */
    void loadDataFromDB(float oxygen, float carbonMonoxide, float lowerExplosiveLimit, float temperature);

    /**
     * @brief loadDataFromSerialPort
     * @param oxygen
     * @param carbonMonoxide
     * @param lowerExplosiveLimit
     * @param temperature
     * @return Error code
     */
    int loadDataFromSerialPort(float oxygen, float carbonMonoxide, float lowerExplosiveLimit, float temperature);

    AirData operator=(AirData airData);

    float getOxygen(void){return oxygen;};
    float getCarbonMonoxide(void){return carbonMonoxide;};
    float getLowerExplosiveLimit(void){return lowerExplosiveLimit;};
    float getTemperature(void){return temperature;};
};

#endif // AIRDATA_H
