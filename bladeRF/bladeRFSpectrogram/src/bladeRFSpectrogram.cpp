#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <libbladeRF.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <thread>
#include <barrier>
#include <vector>
#include <atomic>
#include <csignal>
#include <cstdint>
#include <string>

struct bladerf_complex_int16
{
	int16_t i;
	int16_t q;
};
struct complex_float
{
	float i;
	float q;
};

std::atomic<bool> running(true);

// Simple TX and RX parameters
const int Channels = 1;		// 1 or 2
const int sample_rate = 8e5;//1 << 20;//1000000; // 1 MSPS
const int Duration = 5;	// Duration of signal as a multiple
bladerf_frequency frequency = 426ULL*1000000ULL; // 915 MHz // 2750 MHz best res // 420 most signals // 425 best 50 Hz AC
const bladerf_frequency fIncrement = 0ULL*1000000ULL; // increment for frequency sweep
const bladerf_frequency fEnd = 6001ULL*1000000ULL;
const bladerf_frequency freqDelta = 0ULL*1000ULL;
const int bandwidth = sample_rate * 2;//1500000;   // 1.5 MHz
const int num_samples = sample_rate*Duration;//1 << 16;//sample_rate/1;//1<<19;//4096;    // Samples per buffer
const float ToneAmplitude = 0.75f;
const float ToneFreq = 5000;//5721.117f;
static int x = 0;
const int freqSweepLoop = 2;
int freqLoopIndex = 0;
std::barrier sync_point(2);

// Gaussian function generator
float gaussian(float t,float t0,float sigma)
{
	return std::exp(-((t - t0) * (t - t0)) / (2.0f * sigma * sigma));
}

// Generate time-domain Gaussian I/Q pulse
void generate_gaussian_pulse(std::vector<bladerf_complex_int16>& buffer,float amplitude = 2047.0f)
{
	float duration_sec = static_cast<float>(num_samples) / sample_rate;
	float t0 = duration_sec / 2.0f;           // Center of pulse
	float sigma = duration_sec / 10.0f;       // Width of pulse

	for(size_t i = 0; i < num_samples; ++i) {
		float t = static_cast<float>(i) / sample_rate;
		float env = gaussian(t,t0,sigma);   // Gaussian envelope

		float I = amplitude * env;
		float Q = 0.0f; // Or apply phase shift if desired

		buffer[i].i = I; // I
		buffer[i].q = Q; // Q
	}
}

// Ricker wavelet generator
float ricker_wavelet(float t,float f_c)
{
	float pi2_fc2_t2 = M_PI * M_PI * f_c * f_c * t * t;
	return (1.0f - 2.0f * pi2_fc2_t2) * std::exp(-pi2_fc2_t2);
}

// Generate Ricker wavelet as time-domain I/Q buffer
void generate_ricker_wavelet(std::vector<bladerf_complex_int16>& buffer,float center_freq = 100000.f,float amplitude = 2047.0f)
{
	float t0 = static_cast<float>(num_samples) / (2.0f * sample_rate); // Center time
	float dt = 1.0f / sample_rate;

	for(size_t i = 0; i < num_samples; ++i)
	{
		float t = (static_cast<float>(i) * dt) - t0; // time centered around 0
		float env = ricker_wavelet(t,center_freq);

		float I = amplitude * env;
		float Q = 0.0f; // For now, pure real signal

		// Clip to int16_t range
		buffer[i].i = max(min(I,2047.f),-2048.f); // I
		buffer[i].q = Q;                                  // Q
	}
}

// Generate a simple TX signal (complex sinusoid)
void generate_tx_buffer(std::vector<bladerf_complex_int16>& buffer)
{
	for(int i = 0; i < num_samples; ++i)
	{
		buffer[i].i = ToneAmplitude * 2048 * sin(2 * M_PI * ToneFreq * i / num_samples + x);
		buffer[i].q = ToneAmplitude * 2048 * cos(2 * M_PI * ToneFreq * i / num_samples + x);
	}
	//x++;
}

