/****************************************************************************
 * @file
 * @brief Routines for the Alternative Pairing plugin, the
 *server implementation of the Alternative Pairing cluster.
 *******************************************************************************
 ******************************************************************************/

#pragma once

#include "alternative-pairing-delegate.h"
#include <app-common/zap-generated/cluster-objects.h>

namespace chip {
namespace app {
namespace Clusters {
namespace AlternativePairing {

void SetDefaultDelegate(EndpointId endpoint, Delegate * delegate);

} // namespace AlternativePairing
} // namespace Clusters
} // namespace app
} // namespace chip
