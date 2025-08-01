// DO NOT EDIT MANUALLY - Generated file
//
// Cluster metadata information for cluster AlternativePairing (cluster code: 4294179900/0xFFF3FC3C)
// based on src/controller/data_model/controller-clusters.matter
#pragma once

#include <app/data-model-provider/MetadataTypes.h>
#include <lib/core/DataModelTypes.h>

#include <cstdint>

#include <clusters/AlternativePairing/Ids.h>

namespace chip {
namespace app {
namespace Clusters {
namespace AlternativePairing {

inline constexpr uint32_t kRevision = 1;

namespace Attributes {
namespace DevicesCount {
inline constexpr DataModel::AttributeEntry kMetadataEntry(DevicesCount::Id, BitFlags<DataModel::AttributeQualityFlags>(),
                                                          Access::Privilege::kView, std::nullopt);
} // namespace DevicesCount

} // namespace Attributes

namespace Commands {
namespace Pair {
inline constexpr DataModel::AcceptedCommandEntry
    kMetadataEntry(Pair::Id, BitFlags<DataModel::CommandQualityFlags>(DataModel::CommandQualityFlags::kFabricScoped),
                   Access::Privilege::kAdminister);
} // namespace Pair

} // namespace Commands
} // namespace AlternativePairing
} // namespace Clusters
} // namespace app
} // namespace chip
