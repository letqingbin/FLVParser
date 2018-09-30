//
//  FLVTagVideo.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/23.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef FLVTagVideo_hpp
#define FLVTagVideo_hpp

#include <stdio.h>
#include <iostream>

#include "PtrBuffer.hpp"
#include "FLVTag.hpp"
#include "SPSParser.hpp"

namespace ffstl
{
	struct AVCDecoderConfigurationRecord
	{
		uint8_t configurationVersion;
		uint8_t avcProfileIndication;
		uint8_t profile_compatibility;
		uint8_t AVCLevelIndication;
		uint8_t naluLengthSize;
		uint8_t numOfSequenceParameterSets;
		uint8_t numOfPictureParameterSets;
	};
	
    class FLVTagVideo : public FLVTag
    {
    public:
        FLVTagVideo(std::shared_ptr<PtrBuffer> buffer) : FLVTag(buffer)
        {
            frameType_ = -1;
            codecID_   = -1;
            avcPacketType_ = -1;
            cts_ = 0;
        }
        
        virtual void parser(size_t& size);
        ~FLVTagVideo(){};

		void setTimestamp(int64_t timestamp)
		{
			timestamp_ = timestamp;
		}

    private:
        FLVTagVideo(const FLVTagVideo& _rhs);
        FLVTagVideo& operator= (const FLVTagVideo& _rhs);
        
    protected:
        int frameType_;
        int codecID_;
        int avcPacketType_;
        int cts_;
		int64_t timestamp_;
    };
}

#endif /* FLVTagVideo_hpp */
