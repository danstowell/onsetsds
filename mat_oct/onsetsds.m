function onsets = onsetsds(fftdata, odftype, medspan, srate)
% function onsets = onsetsds(fftdata)
% function onsets = onsetsds(fftdata, odftype)
% function onsets = onsetsds(fftdata, odftype, medspan)
% function onsets = onsetsds(fftdata, odftype, medspan, srate)
%
% Musical onset detector using adaptive whitening 
%     -- see http://onsetsds.sourceforge.net/
%
%  "fftdata": a matrix of FFT frames (1 per column) derived from an audio 
%             file. The common case (used for testing the code) is 44.1 kHz 
%             audio with 50% overlap and Hann windowing.
%
%  "odftype": integer specifying onset detection function to use:
%              0 'power', 
%              1 'magsum', 
%              2 'complex',
%              3 'rcomplex',  <-- default
%              4 'phase', 
%              5 'wphase', 
%              6 'mkl'.
%  "medspan": length of median-filter in frames, default is 11.
%  "srate": sampling rate of audio, default is 44100.
%
% For more info see D. Stowell and M. D. Plumbley, 
%         "Adaptive whitening for improved real-time audio onset detection", 
%         Proceedings of the International Computer Music Conference (ICMC'07), 
%         Copenhagen, Denmark, August 2007.
%
%   onsetsds is free software: you can redistribute it and/or modify
%   it under the terms of the GNU General Public License as published by
%   the Free Software Foundation, either version 2 of the License, or
%   (at your option) any later version.
