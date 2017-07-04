#include "discrete_point_fft.h"

#define LOG_TAG "Ne10_DisCretePointFFT"

#define DSP_SAMPLES 64
#define PI (float)3.14159265358979323846

DisCretePointFFT::DisCretePointFFT() {
	fftRoutine = NULL;
	m_ffttime = new float[DSP_SAMPLES];
	memset(m_ffttime, 0, sizeof(float) * DSP_SAMPLES);
	m_corrsize = DSP_SAMPLES / 2 + 1;
	m_fftfreqre = new float[m_corrsize];
	memset(m_fftfreqre, 0, sizeof(float) * DSP_SAMPLES);
	m_fftfreqim = new float[m_corrsize];
	memset(m_fftfreqim, 0, sizeof(float) * DSP_SAMPLES);
	cfg = 0;
	m_hannwindow = new float[DSP_SAMPLES];
	memset(m_hannwindow, 0, sizeof(float) * DSP_SAMPLES);
	for (int ti = 0; ti < DSP_SAMPLES; ti++) {
		m_hannwindow[ti] = -0.5 * cos(2 * PI * ti / DSP_SAMPLES) + 0.5;
	}
}
DisCretePointFFT::~DisCretePointFFT() {
	if (m_hannwindow != NULL) {
		delete[] m_hannwindow;
		m_hannwindow = NULL;
	}
	if (m_ffttime) {
		delete[] m_ffttime;
		m_ffttime = NULL;
	}
	if (m_fftfreqre) {
		delete[] m_fftfreqre;
		m_fftfreqre = NULL;
	}
	if (m_fftfreqim) {
		delete[] m_fftfreqim;
		m_fftfreqim = NULL;
	}
}

void DisCretePointFFT::buildData(short* buffer, int size) {
	LOGI("Display Discrete Point");
	for (int i = 0; i < size; i++) {
		buffer[i] = (short) ((float) rand() / RAND_MAX * 32767.0f);
		LOGI("index is %d sample is %d", i, buffer[i]);
	}
}

void DisCretePointFFT::test() {
	/** 初始化两个FFT的实现 **/
	this->mayerFFTInit();
	this->dspFFTInit();
	/** 读取声音文件，并且加汉明窗，然后做FFT **/
	short* buffer = new short[DSP_SAMPLES];
	this->buildData(buffer, DSP_SAMPLES);
	LOGI("Mayer FFT Result:");
	this->mayerFFTProcess(buffer, DSP_SAMPLES);
	LOGI("Ne10 FFT Result:");
	this->dspFFTProcess(buffer, DSP_SAMPLES);
	delete[] buffer;
	/** 销毁两个FFT的实现 **/
	this->mayerFFTDestroy();
	this->dspFFTDestroy();
}

void DisCretePointFFT::mayerFFTInit() {
	fftRoutine = new FftRoutine(DSP_SAMPLES);
}
void DisCretePointFFT::mayerFFTProcess(short* buffer, int size) {
	for (int i = 0; i < size; i++) {
		m_ffttime[i] = buffer[i] / 32768.0f * m_hannwindow[i];
	}
	fftRoutine->fft_forward(m_ffttime, m_fftfreqre, m_fftfreqim);
	for (int i = 0; i < m_corrsize - 1; i++) {
		float result = m_fftfreqre[i] * m_fftfreqre[i] + m_fftfreqim[i] * m_fftfreqim[i];
		LOGI("index is %d result is %.6f", i, result);
	}
//	ne10_float32_t* in = (ne10_float32_t*) NE10_MALLOC(DSP_SAMPLES * sizeof(ne10_float32_t));
//	for (int i = 0; i < size; i++) {
//		in[i] = buffer[i] / 32768.0f * m_hannwindow[i];
//	}
//	ne10_fft_cpx_float32_t* out = (ne10_fft_cpx_float32_t*) NE10_MALLOC(DSP_SAMPLES * sizeof(ne10_fft_cpx_float32_t));
//	ne10_fft_r2c_1d_float32_neon(out, in, cfg);
//	for (int i = 0; i < size; i++) {
//		float re = out[i].r;
//		float im = out[i].i;
//		float result = re * re + im * im;
//		int correctIndex = m_corrsize - (i+2-m_corrsize);
//		float expectRe = i >= m_corrsize ? m_fftfreqre[correctIndex] : m_fftfreqre[i];
//		float expectIm = i >= m_corrsize ? (0 - m_fftfreqim[correctIndex]) : m_fftfreqim[i];
//		LOGI("IN[%d] is %.4f --> Expected OUT[%d] %.4f + %.4fi; ",
//				i, m_ffttime[i], i, expectRe, expectIm);
//		LOGI("	    Actual OUT[%d] %.4f + %.4fi ",  i, re, im);
//	}
//	NE10_FREE(in);
//	NE10_FREE(out);
}

void DisCretePointFFT::mayerFFTDestroy() {
	if (fftRoutine) {
		delete fftRoutine;
		fftRoutine = NULL;
	}
}

void DisCretePointFFT::dspFFTInit() {
	if (ne10_init() != NE10_OK) {
		LOGE("Failed to initialise Ne10.\n");
	}
	cfg = ne10_fft_alloc_r2c_float32(DSP_SAMPLES);
}

void DisCretePointFFT::dspFFTProcess(short* buffer, int size) {
	ne10_float32_t* in = (ne10_float32_t*) NE10_MALLOC(DSP_SAMPLES * sizeof(ne10_float32_t));
	for (int i = 0; i < size; i++) {
		in[i] = buffer[i] / 32768.0f * m_hannwindow[i];
	}
	ne10_fft_cpx_float32_t* out = (ne10_fft_cpx_float32_t*) NE10_MALLOC(DSP_SAMPLES * sizeof(ne10_fft_cpx_float32_t));
	LOGI("Ne10 neon Accelerate Result:");
	ne10_fft_r2c_1d_float32_neon(out, in, cfg);
	for (int i = 0; i < m_corrsize - 1; i++) {
		float re = out[i].r;
		float im = out[i].i;
		float result = re * re + im * im;
		LOGI("index is %d result is %.6f", i, result);
	}
	ne10_fft_c2r_1d_float32_neon(in, out, cfg);
	for (int i = 0; i < m_corrsize - 1; i++) {
//		in[i] = in[i] / DSP_SAMPLES;
		buffer[i] = in[i] / m_hannwindow[i] * 32768.0f;
//		LOGI("buffer is %d result is %d", i, buffer[i]);
	}
//	LOGI("Ne10 CPU Cal Result:");
//	ne10_fft_r2c_1d_float32_c(out, in, cfg);
//	for (int i = 0; i < m_corrsize - 1; i++) {
//		float re = out[i].r;
//		float im = out[i].i;
//		float result = re * re + im * im;
//		LOGI("index is %d result is %.6f", i, result);
//	}
	NE10_FREE(in);
	NE10_FREE(out);
}

void DisCretePointFFT::dspFFTDestroy() {
	NE10_FREE(cfg);
}