// TX thread
void tx_thread_fn(bladerf* dev)
{
	std::vector<bladerf_complex_int16> tx_buf(num_samples*Channels);
	if (Channels==1)
	{
		generate_tx_buffer(tx_buf);
		//generate_gaussian_pulse(tx_buf);
		//generate_ricker_wavelet(tx_buf);
	}
	else if (Channels==2)
	{
		std::vector<bladerf_complex_int16> tx_chan0(num_samples);
		std::vector<bladerf_complex_int16> tx_chan1(num_samples);
		generate_tx_buffer(tx_chan0);
		generate_tx_buffer(tx_chan1); // Can be different waveform
		for (int i = 0; i < num_samples; ++i)
		{
			tx_buf[2 * i] = tx_chan0[i];
			tx_buf[2 * i + 1] = tx_chan1[i];
		}
	}
	else
	{
		std::cerr << "Invalid Channels: " << Channels << "\n";
		return;
	}
	while(running && frequency<=fEnd)
	{
		//int status = bladerf_enable_module(dev,BLADERF_CHANNEL_TX(0),false);
		//if(status != 0) { std::cerr << "Failed to enable TX module: " << bladerf_strerror(status) << "\n"; return; }

		//int status = bladerf_set_frequency(dev,BLADERF_CHANNEL_TX(0),frequency + freqDelta);
		//if(status != 0) { std::cerr << "Failed to set TX frequency: " << bladerf_strerror(status) << "\n"; return; }

		//status = bladerf_enable_module(dev,BLADERF_CHANNEL_TX(0),true);
		//if(status != 0) { std::cerr << "Failed to enable TX module: " << bladerf_strerror(status) << "\n"; return; }
		//sync_point.arrive_and_wait();
		int status = bladerf_sync_tx(dev,tx_buf.data(),num_samples*Channels,nullptr,5000);
		if(status != 0)
		{
			std::cerr << "TX error: " << bladerf_strerror(status) << "\n";
			break;
		}
		//frequency+=fIncrement;
	}
	std::cout << "TX thread exiting.\n";
}

