#include "ApiController.hpp"

namespace Core::Tools
{

void ApiController::setApi(const Api& api)
{
    const bool keyChanged  = api.apiKey != m_api.apiKey || api.secretKey != m_api.secretKey;

    if (!keyChanged)
        return;

    m_api = api;

    if (keyChanged)
        emit apiChanged();
}

void ApiController::setInfo(const ApiInfo& info)
{
    if (m_info == info)
        return;

    m_info = info;
    emit infoChanged();
}

void ApiController::clear()
{
    setApi(Api{});
    setInfo(ApiInfo{});
}

}
