#pragma once

namespace FTL {

	template <typename TSizeType, TSizeType TAlignment>
	constexpr TSizeType AlignSize(TSizeType InSize)
	{
		return (InSize + (TAlignment - TSizeType(1))) / TAlignment * TAlignment;
	}

	template <typename TSizeType>
	constexpr TSizeType AlignSize(TSizeType InSize, TSizeType InAlignment)
	{
		return (InSize + (InAlignment - TSizeType(1))) / InAlignment * InAlignment;
	}

}