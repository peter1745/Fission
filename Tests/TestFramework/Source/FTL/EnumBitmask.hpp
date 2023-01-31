#pragma once

#include <stdint.h>
#include <type_traits>

namespace FTL {

	template<typename TEnum>
	inline constexpr bool IsScopedEnumV = std::conjunction_v<std::is_enum<TEnum>, std::negation<std::is_convertible<TEnum, int>>>;

	template <typename TEnum>
	struct IsScopedEnum : std::bool_constant<IsScopedEnumV<TEnum>> {};

	template<typename TEnum, typename = void>
	struct ScopedEnumBitmask : std::false_type {};

	template<typename TEnum>
	struct ScopedEnumBitmask<TEnum, std::void_t<decltype(TEnum::ScopedBitmaskMark)>> : IsScopedEnum<TEnum> {};
	
	template<typename TEnum>
	inline constexpr bool ScopedEnumBitmaskV = ScopedEnumBitmask<TEnum>::value;

	template<typename TEnum, typename = void>
	struct EnumBitmask : std::false_type {};

	template<typename TEnum>
	struct EnumBitmask<TEnum, std::void_t<decltype(TEnum::BitmaskMark)>> : std::is_enum<TEnum> {};

	template <typename TEnum>
	inline constexpr bool EnumBitmaskV = EnumBitmask<TEnum>::value;

	template<typename TEnum>
	inline constexpr bool EnumBitmaskEnabled = ScopedEnumBitmaskV<TEnum> || EnumBitmaskV<TEnum>;
}

template<typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, TEnum> operator|(const TEnum InLHS, const TEnum InRHS) noexcept
{
	return static_cast<TEnum>(static_cast<std::underlying_type_t<TEnum>>(InLHS) | static_cast<std::underlying_type_t<TEnum>>(InRHS));
}

template <typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, TEnum> operator&(const TEnum InLHS, const TEnum InRHS) noexcept
{
	return static_cast<TEnum>(static_cast<std::underlying_type_t<TEnum>>(InLHS) & static_cast<std::underlying_type_t<TEnum>>(InRHS));
}

template <typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, bool> operator!=(const TEnum InLHS, const std::underlying_type_t<TEnum> InRHS) noexcept
{
	return static_cast<std::underlying_type_t<TEnum>>(InLHS) != InRHS;
}

template <typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, TEnum> operator^(const TEnum InLHS, const TEnum InRHS) noexcept
{
	return static_cast<TEnum>(static_cast<std::underlying_type_t<TEnum>>(InLHS) ^ static_cast<std::underlying_type_t<TEnum>>(InRHS));
}

template <typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, TEnum> operator~(const TEnum InValue) noexcept
{
	return static_cast<TEnum>(~static_cast<std::underlying_type_t<TEnum>>(InValue));
}

template <typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, bool> operator!(const TEnum InValue) noexcept
{
	return !static_cast<std::underlying_type_t<TEnum>>(InValue);
}

template <typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, TEnum&> operator|=(TEnum& InLHS, const TEnum& InRHS) noexcept
{
	return (InLHS = (InLHS | InRHS));
}

template <typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, TEnum&> operator&=(TEnum& InLHS, const TEnum& InRHS) noexcept
{
	return (InLHS = (InLHS & InRHS));
}

template <typename TEnum>
[[nodiscard]] constexpr std::enable_if_t<FTL::EnumBitmaskEnabled<TEnum>, TEnum&> operator^=(TEnum& InLHS, const TEnum& InRHS) noexcept
{
	return (InLHS = (InLHS ^ InRHS));
}
