﻿//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#pragma once

#include "armnn/IRuntime.hpp"

#ifdef ARMCOMPUTECL_ENABLED
#include <arm_compute/runtime/CL/CLTuner.h>
#endif

namespace armnn
{

class IGpuAccTunedParameters;
class ClTunedParameters;

// ARM Compute OpenCL context control.
class ClContextControl
{
public:

    ClContextControl(IGpuAccTunedParameters* clTunedParameters = nullptr,
                     bool profilingEnabled = false);

    virtual ~ClContextControl();

    void LoadOpenClRuntime();

    // Users should call this (after freeing all of the cl::Context objects they use)
    // to release the cached memory used by the compute library.
    void UnloadOpenClRuntime();

    // Clear the CL cache, without losing the tuned parameter settings.
    void ClearClCache();

private:

    void DoLoadOpenClRuntime(bool useTunedParameters);

    ClTunedParameters* m_clTunedParameters;

    bool m_ProfilingEnabled;
};

class ClTunedParameters : public IGpuAccTunedParameters
{
public:
    ClTunedParameters(armnn::IGpuAccTunedParameters::Mode mode);

    virtual void Load(const char* filename);
    virtual void Save(const char* filename) const;

    Mode m_Mode;

#ifdef ARMCOMPUTECL_ENABLED
    arm_compute::CLTuner m_Tuner;
#endif
};

} // namespace armnn
