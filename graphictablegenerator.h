#ifndef GRAPHICTABLEGENERATOR_H
#define GRAPHICTABLEGENERATOR_H

#include "mainwindow.h"
#include "airdata.h"
#include "airdatadao.h"

#define IMAGE_PATH_1 ".\table1.bmp"
#define IMAGE_PATH_2 ".\table2.bmp"
#define IMAGE_PATH_SIZE 12

#define DESING_BASSIC 1

#define SIZE_HEADER 54 //default size of the header

#pragma pack(1)
typedef union HeaderBMP
{
    char header[SIZE_HEADER];
    struct FormatHeader
    {
        char typeFile[2];
        int sizeFile;
        short int reserv1;
        short int reserv2;
        int startImg;
        int maxSizeHeader;
        int withBMP;
        int heightBMP;
        short int numPlanes;
        short int sizePoint;
        int compression;
        int sizeImg;
        int resoHorizontal;
        int resoVertical;
        int sizeColorTable;
        int contColors;
    }formatHeader;
}HeaderBMP_t;

#pragma pack(4)

typedef struct pixel
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    int x;
    int y;
} Pixel_t;

class GraphicTableGenerator
{

    int heighValueImg;       // update this value when the screen resizes
    int withValueImg;       // update this value when the screen resizes
    FILE *pFileBMP;
    HeaderBMP_t headerBMP;
    Pixel_t **matPixel;
    char imagePath[IMAGE_PATH_SIZE];
    int numOfMeasur;
public:
    /**
     * @brief GraphicTableGenerator
     * select the image and select the disign of the table
     * @param design
     * @param numOfMeasur
     * @param imagePath
     */
    GraphicTableGenerator(int design, int numOfMeasur, char *imagePath);

    /**
     * @brief updateImgSize
     * update the value of @heighValueImg and @withValueImg atributes with the size of the window
     * @param heighValueImg
     * @param withValueImg
     */
    void updateImgSize(int heighValueImg, int withValueImg);

    /**
     * @brief getImgPath
     * get the immage path
     * @return
     */
    char* getImgPath(){return imagePath;}

    /**
     * @brief ~GraphicTableGenerator
     * free memory and close file
     * @return
    */
    ~GraphicTableGenerator();
private:
    /**
     * @brief generate the first desing for the table
     * this desing is a coordinate plane. In the X axis is the time, and in the Y axis the value of the measurement.
     * load the image with the airData
     * @param [in]airData       the data to load on the graphic, array with the necesary measurements
     * @param [in]xAxisColor    the color of the X axis
     * @param [in]yAxisColor    the color of the Y axis
     * @param [in]yLowerValue   the lower value for the Y axis
     * @param [in]yHeighValue   the heighest value for Y axis
     * @param [in]xLowerValue   the lower value for the X axis
     * @param [in]xHeighValue   the heighest value for X axis
     * @return 0 on success
     */
    int generateDesignOne(AirData *airData, int xAxisColor, int yAxisColor, int yLowerValue, int yHeighValue, int xLowerValue, int xHeighValue);

    /**
     * @brief openFileBMP
     * open a BMP file, load the header struct and load the image in an array
     * @param imagePath
     * @return
     */
    int openFileBMP(char *imagePath);

    /**
     * @brief resizeImg
     * resize the BMP image only when no match with the size of the atributes @heighValueImg and @withValueImg
     * use this dimentions to resize
     * @return
     */
    int resizeImg(void);

    /**
     * @brief updateHeader
     * update the headet
     * @param cabecera
     * @param cabeceraCopia
     * @param ratioX
     * @param ratioY
     */
    void updateHeader(float ratioX, float ratioY);

    /**
     * @brief loadPixelStructure
     * @param [in]imagen image with lineal data of every pixel
     * @return 0 on success
     */
    int loadPixelStructure(unsigned char *imagen);

    /**
     * @todo si no requiero un centro en la imagen, puedo solo tener el primer cuadrante, y no necesito los campos de x e y en la
     * estructura de pixel, pero tengo que adptar las funciones de zoom porque ya no van a trabajar con las coordenadas de la estructura.
     * sino directamente con los subindices de la matriz de pixeles
     * @brief loadCoordinates
     * @return
     */
    int loadCoordinates(void);

    /**
     * @brief rounding
     * rounding a float number and returns the intiger
     * @param num
     * @return
     */
    int rounding(float num);

    /**
     * get the module of a number
     * @brief module
     * @param num
     * @return
     */
    float module(float num);
};
#endif // GRAPHICTABLEGENERATOR_H
