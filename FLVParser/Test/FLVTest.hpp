//
//  FLVTest.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/26.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef FLVTest_hpp
#define FLVTest_hpp

#include <stdio.h>
#include "BitBuffer.hpp"
#include "Golomb.hpp"

namespace ffstl
{
	class FLVTest
	{
	public:
		FLVTest(){};

		void testReadBit()
		{
			char* buffer = new char[1];
			buffer[0] = 0b01100111;		// 0x67
			std::shared_ptr<BitBuffer> bits = std::make_shared<BitBuffer>(buffer,1);
			int bit1 = bits->readBit(1);	//0
			int bit4 = bits->readBit(4);	//12
			int bit3 = bits->readBit(3);	//7

			std::cout<<bit1<<"\t"<<bit4<<"\t"<<bit3<<std::endl;
		}

		void testReadByte()
		{
			char* buffer = new char[7];
			buffer[0] = 0x67;		// 0x67

			buffer[1] = 0xc0;
			buffer[2] = 0x15;

			buffer[3] = 0x02;
			buffer[4] = 0x05;
			buffer[5] = 0xcc;
			buffer[6] = 0x11;

			std::shared_ptr<BitBuffer> bits = std::make_shared<BitBuffer>(buffer,sizeof(buffer)/sizeof(char));
			int byte1 = bits->readBit(8);	//103
			int byte2 = bits->readBit(16);	//49173
			int byte4 = bits->readBit(32);	//33934353

			std::cout<<byte1<<"\t"<<byte2<<"\t"<<byte4<<std::endl;
		}

		void testGolomb()
		{
			{
				char* buffer = new char[1];
				buffer[0] = 0b00010010;	// codesum=8 (the last 0 is for test)
				std::shared_ptr<BitBuffer> bits = std::make_shared<BitBuffer>(buffer,1);
				int value = Golomb::read_ue(bits);

				std::cout<<value<<std::endl;
			}


			{
				char* buffer = new char[1];
				buffer[0] = 0b00111000;	// codesum=6 (the last three's 0 is for test)
				std::shared_ptr<BitBuffer> bits = std::make_shared<BitBuffer>(buffer,1);
				int value = Golomb::read_ue(bits);

				std::cout<<value<<std::endl;
			}

			{
				char* buffer = new char[1];
				buffer[0] = 0b00010010;	// codesum=-4 (the last 0 is for test)
				std::shared_ptr<BitBuffer> bits = std::make_shared<BitBuffer>(buffer,1);
				int value = Golomb::read_se(bits);

				std::cout<<value<<std::endl;
			}
		}
	};
}

#endif /* FLVTest_hpp */
