#include "../include/configDefaults.hpp"

configDefaults::configDefaults()
{
	// listen
	_ip_default.push_back("0.0.0.0");
	_port_default.push_back(80);

	_server_name_default.push_back("");
	_client_max_body_size_default.push_back(1000000);
	_root_default.push_back("/www/html");            // These needs to be checked
	_index_default.push_back("/index.html");       	 // These needs to be checked
	_auto_index_default.push_back("off");

	_methods_default.push_back("GET");
	_upload_default.push_back("off");
};

configDefaults::~configDefaults()
{};
