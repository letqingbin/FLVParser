//
//  AMFParser.cpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#include "AMFParser.hpp"

namespace ffstl
{
    void AMFParser::parser(size_t& size)
    {
        int64_t length = size;
        size_t skip_size = 0;
    
        while (true)
        {
            if(length <= 0) break;
            
            std::shared_ptr<Amf0> amf = std::make_shared<Amf0>(buffer_);
            amf->analyze();
            
            auto fn = names_[amf->getType()];
            skip_size = length;
            if(fn != nullptr)
            {
                fn(skip_size);
            }
            
            length -= skip_size;
            skip_size = 0;
        }
    }
    
    void AMFParser::initParsers()
    {
        int key = Amf0::kNumber;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Number> amf = std::make_shared<Amf0Number>(buffer_);
            amf->parser(size);
        };
        
        key = Amf0::kBoolean;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Boolean> amf = std::make_shared<Amf0Boolean>(buffer_);
            amf->parser(size);
        };
        
        key = Amf0::kString;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0String> amf = std::make_shared<Amf0String>(buffer_);
            amf->parser(size);
        };
        
        key = Amf0::kObject;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Object> amf = std::make_shared<Amf0Object>(buffer_);
            amf->parser(size);
        };
        
        key = Amf0::kArray;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Object> amf = std::make_shared<Amf0Object>(buffer_);
            amf->parser(size);
        };
        
        key = Amf0::kEcmaArray;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0EcmaArray> amf = std::make_shared<Amf0EcmaArray>(buffer_);
            amf->parser(size);
        };
        
        key = Amf0::kDate;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Date> amf = std::make_shared<Amf0Date>(buffer_);
            amf->parser(size);
        };
        
        key = Amf0::kLongString;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0LongString> amf = std::make_shared<Amf0LongString>(buffer_);
            amf->parser(size);
        };
    }
}
