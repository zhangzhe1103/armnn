//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "NeonQuantizeWorkload.hpp"
#include "NeonWorkloadUtils.hpp"

#include <neon/NeonTensorHandle.hpp>
#include <aclCommon/ArmComputeTensorUtils.hpp>
#include <arm_compute/core/Types.h>

#include <boost/polymorphic_pointer_cast.hpp>

namespace armnn
{
using namespace armcomputetensorutils;

arm_compute::Status NeonQuantizeWorkloadValidate(const TensorInfo& input, const TensorInfo& output)
{
    const arm_compute::TensorInfo neonInputInfo  = armcomputetensorutils::BuildArmComputeTensorInfo(input);
    const arm_compute::TensorInfo neonOutputInfo = armcomputetensorutils::BuildArmComputeTensorInfo(output);

    return arm_compute::NEQuantizationLayer::validate(&neonInputInfo, &neonOutputInfo);
}

NeonQuantizeWorkload::NeonQuantizeWorkload(const QuantizeQueueDescriptor& descriptor,
     const WorkloadInfo& workloadInfo)
     : BaseWorkload<QuantizeQueueDescriptor>(descriptor, workloadInfo)
{
    arm_compute::ITensor& input = boost::polymorphic_pointer_downcast<INeonTensorHandle>(
                                                                      m_Data.m_Inputs[0])->GetTensor();
    arm_compute::ITensor& output = boost::polymorphic_pointer_downcast<INeonTensorHandle>(
                                                                       m_Data.m_Outputs[0])->GetTensor();

    m_Layer.reset(new arm_compute::NEQuantizationLayer());
    m_Layer->configure(&input, &output);
    m_Layer->prepare();
}

void NeonQuantizeWorkload::Execute() const
{
    if (m_Layer)
    {
        ARMNN_SCOPED_PROFILING_EVENT_NEON("NeonQuantizeWorkload_Execute");
        m_Layer->run();
    }
}

} // namespace armnn