// RX thread
void rx_thread_fn(bladerf* dev)
{
	std::vector<bladerf_complex_int16> rx_buf(num_samples*Channels);
	complex_float *rx_buf_f = new complex_float[num_samples*Channels];

	while(running && frequency<=fEnd)
	{
		//int status = bladerf_enable_module(dev,BLADERF_CHANNEL_RX(0),false);
		//if(status != 0) { std::cerr << "Failed to enable RX module: " << bladerf_strerror(status) << "\n"; return; }

		//int status = bladerf_set_frequency(dev,BLADERF_CHANNEL_RX(0),frequency - freqDelta);
		//if(status != 0) { std::cerr << "Failed to set RX frequency: " << bladerf_strerror(status) << "\n"; return; }

		//status = bladerf_enable_module(dev,BLADERF_CHANNEL_RX(0),true);
		//if(status != 0) { std::cerr << "Failed to enable RX module: " << bladerf_strerror(status) << "\n"; return; }
		//sync_point.arrive_and_wait();
		int rx0_gain_db;
		bladerf_get_gain(dev, BLADERF_CHANNEL_RX(0), &rx0_gain_db);
		std::cout << "Channel 0 Gain at " << frequency/1000000ULL << " MHz: " << rx0_gain_db << std::endl;
		if (Channels==2)
		{
			int rx1_gain_db;
			bladerf_get_gain(dev, BLADERF_CHANNEL_RX(1), &rx1_gain_db);
			std::cout << "Channel 1 Gain at " << frequency/1000000ULL << " MHz: " << rx1_gain_db << std::endl;

		}
		int status = bladerf_sync_rx(dev,rx_buf.data(),num_samples*Channels,nullptr,5000);
		if(status != 0)
		{
			std::cerr << "RX error: " << bladerf_strerror(status) << "\n";
			break;
		}

		// Process received samples here (example: print first sample)
		////std::cout << "RX sample: I=" << rx_buf[0].i << ", Q=" << rx_buf[0].q << "\n";
		// After receiving IQ samples in RX thread
		std::string str = "../../tmp/" + std::to_string(frequency);
		//std::cout << "String: " << str << std::endl;
		std::ofstream freq(str.c_str(),std::ios::binary);
		std::ofstream pipe("../../tmp/bladerf_iq_pipe",std::ios::binary);
		std::ofstream outfile("../../tmp/received_samples.iq",std::ios::binary);
		//std::cout << "Num of samples: " << num_samples << std::endl;
		for(int i = 0; i < num_samples; ++i)
		{
			rx_buf_f[Channels*i].i = rx_buf[Channels*i].i / 2048.0f; // scale from int16 to float
			rx_buf_f[Channels*i].q = rx_buf[Channels*i].q / 2048.0f;
			if (Channels==2)
			{
				rx_buf_f[Channels*i+1].i = rx_buf[Channels*i+1].i / 2048.0f; // scale from int16 to float
				rx_buf_f[Channels*i+1].q = rx_buf[Channels*i+1].q / 2048.0f;
			}
			//pipe.write(reinterpret_cast<char*>(&i_val),sizeof(float));
			//pipe.write(reinterpret_cast<char*>(&q_val),sizeof(float));
			//outfile.write(reinterpret_cast<char*>(&i_val),sizeof(float));
			//outfile.write(reinterpret_cast<char*>(&q_val),sizeof(float));
		}
		//float freq_f = static_cast<float>(frequency);
		//freq.write((char*)&freq_f,sizeof(float));
		freq.write((char*)rx_buf_f,Channels*num_samples*sizeof(complex_float));
		pipe.write((char*)rx_buf_f,Channels*num_samples*sizeof(complex_float));
		outfile.write((char*)rx_buf_f,Channels*num_samples*sizeof(complex_float));
		freq.close();
		pipe.close();
		outfile.close();
		freqLoopIndex++;
		if (freqLoopIndex==freqSweepLoop)
		{
			freqLoopIndex = 0;
			frequency+=fIncrement;
		}
	}
	delete[] rx_buf_f;

	std::cout << "RX thread exiting.\n";
}

// Signal handler to stop streaming
void signal_handler(int)
{
	running = false;
}

