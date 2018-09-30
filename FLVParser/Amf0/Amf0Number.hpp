//
//  Amf0Number.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef Amf0Number_hpp
#define Amf0Number_hpp

#include <stdio.h>
#include <iostream>

#include "PtrBuffer.hpp"
#include "Amf0.hpp"

namespace ffstl
{
    class Amf0Number : public Amf0
    {
        union av_intfloat64 {
            uint64_t i;
            double   f;
        };
        
    public:
        Amf0Number(std::shared_ptr<PtrBuffer> buffer) : Amf0(buffer)
        {}
        
        virtual void parser(size_t& size)
        {
            Amf0::parser(size);
            parseValue(size);
            size += 1;
        }

        virtual void parseValue(size_t& size)
        {
            length_  = 8;
            value_.i = buffer_->readBig64();
            
            size = length_;
        }
        
    private:
        av_intfloat64 value_;
    };
}

#endif /* Amf0Number_hpp */
