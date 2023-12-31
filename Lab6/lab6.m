 % Lab 6: histogram
clear all; close all; clc;
% Octave 4.2.2: you might need to execute 'pkg load image' in the command line

% Reading image file and converting it to grayscale ([0,255])
  img_file = 'puppet.jpg';
  IM = rgb2gray(imread(img_file)); % IM is uint8.
  [sY, sX]  = size(IM);  % sY: # of rows, sX: # of columns
  
fname_i = 'puppet.bif'; % Input binary file
fname_o = 'puppet.bof'; % Output binary file

figure; imshow(IM,[]); title ('Input image');
  
disp ('ECE4900: Lab 6 - Image test');
disp ('1. Create .bif file that stores image in a binary file as a row vector');
disp ('2. Implement histogram');
disp ('3. Compare .bof file (generated) with the MATLAB processed image');
disp ('Any other number to exit');

while(1)
    method = input ('Select Operator (1-3)', 's');
    switch method
        case '1'
            B = reshape (IM', 1,sY*sX); % raster scan into row vector
            fid = fopen (fname_i, 'w'); % bif: binary image file
            fwrite (fid, B, 'uint8');
            fclose (fid);
            
        case '2'
            IV = double(IM(:));
            bn = hist (IV,256); % computing histogram values. 'hist' works for float,double
            figure; histogram(IV,256); title ('Histogram'); % plot histogram directly
            
            % Serial algorithm for histogram: bm = bn            
            bm = zeros(1,256);            
            % Naive approach (too slow):            
            %             for i = 1:256 % across bins
            %                 for j = 1:length(IV) % across data
            %                     if double(i-1) == IV(j)
            %                         bm(i) = bm(i) + 1;
            %                     end
            %                 end
            %             end            
            
            % More optimal approach: bm(IV(0)) = bm(IV(0)) + 1
            % Traverse all the elements of the image (values between 0 and
            % 255). If we come across a particular value, say 125, then we
            % increase the bin for 125.
            for j = 1:length(IV) % across data
                bm(IV(j)+1) = bm(IV(j)+1) + 1;
            end
            disp ('Difference between hist function and my MATLAB code:');
            disp (sum(abs(bm-bn))); % displays difference between MATLAB
                               % 'hist' function and my MATLAB code (bm)
            
        case '3'
            % Opening output file:
            %   Note that each element is interpreted as a 32-bit integer value.
              fod = fopen (fname_o, 'r');
              myhist = fread(fod, 256, 'int32=>int32');
              figure; plot (myhist); title ('histogram generated by C++ code');
              
            % Comparison between MATLAB histogram and your C++ code:
              diff = sum(abs(double(myhist) - bm')); disp ('Sum of differences (C++ vs MATLAB code):');
              disp (diff);
            
            break;
            
        otherwise
            disp ('unknown option, bye!'); break;
    end
end
