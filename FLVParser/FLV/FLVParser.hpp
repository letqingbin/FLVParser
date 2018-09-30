//
//  FLVParser.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/23.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef FLVParser_hpp
#define FLVParser_hpp

#include <iostream>
#include <memory>
#include <fstream>

#include <vector>

#include "PtrBuffer.hpp"
#include "FLVHeaderParser.hpp"
#include "FLVTag.hpp"
#include "FLVTagVideo.hpp"
#include "FLVTagAudio.hpp"
#include "AMFParser.hpp"

namespace ffstl
{
    class FLVParser
    {
    public:
        FLVParser(std::string filePath)
        {
            std::ifstream file;
            file.open(filePath);
            
            file.seekg(0,std::ios::end);
            int64_t length = file.tellg();
            file.seekg(0,std::ios::beg);
            
            char* buffer = new char[length];
            file.read(buffer, length);
            file.close();
            
            buffer_ = std::make_shared<PtrBuffer>((void*)buffer,length);
        }
        
        void parser()
        {
            int64_t length = buffer_->length();
            size_t skip_size = 0;
            
            if(length <= 9+4) return;
            
            header_ = std::make_shared<FLVHeaderParser>(buffer_);
            header_->parser(skip_size);
            length -= skip_size;
            
            while (true)
            {
                if(length <= 4) break;
                
                //PreviousTagSize
                int len = buffer_->readBig32();
                std::cout<<"PreviousTagSize = "<<len<<std::endl;
                
                length -= 4;
                
                //tag
                std::shared_ptr<FLVTag> tag = std::make_shared<FLVTag>(buffer_);
                skip_size = length;
                tag->parser(skip_size);
                
                length -= skip_size;
                skip_size = 0;
            }
        }
        
    private:
        FLVParser(const FLVParser& _rhs);
        FLVParser& operator= (const FLVParser& _rhs);
        
    private:
        std::shared_ptr<PtrBuffer> buffer_;
        std::shared_ptr<FLVHeaderParser> header_;
    };
}

#endif /* FLVParser_hpp */
