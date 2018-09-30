//
//  FLVTagVideo.cpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/23.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#include "FLVTagVideo.hpp"
#include "Sample.hpp"

namespace ffstl
{
	static struct AVCDecoderConfigurationRecord m_avc = {0};
	
    void FLVTagVideo::parser(size_t& size)
    {
        int type = buffer_->read8();
        frameType_ = type >> 4;
        codecID_   = type & 0x0f;

        if(codecID_ != 7)
        {
            std::cout<<"unsupport codec"<<std::endl;
            return;
        }
        
        avcPacketType_ = buffer_->read8();
		cts_ = (buffer_->readBig24() + 0xff800000) ^ 0xff800000;

        if(avcPacketType_ == 0)
        {
            //AVCDecoderConfigurationRecord
            m_avc.configurationVersion = buffer_->read8();
            m_avc.avcProfileIndication = buffer_->read8();
            m_avc.profile_compatibility = buffer_->read8();
            m_avc.AVCLevelIndication = buffer_->read8();
            
            m_avc.naluLengthSize = buffer_->read8() & 0x03 + 1;
			
			m_avc.numOfSequenceParameterSets = buffer_->read8() & 0b11111;
            assert(m_avc.numOfSequenceParameterSets != 0);
            
            for(int i=0;i<m_avc.numOfSequenceParameterSets;i++)
            {
                uint16_t sequenceParameterSetLength = buffer_->readBig16();

                if(sequenceParameterSetLength == 0) continue;
                
                char* buffer = new char[sequenceParameterSetLength];
                buffer_->read(buffer, sequenceParameterSetLength, true);
                std::shared_ptr<SPSParser> sps = std::make_shared<SPSParser>(buffer,sequenceParameterSetLength);
                sps->parser();
            }

			//pps
			m_avc.numOfPictureParameterSets = buffer_->read8();
			for(int i=0;i<m_avc.numOfPictureParameterSets;i++)
			{
				uint16_t pictureParameterSetLength = buffer_->readBig16();

				if(pictureParameterSetLength == 0) continue;
				buffer_->skip(pictureParameterSetLength);
			}
        }
        else if(avcPacketType_ == 1)
        {
            //One or more NALUs (Full frames are required)

			size_t offset = 0;
			size_t dataSize = size - 5;
			while (offset < dataSize)
			{
				if(offset >= dataSize - 4)
				{
					break;
				}

				int32_t naluSize = buffer_->readBig32(0, false);
				if( m_avc.naluLengthSize == 3)
				{
					naluSize >>= 8;
				}

				if(naluSize > dataSize - m_avc.naluLengthSize) return;
				
				int8_t flag = buffer_->read8(4, false);
//				int8_t nal_ref_idc = (flag & 0b01100000) >> 5;
				int8_t nal_unit_type = flag & 0x1f;

				size_t totalLength = naluSize + m_avc.naluLengthSize;
				char* buffer = new char[totalLength];
				buffer_->read(buffer, totalLength, true);

				std::shared_ptr<Sample> sample = std::make_shared<Sample>(buffer,totalLength);
				sample->length_     = totalLength;
				sample->isKeyframe_ = nal_unit_type == 5;
				sample->dts_        = timestamp_;
				sample->cts_		= cts_;
				sample->pts_		= timestamp_ + cts_;

				offset += totalLength;
			}
        }
        else if(avcPacketType_ == 2)
        {
            //end of sequence
			return;
        }
		else
		{
			std::cout<<"avcpacket type error"<<std::endl;
			return;
		}
    }
}
