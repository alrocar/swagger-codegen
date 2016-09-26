/**
 * Swagger Petstore
 * This is a sample server Petstore server.  You can find out more about Swagger at [http://swagger.io](http://swagger.io) or on [irc.freenode.net, #swagger](http://swagger.io/irc/).  For this sample, you can use the api key `special-key` to test the authorization filters.
 *
 * OpenAPI spec version: 1.0.0
 * Contact: apiteam@swagger.io
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ApiClient.h"
#include "MultipartFormData.h"
#include "ModelBase.h"

namespace io {
namespace swagger {
namespace client {
namespace api {

using namespace io::swagger::client::model;

ApiClient::ApiClient(std::shared_ptr<ApiConfiguration> configuration )
    : m_Configuration(configuration)
{
}
ApiClient::~ApiClient()
{
}

std::shared_ptr<ApiConfiguration> ApiClient::getConfiguration() const
{
    return m_Configuration;
}
void ApiClient::setConfiguration(std::shared_ptr<ApiConfiguration> configuration)
{
    m_Configuration = configuration;
}


utility::string_t ApiClient::parameterToString(utility::string_t value)
{
    return value;
}
utility::string_t ApiClient::parameterToString(int64_t value)
{
    return utility::conversions::to_string_t(std::to_string(value));
}
utility::string_t ApiClient::parameterToString(int32_t value)
{
    return utility::conversions::to_string_t(std::to_string(value));
}

pplx::task<web::http::http_response> ApiClient::callApi( 
    const utility::string_t& path, 
    const utility::string_t& method, 
    const std::map<utility::string_t, utility::string_t>& queryParams, 
    const std::shared_ptr<IHttpBody> postBody,
    const std::map<utility::string_t, utility::string_t>& headerParams, 
    const std::map<utility::string_t, utility::string_t>& formParams, 
    const std::map<utility::string_t, std::shared_ptr<HttpContent>>& fileParams,
    const utility::string_t& contentType
) const
{
	if (postBody != nullptr && formParams.size() != 0)
	{
		throw ApiException(400, U("Cannot have body and form params"));
	}

	if (postBody != nullptr && fileParams.size() != 0)
	{
		throw ApiException(400, U("Cannot have body and file params"));
	}

	if (fileParams.size() > 0 && contentType != U("multipart/form-data"))
	{
		throw ApiException(400, U("Operations with file parameters must be called with multipart/form-data"));
	}

	web::http::client::http_client client(m_Configuration->getBaseUrl(), m_Configuration->getHttpConfig());

	web::http::http_request request;
	for ( auto& kvp : headerParams )
	{
		request.headers().add(kvp.first, kvp.second);
	}

	if (fileParams.size() > 0)
	{
		MultipartFormData uploadData;
		for (auto& kvp : formParams)
		{
			uploadData.add(ModelBase::toHttpContent(kvp.first, kvp.second));
		}
		for (auto& kvp : fileParams)
		{
			uploadData.add(ModelBase::toHttpContent(kvp.first, kvp.second));
		}
		std::stringstream data;
		postBody->writeTo(data);
		auto bodyString = data.str();
		auto length = bodyString.size();
		request.set_body(concurrency::streams::bytestream::open_istream(std::move(bodyString)), length, contentType);
	}
	else
	{
		if (postBody != nullptr)
		{
			std::stringstream data;
			postBody->writeTo(data);
			auto bodyString = data.str();
			auto length = bodyString.size();
			request.set_body(concurrency::streams::bytestream::open_istream(std::move(bodyString)), length, contentType);
		}
		else
		{
			web::http::uri_builder formData;
			for (auto& kvp : formParams)
			{
				formData.append_query(kvp.first, kvp.second);
			}
			request.set_body(formData.query(), U("application/x-www-form-urlencoded"));
		}
	}

	web::http::uri_builder builder(path);
	for (auto& kvp : queryParams)
	{
		builder.append_query(kvp.first, kvp.second);
	}
	request.set_request_uri(builder.to_uri());
	request.set_method(method);
	if ( !request.headers().has( web::http::header_names::user_agent ) )
	{
		request.headers().add( web::http::header_names::user_agent, m_Configuration->getUserAgent() );
	}	

	return client.request(request);
}

}
}
}
}
