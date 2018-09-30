//
//  Sample.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/27.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef Sample_hpp
#define Sample_hpp

#include <stdio.h>
#include <iostream>

#include "PtrBuffer.hpp"

namespace ffstl
{
	class Sample
	{
	public:
		Sample(void* _pBuffer, size_t len)
		{
			buffer_ = std::make_shared<PtrBuffer>(_pBuffer,len);
			length_ = len;
			isKeyframe_ = false;
			dts_ = 0;
			cts_ = 0;
			pts_ = 0;
            
            sampleRate_ = 0;
            bitRate_    = 0;
            channelCount_ = 0;
		}

		~Sample()
		{}

	public:
		int64_t length_;

		//video
		bool isKeyframe_;

		//audio
		int sampleRate_;
		int bitRate_;
		int8_t channelCount_;
		
		//common
		int64_t dts_;
		int64_t cts_;
		int64_t pts_;
		std::shared_ptr<PtrBuffer> buffer_;
	};
}

#endif /* Sample_hpp */
