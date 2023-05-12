#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

// CORE bitmap version
struct BitmapInfoHeader {
    uint32_t biSize = 12;
    uint16_t biWidth;
    uint16_t biHeight;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 32;
};

static_assert(sizeof(BitmapInfoHeader) == 12, "InfoHeader size should be 14 bytes! (It's \"CORE\" BMP version)");
static constexpr uint32_t BITMAP_FILE_HEADER_SIZE = 14;
using BitmapFileHeaderInBytes = std::array<uint8_t, BITMAP_FILE_HEADER_SIZE>;

BitmapFileHeaderInBytes createBitmapFileHeader(uint32_t bfSize, uint32_t bfOffBits) noexcept {
    BitmapFileHeaderInBytes bitmapFileHeader{};

    if constexpr (std::endian::native == std::endian::big) {
        bitmapFileHeader[0] = 0x4D;
        bitmapFileHeader[1] = 0x42;
    } else {
        bitmapFileHeader[0] = 0x42;
        bitmapFileHeader[1] = 0x4D;
    }
    *reinterpret_cast<uint32_t*>(&(bitmapFileHeader[2])) = bfSize;
    *reinterpret_cast<uint32_t*>(&(bitmapFileHeader[10])) = bfOffBits;
    return bitmapFileHeader;
}

void exportToBMP(const uint32_t* imageBuffer, const uint32_t width, const uint32_t height) noexcept {
    try {
        std::ofstream file("RenderedImage.bmp", std::ios::out | std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "exportToBMP: Can't create and open file for writing! File: RenderedImage.bmp ";
            return;
        }

        const uint32_t bufferSize = sizeof(uint32_t) * width * height;
        const uint32_t bfSize = bufferSize + BITMAP_FILE_HEADER_SIZE;
        const uint32_t bfOffBits = BITMAP_FILE_HEADER_SIZE + sizeof(BitmapInfoHeader);
        BitmapFileHeaderInBytes bitmapFileHeader = createBitmapFileHeader(bfSize, bfOffBits);
        BitmapInfoHeader infoHeader;
        infoHeader.biWidth = width;
        infoHeader.biHeight = height;

        // In BMP format pixel buffer is mirrored vertically
        std::vector<uint32_t> mirrorImageBuffer(width * height);
        std::memcpy(mirrorImageBuffer.data(), imageBuffer, bufferSize);

        for (uint32_t i = 0; i < height / 2; ++i)
            for (uint32_t j = 0; j < width; ++j)
                std::swap(mirrorImageBuffer[i * width + j], mirrorImageBuffer[(height - i - 1) * width + j]);

        file.write(reinterpret_cast<const char*>(bitmapFileHeader.data()), BITMAP_FILE_HEADER_SIZE);
        file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BitmapInfoHeader));
        file.write(reinterpret_cast<const char*>(mirrorImageBuffer.data()), bufferSize);

    } catch (const std::exception& e) { std::cerr << "exportToBMP: Critic exception: " << e.what() << std::endl; }
}
