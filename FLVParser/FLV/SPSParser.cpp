//
//  SPSParser.cpp
//  FLVParser
//
//  Created by dKingbin on 2018/9/24.
//  Copyright © 2018年 dKingbin. All rights reserved.
//

#include "SPSParser.hpp"

namespace ffstl
{
    void SPSParser::parser()
    {
        parse_sps();
    }
    
    void SPSParser::parse_sps()
	{
		sps->forbidden_zero_bit = buffer_->readBit(1);
		sps->nal_ref_idc   = buffer_->readBit(2);
		sps->nal_unit_type = buffer_->readBit(5);

        sps->profile_idc = buffer_->readBit(8);
        sps->constraint_set0_flag = buffer_->readBit(1);   // constraint_set_flags[6]
		sps->constraint_set1_flag = buffer_->readBit(1);
		sps->constraint_set2_flag = buffer_->readBit(1);
		sps->constraint_set3_flag = buffer_->readBit(1);
		sps->constraint_set4_flag = buffer_->readBit(1);
		sps->constraint_set5_flag = buffer_->readBit(1);
		buffer_->readBit(2);	//reserved_zero[2]
        sps->level_idc = buffer_->readBit(8);

        sps->seq_parameter_set_id = Golomb::read_ue(buffer_);
        
        int profile_idc = sps->profile_idc;
        if (profile_idc == 100 || profile_idc == 110 || profile_idc == 122
            || profile_idc == 244 || profile_idc == 44 || profile_idc == 83
            || profile_idc == 86 || profile_idc == 118 || profile_idc == 128
            || profile_idc == 138 || profile_idc == 144)
        {
            sps->chroma_format_idc = Golomb::read_ue(buffer_);
            if(sps->chroma_format_idc == YUV_444)
            {
                sps->separate_colour_plane_flag = buffer_->readBit(1);
            }
            
            sps->bit_depth_luma_minus8 = Golomb::read_ue(buffer_);
            sps->bit_depth_chroma_minus8 = Golomb::read_ue(buffer_);
            sps->qpprime_y_zero_transform_bypass_flag = buffer_->readBit(1);
            sps->seq_scaling_matrix_present_flag = buffer_->readBit(1);
            
            if(sps->seq_scaling_matrix_present_flag)
            {
                int scaling_list_count = (sps->chroma_format_idc != YUV_444) ? 8 : 12;
                for(int i=0;i<scaling_list_count;i++)
                {
                    sps->seq_scaling_list_present_flag[i] = buffer_->readBit(1);
                    if(i<6)
                    {
                        scaling_list(sps->ScalingList4x4[i],16,&(sps->UseDefaultScalingMatrix4x4Flag[i]));
                    }
                    else
                    {
                        scaling_list(sps->ScalingList8x8[i-6],64,&(sps->UseDefaultScalingMatrix8x8Flag[i-6]));
                    }
                }
            }
        }
		else
		{
			sps->chroma_format_idc       = YUV_420;
			sps->bit_depth_luma_minus8   = 0;
			sps->bit_depth_chroma_minus8 = 0;
		}
        
        sps->log2_max_frame_num_minus4 = Golomb::read_ue(buffer_);
        sps->pic_order_cnt_type = Golomb::read_ue(buffer_);
        if(sps->pic_order_cnt_type == 0)
        {
            sps->log2_max_pic_order_cnt_lsb_minus4 = Golomb::read_ue(buffer_);
        }
        else if(sps->pic_order_cnt_type == 1)
        {
            sps->delta_pic_order_always_zero_flag = buffer_->readBit(1);
            sps->offset_for_non_ref_pic = Golomb::read_se(buffer_);
            sps->offset_for_top_to_bottom_field = Golomb::read_se(buffer_);
            sps->num_ref_frames_in_pic_order_cnt_cycle = Golomb::read_ue(buffer_);
            
            for(int i=0;i<sps->num_ref_frames_in_pic_order_cnt_cycle;i++)
            {
                sps->offset_for_ref_frame[i] = Golomb::read_se(buffer_);
            }
        }
        
        sps->max_num_ref_frames = Golomb::read_ue(buffer_);
        sps->gaps_in_frame_num_value_allowed_flag = buffer_->readBit(1);
        
        sps->pic_width_in_mbs_minus1 = Golomb::read_ue(buffer_);
        sps->pic_height_in_map_units_minus1 = Golomb::read_ue(buffer_);
        sps->frame_mbs_only_flag = buffer_->readBit(1);
        if (!sps->frame_mbs_only_flag)
        {
            sps->mb_adaptive_frame_field_flag = buffer_->readBit(1);
        }
        
        sps->direct_8x8_inference_flag = buffer_->readBit(1);
        
        sps->frame_cropping_flag = buffer_->readBit(1);
        if (sps->frame_cropping_flag)
        {
            sps->frame_crop_left_offset = Golomb::read_ue(buffer_);
            sps->frame_crop_right_offset = Golomb::read_ue(buffer_);
            sps->frame_crop_top_offset = Golomb::read_ue(buffer_);
            sps->frame_crop_bottom_offset = Golomb::read_ue(buffer_);
        }
        
        sps->vui_parameters_present_flag = buffer_->readBit(1);
        if (sps->vui_parameters_present_flag)
        {
            parse_vui_parameters();
        }
    }
    
