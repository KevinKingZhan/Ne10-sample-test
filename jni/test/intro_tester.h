#ifndef _Intro_SAMPLE_TEST_H_
#define _Intro_SAMPLE_TEST_H_

#include "CommonTools.h"

class IntroTester {
public:
	IntroTester();
	virtual ~IntroTester();
	void test();
private:
	void test_add_static();
	void test_add_dynamic();
};
#endif	//_Intro_SAMPLE_TEST_H_
