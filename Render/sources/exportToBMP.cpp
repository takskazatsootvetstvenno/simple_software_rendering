#include <fstream>
#include <iostream>
#include <cstdint>
#include <vector>

// CORE bitmap version
struct BitmapInfoHeader {
    uint32_t biSize = 12;
    uint16_t biWidth;
    uint16_t biHeight;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 32;
};

#pragma pack(push, 1)
struct BitmapFileHeader {
    uint16_t bfType = 0x4D42; //little-endian
    uint32_t bfSize;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits;
};
#pragma pack(pop)

static_assert(sizeof(BitmapFileHeader) == 14, "FileHeader size should be 14 bytes!");
static_assert(sizeof(BitmapInfoHeader) == 12, "InfoHeader size should be 14 bytes! (It's \"CORE\" BMP version)");

void exportToBMP(const uint32_t* imageBuffer, const uint32_t width, const uint32_t height) noexcept
{
    try {
        std::ofstream file("RenderedImage.bmp", std::ios::out | std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "exportToBMP: Can't create and open file for writing! File: RenderedImage.bmp "; 
            return;
        }
        const uint32_t bufferSize = sizeof(uint32_t) * width * height;
        BitmapFileHeader fileHeader;
        fileHeader.bfSize = bufferSize + sizeof(BitmapFileHeader);
        fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

        BitmapInfoHeader infoHeader;
        infoHeader.biWidth = width;
        infoHeader.biHeight = height;

        //In BMP format pixel buffer is mirrored vertically
        std::vector<uint32_t> mirrorImageBuffer(width * height);
        std::memcpy(mirrorImageBuffer.data(), imageBuffer, bufferSize);

        for (uint32_t i = 0; i < height / 2; ++i)
            for (uint32_t j = 0; j < width; ++j)
                std::swap(mirrorImageBuffer[i * width + j], mirrorImageBuffer[(height - i - 1) * width + j]);

        file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(BitmapFileHeader));
        file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BitmapInfoHeader));
        file.write(reinterpret_cast<const char*>(mirrorImageBuffer.data()), bufferSize);

    } catch (const std::exception& e) {
        std::cerr << "exportToBMP: Critic exception: " << e.what() << std::endl;
    }
}