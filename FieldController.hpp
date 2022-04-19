#if !defined TURTLE_MANAGEMENT_FIELD_CONTROLLER_HPP
#define TURTLE_MANAGEMENT_FIELD_CONTROLLER_HPP
#include "GetIterator.hpp"
#include "GPSInformation.hpp"
#include "HttpException.hpp"
#include <nlohmann/json.hpp>
#include <istream>

namespace TurtleManagement {
	class FieldController {
	private:
		std::unordered_map<std::string, GPSInformation> Fields;
	public:
		FieldController(std::istream& is) : Fields() {
			const nlohmann::json json = nlohmann::json::parse(std::string((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>()));
			for (const auto& fields : json["fields"]) {
				GPSInformation gps{};
				for (const auto& i : fields["hosts"]) gps.emplace(i.get<std::string>(), false);
				this->Fields.emplace(fields["id"].get<std::string>(), gps);
			}
		}
		void RunGPSHost(const std::string& FieldID, const std::string& GPSHostID) {
			GetIterator(GetIterator(this->Fields, FieldID)->second, GPSHostID)->second = true;
		}
		bool AllowRun(const std::string& FieldID) const {
			const GPSInformation gps = GetIterator(this->Fields, FieldID)->second;
			return std::find_if(gps.begin(), gps.end(), [](const std::pair<std::string, bool>& p) { return !p.second; }) == gps.end();
		}
	};
}
#endif
