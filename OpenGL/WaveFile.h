#pragma once

#include <stdint.h>
#include <string>
#include <AL/al.h>
#include <AL/alc.h>

// Wav Header

struct WAVHeader
{

	//=========================================================================
	//  RIFF chunk Descriptor
	//=========================================================================

	char chunkID[4]; // "RIFF"
	uint32_t chunkSize;
	char format[4]; // "WAVE"

	//=========================================================================
	// "fmt " sub-chunk
	//=========================================================================

	char subChunk1ID[4]; // "fmt "
	uint32_t subChunk1Size;
	uint16_t audioFormat; // 1=PCM, 6=mulaw, 7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
	uint16_t numChannels; // 1=mono, 2=stereo
	uint32_t sampleRate; // Sampling frequency in Hz
	uint32_t byteRate; // Bytes per second
	uint16_t blockAlign; // 2=16-bit mono, 4=16-bit stereo
	uint16_t bitsPerSample; // Number of bits per sample

	//=========================================================================
	// "data" sub-chunk
	//=========================================================================

	char subChunk2ID[4]; // "data"
	uint32_t subChunk2Size;

	// Variable data size here...
};

class WaveFile
{
public:
	explicit WaveFile(const std::string& path, ALboolean loop = false);
	~WaveFile();

	void setLoop(ALboolean value);

	ALenum getFormat() const;
	ALsizei getSize() const;
	ALsizei getFrequency() const;
	ALboolean getLoop() const;
	ALvoid* getData() const;
private:
	ALenum format{ 0 };
	ALsizei size{ 0 };
	ALsizei frequency{ 0 };
	ALboolean loop{ false };
	ALvoid* data{ nullptr };
};