#pragma once
#include <Runtime/Core/Core.h>

namespace Dream
{
	enum class RUNTIME_API LogicOperation : unsigned char
	{
		Clear,
		Set,
		Copy,
		CopyInverted,
		NoOp,
		Invert,
		And,
		NotAnd,
		Or,
		Nor,
		Xor,
		Equal,
		AndReverse,
		AndInverted,
		OrReverse,
		OrInverted
	};
}

