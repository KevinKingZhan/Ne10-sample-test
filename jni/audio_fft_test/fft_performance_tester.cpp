#include "fft_performance_tester.h"

#define LOG_TAG "FFT_Performance_Tester"

#define DSP_SAMPLES 2048
#define PI (float)3.14159265358979323846

FFTPerformanceTester::FFTPerformanceTester() {
	mayerFFTTime = 0;
	mayerIFFTTime = 0;
	dspFFTTime = 0;
	dspIFFTTime = 0;
	audioFile = NULL;
	mayerFFTResultFile = NULL;
	mayerIFFTResultPCMFile = NULL;
	dspFFTResultFile = NULL;
	dspIFFTResultPCMFile = NULL;
	fftRoutine = NULL;
	m_ffttime = new float[DSP_SAMPLES];
	memset(m_ffttime, 0, sizeof(float) * DSP_SAMPLES);
	m_corrsize = DSP_SAMPLES / 2 + 1;
	m_fftfreqre = new float[m_corrsize];
	memset(m_fftfreqre, 0, sizeof(float) * m_corrsize);
	m_fftfreqim = new float[m_corrsize];
	memset(m_fftfreqim, 0, sizeof(float) * m_corrsize);
	mayerFFTTmpResult = new float[m_corrsize];
	memset(mayerFFTTmpResult, 0, sizeof(float) * m_corrsize);
	cfg = 0;
	m_hannwindow = new float[DSP_SAMPLES];
    memset(m_hannwindow, 0, sizeof(float) * DSP_SAMPLES);
	for (int ti = 0; ti < DSP_SAMPLES; ti++) {
		m_hannwindow[ti] = -0.5 * cos(2 * PI * ti / (DSP_SAMPLES - 1)) + 0.5;
	}
    m_slideWindow = new CSlideWindow(DSP_SAMPLES, DSP_SAMPLES / 2);
    m_slideWindow->SetHammWindow(false);
    m_inputDataBuf = new float[DSP_SAMPLES];
    memset(m_inputDataBuf, 0, sizeof(float) * DSP_SAMPLES);
    m_dspMixBuffer = new float[DSP_SAMPLES];
    memset(m_dspMixBuffer, 0, sizeof(float) * DSP_SAMPLES);
    m_mayerMixBuffer = new float[DSP_SAMPLES];
    memset(m_mayerMixBuffer, 0, sizeof(float) * DSP_SAMPLES);
}
FFTPerformanceTester::~FFTPerformanceTester() {
    if (m_slideWindow != NULL) {
        delete m_slideWindow;
        m_slideWindow = NULL;
    }
    if (m_dspMixBuffer != NULL) {
        delete[] m_dspMixBuffer;
        m_dspMixBuffer = NULL;
    }
    if (m_mayerMixBuffer != NULL) {
        delete[] m_mayerMixBuffer;
        m_mayerMixBuffer = NULL;
    }
    if (m_inputDataBuf != NULL) {
        delete[] m_inputDataBuf;
        m_inputDataBuf = NULL;
    }
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

void FFTPerformanceTester::test(const char* audioFilePath, const char* mayerFFTResultPath, const char* dspFFTResultPath) {
	audioFile = fopen(audioFilePath, "rb+");
	mayerFFTResultFile = fopen(mayerFFTResultPath, "wb+");
	mayerIFFTResultPCMFile = fopen("/mnt/sdcard/a_songstudio/mayerIFFT.pcm", "wb+");
	dspFFTResultFile = fopen(dspFFTResultPath, "wb+");
	dspIFFTResultPCMFile = fopen("/mnt/sdcard/a_songstudio/dspIFFT.pcm", "wb+");
	/** 初始化两个FFT的实现 **/
	this->mayerFFTInit();
	this->dspFFTInit();
	/** 读取声音文件，并且加汉明窗，然后做FFT **/
	short* buffer = new short[DSP_SAMPLES];
	int actualSize = -1;
	float* inBuffer = new float[DSP_SAMPLES];
	while ((actualSize = fread(buffer, sizeof(short), DSP_SAMPLES, audioFile)) > 0) {
	    for (int i = 0; i < actualSize; i++) {
	    		inBuffer[i] = buffer[i] / 32768.0f;
	    }
	    this->process(inBuffer, actualSize);
	}
	LOGI("mayerFFTTime is %d us dspFFTTime is %d us", mayerFFTTime, dspFFTTime);
	LOGI("mayerIFFTTime is %d us dspIFFTTime is %d us", mayerIFFTTime, dspIFFTTime);
	delete[] inBuffer;
	delete[] buffer;
	/** 销毁两个FFT的实现 **/
	this->mayerFFTDestroy();
	this->dspFFTDestroy();
	fclose(audioFile);
	fclose(mayerFFTResultFile);
	fclose(mayerIFFTResultPCMFile);
	fclose(dspFFTResultFile);
	fclose(dspIFFTResultPCMFile);
}

void FFTPerformanceTester::process(float* inBuffer, int actualSize) {
	float* outBuffer = new float[DSP_SAMPLES];
	size_t slice = 128;

    for (size_t idx = 0; idx < actualSize; idx += slice) {
        if ((actualSize - idx) < slice) {
            slice = actualSize - idx;
        }
        bool ret = m_slideWindow->Process(inBuffer + idx, slice);
        if(ret) {
            size_t cLatency;
            size_t fLatency;
            size_t retSize = m_slideWindow->CheckOutSlideWindow(m_inputDataBuf, cLatency, fLatency);
            if (retSize != DSP_SAMPLES) {
                memset(m_inputDataBuf, 0, sizeof(float) * DSP_SAMPLES);
            }
            //Do FFT
			long long startTimeUs = currentTimeUs();
			this->mayerFFTProcess(m_inputDataBuf, actualSize);
			mayerFFTTime+=(currentTimeUs() - startTimeUs);
			startTimeUs = currentTimeUs();
			this->dspFFTProcess(m_inputDataBuf, actualSize);
			dspFFTTime+=(currentTimeUs() - startTimeUs);
			//DO IFFT
			startTimeUs = currentTimeUs();
			this->mayerIFFTProcess(outBuffer, actualSize);
			mayerIFFTTime+=(currentTimeUs() - startTimeUs);
			this->prcocessMayerIFFTResult(outBuffer, actualSize);
			startTimeUs = currentTimeUs();
			this->dspIFFTProcess(outBuffer, actualSize);
			dspIFFTTime+=(currentTimeUs() - startTimeUs);
			this->prcocessDSPIFFTResult(outBuffer, actualSize);
        }
    }
	delete[] outBuffer;
}

void FFTPerformanceTester::prcocessDSPIFFTResult(float* buffer, int actualSize) {
	for (int i = 0; i < actualSize; i++) {
		buffer[i] = buffer[i] * m_hannwindow[i];
	}
	float tmp_fMixOut[DSP_SAMPLES];
	for(size_t i = 0; i < (DSP_SAMPLES / 2); i++) {
		tmp_fMixOut[i] = mix(buffer[i], m_dspMixBuffer[i]);
	}
	memcpy(m_dspMixBuffer, buffer + actualSize / 2, actualSize / 2 * sizeof(float));
	this->writeToPCMFile(tmp_fMixOut, actualSize / 2, dspIFFTResultPCMFile);
}

void FFTPerformanceTester::prcocessMayerIFFTResult(float* buffer, int actualSize) {
	for (int i = 0; i < actualSize; i++) {
		buffer[i] = buffer[i] / DSP_SAMPLES; // Mayer iFFt result need divide fft window size
		buffer[i] = buffer[i] * m_hannwindow[i];
	}
	float tmp_fMixOut[DSP_SAMPLES];
	for(size_t i = 0; i< (DSP_SAMPLES / 2); i++) {
		tmp_fMixOut[i] = mix(buffer[i], m_mayerMixBuffer[i]);
	}
	memcpy(m_mayerMixBuffer, buffer + actualSize / 2, actualSize / 2 * sizeof(float));
	this->writeToPCMFile(tmp_fMixOut, actualSize / 2, mayerIFFTResultPCMFile);
}
void FFTPerformanceTester::writeToPCMFile(float* outBuffer, int actualSize, FILE* outputFile) {
	//Convert To SInt16
	short* buffer = new short[actualSize];
	for (int i = 0; i < actualSize; i++) {
		buffer[i] = (short)(outBuffer[i] * 32768.0f);
	}
	//Write To PCM FILE
	fwrite(buffer, sizeof(short), actualSize, outputFile);
	delete[] buffer;
}

void FFTPerformanceTester::mayerFFTInit() {
	fftRoutine = new FftRoutine(DSP_SAMPLES);
}
void FFTPerformanceTester::mayerIFFTProcess(float* buffer, int size) {
	fftRoutine->fft_inverse(m_fftfreqre, m_fftfreqim, buffer);
}
void FFTPerformanceTester::mayerFFTProcess(float* buffer, int size) {
	fftRoutine->fft_forward(buffer, m_fftfreqre, m_fftfreqim);
}

void FFTPerformanceTester::mayerFFTDestroy() {
	if (fftRoutine) {
		delete fftRoutine;
		fftRoutine = NULL;
	}
}

void FFTPerformanceTester::dspFFTInit() {
	// Initialise Ne10, using hardware auto-detection to set library function pointers.
	if (ne10_init() != NE10_OK) {
		LOGE("Failed to initialise Ne10.\n");
	}
	LOGI("Ne10_FFTPerformanceTester Init Neon Accelerate...");
	cfg = ne10_fft_alloc_r2c_float32(DSP_SAMPLES);
    out = (ne10_fft_cpx_float32_t*) NE10_MALLOC (DSP_SAMPLES * sizeof (ne10_fft_cpx_float32_t));
}

void FFTPerformanceTester::dspIFFTProcess(float* buffer, int size) {
	ne10_fft_c2r_1d_float32_neon(in, out, cfg);
	memcpy(buffer, in, sizeof(float) * size);
}

void FFTPerformanceTester::dspFFTProcess(float* buffer, int size) {
    in = buffer;
	ne10_fft_r2c_1d_float32_neon(out, in, cfg);
}

void FFTPerformanceTester::dspFFTDestroy() {
    NE10_FREE(out);
    NE10_FREE(cfg);
}
