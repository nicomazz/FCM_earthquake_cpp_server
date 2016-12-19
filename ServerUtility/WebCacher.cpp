
#include "WebCacher.hpp"

WebCacher::WebCacher(std::function<std::string()> & generator_f, 
	int ttl_millis):ttl(ttl_millis),generator(generator_f){
	last_update = 0;
}

std::string WebCacher::getResponse(){
    std::lock_guard<std::mutex> guard(response_mutex);

	int millis = TimeUtility::getCurrentMillis();
	if (millis - last_update > ttl){
		response = generator();
		last_update = TimeUtility::getCurrentMillis();
	}
	return response;
}