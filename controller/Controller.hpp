#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "randoms/Normal.h"
#include "randoms/Uniform.h"

using namespace web;
using namespace http;
using namespace http::experimental::listener;
using namespace std;

class Controller
{
public:
    Controller() {}
    ~Controller() {}

    void initHandlers()
    {
        listener.support(methods::GET, std::bind(&Controller::handleGet, this, std::placeholders::_1));
    }

    pplx::task<void> open()
    {
        initHandlers();
        return listener.open();
    }
    pplx::task<void> close() { return listener.close(); }

    void handleGet(http_request message)
    {
        auto response = json::value::object();
        std::vector<std::string> request_params;
        parse_get_request(message, request_params);

        if (is_get_uniform(request_params))
        {
            return handleUniformGet(message, request_params, response);
        }
        else if (is_get_normal(request_params))
        {
            return handleNormalGet(message, request_params, response);
        }

        response["error"] = json::value::string("unsupported");
        message.reply(status_codes::UnprocessableEntity, response);
        return;
    }

    void setEndPoint(const std::string &value)
    {
        uri endpointURI(value);
        uri_builder endpointBuilder;

        endpointBuilder.set_scheme(endpointURI.scheme());
        endpointBuilder.set_host(endpointURI.host());
        endpointBuilder.set_port(endpointURI.port());
        endpointBuilder.set_path(endpointURI.path());

        listener = http_listener(endpointBuilder.to_uri());
    }

private:
    http_listener listener;

    http::status_code set_uniform_response(web::json::value &response, const std::vector<std::string> &request_params)
    {
        Randoms::Uniform uniform_rng;
        try
        {
            auto [count, res] = uniform_rng.get_uniform(stod(request_params[1]), stod(request_params[2]), stod(request_params[3]));
            if (count == 0)
            {
                return status_codes::UnprocessableEntity;
            }
            else
            {
                for (uint64_t i = 0; i < count; i++)
                {
                    response["result"][i] = json::value::string(std::to_string(res[i]));
                }
            }
            return status_codes::OK;
        }
        catch (...)
        {
            return status_codes::UnprocessableEntity;
        }
    }

    http::status_code set_normal_response(web::json::value &response, const std::vector<std::string> &request_params)
    {
        Randoms::Normal normal_rng;
        try
        {
            auto [count, res] = normal_rng.get_normal(stod(request_params[1]), stod(request_params[2]), stod(request_params[3]));
            if (count == 0)
            {
                return status_codes::UnprocessableEntity;
            }
            else
            {
                for (uint64_t i = 0; i < count; i++)
                {
                    response["result"][i] = json::value::string(std::to_string(res[i]));
                }
            }
            return status_codes::OK;
        }
        catch (...)
        {
            return status_codes::UnprocessableEntity;
        }
    }

    void parse_get_request(const http_request &message, std::vector<std::string> &request_params)
    {
        std::istringstream ss(message.request_uri().to_string());
        std::string token;
        while (std::getline(ss, token, '/'))
        {
            if (token.size() > 0)
            {
                request_params.push_back(token);
            }
        }
    }

    bool is_get_uniform(const std::vector<std::string> &request_params)
    {
        if (request_params.size() > 0)
        {
            if (request_params[0] == "uniform")
            {
                return true;
            }
        }
        return false;
    }

    bool is_get_normal(const std::vector<std::string> &request_params)
    {
        if (request_params.size() > 0)
        {
            if (request_params[0] == "normal")
            {
                return true;
            }
        }
        return false;
    }

    void handleUniformGet(const http_request &message, std::vector<std::string> &request_params, web::json::value &response)
    {
        http::status_code code = set_uniform_response(response, request_params);
        if (code == status_codes::OK)
        {
            message.reply(code, response);
            return;
        }
        else
        {
            response["error"] = json::value::string("wrong req params for uniform dist");
            message.reply(code, response);
            return;
        }
    }
    void handleNormalGet(const http_request &message, std::vector<std::string> &request_params, web::json::value &response)
    {
        http::status_code code = set_normal_response(response, request_params);
        if (code == status_codes::OK)
        {
            message.reply(code, response);
            return;
        }
        else
        {
            response["error"] = json::value::string("wrong req params for normal dist");
            message.reply(code, response);
            return;
        }
    }
};
