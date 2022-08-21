#pragma once

#include "RDITypeTraits.inl"

template<typename _rdiResourceType>
concept CRDIResource = RDITypeTraitsDelta::CRDIResource<_rdiResourceType>;

template<CRDIResource _rdiResourceType>
using TRDIResourceDescType = RDITypeTraitsDelta::TRDIResourceDescType<_rdiResourceType>::Type;
