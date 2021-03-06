// megafunction wizard: %PARALLEL_ADD%VBB%
// GENERATION: STANDARD
// VERSION: WM1.0
// MODULE: parallel_add 

// ============================================================
// File Name: add4.v
// Megafunction Name(s):
// 			parallel_add
//
// Simulation Library Files(s):
// 			altera_mf
// ============================================================
// ************************************************************
// THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
//
// 10.1 Build 153 11/29/2010 SJ Full Version
// ************************************************************

//Copyright (C) 1991-2010 Altera Corporation
//Your use of Altera Corporation's design tools, logic functions 
//and other software and tools, and its AMPP partner logic 
//functions, and any output files from any of the foregoing 
//(including device programming or simulation files), and any 
//associated documentation or information are expressly subject 
//to the terms and conditions of the Altera Program License 
//Subscription Agreement, Altera MegaCore Function License 
//Agreement, or other applicable license agreement, including, 
//without limitation, that your use is for the sole purpose of 
//programming logic devices manufactured by Altera and sold by 
//Altera or its authorized distributors.  Please refer to the 
//applicable agreement for further details.

module add4 (
	data0x,
	data1x,
	data2x,
	data3x,
	result);

	input	[11:0]  data0x;
	input	[11:0]  data1x;
	input	[11:0]  data2x;
	input	[11:0]  data3x;
	output	[13:0]  result;

endmodule

// ============================================================
// CNX file retrieval info
// ============================================================
// Retrieval info: PRIVATE: INTENDED_DEVICE_FAMILY STRING "Cyclone IV E"
// Retrieval info: PRIVATE: SYNTH_WRAPPER_GEN_POSTFIX STRING "0"
// Retrieval info: LIBRARY: altera_mf altera_mf.altera_mf_components.all
// Retrieval info: CONSTANT: MSW_SUBTRACT STRING "NO"
// Retrieval info: CONSTANT: PIPELINE NUMERIC "0"
// Retrieval info: CONSTANT: REPRESENTATION STRING "UNSIGNED"
// Retrieval info: CONSTANT: RESULT_ALIGNMENT STRING "LSB"
// Retrieval info: CONSTANT: SHIFT NUMERIC "0"
// Retrieval info: CONSTANT: SIZE NUMERIC "4"
// Retrieval info: CONSTANT: WIDTH NUMERIC "12"
// Retrieval info: CONSTANT: WIDTHR NUMERIC "14"
// Retrieval info: USED_PORT: data0x 0 0 12 0 INPUT NODEFVAL "data0x[11..0]"
// Retrieval info: USED_PORT: data1x 0 0 12 0 INPUT NODEFVAL "data1x[11..0]"
// Retrieval info: USED_PORT: data2x 0 0 12 0 INPUT NODEFVAL "data2x[11..0]"
// Retrieval info: USED_PORT: data3x 0 0 12 0 INPUT NODEFVAL "data3x[11..0]"
// Retrieval info: USED_PORT: result 0 0 14 0 OUTPUT NODEFVAL "result[13..0]"
// Retrieval info: CONNECT: @data 0 0 12 0 data0x 0 0 12 0
// Retrieval info: CONNECT: @data 0 0 12 12 data1x 0 0 12 0
// Retrieval info: CONNECT: @data 0 0 12 24 data2x 0 0 12 0
// Retrieval info: CONNECT: @data 0 0 12 36 data3x 0 0 12 0
// Retrieval info: CONNECT: result 0 0 14 0 @result 0 0 14 0
// Retrieval info: GEN_FILE: TYPE_NORMAL add4.v TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL add4.inc FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL add4.cmp FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL add4.bsf FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL add4_inst.v FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL add4_bb.v TRUE
// Retrieval info: LIB_FILE: altera_mf
