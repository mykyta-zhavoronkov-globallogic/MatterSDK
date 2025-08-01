#pragma once

#include <app-common/zap-generated/cluster-objects.h>
#include <app/AttributeValueEncoder.h>
#include <lib/core/ScopedNodeId.h>

namespace chip {
namespace app {
namespace Clusters {
namespace AlternativePairing {

class Delegate
{
public:
    virtual CHIP_ERROR HandleGetDevicesCount(app::AttributeValueEncoder & aEncoder) = 0;
    virtual bool HandlePair(const ScopedNodeId nodeId) = 0;

    virtual ~Delegate() = default;
};

} // namespace AlternativePairing
} // namespace Clusters
} // namespace app
} // namespace chip
