#if !defined TURTLE_TRAFFIC_CONTROLLER_HPP
#define TURTLE_TRAFFIC_CONTROLLER_HPP
#include "FieldController.hpp"
#include "ProtectionAreaController.hpp"
#include "httplib.h"
#include <iostream>
#include <mutex>

namespace TurtleManagement {
	class TurtleTrafficController {
	private:
		FieldController Field;
		ProtectionAreaController ProtectArea;
		httplib::Server Server;
		nlohmann::json ServerConfig;
	public:
		TurtleTrafficController(std::istream& FieldControllerConfig, std::istream& ProtectionAreaControllerConfig, std::istream& ServerConfig)
			: Field(FieldControllerConfig), 
			ProtectArea(ProtectionAreaControllerConfig), 
			Server(),
			ServerConfig(nlohmann::json::parse(std::string((std::istreambuf_iterator<char>(ServerConfig)), std::istreambuf_iterator<char>()))) {
			this->Server.Post(
				"/fields/([0-9A-Za-z]{1,})/([0-9A-Za-z]{1,})",
				[this](const httplib::Request& req, httplib::Response& res) {
					try {
						this->Field.RunGPSHost(req.matches[1], req.matches[2]);
						res.status = 200;
					}
					catch (const HttpException& hex) {
						res.status = hex.code();
						res.body = hex.what();
					}
					catch (const std::exception& er) {
						res.status = 500;
						std::cout << req.method << " " << req.path << " : " << er.what() << std::endl;
					}
				}
			);
			this->Server.Get(
				"/fields/([0-9A-Za-z]{1,})/departure",
				[this](const httplib::Request& req, httplib::Response& res) {
					try {
						res.status = this->Field.AllowRun(req.matches[1]) ? 200 : 403;
					}
					catch (const HttpException& hex) {
						res.status = hex.code();
						res.body = hex.what();
					}
					catch (const std::exception& er) {
						res.status = 500;
						std::cout << req.method << " " << req.path << " : " << er.what() << std::endl;
					}
				}
			);
			this->Server.Get(
				"/protection_area",
				[this](const httplib::Request& req, httplib::Response& res) {
					try {
						res.status = 200;
						res.set_content(this->ProtectArea.GetAllManagementCoordinate(), "application/json");
					}
					catch (const std::exception& er) {
						res.status = 500;
						std::cout << req.method << " " << req.path << " : " << er.what() << std::endl;
					}
				}
			);
			this->Server.Post(
				"/protection_area/([0-9A-Za-z]{1,})/approach",
				[this](const httplib::Request& req, httplib::Response& res) {
					try {
						res.status = this->ProtectArea.ApproachToProtectionArea(req.matches[1]) ? 200 : 403;
					}
					catch (const HttpException& hex) {
						res.status = hex.code();
						res.body = hex.what();
					}
					catch (const std::exception& er) {
						res.status = 500;
						std::cout << req.method << " " << req.path << " : " << er.what() << std::endl;
					}
				}
			);
			this->Server.Post(
				"/protection_area/([0-9A-Za-z]{1,})/departure",
				[this](const httplib::Request& req, httplib::Response& res) {
					try {
						this->ProtectArea.DepartureFromProtectionArea(req.matches[1]);
						res.status = 200;
					}
					catch (const HttpException& hex) {
						res.status = hex.code();
						res.body = hex.what();
					}
					catch (const std::exception& er) {
						res.status = 500;
						std::cout << req.method << " " << req.path << " : " << er.what() << std::endl;
					}
				}
			);
			this->Server.Get("/control/stop", [this](const httplib::Request&, httplib::Response&) { this->Server.stop(); });
		}
		void Run() {
			this->Server.listen(this->ServerConfig["host"].get<std::string>().c_str(), this->ServerConfig["port"].get<int>());
		}
	};
}
#endif
