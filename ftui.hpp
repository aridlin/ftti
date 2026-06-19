// Compatibility shim for early FTTI ports.
// New code should include "ftti.hpp" and use namespace ftti.

#pragma once

#ifdef FTUI_IMPLEMENTATION
#ifndef FTTI_IMPLEMENTATION
#define FTTI_IMPLEMENTATION
#endif
#endif

#include "ftti.hpp"

namespace ftui = ftti;
