//
//  MNNTestSuite.h
//  MNN
//
//  Created by MNN on 2019/01/10.
//  Copyright © 2018, Alibaba Group Holding Limited
//

#ifndef TEST_MNNTEST_H
#define TEST_MNNTEST_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <cstdint>



#if defined(_MSC_VER)
#include <Windows.h>
#undef min
#undef max
#undef NO_ERROR
#else
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

/** test case */
class MNNTestCase {
    friend class MNNTestSuite;

public:
    /**
     * @brief deinitializer
     */
    virtual ~MNNTestCase() = default;
    /**
     * @brief run test case with runtime precision, see FP32Converter in TestUtil.h.
     * @param precision  fp32 / bf16 precision should use FP32Converter[1 - 2].
     * fp16 precision should use FP32Converter[3].
     */
    virtual bool run(int precision) = 0;

private:
    /** case name */
    std::string name;
};

/** test suite */
class MNNTestSuite {
public:
    /**
     * @brief deinitializer
     */
    ~MNNTestSuite();
    /**
     * @brief get shared instance
     * @return shared instance
     */
    static MNNTestSuite* get();
    struct Status {
        int precision = 0;
        int memory = 0;
        int power = 0;
    };
    Status pStaus;

public:
    /**
     * @brief register runable test case
     * @param test test case
     * @param name case name
     */
    void add(MNNTestCase* test, const char* name);
    /**
     * @brief run all registered test case with runtime precision, see FP32Converter in TestUtil.h.
     * @param precision . fp32 / bf16 precision should use FP32Converter[1 - 2].
     * fp16 precision should use FP32Converter[3].
     */
    static int runAll(int precision, const char* flag = "");
    /**
     * @brief run test case with runtime precision, see FP32Converter in TestUtil.h.
     * @param precision . fp32 / bf16 precision should use FP32Converter[1 - 2].
     * fp16 precision should use FP32Converter[3].
     */
    static int run(const char* name, int precision, const char* flag = "");

private:
    /** get shared instance */
    static MNNTestSuite* gInstance;
    /** registered test cases */
    std::vector<MNNTestCase*> mTests;
};

/**
 static register for test case
 */
template <class Case>
class MNNTestRegister {
public:
    /**
     * @brief initializer. register test case to suite.
     * @param name test case name
     */
    MNNTestRegister(const char* name) {
        MNNTestSuite::get()->add(new Case, name);
    }
    /**
     * @brief deinitializer
     */
    ~MNNTestRegister() {
    }
};

#define MNNTestSuiteRegister(Case, name) static MNNTestRegister<Case> __r##Case(name)
#define MNNTEST_ASSERT(x)                                        \
    {                                                            \
        int res = (x);                                           \
        if (!res) {                                              \
            MNN_ERROR("Error for %s, %d\n", __func__, __LINE__); \
            return false;                                        \
        }                                                        \
    }

#endif
