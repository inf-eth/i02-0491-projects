filename = '../tmp/received_samples.iq';  % or .bin or whatever you saved
filename = '../tmp/bladerf_iq_pipe';

Channels = 1;
fs = 8e5;%bitshift(1,20);
DurationMultiplier = 5;
numSamplesRcv = fs*DurationMultiplier;
DownSamplingFactor = 10;
fs = fs/DownSamplingFactor;
numSamples = fs*DurationMultiplier;%bitshift(1,16);%fs/1;%bitshift(1,19);%4096;
fTone = 5000;

% Frequency sweep
fSweep = 0;
fRange = (100:100:6000)*1e6;
i__=1;

EnvelopeSamplingFactor = 10;   % Envelope sampling factor
MinPeakProminence = 0.8;
% Interpolation method: 'linear', 'squared', 'nearest', 'pchip','cubic', 'makima', or 'spline'
InterpMethod = 'makima';
t_frames = 1;%bitshift(1,2);
numFreqPoints = 1*150*(numSamples/fs);
numIQt = 1;
PlotRange = [-120, -20]; %[0, 1.5e-1]; %[-100, 0] for dB, %[60, 100] scaled dB
FreqRange = (t_frames*numSamples)/2+(1:+numFreqPoints);
FreqRangeIQt = (t_frames*numSamples*numIQt)/2+(1:(numFreqPoints*numIQt));
IQData = complex(zeros(t_frames*numSamples,1));
IData = zeros(t_frames*numSamples,1);
QData = zeros(t_frames*numSamples,1);
ItData = zeros(t_frames*numSamples,1);
QtData = zeros(t_frames*numSamples,1);

IQtData = zeros(t_frames*numSamples*numIQt,1);
FlatScaler = 0.5;
linearScaler = 1:(t_frames*numSamples);
len = length(linearScaler);
for i=1:len
    linearScaler(i) = (len-i+1)/len;
end
if fSweep==1
    spec_data_Sweep = 1e-100*ones(length(fRange), numFreqPoints);
end
%load('CustomJetColormap.mat');
%Downconversion 'carrier'
Ac = 0.5;
fc = 5000;
Ic = Ac.*sin(2.*pi.*fc.*(0:(numSamples-1))./numSamples)';
Qc = Ac.*cos(2.*pi.*fc.*(0:(numSamples-1))./numSamples)';

% Frequency axis for plotting
f = linspace(-fs/2, fs/2-1/t_frames/(numSamples/fs), t_frames*numSamples);
fIQt = linspace(-fs/2, fs/2-1/numIQt/t_frames/(numSamples/fs), t_frames*numSamples*numIQt);
frame_history = 50;     % How many FFT frames to show in the waterfall
spec_data = 1e-100*ones(frame_history, numFreqPoints);  % Time x Frequency matrix
spec_dataIQt = 1e-100*ones(frame_history, numFreqPoints*numIQt);  % Time x Frequency matrix


% Set up fft figure
figure(3);
subplot(211)
hPlot = plot(f(FreqRange), zeros(1, numFreqPoints));
hold on
hPlot1 = plot(f(FreqRange), zeros(1, numFreqPoints));
hPlot2 = plot(f(FreqRange), zeros(1, numFreqPoints));
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Real-Time Spectrum');
%ylim([-100, 100]);
ylim(PlotRange);
grid on;
hold off
subplot(212)
hPlotIQt = plot(fIQt(FreqRangeIQt), zeros(1, numFreqPoints*numIQt));
hold on
hPlotIQtrms = plot(fIQt(FreqRangeIQt), zeros(1, numFreqPoints*numIQt));
if Channels==2
    hPlotIQ1t = plot(fIQt(FreqRangeIQt), zeros(1, numFreqPoints*numIQt));
    hPlotIQ1trms = plot(fIQt(FreqRangeIQt), zeros(1, numFreqPoints*numIQt));
end
hold off
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Real-Time Spectrum');
%ylim([-100, 100]);
ylim(PlotRange);
grid on;

