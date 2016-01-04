function [f,x]=wavspectrum(filename)
% Computes spectrum of wave file filename

	[x,fs]=wavread(filename);
	N=numel(x);
	f=linspace(0,1,N)*fs;
	x=abs(fft(x)/N);
end
