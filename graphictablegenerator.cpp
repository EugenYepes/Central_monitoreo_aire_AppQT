#include "graphictablegenerator.h"

GraphicTableGenerator::GraphicTableGenerator(int design, int numOfMeasur, char *imagePath)
{
    this->numOfMeasur = numOfMeasur;
    openFileBMP(imagePath);
    resizeImg();
    // get all needed airData
    AirData airData[numOfMeasur];
    AirDataDAO airDataDAO;
    //airDataDAO.selectDB();
    for(int i = 0; i < numOfMeasur; i++) {

    }
    switch (design){
        case DESING_BASSIC:
            generateDesignOne(airData, 0xFF0000, 0x000000, 0, 100, 0, 20);
            break;
        default:
            break;
    }
}



int GraphicTableGenerator::generateDesignOne(AirData *airData, int xAxisColor, int yAxisColor, int yLowerValue, int yHeighValue, int xLowerValue, int xHeighValue)
{
    for(;;){
        for(;;){

        }
    }
}

int GraphicTableGenerator::openFileBMP(char *imagePath)
{
    unsigned char *arrImage;
    if ((pFileBMP = fopen(imagePath, "rb")) == NULL) {
        printf("ERROR file not found");
        return -1;
    }
    fread(headerBMP.header, sizeof(char), SIZE_HEADER, pFileBMP);
    // load image in an array
    arrImage = (unsigned char*) malloc(sizeof(unsigned char) * headerBMP.formatHeader.sizeImg);
    if (arrImage == NULL) {
        printf("ERROR fail to alloc memory at %s", __func__);
        return 0;
    }
    fread(arrImage, sizeof(unsigned char), headerBMP.formatHeader.sizeImg, pFileBMP);
    loadPixelStructure(arrImage);
    return 0;
}

int GraphicTableGenerator::loadPixelStructure(unsigned char *imagen)
{
    int row, column, i = 0;
    for (row = 0; row < headerBMP.formatHeader.heightBMP; row++)
    {
        for (column = 0; column < headerBMP.formatHeader.withBMP; column++)
        {
            // BGR because the file starts from the end
            matPixel[row][column].B = imagen[i];
            i++;
            matPixel[row][column].G = imagen[i];
            i++;
            matPixel[row][column].R = imagen[i];
            i++;
        }
    }
    loadCoordinates();
    return 0;
}

int GraphicTableGenerator::loadCoordinates(void)
{
    int row, column, x, y;
    y = 0;
    for (row = 0; row < headerBMP.formatHeader.heightBMP; row++)
    {
        x = 0;
        for (column = 0; column < headerBMP.formatHeader.withBMP; column++, x++)
        {
            matPixel[row][column].x = x;
            matPixel[row][column].y = y;
        }
        y++;
    }
    return 0;
}

void GraphicTableGenerator::updateImgSize(int heighValueImg, int withValueImg)
{
    //call this function every time when the screen resize
    //get the size of the tab
    // QSize  Var = ui->tabWidget->size();// como tener visibilidad de ui
}

int GraphicTableGenerator::resizeImg(void)
{
    int i, j, aux;
    float ratioX, ratioY;//apply this values to made the zoom

    if (heighValueImg != headerBMP.formatHeader.heightBMP || withValueImg != headerBMP.formatHeader.withBMP){
        // get the change size ratio
        ratioX = heighValueImg/headerBMP.formatHeader.heightBMP;
        ratioY = withValueImg/headerBMP.formatHeader.withBMP;

        // update the header with the new sizes
        updateHeader(ratioX, ratioY);

        // resize image matrix
        matPixel = (pixel**)realloc(matPixel, sizeof(pixel *) * headerBMP.formatHeader.heightBMP);
        if (matPixel == NULL) {
            printf("realloc matPixel: error asking for memory\n");
            exit(-1);
        }
        for (i = 0; i < headerBMP.formatHeader.withBMP; i++) {
            matPixel[i] = (pixel*)realloc(matPixel[i], sizeof(pixel) * headerBMP.formatHeader.withBMP);
            if (matPixel[i] == NULL) {
                printf("realloc copiaPixel[%d]: error al pedir memoria\n", i);
                exit(-1);
            }
        }

        for (i = 0; i < headerBMP.formatHeader.heightBMP; i++) {
            for (j = 0; j < headerBMP.formatHeader.withBMP; j++) {
                aux = rounding(ratioX * matPixel[i][j].x);
                matPixel[i][j].y = rounding(ratioY * matPixel[i][j].y);
                matPixel[i][j].x = aux;
            }
        }
        return 0;
    } else {
        printf("The image have the correct size");
        return 1;
    }
    return 0;
}


void GraphicTableGenerator::updateHeader(float ratioX, float ratioY)
{
    HeaderBMP_t headerCopy;
    memcpy(&headerCopy, &headerBMP, sizeof(HeaderBMP_t));
    // calculate the new with
    headerCopy.formatHeader.withBMP = rounding(module(headerBMP.formatHeader.withBMP * ratioX));
    // calculate the new height
    headerCopy.formatHeader.heightBMP = rounding(module(headerBMP.formatHeader.heightBMP * ratioY));
    // size in bytes of the image
    headerCopy.formatHeader.sizeImg = headerCopy.formatHeader.withBMP * headerCopy.formatHeader.heightBMP * (headerBMP.formatHeader.sizePoint/8);
    // size of file
    headerCopy.formatHeader.sizeFile= headerCopy.formatHeader.sizeImg + SIZE_HEADER;
    memcpy(&headerBMP, &headerCopy, sizeof(HeaderBMP_t));
}

int GraphicTableGenerator::rounding(float num)
{
    int rounding;
    float dif;
    dif = num - (int)num;
    rounding = (int)num;
    if (dif > 0.5) {
        rounding++;
    }
    else if (dif < -0.5) {
        rounding--;
    }
    return rounding;
}

float GraphicTableGenerator::module(float num)
{
    if (num > 0)
    {
    }
    else
    {
        num = num * -1;
    }
    return num;
}
