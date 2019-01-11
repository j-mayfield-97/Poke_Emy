#pragma once


#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <cstdio>
#include <iostream>
#include <fstream> 

//Tutorials from rapidjson.com to test the necessary modules

void jsonT() {
	//way to init json
	const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
	//Document seems to handle most of the useful json fucntions
	rapidjson::Document d;

	//d will contain the json pasrsed
	d.Parse(json);

	//access member then set it
	rapidjson::Value& s = d["stars"];
	s.SetInt(s.GetInt() + 1);

	

	// Output to terminal {"project":"rapidjson","stars":11}
	

	//reading json stuff from files
	// If the file is not UTF-8, the byte stream can be wrapped in a EncodedInputStream
	/*
	FILE* fp = fopen("SaveData/big.json", "rb"); // non-Windows use "r"
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document doc;
	doc.ParseStream(is);
	fclose(fp);*/

	std::ifstream ifs("SaveData/big.json");
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document dread;
	dread.ParseStream(isw);
	ifs.close();

	//stringifying
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	dread.Accept(writer);
	std::cout << buffer.GetString() << std::endl;
	//std::cout << dread.GetString() << std::endl;


	//writing
	/*
	FILE* fpw = fopen("SaveData/output.json", "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fpw, writeBuffer, sizeof(writeBuffer));
	rapidjson::Writer<rapidjson::FileWriteStream> writerw(os);
	d.Accept(writerw);
	fclose(fp); */
	std::ofstream ofs("SaveData/output.json");
	rapidjson::OStreamWrapper osw(ofs);
	rapidjson::Writer<rapidjson::OStreamWrapper> writerwr(osw);
	d.Accept(writerwr);
	ofs.close();
}