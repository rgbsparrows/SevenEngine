#pragma once

struct SRDIOutputDesc;

__interface IRDIOutput
{
	void GetDesc(SRDIOutputDesc* _desc) noexcept;
};
