#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>

using namespace std;

typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data" string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;

int getFileSize(FILE* inFile);

int main(int argc, char* argv[])
{
    wav_hdr wavHeader;
    int headerSize = sizeof(wav_hdr), filelength = 0;

    const char* filePath;
    string input;
    if (argc <= 1)
    {
        cout << "Input wave file name: ";
        cin >> input;
        cin.get();
        filePath = input.c_str();
    }
    else
    {
        filePath = argv[1];
        cout << "Input wave file name: " << filePath << endl;
    }

    FILE* wavFile = fopen(filePath, "r"); // mở file wav ở chế độ đọc (r)
    if (wavFile == nullptr)
    {
        fprintf(stderr, "Unable to open wave file: %s\n", filePath);
        return 1;
    }

    //Read the header
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    cout << "Header Read " << bytesRead << " bytes." << endl; // độ dài header file wav (44 bytes)
    if (bytesRead > 0)
    {
        //Read the data
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number of bytes per sample
        if (bytesPerSample != 0)uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
        static const uint16_t BUFFER_SIZE = 4096;
        int8_t* buffer = new int8_t[BUFFER_SIZE];
        while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0)
        {
            /** DO SOMETHING WITH THE WAVE DATA HERE **/
            cout << "Read " << bytesRead << " bytes." << endl;
        }
        delete[] buffer;
        buffer = nullptr;
        filelength = getFileSize(wavFile); // tính kích thước tệp

        cout << "File is                    :" << filelength << " bytes." << endl;
        cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << endl;
        cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << endl;
        cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << endl;
        cout << "Data size                  :" << wavHeader.ChunkSize << endl;

        // Hiển thị Tỷ lệ lấy mẫu từ header
        cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << endl; // Mẫu mỗi giây
        cout << "Number of bits used        :" << wavHeader.bitsPerSample << endl; // Số lượng bits được sử dụng
        cout << "Number of channels         :" << wavHeader.NumOfChan << endl;     // Số kênh
        cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;   // Số byte mỗi giây
        cout << "Data length                :" << wavHeader.Subchunk2Size << endl; // Độ dài dữ liệu
        cout << "Audio Format               :" << wavHeader.AudioFormat << endl;   // Định dạng âm thanh
        // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

        cout << "Block align                :" << wavHeader.blockAlign << endl;    // Căn chỉnh khối
        cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << endl;
                                                                                   // Chuỗi dữ liệu
    }
    fclose(wavFile); // đóng file
    return 0;
}

// find the file size
int getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END); // đặt con trỏ ghi tại vị trí thứ 0 bắt đầu từ phần cuối file

    fileSize = ftell(inFile); // lấy tổng kích thước của một file sau khi di chuyển con trỏ tệp ở cuối tệp

    fseek(inFile, 0, SEEK_SET); // đặt lại con trỏ ghi tại vị trí thứ 0 bắt đầu từ phần đầu file
    return fileSize;
}
