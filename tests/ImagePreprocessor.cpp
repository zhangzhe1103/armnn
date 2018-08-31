//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#include "InferenceTestImage.hpp"
#include "ImagePreprocessor.hpp"
#include "Permute.hpp"
#include <armnn/TypesUtils.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/assert.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <fcntl.h>
#include <array>

template <typename TDataType>
unsigned int ImagePreprocessor<TDataType>::GetLabelAndResizedImageAsFloat(unsigned int testCaseId,
                                                                          std::vector<float> & result)
{
    testCaseId = testCaseId % boost::numeric_cast<unsigned int>(m_ImageSet.size());
    const ImageSet& imageSet = m_ImageSet[testCaseId];
    const std::string fullPath = m_BinaryDirectory + imageSet.first;

    InferenceTestImage image(fullPath.c_str());

    // this ResizeBilinear result is closer to the tensorflow one than STB.
    // there is still some difference though, but the inference results are
    // similar to tensorflow for MobileNet

    result = image.Resize(m_Width, m_Height, CHECK_LOCATION(),
                          InferenceTestImage::ResizingMethods::BilinearAndNormalized,
                          m_Mean, m_Stddev);

    if (m_DataFormat == DataFormat::NCHW)
    {
        const armnn::PermutationVector NHWCToArmNN = { 0, 2, 3, 1 };
        armnn::TensorShape dstShape({1, 3, m_Height, m_Width});
        std::vector<float> tempImage(result.size());
        armnnUtils::Permute<float>(dstShape, NHWCToArmNN, result.data(), tempImage.data());
        result.swap(tempImage);
    }

    return imageSet.second;
}

template <>
std::unique_ptr<ImagePreprocessor<float>::TTestCaseData>
ImagePreprocessor<float>::GetTestCaseData(unsigned int testCaseId)
{
    std::vector<float> resized;
    auto label = GetLabelAndResizedImageAsFloat(testCaseId, resized);
    return std::make_unique<TTestCaseData>(label, std::move(resized));
}

template <>
std::unique_ptr<ImagePreprocessor<uint8_t>::TTestCaseData>
ImagePreprocessor<uint8_t>::GetTestCaseData(unsigned int testCaseId)
{
    std::vector<float> resized;
    auto label = GetLabelAndResizedImageAsFloat(testCaseId, resized);

    size_t resizedSize = resized.size();
    std::vector<uint8_t> quantized(resized.size());

    for (size_t i=0; i<resizedSize; ++i)
    {
        quantized[i] = armnn::Quantize<uint8_t>(resized[i],
                                                m_Scale,
                                                m_Offset);
    }
    return std::make_unique<TTestCaseData>(label, std::move(quantized));
}
