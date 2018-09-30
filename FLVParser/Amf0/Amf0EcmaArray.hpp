//
//  Amf0EcmaArray.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef Amf0EcmaArray_hpp
#define Amf0EcmaArray_hpp

#include <stdio.h>
#include <iostream>
#include <map>

#include "PtrBuffer.hpp"
#include "Amf0.hpp"

namespace ffstl
{
    //video_file_format_spec_v10_1
    class Amf0EcmaArray : public Amf0
    {        
    public:
        Amf0EcmaArray(std::shared_ptr<PtrBuffer> buffer) : Amf0(buffer)
        {
            initParser();
        }
        
        virtual void parser(size_t& size)
        {
            length_ = size;
            Amf0::parser(size);
            
            //Approximate number of items in ECMA array
            buffer_->readBig32();
            
            // 8 === type(UI8) + ECMAArrayLength(UI32) + ScriptDataVariableEnd(UI24)
            int64_t length = length_ - 8;
            size_t skip_size = 0;
            
            while (true)
            {
                if(length <= 0) break;
                
                std::string key = "";
                
                {
                    std::shared_ptr<Amf0String> amf = std::make_shared<Amf0String>(buffer_);
                    amf->parseValue(skip_size);
                    
                    length -= skip_size;
                    skip_size = 0;
                    
                    key = amf->getValue();
                }
                
                if(length <= 0) break;
                
                std::cout<<"key = "<<key<<std::endl;
                
                std::shared_ptr<Amf0> amf = std::make_shared<Amf0>(buffer_);
                amf->analyze();
                
                if(amf->getType() == kEndOfObject
                   || amf->getType() == kUndefined
                   || amf->getType() == kNull
                   || amf->getType() == kUnsupported)
                {
                    char endOfObject;
                    buffer_->read(endOfObject);
                    break;
                }
                
                auto fn = names_[amf->getType()];
                if(fn != nullptr)
                {
                    size_t len = length;
                    push(key, fn(len));
                    skip_size = len;
                }
                
                length -= skip_size;
                skip_size = 0;
            }
        }
        
        void push(std::string key,std::shared_ptr<Amf0> value)
        {
            values_[key] = value;
        }
    
    private:
        std::map<std::string,std::shared_ptr<Amf0>> values_;
    };
}

#endif /* Amf0EcmaArray_hpp */
