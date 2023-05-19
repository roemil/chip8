#include "RegValue.h"

#include <optional>

// bool operator==(const std::optional<RegValue> &lh, const std::optional<RegValue> &rh)
// {
//     if(lh && rh)
//     {
//         return *lh == *rh;
//     }
//     return false;
// }

// inline bool operator==(const RegValue& lh, const RegValue& rh) 
// {
//     return lh.reg == rh.reg && lh.value == rh.value;
// }

// inline bool operator!=(const RegValue& lh, const RegValue& rh) 
// {
//     return !(lh==rh);
// }