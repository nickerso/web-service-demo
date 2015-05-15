#ifndef API_HPP
#define API_HPP

class API
{
public:
    API();
    ~API();
    std::string executeAPI(const std::string& url, const std::map<std::string, std::string>& argvals);

private:
    void getInvalidResponse(std::string& response);

};
#endif
