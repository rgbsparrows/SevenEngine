#pragma once

#include <vector>

struct SRDIAdapterDesc;

__interface IRDIOutput;

__interface IRDIAdapter
{
	void GetDesc(SRDIAdapterDesc* _desc) const noexcept;
	const std::vector<IRDIOutput*>& GetOutputs() const noexcept;
};
