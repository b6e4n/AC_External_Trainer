#pragma once

#include <vector>
#include "stdafx.h"

namespace mem {

	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hPRocess);

}
