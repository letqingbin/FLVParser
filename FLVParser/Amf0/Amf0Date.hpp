//
//  Amf0Date.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef Amf0Date_hpp
#define Amf0Date_hpp

#include <stdio.h>
#include <iostream>

#include "PtrBuffer.hpp"
#include "Amf0.hpp"

namespace ffstl
{
    //video_file_format_spec_v10_1 - SCRIPTDATADATE
    class Amf0Date : public Amf0
    {
    public:
        Amf0Date(std::shared_ptr<PtrBuffer> buffer) : Amf0(buffer)
        {}
        
        virtual void parser(size_t& size)
        {
            Amf0::parser(size);
            parseValue(size);
            size += 1;
        }
        
        virtual void parseValue(size_t& size)
        {
            length_ = 10;
            value_  = buffer_->readBig64();
            
            //SI16
            int timestamp = 0;
            buffer_->read(&timestamp, 2, true);;
            
            //UTC time
            value_ += timestamp * 60 * 1000;
            
            size = length_;
        }
        
    private:
        double value_;
    };
}

#endif /* Amf0Date_hpp */
