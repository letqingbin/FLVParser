//
//  Amf0.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef Amf0_hpp
#define Amf0_hpp

#include <stdio.h>
#include <iostream>
#include <map>

#include "PtrBuffer.hpp"

namespace ffstl
{
    class Amf0
    {
        typedef std::function<std::shared_ptr<Amf0>(size_t&)> FN;
        
    public:
        enum Type
        {
            kNumber      = 0x00,
            kBoolean     = 0x01,
            kString      = 0x02,
            kObject      = 0x03,
            kMovieClip   = 0x04,
            kNull        = 0x05,
            kUndefined   = 0x06,
            kReference   = 0x07,
            kEcmaArray   = 0x08,
            kEndOfObject = 0x09,
            kArray       = 0x0A,
            kDate        = 0x0B,
            kLongString  = 0x0C,
            kUnsupported = 0x0D,
        };
        
        Amf0(std::shared_ptr<PtrBuffer> buffer)
        {
            buffer_ = buffer;
            type_   = kNull;
        }
        
        virtual void analyze()
        {
            type_ = (Type)buffer_->read8(0, false);
        }
        
        virtual void parser(size_t& size)
        {
            type_ = (Type)buffer_->read8();
        }
        
        virtual void parseValue(size_t& size)
        {}
        
        ~Amf0(){};
        
        void initParser();
        
        void setNames(std::map<int,FN> names)
        {
            names_ = names;
        }
        
        Type getType() const
        {
            return type_;
        }
        
        uint64_t getLength() const
        {
            return length_;
        }
        
    private:
        Amf0(const Amf0& _rhs);
        Amf0& operator= (const Amf0& _rhs);
        
    protected:
        Type type_;
        uint64_t length_;
        std::shared_ptr<PtrBuffer> buffer_;
        std::map<int,FN> names_;
    };
}

#endif /* Amf0_hpp */
