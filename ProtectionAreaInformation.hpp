#if !defined TURTLE_MANAGEMENT_PROTECTION_AREA_INFORMATION_HPP
#define TURTLE_MANAGEMENT_PROTECTION_AREA_INFORMATION_HPP
#include <string>
#include <unordered_map>

namespace TurtleManagement {
	struct Position {
		int X;
		int Y;
		int Z;
	};

	struct ProtectionArea {
		ProtectionArea(const int& X, const int& Y, const int& Z)
			: pos({X, Y, Z}), AllowApproach(true) {}
		Position pos;
		bool AllowApproach;
	};

	using ProtectionAreaInformation = std::unordered_map<std::string, ProtectionArea>;
}
#endif
