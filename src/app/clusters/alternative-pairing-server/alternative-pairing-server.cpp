/****************************************************************************
 * @file
 * @brief Routines for the Alternative Pairing plugin, the
 *server implementation of the Alternative Pairing cluster.
 *******************************************************************************
 ******************************************************************************/

#include <app/clusters/alternative-pairing-server/alternative-pairing-delegate.h>
#include <app/clusters/alternative-pairing-server/alternative-pairing-server.h>

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/AttributeAccessInterface.h>
#include <app/AttributeAccessInterfaceRegistry.h>
#include <app/CommandHandler.h>
#include <app/ConcreteCommandPath.h>
#include <app/data-model/Encode.h>
#include <app/util/attribute-storage.h>

using namespace chip;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::AlternativePairing;

static constexpr size_t kAlternativePairingDelegateTableSize =
    MATTER_DM_ALTERNATIVE_PAIRING_CLUSTER_SERVER_ENDPOINT_COUNT + CHIP_DEVICE_CONFIG_DYNAMIC_ENDPOINT_COUNT;
static_assert(kAlternativePairingDelegateTableSize < kEmberInvalidEndpointIndex, "AlternativePairing Delegate table size error");

// -----------------------------------------------------------------------------
// Delegate Implementation

using chip::app::Clusters::AlternativePairing::Delegate;
using chip::Protocols::InteractionModel::Status;

namespace {

Delegate * gDelegateTable[kAlternativePairingDelegateTableSize] = { nullptr };

Delegate * GetDelegate(EndpointId endpoint)
{
    uint16_t ep =
        emberAfGetClusterServerEndpointIndex(endpoint, AlternativePairing::Id, MATTER_DM_ALTERNATIVE_PAIRING_CLUSTER_SERVER_ENDPOINT_COUNT);
    return (ep >= kAlternativePairingDelegateTableSize ? nullptr : gDelegateTable[ep]);
}
} // namespace

namespace chip {
namespace app {
namespace Clusters {
namespace AlternativePairing {

void SetDefaultDelegate(EndpointId endpoint, Delegate * delegate)
{
    uint16_t ep =
        emberAfGetClusterServerEndpointIndex(endpoint, AlternativePairing::Id, MATTER_DM_ALTERNATIVE_PAIRING_CLUSTER_SERVER_ENDPOINT_COUNT);
    if (ep < kAlternativePairingDelegateTableSize)
    {
        gDelegateTable[ep] = delegate;
    }
}
} // namespace AlternativePairing
} // namespace Clusters
} // namespace app
} // namespace chip

// -----------------------------------------------------------------------------
// Attribute Accessor Implementation

namespace {

class AlternativePairingAttrAccess : public app::AttributeAccessInterface
{
public:
    AlternativePairingAttrAccess() : app::AttributeAccessInterface(Optional<EndpointId>::Missing(), chip::app::Clusters::AlternativePairing::Id) {}

    CHIP_ERROR Read(const app::ConcreteReadAttributePath & aPath, app::AttributeValueEncoder & aEncoder) override;

private:
    CHIP_ERROR ReadDevicesCountAttribute(app::AttributeValueEncoder & aEncoder, Delegate * delegate);
};

AlternativePairingAttrAccess gAlternativePairingAttrAccess;

CHIP_ERROR AlternativePairingAttrAccess::Read(const app::ConcreteReadAttributePath & aPath, app::AttributeValueEncoder & aEncoder)
{
    EndpointId endpoint = aPath.mEndpointId;
    Delegate * delegate = GetDelegate(endpoint);

    if (!delegate)
    {
        ChipLogError(Zcl, "AlternativePairingAttrAccess::Read: No delegate found for endpoint %u", endpoint);
        return CHIP_ERROR_INCORRECT_STATE;
    }

    switch (aPath.mAttributeId)
    {
    case app::Clusters::AlternativePairing::Attributes::DevicesCount::Id: {
        return ReadDevicesCountAttribute(aEncoder, delegate);
    }
    default: {
        break;
    }
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR AlternativePairingAttrAccess::ReadDevicesCountAttribute(app::AttributeValueEncoder & aEncoder, Delegate * delegate)
{
    return delegate->HandleGetDevicesCount(aEncoder);
}

} // anonymous namespace

// -----------------------------------------------------------------------------
// Matter Framework Callbacks Implementation

bool emberAfAlternativePairingClusterPairCallback(app::CommandHandler * command, const app::ConcreteCommandPath & commandPath,
                                              const Commands::Pair::DecodableType & commandData)
{

    ChipLogDetail(Zcl, "emberAfAlternativePairingClusterPairCallback: Endpoint %u", commandPath.mEndpointId);

    EndpointId endpoint = commandPath.mEndpointId;
    Status status       = Status::Success;

    auto & nodeId       = commandData.nodeId;

    Delegate * delegate = GetDelegate(endpoint);
    if (!delegate)
    {
        ChipLogError(Zcl, "emberAfAlternativePairingClusterPairCallback: No delegate found for endpoint %u", endpoint);
        status = Status::Failure;
    }

    const auto scopedNodeId = ScopedNodeId(nodeId, command->GetAccessingFabricIndex());

    if (!delegate->HandlePair(scopedNodeId))
    {
        status = Status::Failure;
    }

    command->AddStatus(commandPath, status);
    return true;
}

void MatterAlternativePairingPluginServerInitCallback()
{
    app::AttributeAccessInterfaceRegistry::Instance().Register(&gAlternativePairingAttrAccess);
}

void MatterAlternativePairingPluginServerShutdownCallback()
{
    app::AttributeAccessInterfaceRegistry::Instance().Unregister(&gAlternativePairingAttrAccess);
}