int main()
{
	struct bladerf* dev = nullptr;
	int status = bladerf_open(&dev,nullptr);
	if(status != 0)
	{
		std::cerr << "Failed to open device: " << bladerf_strerror(status) << "\n";
		return 1;
	}

	for (int c=0; c<Channels; c++)
	{
		// Set Gain
		// Set RX gain (automatic or manual)
		status = bladerf_set_gain_mode(dev, BLADERF_CHANNEL_RX(c), BLADERF_GAIN_DEFAULT);
		int rx_gain_db = 40;  // 0-66. valid range depends on board
		////status = bladerf_set_gain(dev, BLADERF_CHANNEL_RX(0), rx_gain_db);
		//status = bladerf_set_lna_gain(dev, BLADERF_LNA_GAIN_MAX);
		//if (status != 0) { std::cerr << "Failed to set manual gain mode: " << bladerf_strerror(status) << std::endl; }
		//status = bladerf_set_rxvga1(dev,25);
		//if(status != 0) { std::cerr << "RX gain error: " << bladerf_strerror(status) << "\n"; }
		//status = bladerf_set_rxvga2(dev,12);
		//if(status != 0) { std::cerr << "RX gain error: " << bladerf_strerror(status) << "\n"; }

		//status = bladerf_set_gain_mode(dev, BLADERF_CHANNEL_TX(0), BLADERF_GAIN_DEFAULT);
		//if (status != 0) { std::cerr << "Failed to set manual gain mode: " << bladerf_strerror(status) << std::endl; }
		// Set TX gain
		int tx_gain_db = 60;  // typical range: -35 to 0 dB
		status = bladerf_set_gain(dev, BLADERF_CHANNEL_TX(c), tx_gain_db);
		//status = bladerf_set_txvga1(dev,-10);
		//if(status != 0) { std::cerr << "TX gain error: " << bladerf_strerror(status) << "\n"; }
		//status = bladerf_set_txvga2(dev,20);
		//if(status != 0) { std::cerr << "TX gain error: " << bladerf_strerror(status) << "\n"; }

		// Configure RX
		status = bladerf_set_sample_rate(dev,BLADERF_CHANNEL_RX(c),sample_rate,nullptr);
		if(status != 0) { std::cerr << "Failed to set RX sample rate: " << bladerf_strerror(status) << "\n"; return 1; }

		status = bladerf_set_bandwidth(dev,BLADERF_CHANNEL_RX(c),bandwidth,nullptr);
		if(status != 0) { std::cerr << "Failed to set RX bandwidth: " << bladerf_strerror(status) << "\n"; return 1; }

		status = bladerf_set_frequency(dev,BLADERF_CHANNEL_RX(c),frequency - freqDelta);
		if(status != 0) { std::cerr << "Failed to set RX frequency: " << bladerf_strerror(status) << "\n"; return 1; }

		// Configure TX
		status = bladerf_set_sample_rate(dev,BLADERF_CHANNEL_TX(c),sample_rate,nullptr);
		if(status != 0) { std::cerr << "Failed to set TX sample rate: " << bladerf_strerror(status) << "\n"; return 1; }

		status = bladerf_set_bandwidth(dev,BLADERF_CHANNEL_TX(c),bandwidth,nullptr);
		if(status != 0) { std::cerr << "Failed to set TX bandwidth: " << bladerf_strerror(status) << "\n"; return 1; }

		status = bladerf_set_frequency(dev,BLADERF_CHANNEL_TX(c),frequency + freqDelta);
		if(status != 0) { std::cerr << "Failed to set TX frequency: " << bladerf_strerror(status) << "\n"; return 1; }
	}

	bladerf_channel_layout RX_layout = BLADERF_RX_X1;
	bladerf_channel_layout TX_layout = BLADERF_TX_X1;
	if (Channels==2)
	{
		bladerf_channel_layout RX_layout = BLADERF_RX_X2;
		bladerf_channel_layout TX_layout = BLADERF_TX_X2;
	}

	// Init sync interface
	status = bladerf_sync_config(dev,RX_layout,BLADERF_FORMAT_SC16_Q11,16,Channels*4096,8,3500);
	if(status != 0) { std::cerr << "Failed to configure RX sync: " << bladerf_strerror(status) << "\n"; return 1; }

	status = bladerf_sync_config(dev,TX_layout,BLADERF_FORMAT_SC16_Q11,16,Channels*4096,8,3500);
	if(status != 0) { std::cerr << "Failed to configure TX sync: " << bladerf_strerror(status) << "\n"; return 1; }

	for (int c=0; c<Channels; c++)
	{
		// Enable modules
		status = bladerf_enable_module(dev,BLADERF_CHANNEL_RX(c),true);
		if(status != 0) { std::cerr << "Failed to enable RX module: " << bladerf_strerror(status) << "\n"; return 1; }

		status = bladerf_enable_module(dev,BLADERF_CHANNEL_TX(c),true);
		if(status != 0) { std::cerr << "Failed to enable TX module: " << bladerf_strerror(status) << "\n"; return 1; }
	}
	// Handle Ctrl+C
	signal(SIGINT,signal_handler);

	std::thread tx_thread(tx_thread_fn,dev);
	std::thread rx_thread(rx_thread_fn,dev);

	tx_thread.join();
	rx_thread.join();


	bladerf_close(dev);

	std::cout << "Full-duplex example completed.\n";
	return 0;
}
