// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

/// @file closure.cc
/// @note this is an extremely pared down and highly modified version
/// of closure.cpp from OSL's distribution.  It is sufficient for
/// compiling just the lpe aspects of the osl library that we
/// are interested in.

/*
Copyright (c) 2009-2010 Sony Pictures Imageworks Inc., et al.
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of Sony Pictures Imageworks nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "oslconfig.h"
#include "oslclosure.h"


OSL_NAMESPACE_ENTER

const ustring Labels::NONE       = ustring(NULL);
const ustring Labels::CAMERA     = ustring("C");
const ustring Labels::LIGHT      = ustring("L");
const ustring Labels::BACKGROUND = ustring("B");
const ustring Labels::VOLUME     = ustring("V");
const ustring Labels::OBJECT     = ustring("O");
const ustring Labels::TRANSMIT   = ustring("T");
const ustring Labels::REFLECT    = ustring("R");
const ustring Labels::DIFFUSE    = ustring("D");
const ustring Labels::GLOSSY     = ustring("G");
const ustring Labels::SINGULAR   = ustring("S");
const ustring Labels::STRAIGHT   = ustring("s");
const ustring Labels::STOP       = ustring("__stop__");

OSL_NAMESPACE_EXIT


