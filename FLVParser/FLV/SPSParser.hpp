//
//  SPSParser.hpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/24.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#ifndef SPSParser_hpp
#define SPSParser_hpp

#include <stdio.h>
#include <iostream>

#include "BitBuffer.hpp"
#include "Golomb.hpp"

#define MAX_COUNT   32

namespace ffstl
{
    typedef enum {
        Unknown     = -1,     // Unknown
        YUV_400     =  0,     // 没有色度，只有亮度
        YUV_420     =  1,     // 4:2:0
        YUV_422     =  2,     // 4:2:2
        YUV_444     =  3      // 4:4:4
    } Color_Space_Format;
    
    /**
     hrd_parameters()
     @see Annex E E.1.2 HRD parameters syntax
     */
    typedef struct {
        int cpb_cnt_minus1;                                 // ue(v)
        int bit_rate_scale;                                 // u(4)
        int cpb_size_scale;                                 // u(4)
        int bit_rate_value_minus1[MAX_COUNT];               // ue(v)
        int cpb_size_value_minus1[MAX_COUNT];               // ue(v)
        int cbr_flag[MAX_COUNT];                            // u(1)
        int initial_cpb_removal_delay_length_minus1;        // u(5)
        int cpb_removal_delay_length_minus1;                // u(5)
        int dpb_output_delay_length_minus1;                 // u(5)
        int time_offset_length;                             // u(5)
    } hrd_parameters;
    
    /**
     vui_parameters()
     @see Annex E E.1.1 VUI parameters syntax
     */
    typedef struct {
        int aspect_ratio_info_present_flag;                 // u(1)
        int aspect_ratio_idc;                               // u(8)
        int sar_width;                                      // u(16)
        int sar_height;                                     // u(16)
        int overscan_info_present_flag;                     // u(1)
        int overscan_appropriate_flag;                      // u(1)
        int video_signal_type_present_flag;                 // u(1)
        int video_format;                                   // u(3)
        int video_full_range_flag;                          // u(1)
        int colour_description_present_flag;                // u(1)
        int colour_primaries;                               // u(8)
        int transfer_characteristics;                       // u(8)
        int matrix_coefficients;                            // u(8)
        
        int chroma_loc_info_present_flag;               // u(1)
        int chroma_sample_loc_type_top_field;           // ue(v)
        int chroma_sample_loc_type_bottom_field;        // ue(v)
        
        int timing_info_present_flag;                   // u(1)
        int num_units_in_tick;                          // u(32)
        int time_scale;                                 // u(32)
        int fixed_frame_rate_flag;                      // u(1)
        
        int nal_hrd_parameters_present_flag;            // u(1)
        hrd_parameters nal_hrd_parameters;
        
        int vcl_hrd_parameters_present_flag;            // u(1)
        hrd_parameters vcl_hrd_parameters;
        int low_delay_hrd_flag;                         // u(1)
        
        int pic_struct_present_flag;                    // u(1)
        int bitstream_restriction_flag;                 // u(1)
        int motion_vectors_over_pic_boundaries_flag;    // u(1)
        int max_bytes_per_pic_denom;                    // ue(v)
        int max_bits_per_mb_denom;                      // ue(v)
        int log2_max_mv_length_horizontal;              // ue(v)
        int log2_max_mv_length_vertical;                // ue(v)
        int max_num_reorder_frames;                     // ue(v)
        int max_dec_frame_buffering;                    // ue(v)
    } vui_parameters;
    
    /**
     Sequence Parameter Set
     @see 7.3.2.1 Sequence parameter set RBSP syntax
     */
    typedef struct
    {
		int8_t forbidden_zero_bit;							// f(1)
		int8_t nal_ref_idc;									// u(2)
		int8_t nal_unit_type;						    	// f(5)
		
        int8_t profile_idc;                                    // u(8)
		int8_t constraint_set0_flag;                           // u(1)
        int8_t constraint_set1_flag;                           // u(1)
        int8_t constraint_set2_flag;                           // u(1)
        int8_t constraint_set3_flag;                           // u(1)
        int8_t constraint_set4_flag;                           // u(1)
        int8_t constraint_set5_flag;                           // u(1)
        int8_t reserved_zero_2bits;                            // u(2)
        int8_t level_idc;                                      // u(8)
        int seq_parameter_set_id;                               // ue(v)
        int chroma_format_idc;                                  // ue(v)
        int8_t separate_colour_plane_flag;                         // u(1)
        int bit_depth_luma_minus8;                              // ue(v)
        int bit_depth_chroma_minus8;                            // ue(v)
        int8_t qpprime_y_zero_transform_bypass_flag;               // u(1)
        
        int8_t seq_scaling_matrix_present_flag;                    // u(1)
        int8_t seq_scaling_list_present_flag[12];                  // u(1)
        int ScalingList4x4[6][16];
        int UseDefaultScalingMatrix4x4Flag[6];
        int ScalingList8x8[6][64];
        int UseDefaultScalingMatrix8x8Flag[6];
        int log2_max_frame_num_minus4;                          // ue(v)
        int pic_order_cnt_type;                                 // ue(v)
        int log2_max_pic_order_cnt_lsb_minus4;                  // ue(v)
        int8_t delta_pic_order_always_zero_flag;                   // u(1)
        int offset_for_non_ref_pic;                             // se(v)
        int offset_for_top_to_bottom_field;                     // se(v)
        int num_ref_frames_in_pic_order_cnt_cycle;              // ue(v)
        int offset_for_ref_frame[256];                          // se(v)
        int max_num_ref_frames;                                 // ue(v)
        int8_t gaps_in_frame_num_value_allowed_flag;               // u(1)
        int pic_width_in_mbs_minus1;                            // ue(v)
        int pic_height_in_map_units_minus1;                     // ue(v)
        int8_t frame_mbs_only_flag;                                // u(1)
        int8_t mb_adaptive_frame_field_flag;                       // u(1)
        int8_t direct_8x8_inference_flag;                          // u(1)
        int8_t frame_cropping_flag;                                // u(1)
        int frame_crop_left_offset;                             // ue(v)
        int frame_crop_right_offset;                            // ue(v)
        int frame_crop_top_offset;                              // ue(v)
        int frame_crop_bottom_offset;                           // ue(v)
        
        int8_t vui_parameters_present_flag;                        // u(1)
        vui_parameters vui_parameters;
    } sps_t;
    
    class SPSParser
    {
    public:
        SPSParser(void* _pBuffer, size_t len)
        {
            ebsp2rbsp((const char*)_pBuffer,len);
            sps = std::make_shared<sps_t>();
			
			memset(sps.get(), 0, sizeof(sps_t));
        }
        
        void parser();
        void parse_sps();
        void parse_vui_parameters();
        void parse_hrd_parameters(hrd_parameters* hrd);
        void ebsp2rbsp(const char* _pBuffer, size_t& len);
        void scaling_list(int *scalingList, int sizeOfScalingList, int *useDefaultScalingMatrixFlag);
        
    protected:
        std::shared_ptr<sps_t> sps;
        std::shared_ptr<BitBuffer> buffer_;
    };
}

#endif /* SPSParser_hpp */

