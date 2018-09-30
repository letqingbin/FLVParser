//
//  FLVTag.cpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/23.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#include "FLVTag.hpp"
#include "FLVTagVideo.hpp"
#include "FLVTagAudio.hpp"
#include "AMFParser.hpp"

namespace ffstl
{
    void FLVTag::parser(size_t& size)
    {
        type_ = buffer_->read8() & 0x1f;
        size_ = buffer_->readBig24();
        int64_t time = buffer_->readBig24();
        timestamp_ = buffer_->read8() << 24 | time;
        
        //streamid
        buffer_->readBig24();
        
        size_t skip_size = size_;
        
        //video/audio/script
        if(type_ == 8)
        {
            //audio
			std::shared_ptr<FLVTagAudio> tag = std::make_shared<FLVTagAudio>(buffer_);
			tag->parser(skip_size);
        }
        else if(type_ == 9)
        {
            //video
            std::shared_ptr<FLVTagVideo> tag = std::make_shared<FLVTagVideo>(buffer_);
			tag->setTimestamp(timestamp_);
            tag->parser(skip_size);
        }
        else if(type_ == 18)
        {
            //script
            std::shared_ptr<AMFParser> amf = std::make_shared<AMFParser>(buffer_);
            amf->parser(skip_size);
        }
        
        size = size_ + 11;
    }
}
