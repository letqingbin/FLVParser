//
//  Amf0Boolean.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef Amf0Boolean_hpp
#define Amf0Boolean_hpp

#include <stdio.h>
#include <iostream>

#include "PtrBuffer.hpp"
#include "Amf0.hpp"

namespace ffstl
{
    class Amf0Boolean : public Amf0
    {
    public:
        Amf0Boolean(std::shared_ptr<PtrBuffer> buffer) : Amf0(buffer)
        {}
        
        virtual void parser(size_t& size)
        {
            Amf0::parser(size);
            parseValue(size);
            size += 1;
        }
        
        virtual void parseValue(size_t& size)
        {
            length_ = 1;
            value_  = buffer_->read8();
            
            size = length_;
        }
        
    private:
        bool value_;
    };
}

#endif /* Amf0Boolean_hpp */
