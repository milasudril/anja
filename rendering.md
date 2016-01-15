Rendering pipeline
==================

 1. Generate waveform data from all active slots. This step includes playback
    gain, and speed adjustments. Data is written to a temporary buffer `a`

 2. Mix data from `a` with the data in the corresponding output channel.

 3. Mix all channels. Channel gain/expression probably needs to be evaluated per
    frame (interpolation needed?). If there is one JACK port per channel, copy
    data without mixing.

