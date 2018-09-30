//
//  FLVTag.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/23.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef FLVTag_hpp
#define FLVTag_hpp

#include <stdio.h>
#include <iostream>

#include "PtrBuffer.hpp"

namespace ffstl
{
    class FLVTag
	{
    public:
        FLVTag(std::shared_ptr<PtrBuffer> buffer)
        {
            buffer_  = buffer;
            type_ = -1;
            size_ = 0;
            timestamp_ = 0;
        }
        
        virtual void parser(size_t& size);
        ~FLVTag(){};
        
    private:
        FLVTag(const FLVTag& _rhs);
        FLVTag& operator= (const FLVTag& _rhs);
        
    protected:
        int type_;
        size_t size_;
        int64_t timestamp_;
        std::shared_ptr<PtrBuffer> buffer_;
    };
}

#endif /* FLVTag_hpp */
