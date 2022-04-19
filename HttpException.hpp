#if !defined HTTP_EXCEPTION_HPP
#define HTTP_EXCEPTION_HPP
#include <string>

class HttpException {
private:
	unsigned int ResponseCode;
	std::string Message;
public:
	HttpException(const unsigned int& ResCode, const std::string& Msg = "")
		: ResponseCode(ResCode), Message(Msg) {}
	const unsigned int& code() const noexcept { return this->ResponseCode; }
	const std::string& what() const noexcept { return this->Message; }
};
#endif
