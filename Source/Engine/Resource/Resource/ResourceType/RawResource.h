#pragma once

#include "Resource/ResourceBase.h"

class SRawResource : public SResourceBase
{
	DECLARE_CLASSOBJECT_BODY(SRawResource, SResourceBase)

public:
	virtual void Serialize(SWriteStream& _writeStream)const noexcept
	{
		_writeStream.WriteBuffer(mContent);
	}

	virtual bool Deserialize(SReadStream& _readStream)
	{
		mContent = SBlob(_readStream.GetContentSize());
		_readStream.ReadBuffer(mContent.GetBuffer(), mContent.GetBufferSize());
		return true;
	}

private:
	SBlob mContent;
};

using SRawRP = TResourceProxy<SRawResource>;
