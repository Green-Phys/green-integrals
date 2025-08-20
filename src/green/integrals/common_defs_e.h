/*
 * Copyright (c) 2024 University of Michigan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the “Software”), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef INTEGRALS_COMMON_DEFS_E_H
#define INTEGRALS_COMMON_DEFS_E_H
#include<string>
#include<cstdio>

namespace green::integrals{

  static const std::string INPUT_VERSION = "0.2.4";

  enum integral_symmetry_type_e {
    direct, conjugated, transposed
  };

  enum integral_reading_type {
    chunks, as_a_whole
  };

  /**
   * @brief Compare two version strings
   * 
   * @param v (std::string)
   * @return true if v >= INPUT_VERSION
   * @return false otherwise
   */
  bool CheckVersion(const std::string& v) {
    int major_Vin, minor_Vin, patch_Vin;
    int major_Vref, minor_Vref, patch_Vref;
  
    char suffixV[32] = "";
    char suffixM[32] = "";
  
    std::sscanf(v.c_str(), "%d.%d.%d%31s", &major_Vin, &minor_Vin, &patch_Vin, suffixV);
    std::sscanf(INPUT_VERSION.c_str(), "%d.%d.%d%31s", &major_Vref, &minor_Vref, &patch_Vref, suffixM);
  
    if (major_Vin != major_Vref) return major_Vin > major_Vref;
    if (minor_Vin != minor_Vref) return minor_Vin > minor_Vref;
    if (patch_Vin != patch_Vref) return patch_Vin > patch_Vref;
  
    // If numeric parts in version are all equal, do not worry about suffix
    // e.g., 0.2.4b10 has same integral format as 0.2.4
    return true;
  }
}
#endif //INTEGRALS_COMMON_DEFS_E_H
