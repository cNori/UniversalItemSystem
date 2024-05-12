#include "../Item.h"
#include <Engine/Serialization/Serialization.h>
#include <Engine\Networking\NetworkStream.h>
#include <Engine\Debug\DebugLog.h>

Item::Item(const SpawnParams& params)
    : Script(params)
{
    // Enable ticking OnUpdate function
    _tickUpdate = false;
    ItemData = {};
}
