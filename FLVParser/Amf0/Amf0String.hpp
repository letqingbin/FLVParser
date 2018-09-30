//
//  Amf0String.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef Amf0String_hpp
#define Amf0String_hpp

#include <stdio.h>
#include <iostream>

#include "PtrBuffer.hpp"
#include "Amf0.hpp"

namespace ffstl
{
    class Amf0String : public Amf0
    {
    public:
        Amf0String(std::shared_ptr<PtrBuffer> buffer) : Amf0(buffer)
        {}
        
        virtual void parser(size_t& size)
        {
            Amf0::parser(size);
            parseValue(size);
            size += 3;
        }
        
        virtual void parseValue(size_t& size)
        {
            length_ = buffer_->readBig16();
            
            char* str = new char[length_];
            buffer_->read(str, length_, true);
            
            str[length_] = '\0';
            
            value_ = str;
            delete [] str;
            
            size = length_;
        }
        
        std::string getValue() const
        {
            return value_;
        }
        
    private:
        std::string value_;
    };
}

#endif /* Amf0String_hpp */
