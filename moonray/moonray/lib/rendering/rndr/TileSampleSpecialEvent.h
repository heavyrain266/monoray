// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

//
//
#pragma once

#include <functional>
#include <string>
#include <vector>

namespace moonray {
namespace rndr {

class TileSampleSpecialEvent
//
// This is a information for special event control based on the tile sample (not a pixel sample)
//
// Each tiles are independently rendering by different threads based on the tile sample start/end Id
// regarding single tile rendering operation. This tile sample start/end Id are carefully designed
// based on the rendering mode and it��s parameters. However sometimes we would like to execute
// special logic based on after finish some particular tile sample id rendering.
// Typically we need this type of operation when we use multi-stage rendering (like Path Guiding).
// This TileSampleSpecilaEvent object is used for this purpose. And we call this special operation
// which executed at after finish some particular sampling as ��special event�� also call it��s timing
// as ��special event tile sampleId��.
//
// Regardless of what kind of tile sample start/end Id are scheduled at first place, final tile sample
// start/end id are computed considered by this TileSampleSpecialEvent��s mTileSampleIdTable information.
// If original tile start/end id is not think over special event yet, Sometimes ��special event tile sampleId��
// is exist middle of originally scheduled time sample start/end span. In this case, this span will be split
// into small mini spans based on the "special event tile sampleIds".
// Checkpoint tile processing logic can handle this "special event" inside mini-stint loop and executes
// call back function as ��special event�� at proper timing.
//
// So far this type of split schedule based on TileSampleSpecialEvent information is supported by
// Checkpoint Rendering logic only.
//
{
public:
    using UIntTable = std::vector<unsigned>;
    using SpecialEventCallBack = std::function<bool(const unsigned sampleId)>;
    
    TileSampleSpecialEvent(const UIntTable &&tileSampleIdTable,
                           SpecialEventCallBack callBack) :
        mTileSampleIdTable(std::move(tileSampleIdTable)),
        mCallBack(callBack)
    {}

    const UIntTable &table() const { return mTileSampleIdTable; }
    const SpecialEventCallBack &callBack() const { return mCallBack; }

    std::string show() const;

private:
    // tile sampleId table. After this tile samples, execute mCallBack functions.
    UIntTable mTileSampleIdTable;

    // Callback function to execute every tile sampleId which defined by mTileSampleIdTable.
    SpecialEventCallBack mCallBack;
};

} // namespace rndr
} // namespace moonray

