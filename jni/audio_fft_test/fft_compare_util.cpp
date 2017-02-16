#include "fft_compare_util.h"

#define LOG_TAG "Ne10_FFTCompareUtil"

#define DSP_SAMPLES 2048
#define PI (float)3.14159265358979323846

FFTCompareUtil::FFTCompareUtil() {
	audioFile = NULL;
	mayerFFTResultFile = NULL;
	dspFFTResultFile = NULL;
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
FFTCompareUtil::~FFTCompareUtil() {
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

void FFTCompareUtil::test(const char* audioFilePath, const char* mayerFFTResultPath, const char* dspFFTResultPath) {
	audioFile = fopen(audioFilePath, "rb+");
	mayerFFTResultFile = fopen(mayerFFTResultPath, "wb+");
	dspFFTResultFile = fopen(dspFFTResultPath, "wb+");
	/** 初始化两个FFT的实现 **/
	this->mayerFFTInit();
	this->dspFFTInit();
	/** 读取声音文件，并且加汉明窗，然后做FFT **/
	short* buffer = new short[DSP_SAMPLES];
	int actualSize = -1;
	while ((actualSize = fread(buffer, sizeof(short), DSP_SAMPLES, audioFile)) > 0) {
		this->mayerFFTProcess(buffer, actualSize);
		this->dspFFTProcess(buffer, actualSize);
	}
	delete[] buffer;
	/** 销毁两个FFT的实现 **/
	this->mayerFFTDestroy();
	this->dspFFTDestroy();
	fclose(audioFile);
	fclose(mayerFFTResultFile);
	fclose(dspFFTResultFile);
}

void FFTCompareUtil::mayerFFTInit() {
	fftRoutine = new FftRoutine(DSP_SAMPLES);
}
void FFTCompareUtil::mayerFFTProcess(short* buffer, int size) {
	for(int i = 0; i < size; i++) {
		m_ffttime[i] = buffer[i] / 32768.0f * m_hannwindow[i];
	}
	fftRoutine->fft_forward(m_ffttime, m_fftfreqre, m_fftfreqim);
	for(int i = 0; i < m_corrsize - 1; i++) {
		float result = m_fftfreqre[i] * m_fftfreqre[i] + m_fftfreqim[i] * m_fftfreqim[i];
		fwrite(&result, sizeof(float), 1, mayerFFTResultFile);
	}
}

void FFTCompareUtil::mayerFFTDestroy() {
	if (fftRoutine) {
		delete fftRoutine;
		fftRoutine = NULL;
	}
}

void FFTCompareUtil::dspFFTInit() {
	// Initialise Ne10, using hardware auto-detection to set library function pointers.
	if (ne10_init() != NE10_OK) {
		LOGE("Failed to initialise Ne10.\n");
	}
	LOGI("Ne10_FFTCompareUtil Init Neon Accelerate...");
	// Prepare the complex-to-complex single precision floating point FFT configuration
	// structure for inputs of length `SAMPLES`. (You need only generate this once for a
	// particular input size.)
	cfg = ne10_fft_alloc_r2c_float32(DSP_SAMPLES);
}

void FFTCompareUtil::dspFFTProcess(short* buffer, int size) {
	ne10_float32_t* in=(ne10_float32_t*) NE10_MALLOC (DSP_SAMPLES * sizeof (ne10_float32_t));
    for (int i = 0; i < size; i++) {
        in[i] = buffer[i] / 32768.0f * m_hannwindow[i];
    }
    ne10_fft_cpx_float32_t* out = (ne10_fft_cpx_float32_t*) NE10_MALLOC (DSP_SAMPLES * sizeof (ne10_fft_cpx_float32_t));
	// Perform the FFT (for an IFFT, the last parameter should be `1`)
	ne10_fft_r2c_1d_float32_neon(out, in, cfg);
	for(int i = 0; i < m_corrsize - 1; i++) {
		float re = out[i].r;
		float im = out[i].i;
		float result = re * re + im * im;
		fwrite(&result, sizeof(float), 1, dspFFTResultFile);
	}
    NE10_FREE(in);
    NE10_FREE(out);
}

void FFTCompareUtil::dspFFTDestroy() {
	// Free the allocated configuration structure
//	ne10_fft_destroy_c2c_float32(cfg);
    NE10_FREE(cfg);
}
