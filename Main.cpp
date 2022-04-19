#include "TurtleTrafficController.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <mutex>

void Server() {
	using TrafficController = TurtleManagement::TurtleTrafficController;
	std::filesystem::path sc("./config/server.json"), fc("./config/fields.json"), pa("./config/protection-area.json");
	if (!std::filesystem::exists(sc) || !std::filesystem::exists(fc) || !std::filesystem::exists(pa)) throw std::runtime_error("server or controller config file is not found");
	std::ifstream Server(sc), Field(fc), ProtectionArea(pa);
	TrafficController TrafficCtrl(Field, ProtectionArea, Server);
	TrafficCtrl.Run();
	std::cout << "Terminated" << std::endl;
}

void StopRequest() {
	std::filesystem::path sc("./config/server.json");
	if (!std::filesystem::exists(sc)) throw std::runtime_error("server config file is not found");
	std::ifstream ifs(sc);
	const nlohmann::json json = nlohmann::json::parse(std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()));
	httplib::Client client(json["host"].get<std::string>(), json["port"].get<int>());
	client.Get("/control/stop"); // レスポンスは受け取れないのでそのまま終了
}

int main(int argc, char* argv[]) {
	try {
		if (argc != 2) return 1;
		const std::string ProcessArg(argv[1]);
		if (ProcessArg == "start-server") Server();
		else if (ProcessArg == "stop-server") StopRequest();
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
	}
	return 0;
}
