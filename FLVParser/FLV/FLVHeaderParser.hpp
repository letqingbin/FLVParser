//
//  FLVHeaderParser.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/23.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef FLVHeaderParser_hpp
#define FLVHeaderParser_hpp

#include <stdio.h>
#include <iostream>

#include "PtrBuffer.hpp"

namespace ffstl
{
    class FLVHeaderParser
    {
    public:
        FLVHeaderParser(std::shared_ptr<PtrBuffer> buffer)
        {
            buffer_  = buffer;
            hasVideo_ = false;
            hasAudio_ = false;
        }
        
        void parser(size_t& size)
        {
            int ch_F = buffer_->read8();
            int ch_L = buffer_->read8();
            int ch_V = buffer_->read8();
            
            int version = buffer_->read8();
            int flags = buffer_->read8();
            
            int dataOffset = buffer_->readBig32();
            
            if(ch_F != 0x46 || ch_L != 0x4c || ch_V != 0x56 || version != 0x01 || dataOffset != 0x09)
            {
                std::cout<<"invalid flv format"<<std::endl;
            }
            
            hasAudio_ = flags & 0x04;
            hasVideo_ = flags & 0x01;
            
            size = 9;
        }
    
        ~FLVHeaderParser(){};
        
    private:
        FLVHeaderParser(const FLVHeaderParser& _rhs);
        FLVHeaderParser& operator= (const FLVHeaderParser& _rhs);
        
    protected:
        bool hasVideo_;
        bool hasAudio_;
        std::shared_ptr<PtrBuffer> buffer_;
    };
}

#endif /* FLVHeaderParser_hpp */
