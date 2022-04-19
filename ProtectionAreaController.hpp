#if !defined TURTLE_MANAGEMENT_PROTECTION_AREA_CONTROLLER_HPP
#define TURTLE_MANAGEMENT_PROTECTION_AREA_CONTROLLER_HPP
#include "GetIterator.hpp"
#include "ProtectionAreaInformation.hpp"
#include <nlohmann/json.hpp>
#include <istream>
#include <sstream>

namespace TurtleManagement {
	class ProtectionAreaController {
	private:
		ProtectionAreaInformation ProtectAreaInfo;
		std::string ProtectAreaInfoJson;
	public:
		ProtectionAreaController(std::istream& is) 
			: ProtectAreaInfo(), ProtectAreaInfoJson((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>()) {
			const nlohmann::json json = nlohmann::json::parse(this->ProtectAreaInfoJson);
			for (const auto& area : json["protectionarea"]) {
				this->ProtectAreaInfo.emplace(
					area["id"].get<std::string>(),
					ProtectionArea(area["pos"]["x"].get<int>(), area["pos"]["y"].get<int>(), area["pos"]["z"].get<int>())
				);
			}
		}
		bool ApproachToProtectionArea(const std::string& ProtectionAreaID) {
			auto it = GetIterator(this->ProtectAreaInfo, ProtectionAreaID);
			if (!it->second.AllowApproach) return false;
			it->second.AllowApproach = false;
			return true;
		}
		void DepartureFromProtectionArea(const std::string& ProtectionAreaID) {
			GetIterator(this->ProtectAreaInfo, ProtectionAreaID)->second.AllowApproach = true;
		}
		std::string GetAllManagementCoordinate() const { return this->ProtectAreaInfoJson; }
	};
}
#endif
