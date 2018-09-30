//
//  main.cpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/30.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#include <iostream>

#include "FLVParser.hpp"

int main(int argc, const char * argv[])
{
	//input your flv file url
	std::shared_ptr<ffstl::FLVParser> flv = std::make_shared<ffstl::FLVParser>("");
	flv->parser();

	return 0;
}
