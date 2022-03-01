#include <limits>
#include <type_traits>
#include <string_view>

namespace StringParserDetail
{
	template<typename _intType>
	struct TIntegralStringParserHelper
	{
		using IntType = _intType;
		static constexpr bool IsUnsigned = std::is_unsigned_v<IntType>;
		static_assert(std::is_integral_v<IntType>, "_intType must be integral type");

		IntType ParseCharacter(wchar_t _character) noexcept
		{
			constexpr IntType Error = std::numeric_limits<IntType>::max();
			constexpr IntType map[55] = {
				0,      1,        2,     3,     4,     5,     6,     7,     8,     9, Error, Error, Error, Error, Error, Error,	//0x30-0x3f('0' - '?')
				Error, 10,       11,    12,    13,    14,    15, Error, Error, Error, Error, Error, Error, Error, Error, Error,	//0x40-0x4f('@' - 'O')
				Error, Error, Error, Error, Error, Error, Error, Error, Error, Error, Error, Error, Error, Error, Error, Error,	//0x50-0x5f('P' - '_')
				Error, 10,       11,    12,    13,    14,    15																	//0x60-0x6f('`' - 'f')
			};

			if (_character >= L'0' && _character <= L'f') return map[_character - L'0'];
			return Error;
		}

		bool operator()(std::wstring_view _str, IntType& _value) noexcept
		{
			IntType valueSignal = 1;
			if (_str.size() >= 1 && _str[0] == L'-')
			{
				if (IsUnsigned) return false;
				else valueSignal = -1;
				_str.remove_prefix(1);
			}
			else if (_str.size() >= 1 && _str[0] == L'+')
			{
				valueSignal = 1; 
				_str.remove_prefix(1);
			}

			IntType numberSystem = 0;
			if (_str.size() >= 2 && _str[0] == L'0' && (_str[1] == L'X' || _str[1] == L'x'))
			{
				numberSystem = 16;
				_str.remove_prefix(2);
			}
			if (_str.size() >= 2 && _str[0] == L'0' && (_str[1] == L'B' || _str[1] == L'b'))
			{
				numberSystem = 2;
				_str.remove_prefix(2);
			}
			else if (_str.size() >= 1 && _str[0] == L'0')
			{
				numberSystem = 8;
				_str.remove_prefix(1);
			}
			else
			{
				numberSystem = 10;
			}

			_value = 0;
			for (size_t i = 0; i != _str.size(); ++i)
			{
				_value *= numberSystem;
				if (ParseCharacter(_str[i]) == std::numeric_limits<IntType>::max()) return false;
				_value += ParseCharacter(_str[i]) * valueSignal;
			}

			return true;
		}
	};
}
