#include "header.h"

void scrambler(STREAM_T &inData, cn_STREAM &cnData, STREAM_T &goldenData, STREAM_T &outData)
{
#pragma HLS interface ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=inData
#pragma HLS INTERFACE axis port=cnData
#pragma HLS INTERFACE axis port=goldenData
#pragma HLS INTERFACE axis port=outData

	datau64b cnTemp = cnData.read();
	datau11b E = cnTemp.range(29,19);
	datau5b j;
	data_axi input,golden;


    datau5b burst = E/108;
    data_axi output;
    for(j=0;j<burst;j++){
		#pragma HLS PIPELINE
		#pragma HLS LOOP_TRIPCOUNT min=1 max=16
        input = inData.read();
        golden = goldenData.read();
        output.last = (j == burst-1);
        output.data = (input.data)^(golden.data);
        outData.write(output);
    }
}
