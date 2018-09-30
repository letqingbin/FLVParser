//
//  AMFParser.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/22.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef AMFParser_hpp
#define AMFParser_hpp

#include <iostream>
#include <memory>
#include <fstream>

#include <vector>

#include "Amf0.hpp"
#include "Amf0Number.hpp"
#include "Amf0Boolean.hpp"
#include "Amf0String.hpp"
#include "Amf0Object.hpp"
#include "Amf0EcmaArray.hpp"
#include "Amf0Array.hpp"
#include "Amf0Date.hpp"
#include "Amf0LongString.hpp"

#include "PtrBuffer.hpp"

namespace ffstl
{
    class AMFParser
    {
    public:
        AMFParser(std::shared_ptr<PtrBuffer> buffer) : buffer_(buffer)
        {
            initParsers();
        }
        
        void parser(size_t& size);
        void initParsers();
        
    private:
        AMFParser(const AMFParser& _rhs);
        AMFParser& operator= (const AMFParser& _rhs);
        
    private:
        std::shared_ptr<PtrBuffer> buffer_;
        std::vector<std::shared_ptr<Amf0>> results_;
        std::map<int,std::function<void(size_t&)>> names_;
    };
}

#endif /* AMFParser_hpp */
