#include "header.h"

void RateMatchPDCCH(STREAM_in &inData, cn_STREAM &cnData, STREAM_out &outData){
#pragma HLS INTERFACE axis port = inData
#pragma HLS INTERFACE axis port = outData
#pragma HLS INTERFACE axis port = cnData
#pragma HLS interface ap_ctrl_none port=return

	datau64b config = cnData.read();
	int K, N, E, a, b, i = 0;
	K = config.range(7, 0) + 24;
	N = config.range(18, 8);
	E = config.range(29, 19);

	a = (N+127)/128;
	b = (E+107)/108;

	datau512b temp = 0;
	data_axi_108 out_temp;
	out_temp.data = 0;
	out_temp.last = 0;

	for(i = 0; i < a; i++)
	{
#pragma HLS UNROLL
#pragma HLS LOOP_TRIPCOUNT min = 1 max = 4
		temp.range(511-128*i, 384 - 128*i) =  inData.read().data;
	}

	int burst;
	if(a > b){
		burst = b;
	}
	else{
		burst = a;
	}

	if(16*K > 7*E | E > N){
		for(i = 0; i < burst; i++){
#pragma HLS UNROLL
#pragma HLS LOOP_TRIPCOUNT min = 1 max = 4
				out_temp.data = temp.range(511 - 108*i, 404 - 108*i);
				if(i == burst-1 && E <= N){
					out_temp.last = 1;
				}
				outData.write(out_temp);
		}
	}

	else{
		for(i = 0; i < b; i++)
		{
#pragma HLS UNROLL
#pragma HLS LOOP_TRIPCOUNT min = 1 max = 4
			out_temp.data = temp.range(511 - 20*b - 108*i, 404 - 20*b - 108*i);
			if(i ==  b-1){
				out_temp.last = 1;
			}
			outData.write(out_temp);
		}
	}

	datau108b temp1;
	if(E > N){
		temp1 = temp.range(79, 0);
		out_temp.data = (temp1 << 28) + temp.range(511, 484);
		outData.write(out_temp);
		temp1 = 0;
		for(i = 0; i < 3; i++){
#pragma HLS UNROLL
#pragma HLS LOOP_TRIPCOUNT min = 3 max = 3
			out_temp.data = temp.range(483 - 108*i, 376 - 108*i);
			if(i == 2 && E != 1728){
				out_temp.last = 1;
			}
			outData.write(out_temp);
		}
		if(E == 1728){
			out_temp.data = temp.range(159, 52);
			outData.write(out_temp);
			temp1 = temp.range(51, 0);
			out_temp.data = (temp1 << 56) + temp.range(511, 456);
			outData.write(out_temp);
			temp1 = 0;
			for(i = 0; i < 4; i++){
#pragma HLS UNROLL
#pragma HLS LOOP_TRIPCOUNT min = 4 max = 4
				out_temp.data = temp.range(455 - 108*i, 348 - 108*i);
				outData.write(out_temp);
			}
			temp1 = temp.range(23, 0);
			out_temp.data = (temp1 << 84) + temp.range(511, 428);
			outData.write(out_temp);
			out_temp.data = temp.range(427, 320);
			out_temp.last = 1;
			outData.write(out_temp);
		}
	}
}
