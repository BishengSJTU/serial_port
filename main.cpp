#include <iostream>
#include <vector>
#include "serialport.h"
#include "unistd.h"
#include <assert.h>
using namespace std;
int getCRC(vector<unsigned char>& data) {
    assert(data.size() >= 2);
    int crc = 0xffff;
    int pol = 0xa001;
    for(int i = 0; i < data.size() - 2; i++) {
        crc ^= data[i] & 0xff;
        for(int j = 0; j < 8; j++) {
            if(crc % 2 != 0) {
                crc >>= 1;
                crc ^= pol;
            } else {
                crc >>= 1;
            }
        }
    }
    int crc_L = crc / 256;
    int crc_H = crc % 256;
    data[data.size() - 1] = crc_L;
    data[data.size() - 2] = crc_H;
//    std::cout << "CRC计算结果:";
//    for(int i = 0; i < data.size(); i++)
//    {
//        std::cout << std::hex << (unsigned int)(unsigned char)data[i] << ",";
//    }
//    std::cout << std::endl;
    return crc;
}

int main() {
    SerialPort serialPort("/dev/ttyUSB0");
    vector<unsigned char> getState = {0x01,0x03,0x32,0x00,0x00,0x02,0x00,0x00};
    int crc = getCRC(getState);
    serialPort.write(getState);
    usleep(1000000);
    unsigned char dataRead[256];
    int dataLength;
    dataLength = serialPort.read(dataRead, sizeof(dataRead));


    vector<unsigned char> controlWord = {0x01,0x10,0x31,0x00,0x00,0x01,0x02,0x00,0x06,0x00,0x00};
    getCRC(controlWord);
    serialPort.write(controlWord);
    usleep(1000000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));

    vector<unsigned char> workMode = {0x01,0x10,0x35,0x00,0x00,0x01,0x02,0x00,0x06,0x00,0x00};
    getCRC(workMode);
    serialPort.write(workMode);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));

    controlWord[7] = 0x00;
    controlWord[8] = 0x1F;
    getCRC(controlWord);
    serialPort.write(controlWord);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));

    while(1) {
        vector<unsigned char> getPosition= {0x01,0x03,0x37,0x00,0x00,0x04,0x00,0x00};
        getCRC(getPosition);
        serialPort.write(getPosition);
        usleep(100000);

        dataLength = serialPort.read(dataRead, sizeof(dataRead));
        std::cout << "dataRead";
        for (int i = 0; i < dataLength; i++) {
            std::cout << std::hex << (int) dataRead[i] << ",";
        }
        std::cout << std::endl;
        int currentPosition = (dataRead[5] << 24) + (dataRead[6] << 16) + (dataRead[3] << 8) + dataRead[4];
        if(currentPosition == 0x00)
            break;
        usleep(100000);
    }

    controlWord[7] = 0x00;
    controlWord[8] = 0x06;
    getCRC(controlWord);
    serialPort.write(controlWord);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));

    workMode[7] = 0x00;
    workMode[8] = 0x01;
    getCRC(workMode);
    serialPort.write(workMode);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));

    controlWord[7] = 0x10;
    controlWord[8] = 0x3F;
    getCRC(controlWord);
    serialPort.write(controlWord);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));

    vector<unsigned char> setSpeed = {0x01,0x10,0x4A,0x00,0x00,0x02,0x04,0xA8,0x10,0x00,0x29,0x00,0x00};
    getCRC(setSpeed);
    serialPort.write(setSpeed);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));


    vector<unsigned char> setPosition = {0x01,0x10,0x40,0x00,0x00,0x02,0x04,0x0D,0x40,0x00,0x03,0x00,0x00};
    getCRC(setPosition);
    serialPort.write(setPosition);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));
    while(1) {
        vector<unsigned char> getPosition= {0x01,0x03,0x37,0x00,0x00,0x04,0x00,0x00};
        getCRC(getPosition);
        serialPort.write(getPosition);
        usleep(100000);
        dataLength = serialPort.read(dataRead, sizeof(dataRead));
        std::cout << "dataRead";
        for (int i = 0; i < dataLength; i++) {
            std::cout << std::hex << (int) dataRead[i] << ",";
        }
        std::cout << std::endl;
        int currentPosition = (dataRead[5] << 24) + (dataRead[6] << 16) + (dataRead[3] << 8) + dataRead[4];
        if(currentPosition == (0x00 << 24) + (0x03 << 16) + (0x0D << 8) + 0x40)
            break;
        usleep(100000);
    }
    controlWord[7] = 0x00;
    controlWord[8] = 0x06;
    getCRC(controlWord);
    serialPort.write(controlWord);
    usleep(100000);

    workMode[7] = 0x00;
    workMode[8] = 0x01;
    getCRC(workMode);
    serialPort.write(workMode);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));

    controlWord[7] = 0x10;
    controlWord[8] = 0x3F;
    getCRC(controlWord);
    serialPort.write(controlWord);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));


    serialPort.write(setSpeed);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));


    setPosition[7] = 0x00;
    setPosition[8] = 0x00;
    setPosition[9] = 0x00;
    setPosition[10] = 0x00;
    getCRC(setPosition);
    serialPort.write(setPosition);
    usleep(100000);
    dataLength = serialPort.read(dataRead, sizeof(dataRead));
    while(1) {
        vector<unsigned char> getPosition= {0x01,0x03,0x37,0x00,0x00,0x04,0x00,0x00};
        getCRC(getPosition);
        serialPort.write(getPosition);
        usleep(100000);
        dataLength = serialPort.read(dataRead, sizeof(dataRead));
        std::cout << "dataRead";
        for (int i = 0; i < dataLength; i++) {
            std::cout << std::hex << (int) dataRead[i] << ",";
        }
        std::cout << std::endl;
        int currentPosition = (dataRead[5] << 24) + (dataRead[6] << 16) + (dataRead[3] << 8) + dataRead[4];
        if(currentPosition == 0x00)
            break;
        usleep(100000);
    }
    controlWord[7] = 0x00;
    controlWord[8] = 0x06;
    getCRC(controlWord);
    serialPort.write(controlWord);
    usleep(100000);
    return 0;
}
