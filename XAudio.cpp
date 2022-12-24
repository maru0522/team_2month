#include "XAudio.h"
#include <wrl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <map>

Microsoft::WRL::ComPtr<IXAudio2> XAudio::xAudio2_{ nullptr };
IXAudio2MasteringVoice* XAudio::masterVoice_{ nullptr };
std::map<XAudio::AUDIO_KEY, XAudio::AUDIO_VALUE> XAudio::audios_{};

void XAudio::Initialize(void)
{
    HRESULT r = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    r = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

void XAudio::Load(const fsPath& pathAndFileName)
{
    // d•¡‚ª‚ ‚Á‚½ê‡“Ç‚Ýž‚Ý‚ð‚Í‚¶‚­B
    if (audios_.count(pathAndFileName)) return;

    if (pathAndFileName.extension() == ".wav") LoadWave(pathAndFileName);
}

void XAudio::UnLoad(SoundData* soundData)
{
    delete[] soundData->pBuffer;

    soundData->pBuffer = 0;
    soundData->bufferSize = 0;
    soundData->wfex = {};
}

void XAudio::PlayWave(IXAudio2* xAudio2, const SoundData& soundData)
{
    IXAudio2SourceVoice* pSourceVoice{ nullptr };
    HRESULT r = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
    assert(SUCCEEDED(r));

    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundData.pBuffer;
    buf.AudioBytes = soundData.bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    r = pSourceVoice->SubmitSourceBuffer(&buf);
    r = pSourceVoice->Start();
}

void XAudio::Finalize(void)
{
    xAudio2_.Reset();

    for ( auto& m : audios_)
    {
        delete[] m.second.pBuffer;

        m.second.wfex = {};
        m.second.pBuffer = 0;
        m.second.bufferSize = 0;
    }

    masterVoice_->DestroyVoice();
}

void XAudio::LoadWave(const fsPath& pathAndFileName)
{
    std::ifstream file{ pathAndFileName, std::ios_base::binary };
    assert(file.is_open());

    RiffHeader riff;
    file.read((char*)&riff, sizeof(riff));
    if (std::strncmp(riff.chunk.id, "RIFF", 4) != 0) assert(0);
    if (std::strncmp(riff.type, "WAVE", 4) != 0) assert(0);

    std::streampos begPos{ file.tellg() };
    SearchHeader(file, "fmt ");

    FormatChunk format{};
    file.read((char*)&format, sizeof(ChunckHeader));
    if (std::strncmp(format.chunk.id, "fmt", 4) != 0) assert(0);
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char*)&format.fmt, format.chunk.size);

    file.seekg(begPos);
    SearchHeader(file, "data");

    ChunckHeader data;
    file.read((char*)&data, sizeof(data));
    if (std::strncmp(data.id, "data", 4) != 0) assert(0);
    char* pBuffer = new char[data.size];
    file.read(pBuffer, data.size);

    AUDIO_VALUE tmp{ format.fmt, reinterpret_cast<uint8_t*>(pBuffer), (uint32_t)data.size };
    audios_.insert({ pathAndFileName, tmp });

    file.close();
}

void XAudio::SearchHeader(std::ifstream& file, const char* chunkId)
{
    ChunckHeader checkHeader;
    std::streampos curPos{ file.tellg()};

    file.read((char*)&checkHeader, sizeof(checkHeader));
    while (std::strncmp(checkHeader.id, "fmt ", 4) != 0)
    {
        file.seekg(curPos);
        file.seekg(checkHeader.size, std::ios_base::cur);
        curPos = file.tellg();
        file.read((char*)&checkHeader, sizeof(checkHeader));
    }
    file.seekg(curPos);
}
