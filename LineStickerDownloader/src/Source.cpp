#include <fstream>
#include <regex>
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <curl/curl.h>


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static size_t DownloadCallback(void *ptr, size_t size, size_t nmemb, void* userdata)
{
	FILE* stream = (FILE*)userdata;
	return fwrite((FILE*)ptr, size, nmemb, stream);
}

std::vector<std::string> Parse()
{
	std::ifstream file;
	file.open("code.txt", std::ios::in);

	std::string buffer;
	std::vector<std::string> v;
	std::regex reg("(https?:\/\/.*sticker.png)");
	std::smatch match;
	while (std::getline(file, buffer))
	{
		if (std::regex_search(buffer, match, reg))
		{
			v.push_back(match[0]);
		}
	}
	return v;
}


void downloadPage(const char* url)
{
	CURL* curl;
	CURLcode res;
	std::fstream code;
	std::string buffer;

	curl = curl_easy_init();
	code.open("code.txt", std::ios::out | std::ios::in | std::ios::trunc);

	if (!code.is_open())
	{
		std::cout << "Nie mozna otworzyc pliku" << std::endl;

		Sleep(2000);
		_exit(-1);
	}

	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		res = curl_easy_perform(curl);
		code << buffer;
	}
	curl_easy_cleanup(curl);
	code.close();
}

void downloadIMG(std::vector<std::string>& links)
{
	for (auto &v : links)
	{
		std::string temp;
		const char* tmp = v.c_str();
		CURL* curl;
		FILE* img;
		CURLcode res;
		std::regex reg("([[:digit:]]{2,})");
		std::smatch match;
		std::regex_search(v, match, reg);
		temp = match[0];
		temp.append(".png");

		std::cout << v << "   " << temp << std::endl;
		Sleep(500);
		fopen_s(&img, temp.c_str(), "wb");
		if (!img)
		{
			std::cout << "Nie mozna zapisac obrazka" << std::endl;
			_exit(-1);
		}
		

		curl = curl_easy_init();
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, tmp);
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36");
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownloadCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, img);
			res = curl_easy_perform(curl);
		}
		curl_easy_cleanup(curl);
		fclose(img);
	}
}


int main(int argc, char * argv[])
{
	const char* url = argv[1];

	downloadPage(url);
	std::vector<std::string> links = Parse();
	downloadIMG(links);







	//std::fstream f("link.txt", std::ios::out | std::ios::in | std::ios::trunc);
	//f.close();

	return 0;
}