    void SPSParser::parse_vui_parameters()
    {
        static int sar_w_table[16] = {1, 12, 10, 16, 40, 24, 20, 32, 80, 18, 15, 64, 160, 4, 3, 2};
        static int sar_h_table[16] = {1, 11, 11, 11, 33, 11, 11, 11, 33, 11, 11, 33,  99, 3, 2, 1};
        
        sps->vui_parameters.aspect_ratio_info_present_flag = buffer_->readBit(1);
        if (sps->vui_parameters.aspect_ratio_info_present_flag)
        {
            sps->vui_parameters.aspect_ratio_idc = buffer_->readBit(8);
            if(sps->vui_parameters.aspect_ratio_idc > 0
               && sps->vui_parameters.aspect_ratio_idc < 16)
            {
                int index = sps->vui_parameters.aspect_ratio_idc - 1;
                sps->vui_parameters.sar_width  = sar_w_table[index];
                sps->vui_parameters.sar_height = sar_h_table[index];
            }
            else if (sps->vui_parameters.aspect_ratio_idc == 255)
            {
				sps->vui_parameters.sar_width = buffer_->readBit(16);
                sps->vui_parameters.sar_height = buffer_->readBit(16);
            }
        }
        
        sps->vui_parameters.overscan_info_present_flag = buffer_->readBit(1);
        if (sps->vui_parameters.overscan_info_present_flag)
        {
            sps->vui_parameters.overscan_appropriate_flag = buffer_->readBit(1);
        }
        
        sps->vui_parameters.video_signal_type_present_flag = buffer_->readBit(1);
        if (sps->vui_parameters.video_signal_type_present_flag)
        {
            sps->vui_parameters.video_format = buffer_->readBit(3);
            sps->vui_parameters.video_full_range_flag = buffer_->readBit(1);
            
            sps->vui_parameters.colour_description_present_flag = buffer_->readBit(1);
            if (sps->vui_parameters.colour_description_present_flag)
            {
                sps->vui_parameters.colour_primaries = buffer_->readBit(8);
                sps->vui_parameters.transfer_characteristics = buffer_->readBit(8);
                sps->vui_parameters.matrix_coefficients = buffer_->readBit(8);
            }
        }
        
        sps->vui_parameters.chroma_loc_info_present_flag = buffer_->readBit(1);
        if (sps->vui_parameters.chroma_loc_info_present_flag)
        {
            sps->vui_parameters.chroma_sample_loc_type_top_field = Golomb::read_ue(buffer_);
            sps->vui_parameters.chroma_sample_loc_type_bottom_field = Golomb::read_ue(buffer_);
        }
        
        sps->vui_parameters.timing_info_present_flag = buffer_->readBit(1);
        if (sps->vui_parameters.timing_info_present_flag)
        {
            sps->vui_parameters.num_units_in_tick = buffer_->readBit(32);
            sps->vui_parameters.time_scale = buffer_->readBit(32);
            sps->vui_parameters.fixed_frame_rate_flag = buffer_->readBit(1);
            
            /*
             fps_num = time_scale;
             fps_den = num_units_in_tick * 2;
             fps = fps_num / fps_den;
             */
        }
        
        sps->vui_parameters.nal_hrd_parameters_present_flag = buffer_->readBit(1);
        if (sps->vui_parameters.nal_hrd_parameters_present_flag)
        {
            parse_hrd_parameters(&sps->vui_parameters.nal_hrd_parameters);
        }
        
        sps->vui_parameters.vcl_hrd_parameters_present_flag = buffer_->readBit(1);
        if (sps->vui_parameters.vcl_hrd_parameters_present_flag)
        {
            parse_hrd_parameters(&sps->vui_parameters.vcl_hrd_parameters);
        }
        
        if (sps->vui_parameters.nal_hrd_parameters_present_flag
            || sps->vui_parameters.vcl_hrd_parameters_present_flag)
        {
            sps->vui_parameters.low_delay_hrd_flag = buffer_->readBit(1);
        }
        
        sps->vui_parameters.pic_struct_present_flag = buffer_->readBit(1);
        sps->vui_parameters.bitstream_restriction_flag = buffer_->readBit(1);
        if (sps->vui_parameters.bitstream_restriction_flag)
        {
            sps->vui_parameters.motion_vectors_over_pic_boundaries_flag = buffer_->readBit(1);
            sps->vui_parameters.max_bytes_per_pic_denom       = Golomb::read_ue(buffer_);
            sps->vui_parameters.max_bits_per_mb_denom         = Golomb::read_ue(buffer_);
            sps->vui_parameters.log2_max_mv_length_horizontal = Golomb::read_ue(buffer_);
            sps->vui_parameters.log2_max_mv_length_vertical   = Golomb::read_ue(buffer_);
            sps->vui_parameters.max_num_reorder_frames        = Golomb::read_ue(buffer_);
            sps->vui_parameters.max_dec_frame_buffering       = Golomb::read_ue(buffer_);
        }
    }
    