% Set up figure
figure(4);
subplot(211)
hImg = imagesc(f(FreqRange), 1:frame_history, spec_data);
xlabel('Frequency (Hz)');
ylabel('Time (Frames)');
title('Real-Time Spectrogram');
colorbar;
%caxis([-100, 100]);  % Adjust as needed
clim(PlotRange);  % Adjust as needed
colormap jet;
axis xy;
subplot(212)
hImgIQt = imagesc(fIQt(FreqRangeIQt), 1:frame_history, spec_dataIQt);
xlabel('Frequency (Hz)');
ylabel('Time (Frames)');
title('Real-Time Spectrogram');
colorbar;
%caxis([-100, 100]);  % Adjust as needed
clim(PlotRange);  % Adjust as needed
colormap jet;
axis xy;


if fSweep == 1
    % Set up figure
    figure(5);
    hImgSweep = imagesc(f(FreqRange), fRange./1e6, spec_data_Sweep);
    xlabel('Frequency (Hz)');
    ylabel('Carrier Frequency (MHz)');
    title('Real-Time Spectrogram Frequency Sweep');
    colorbar;
    %caxis([-100, 100]);  % Adjust as needed
    clim(PlotRange);  % Adjust as needed
    colormap jet;
    axis xy;
end

while true
    if fSweep == 1
        if i__>length(fRange)
            return;
        end
        freqFile = '../tmp/'+string(fRange(i__));
        fid = fopen(freqFile, 'rb');
        if fid == -1
            error('Failed to open file.');
        end
        freqSweepData = fread(fid, 'float32');  % Read all float32 data
        freqSweepData = reshape(freqSweepData(1:Channels * 2*numSamplesRcv), Channels * 2, []);
        fclose(fid);
    end
    fid = fopen(filename, 'rb');
    if fid == -1
        error('Failed to open file.');
    end
    data = fread(fid, 'float32');  % Read all float32 data
    fclose(fid);

    if length(data) < Channels * 2 * numSamplesRcv
        pause(0.01);
        continue;
    end

    data = reshape(data(1:Channels * 2*numSamplesRcv), Channels * 2, []);
    % Separate I and Q
    I = data(1,:)';%data(1:2:end); %I = I-mean(I);
    Q = data(2,:)';%data(2:2:end); %Q = Q-mean(Q);
    I = downsample(I,DownSamplingFactor);
    Q = downsample(Q,DownSamplingFactor);
    Iorg = I;
    Qorg = Q;

    if Channels==2
        I1 = data(3,:)';%data(1:2:end); %I = I-mean(I);
        Q1 = data(4,:)';%data(2:2:end); %Q = Q-mean(Q);
        I1 = downsample(I1,DownSamplingFactor);
        Q1 = downsample(Q1,DownSamplingFactor);
        I1org = I1;
        Q1org = Q1;
    end

    if fSweep==1
         % Separate I and Q
        Ifs = freqSweepData(1,:)'; %I = I-mean(I);
        Qfs = freqSweepData(2,:)'; %Q = Q-mean(Q);
        Ifs = downsample(Ifs,DownSamplingFactor);
        Qfs = downsample(Qfs,DownSamplingFactor);
        Iorgfs = Ifs;
        Qorgfs = Qfs;
    end

    % Reference: https://dsp.stackexchange.com/questions/74677/envelope-detection-using-matlab-hilbert-function
    if 0
    temp = fft(Iorg);
    temp(numSamples/2+1:end) = 0;
    temp(2:numSamples/2-1) = 2*temp(2:numSamples/2-1);
    I = abs(ifft(temp));
    temp = fft(Qorg);
    temp(numSamples/2+1:end) = 0;
    temp(2:numSamples/2-1) = 2*temp(2:numSamples/2-1);
    Q = abs(ifft(temp));
    end

    if 1
    nh = 525;
    [ahI,alI] = envelope(I,nh,'analytic');
    [ahQ,alQ] = envelope(Q,nh,'analytic');
    [phI,plI] = envelope(I,nh,'peak');
    [phQ,plQ] = envelope(Q,nh,'peak');
    [rhI,rlI] = envelope(I,nh,'rms');
    [rhQ,rlQ] = envelope(Q,nh,'rms');
    %I = ahI+phI+rhI;%ahI-alI+phI-plI+rhI-rlI;
    %Q = ahQ+phQ+rhQ;%ahQ-alQ+phQ-plQ+rhQ-rlQ;
    I=I-mean(I);
    Q=Q-mean(Q);
    end

    a = 2*abs(hilbert(Iorg));
    a = a-mean(a);
    rhI = rhI-mean(rhI); rlI = rlI-mean(rlI); rhQ = rhQ-mean(rhQ); rlQ = rlQ-mean(rlQ);
    %%%%IQtData = [rhQ;rhQ;rlI;rlQ;rhI];
    %IQtData = IQtData-mean(IQtData);
    %nh = 525;
    %[rhI,rlI] = envelope(I,nh,'rms');
    %[rhQ,rlQ] = envelope(Q,nh,'rms');
    IQtEnvelope = rhI+rlI+rhQ+rlQ;
    IQtEnvelope = IQtEnvelope-mean(IQtEnvelope);

    %I = I.*Ic;
    %Q = Q.*Qc;
    %I = lowpass(I,150,fs);
    %Q = lowpass(Q,150,fs);
    %%I = amdemod(Iorg,fTone,fs,0,0);
    %%Q = amdemod(Qorg,fTone,fs,0,0);

    %[I, ~] = findpeaks(I,"NPeaks",1);
    %[I,~] = findpeaks(Iorg,'MinPeakDistance',fs/fTone);
    %[Q,~] = findpeaks(Qorg,'MinPeakDistance',fs/fTone);

    figure(5);
    %findpeaks(Iorg,'MinPeakDistance',fs/fTone);
    [pks,~] = findpeaks(Iorg,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
    Ac = mean(pks);
    %PhaseC = phase(Iorg(1)+1j*Qorg(1));
    IPhaseIndex = GetPhaseIndex(Iorg,numSamples);
    ICarrier = Ac.*sin(2.*pi.*fTone.*((1:numSamples)-IPhaseIndex-0.5)./numSamples)';
    QPhaseIndex = GetPhaseIndex(Qorg,numSamples);
    QCarrier = Ac.*sin(2.*pi.*fTone.*((1:numSamples)-QPhaseIndex-0.5)./numSamples)';
    Ids = Iorg.*ICarrier;
    Ids = (Iorg-ICarrier).*ICarrier;
    Ids = lowpass(Ids,100,fs,'Steepness',0.95);
    Ids = abs(Iorg)-abs(ICarrier);
    Ids = Ids-mean(Ids);
    %Ids = lowpass(Ids,100,fs,'Steepness',0.95);
    Qds = abs(Qorg)-abs(QCarrier);
    Qds = Qds-mean(Qds);
    %Qds = lowpass(Qds,100,fs,'Steepness',0.95);
    Ipds = Iorg; Ipds(Ipds<0) = 0;
    Qpds = Qorg; Qpds(Qpds<0) = 0;
    IpCarrier = ICarrier; IpCarrier(IpCarrier<0) = 0;
    QpCarrier = QCarrier; QpCarrier(QpCarrier<0) = 0;
    Ipds = Ipds-IpCarrier; Ipds = Ipds-mean(Ipds);
    Qpds = Qpds-QpCarrier; Qpds = Qpds-mean(Qpds);

    Inds = Iorg; Inds(Inds>0) = 0;
    Qnds = Qorg; Qnds(Qnds>0) = 0;
    InCarrier = ICarrier; InCarrier(InCarrier>0) = 0;
    QnCarrier = QCarrier; QnCarrier(QnCarrier>0) = 0;
    Inds = Inds-InCarrier; Inds = Inds-mean(Inds);
    Qnds = Qnds-QnCarrier; Qnds = Qnds-mean(Qnds);

    IQphase = angle(complex(Iorg,Qorg))-angle(complex(ICarrier,QCarrier));
    IQamp = abs(complex(Iorg,Qorg))-abs(complex(ICarrier,QCarrier));
    IQrecovered = 100/DownSamplingFactor/DurationMultiplier.*Ac.*IQamp.*sin(IQphase);
    IQrecovered = IQrecovered-mean(IQrecovered);

    if Channels==2
        [pks,~] = findpeaks(I1org,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
        Ac = mean(pks);
        %PhaseC = phase(Iorg(1)+1j*Qorg(1));
        I1PhaseIndex = GetPhaseIndex(I1org,numSamples);
        I1Carrier = Ac.*sin(2.*pi.*fTone.*((1:numSamples)-I1PhaseIndex-0.5)./numSamples)';
        Q1PhaseIndex = GetPhaseIndex(Q1org,numSamples);
        Q1Carrier = Ac.*sin(2.*pi.*fTone.*((1:numSamples)-Q1PhaseIndex-0.5)./numSamples)';
        I1ds = abs(I1org)-abs(I1Carrier);
        I1ds = I1ds-mean(I1ds);
        %Ids = lowpass(Ids,100,fs,'Steepness',0.95);
        Q1ds = abs(Q1org)-abs(Q1Carrier);
        Q1ds = Q1ds-mean(Q1ds);

        IQ1phase = angle(complex(I1org,Q1org))-angle(complex(I1Carrier,Q1Carrier));
        IQ1amp = abs(complex(I1org,Q1org))-abs(complex(I1Carrier,Q1Carrier));
        IQ1recovered = 100/DownSamplingFactor/DurationMultiplier.*Ac.*IQ1amp.*sin(IQ1phase);
        IQ1recovered = IQ1recovered-mean(IQ1recovered);
    end

    subplot(311)
    plot(Iorg)
    hold on
    plot(Ids)
    plot(ICarrier)
    ylim([-0.7 0.7])
    hold off
    subplot(312)
    plot(Qorg)
    hold on
    plot(Qds)
    plot(QCarrier)
    ylim([-0.7 0.7])
    hold off
    subplot(313)
    fftIds = 20.*log10(abs(fftshift(fft(Ids)))./numSamples+1e-6);
%    plot(f(FreqRange),fftIds(FreqRange))
    ylim(PlotRange)

    % [pks,locs] = findpeaks(Iorg,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
    % [pks,locs] = PadLocs(pks,locs,numSamples);
    % I = interpn(locs,pks,1:numSamples,InterpMethod)';
    % [pks,locs] = findpeaks(-Iorg,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
    % [pks,locs] = PadLocs(pks,locs,numSamples);
    % It = interpn(locs,pks,1:numSamples,InterpMethod)';
    I = GetInterpolatedSignalEnvelope(Iorg,InterpMethod,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples);
    It = GetInterpolatedSignalEnvelope(-Iorg,InterpMethod,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples);
    %I = I+It;

    % [pks,locs] = findpeaks(Qorg,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
    % [pks,locs] = PadLocs(pks,locs,numSamples);
    % Q = interpn(locs,pks,1:numSamples,InterpMethod)';
    % [pks,locs] = findpeaks(-Qorg,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
    % [pks,locs] = PadLocs(pks,locs,numSamples);
    % Qt = interpn(locs,pks,1:numSamples,InterpMethod)';
    Q = GetInterpolatedSignalEnvelope(Qorg,InterpMethod,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples);
    Qt = GetInterpolatedSignalEnvelope(-Qorg,InterpMethod,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples);
    %Q = Q+Qt;

    if fSweep==1
        [pks,locs] = findpeaks(Iorgfs,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
        [pks,locs] = PadLocs(pks,locs,numSamples);
        Ifs = interpn(locs,pks,1:numSamples,InterpMethod)';
    
        [pks,locs] = findpeaks(Qorgfs,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
        [pks,locs] = PadLocs(pks,locs,numSamples);
        Qfs = interpn(locs,pks,1:numSamples,InterpMethod)';

        Ifs=Ifs-mean(Ifs);
        Qfs=Qfs-mean(Qfs);
    end
    %figure(5);
    %hold on
    %plot(Ipks)
    %plot(I)
    %hold off

    %I = abs(hilbert(Iorg));
    %Q = abs(hilbert(Qorg));
    I=I-mean(I);
    Q=Q-mean(Q);
    It=It-mean(It);
    Qt=Qt-mean(Qt);
    %I=Ids;
    %Q=Qds;
    %da = designfilt('lowpassiir','PassbandFrequency',0.0005,'StopbandFrequency',0.01,'PassbandRipple',0.1,'StopbandAttenuation',60);
    %I = filtfilt(da,I);
    %Q = filtfilt(da,Q);

    % Combine into complex samples
    IQ = complex(I,Q);
    IQorg = complex(Iorg,Qorg);
    
    % Plot time-domain waveform (I and Q)
    figure(1);
    subplot(2,1,1);
    plot(Iorg); title('I Component'); xlabel('Sample'); ylabel('Amplitude');
    ylim([-1 1])
    hold on
    plot(I)
    hold off
    
    subplot(2,1,2);
    plot(Qorg); title('Q Component'); xlabel('Sample'); ylabel('Amplitude');
    ylim([-1 1])
    hold on
    plot(Q)
    hold off

    % Plot constellation
    figure(2);
    plot(IQorg, '.'); title('Constellation'); xlabel('I'); ylabel('Q'); axis equal;

    % Compute FFT and convert to dB
    if t_frames>1
        %IQData((1+numSamples):end) = IQData(1:(numSamples*(t_frames-1)));
        %IQData(1:numSamples) = flipud(IQ);
        %IQData = IQData.*linearScaler';
        IData = paddata(IData((1+numSamples):end), numel(IData));
        IData = FlatScaler*IData;
        IData(numSamples*(t_frames-1)+1:end) = I;
        QData = paddata(QData((1+numSamples):end), numel(QData));
        QData = FlatScaler*QData;
        QData(numSamples*(t_frames-1)+1:end) = Q;

        ItData = paddata(ItData((1+numSamples):end), numel(ItData));
        ItData = FlatScaler*ItData;
        ItData(numSamples*(t_frames-1)+1:end) = It;
        QtData = paddata(QtData((1+numSamples):end), numel(QtData));
        QtData = FlatScaler*QtData;
        QtData(numSamples*(t_frames-1)+1:end) = Qt;

        IQData = paddata(IQData((1+numSamples):end), numel(IQData));
        IQData = FlatScaler*IQData;
        IQData(numSamples*(t_frames-1)+1:end) = IQ;

        IQtData = paddata(IQtData((1+(numSamples*numIQt)):end), numel(IQtData));
        IQtData = FlatScaler*IQtData;
        %%%%IQtData(numSamples*numIQt*(t_frames-1)+1:end) = [I;Q;It;Qt;1*IQtEnvelope];%[I;Q;It;Qt];%[I;Q;It;Qt;1*IQtEnvelope];%GetIQtSignal(Iorg,Qorg,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples,numIQt);%[I;Q;It;Qt;1*IQtEnvelope];
        %%%%IQtData(numSamples*numIQt*(t_frames-1)+1:end) = GetInterpolatedSignalEnvelope([Iorg;-Iorg;Qorg;-Qorg],InterpMethod,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples*numIQt); IQtData = IQtData-mean(IQtData);
        if Channels==1
            IQtData(numSamples*numIQt*(t_frames-1)+1:end) = (Ids+Qds)./2;
        else
            IQtData(numSamples*numIQt*(t_frames-1)+1:end) = (Ids+Qds+I1ds+Q1ds)./4;
        end
    else
        IData = I;
        QData = Q;
        ItData = It;
        QtData = Qt;
        IQData = IQ;
        IQtData = [I;Q];%[I;Q;It;Qt;1*IQtEnvelope];%[I;Q;It;Qt];%[I;Q;It;Qt;1*IQtEnvelope];
        %%%%IQtData = GetInterpolatedSignalEnvelope([Iorg;circshift(Qorg,numSamples/4/fTone);circshift(-Iorg,numSamples/2/fTone);circshift(-Qorg,round(3*numSamples/4/fTone))],InterpMethod,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples*numIQt); IQtData = IQtData-mean(IQtData); IQtData = filter(ones(1,160*t_frames*numIQt)/(160*t_frames*numIQt),1,IQtData); %conv(IQtData, ones(1,160*t_frames*numIQt)/(160*t_frames*numIQt), 'same');
        %IQtData = GetIQtSignal(Iorg,Qorg,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples,numIQt);
        if Channels==1
            IQtData = IQrecovered;%(Ids+Qds)./2;%[Ipds;Qpds;Inds;Qnds];%(Ids+Qds)./2;%[Ipds;Qpds;Inds;Qnds];%[GetInterleave(Ipds,Qpds)];%
        else
            IQtData = IQrecovered;%(Ids+Qds)./2;
            IQtMIMOData = (IQrecovered+IQ1recovered)./2;%complex(Ids+I1ds,Qds+Q1ds)./2;
        end
    end
    fftData = fftshift(fft(IQData));
    spectrum = 20 * log10(abs(fftData) + 1e-6);  % add epsilon to avoid log(0)

    spectrum1 = 20 * log10(abs(fftshift(fft(IData))) + 1e-6);
    spectrum2 = 20 * log10(abs(fftshift(fft(QData))) + 1e-6);
    spectrum3 = 20 * log10(abs(fftshift(fft(ItData))) + 1e-6);
    spectrum4 = 20 * log10(abs(fftshift(fft(QtData))) + 1e-6);
    % non-dB data
    %%%%spectrum = abs(fftData)/max(abs(fftData));
    spec1 = abs(fftshift(fft(IData)))./numSamples;
    spec2 = abs(fftshift(fft(QData)))./numSamples;
    spec3 = abs(fftshift(fft(ItData)))./numSamples;
    spec4 = abs(fftshift(fft(QtData)))./numSamples;
    w = blackman(length(IQtData));    % flattopwin loses info
    spec5 = abs(fftshift(fft(IQtData.*1)))./(numSamples*numIQt);

    N = t_frames*numIQt;
    h = ones(1,N)/N;
    %spectrum5filt = filter(h,1,spectrum5);
    spec5filt = conv(spec5,h,'same');
    
    %spec5filt = spec5filt.^2;
    %%%%spec = sqrt(spec1.*spec2);
    spec = (spec1.*spec2.*spec3.*spec4).^(0.25);
    spectrum = 20 * log10(spec + 1e-6);
    spectrum5 = 20 * log10(spec5 + 1e-6);
    spectrum5filt = 20 * log10(spec5filt + 1e-6);

    if Channels==2
        spec6 = abs(fftshift(fft(IQtMIMOData.*1)))./(numSamples*numIQt);
        spec6filt = conv(spec6,h,'same');
        spectrum6 = 20 * log10(spec6 + 1e-6);
        spectrum6filt = 20 * log10(spec6filt + 1e-6); 
    end
    figure(3);
    % Update plot
    %plot(f(FreqRange),spectrum(FreqRange))
    set(hPlot, 'YData', spectrum(FreqRange));
    set(hPlotIQt, 'YData', spectrum5(FreqRangeIQt));
    
    % Wavelet black magic for sharpening averaged spectral peaks
    [c,l] = wavedec(spectrum5filt,4,'db4');   % 4-level wavelet decomposition
    cD1 = wrcoef('d',c,l,'db4',1); % detail level
    cD2 = wrcoef('d',c,l,'db4',2);
    
    % Reconstruct with boosted details
    spectrum5filt = spectrum5filt + 2*(cD1 + cD2);
    spectrum5 = spectrum5filt;
    %%spectrum5filt_sharp = spectrum5filt + 1.5*(spectrum5filt - spectrum5);
    set(hPlotIQtrms, 'YData', spectrum5filt(FreqRangeIQt));

    %=== Writing to file ===
    y = spectrum5filt(FreqRangeIQt);
    in50Hz = 50*DurationMultiplier+1;
    in100Hz= 100*DurationMultiplier+1;
    y50Hz = max( y(in50Hz-DurationMultiplier:in50Hz+DurationMultiplier) );
    y100Hz = max( y(in100Hz-DurationMultiplier:in100Hz+DurationMultiplier) );
    aveSig = mean(y);
    % Timestamp
    ts = datetime('now','Format','dd/MM/yyyy HH:mm:ss');
    % Open CSV in append mode
    fid = fopen('spectra.csv','a');
    % Write timestamp
    fprintf(fid,'%s', char(ts));
    % Write all y values
    fprintf(fid, ',%.6f', [y50Hz;y100Hz;aveSig]);
    % End row
    fprintf(fid, '\n');
    fclose(fid);
    %=======================

    %set(hPlot1, 'YData', spectrum1(FreqRange));
    %set(hPlot2, 'YData', spectrum2(FreqRange));
    if Channels==2
        set(hPlotIQ1t, 'YData', spectrum6(FreqRangeIQt));
        [c,l] = wavedec(spectrum6filt,4,'db4');   % 4-level wavelet decomposition
        cD1 = wrcoef('d',c,l,'db4',1); % detail level
        cD2 = wrcoef('d',c,l,'db4',2);
        spectrum6filt = spectrum6filt + 2*(cD1 + cD2);
        set(hPlotIQ1trms, 'YData', spectrum6filt(FreqRangeIQt));
    end
    drawnow;

    figure(4);
    % Scroll spectrogram data up and insert new row at bottom
    spec_data = [spec_data(2:end, :); spectrum(FreqRange)'];
    % Update plot
    set(hImg, 'CData', spec_data);

    spec_dataIQt = [spec_dataIQt(2:end, :); spectrum5(FreqRangeIQt)'];
    % Update plot
    set(hImgIQt, 'CData', spec_dataIQt);
    
    drawnow;

    if fSweep == 1
        spec1fs = abs(fftshift(fft(Ifs)));
        spec2fs = abs(fftshift(fft(Qfs)));
        spectrumSweep = 20*log10(sqrt(spec1fs.*spec2fs)+1e-6);
        figure(5);
        %spec_data_Sweep = [spec_data_Sweep(2:end, :); spectrum(FreqRange)'];
        spec_data_Sweep(i__,:) = spectrumSweep(FreqRange)';
        % Update plot
        set(hImgSweep, 'CData', spec_data_Sweep);
        drawnow;
        i__=i__+1;
    end

end

function [pks,locs] = PadLocs(pks,locs,numSamples)
    if locs(1) ~= 1
        locs = [1;locs];
        pks = [mean(pks);pks];
    end
    if locs(end) ~= numSamples
        locs = [locs;numSamples];
        pks = [pks;mean(pks)];
    end
end

function index = GetPhaseIndex(signal,numSamples)
    index = 0;
    for i=1:numSamples-2
        if signal(i) < 0 && signal (i+2) > 0
            index = i+1;
            return;
        end
    end
end

function IQtSignal = GetIQtSignal(Iorg,Qorg,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples,numIQt)
    InterpMethods = ["cubic";"makima";"spline";"linear";"squared";"cubic";"nearest";"pchip"];
    IQtSignal = zeros(numSamples,numIQt);
    for i=1:5
        I = GetInterpolatedSignalEnvelope(Iorg,InterpMethods{i,:},EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples);
        Q = GetInterpolatedSignalEnvelope(-Iorg,InterpMethods{i,:},EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples);
        It = GetInterpolatedSignalEnvelope(Qorg,InterpMethods{i,:},EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples);
        Qt = GetInterpolatedSignalEnvelope(-Qorg,InterpMethods{i,:},EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples);
        Sig = I+Q+It+Qt;
        Sig = Sig-mean(Sig);
        IQtSignal(:,i) = Sig;
    end
    IQtSignal = reshape(IQtSignal,numIQt*numSamples,1);
end

function InterpolatedSignalEnvelope = GetInterpolatedSignalEnvelope(Signal,InterpMethod,EnvelopeSamplingFactor,fs,fTone,MinPeakProminence,numSamples)
    [pks,locs] = findpeaks(Signal,'MinPeakDistance',EnvelopeSamplingFactor*fs/fTone,'MinPeakProminence',MinPeakProminence);
    [pks,locs] = PadLocs(pks,locs,numSamples);
    InterpolatedSignalEnvelope = interpn(locs,pks,1:numSamples,InterpMethod)';
end

function interleaveSig = GetInterleave(v1,v2)
    interleaveSig = reshape([v1'; v2'],[],1);
end