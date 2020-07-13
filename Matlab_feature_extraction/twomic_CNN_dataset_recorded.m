%%
% Created by Abdullah Kucuk
% Generates CNN dataset for two mic DOA for Recorded Data
% Nov 16th 2018

%%Real Imag 

function [output]=twomic_CNN_dataset_recorded(noisysig,fs,L,GT);



x = noisysig(:,1); 
h = noisysig(:,2); % Noisy speech


n = round(L*fs);
l=length(x);
n_F = round(l/n);




NFFT = 2^nextpow2(n);



n_F = round(l/n);
win = hamming(n);

x_frame = zeros(n,n_F);
h_frame = zeros(n,n_F);
resolution=20;


for i = 1:n_F-1
  


%     tic;
    x_frame = win.*x((i-1)*n+1:i*n);
    h_frame = win.*h((i-1)*n+1:i*n);
   

    fft_Framex =( fft(x_frame,NFFT));
    fft_Frameh = fft(h_frame,NFFT);
    
  
    imag_fft_Framex(:,i)=((imag(fft_Framex)));%
    real_fft_Framex(:,i)=((real(fft_Framex)));%
    imag_fft_Frameh(:,i)=(imag(fft_Frameh));%
    real_fft_Frameh(:,i)=(real(fft_Frameh));%

    

  
    
end


 %% Each CNN input is 10 frame* NFFT points image

 
 imag_fft_Framex=imag_fft_Framex';%
 real_fft_Framex=real_fft_Framex';%
 imag_fft_Frameh=imag_fft_Frameh';%
 real_fft_Frameh=real_fft_Frameh';%
   
 chck_val=10*floor((n_F-1)/10); % since I try to bundle 10 frames as a image for estimation, dimension should be 10x

 
output=[imag_fft_Framex(1:chck_val,1:NFFT/2+1),imag_fft_Frameh(1:chck_val,1:NFFT/2+1),real_fft_Framex(1:chck_val,1:NFFT/2+1),real_fft_Frameh(1:chck_val,1:NFFT/2+1),GT(1:chck_val,:) ];

 
%  % Plot
%  len_xx=length(GT_modified);
% t=(0:len_xx-1).*L/1000;
% f=(1:NFFT).*((fs/NFFT/2));
% 
% surface(t,f,fft_xx')
% view(2)
% xlabel('Time in seconds')
% ylabel('Frequency in Hz')
% h=colorbar
% ylabel(h,'Magnitude in dB')
% title('LPC Spectrum');
% 
% figure;
% spectrogram(x,win,0,NFFT,fs,'yaxis');
end