    void SPSParser::parse_hrd_parameters(hrd_parameters* hrd)
    {
        hrd->cpb_cnt_minus1 = Golomb::read_ue(buffer_);
        hrd->bit_rate_scale = buffer_->readBit(4);
        hrd->cpb_size_scale = buffer_->readBit(4);
        
        for (int SchedSelIdx = 0; SchedSelIdx <= hrd->cpb_cnt_minus1; SchedSelIdx++)
        {
            hrd->bit_rate_value_minus1[SchedSelIdx] = Golomb::read_ue(buffer_);
            hrd->cpb_size_value_minus1[SchedSelIdx] = Golomb::read_ue(buffer_);
            hrd->cbr_flag[SchedSelIdx] = buffer_->readBit(1);
        }
        
        hrd->initial_cpb_removal_delay_length_minus1 = buffer_->readBit(5);
        hrd->cpb_removal_delay_length_minus1 = buffer_->readBit(5);
        hrd->dpb_output_delay_length_minus1 = buffer_->readBit(5);
        hrd->time_offset_length = buffer_->readBit(5);
    }
    
    void SPSParser::ebsp2rbsp(const char* _pBuffer, size_t& len)
    {
        //remove 0x03 from 0x00 0x00 0x03
        char* rbsp = new char[len];
        
        int index = 0;
        int count = 0;
        while (true)
        {
			if(count >= len-2) break;

			if(_pBuffer[count] == 0x00
			   && _pBuffer[count+1] == 0x00
			   && _pBuffer[count+2] == 0x03)
			{
				rbsp[index++] = _pBuffer[count++];
				rbsp[index++] = _pBuffer[count++];
				count++;
			}
			else
			{
				rbsp[index++] = _pBuffer[count++];
			}
        }

        buffer_ = std::make_shared<BitBuffer>(rbsp,index);	//x/19bx 
		len = index;
    }
    
    void SPSParser::scaling_list(int *scalingList, int sizeOfScalingList, int *useDefaultScalingMatrixFlag)
    {
        int deltaScale = 0;
        int lastScale = 8;
        int nextScale = 8;
        
        for (int j = 0; j < sizeOfScalingList; j++)
        {
            if (nextScale != 0)
            {
                deltaScale = Golomb::read_se(buffer_);
                nextScale = (lastScale + deltaScale + 256) % 256;
                *useDefaultScalingMatrixFlag = (j == 0 && nextScale == 0);
            }
            
            scalingList[j] = (nextScale == 0) ? lastScale : nextScale;
            lastScale = scalingList[j];
        }
    }
}

