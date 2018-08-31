//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "ClSoftmaxBaseWorkload.hpp"

#include "backends/ArmComputeTensorUtils.hpp"

namespace armnn
{

arm_compute::Status ClSoftmaxWorkloadValidate(const TensorInfo& input,
                                              const TensorInfo& output)
{
    // NOTE: We report 4D Softmax as unsupported until full support is added to ACL
    if(input.GetShape().GetNumDimensions() >= 4u)
    {
        return arm_compute::Status(arm_compute::ErrorCode::RUNTIME_ERROR, "4d softmax is not supported");
    }

    const arm_compute::TensorInfo aclInputInfo = armcomputetensorutils::BuildArmComputeTensorInfo(input);
    const arm_compute::TensorInfo aclOutputInfo = armcomputetensorutils::BuildArmComputeTensorInfo(output);

    return arm_compute::CLSoftmaxLayer::validate(&aclInputInfo, &aclOutputInfo);
}

}
