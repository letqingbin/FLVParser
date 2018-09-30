//
//  FLVTagAudio.cpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/23.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#include "FLVTagAudio.hpp"
#include <cassert>

#include "Sample.hpp"

namespace ffstl
{
	static struct MP3Header m_mp3 = {0};
	static struct ACCHeader m_acc = {0};

	void FLVTagAudio::parser(size_t& size)
	{
		int flag = buffer_->read8();

		soundFormat_ = flag >> 4;
		soundRate_   = (flag & 0b00001100) >> 2;
		soundSize_   = (flag & 0b00000010) >> 1;
		soundType_   = flag & 0x01;

		if(soundFormat_ != 2 && soundFormat_ != 10)
		{
			//mp3/aac
			std::cout<<"unsupport audio format"<<std::endl;
			return;
		}

		if(soundFormat_ == 10)
		{
			//aac
			aacPacketType_ = buffer_->read8();
			
			if(aacPacketType_ == 0)
			{
				//AudioSpecificConfig 14496-3
				parse_acc(size-2);
			}
			else if(aacPacketType_ == 1)
			{
				//Raw AAC frame data in UI8[]
			}
		}
		else if(soundFormat_ == 2)
		{
			//mp3
			parse_mp3(size-1);
		}
	}

	void FLVTagAudio::parse_acc(size_t size)
	{
		static int mpegSamplingRates[13] = {96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, 7350};

		//10 bytes
        size_t min_size = size > 10 ? 10 : size;
		char* accBufffer = new char[min_size];
		buffer_->read(accBufffer, min_size, false);
		std::shared_ptr<BitBuffer> bits = std::make_shared<BitBuffer>(accBufffer,min_size);

		m_acc.audioObjectType_ = bits->readBit(5);
		m_acc.samplingFrequencyIndex_ = bits->readBit(4);
		if(m_acc.samplingFrequencyIndex_ >= (sizeof(mpegSamplingRates) / sizeof(int))) return;

		m_acc.channelConfiguration_ = bits->readBit(4);
		if(m_acc.channelConfiguration_ < 0 || m_acc.channelConfiguration_ >= 8)
		{
			std::cout<<"Flv: AAC invalid channel configuration"<<std::endl;
			return;
		}

		if(m_acc.audioObjectType_ == 5)
		{
			// HE-AAC
			m_acc.extensionSamplingIndex_ = bits->readBit(4);
			m_acc.extensionAudioObjectType_ = bits->readBit(5);
		}

		char* sampleBuffer = new char[size];
		buffer_->read(sampleBuffer, size, true);
		std::shared_ptr<Sample> sample = std::make_shared<Sample>(sampleBuffer,size);
		sample->sampleRate_ = mpegSamplingRates[m_acc.samplingFrequencyIndex_];
		sample->channelCount_ = m_acc.channelConfiguration_;
	}

	void FLVTagAudio::parse_mp3(size_t size)
	{
		//MP3Header should be initial only once;
		static int mpegAudioV10SampleRateTable[4] = {44100, 48000, 32000, 0};
		static int mpegAudioV20SampleRateTable[4] = {22050, 24000, 16000, 0};
		static int mpegAudioV25SampleRateTable[4] = {11025, 12000, 8000,  0};

		static int mpegAudioL1BitRateTable[16] = {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1};
		static int mpegAudioL2BitRateTable[16] = {0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, -1};
		static int mpegAudioL3BitRateTable[16] = {0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, -1};

		char* mp3Bufffer = new char[4];
		buffer_->read(mp3Bufffer, 4, false);
		std::shared_ptr<BitBuffer> bits = std::make_shared<BitBuffer>(mp3Bufffer,4);
		m_mp3.syn_    		  = bits->readBit(11);
		m_mp3.version_ 		  = bits->readBit(2);
		m_mp3.layer_  		  = bits->readBit(2);
		m_mp3.protection_ 	  = bits->readBit(1);
		m_mp3.bitrate_   	  = bits->readBit(4);
		m_mp3.frequency_ 	  = bits->readBit(2);
		m_mp3.padding_  	  = bits->readBit(1);
		m_mp3.private_  	  = bits->readBit(1);
		m_mp3.mode_		      = bits->readBit(2);
		m_mp3.mode_extension_ = bits->readBit(2);
		m_mp3.copyright_      = bits->readBit(1);
		m_mp3.original_   	  = bits->readBit(1);
		m_mp3.emphasis_  	  = bits->readBit(1);

		char* sampleBuffer = new char[size];
		buffer_->read(sampleBuffer, size, true);
		std::shared_ptr<Sample> sample = std::make_shared<Sample>(sampleBuffer,size);

		switch (m_mp3.version_) {
			case 0:	//MPEG 2.5
				sample->sampleRate_ = mpegAudioV25SampleRateTable[m_mp3.frequency_];
				break;
			case 2:	//MPEG 2
				sample->sampleRate_ = mpegAudioV20SampleRateTable[m_mp3.frequency_];
				break;
			case 3:	//MPEG 1
				sample->sampleRate_ = mpegAudioV10SampleRateTable[m_mp3.frequency_];
				break;

			default:
				break;
		}

		switch (m_mp3.layer_) {
			case 1:	// Layer 3
				if(m_mp3.bitrate_ < 16)
				{
					sample->bitRate_ = mpegAudioL3BitRateTable[m_mp3.bitrate_];
				}
				break;
			case 2:	// Layer 2
				if(m_mp3.bitrate_ < 16)
				{
					sample->bitRate_ = mpegAudioL2BitRateTable[m_mp3.bitrate_];
				}
				break;
			case 3:	// Layer 1
				if(m_mp3.bitrate_ < 16)
				{
					sample->bitRate_ = mpegAudioL1BitRateTable[m_mp3.bitrate_];
				}
				break;

			default:
				break;
		}

		sample->channelCount_ = m_mp3.mode_ == 0b11 ? 1 : 2;
	}
}

