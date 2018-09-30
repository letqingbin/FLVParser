//
//  Amf0.cpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#include "Amf0.hpp"

#include "Amf0Number.hpp"
#include "Amf0Boolean.hpp"
#include "Amf0String.hpp"
#include "Amf0Object.hpp"
#include "Amf0EcmaArray.hpp"
#include "Amf0Array.hpp"
#include "Amf0Date.hpp"
#include "Amf0LongString.hpp"

namespace ffstl
{
    void Amf0::initParser()
    {
        int key = Amf0::kNumber;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Number> amf = std::make_shared<Amf0Number>(buffer_);
            amf->parser(size);

            return amf;
        };

        key = Amf0::kBoolean;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Boolean> amf = std::make_shared<Amf0Boolean>(buffer_);
            amf->parser(size);

            return amf;
        };

        key = Amf0::kString;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0String> amf = std::make_shared<Amf0String>(buffer_);
            amf->parser(size);

            return amf;
        };

        key = Amf0::kObject;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Object> amf = std::make_shared<Amf0Object>(buffer_);
            amf->parser(size);

            return amf;
        };

        key = Amf0::kArray;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Object> amf = std::make_shared<Amf0Object>(buffer_);
            amf->parser(size);

            return amf;
        };

        key = Amf0::kEcmaArray;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0EcmaArray> amf = std::make_shared<Amf0EcmaArray>(buffer_);
            amf->parser(size);

            return amf;
        };

        key = Amf0::kDate;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0Date> amf = std::make_shared<Amf0Date>(buffer_);
            amf->parser(size);

            return amf;
        };

        key = Amf0::kLongString;
        names_[key] = [this](size_t& size){
            std::shared_ptr<Amf0LongString> amf = std::make_shared<Amf0LongString>(buffer_);
            amf->parser(size);

            return amf;
        };
    }
}

