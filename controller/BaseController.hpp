#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "randoms/Normal.h"

using namespace web;
using namespace http;
using namespace http::experimental::listener;
using namespace std;

class BaseController
{
public:
    BaseController() {}
    ~BaseController() {}

    void initHandlers()
    {
        listener.support(methods::GET, std::bind(&BaseController::handleGet, this, std::placeholders::_1));
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

        std::istringstream ss(message.request_uri().to_string());
        std::string token;

        std::vector<std::string> get_params;
        while (std::getline(ss, token, '/'))
        {
            if (token.size() > 0)
            {
                get_params.push_back(token);
            }
        }
        if (get_params.size() > 0)
        {
            if (get_params[0] == "uniform")
            {
                if (get_params.size() == 4)
                {
                    response["distribution"] = json::value::string("uniform");
                    response["left"] = json::value::string(get_params[1]);
                    response["right"] = json::value::string(get_params[2]);
                    response["count"] = json::value::string(get_params[3]);

                    Randoms::Uniform uniform_rng;
                    auto [count, res] = uniform_rng.get_uniform(stod(get_params[1]), stod(get_params[2]), stod(get_params[3]));
                    for (uint64_t i = 0; i < count; i++) {
                        response["result"][i] = json::value::string(std::to_string(res[i]));
                    }
                    message.reply(status_codes::OK, response);
                }
                else
                {
                    response["error"] = json::value::string("wrong req params fro uniform dist");
                }
            }
            else
            {
                response["error"] = json::value::string("non-uniform requested");
            }
        }
        message.reply(status_codes::UnprocessableEntity, response);
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

protected:
    http_listener listener;
};
