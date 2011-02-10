/**
    This file is part of "onsetsds".
    (c) 2011 Dan Stowell and Queen Mary University of London
    All rights reserved.

    onsetsds is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    onsetsds is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with kdpee.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "math.h"
#include "mex.h"
#include "onsetsds.h"

////////////////////////////////////////////////////////////////////////
// MATLAB ENTRY FUNCTION:
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// implementation of: onsetsds(fftdata, odftype, medspan, srate)
	
	// Get the star of the show, the fft data
	const mxArray *fftdata;	
	fftdata = prhs[0];
	if(!mxIsComplex(fftdata)){
    	mexErrMsgTxt("onsetsds expects a matrix of complex data (e.g. from fft()).\n");
	}
	const size_t fftframesize = mxGetM(fftdata);
	const size_t numframes    = mxGetN(fftdata);
	
	const double* real = mxGetPr(fftdata);
	const double* imag = mxGetPi(fftdata);
	
	// optional args
	int odftype = ODS_ODF_RCOMPLEX;
	if(nrhs>1){
		odftype = mxGetScalar(prhs[1]);
		if((odftype < 0) || (newodftype > 6))
    		mexErrMsgTxt("onsetsds: unrecognised ODF type.\n");
	}
	const unsigned int medspan
	                  = (nrhs<3) ? 11 : (unsigned int) mxGetScalar(prhs[2]);
	const float srate = (nrhs<4) ? 44100.f : (float) mxGetScalar(prhs[3]);
	
	// set up the ods
	OnsetsDS ods;
	float* odsdata = (float*) malloc( onsetsds_memneeded(odftype, fftframesize, medspan) );
	onsetsds_init(&ods, odsdata, ODS_FFT_SC3_COMPLEX, odftype, fftframesize, medspan, srate);
	
	// This is where we'll repack an individual frame
	float* packeddata = malloc(fftframesize * sizeof(float));
	
	// set up the output array, size numframes
	double *results = (double*) mxCalloc(numframes, sizeof(double));
	if(!results){
    	mexErrMsgTxt("onsetsds error, could not create results array (out of memory?).\n");
	}
	
	////////////////////////////////////////////////////////////////////////////
	// iterate the frames, converting to expected format and onset-detecting
	size_t i;
	for(i=0; i<numframes; ++i){
		// copy the data into an array in ODS_FFT_SC3_COMPLEX format.
		// note that here we're discarding stuff above nyquist, since assuming real-valued input.
		size_t frameoffset = fftframesize * i;
		packeddata[0] = (float) real[frameoffset];
		packeddata[1] = (float) real[frameoffset + fftframesize / 2];
		size_t j;
		for(j=1; j< fftframesize/2; ++j){
			packeddata[j+j]   = (float) real[frameoffset + j];
			packeddata[j+j+1] = (float) imag[frameoffset + j];
		}
		bool onset = onsetsds_process(&ods, packeddata);
		results[i] = (onset ? 1.0 : 0.0);
	}	
	
	// Copy the results from double-array into matlab format
	mxArray *resultsM = mxCreateDoubleMatrix(1,numframes,mxREAL);
	memcpy(mxGetPr(resultsM), results, numframes * sizeof(double));
	plhs[0] = resultsM;
	
	// destroy the ods correctly, and any matlabby things that need to go too.
	free(ods.data);
	free(packeddata);
	mxFree(results);
}
