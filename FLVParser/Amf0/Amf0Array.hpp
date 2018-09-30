//
//  Amf0Array.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef Amf0Array_hpp
#define Amf0Array_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

#include "PtrBuffer.hpp"
#include "Amf0.hpp"

#include "Amf0Number.hpp"
#include "Amf0Boolean.hpp"
#include "Amf0String.hpp"
#include "Amf0Date.hpp"
#include "Amf0LongString.hpp"

class Amf0Object;
class Amf0EcmaArray;

namespace ffstl
{
    //video_file_format_spec_v10_1
    class Amf0Array : public Amf0
    {        
    public:
        Amf0Array(std::shared_ptr<PtrBuffer> buffer) : Amf0(buffer)
        {
            initParser();
        }
        
        virtual void parser(size_t& size)
        {
            Amf0::parser(size);
            length_ = buffer_->readBig32();
            
            //8 === type(UI8) + ECMAArrayLength(UI32) + ScriptDataVariableEnd(UI24)
            int64_t length = length_ - 8;
            size_t skip_size = 0;
            
            while (true)
            {
                if(length <= 0) break;
                
                std::shared_ptr<Amf0> amf = std::make_shared<Amf0>(buffer_);
                amf->analyze();
                
                if(amf->getType() == kEndOfObject
                   || amf->getType() == kUndefined
                   || amf->getType() == kNull
                   || amf->getType() == kUnsupported)
                    break;
                
                auto fn = names_[amf->getType()];
                if(fn != nullptr)
                {
                    size_t len = length;
                    push(fn(len));
                    skip_size = len;
                }
                
                length -= skip_size;
                skip_size = 0;
            }
        }
        
        void push(std::shared_ptr<Amf0> obj)
        {
            values_.push_back(obj);
        }
    
    private:
        std::vector<std::shared_ptr<Amf0>> values_;
    };
}

#endif /* Amf0Array_hpp */
